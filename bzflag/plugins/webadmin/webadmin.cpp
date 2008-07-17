// webadmin.cpp : Defines the entry point for the DLL application.
//

#include "bzfsAPI.h"
#include "plugin_utils.h"
#include "plugin_HTTP.h"
#include <fstream>
#include <cstring>
#include <algorithm>

class WebAdmin : public BZFSHTTPAuth, TemplateCallbackClass
{
public:
  WebAdmin();
  virtual const char * getVDir ( void ){return "webadmin";}
  virtual const char * getDescription ( void ){return "Server Administration (Login Required)";}

  void init (const char *tDir);

  virtual bool handleAuthedRequest ( int level, const HTTPRequest &request, HTTPReply &reply );
 
  // from TemplateCallbackClass
  virtual void keyCallback (std::string &data, const std::string &key);
  virtual bool loopCallback (const std::string &key);
  virtual bool ifCallback (const std::string &key);

private:
  unsigned int loopPos;
  std::map<std::string,std::string> templateVars;
  
  typedef void (WebAdmin::*page_callback)(const HTTPRequest &);
  std::map<std::string,page_callback> controllers;
  std::vector<std::string> pagenames;
  
  void mainPageCallback (const HTTPRequest &request);
  void banlistPageCallback (const HTTPRequest &request);
  void groupPageCallback (const HTTPRequest &request);
  
  bz_APIIntList players;
  bz_APIStringList *stringList;
  int listSize;
  
  bool editing, checked;
};

WebAdmin *webAdmin = NULL;

BZ_GET_PLUGIN_VERSION

BZF_PLUGIN_CALL int bz_Load(const char* commandLine)
{
  if(webAdmin)
    delete(webAdmin);
  webAdmin = new WebAdmin;
  webAdmin->init(commandLine);

  bz_debugMessage(4,"webadmin plugin loaded");
  return 0;
}

BZF_PLUGIN_CALL int bz_Unload(void)
{
  if(webAdmin)
    delete(webAdmin);
  webAdmin = NULL;

  bz_debugMessage(4,"webadmin plugin unloaded");
  return 0;
}

WebAdmin::WebAdmin():BZFSHTTPAuth(),loopPos(0)
{  
	registerVDir();
	
	// add new pages here
	controllers["main"] = &WebAdmin::mainPageCallback;
	controllers["banlist"] = &WebAdmin::banlistPageCallback;
	controllers["helpmsg"] = NULL;
	controllers["group"] = &WebAdmin::groupPageCallback;
	
  std::map<std::string,page_callback>::iterator pair;
  for(pair = controllers.begin(); pair != controllers.end(); pair++)
    pagenames.push_back(pair->first);
}


void WebAdmin::init(const char* cmdln)
{
  templateSystem.addSearchPath(cmdln ? cmdln : "./");

  // level one has admin perms
  addPermToLevel(1,"ADMIN");
  
  templateSystem.addIF("IsCurrentPage",this);
  templateSystem.addIF("Error",this);
  templateSystem.addIF("Editing",this);
  templateSystem.addIF("Checked",this);

  templateSystem.addKey("Error",this);
  templateSystem.addKey("Callsign",this);
  templateSystem.addKey("BannedUser",this);
  templateSystem.addKey("PageName",this);
  templateSystem.addKey("HelpMsgName",this);
  templateSystem.addKey("HelpMsgBody",this);
  templateSystem.addKey("GroupName",this);
  
  templateSystem.addLoop("Navigation",this);
  templateSystem.addLoop("Players",this);
  templateSystem.addLoop("IPBanList",this);
  templateSystem.addLoop("IDBanList",this);
  templateSystem.addLoop("HelpMsgs",this);
  templateSystem.addLoop("Groups",this);

  templateSystem.setPluginName("webadmin", getBaseURL().c_str());

  setupAuth();
}

// event hook for [$Something] in templates
void WebAdmin::keyCallback (std::string &data, const std::string &key)
{
  const std::map<std::string,std::string>::iterator &pair = templateVars.find(key);
  if (pair != templateVars.end())
		data = pair->second;
}

// condition check for [*START] in templates
bool WebAdmin::loopCallback (const std::string &key)
{
  if (key == "players") {
    if (!loopPos) {
      bz_getPlayerIndexList(&players);
      listSize = players.size();
    }
    else if (loopPos < listSize) {
      templateVars["playerid"] = players[loopPos];
      templateVars["callsign"] = bz_getPlayerCallsign(players[loopPos++]);
      return true;
    } else {
      players.clear();
      return loopPos = 0;
    }
  } else if (key == "navigation") {
    if (!loopPos) listSize = pagenames.size();
    if (loopPos < pagenames.size()) {
      templateVars["pagename"] = pagenames[loopPos++];
      return true;
    } else return loopPos = 0;
  } else if (key == "permissions") {
    if (!loopPos) listSize = bzu_standardPerms().size();
    if (loopPos < listSize) {
      const std::string &perm = bzu_standardPerms()[loopPos++];
      if (stringList) checked = stringList->contains(perm);
      templateVars["permission"] = perm;
      return true;
    } else return loopPos = 0;
  } else if (key == "helpmsgs") {
    if (!loopPos) {
      stringList = bz_getHelpTopics();
      listSize = stringList->size();
    }
    if (loopPos < listSize) {
      templateVars["helpmsgname"] = (*stringList)[loopPos++].c_str();
      return true;
    } else {
      delete(stringList);
      return loopPos = 0;
    }
  } else if (key == "groups") {
    if (!loopPos) {
      stringList = bz_getGroupList();
      listSize = stringList->size();
    }
    if (loopPos < listSize) {
      templateVars["groupname"] = (*stringList)[loopPos++].c_str();
      return true;
    } else {
      delete(stringList);
      return loopPos = 0;
    }
  } else if (key == "servervars") {
    if (!loopPos) listSize = bz_getBZDBVarList(stringList);
    if (loopPos < listSize) {
      const char *varname = (*stringList)[loopPos++].c_str();
      templateVars["servervarname"] = varname;
      templateVars["servervarvalue"] = bz_getBZDBString(varname).c_str();
    } else {
      delete(stringList);
      return loopPos = 0;
    }
  } else return false;
}

// condition check for [?IF] in templates
bool WebAdmin::ifCallback (const std::string &key)
{
  if (key == "iscurrentpage")
    return templateVars["pagename"] == templateVars["currentpage"];
  if (key == "editing")
    return editing;
  if (key == "checked")
    return checked;
  return false;
}

bool WebAdmin::handleAuthedRequest ( int level, const HTTPRequest &request, HTTPReply &reply )
{
  std::map<std::string,page_callback>::iterator pair;
  size_t last;
  std::string pagename = request.resource;

  switch(level) {
  case 1:
  case VERIFIED:
    last = pagename.size()-1;
    if (pagename[last] == '/') pagename.erase(last);
    if (pagename.empty()) pagename = "main";
    pair = controllers.find(pagename);
    if (pair != controllers.end()) {
      (this->*pair->second)(request);
      if (!templateSystem.processTemplateFile(reply.body, (pagename + ".tmpl").c_str())) {
        reply.returnCode = HTTPReply::e500ServerError;
          if (!templateSystem.processTemplateFile(reply.body, "500.tmpl"))
            reply.body = format("Missing template: %s.tmpl", pagename.c_str());
      }
    } else {
      reply.returnCode = HTTPReply::e404NotFound;
      if (!templateSystem.processTemplateFile(reply.body, "404.tmpl"))
        reply.body = format("No such resource: %s", pagename.c_str());
    }
    break;
  //reply.body = format("Not authenticated(Verified) sessionID %d",request.sessionID);
  default:
    reply.body = format("Not authenticated sessionID %d, access level %d",request.sessionID,level);
  }

  reply.docType = HTTPReply::eHTML;

  templateVars.clear();
  return true;
}

void WebAdmin::mainPageCallback (const HTTPRequest &request)
{
  std::string s1, s2;
  if (request.request != ePost) return;
  std::vector<std::string> v;
  // kick/ban players
  if (request.getParam("players", v)) {
    bool notify = request.getParam("notify", s1);
    request.getParam("reason", s2);
    std::vector<std::string>::iterator i;
    if (request.getParam("kick", s1))
      for (i = v.begin(); i != v.end(); i++)
        bz_kickUser(atoi(i->c_str()), s2.c_str(), notify);
    else if (request.getParam("ipban", v)) {
      request.getParam("duration", s1);
      int duration = atoi(s1.c_str());
      for (i = v.begin(); i != v.end(); i++) {
        int playerID = atoi(i->c_str());
        bz_BasePlayerRecord *player = bz_getPlayerByIndex(playerID);
        bz_IPBanUser(playerID, bz_getPlayerIPAddress(playerID), duration, s2.c_str());
      }
    }
    else if (request.getParam("idban", v)) {
      request.getParam("duration", s1);
      int duration = atoi(s1.c_str());
      for (i = v.begin(); i != v.end(); i++) {
        int playerID = atoi(i->c_str());
        bz_BasePlayerRecord *player = bz_getPlayerByIndex(playerID);
        bz_IPBanUser(playerID, bz_getPlayerCallsign(playerID), duration, s2.c_str());
      }
    }
  }
  // update server vars
  stringList = new bz_APIStringList;
  listSize = bz_getBZDBVarList(stringList);
  for (loopPos = 0; loopPos < listSize; loopPos++) {
    s1 = "var";
    s1 += (*stringList)[loopPos].c_str();
    if (request.getParam(s1, s2))
      bz_setBZDBString(s1.c_str(), s2.c_str());
  }
}

void WebAdmin::banlistPageCallback (const HTTPRequest &request)
{
  if (request.request != ePost) return;
  std::vector<std::string> banRemovals;
  std::vector<std::string>::iterator i;
  if (request.getParam("delip", banRemovals))
    for(i = banRemovals.begin(); i != banRemovals.end(); i++)
      bz_IPUnbanUser(i->c_str());
  if (request.getParam("delid", banRemovals))
    for(i = banRemovals.begin(); i != banRemovals.end(); i++)
      bz_IDUnbanUser(i->c_str()); 
}

void WebAdmin::groupPageCallback (const HTTPRequest &request)
{
  
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=2 expandtab
