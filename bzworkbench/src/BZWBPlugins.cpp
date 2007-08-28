/* BZWorkbench
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

#include "BZWBPlugins.h"
#include "TextUtils.h"
#include "OSFile.h"
#include <map>

#ifdef _WIN32
std::string extension = ".dll";
std::string globalPluginDir = ".\\plugins\\";
#else
std::string extension = ".so";
std::string globalPluginDir = "$(prefix)/lib/bzwb/";
#endif

typedef std::map<std::string, bzwb_APIPluginHandler*> tmCustomPluginMap;
tmCustomPluginMap customPluginMap;

typedef struct
{
	std::string foundPath;
	std::string plugin;

#ifdef _WIN32
	HINSTANCE	handle;
#else
	void*		handle;
#endif
}trPluginRecord;

std::vector<trPluginRecord>	vPluginList;

typedef enum
{
	eLoadFailedDupe = -1,
	eLoadFailedError = 0,
	eLoadComplete
}PluginLoadReturn;

bool pluginExists ( std::string plugin )
{
	for ( int i = 0; i < (int)vPluginList.size(); i++ )
	{
		if ( vPluginList[i].foundPath == plugin )
			return true;
	}
	return false;
}

std::string findPlugin ( std::string pluginName )
{
	// see if we can just open the bloody thing
	FILE	*fp = fopen(pluginName.c_str(),"rb");
	if (fp)
	{
		fclose(fp);
		return pluginName;
	}

	// now try it with the standard extension
	std::string name = pluginName + extension;
	fp = fopen(name.c_str(),"rb");
	if (fp)
	{
		fclose(fp);
		return name;
	}

	// check the global plugins dir
	name = globalPluginDir + pluginName + extension;
	fp = fopen(name.c_str(),"rb");
	if (fp)
	{
		fclose(fp);
		return name;
	}

	return std::string("");
}

void unload1Plugin ( int iPluginID );

#ifdef _WIN32
#  include <windows.h>

int getPluginVersion ( HINSTANCE hLib )
{
	int (*lpProc)(void);
	lpProc = (int (__cdecl *)(void))GetProcAddress(hLib, "bzwb_GetVersion");
	if (lpProc)
		return lpProc();
	return 0;
}

PluginLoadReturn load1Plugin ( std::string plugin, std::string config )
{
	int (*lpProc)(const char*);

	std::string realPluginName = findPlugin(plugin);
	if (pluginExists(realPluginName))
	{
		printf("LoadPlugin fialed:%s is already loaded\n",realPluginName.c_str());
		return eLoadFailedDupe;
	}

	HINSTANCE	hLib = LoadLibraryA(realPluginName.c_str());
	if (hLib)
	{
		if (getPluginVersion(hLib) > BZWB_API_VERSION)
		{
			printf("Plugin:%s found but expects an newer API version (%d), upgrade your application\n",plugin.c_str(),getPluginVersion(hLib));
			FreeLibrary(hLib);
			return eLoadFailedError;
		}
		else
		{
			lpProc = (int (__cdecl *)(const char*))GetProcAddress(hLib, "bzwb_Load");
			if (lpProc)
			{
				lpProc(config.c_str());
				printf("Plugin:%s loaded\n",plugin.c_str());

				trPluginRecord pluginRecord;
				pluginRecord.foundPath = realPluginName;
				pluginRecord.handle = hLib;
				pluginRecord.plugin = plugin;
				vPluginList.push_back(pluginRecord);
			}
			else
			{
				printf("Plugin:%s found but does not contain bzwb_Load method\n",plugin.c_str());
				FreeLibrary(hLib);
				return eLoadFailedError;
			}
		}
	}
	else
	{
		printf("Plugin:%s not found\n",plugin.c_str());
		return eLoadFailedError;
	}

	return eLoadComplete;
}

void unload1Plugin ( int iPluginID )
{
	int (*lpProc)(void);

	trPluginRecord &plugin = vPluginList[iPluginID];
	if (!plugin.handle)
		return;

	lpProc = (int (__cdecl *)(void))GetProcAddress(plugin.handle, "bzwb_Unload");
	if (lpProc)
		lpProc();
	else
		printf("Plugin does not contain bzwb_UnLoad method\n");

	FreeLibrary(plugin.handle);
	plugin.handle = NULL;
	plugin.foundPath = "";
	plugin.plugin = "";
}
#else

#  include <dlfcn.h>
std::vector<void*>	vLibHandles;

int getPluginVersion ( void* hLib )
{
	int (*lpProc)(void);

	lpProc = force_cast<int (*)(void)>(dlsym(hLib,"bzwb_GetVersion"));
	if (lpProc)
		return (*lpProc)();
	return 0;
}

PluginLoadReturn load1Plugin ( std::string plugin, std::string config )
{
	int (*lpProc)(const char*);

	std::string realPluginName = findPlugin(plugin);

	if (pluginExists(realPluginName))
	{
		printf("LoadPlugin fialed:%s is already loaded\n",realPluginName.c_str());
		return eLoadFailedDupe;
	}

	void *hLib = dlopen(realPluginName.c_str(), RTLD_LAZY | RTLD_GLOBAL);
	if (hLib)
	{
		if (dlsym(hLib, "bzwb_Load") == NULL) {
			printf("Plugin:%s found but does not contain bzwb_Load method, error %s\n",plugin.c_str(),dlerror());
			dlclose(hLib);
			return eLoadFailedError;
		}

		int version = getPluginVersion(hLib);
		if (version < BZWB_API_VERSION)
		{
			printf("Plugin:%s found but expects an older API version (%d), upgrade it\n", plugin.c_str(), version);
			dlclose(hLib);
			return eLoadFailedError;
		}
		else
		{
			lpProc = force_cast<int (*)(const char*)>(dlsym(hLib,"bzwb_Load"));
			if (lpProc)
			{
				(*lpProc)(config.c_str());
				printf("Plugin:%s loaded\n",plugin.c_str());
				trPluginRecord pluginRecord;
				pluginRecord.handle = hLib;
				pluginRecord.plugin = plugin;
				vPluginList.push_back(pluginRecord);
				return eLoadComplete;
			}
		}
	}
	else
	{
		printf("Plugin:%s not found, error %s\n",plugin.c_str(), dlerror());
		return eLoadFailedError;
	}

	printf(,"If you see this, there is something terribly wrong.\n");
	return eLoadFailedError;
}

void unload1Plugin ( int iPluginID )
{
	int (*lpProc)(void);
	trPluginRecord &plugin = vPluginList[iPluginID];

	if(!plugin.handle)
		return;

	lpProc = force_cast<int (*)(void)>(dlsym(plugin.handle, "bzwb_Unload"));
	if (lpProc)
		(*lpProc)();
	else
		printf("Plugin does not contain bzwb_UnLoad method, error %s\n",dlerror());

	dlclose(plugin.handle);
	plugin.handle = NULL;
	plugin.foundPath = "";
	plugin.plugin = "";

}
#endif


bool loadPlugin ( std::string plugin, std::string config )
{
	// check and see if it's an extension we have a handler for
	std::string ext;

	std::vector<std::string> parts = TextUtils::tokenize(plugin,std::string("."));
	ext = parts[parts.size()-1];

	tmCustomPluginMap::iterator itr = customPluginMap.find(TextUtils::tolower(ext));

	if (itr != customPluginMap.end() && itr->second)
	{
		bzwb_APIPluginHandler *handler = itr->second;
		return handler->handle(plugin.c_str(),config.c_str());
	}
	else
		return load1Plugin(plugin,config) == eLoadComplete;
}

bool unloadPlugin ( std::string plugin )
{
	// unload the first one of the name we find
	for (unsigned int i = 0; i < vPluginList.size();i++)
	{
		if ( vPluginList[i].plugin == plugin )
		{
			unload1Plugin(i);
			vPluginList.erase(vPluginList.begin()+i);
			return true;
		}
	}
	return false;
}

void unloadPlugins ( void )
{
	for (unsigned int i = 0; i < vPluginList.size();i++)
		unload1Plugin(i);
	vPluginList.clear();
}

std::vector<std::string> getPluginList ( void )
{
	std::vector<std::string> plugins;
	for (unsigned int i = 0; i < vPluginList.size();i++)
		plugins.push_back(vPluginList[i].plugin);

	return plugins;
}

// auto load plugin dir

std::string getAutoLoadDir ( void )
{
#if (defined(_WIN32) || defined(WIN32))
	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL,exePath,MAX_PATH);
	char* last = strrchr(exePath,'\\');
	if (last)
		*last = '\0';
	strcat(exePath,"\\plugins");
	return std::string(exePath);
#else
	return globalPluginDir;
#endif
}

void initPlugins ( void )
{
	customPluginMap.clear();

	OSDir	dir;
	std::string path = getAutoLoadDir();
	if (getAutoLoadDir().size())
	{
		dir.setOSDir(getAutoLoadDir());

		OSFile file;
		std::string searchdir = "*" + extension;
		while(dir.getNextFile(file,searchdir.c_str(),false) )
			loadPlugin(file.getOSName(),std::string(""));
	}
}

bool registerCustomPluginHandler ( std::string exte, bzwb_APIPluginHandler *handler )
{
	std::string ext = TextUtils::tolower(exte);
	customPluginMap[ext] = handler;
	return true;
}

bool removeCustomPluginHandler ( std::string ext, bzwb_APIPluginHandler *handler )
{
	tmCustomPluginMap::iterator itr = customPluginMap.find(TextUtils::tolower(ext));

	if (itr == customPluginMap.end() || itr->second != handler)
		return false;

	customPluginMap.erase(itr);
	return true;
}

