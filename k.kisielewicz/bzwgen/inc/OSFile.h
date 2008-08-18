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



#ifndef _OSFILE_H_
#define _OSFILE_H_


#ifdef _WIN32
  #pragma warning( disable : 4786 )  // Disable warning message
  #define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
  #include <windows.h>
  #include <io.h>
  #include <direct.h>
#else
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include <dirent.h>
  #include <ctype.h>
#endif

#include <string>
#include <vector>

#include <stdio.h>

typedef enum 
{
	eFileStart,
	eCurentPos,
	eFileEnd
}teFilePos;

void SetOSFileBaseDir ( const char *szDir );

class COSFile
{
public:
	COSFile ();
	COSFile ( const COSFile &r);
	COSFile& operator = (const COSFile &r);

	COSFile ( const char *szName );
	COSFile ( const char *szName, const char *szMode );
	~COSFile();

	bool Open ( const char *szName, const char *szMode );
	bool Open ( const char *szMode );
	bool Close ( void );

	void StdName ( const char *szName );
	void OSName ( const char *szName );

	FILE* GetFile ( void );

	const char* GetStdName ( void );
	const char* GetOSName ( void );

	const char* GetFileTitle ( void );

	const char* GetExtension ( void );
	
	const char* GetFullOSPath ( void );

	bool IsOpen ( void );

	int Read ( void* data, int size, int count = 1 );
	unsigned char ReadChar ( void );
	int ScanChar ( unsigned char *pChar );
	const char* ScanStr ( void );
	const char* ReadLine ( void );
	int Write ( const void* data, int size );
	void Write ( const char* data );
	void Write ( std::string data );
	void Flush ( void );

	std::vector<std::string> ParseFile ( std::string delim );

	int Seek ( teFilePos ePos, int iOffset );
	unsigned int Size ( void );
	unsigned int Tell ( void );

	bool GetFileText ( std::string &text );

	void SetUseGlobalPath ( bool use = false );
protected:
	class COSFileInfo;
	COSFileInfo		*info;
};


class COSDir
{
public:
	COSDir();
	COSDir( const COSDir &r);
	COSDir& operator = (const COSDir &r);
	explicit COSDir( const char* szDirName );
	~COSDir();

	void SetStdDir ( const char* szDirName );
	void SetOSDir ( const char* szDirName );

	void MakeStdDir ( const char* szDirName );
	void MakeOSDir ( const char* szDirName );

	bool GetNextFile ( COSFile &oFile, bool bRecursive );
	bool GetNextFile ( COSFile &oFile, const char* fileMask, bool bRecursive );

	const char* GetStdName ( void );
	const char* GetOSName ( void );
	const char* GetFullOSPath ( void );

protected:
	class COSDirInfo;
	COSDirInfo		*info;

	bool WindowsAddFileStack ( const char *szPathName, const char* fileMask , bool bRecursive );
	bool LinuxAddFileStack( const char *szPathName, const char* fileMask , bool bRecursive);
};


#endif//_OSFILE_H_

