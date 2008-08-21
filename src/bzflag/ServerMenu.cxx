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

/* interface header */
#include "ServerMenu.h"

/* common implementation headers */
#include "FontManager.h"
#include "StateDatabase.h"

/* local implementation headers */
#include "FontSizer.h"
#include "MainMenu.h"
#include "HUDDialogStack.h"
#include "clientConfig.h"
#include "ConfigFileManager.h"
#include "bzflag.h"
#include "LocalFontFace.h"
#include "playing.h"

bool ServerMenuDefaultKey::keyPress(const BzfKeyEvent& key)
{
  ServerList &serverList = ServerList::instance();

  if (key.chr == 'f') {
    if (menu->tabbedControl->getActiveTab() == (HUDuiControl*)(menu->customTabControl))
      return false;

    if ((((HUDuiServerList*)menu->tabbedControl->getActiveTab()) == menu->favoritesList)||(((HUDuiServerList*)menu->tabbedControl->getActiveTab()->hasFocus()))||(((HUDuiServerList*)menu->tabbedControl->hasFocus())))
      return false;
    serverList.markAsFavorite(((HUDuiServerList*)menu->tabbedControl->getActiveTab())->getSelectedServer());
    return true;
  }

  return MenuDefaultKey::keyPress(key);
}

bool ServerMenuDefaultKey::keyRelease(const BzfKeyEvent& key)
{
  switch (key.chr) {
  case 27: // escape
  case 13: // return
    return true;
  }

  return false;
}

std::map<ServerPing*, std::pair<ServerItem, HUDuiServerList*>> ServerMenu::activePings;

ServerMenu::ServerMenu(): defaultKey(this), serverList(ServerList::instance()),
			  normalList(new HUDuiServerList()), favoritesList(new HUDuiServerList()),
			  recentList(new HUDuiServerList()), serverInfo(new HUDuiServerInfo()),
			  customTabControl(new HUDuiServerListCustomTab())
{
  // cache font face ID
  const LocalFontFace* fontFace = MainMenu::getFontFace();

  addPlayingCallback(&playingCB, this);

  serverList.updateFromCache();
  serverList.startServerPings(getStartupInfo());

  normalList->setFontFace(fontFace);
  favoritesList->setFontFace(fontFace);
  recentList->setFontFace(fontFace);
  serverInfo->setFontFace(fontFace);
  customTabControl->setFontFace(fontFace);

  title = new HUDuiLabel();
  title->setString("Servers");
  title->setFontFace(fontFace);

  tabbedControl = new HUDuiTabbedControl;
  tabbedControl->setFontFace(fontFace);
  tabbedControl->addTab(normalList, "All");
  tabbedControl->addTab(recentList, "Recent");
  tabbedControl->addTab(favoritesList, "Favorites");
  tabbedControl->addTab(customTabControl, "Create New Tab");
  tabbedControl->setActiveTab(0);

  serverList.addServerCallback(newServer, normalList);
  serverList.addFavoriteServerCallback(newServer, favoritesList);
  serverList.addRecentServerCallback(newServer, recentList);

  addControl(title, false);
  addControl(tabbedControl);
  addControl(serverInfo);

  initNavigation();
}

ServerMenu::~ServerMenu()
{
  // Blank
}

void ServerMenu::newServer(ServerItem* addedServer, void* data)
{
  if (addedServer->ping.pingTime != 0)
  {
    ((HUDuiServerList*)data)->addItem(*addedServer);
    return;
  }

  ServerPing *newping = new ServerPing(addedServer->ping.serverId.serverHost, ntohs(addedServer->ping.serverId.port));
  newping->start();
  ServerMenu::activePings.insert(std::pair<ServerPing*, std::pair<ServerItem, HUDuiServerList*>>(newping, std::pair<ServerItem, HUDuiServerList*>((*addedServer), ((HUDuiServerList*)data))));}

void ServerMenu::execute()
{
  if ((tabbedControl->getActiveTab() == (HUDuiControl*)customTabControl)&&
     (((HUDuiServerListCustomTab*)tabbedControl->getActiveTab())->createNew->hasFocus()))
  {
    HUDuiServerList* newServerList = customTabControl->createServerList();
    tabbedControl->addTab(newServerList, customTabControl->tabName->getString(), tabbedControl->getTabCount() - 1);
    for (int i=0; i<(int)serverList.size(); i++)
    {
      ((HUDuiServerList*)newServerList)->addItem(*(serverList.getServerAt(i)));
    }
    newServerList->searchServers(customTabControl->serverName->getString());
    ((HUDuiNestedContainer*)(tabbedControl->getTab(tabbedControl->getTabCount() - 2)))->getNav().set((size_t)1);
    return;
  }

  if ((tabbedControl->hasFocus())||
      (tabbedControl->getActiveTab()->hasFocus())||
      (tabbedControl->getActiveTabName() == "Create New Tab"))
    return;

  // update startup info
  StartupInfo* info = getStartupInfo();
  ServerItem* selectedServer = ((HUDuiServerList*)tabbedControl->getActiveTab())->getSelectedServer();
  strncpy(info->serverName, selectedServer->name.c_str(), ServerNameLen-1);
  info->serverPort = ntohs((unsigned short) selectedServer->ping.serverId.port);

  // all done
  HUDDialogStack::get()->pop();
}

void ServerMenu::resize(int _width, int _height)
{
  HUDDialog::resize(_width, _height);
  FontSizer fs = FontSizer(_width, _height);

  FontManager &fm = FontManager::instance();
  const LocalFontFace* fontFace = MainMenu::getFontFace();

  // reposition title
  float x, y;

  // use a big font for title, smaller font for the rest
  fs.setMin(0, (int)(1.0 / BZDB.eval("headerFontSize") / 2.0));
  const float titleFontSize = fs.getFontSize(fontFace->getFMFace(), "headerFontSize");

  title->setFontSize(titleFontSize);
  const float titleWidth = fm.getStringWidth(fontFace->getFMFace(), titleFontSize, title->getString().c_str());
  const float titleHeight = fm.getStringHeight(fontFace->getFMFace(), titleFontSize);
  x = 0.5f * ((float)_width - titleWidth);
  y = (float)_height - titleHeight;
  title->setPosition(x, y);

  fs.setMin(40,10);
  const float fontSize = fs.getFontSize(fontFace->getFMFace(), "alertFontSize");

  float edgeSpacer = fm.getStringWidth(fontFace->getFMFace(), fontSize, "X");
  float bottomSpacer = fm.getStringHeight(fontFace->getFMFace(), fontSize);

  float remainingSpace = y - bottomSpacer*2;
  float listHeight = remainingSpace*0.7f;

  tabbedControl->setSize((_width - (2*edgeSpacer)),listHeight);
  serverInfo->setSize((_width - (2*edgeSpacer)), (remainingSpace - bottomSpacer)*0.3f);

  y = y - (titleHeight/2) - tabbedControl->getHeight();

  tabbedControl->setPosition(edgeSpacer, y);
  serverInfo->setPosition(edgeSpacer, bottomSpacer/2);
  tabbedControl->setFontSize(fontSize);
  serverInfo->setFontSize(fontSize);
}

void ServerMenu::updateStatus()
{
  if ((tabbedControl->hasFocus())||(((HUDuiServerList*)tabbedControl->getActiveTab())->hasFocus())||(tabbedControl->getActiveTabName() == "Create New Tab"))
    serverInfo->setServerItem(NULL);
  else
    serverInfo->setServerItem(((HUDuiServerList*)tabbedControl->getActiveTab())->getSelectedServer());
}

void ServerMenu::playingCB(void* _self)
{
  ServerList &list = ServerList::instance();
  for (std::map<ServerPing*, std::pair<ServerItem, HUDuiServerList*>>::iterator
    i = ServerMenu::activePings.begin(); i != ServerMenu::activePings.end();) {
    i->first->doPings();
    if (i->first->done()) {
      std::string test = i->second.first.getServerKey();
      //ServerList &list = ServerList::instance();
      ServerItem* server = list.lookupServer(i->second.first.getServerKey());
      if (server == NULL)
	break;
      server->ping.pingTime = i->first->calcLag();
      i->second.second->addItem(*(server));
      delete i->first;
      ServerMenu::activePings.erase(i++);
      continue;
    }
    ++i;
  }

  ((ServerMenu*)_self)->serverList.checkEchos(getStartupInfo());
  ((ServerMenu*)_self)->updateStatus();
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
