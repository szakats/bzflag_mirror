/* bzflag
 * Copyright (c) 1993 - 2003 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <iostream>
#include <map>
#include <string>

#include "BZAdminClient.h"
#include "BZAdminUI.h"
#include "OptionParser.h"
#include "ServerLink.h"
#include "UIMap.h"

// causes persistent rebuilding to obtain build versioning
#include "version.h"

using namespace std;

/** @file
    This is the main file for bzadmin, the bzflag text client.
*/


// some global variables
struct CLOptions {
  CLOptions() : ui("curses"), showHelp(false) { }
  string ui;
  bool showHelp;
} clOptions;


int main(int argc, char** argv) {

  // no curses, use stdboth as default instead
  const UIMap::map_t& interfaces(UIMap::getInstance().getMap());
  if (interfaces.find("curses") == interfaces.end())
    clOptions.ui = "stdboth";
  
  // build a usage string with all interfaces
  UIMap::map_t::const_iterator uiIter;
  string uiUsage;
  for (uiIter = interfaces.begin(); uiIter != interfaces.end(); ++uiIter)
    uiUsage += uiIter->first + '|';
  uiUsage = string("[-ui ") + uiUsage.substr(0, uiUsage.size() - 1) + ']';
  
  // register and parse command line arguments
  OptionParser op;
  op.registerVariable("ui", clOptions.ui, uiUsage,
		      "choose a user interface");
  op.registerVariable("help", clOptions.showHelp, "[-help]",
		      "print this help message");
  if (!op.parse(argc, argv)) {
    cerr<<op.getError()<<endl;
    op.printUsage(cout, argv[0]);
    cout<<"CALLSIGN@HOST[:PORT] [COMMAND] [COMMAND] ..."<<endl;
    return 1;
  }
  if (clOptions.showHelp) {
    cout<<"bzadmin "<<getAppVersion()<<endl;
    op.printUsage(cout, argv[0]);
    cout<<"CALLSIGN@HOST[:PORT] [COMMAND] [COMMAND] ..."<<endl<<endl;
    op.printHelp(cout);
    return 0;
  }
  
  // check that we have callsign and host in the right format and extract them
  if (op.getParameters().size() == 0) {
    cerr<<"You have to specify callsign@host."<<endl;
    return 1;
  }
  const string& namehost = op.getParameters()[0];
  int atPos = namehost.find('@');
  if (atPos == -1) {
    cerr<<"You have to specify callsign@host."<<endl;
    return 1;
  }
  string name = namehost.substr(0, atPos);
  string host = namehost.substr(atPos + 1);
  int port = ServerPort;
  int cPos = host.find(':');
  if (cPos != -1) {
    port = atoi(host.substr(cPos + 1).c_str());
    host = host.substr(0, cPos);
  }
  
  // check that the ui is valid
  uiIter = UIMap::getInstance().getMap().find(clOptions.ui);
  if (uiIter == UIMap::getInstance().getMap().end()) {
    cerr<<"There is no interface called \""<<clOptions.ui<<"\"."<<endl;
    return 1;
  }
  
  // try to connect
  BZAdminClient client(name, host, port);
  if (!client.isValid())
    return 1;
  
  // if we got commands as arguments, send them and exit
  if (op.getParameters().size() > 1) {
    for (unsigned int i = 1; i < op.getParameters().size(); ++i)
      client.sendMessage(op.getParameters()[i], AllPlayers);
    client.waitForServer();
    return 0;
  }

  // choose UI and run the main loop
  BZAdminUI*  ui = uiIter->second(client.getPlayers(), client.getMyId());
  client.setUI(ui);
  client.runLoop();
  delete ui;

  return 0;
}


