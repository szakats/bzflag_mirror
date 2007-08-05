/* bzflag
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* class interface header */
#include "ircControl.h"


#include <fstream>

#include "libIRC.h"


// bzflag common header
#include "TextUtils.h"



ircControl::ircControl() {

  //mmm, defaults.

  configured = false;
  connected = false;

  server = "irc.freenode.net" //subject to change if we use our own IRCd 
  port = 6667;

  //TODO: pick a default control channel and set it up here

}

//FIXME: Some of this text wrangling could probably be added to TextUtils... 
bool ircControl::loadConfigFile(std::string filename) {

  std::string errmsg;

  //open conf file

  std::ifstream ircconf;
  ircconf.open(filename.c_str());

  if (!ircconf.is_open()) {

    errmsg = "Failed to load IRC configuration file \"" + filename + "\"\n; IRC will not be used.";
    logDebugMessage(1, errmsg.c_str());

    return false;

  }

  //read the conf file
  std::string curline;
  std::vector<std::string> curlineargs;

  int lineNum = 1;

  while (getline(ircconf, curline)) {

    //snip the comment out of the line if it exists
    std::string::size_type index = curline.find('#');
    if (index != std::string::npos) {

      curline = curline.substr(0, index);

    }

    //split the line into arguments
    curlineargs = TextUtils::tokenize(curline, std::string("-"), 0, true);

    //there should be nothing other than whitespace before the first hyphen... check.
    std::string trimmedGarbage = TextUtils::no_whitespace(curlineargs.at(0));

    if (trimmedGarbage != "") {
      errmsg = "Unrecognized garbage \"" + trimmedGarbage + "\" in IRC config at line " + lineNum + ", ignoring";
      logDebugMessage(2, errmsg.c_str());
    }

    for (unsigned int x = 1; x < curlineargs.size(); x++) {

      //trim whitespace from the ends
      //Find the last non-whitespace char...
      int last;
      for (last = curlineargs.at(x).length() - 1; last >= 0; last--)
	if (!TextUtils::isWhitespace(str[x])) 
	  break;

      if (last >= 0) { //if last gets to -1, the string is all whitespace (or empty). No reason to continue processing.
	
	//...otherwise, time to do the front...
	for (unsigned int first = 0; first < curlineargs.at(x).length(); first++) 
	  if (!TextUtils::isWhitespace(str[first]))
	    break;

	//trim the string...
	curlineargs.at(x) = curlineargs.at(x).substr(first, last - first + 1);

	//okay, split the arg into params
	std::vector<std::string> params = TextUtils::tokenize(curlineargs->at(x), std::string(" "), 0, true);

	//erase blank entries
	for (std::vector<std::string>::iterator x = params.begin(); x != params.end(); x++) 
	  if (*x == "") 
	    params.erase(x);

	if (params.size() == 0) //sanity check- an empty param should have been caught already
	  continue;

	//Now we start processing the arguments (finally!) Ordered by likely frequency in the configuration.
	if (TextUtils::compare_nocase(params.at(0), std::string("channel")) && params.size() >= 2) {

	  if (channels.count(params.at(1)) > 0) //make sure we don't have this channel already
	    continue;

	  ircChannel newchan;

	  newchan.name = params.at(1);

	  //loop through the remaining params and configure this channel accordingly
	  for (int i = 2; i < params.size(); i++) {
	    if (TextUtils::compare_nocase(params.at(0), std::string("relayGameChat")) == 0) {
	      newchan.relayGameChat = true;

	    } else if (TextUtils::compare_nocase(params.at(0), std::string("relayGameJoinsAndParts")) == 0) {
	      newchan.relayGameJoinsAndParts = true;

	    } else if (TextUtils::compare_nocase(params.at(0), std::string("relayIRCChat")) == 0) {
	      newchan.relayIRCChat = true;

	    } else if (TextUtils::compare_nocase(params.at(0), std::string("relayIRCJoinsAndParts")) == 0) {
	      newchan.relayIRCJoinsAndParts = true;

	    } else if (TextUtils::compare_nocase(params.at(0), std::string("acceptCommands")) == 0) {
	      newchan.acceptCommands = true;

	    } else { 
	      errmsg = "Unrecognized parameter \"" + params.at(i) + "\" for channel \"" + params.at(1) + "\" on line " + lineNum + ", ignoring.\n";
	      logDebugMessage(2, errmsg.c_str());
	    }
	  }

	  //our channel is configured. Add it to the system.
	  channels[newchan.name] = newchan;

	} else if (TextUtils::compare_nocase(params.at(0), std::string("initCommand")) == 0 && params.size() == 2) {
	  initCommands.push_back(params.at(1));

	} else if (TextUtils::compare_nocase(params.at(0), std::string("nick")) == 0 && params.size() == 2) {
	  nick = params.at(1);

	} else if (TextUtils::compare_nocase(params.at(0), std::string("pwd")) == 0 && params.size() == 2) {
	  pwd = params.at(1);

	} else if (TextUtils::compare_nocase(params.at(0), std::string("controlChannel")) == 0 && params.size() == 2) {
	  controlChannel.name = params.at(1);

	} else if (TextUtils::compare_nocase(params.at(0), std::string("server")) == 0 && params.size() == 2) {
	  server = params.at(1);

	} else if (TextUtils::compare_nocase(params.at(0), std::string("port")) == 0 && params.size() == 2) {
	  port = atoi(params.at(1).c_str());

	} else {
	  errmsg = "Unrecognized argument \"" + curlineargs.at(x) + "\" on line " + lineNum + ", ignoring.\n";
	  logDebugMessage(2, errmsg.c_str());
	}
      }
    }

      lineNum++;
  }

  if (nick == "") {
    errmsg = "No nick specified. IRC will not be used.";
    logDebugMessage(1, errmsg.c_str());

    return false;
  }

  configured = true;
  return true; 
}

bool ircControl::init(){

  std::string errmsg;

  //make sure it's configured... sanity check
  if (!configured) 
    return false; //we don't need to explain- the conf loader will do that when it fails

  //register events
  client.registerEventHandler(eIRCNoticeEvent, this);
  client.registerEventHandler(eIRCNickNameError, this);
  client.registerEventHandler(eIRCNickNameChange, this);
  //client.registerEventHandler(eIRCWelcomeEvent, this);
  //client.registerEventHandler(eIRCEndMOTDEvent, this);
  client.registerEventHandler(eIRCChannelJoinEvent, this);
  client.registerEventHandler(eIRCChannelPartEvent, this);
  client.registerEventHandler(eIRCChannelBanEvent, this);
  client.registerEventHandler(eIRCChannelMessageEvent, this);
  client.registerEventHandler(eIRCPrivateMessageEvent, this);
  //client.registerEventHandler(eIRCTopicEvent, this);
  client.registerEventHandler(eIRCUserJoinEvent, this);
  client.registerEventHandler(eIRCUserPartEvent, this);
  client.registerEventHandler(eIRCUserKickedEvent, this);
  //client.registerEventHandler(eIRCTopicChangeEvent, this);
  client.registerEventHandler(eIRCChanInfoCompleteEvent, this);
  client.registerEventHandler(eIRCChannelModeSet, this);
  client.registerEventHandler(eIRCChannelUserModeSet, this);
  client.registerEventHandler(eIRCUserModeSet, this);
  client.registerEventHandler(eIRCQuitEvent, this);

  //connect
  if (!client.connect(server, port)) {
    errmsg = "IRC Connection failed.";
    logDebugMessage(1, errmsg.c_str());
  
  //send init commands and wait
  //TODO: Implement this... possibly better handled by an event, like the MOTD/welcome?

  //join channels
  for (std::map<std::string, ircChannel>::iterator itr = channels.begin(); itr != channels.end(); itr++) {
    if (client.join(itr->name)) {
      itr->joined = true;
    } else {
      errmsg = "Unable to join channel \"" + itr->name + "\"\n";
      logDebugMessage(1, errmsg.c_str());
    }
  }

}

bool ircControl::terminate(bool forShutdown){

  if (!connected)
    return false; //can't disconnect if we're not connected...

  //issue a /quit and destroy everything
  if (forShutdown)
    return client.disconnect("BZFS: Shutting down");
  else
    return client.disconnect("BZFS: Premature IRC disconnection!");
}

bool ircControl::update(){

  if (!client->process()) {
    //there seems to be a connection problem- do something
  }

}

bool ircControl::process (IRCClient &ircClient, teIRCEventType eventType, trBaseEventInfo &info) {

  //meat and potatoes of the implementation goes here

  if (eventType == eIRCNoticeEvent) {

  } else if eventType == eIRCNickNameError) {

  } else if eventType == eIRCNickNameChange) {

  } else if eventType == eIRCWelcomeEvent) {

  } else if eventType == eIRCEndMOTDEvent) {
    //this will finish off the connection stuff, like commands and channel joins.

  } else if eventType == eIRCChannelJoinEvent) {

  } else if eventType == eIRCChannelPartEvent) {

  } else if eventType == eIRCChannelBanEvent) {

  } else if eventType == eIRCChannelMessageEvent) {

    trMessageEventInfo chanMsgInfo = (trMessageEventInfo)info;

    if (channels.count(chanMsgInfo.source) == 1) { //make sure we're actually supposed to be in this channel
    if (channels[chanMsgInfo.source].relayIRCChat) {
      //forward the chat msg to the game if we're supposed to.
      std::string msg = "[IRC]" + chanMsgInfo.from + ": " + chanMsgInfo.message;
      sendMessage(serverPlayer, AllPlayers, msg);

  } else if eventType == eIRCPrivateMessageEvent) {

  } else if eventType == eIRCTopicEvent) {

  } else if eventType == eIRCUserJoinEvent) {

  } else if eventType == eIRCUserPartEvent) {

  } else if eventType == eIRCUserKickedEvent) {

  } else if eventType == eIRCTopicChangeEvent) {

  } else if eventType == eIRCChanInfoCompleteEvent) {

  } else if eventType == eIRCChannelModeSet) {

  } else if eventType == eIRCChannelUserModeSet) {

  } else if eventType == eIRCUserModeSet) {

  } else if eventType == eIRCQuitEvent) {

  }

}


