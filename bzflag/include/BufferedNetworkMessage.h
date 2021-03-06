/* bzflag
 * Copyright (c) 1993 - 2009 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef _BUFFERED_NETWORK_MESSAGE_H_
#define _BUFFERED_NETWORK_MESSAGE_H_

#include "common.h"

// System headers
#include <list>
#include <deque>
#include <string>

// Common headers
#include "Singleton.h"
#include "vectors.h"

class NetHandler;
class BufferedNetworkMessageManager;


class BufferedNetworkMessage
{
public:
  ~BufferedNetworkMessage();

  // sending
  void send(NetHandler *to, uint16_t messageCode);
  void broadcast(uint16_t messageCode, bool toAdminClients = true);

  void packUInt8(uint8_t);
  void packInt16(int16_t);
  void packInt32(int32_t);
  void packInt64(int64_t);
  void packUInt16(uint16_t);
  void packUInt32(uint32_t);
  void packUInt64(uint64_t);
  void packFloat(float);
  void packDouble(double);
  void packFVec2(const fvec2&);
  void packFVec3(const fvec3&);
  void packFVec4(const fvec4&);
  void packString(const char* data, int len);
  void packStdString(const std::string& str);

  uint8_t  unpackUInt8(void);
  int16_t  unpackInt16(void);
  int32_t  unpackInt32(void);
  int64_t  unpackInt64(void);
  uint16_t unpackUInt16(void);
  uint32_t unpackUInt32(void);
  uint64_t unpackUInt64(void);
  float    unpackFloat(void);
  double   unpackDouble(void);
  fvec2&   unpackFVec2(fvec2&);
  fvec3&   unpackFVec3(fvec3&);
  fvec4&   unpackFVec4(fvec4&);
  const std::string& unpackStdString(std::string& str);

  void clear(void);
  void reset(void);

  size_t size(void);
  char * buffer(void) {
    return data+4;
  }
  void addPackedData(const char* d, size_t s);

  void setCode(uint16_t c) {
    code = c;
  }
  uint16_t getCode(void) {
    return code;
  }

protected:
  friend class BufferedNetworkMessageManager;

  // BufferedNetworkMessages should never be created directly.
  // Use MSGMGR->newMessage instead.
  BufferedNetworkMessage();
  BufferedNetworkMessage(const BufferedNetworkMessage &msg);

  bool process(void);

  char* getWriteBuffer(void);
  char* getReadBuffer(void);

  void checkData(size_t s);

  char *data;
  size_t dataSize;
  size_t packedSize;
  size_t readPoint;

  uint16_t    code;
  NetHandler  *recipient;  // NULL if broadcast;
  bool	      toAdmins;
};

class NetworkMessageTransferCallback
{
public:
  virtual ~NetworkMessageTransferCallback() {};

  virtual size_t send(NetHandler* /*handler*/, void * /*data*/, size_t /*size*/) {
    return 0;
  }
  virtual size_t broadcast(void * /*data*/, size_t /*size*/, int/* mask*/, int /*code*/) {
    return 0;
  }
  virtual size_t receive(BufferedNetworkMessage * /*message*/) {
    return 0;
  }
};

class BufferedNetworkMessageManager : public Singleton<BufferedNetworkMessageManager>
{
public:
  BufferedNetworkMessage  *newMessage(BufferedNetworkMessage *msgToCopy = NULL);
  template <class T> T* newMessage(T* msgToCopy = NULL);

  typedef std::list<BufferedNetworkMessage*> MessageList;

  size_t receiveMessages(NetworkMessageTransferCallback *callback,  MessageList &incomingMessages);

  void update(void);

  void sendPendingMessages(void);
  void clearDeadIncomingMessages(void);

  void purgeMessages(NetHandler *handler);
  void flushMessages(NetHandler *handler);

  void setTransferCallback(NetworkMessageTransferCallback *cb) {
    transferCallback = cb;
  }
  NetworkMessageTransferCallback* getTransferCallback(void) {
    return transferCallback;
  }

  void queueMessage(BufferedNetworkMessage *msg);

protected:
  friend class Singleton<BufferedNetworkMessageManager>;

  MessageList pendingOutgoingMessages;

  typedef std::deque<BufferedNetworkMessage*> MessageDeque;
  MessageDeque outgoingQueue;

  NetworkMessageTransferCallback *transferCallback;
private:
  BufferedNetworkMessageManager();
  ~BufferedNetworkMessageManager();
};

template <class T>
inline T* BufferedNetworkMessageManager::newMessage(T* msgToCopy)
{
  T* msg = NULL;
  if (msgToCopy)
    msg = new T(*msgToCopy);
  else
    msg = new T;
  pendingOutgoingMessages.push_back(msg);
  return msg;
}

inline BufferedNetworkMessage* BufferedNetworkMessageManager::newMessage(BufferedNetworkMessage* msgToCopy)
{
  return newMessage<BufferedNetworkMessage>(msgToCopy);
}

#define MSGMGR (BufferedNetworkMessageManager::instance())

typedef BufferedNetworkMessage* NetMsg;

#endif //_BUFFERED_NETWORK_MESSAGE_H_

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
