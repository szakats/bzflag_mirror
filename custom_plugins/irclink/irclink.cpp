// irclink.cpp : Defines the entry point for the DLL application.
//

#include "bzfsAPI.h"
#include "plugin_utils.h"

#include "libIRC.h"
#include "IRCTextUtils.h"

class IRCOptions  
{
public:
  IRCOptions()
  {
    ircPort = 6667;
    ircServer = "irc.freenode.net";
  }

  std::string ircServer;
  int	      ircPort;
  std::string channel;
  std::string nick;
  std::string host;
};

class UserOnIRC : public bz_ServerSidePlayerHandler
{
public:
  UserOnIRC( const std::string &nick)
  {
    IRCNick = nick;
    bzflagCallsign = "IRC-" + nick;
    bzPlayerID = bz_addServerSidePlayer(this);
  }
  virtual ~UserOnIRC()
  {
    bz_removeServerSidePlayer(bzPlayerID,this);
  }

  virtual void added(int player)
  {
    bzPlayerID = player;
    setPlayerData(bzflagCallsign.c_str(),NULL,"irclink",eObservers);
    joinGame();
  }

  virtual void playerRemoved(int player)
  {
    if (player == bzPlayerID)
      bzPlayerID = -1; // flag us for removal later
  }

  std::string IRCNick;
  std::string bzflagCallsign;

  int bzPlayerID;
};

class PlayerOnBZFlag : public IRCClientEventCallback
{
public:
  PlayerOnBZFlag (const std::string &callsign, int id );
  ~PlayerOnBZFlag ();
  virtual bool process ( IRCClient &ircClient, teIRCEventType	eventType, trBaseEventInfo &info );

  void sentToChannel ( const char* message );
  void sentToUser ( const std::string &to, const char* message );

  std::string IRCNick;
  std::string bzflagCallsign;
  int bzPlayerID;

  IRCClient *client;
};

// the class that handles the actual server portion
class RootServer : public bz_EventHandler , IRCClientEventCallback
{
public:
  RootServer();
  virtual void process(bz_EventData *eventData);
  virtual bool process ( IRCClient &ircClient, teIRCEventType	eventType, trBaseEventInfo &info );

  void ircToBZPrivageMessage ( const std::string &from, int to, const std::string &message );

  bool sendingChat;
  IRCClient client;

  std::map<std::string,UserOnIRC*>  ircUsersAsPlayers;
  std::map<int,PlayerOnBZFlag*>  playersOnIRC;

  UserOnIRC* getIRCUserFromID ( int id )
  {
    std::map<std::string,UserOnIRC*>::iterator itr = ircUsersAsPlayers.begin();

    while (itr != ircUsersAsPlayers.end())
    {
      if (itr->second->bzPlayerID == id)
	return itr->second;
      itr++;
    }

    return NULL;
  }

  PlayerOnBZFlag* getBZFlagPlayerFromNick ( const std::string &nick )
  {
     std::map<int,PlayerOnBZFlag*>::iterator itr = playersOnIRC.begin();

    while (itr != playersOnIRC.end())
    {
      if (compare_nocase(itr->second->IRCNick,nick) == 0)
	return itr->second;
      itr++;
    }

    return NULL;
  }
};

RootServer *rootServer = NULL;

IRCOptions ircOptions;

BZ_GET_PLUGIN_VERSION

BZF_PLUGIN_CALL int bz_Load ( const char* commandLine )
{
  if(commandLine)
  {
    std::vector<std::string> t = tokenize(std::string(commandLine),std::string("/"),0,false);
    if (t.size() >= 2)
    {
      ircOptions.channel = t[1];
      t = tokenize(t[0],std::string(":"),0,false);
      if (t.size()>1)
	ircOptions.ircPort = atoi(t[1].c_str());
      if (t.size() >0)
	ircOptions.ircServer = t[0];
    }
  }

  rootServer = new RootServer;

  bz_debugMessage(4,"irclink plugin loaded");
  bz_setMaxWaitTime(0.01f,"irclink");
  return 0;
}

BZF_PLUGIN_CALL int bz_Unload ( void )
{
  if (rootServer)
    delete(rootServer);
  bz_debugMessage(4,"irclink plugin unloaded");
  bz_clearMaxWaitTime("irclink");
  return 0;
}

size_t getStringSum ( const std::string &str )
{
  size_t v = 0;
  std::string::const_iterator itr = str.begin();
  while (itr != str.end())
  {
    v += (unsigned char)*itr;
    itr++;
  }
  return v;
}

bool validIRCSetup ( void )
{
  ircOptions.host = bz_getPublicAddr().c_str();
  if (!ircOptions.host.size())
    ircOptions.host = format("LocalBZServer%d",(int)bz_getCurrentTime());

  std::string hostNoDot = replace_all(ircOptions.host,std::string("."),std::string("-"));

  int hostPort = bz_getPublicPort();
  if (hostPort < 1)
    hostPort = 5154;

  if (!ircOptions.ircServer.size())
    return false;

  if (!ircOptions.channel.size())
  {
    std::string port = format("%d",hostPort);
    ircOptions.channel = "#bzflag-srv-" + hostNoDot + format("%d",hostPort);
  }

  if (ircOptions.ircPort == 0)
    ircOptions.ircPort = 6667;

  if (ircOptions.ircServer == "irc.freenode.net")
    if (ircOptions.channel == "#bzflag" || ircOptions.channel == "#bzflag-chat" || ircOptions.channel == "#bzchat")
      return false;

  if (!ircOptions.nick.size())
  {
    ircOptions.nick = format("server%d-%d",getStringSum(hostNoDot),hostPort);
  }

  return true;
}

RootServer::RootServer()
{
  sendingChat = false;
  if (!validIRCSetup())
    return;

  bz_registerEvent(bz_eTickEvent,this);
  bz_registerEvent(bz_eFilteredChatMessageEvent,this);
  bz_registerEvent(bz_ePlayerJoinEvent,this);
  bz_registerEvent(bz_ePlayerPartEvent,this);

  client.registerEventHandler(eIRCConnectedEvent,this);
  client.registerEventHandler(eIRCNoticeEvent,this);
  client.registerEventHandler(eIRCWelcomeEvent,this);
  client.registerEventHandler(eIRCChannelJoinEvent,this);
  client.registerEventHandler(eIRCUserPartEvent,this);
  client.registerEventHandler(eIRCChannelPartEvent,this);
  client.registerEventHandler(eIRCChannelMessageEvent,this);
  client.registerEventHandler(eIRCNickNameError,this);
  client.registerEventHandler(eIRCNickNameChange,this);
  client.registerEventHandler(eIRCUserJoinEvent,this);
  client.registerEventHandler(eIRCUserPartEvent,this);
  client.registerEventHandler(eIRCChanInfoCompleteEvent,this);

  client.connect(ircOptions.ircServer.c_str(),ircOptions.ircPort);
}

void RootServer::ircToBZPrivageMessage ( const std::string &from, int to, const std::string &message )
{
  // find the IRC users player ID
  std::string nick = from;
  makelower(nick);

  std::map<std::string,UserOnIRC*>::iterator itr = ircUsersAsPlayers.find(nick);

  sendingChat = true;
  if (itr == ircUsersAsPlayers.end())
  {
    std::string newMsg = from +":"+message;
    bz_sendTextMessage(BZ_SERVER,to,newMsg.c_str());
  }
  else
    bz_sendTextMessage(itr->second->bzPlayerID,to,message.c_str());

  sendingChat = false;
}

void RootServer::process(bz_EventData *eventData)
{
  switch(eventData->eventType)
  {
  case bz_eTickEvent:
    if (!client.process())
    {
      // cleanup connections?

      // try again
      client.connect(ircOptions.ircServer.c_str(),ircOptions.ircPort);
    }
    break;

  case bz_ePlayerJoinEvent:
    {
      bz_PlayerJoinPartEventData_V1 *data = (bz_PlayerJoinPartEventData_V1*)eventData;
      if (!getIRCUserFromID(data->playerID)) // don't worry about your SSPs joining
      {
	if (data->playerID >= 0)
	  playersOnIRC[data->playerID] = new PlayerOnBZFlag(std::string(bz_getPlayerCallsign(data->playerID)),data->playerID);
      }
    }
    break;

  case bz_ePlayerPartEvent:
    {
      bz_PlayerJoinPartEventData_V1 *data = (bz_PlayerJoinPartEventData_V1*)eventData;
      if (!getIRCUserFromID(data->playerID)) // don't worry about your SSPs parting
      {
	std::map<int,PlayerOnBZFlag*>::iterator itr = playersOnIRC.find(data->playerID);
	if (itr != playersOnIRC.end())
	{
	  delete(itr->second);
	  playersOnIRC.erase(itr);
	}
      }
    }
    break;

  case bz_eFilteredChatMessageEvent:
    {
      if (!sendingChat)
      {
	bz_ChatEventData_V1 *data = (bz_ChatEventData_V1*)eventData;
	if (data->from == BZ_SERVER && data->to == BZ_ALLUSERS && client.listChannels().size()) // it's server data, have the root server send it to the channel
	  client.sendMessage(ircOptions.channel,std::string(data->message.c_str()));
	else if (data->to == BZ_ALLUSERS)
	{
	   // it's to all the users from some player so find out the IRC alias of the player
	  // and send it to the channel from them
	  std::map<int,PlayerOnBZFlag*>::iterator itr = playersOnIRC.find(data->from);
	  if (itr != playersOnIRC.end())
	    itr->second->sentToChannel(data->message.c_str());
	}
	else
	{
	  // ok it's a team or PM
	  // find out if it's to an IRC user, from a real player
	  UserOnIRC *ircUSer = getIRCUserFromID(data->to);
	  std::map<int,PlayerOnBZFlag*>::iterator itr = playersOnIRC.find(data->from);

	  if (ircUSer && itr != playersOnIRC.end()) // it's from a player to an IRC user
	  {
	    itr->second->sentToUser(ircUSer->IRCNick,data->message.c_str());
	  }
	}
      }
    }
    break;
  }
}

bool RootServer::process ( IRCClient &ircClient, teIRCEventType	eventType, trBaseEventInfo &info )
{
  switch (eventType)
  {
  case eIRCConnectedEvent:
    client.login(ircOptions.nick,format("%s-%d","Server",ircOptions.ircPort),ircOptions.host,ircOptions.host);
    break;

  case eIRCWelcomeEvent:
    client.join(ircOptions.channel);
    break;

  case eIRCUserJoinEvent:
    {
      trClientJoinEventInfo *data = (trClientJoinEventInfo*)&info;

      // make sure it's not one of our sub bots, or us, or a services bot
      if (!getBZFlagPlayerFromNick(data->user) && data->user != ircOptions.nick && data->user != "ChanServ")
      {
	// check to see if we dont' have them already
	if (ircUsersAsPlayers.find(data->user) == ircUsersAsPlayers.end())
	{
	  ircUsersAsPlayers[data->user] = new UserOnIRC(data->user);
	}
      }
    }
    break;

  case eIRCUserPartEvent:
    {
      trClientPartEventInfo *data = (trClientPartEventInfo*)&info;

      // make sure it's not one of our sub bots, or us, or a services bot
      if (data->user != ircOptions.nick && data->user != "ChanServ")
      {
	PlayerOnBZFlag *playerSubBot = getBZFlagPlayerFromNick(data->user);
	
	if (playerSubBot)
	{
	  // it was one of our sub bots, get em to rejoin
	  playerSubBot->client->join(ircOptions.channel);
	}
	else
	{
	  // see if it's an IRC user, if so go and remove there player too
	  std::map<std::string,UserOnIRC*>::iterator itr = ircUsersAsPlayers.find(data->user);
	  if (itr != ircUsersAsPlayers.end())
	  {
	    delete(itr->second);
	    ircUsersAsPlayers.erase(itr);
	  }
	}
      }
    }
    break;

  case eIRCChanInfoCompleteEvent:
    {
      // save off the current set of players
      bz_APIIntList *players = bz_getPlayerIndexList();

      // scan the users and make some bots
 //     client.sendMessage(ircOptions.channel,format("%s online",ircOptions.host.c_str()));

      string_list ircUsers = client.listUsers(ircOptions.channel);
      for ( size_t s = 0; s < ircUsers.size(); s++)
      {
	if (ircUsers[s] != "ChanServ" && ircUsers[s] != ircOptions.nick)
	{
	  std::string nick = ircUsers[s];	 
	  ircUsersAsPlayers[nick] = new UserOnIRC(nick);
	}
      }

      if (players)
      {
	for ( unsigned int i = 0; i < players->size(); i++ )
	{
	  int playerID = players->get(i);
	  
	  if (playerID >= 0)
	    playersOnIRC[playerID] = new PlayerOnBZFlag(std::string(bz_getPlayerCallsign(playerID)),playerID);
	}
	bz_deleteIntList(players);

      }
    }
    break;

  case eIRCChannelPartEvent:
    break;

  case eIRCChannelMessageEvent:
    {
      // find out who it's from, if it's from an IRC user that has a player, have the player say it
      trClientMessageEventInfo * mesageInfo = (trClientMessageEventInfo*)&info;

      std::map<std::string,UserOnIRC*>::iterator itr = ircUsersAsPlayers.find(makelower(mesageInfo->from));
      if ( itr != ircUsersAsPlayers.end())
      {
	sendingChat = true;
	itr->second->sendChatMessage(mesageInfo->message.c_str());
	sendingChat = false;
      }
      // its not a player so just ignore it
    }
    break;

  case eIRCNickNameError:
    ircOptions.nick += "1";
    client.login(ircOptions.nick,format("%s:%d",ircOptions.host.c_str(),ircOptions.ircPort),ircOptions.host,ircOptions.host);
    return false;

   case eIRCNickNameChange:
    {
	trClientNickChangeEventInfo * nickInfo = (trClientNickChangeEventInfo*)&info;

	std::string oldNick = nickInfo->oldname;
	std::string newNick = nickInfo->newName;
	makelower(oldNick);
	makelower(newNick);

	// someone changed nicks, find them and keep the maping correct.

	std::map<std::string,UserOnIRC*>::iterator itr = ircUsersAsPlayers.find(oldNick);
	if (itr != ircUsersAsPlayers.end())
	{
	  if (ircUsersAsPlayers.find(newNick) == ircUsersAsPlayers.end())
	  {
	    UserOnIRC* user = itr->second;
	    user->IRCNick = nickInfo->newName;
	    ircUsersAsPlayers.erase(itr);
	    ircUsersAsPlayers[newNick] = user;
	  }
	}
	else
	{
	  // we didnt' know about the old player, so go and add it as a new
	  if (ircUsersAsPlayers.find(newNick) == ircUsersAsPlayers.end())
	    ircUsersAsPlayers[newNick] = new UserOnIRC(nickInfo->newName);
	}
    }
    break;
  }
  return true;
}

PlayerOnBZFlag::PlayerOnBZFlag (const std::string &callsign, int id )
{
  bzPlayerID = id;
  client = NULL;
  if (!callsign.size())
    return;
  client = new IRCClient;

  client->registerEventHandler(eIRCConnectedEvent,this);
  client->registerEventHandler(eIRCWelcomeEvent,this);
  client->registerEventHandler(eIRCNickNameError,this);
  client->registerEventHandler(eIRCPrivateMessageEvent,this);

  bzflagCallsign = callsign;
  IRCNick = "BZ-" + bzflagCallsign;

  client->connect(ircOptions.ircServer,ircOptions.ircPort);
}

PlayerOnBZFlag::~PlayerOnBZFlag ()
{
  if (client)
  {
    client->disconnect("leaving");
    delete(client);
  }
}

void PlayerOnBZFlag::sentToChannel ( const char* message )
{
  if (message && client)
    client->sendMessage(ircOptions.channel,std::string(message));
}

void PlayerOnBZFlag::sentToUser ( const std::string &to, const char* message )
{
  if (message && client)
    client->sendMessage(to,std::string(message));
}

bool PlayerOnBZFlag::process ( IRCClient &ircClient, teIRCEventType	eventType, trBaseEventInfo &info )
{
  switch (eventType)
  {
  case eIRCConnectedEvent:
    client->login(IRCNick,bzflagCallsign,ircOptions.host,ircOptions.host);
    break;

  case eIRCWelcomeEvent:
    client->join(ircOptions.channel);
    break;

  case eIRCPrivateMessageEvent:
    {
      trClientMessageEventInfo* messageInfo = (trClientMessageEventInfo*)&info;
      rootServer->ircToBZPrivageMessage(messageInfo->from,bzPlayerID,messageInfo->message);
    }
    break;

  case eIRCNickNameError:
    IRCNick += "1";
    client->login(IRCNick,bzflagCallsign,bzflagCallsign,ircOptions.host);
    return false;
  }
  return true;
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
