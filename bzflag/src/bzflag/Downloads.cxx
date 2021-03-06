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

/* interface header */
#include "Downloads.h"

/* common implementation headers */
#include "cURLManager.h"
#include "AnsiCodes.h"
#include "CacheManager.h"
#include "ObstacleMgr.h"
#include "TextureManager.h"
#include "WorldText.h"

/* local implementation headers */
#include "playing.h"
#include "HUDDialogStack.h"


// local variables for file tracker
static int totalTex = 0;
static int currentTex = 0;
static int runs = 0;


// FIXME - someone write a better explanation
static const char DownloadContent[] =
  "#\n"
  "# This file controls the access to servers for downloads.\n"
  "# Patterns are attempted in order against both the hostname\n"
  "# and ip. The first matching pattern sets the state. If no\n"
  "# patterns are matched, then the server is authorized. There\n"
  "# are four types of matches:\n"
  "#\n"
  "#   simple globbing (* and ?)\n"
  "#     allow\n"
  "#     deny\n"
  "#\n"
  "#   regular expressions\n"
  "#     allow_regex\n"
  "#     deny_regex\n"
  "#\n"
  "\n"
  "#\n"
  "# To authorize all servers, remove the following lines:\n"
  "#\n"
  "\n"
  "allow   images.bzflag.org\n"
  "allow *.images.bzflag.org\n"
  "\n"
  "allow   data.bzflag.org\n"
  "allow *.data.bzflag.org\n"
  "\n"
  "allow   safe.bzflag.org\n"
  "allow *.safe.bzflag.org\n"
  "\n"
  "allow   moderated.bzflag.org\n"
  "allow *.moderated.bzflag.org\n"
  "\n"
  "deny *\n";


template <>
Downloads* Singleton<Downloads>::_instance = (Downloads*)NULL;


class CachedTexture : private cURLManager {
  public:
    CachedTexture(const std::string &texUrl);

    virtual void finalization(char *data, unsigned int length, bool good);

    static void  setParams(bool check, long timeout);
    static int   activeTransfer();
  private:

    virtual void collectData(char* ptr, int len);

    std::string url;
    static bool checkForCache;
    static long httpTimeout;
    static int  textureCounter;
    static int  byteTransferred;
    bool        timeRequest;
};


bool CachedTexture::checkForCache   = false;
long CachedTexture::httpTimeout     = 0;
int CachedTexture::textureCounter = 0;
int CachedTexture::byteTransferred = 0;


CachedTexture::CachedTexture(const std::string &texUrl) : cURLManager()
{
  CacheManager::CacheRecord oldrec;

  setURL(texUrl);
  url = texUrl;

  // use the cache?
  bool cached = CACHEMGR.findURL(texUrl, oldrec);
  if (cached && !checkForCache) {
    // use the cached file
    MATERIALMGR.setTextureLocal(texUrl, oldrec.name);
  }
  else {
    textureCounter++;
    if (httpTimeout > 0.0) {
      setTimeout(httpTimeout);
    }
    setRequestFileTime(true);
    timeRequest = cached;
    std::string msg = ColorStrings[GreyColor];
    msg	    += "downloading: " + url;
    addMessage(NULL, msg);
    if (cached) {
      // use the cached file -- just in case
      MATERIALMGR.setTextureLocal(url, oldrec.name);
      setTimeCondition(ModifiedSince, oldrec.date);
    }
    addHandle();
  }
}


void CachedTexture::setParams(bool check, long timeout)
{
  checkForCache   = check;
  httpTimeout     = timeout;
  textureCounter  = 0;
  byteTransferred = 0;
}


void CachedTexture::finalization(char *data, unsigned int length, bool good)
{
  time_t filetime;

  textureCounter--;
  if (good) {
    if (length) {
      getFileTime(filetime);
      // CACHEMGR generates name, usedDate, and key
      CacheManager::CacheRecord rec;
      rec.url  = url;
      rec.size = length;
      rec.date = filetime;
      CACHEMGR.addFile(rec, data);
      const std::string localname = CACHEMGR.getLocalName(url);
      TextureManager& TEXMGR = TextureManager::instance();
      if (TEXMGR.isLoaded(localname)) {
	TEXMGR.reloadTextureImage(localname); // reload with the new image
      }
      MATERIALMGR.setTextureLocal(url, localname);
    }
  }
  else {
    CacheManager::CacheRecord rec;
    if (CACHEMGR.findURL(url, rec)) {
      MATERIALMGR.setTextureLocal(url, rec.name);
    } else {
      MATERIALMGR.setTextureLocal(url, "");
    }
  }
}


int CachedTexture::activeTransfer()
{
  return textureCounter;
}


void CachedTexture::collectData(char* ptr, int len)
{
  char buffer[128];

  if (runs == 0) {
    totalTex = textureCounter;
  }

  cURLManager::collectData(ptr, len);
  byteTransferred += len;

  //Make it so it counts textures in reverse order (0 to max instead of max to 0)
  currentTex = totalTex - textureCounter + 1;

  //Turn bytes into kilobytes
  sprintf (buffer, "Downloading texture (%d of %d): %d KB", currentTex, totalTex, byteTransferred/1024);
  runs++;

  HUDDialogStack::get()->setFailedMessage(buffer);
}


static std::vector<CachedTexture*> cachedTexVector;


Downloads::Downloads()
{
  downloadAccessList = new AccessList("DownloadAccess.txt", DownloadContent);
  textureDownloading = false;
}


Downloads::~Downloads()
{
  delete downloadAccessList;
}


void Downloads::startDownloads(bool doDownloads, bool updateDownloads,
			       bool referencing)
{
  totalTex = 0;
  currentTex = 0;
  runs = 0;

  CACHEMGR.loadIndex();
  CACHEMGR.limitCacheSize();

  downloadAccessList->reload();

  BzMaterialManager::TextureSet texSet;
  BzMaterialManager::TextureSet::iterator set_it;
  MATERIALMGR.makeTextureList(texSet, referencing);

  // get the fonts as well
  const std::vector<WorldText*>& texts = OBSTACLEMGR.getTexts();
  for (size_t i = 0; i < texts.size(); i++) {
    const WorldText* text = texts[i];
    if (CacheManager::isCacheFileType(text->font)) {
      texSet.insert(text->font);
    }
  }

  float timeout = 15;
  if (BZDB.isSet("httpTimeout")) {
    timeout = BZDB.eval("httpTimeout");
  }
  CachedTexture::setParams(updateDownloads, (long)timeout);

  // check hosts' access permissions
  bool authNotice = checkAuthorizations(texSet);

  if (!referencing) {
    // Clear old cached texture
    // This is the first time is called after joining
    std::vector<CachedTexture*>::iterator itr;
    for (itr = cachedTexVector.begin(); itr != cachedTexVector.end(); ++itr) {
      delete (*itr);
    }
    cachedTexVector.clear();
  }

  if (doDownloads) {
    for (set_it = texSet.begin(); set_it != texSet.end(); set_it++) {
      const std::string& texUrl = set_it->c_str();
      if (CACHEMGR.isCacheFileType(texUrl)) {
	if (!referencing) {
	  MATERIALMGR.setTextureLocal(texUrl, "");
        }
	cachedTexVector.push_back(new CachedTexture(texUrl));
      }
    }
  }
  else {
    for (set_it = texSet.begin(); set_it != texSet.end(); set_it++) {
      const std::string& texUrl = set_it->c_str();
      if (CACHEMGR.isCacheFileType(texUrl)) {
	// use the cache?
	CacheManager::CacheRecord oldrec;
	if (CACHEMGR.findURL(texUrl, oldrec)) {
	  // use the cached file
	  MATERIALMGR.setTextureLocal(texUrl, oldrec.name);
	} else {
	  // bail here if we can't download
	  MATERIALMGR.setTextureLocal(texUrl, "");
	  std::string msg = ColorStrings[GreyColor];
	  msg += "not downloading: " + texUrl;
	  addMessage(NULL, msg);
	}
      }
    }
  }

  if (authNotice) {
    printAuthNotice();
  }

  textureDownloading = true;
}


void Downloads::finalizeDownloads()
{
  textureDownloading = false;

  std::vector<CachedTexture*>::iterator itr;
  for (itr = cachedTexVector.begin(); itr != cachedTexVector.end(); ++itr)
    delete (*itr);
  cachedTexVector.clear();

  CACHEMGR.saveIndex();
}


bool Downloads::requestFinalized()
{
  return textureDownloading && (CachedTexture::activeTransfer() == 0);
}


void Downloads::removeTextures()
{
  BzMaterialManager::TextureSet texSet;
  BzMaterialManager::TextureSet::iterator set_it;
  MATERIALMGR.makeTextureList(texSet, false /* ignore referencing */);

  TextureManager& TEXMGR = TextureManager::instance();

  for (set_it = texSet.begin(); set_it != texSet.end(); set_it++) {
    const std::string& texUrl = set_it->c_str();
    if (CACHEMGR.isCacheFileType(texUrl)) {
      const std::string& localname = CACHEMGR.getLocalName(texUrl);
      if (TEXMGR.isLoaded(localname)) {
	TEXMGR.removeTexture(localname);
      }
    }
  }

  return;
}


void Downloads::printAuthNotice()
{
  std::string msg = ColorStrings[WhiteColor];
  msg += "NOTE: ";
  msg += ColorStrings[GreyColor];
  msg += "download access is controlled by ";
  msg += ColorStrings[YellowColor];
  msg += downloadAccessList->getFileName();
  addMessage(NULL, msg);
  return;
}


bool Downloads::authorizedServer(const std::string& hostname)
{
  // Don't do here a DNS lookup, it can block the client
  // DNS is temporary removed until someone code it unblocking

  // make the list of strings to check
  std::vector<std::string> nameAndIp;
  if (hostname.size() > 0) {
    nameAndIp.push_back(hostname);
  }

  return downloadAccessList->authorized(nameAndIp);
}


bool parseHostname(const std::string& url, std::string& hostname)
{
  std::string protocol, path, ip;
  int port;
  if (BzfNetwork::parseURL(url, protocol, hostname, port, path)) {
    if ((protocol == "http") || (protocol == "ftp")) {
      return true;
    }
  }
  return false;
}


bool Downloads::checkAuthorizations(BzMaterialManager::TextureSet& texSet)
{
  // avoid the DNS lookup
  if (downloadAccessList->alwaysAuthorized()) {
    return false;
  }

  bool hostFailed = false;

  BzMaterialManager::TextureSet::iterator set_it;

  std::map<std::string, bool> hostAccess;
  std::map<std::string, bool>::iterator host_it;

  // get the list of hosts to check
  for (set_it = texSet.begin(); set_it != texSet.end(); set_it++) {
    const std::string& url = *set_it;
    std::string hostname;
    if (parseHostname(url, hostname)) {
      hostAccess[hostname] = true;
    }
  }

  // check the hosts
  for (host_it = hostAccess.begin(); host_it != hostAccess.end(); host_it++) {
    const std::string& host = host_it->first;
    host_it->second = authorizedServer(host);
  }

  // clear any unauthorized urls
  set_it = texSet.begin();
  while (set_it != texSet.end()) {
    BzMaterialManager::TextureSet::iterator next_it = set_it;
    next_it++;
    const std::string& url = *set_it;
    std::string hostname;
    if (parseHostname(url, hostname) && !hostAccess[hostname]) {
      hostFailed = true;
      // send a message
      std::string msg = ColorStrings[RedColor];
      msg += "local denial: ";
      msg += ColorStrings[GreyColor];
      msg += url;
      addMessage(NULL, msg);
      // remove the url
      MATERIALMGR.setTextureLocal(url, "");
      texSet.erase(set_it);
    }
    set_it = next_it;
  }

  return hostFailed;
}


// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
