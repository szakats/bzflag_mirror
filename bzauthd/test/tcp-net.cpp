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

#include "common.h"

#ifdef TEST_NET

#include "../bzauthd/NetHandler.h"
#include <TCPConnection.h>

int sleep_var;

void sleep_thread(void *)
{
    sleep_var = 0;
    while(1)
    {
        if(sleep_var != 0)
        {
            _sleep(sleep_var);
            sleep_var = 0;
        }
        _sleep(1);
    }
}

class MyClientListener : public TCPClientDataPendingListener
{
    public:
        void pending ( TCPClientConnection *connection, int count )
        {
            char buf[2048];
            tvPacketList& packets = connection->getPackets();
            for(tvPacketList::iterator itr = packets.begin(); itr != packets.end(); ++itr)
            {
                unsigned int opcode = (*itr).first;
                unsigned int len;
                unsigned char * data = (*itr).second.get(len);
                if(len >= 2048) break;
                strncpy(buf, (char*)data, len);
                buf[len] = 0;
                printf("SRV: %s\n", buf);
            }
        }
};

void test_listen(void *)
{
  
}

void sendPacket(Packet &packet, TCPClientConnection *client)
{
  client->sendData(packet.getOpcode(), (void*)packet.getData(), (int)packet.getLength());
}

void test_connect(void *number)
{
    /*const int NR_CLIENTS = 10;
    int i;
    TCPClientConnection* client[NR_CLIENTS];
    MyClientListener listener[NR_CLIENTS];

    for(i = 0; i < NR_CLIENTS; i++)
    {
        client[i] = TCPConnection::instance().newClientConnection("127.0.0.1", 1234);
        if(!client[i])
        {
            printf("cannot connect!");
            return;
        }
        client[i]->addListener(&listener[i]);
    }

    //srand((int)time(NULL));
    
    for(i = 0; i < 30; i++)
    {
        //sleep_var = 50 + rand()%250;


        int cli = rand() % 10;
        char msg[1024];
        sprintf(msg, "%d sends %d", cli, rand() % 1000);
        client[cli]->sendData(0, msg);
        while(sleep_var != 0)
            TCPConnection::instance().update();
    }

    _sleep(2000);


    for(i = 0; i < NR_CLIENTS; i++)
    {
        client[i]->disconnect();
        TCPConnection::instance().deleteClientConnection(client[i]);
    }*/

    TCPClientConnection* client = TCPConnection::instance().newClientConnection("127.0.0.1", 1234);
    
    {
      uint8 peerType = PEER_CLIENT;
      uint16 protoVersion = 1;
      uint32 cliVersion = 2;
      uint8 commType = 0;
      Packet msg(MSG_HANDSHAKE);
      msg << peerType << protoVersion << cliVersion << commType;
      sendPacket(msg, client);
    }

    client->disconnect();
}

void test_net()
{
    // thread for async sleep
    _beginthread(sleep_thread, 0, NULL);

    // initialize the net library
    TCPConnection::instance();

    _beginthread(test_listen, 0, NULL);
    _sleep(500);
    _beginthread(test_connect, 0, NULL);
}
#endif

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8