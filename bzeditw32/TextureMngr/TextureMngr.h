// Texture Manager.h

#ifdef TEXTURE_MNGR_EXPORTS
#define TEXMAN_API __declspec(dllexport)
#else
#define TEXMAN_API __declspec(dllimport)
#endif

#ifndef _Texture_Manager_H_
#define _Texture_Manager_H_

typedef struct 
{
	unsigned short	x,y;
	bool			alpha;
	int	unsigned	glID;
	int				manID;
	char			file[512];
	char			name[64];

}trTextureInfo;

class TEXMAN_API CTextureManager
{
public:
	CTextureManager();
	~CTextureManager();

	int	Load ( const char* szPath, bool	bSkin = false  );
	int LoadAll ( const char * szPath, bool	bSkin = false );
	int GetID ( char * szName );
	bool GetInfo ( int iID, trTextureInfo* pInfo );

	bool Bind ( int iID );
	
	bool Flush ( int iID = -1);

private:
	
	int	LoadSME ( const char* szPath,trTextureInfo	*pTexInfo, bool	bSkin = false  );
	int	LoadBMP ( const char* szPath,trTextureInfo	*pTexInfo, bool	bSkin = false  );
	int	LoadTGA ( const char* szPath,trTextureInfo	*pTexInfo, bool	bSkin = false  );

	void*	m_pTextureMap;
	void*	m_pTextureList;

	unsigned int m_uiLastBindID;
};


#endif //_Texture_Manager_H_