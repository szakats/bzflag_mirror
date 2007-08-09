// GPx01
// Copyright (c) 2005 - 2006 Jeff Myers
//
// This package is free software;  you can redistribute it and/or
// modify it under the terms of the license found in the file
// named COPYING that should have accompanied this file.
//
// THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//


#include "commandArgs.h"
#pragma warning(disable:4996)
inline  bool operator < (const std::string &s1,const std::string &s2) { return (s1.compare(s2)<0);}

void GetCommandName ( char *szData )
{
#ifdef	_WIN32
	strupr(szData);
#else
	while(*szData++)
		*szData = toupper(*szData);
#endif
}

CCommandLineArgs::CCommandLineArgs()
{
	Clear();
}

CCommandLineArgs::~CCommandLineArgs()
{
	Clear();
}

CCommandLineArgs::CCommandLineArgs( int argc, char *argv[] )
{
	Clear();
	Set(argc,argv);
}

CCommandLineArgs::CCommandLineArgs( const char* szCommandLine )
{
	Clear();
	Set(szCommandLine);
}

CCommandLineArgs::CCommandLineArgs( COSFile &file )
{
	Clear();
	Set(file);
}

void CCommandLineArgs::Set ( int argc, char *argv[] )
{
	std::string str;

	for (int i = 1; i < argc; i++)
	{
		str += argv[i];
		if ( i != argc-1)
			str += " ";
	}
	Set(str.c_str());
}

void CCommandLineArgs::Set ( const char* szCommandLine )
{
	if (!szCommandLine || !szCommandLine[0])
		return;

	std::string key;
	std::string data;
	
	char szTemp[512];
	const char *pTemp;

	int pos = 0;
	int len = (int)strlen(szCommandLine);

	while (pos < len)
	{
		pTemp = strchr(&szCommandLine[pos],'-');
		if (!pTemp)
			pos = len;
		else
		{
			pos = (int)(pTemp-szCommandLine);
			pos += 1;

			pTemp=strchr(pTemp+1,' ');
			if (pTemp)
			{
				strncpy(szTemp,&szCommandLine[pos],pTemp-&szCommandLine[pos]);
				szTemp[pTemp-&szCommandLine[pos]]= '\0';

				GetCommandName(szTemp);
				key=szTemp;

				pos = (int)(pTemp-szCommandLine+1);
				
				pTemp=strchr(pTemp+1,'-');	
				if (!pTemp)
				{	// we are done
					pTemp = (char*)&szCommandLine[len];
					strncpy(szTemp,&szCommandLine[pos],pTemp-&szCommandLine[pos]);
					szTemp[pTemp-&szCommandLine[pos]]= '\0';
					pos = len;
				}
				else
				{	// we are not done
					if (pTemp-&szCommandLine[pos] != 0)
					{
						strncpy(szTemp,&szCommandLine[pos],pTemp-&szCommandLine[pos]-1);
						szTemp[pTemp-&szCommandLine[pos]-1]= '\0';
						pos = (int)(pTemp-szCommandLine);
					}
					else
						szTemp[0] = '\0';
				}
				
				data = szTemp;
				commands[key]=data;
			}
		}
	}
}

void CCommandLineArgs::Set ( COSFile &file )
{
	// read in a file or something

	std::string key;
	std::string data;

	unsigned char c;

	file.Open("rt");
	while (file.ScanChar(&c) != EOF)
	{
		if ( (c != ' ') && (c != '\n'))
		{
			if (data.size()>0)
				data+=c;
			else
				key += c;
		}
		else if (c == '\n')
		{
			data.erase(0,1);
			GetCommandName((char*)key.c_str());
			commands[key] = data;
			data.erase(0,data.size());
			key.erase(0,key.size());
		}
		else
			data = c;
	}

	if ((key.size() > 0) )
	{
		data.erase(0,1);
		GetCommandName((char*)key.c_str());
		commands[key] = data;
	}
	file.Close();
}

void CCommandLineArgs::Clear ( void )
{
	commands.clear();
}

bool CCommandLineArgs::Exists ( const char* szKey )
{
	std::string key = szKey;

	GetCommandName((char*)key.c_str());

	tmCommandMap::iterator itr = commands.find(key);

	return (itr != commands.end());
}

const char* CCommandLineArgs::GetDataS ( const char* szKey )
{
	std::string key = szKey;

	GetCommandName((char*)key.c_str());

	tmCommandMap::iterator itr = commands.find(key);

	if(itr == commands.end())
		return false;

	return itr->second.c_str();

}

int CCommandLineArgs::GetDataI ( const char* szKey )
{
	std::string key = szKey;

	GetCommandName((char*)key.c_str());

	tmCommandMap::iterator itr = commands.find(key);

	if(itr == commands.end())
		return false;

	return atoi(itr->second.c_str());
}

bool CCommandLineArgs::GetDataB ( const char* szKey )
{
	std::string key = szKey;

	GetCommandName((char*)key.c_str());

	tmCommandMap::iterator itr = commands.find(key);

	if(itr == commands.end())
		return false;

	return itr->second.c_str()[0] != '0';

}

float CCommandLineArgs::GetDataF ( const char* szKey )
{
	std::string key = szKey;

	GetCommandName((char*)key.c_str());

	tmCommandMap::iterator itr = commands.find(key);

	if(itr == commands.end())
		return false;

	return (float)atof(itr->second.c_str());

}
