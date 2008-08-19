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

// interface headers
#include "HUDuiServerList.h"

// common implementation headers
#include "TextUtils.h"
#include "bzglob.h"

#include "FontManager.h"
#include "LocalFontFace.h"

#include "OpenGLGState.h"
#include "OpenGlUtils.h"
#include "bzfgl.h"

//
// HUDuiServerList
//

float HUDuiServerList::DOMAIN_PERCENTAGE = 0.375f;
float HUDuiServerList::SERVER_PERCENTAGE = 0.375f;
float HUDuiServerList::PLAYER_PERCENTAGE = 0.125f;
float HUDuiServerList::PING_PERCENTAGE = 0.125f;

HUDuiServerList::HUDuiServerList() : HUDuiScrollList(), filterOptions(0), sortMode(NoSort), activeColumn(DomainName), reverseSort(false), devInfo(false), dataList(ServerList::instance())
{
  columns[DomainName] = std::pair<std::string, float*>("Addressed", &DOMAIN_PERCENTAGE);
  columns[ServerName] = std::pair<std::string, float*>("Server Name", &SERVER_PERCENTAGE);
  columns[PlayerCount] = std::pair<std::string, float*>("Player Count", &PLAYER_PERCENTAGE);
  columns[Ping] = std::pair<std::string, float*>("Ping", &PING_PERCENTAGE);
  getNav().push_front(this);
}

HUDuiServerList::HUDuiServerList(bool paged) : HUDuiScrollList(paged), filterOptions(0), sortMode(NoSort), dataList(ServerList::instance())
{
  // do nothing
}

HUDuiServerList::~HUDuiServerList()
{
  // do nothing
}

bool HUDuiServerList::comp(HUDuiControl* first, HUDuiControl* second)
{
  return (((HUDuiServerListItem*)first)->getServerKey() < ((HUDuiServerListItem*)second)->getServerKey());
}

struct HUDuiServerList::search: public std::binary_function<HUDuiControl*, std::string, bool>
{
public:
  result_type operator()(first_argument_type control, second_argument_type pattern) const
    {
      HUDuiServerListItem* item = (HUDuiServerListItem*) control;

      return !(glob_match(TextUtils::tolower(pattern), TextUtils::tolower(item->getServerName())));
    }
};

template<int sortType> struct HUDuiServerList::compare: public std::binary_function<HUDuiControl*, HUDuiControl*, bool>
{
public:
  bool operator()(HUDuiControl* first, HUDuiControl* second) const
    {
      HUDuiServerListItem* _first = (HUDuiServerListItem*) first;
      HUDuiServerListItem* _second = (HUDuiServerListItem*) second;

      switch (sortType) {
	case DomainName:
	  return (_first->getDomainName().compare(_second->getDomainName()) < 0);
	  break;

	case ServerName:
	  return (_first->getServerName().compare(_second->getServerName()) < 0);
	  break;

	case PlayerCount:
	  return (_first->getPlayerCount().compare(_second->getPlayerCount()) < 0);
	  break;

	case Ping:
	  return (_first->getServerPing().compare(_second->getServerPing()) < 0);
	  break;
      }
      return false;
    }
};

struct HUDuiServerList::filter: public std::binary_function<HUDuiControl*, uint16_t, bool>
{
public:
  result_type operator()(first_argument_type control, second_argument_type filter) const
    {
      ServerList &serverList = ServerList::instance();
      HUDuiServerListItem* item = (HUDuiServerListItem*) control;
      ServerItem* server = serverList.lookupServer(item->getServerKey());

      bool returnValue = false;

      for (int i = 2; i < EndOfFilterConstants; ++i)
      {
	if ((filter & i) == i)
	{
	  switch (i) {
	    case EmptyServer:
	      returnValue = (server->getPlayerCount() == 0);
	      break;

	    case FullServer:
	      returnValue = (server->getPlayerCount() == server->ping.maxPlayers);
	      break;

	    case JumpingOn:
	      returnValue = ((server->ping.gameOptions & JumpingGameStyle) != JumpingGameStyle);
	      break;

	    case JumpingOff:
	      returnValue = ((server->ping.gameOptions & JumpingGameStyle) == JumpingGameStyle);
	      break;

	    case RicochetOn:
	      returnValue = ((server->ping.gameOptions & RicochetGameStyle) != RicochetGameStyle);
	      break;

	    case RicochetOff:
	      returnValue = ((server->ping.gameOptions & RicochetGameStyle) == RicochetGameStyle);
	      break;
	  }
	}
	if (returnValue == true)
	  return true;
      }
      return false;
    }
};

// Add a new item to our scrollable list
void HUDuiServerList::addItem(ServerItem item)
{
  HUDuiServerListItem* newItem = new HUDuiServerListItem(item);
  newItem->setColumnSizes(DOMAIN_PERCENTAGE, SERVER_PERCENTAGE, PLAYER_PERCENTAGE, PING_PERCENTAGE);
  newItem->setFontFace(getFontFace());
  newItem->setFontSize(getFontSize());  
  newItem->setSize(getWidth(), 10);  

  // Don't add duplicates to the list, apply filters now before adding
  if ((std::binary_search(originalItems.begin(), originalItems.end(), (HUDuiControl*) newItem, comp))||
     (std::bind2nd(filter(), filterOptions)(newItem)))
  {
    delete newItem;
    return;
  }

  originalItems.push_back(newItem);
  originalItems.sort(comp);
  items.push_back(newItem);

  addControl(newItem);
  sortBy(sortMode);
}

// Over-ride the generic HUDuiControl version of addItem
void HUDuiServerList::addItem(HUDuiControl* item)
{
  return; // Do nothing
}

void HUDuiServerList::setFontSize(float size)
{
  HUDuiScrollList::setFontSize(size);
}

void HUDuiServerList::setFontFace(const LocalFontFace* face)
{
  HUDuiScrollList::setFontFace(face);
}

void HUDuiServerList::setSize(float width, float height)
{
  FontManager &fm = FontManager::instance();

  float columnsHeight = fm.getStringHeight(getFontFace()->getFMFace(), getFontSize());
  HUDuiScrollList::setSize(width, height - columnsHeight - columnsHeight/2);
}

void HUDuiServerList::update()
{
  HUDuiScrollList::update();

  std::list<HUDuiControl*>::iterator it;

  for (it=items.begin(); it != items.end(); it++)
  {
    ((HUDuiServerListItem*)(*it))->setColumnSizes(DOMAIN_PERCENTAGE, SERVER_PERCENTAGE, PLAYER_PERCENTAGE, PING_PERCENTAGE);
  }
}

float HUDuiServerList::getHeight() const
{
  FontManager &fm = FontManager::instance();

  float columnsHeight = fm.getStringHeight(getFontFace()->getFMFace(), getFontSize());

  return HUDuiScrollList::getHeight() + columnsHeight + columnsHeight/2;
}

void HUDuiServerList::doRender()
{
  HUDuiScrollList::doRender();

  FontManager &fm = FontManager::instance();

  float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  float activeColor[3] = {0.0f, 1.0f, 0.0f};

  glColor4fv(color);

  float columnsHeight = fm.getStringHeight(getFontFace()->getFMFace(), getFontSize());

  glOutlineBoxHV(1.0f, getX(), getY(), getX() + getWidth(), getY() + getHeight() + 1, -0.5f);
  glOutlineBoxHV(1.0f, getX(), getY(), getX() + getWidth(), getY() + getHeight() - columnsHeight - columnsHeight/2 + 1, -0.5f);

  float y = getY() + getHeight() - columnsHeight;
  float x = getX();

  std::string columnTitle = "";

  for (int i=DomainName; i != NoSort; i++)
  {
    if (sortMode == i)
      if (reverseSort)
	columnTitle = columns[i].first + "   V";
      else
	columnTitle = columns[i].first + "   ^";
    else
      columnTitle = columns[i].first;

    if ((activeColumn == i)&&(hasFocus()))
      fm.drawString(x, y, 0, getFontFace()->getFMFace(), getFontSize(), columnTitle.c_str(), activeColor);
    else
      fm.drawString(x, y, 0, getFontFace()->getFMFace(), getFontSize(), columnTitle.c_str(), color);

    x = x + ((*columns[i].second)*(getWidth()));
    glOutlineBoxHV(1.0f, getX(), getY(), x, getY() + getHeight() + 1, -0.5f);
  }

  if (devInfo)
  {
    char temp[50];
    sprintf(temp, "COLUMN SIZES: %f %f %f %f", HUDuiServerList::DOMAIN_PERCENTAGE, HUDuiServerList::SERVER_PERCENTAGE, HUDuiServerList::PLAYER_PERCENTAGE, HUDuiServerList::PING_PERCENTAGE);
    fm.drawString(getX(), getY() + getHeight() + 7*columnsHeight, 0, getFontFace()->getFMFace(), getFontSize(), temp);
  }
}

ServerItem* HUDuiServerList::getSelectedServer()
{
  if (items.size() <= 0)
    return NULL;

  std::list<HUDuiControl*>::iterator it;
  it = items.begin();
  std::advance(it, getSelected());

  HUDuiServerListItem* selected = (HUDuiServerListItem*) *it;
  return dataList.lookupServer(selected->getServerKey());
}

void HUDuiServerList::searchServers(std::string pattern)
{
  applyFilters();
  items.remove_if(std::bind2nd(search(), pattern));
  refreshNavQueue();
  setSelected(0);
  getNav().set((size_t) 0);
}

void HUDuiServerList::applyFilters()
{
  items = originalItems;

  items.remove_if(std::bind2nd(filter(), filterOptions));

  refreshNavQueue();
  setSelected(0);
}

void HUDuiServerList::toggleFilter(FilterConstants filter)
{
  filterOptions ^= filter;
  applyFilters();
}

void HUDuiServerList::sortBy(SortConstants sortType)
{
  if (sortMode == sortType)
    reverseSort = !reverseSort;
  else
    sortMode = sortType;

  switch (sortType) {
    case DomainName:
      items.sort(compare<DomainName>());
      break;

    case ServerName:
      items.sort(compare<ServerName>());
      break;

    case PlayerCount:
      items.sort(compare<PlayerCount>());
      break;

    case Ping:
      items.sort(compare<Ping>());
      break;
  }

  if (reverseSort)
    items.reverse();

  refreshNavQueue();
  setSelected((int) getNav().getIndex());
}

void HUDuiServerList::setActiveColumn(int column)
{
  if (column < DomainName)
    column = DomainName;
  else if (column >= Ping)
    column = Ping;

  activeColumn = column;
}

int HUDuiServerList::getActiveColumn()
{
  return activeColumn;
}

size_t HUDuiServerList::callbackHandler(size_t oldFocus, size_t proposedFocus, HUDNavChangeMethod changeMethod)
{
  // Don't scroll up any further once you've hit the top of the list
  if ((oldFocus == 0)&&(changeMethod == hnPrev)) proposedFocus = oldFocus;
  
  // Don't scroll past the bottom of the list
  if ((oldFocus == getNav().size() - 1)&&(changeMethod == hnNext)) proposedFocus = oldFocus;

  int newFocus = (int) proposedFocus - 1;
  setSelected(newFocus);
  if (newFocus != -1)
    return (size_t) getSelected() + 1;
  else
    return (size_t) getSelected();
}

void HUDuiServerList::refreshNavQueue()
{
  HUDuiControl* currentFocus = getNav().get();
  bool inFocus = currentFocus->hasFocus();
  getNav().clear();

  getNav().push_front(this);

  std::list<HUDuiControl*>::iterator it;

  for (it = items.begin(); it != items.end(); ++it)
  {
    HUDuiControl* item = *it;
    addControl(item);
  }
  if (inFocus)
    getNav().set(currentFocus);
  else
    getNav().setWithoutFocus(currentFocus);
}

bool HUDuiServerList::doKeyPress(const BzfKeyEvent& key)
{
  if (key.chr == 0)
    switch (key.button) {

      case BzfKeyEvent::Down:
	if (hasFocus())
          getNav().next();
        break;

      case BzfKeyEvent::Up:
	if (hasFocus())
          getNavList()->prev();
        break;

      case BzfKeyEvent::Left:
	if (hasFocus())
          setActiveColumn(getActiveColumn() - 1);
        break;

      case BzfKeyEvent::Right:
	if (hasFocus())
          setActiveColumn(getActiveColumn() + 1);
        break;

      default:
        return false;
  }
  else if (key.chr == 's')
  {
      if (hasFocus())
      {
	switch (getActiveColumn()) {
	  case DomainName:
	    sortBy(DomainName);
	    break;

	  case ServerName:
	    sortBy(ServerName);
	    break;

	  case PlayerCount:
	    sortBy(PlayerCount);
	    break;

	  case Ping:
	    sortBy(Ping);
	    break;
	}
      }
  } else if (key.chr == '+') {
    if (hasFocus())
    {
	switch (getActiveColumn()) {
	  case DomainName:
	    HUDuiServerList::DOMAIN_PERCENTAGE += 0.005f;
	    HUDuiServerList::SERVER_PERCENTAGE -= 0.005f;
	    break;

	  case ServerName:
	    HUDuiServerList::SERVER_PERCENTAGE += 0.005f;
	    HUDuiServerList::PLAYER_PERCENTAGE -= 0.005f;
	    break;

	  case PlayerCount:
	    HUDuiServerList::PLAYER_PERCENTAGE += 0.005f;
	    HUDuiServerList::PING_PERCENTAGE -= 0.005f;
	    break;

	  case Ping:
	    HUDuiServerList::PING_PERCENTAGE += 0.005f;
	    HUDuiServerList::PLAYER_PERCENTAGE -= 0.005f;
	    break;
	}
	update();
    }
  } else if (key.chr == '-') {
    if (hasFocus())
    {
	switch (getActiveColumn()) {
	  case DomainName:
	    HUDuiServerList::DOMAIN_PERCENTAGE -= 0.005f;
	    HUDuiServerList::SERVER_PERCENTAGE += 0.005f;
	    break;

	  case ServerName:
	    HUDuiServerList::SERVER_PERCENTAGE -= 0.005f;
	    HUDuiServerList::PLAYER_PERCENTAGE += 0.005f;
	    break;

	  case PlayerCount:
	    HUDuiServerList::PLAYER_PERCENTAGE -= 0.005f;
	    HUDuiServerList::PING_PERCENTAGE += 0.005f;
	    break;

	  case Ping:
	    HUDuiServerList::PING_PERCENTAGE -= 0.005f;
	    HUDuiServerList::PLAYER_PERCENTAGE += 0.005f;
	    break;
	}
	update();
    }
  }
  if (key.chr == 'd') {
    if (hasFocus())
    {
      devInfo = !devInfo;
    }
  }

  switch (key.chr) {
    case 13: // Return
    case 27:
      return false;
  }

  return false;
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
