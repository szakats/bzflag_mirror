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


#ifndef _COMMAND_ARGS_H_
#define _COMMAND_ARGS_H_

#ifdef _WIN32
#pragma warning( disable : 4181 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4503 )
#endif //_WIN32

#include <map>
#include <string>
#include "OSFile.h"

class CCommandLineArgs
{
public:
	CCommandLineArgs();
	~CCommandLineArgs();
	CCommandLineArgs( int argc, char *argv[] );
	CCommandLineArgs( const char* szCommandLine );
	CCommandLineArgs( const COSFile &file );

	void SetDelimnator ( std::string d ){deliminator = d;}

	void Set ( int argc, char *argv[] );
	void Set ( const char* szCommandLine );
	void Set ( const COSFile &file );

	void Clear ( void );

	bool Exists ( const char* szKey );
	
	const char* GetDataS ( const char* szKey );
	int GetDataI ( const char* szKey );
	bool GetDataB ( const char* szKey );
	double GetDataF ( const char* szKey );

private:
	typedef std::map<std::string,std::string> tmCommandMap;
	tmCommandMap		commands;
	std::string			deliminator;
};
#endif//_COMMAND_ARGS_H_

