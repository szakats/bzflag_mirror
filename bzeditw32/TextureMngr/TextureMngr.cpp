// TextureMngr.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "textureMngr.h"

BOOL APIENTRY DllMain( HANDLE hModule,  DWORD  ul_reason_for_call,  LPVOID lpReserved)
{
    return TRUE;
}

#include "SMELoad.h"

#include <GL/gl.h>
#include <GL/glu.h>

#pragma warning( disable : 4786 )  // Disable warning message

#include <STRING>
#include <VECTOR>
#include <MAP>

#include <io.h>
#include <direct.h>

inline  bool operator < (const std::string &s1,const std::string &s2) { return (s1.compare(s2)<0);}
typedef std::map<std::string,int>  tmTextureNameMap;
typedef std::vector<trTextureInfo*>  tvTextureList;

#define _m_pTextureMap ((tmTextureNameMap*)m_pTextureMap)
#define _m_pTextureList ((tvTextureList*)m_pTextureList)

#define m_TextureMap (*(tmTextureNameMap*)m_pTextureMap)
#define m_TextureList (*(tvTextureList*)m_pTextureList)

CTextureManager::CTextureManager()
{
	m_pTextureMap = (void*) new tmTextureNameMap;
	m_pTextureList = (void*) new tvTextureList;

	m_uiLastBindID = -1;
}

CTextureManager::~CTextureManager()
{
	Flush();
	if (m_pTextureMap)
		delete (_m_pTextureMap);

	if (m_pTextureList)
	{
		tvTextureList::iterator	listItr = m_TextureList.begin();

		while(listItr != m_TextureList.end() )
			free ( (*listItr++) );
	}

	m_pTextureList = NULL;
	m_pTextureMap = NULL;

}

int	CTextureManager::LoadSME ( const char* szPath,trTextureInfo	*pTexInfo, bool	bSkin )
{
	trSMEHeaderInfo		rImageInfo;

	// make sure it's a real file
	if (!::LoadSME(szPath, &rImageInfo))
		return -1;

	pTexInfo->x = rImageInfo.width;
	pTexInfo->y = rImageInfo.height;
	pTexInfo->alpha = rImageInfo.panes > 3;

	// load the image
	unsigned int id = m_uiLastBindID+1 ;
	trSME8BitData		data8;
	trSME16BitData		data16;

	glGenTextures(1,&id);
	glBindTexture(GL_TEXTURE_2D,id);

	int	iEdgeType;

	if (bSkin)
		iEdgeType = GL_CLAMP;
	else
		iEdgeType = GL_REPEAT;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, iEdgeType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, iEdgeType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	if (rImageInfo.depth == 8)
	{
		GetSME8BitData(&rImageInfo,&data8,true);

		if ( rImageInfo.panes == 3 )
			gluBuild2DMipmaps(GL_TEXTURE_2D,3,rImageInfo.width,rImageInfo.height,GL_RGB,GL_UNSIGNED_BYTE,data8.data);
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D,4,rImageInfo.width,rImageInfo.height,GL_RGBA,GL_UNSIGNED_BYTE,data8.data);
	}
	else
	{
		GetSME16BitData(&rImageInfo,&data16,true);

		if ( rImageInfo.panes == 3 )
			gluBuild2DMipmaps(GL_TEXTURE_2D,3,rImageInfo.width,rImageInfo.height,GL_RGB,GL_UNSIGNED_SHORT,data16.data);
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D,4,rImageInfo.width,rImageInfo.height,GL_RGBA,GL_UNSIGNED_SHORT,data16.data);
	}

	glBindTexture(GL_TEXTURE_2D,0);

	ReleaseSME(&rImageInfo);

	return id;
}

int	CTextureManager::LoadBMP ( const char* szPath,trTextureInfo	*pTexInfo, bool	bSkin)
{
	long				iImageH, iImageW,offset;
	BITMAPFILEHEADER	FileHeader;
	BITMAPINFOHEADER	InfoHeader;

	unsigned int		iglID;
	
	unsigned char		*pData;

	FILE	*fp = fopen(szPath,"rb");

	if (!fp)
		return -1;

	// first parts of a BMP
	fread(&FileHeader,sizeof(BITMAPFILEHEADER),1,fp);
	fread(&InfoHeader,sizeof(BITMAPINFOHEADER),1,fp);

	if (InfoHeader.biBitCount != 24)
		return -1;

	iImageH = InfoHeader.biHeight;
	iImageW = InfoHeader.biWidth;

	// read in h*w*3 24 bit only
	pData = (unsigned char*)malloc(iImageH*iImageW*3);
	offset = FileHeader.bfOffBits;
	fseek(fp,offset,SEEK_SET);

	fread(pData,iImageH*iImageW*3,1,fp);
	fclose (fp);

	// get us a new Texture ID
	glGenTextures(1,&iglID);

	// and in the darkness bind it
	glBindTexture(GL_TEXTURE_2D,iglID);

	// set the params
	int	iEdgeType;

	if (bSkin)
		iEdgeType = GL_CLAMP;
	else
		iEdgeType = GL_REPEAT;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// make it a GL texture object and mip map it
	gluBuild2DMipmaps(GL_TEXTURE_2D,3,iImageW,iImageH,GL_BGR_EXT,GL_UNSIGNED_BYTE,pData);

	free(pData);

	pTexInfo->x = iImageW;
	pTexInfo->y = iImageH;
	pTexInfo->alpha = false;

	return (int)iglID;
}

bool checkSize(int x) 
{
  if(x == 2 || x == 4 || x == 8 || x == 16 || x == 32 || x == 64 || x == 128 || x == 256 || x == 512 || x == 1024)
    return true;
  
  return false;
}

unsigned char *getRGBA(FILE *s, int size,GLenum &texFormat)
{
	unsigned char *rgba;
	unsigned char temp;
	int bread;

	rgba = (unsigned char *) malloc(size * 4);
	if(rgba == NULL)
		return NULL;

	bread = fread(rgba, sizeof(unsigned char), size * 4, s);
	if(bread != size * 4)
	{
		free(rgba);
		return NULL;
	}

	for(int i = 0; i < size * 4; i += 4)
	{
		temp = rgba[i];
		rgba[i] = rgba[i + 2];
		rgba[i] = temp;
	}
	
	texFormat = GL_RGBA;
	return rgba;
}

unsigned char *getRGB(FILE *s, int size,GLenum &texFormat)
{
	unsigned char *rgb;
	unsigned char temp;
	int bread;

	rgb = (unsigned char *) malloc(size * 3);

	if(rgb == NULL)
		return NULL;

	bread = fread(rgb, sizeof(unsigned char), size * 3, s);
	if(bread != size * 3)
	{
		free(rgb);
		return NULL;
	}
	for(int i = 0; i < size * 3; i += 3)
	{
		temp = rgb[i];
		rgb[i] = rgb[i + 2];
		rgb[i + 2] = temp;
	}

	texFormat = GL_RGB;
	return rgb;
}

unsigned char *getGrey(FILE *s, int size,GLenum &texFormat)
{
	unsigned char *grey;
	int bread;

	grey = (unsigned char *) malloc(size);
	if(grey == NULL)
		return NULL;

	bread = fread(grey, sizeof(unsigned char), size, s);
	if(bread != size)
	{
		free(grey);
		return NULL;
	}

	texFormat = GL_ALPHA;
	return grey;
}

unsigned char *getData(FILE *s, int size, int bits,GLenum &texFormat)
{
  switch(bits)
  {
    case 32:
      return getRGBA(s,size,texFormat);
    case 24:
      return getRGB(s,size,texFormat);
    case 8:
      return getGrey(s,size,texFormat);
  }
  return NULL;
}

int	CTextureManager::LoadTGA ( const char* szPath,trTextureInfo	*pTexInfo, bool	bSkin)
{
	unsigned char type[4];
	unsigned char info[7];
	unsigned char *imageData = NULL;
	int w, h;
	int bits, size;
	FILE *s;

	if(!(s = fopen(szPath, "rb")))
		return -1;

	fread(&type, sizeof(char), 3, s);
	fseek(s, 12, SEEK_SET);

	fread(&info, sizeof(char), 6, s);
	if(type[1] != 0 || (type[2] != 2 && type[2] != 3))
		return -1;

	w = info[0] + info[1] * 256;
	h = info[2] + info[3] * 256;

	bits = info[4];
	size = w * h;

	GLenum texFormat;

	if(!checkSize(w) || !checkSize(h))
		return -1;

	if(bits != 32 && bits != 24 && bits != 8)
		return -1;

	imageData = getData(s, size, bits,texFormat);
	if(imageData == NULL)
		return -1;

	fclose(s);

	// get us a new Texture ID
	unsigned int iglID;
	glGenTextures(1,&iglID);


	glBindTexture(GL_TEXTURE_2D, iglID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	int	iEdgeType;

	if (bSkin)
		iEdgeType = GL_CLAMP;
	else
		iEdgeType = GL_REPEAT;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, iEdgeType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, iEdgeType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	glTexImage2D(GL_TEXTURE_2D, 0, texFormat, w, h, 0, texFormat, GL_UNSIGNED_BYTE, imageData);
	gluBuild2DMipmaps(GL_TEXTURE_2D, texFormat, w, h, texFormat, GL_UNSIGNED_BYTE, imageData);

		// make it a GL texture object and mip map it
//	gluBuild2DMipmaps(GL_TEXTURE_2D,3,iImageW,iImageH,GL_BGR_EXT,GL_UNSIGNED_BYTE,pData);
	
	pTexInfo->alpha = ((texFormat == GL_RGBA) || (texFormat == GL_ALPHA));
	pTexInfo->x = w;
	pTexInfo->y = h;

	free(imageData);
	return iglID;
}

int	CTextureManager::Load ( const char* szFile, bool bSkin )
{
	if (!szFile)
		return -1;

	trTextureInfo	*pTexInfo = (trTextureInfo*)malloc(sizeof(trTextureInfo));

		// get a name for it
	char	szName[255],*pSlash,extension[32] = {0};

	pSlash = strrchr(szFile,'\\');

	if (pSlash)
		pSlash++;

	if (pSlash && *pSlash)
		strcpy(szName,pSlash);
	else
		strcpy(szName,szFile);

	pSlash = strrchr(szName,'.');

	if (pSlash)	
	{
		strcpy(extension,pSlash+1);
		(*pSlash) = NULL;
	}

	strcpy(pTexInfo->name,szName);
	strcpy(pTexInfo->file,szFile);

	unsigned int id = 0;

	if (extension[0] && (stricmp(extension,"BMP")==0) )
		id = LoadBMP(szFile,pTexInfo,bSkin);
	else if (extension[0] && (stricmp(extension,"TGA")==0) )
		id = LoadTGA(szFile,pTexInfo,bSkin);
	else if (extension[0] && (stricmp(extension,"SME")==0) )
		id = LoadSME(szFile,pTexInfo,bSkin);
	else
		return -1;
		
	m_uiLastBindID = id;
	pTexInfo->glID = id;
	pTexInfo->manID = _m_pTextureList->size();

	std::string	str = szName;
	m_TextureMap[str] = pTexInfo->manID;
	_m_pTextureList->push_back(pTexInfo);

	return pTexInfo->manID;
}

int CTextureManager::LoadAll ( const char * szPath, bool bSkin )
{
	struct _finddata_t fileInfo;
	long	hFile;
	char	searchstr[1024];

	char	FilePath[1024];

	bool	bDone = false;

	int		iFiles = 0;

	strcpy(searchstr,szPath);
	strcat(searchstr,"\\*.*");

	hFile = _findfirst(searchstr,&fileInfo);

	if (hFile != -1)
	{
		while (!bDone)
		{
			strcpy(FilePath,szPath);
			strcat(FilePath,"\\");
			strcat(FilePath,fileInfo.name);

			if (Load(FilePath,bSkin)!= -1)
				iFiles++;

			if (_findnext(hFile,&fileInfo) == -1)
				bDone = true;
		}
	}
	return iFiles;
}

int CTextureManager::GetID ( char * szName )
{
	std::string	str = szName;

	tmTextureNameMap::iterator	itr = m_TextureMap.find(str);
	
	if (itr == m_TextureMap.end())
		return -1;
	else
		return itr->second;
	
}

bool CTextureManager::GetInfo ( int iID, trTextureInfo* pInfo )
{
	if ( (iID<0) || (iID>=_m_pTextureList->size()) || !pInfo)
		return false;

	pInfo = m_TextureList[iID];
	return true;
}

bool CTextureManager::Bind ( int iID )
{
	if ( (iID<0) || (iID>=_m_pTextureList->size()) )
		return false;

	glBindTexture(GL_TEXTURE_2D,m_TextureList[iID]->glID);
	return true;

}

bool CTextureManager::Flush ( int iID )
{
	if (iID == -1)
	{
		tvTextureList::iterator itr = m_TextureList.begin();

		while (itr !=  m_TextureList.end())
		{
			glDeleteTextures(1,&((*itr)->glID));
			free((*itr));
			itr++;
		}
		m_TextureList.clear();
		m_TextureMap.clear();
		return true;
	}
	else
		return false;
}
