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


// OSFile.cpp : Defines the entry point for the DLL application.
//

#include "OSFile.h"
#include "utils.h"

#ifdef _WIN32
  #define strcasecmp stricmp
  #pragma warning( disable : 4786 )  // Disable warning message
#else
  #include <stddef.h>
#endif

void GetUperName ( char *szData )
{
#ifdef	_WIN32
	_strupr(szData);
#else
	while(*szData) {
		*szData = toupper(*szData);
		szData++;
    }
#endif
}

typedef std::vector<std::string> tvFileNameList;

// these are the core file name conversions
// std pathnames are unix style
// if you are adding a platform
// this is one place you need to add conversions to/from your OS

// util functions
void OSFileStdToOSDir ( char *dir )
{
#ifndef _WIN32
	return;
#else
	char *p = dir;

	while ( *p != NULL )
	{
		if (*p == '/')
			*p = '\\';

		p++;
	}
#endif//WIN32
}

void OSFileOSToStdDir ( char *dir )
{
#ifndef _WIN32
	return;
#else
	char *p = dir;

	while ( *p != NULL )
	{
		if (*p == '\\')
			*p= '/';

		p++;
	}
#endif//WIN32
}

// windows specific make path
#ifdef _WIN32
void WindowsMakePath(const char * path)
{
	// scan the folder path and make it
	const char *pos = strchr (path,'\\');

	if (!pos) // dono maybe the screwed up
	{
		std::string str = "\\";
		str += path;
		_mkdir(str.c_str());
		return;
	}

	while (pos)
	{
		std::string str = path;
		str.resize(pos-path);
		mkdir(str.c_str());
		pos = strchr (pos+1,'\\');
	}
}
#else 
void nixMakePath(const char * path)
{
	// scan the folder path and make it
	char *pos = strchr (path,'/');

	if (!pos) // dono maybe the screwed up
	{
		std::string str = "//";
		str += path;
		mkdir(str.c_str(), 0777);
		return;
	}

	while (pos)
	{
		std::string str = path;
		str.resize(pos-path);
		mkdir(str.c_str(), 0777);
		pos = strchr (pos+1,'/');
	}
}
#endif // windows


/// global make path
void OSMakePath(const char * path)
{
#ifdef _WIN32
	WindowsMakePath(path);
#else
	nixMakePath(path);
#endif // win32	
}


// global path stuff
std::string	stdBaseDir;
std::string	osBaseDir;

void SetOSFileBaseDir ( const char *dir )
{
	stdBaseDir = dir;
	if (!dir)
		osBaseDir.empty();
	else
	{
		osBaseDir = stdBaseDir;
		OSFileStdToOSDir((char*)osBaseDir.c_str());
	}
}

// OSFileClass
class COSFile::COSFileInfo
{
	public:
		COSFileInfo();
		COSFileInfo( const COSFile::COSFileInfo &r);
		std::string		stdName;
		std::string		osName;
		std::string		title;
		bool			useGlobalPath;
		FILE			*fp;
};

COSFile::COSFileInfo::COSFileInfo ( void )
{
	fp = NULL;
	osName = "";
	stdName = "";
	title = "";
	useGlobalPath = true;
}

COSFile::COSFileInfo::COSFileInfo( const COSFile::COSFileInfo &r)
{
	// never copy the file
	fp = NULL;
	osName = r.osName;
	stdName = r.stdName;
	title =  r.title;
	useGlobalPath = r.useGlobalPath;
}

void COSFile::SetUseGlobalPath ( bool use )
{
	info->useGlobalPath = use;
}

COSFile::COSFile ()
{
	info = new COSFileInfo;
}

COSFile::COSFile ( const char *name )
{
	info = new COSFileInfo;
	StdName(name);
}

COSFile::COSFile ( const COSFile &r)
{
	if (this == &r)
		return;

	info = new COSFileInfo(*r.info);
}

COSFile& COSFile::operator = (const COSFile &r)
{
	if (this == &r)
		return *this;

	if (info)
		delete (info);

	info = new COSFileInfo(*r.info);
	return *this;
}

COSFile::COSFile ( const char *name, const char *mode   )
{
	info = new COSFileInfo;
	Open(name,mode);
}

COSFile::~COSFile()
{
	Close();
	if (info)
		delete (info);
}

bool COSFile::Delete ( void )
{
#ifdef _WIN32
	return DeleteFileA(GetFullOSPath()) != FALSE; 
#else
	// daniel dosn't remember
#endif
}

bool COSFile::Open ( const char *name, const char *mode  )
{
	Close();

	StdName(name);

	return Open(mode);
}

bool COSFile::Open ( const char *mode )
{
	Close();

	char	modeToUse[32];

	if (!mode)
		sprintf(modeToUse,"rb");
	else
		strcpy(modeToUse,mode);

	std::string	fileName;
	
	if (info->useGlobalPath)
		fileName = osBaseDir;
	fileName += info->osName;

	info->fp = fopen(fileName.c_str(),mode);

	// we may need to make the file path to the file, if we are writing then lets get on with it.
	if (!info->fp && strchr(mode,'w'))
	{
		OSMakePath(fileName.c_str());
		info->fp = fopen(fileName.c_str(),mode);
	}

	return IsOpen();
}

bool COSFile::Close ( void )
{
	if (info->fp)
		fclose(info->fp);

	info->fp = NULL;

	return (!IsOpen());
}

int COSFile::Read ( void* data, int size, int count )
{
	if (!IsOpen())
		return 0;

	return (int)fread(data,size,count,info->fp);
}

unsigned char  COSFile::ReadChar ( void )
{
	if (!IsOpen())
		return 0;

	char c = 0;

	if (fscanf(info->fp,"%c",&c) != 1)
		return 0;
	return c;
}

const char* COSFile::ReadLine ( void )
{
	static std::string line;
	
	line = "";
	char c = ReadChar();
	while (c != 0 && c != '\n' && c != 10)
	{
		line += c;
		c = ReadChar();
	}
	return line.c_str();
}

int COSFile::ScanChar ( unsigned char *pChar )
{
	if (!pChar || !IsOpen())
		return 0;

	return fscanf(info->fp,"%c",pChar);
}

const char* COSFile::ScanStr ( void )
{
	if (!IsOpen())
		return 0;
	
	static char temp[1024] = {0};
	if (fscanf(info->fp,"%s",temp)!=1)
		return NULL;
	return temp;
}

int  COSFile::Write ( const void* data, int size )
{
	if (!IsOpen())
		return 0;

	return (int)fwrite(data,size,1,info->fp);
}

void COSFile::Write ( const char* data )
{
	if (!IsOpen())
		return;

	fprintf(info->fp,"%s",data);
}

void COSFile::Write ( std::string data )
{
	Write(data.c_str(),(int)data.size());
}

void COSFile::Flush ( void )
{
	fflush(info->fp);
}

std::vector<std::string> COSFile::ParseFile ( std::string delim )
{
	std::vector<std::string>	fileSegments;
	if (!IsOpen())
		return fileSegments;

	int pos = Tell();
	int size = Size();
	char *data = (char*)malloc(size+1);
	data[size] = 0;
	Read(data,size);
	Seek(eFileStart,pos);

	std::string dataStr = data;
	free(data);

	fileSegments = tokenize(dataStr,delim,0,true);
	return fileSegments; 
}

int COSFile::Seek ( teFilePos ePos, int iOffset )
{
	if (!IsOpen())
		return 0;

	long iMode;
	switch(ePos)
	{
		case eFileStart:
			iMode = SEEK_SET;
			break;

		case eFileEnd:
			iMode = SEEK_END ;
			break;

		case eCurentPos:
		default:
			iMode = SEEK_CUR ;
			break;
	}

	return fseek(info->fp,iOffset,iMode);
}

unsigned int COSFile::Size ( void )
{
	if (!IsOpen())
		return 0;

	unsigned int pos = ftell(info->fp);
	fseek(info->fp,0,SEEK_END);
	unsigned int len = ftell(info->fp);
	fseek(info->fp,pos, SEEK_SET);
	
	return len;
}

unsigned int COSFile::Tell ( void )
{
	if (!IsOpen())
		return 0;
	return ftell(info->fp);
}

bool  COSFile::GetFileText ( std::string &text )
{
	bool opened = false;

	if ( !IsOpen() )
	{
		if (!Open("r"))
			return false;
		opened = true;
	}

	unsigned int pos = Tell();
	Seek(eFileStart,0);
	unsigned int size = Size();
	unsigned char *p = (unsigned char *)malloc(size+1);
	Read(p,size,1);
	p[size] = 0;
	text = (char*)p;
	free(p);

	if ( !opened )
		Seek(eFileStart,pos);
	else
		Close();

	return true;
}

void COSFile::StdName ( const char *name )
{
	info->stdName = name;
	info->osName = name;
	OSFileStdToOSDir((char*)info->osName.c_str());
}

void COSFile::OSName ( const char *name )
{
	info->stdName = name;
	info->osName = name;
	OSFileOSToStdDir((char*)info->stdName.c_str());
}

const char* COSFile::GetFileTitle ( void )
{
	if (info->stdName.size()== 0)
		return NULL;

	// yes I know this part is horible, put it in the pimple
	static std::string title;
	const char	*path = strrchr(info->stdName.c_str(),'/');

	if (path)
		path++;
	else
		path = info->stdName.c_str();

	title = path;

    // if no ext then we just give them the raw, as it may not have an extension
	const char *ext = GetExtension();
    if (ext)
	  title.resize(title.size() - strlen(ext) - 1);
    
	return title.c_str();
}

// this CAN return null, cus the file may not have an extenstion, if it just happens to end in a '.' then well, your really wierd Mr. File.
const char* COSFile::GetExtension ( void )
{
	if (info->stdName.size()== 0)
		return NULL;

	const char	*pEnd = strrchr(info->stdName.c_str(),'.');

	if (pEnd)
		pEnd++;

	return pEnd;
}

const char* COSFile::GetFullOSPath ( void )
{
	static std::string	szPath;
	
	szPath.empty();
	szPath = osBaseDir;
	szPath += info->osName;
	return szPath.c_str();
}

FILE* COSFile::GetFile ( void )
{
	return info->fp;
}

const char* COSFile::GetStdName ( void )
{
	return info->stdName.c_str();
}

const char* COSFile::GetOSName ( void )
{
	return info->osName.c_str();
}

bool COSFile::IsOpen ( void )
{
	return info->fp != NULL;
}

// OS Dir classes
class COSDir::COSDirInfo
{
public:
	COSDirInfo();
	COSDirInfo(const COSDir::COSDirInfo &r);

	std::string		baseStdDir;
	std::string		baseOSDir;
	tvFileNameList	nameList;
	unsigned int	namePos;
};

COSDir::COSDirInfo::COSDirInfo ( void )
{
	namePos = 0xFFFFFFFF;
}

COSDir::COSDirInfo::COSDirInfo ( const COSDir::COSDirInfo &r )
{
	if (this == &r)
		return;

	baseStdDir = r.baseStdDir;
	baseOSDir = r.baseOSDir;
	nameList = r.nameList;
	namePos = r.namePos;
}

COSDir::COSDir()
{
	info = new COSDirInfo;
}

COSDir::COSDir( const COSDir& r)
{
	info = new COSDirInfo(*r.info);
}

COSDir& COSDir::operator = (const COSDir& r)
{
	if (this == &r)
		return *this;

	if (info)
		delete(info);

	info = new COSDirInfo(*r.info);

	return *this;
}

COSDir::COSDir( const char* szDirName )
{
	info = new COSDirInfo;
	SetStdDir(szDirName);
}

COSDir::~COSDir()
{
	if (info)
	{
		info->nameList.clear();
		delete(info);
	}
	info = NULL;
}

void COSDir::SetStdDir ( const char* szDirName )
{
	info->baseStdDir = szDirName;
	info->baseOSDir = szDirName;
	OSFileStdToOSDir((char*)info->baseOSDir.c_str());
}

void COSDir::SetOSDir ( const char* szDirName )
{
	info->baseStdDir = szDirName;
	info->baseOSDir = szDirName;
	OSFileOSToStdDir((char*)info->baseStdDir.c_str());
}

void COSDir::MakeStdDir ( const char* szDirName )
{
	SetStdDir(szDirName);
#ifdef _WIN32
	mkdir(info->baseOSDir.c_str());
#else
	mkdir(info->baseOSDir.c_str(), 0777);
#endif
}

void COSDir::MakeOSDir ( const char* szDirName )
{
	SetOSDir(szDirName);
#ifdef _WIN32
	mkdir(info->baseOSDir.c_str());
#else
	mkdir(info->baseOSDir.c_str(), 0777);
#endif
}

bool COSDir::Delete ( bool recursive )
{
	if (recursive)
	{
	//	COSDir
	}
#ifdef _WIN32
	return RemoveDirectoryA(GetFullOSPath()) != FALSE; 
#else
	// daniel dosn't remember
#endif
}


const char* COSDir::GetStdName ( void )
{
	return info->baseStdDir.c_str();
}

const char* COSDir::GetOSName ( void )
{
	return info->baseOSDir.c_str();
}

const char* COSDir::GetFullOSPath ( void )
{
	static std::string	szFilePath;

	szFilePath.empty();
	szFilePath = osBaseDir; 

	szFilePath += info->baseOSDir;
	return szFilePath.c_str();
}

bool COSDir::GetNextFile ( COSFile &oFile, bool bRecursive )
{
	return GetNextFile(oFile,NULL,bRecursive);
}

bool COSDir::GetNextFile ( COSFile &oFile, const char* fileMask, bool bRecursive )
{
#ifdef _WIN32
	std::string realMask = "*.*";	//FIXME -- could this also be just '*' ?
#else
	std::string realMask = "*";
#endif
	if (fileMask)
		realMask = fileMask;

	GetUperName((char*)realMask.c_str());
	
	std::string theFileExt;
	if (info->namePos == 0xFFFFFFFF)
	{
		info->nameList.clear();
        //FIXME -- just do the #ifdef'ing here?
		WindowsAddFileStack(GetFullOSPath(),realMask.c_str(),bRecursive);
		LinuxAddFileStack(GetFullOSPath(),realMask.c_str(),bRecursive);

		info->namePos = 0;
	}
	
	unsigned int size = (int)info->nameList.size();
	if (info->namePos >= size)
	{
		info->namePos = 0xFFFFFFFF;
		return false;
	}

	std::string	fileName = info->nameList[info->namePos];

	if (osBaseDir.size()>1)
	{
		std::string temp = &(fileName.c_str()[osBaseDir.size()]);
		fileName = temp;
	}

	oFile.OSName(fileName.c_str());
	info->namePos++;

	return true;
}

bool COSDir::WindowsAddFileStack ( const char *szPathName, const char* fileMask, bool bRecursive   )
{
#ifdef _WIN32
	struct _finddata_t fileInfo;
	
	long		hFile;
	std::string searchstr;

	std::string FilePath;

	bool	bDone = false;

	searchstr = szPathName;
	searchstr += "\\";
	if (bRecursive)
		searchstr += "*.*";
	else if (fileMask)
		searchstr += fileMask;
	else
		searchstr += "*.*";

	std::string extenstionSearch;

	if ( fileMask && strchr(fileMask,'.'))
		extenstionSearch = strchr(fileMask,'.')+1;

	hFile = (long)_findfirst(searchstr.c_str(),&fileInfo);

	if (hFile != -1)
	{
		while (!bDone)
		{
			if ((strlen(fileInfo.name) >0) && (strcmp(fileInfo.name,".") != 0) && 
                (strcmp(fileInfo.name,"..") != 0))
			{
				FilePath = szPathName;
				FilePath += "\\";
				FilePath += fileInfo.name;

				if ( (fileInfo.attrib & _A_SUBDIR ) && bRecursive)
					WindowsAddFileStack(FilePath.c_str(),fileMask,bRecursive);
				else if (!(fileInfo.attrib & _A_SUBDIR) )
				{
					if (bRecursive && fileMask)	// if we are recusive we need to check extension manualy, so we get dirs and stuf
					{
						if (strrchr(FilePath.c_str(),'.'))
						{
							if ( stricmp(strrchr(FilePath.c_str(),'.')+1, extenstionSearch.c_str() ) == 0 )
								info->nameList.push_back(FilePath);
						}
					}
					else
						info->nameList.push_back(FilePath);
				}
			}
			if (_findnext(hFile,&fileInfo) == -1)
				bDone = true;
		}
	}
	return true;
#else
	return false;
#endif 
}

// linux mask filter functions
// we don't need these for windows as it can do it right in findNextFile
#ifndef _WIN32
static int match_multi (const char **mask, const char **string)
{
	const char *msk;
	const char *str;
	const char *msktop;
	const char *strtop;

	msk = *mask;
	str = *string;

	while ((*msk != '\0') && (*msk == '*'))
		msk++;                      /* get rid of multiple '*'s */

	if (*msk == '\0')				/* '*' was last, auto-match */
		return +1;

	msktop = msk;
	strtop = str;

	while (*msk != '\0')
	{
		if (*msk == '*')
		{
			*mask = msk;
			*string = str;
			return 0;                 /* matched this segment */
		}
		else if (*str == '\0')
			return -1;                /* can't match */
		else
		{
			if ((*msk == '?') || (*msk == *str))
			{
				msk++;
				str++;
				if ((*msk == '\0') && (*str != '\0'))	/* advanced check */
				{										
					str++;
					strtop++;
					str = strtop;
					msk = msktop;
				}
			}
			else
			{
				str++;
				strtop++;
				str = strtop;
				msk = msktop;
			}
		}
	}

	*mask = msk;
	*string = str;
	return +1;											 /* full match */
}

static int match_mask (const char *mask, const char *string)
{ 
	if (mask == NULL)
		return 0;

	if (string == NULL)
		return 0;

	if ((mask[0] == '*') && (mask[1] == '\0'))
		return 1;									/* instant match */

	while (*mask != '\0')
	{
		if (*mask == '*')
		{
			mask++;
			switch (match_multi (&mask, &string))
			{
				case +1:
					return 1;
				case -1:
					return 0;
			}
		}
		else if (*string == '\0')
			return 0;
		else if ((*mask == '?') || (*mask == *string))
		{
			mask++;
			string++;
		}
		else
			return 0;
	}

	if (*string == '\0')
		return 1;
	else
		return 0;
}
#endif

bool COSDir::LinuxAddFileStack (  const char *szPathName, const char* fileMask, bool bRecursive  )
{
#ifdef _WIN32
	return false;//WIN32
#else
	DIR	*directory;
	dirent	*fileInfo;
	struct stat	statbuf;
	char	searchstr[1024];
	std::string	FilePath;

	strcpy(searchstr, szPathName);
	if (searchstr[strlen(searchstr) - 1] != '/')
		strcat(searchstr, "/");
	directory = opendir(searchstr);
	if (!directory)
		return false;

	// TODO: make it use the filemask
	while ((fileInfo = readdir(directory)))
	{
		if (!((strcmp(fileInfo->d_name, ".") == 0) || (strcmp(fileInfo->d_name, "..") == 0)))
		{
			FilePath = searchstr;
			FilePath += fileInfo->d_name;
			GetUperName(fileInfo->d_name);

			stat(FilePath.c_str(), &statbuf);

			if (S_ISDIR(statbuf.st_mode) && bRecursive)
				LinuxAddFileStack(FilePath.c_str(),fileMask,bRecursive);
			else if (match_mask (fileMask, fileInfo->d_name))
				info->nameList.push_back(FilePath);
		}
	}
	closedir(directory);
	return true;
#endif// !Win32
}
