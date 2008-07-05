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

#include "commandArgs.h"
#include "TextUtils.h"

inline  bool operator < (const std::string &s1,const std::string &s2) { return (s1.compare(s2)<0);}

std::string GetCommandName ( std::string name )
{
	return TextUtils::toupper(name);
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

CCommandLineArgs::CCommandLineArgs( const COSFile &file )
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

	std::vector<std::string> params = TextUtils::tokenize(std::string(szCommandLine),deliminator);

	for ( int i = 0; i < (int) params.size(); i++ )
	{
		std::string param = params[i];
		if (param.size())
		{
			if (strrchr(param.c_str(),' ') == NULL)
				commands[GetCommandName(param)] = std::string("1");
			else
			{
				std::string key = TextUtils::tokenize(param,std::string(" "))[0];
				std::string data = param.c_str()+1+key.size();
				data = TextUtils::trim_whitespace(data);
				commands[GetCommandName(key)] = data;
			}
		}
	}
}

void CCommandLineArgs::Set ( const COSFile &_file )
{
	// read in a file or something

	std::string key;
	std::string data;

	COSFile	file = _file;
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
			key = GetCommandName(key);
			commands[key] = data;
			data.clear();
			key.clear();
		}
		else
			data = c;
	}

	if ((key.size() > 0) )
	{
		data.erase(0,1);
		commands[GetCommandName(key)] = data;
	}
	file.Close();
}

void CCommandLineArgs::Clear ( void )
{
	deliminator = "-";
	commands.clear();
}

bool CCommandLineArgs::Exists ( const char* szKey )
{
	return (commands.find(GetCommandName(std::string(szKey))) != commands.end());
}

const char* CCommandLineArgs::GetDataS ( const char* szKey )
{
	tmCommandMap::iterator itr = commands.find(GetCommandName(std::string(szKey)));

	if(itr == commands.end())
		return false;

	return itr->second.c_str();
}

int CCommandLineArgs::GetDataI ( const char* szKey )
{
	tmCommandMap::iterator itr = commands.find(GetCommandName(std::string(szKey)));

	if(itr == commands.end())
		return false;

	return atoi(itr->second.c_str());
}

bool CCommandLineArgs::GetDataB ( const char* szKey )
{
	tmCommandMap::iterator itr = commands.find(GetCommandName(std::string(szKey)));

	if(itr == commands.end())
		return false;

	return itr->second.c_str()[0] != '0';
}

double CCommandLineArgs::GetDataF ( const char* szKey )
{
	tmCommandMap::iterator itr = commands.find(GetCommandName(std::string(szKey)));

	if(itr == commands.end())
		return false;

	return (double)atof(itr->second.c_str());
}
