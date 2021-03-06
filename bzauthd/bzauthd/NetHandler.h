/* bzflag
* Copyright (c) 1993 - 2008 Tim Riker
*
* This package is free software;  you can redistribute it and/or
* modify it under the terms of the license found in the file
* named COPYING that should have accompanied this file.
*
* THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef __BZAUTHD_NETHANDLER_H__
#define __BZAUTHD_NETHANDLER_H__

#include "Platform.h"
#include "Singleton.h"
#include <string.h>
#include <string>
#include <map>

#include "../tcp-net/include/net.h"

#define MAX_PACKET_SIZE 4096

enum Opcodes
{
  MSG_HANDSHAKE             = 0,
  CMSG_AUTH_REQUEST         = 1,
  DMSG_AUTH_FAIL            = 2,
  DMSG_AUTH_CHALLENGE       = 3,
  CMSG_AUTH_RESPONSE        = 4,
  DMSG_AUTH_SUCCESS         = 5,
  CMSG_REGISTER_GET_FORM    = 6,
  DMSG_REGISTER_FAIL        = 7,
  DMSG_REGISTER_SEND_FORM   = 8,
  CMSG_REGISTER_REQUEST     = 9,
  DMSG_REGISTER_CHALLENGE   = 10,
  CMSG_REGISTER_RESPONSE    = 11,
  DMSG_REGISTER_SUCCESS     = 12,
  SMSG_TOKEN_VALIDATE       = 13,
  DMSG_TOKEN_VALIDATE       = 14,
  NUM_OPCODES
};

enum SessionTypes
{
  SESSION_INIT = 0,
  SESSION_AUTH = 1,
  SESSION_REG = 2,
  SESSION_TOKEN = 3,
  NUM_SESSION_TYPES
};

enum PeerType
{
  PEER_ANY = 0,
  PEER_CLIENT = 1,
  PEER_SERVER = 2,
  PEER_DAEMON = 3,
  NUM_PEER_TYPES
};

enum AuthErrors
{
  AUTH_INVALID_MESSAGE = 0,
  AUTH_INCORRECT_CREDENTIALS = 1
};

enum RegErrors
{
  REG_INVALID_MESSAGE = 0
};

class Packet;

class Peer
{
public:
private:
};

class Server : public Peer
{
public:
};

class Client : public Peer
{
public:
};

class Daemon : public Peer
{
public:
};


class Session
{
public:
};

class AuthSession : public Session
{
public:
};

class RegisterSession : public Session
{
public:
};

class ServerSession : public Session
{
public:
};

class ConnectSocket;

class PacketHandlerBase
{
public:
  PacketHandlerBase(ConnectSocket *socket) : m_socket(socket) {}
  virtual ~PacketHandlerBase() {}
protected:
  ConnectSocket *m_socket;
};

class PacketHandler : public PacketHandlerBase
{
public:
  PacketHandler(ConnectSocket *socket) 
    : PacketHandlerBase(socket), m_peer(NULL), m_authSession(NULL), m_regSession(NULL) {}
  ~PacketHandler() { delete m_peer; delete m_authSession; delete m_regSession; }

  static PacketHandler* handleHandshake(Packet &packet, ConnectSocket *socket);

  bool handleNull(Packet &packet);
  bool handleInvalid(Packet &packet);
  bool handleAuthRequest(Packet &packet);
  bool handleRegisterGetForm(Packet &packet);
  bool handleRegisterRequest(Packet &packet);
  bool handleRegisterResponse(Packet &packet);
  bool handleAuthResponse(Packet &packet);
  bool handleTokenValidate(Packet &packet);
private:
  Peer *m_peer;
  AuthSession *m_authSession;
  RegisterSession *m_regSession;
};

typedef bool (PacketHandler::*PHFunc)(Packet &packet);

struct OpcodeEntry
{
  const char * name;
  PHFunc handler;
};

extern OpcodeEntry opcodeTable[NUM_OPCODES];

class Packet
{
public:
  Packet(uint16 opcode, uint8 *data, size_t size) { init(data, size, opcode); }
  Packet(uint16 opcode, size_t size = 1024) { init(size, opcode); }
  Packet(Packet & packet) { init(packet.m_data, packet.m_size, packet.m_opcode); }

  ~Packet() { free(m_data); }

  template < class T >
  Packet& operator >> (T &x)
  {
    if(m_rpoz + sizeof(T) <= m_size)
    {
      x = *(T *)(m_data + m_rpoz);
      m_rpoz += sizeof(T);
    }
    else
      m_rpoz = m_size + 1;

    return (*this);
  }

  template < class T >
  Packet &operator << (const T &x)
  {
    append((const uint8*)&x, sizeof(T));
    return (*this);
  }

  Packet &operator << (const uint8 * &x)
  {
    // get string length and protect against overflow
    size_t len = 0;
    // TODO: replace with a constant
    while(len < 4096) if(!x[len++]) break;
    
    append(x, len);
    return (*this);
  }

  template <>
  Packet &operator << (const std::string &x)
  {
    append((const uint8*)x.c_str(), x.size());
    return (*this);
  }

  void append(const uint8 *x, size_t size)
  {
    if(m_wpoz + size >= m_size)
    {
      m_data = (uint8*)realloc((void*)m_data, 2*m_size);
      m_size *= 2;
    }

    memcpy(m_data + m_wpoz, x, size);
    m_wpoz += size;
  }

  bool read(uint8 *x, size_t size)
  {
    if(m_rpoz + size > m_size)
    {
      m_rpoz = m_size + 1;
      return false;
    }

    memcpy(x, m_data + m_rpoz, size);
    m_rpoz += size;
    return true;
  }

  bool read_string(uint8 *x, size_t buf_size)
  {
    for(size_t i = m_rpoz; i < min(m_rpoz + buf_size, m_size); i++)
    {
      x[i] = m_data[i];
      if(m_data[i] == '\0')
      {
        m_rpoz = i + 1;
        return true;
      }
    }

    m_rpoz = m_size + 1;
    return false;
  }


  operator bool() const { return m_rpoz <= m_size; }

  size_t getLength() const { return m_wpoz; }
  uint16 getOpcode() const { return m_opcode; }
  const uint8 * getData() const { return (const uint8 *)m_data; }
  const char *getOpcodeName() const { return opcodeTable[getOpcode()].name; }

protected:
  void init(size_t size, uint16 opcode)
  {
    m_data = (uint8*)malloc(size);
    m_size = size;
    m_rpoz = 0;
    m_wpoz = 0;
    m_opcode = opcode;
  }

  void init(uint8 *data, size_t size, uint16 opcode)
  {
    init(size, opcode);
    memcpy(m_data, data, size);
    m_wpoz = size;
  }

  uint16 m_opcode;
  uint8 *m_data;
  size_t m_size;
  size_t m_rpoz;
  size_t m_wpoz;
};

typedef enum
{
  eTCPNoError = 0,
  eTCPNotInit,
  eTCPTimeout,
  eTCPBadAddress,
  eTCPBadPort,
  eTCPConnectionFailed,
  eTCPSocketNFG,
  eTCPInitFailed,
  eTCPSelectFailed,
  eTCPDataNFG,
  eTCPUnknownError
}teTCPError;

class Socket
{
public:
  Socket(const TCPsocket &s) : socket(s) {}
  Socket() : socket(NULL) {}
  virtual ~Socket() {}
  uint16 getPort() const { return serverIP.port; }
  virtual void disconnect() = 0;
  TCPsocket &getSocket() { return socket; }
protected:
  IPaddress serverIP;
  TCPsocket socket;
};

class ConnectSocket : public Socket
{
public:
  ConnectSocket(const TCPsocket &s, bool isConn);
  Packet * readData();
  teTCPError sendData(Packet &packet);
  void initRead();
  void disconnect();

  bool isConnected() { return connected; }
private:
  uint8 buffer[MAX_PACKET_SIZE];
  uint16 poz;
  uint16 remainingHeader;
  uint16 remainingData;
  bool connected;
};

class ListenSocket : public Socket
{
public:
  ListenSocket() : socketSet(NULL) {}
  ~ListenSocket() { disconnect(); }
  teTCPError listen(uint16 port, uint32 connections);
  bool update();
  void disconnect();
  
  bool onConnect(TCPsocket &socket);
  void onReadData(ConnectSocket *socket, PacketHandlerBase *&handler, Packet *packet);
  void onDisconnect(ConnectSocket *socket);

  uint32 getMaxConnections () const { return maxUsers; }
private:
  net_SocketSet socketSet;
  uint32 maxUsers;
  typedef std::map<ConnectSocket *, PacketHandlerBase *> SocketMapType;
  SocketMapType socketMap;
};

class NetHandler : public Singleton<NetHandler>
{
public:
  NetHandler();
  ~NetHandler();
  bool initialize();
  void update();
private:
  ListenSocket *localServer;
};

#define sNetHandler NetHandler::instance()

#endif // __BZAUTHD_NETHANDLER_H__

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8