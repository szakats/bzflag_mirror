// Model.h

#ifndef _MODEL_H_
#define _MODEL_H_

#include "pocketengine.h"

#include <STDIO.H>
#include <STDLIB.H>

typedef enum
{
	eUnknownFile,
	eFBFile,
	eOBJFile,
	eTMFFile,
	eBMFFile,
	eRawFile,
	eSTLFile
}teFileType;

class POCKETENGINE_API CModel
{
public :
	CModel();
	~CModel();

	bool Load ( const char* fileName );
	bool Save ( const char* fileName,teFileType fileType ); 

	virtual void Clear ( void );

	virtual void Draw ( void );
	virtual void BuildList ( void );

	void SetTexture ( UINT iTextureID ) {m_iTexture = iTextureID;}

	bool Valid (void ) {return (m_pTris!=0);}

	bool			m_bTextured;
	bool			m_bLit;

	UINT			m_iNorms;
	UINT			m_iVerts;
	UINT			m_iTCs;
	UINT			m_iTris;
	trVertex3D		*m_pVerts;	
	trVertex3D		*m_pNorms;	
	trVertex2D		*m_pTCs;	
	trTriangle		*m_pTris;	

	trColor			m_rColor;	
	UINT			m_iList;	
	UINT			m_iTexture;	
	char			m_szTexture[256];	

	float			m_aMatrix[16];	
	float			m_aEulers[6];	

	teFileType		m_eInputType;	

protected:
	virtual void Init ( void );
	teFileType ClasifyFile ( char* szFileName );

	char		m_szFilePath[512];

	void LoadFB ( FILE *pFile );
	void LoadOBJ ( FILE *pFile );
	void LoadTMF ( FILE *pFile );
	void LoadBMF ( FILE *pFile );
	void LoadRAW ( FILE *pFile );
	void LoadSTL ( FILE *pFile );

	void LoadMemBMF ( char*	pData );
	void LoadMemOBJ ( char*	pData );
	void LoadMemFB ( char*	pData );

	void SaveFB ( FILE *pFile );
	void SaveOBJ ( FILE *pFile );
	void SaveTMF ( FILE *pFile );
	void SaveBMF ( FILE *pFile );
	void SaveRAW ( FILE *pFile );
	void SaveSTL ( FILE *pFile );
};

// for drawing text
class POCKETENGINE_API CUIFont
{
public:
	CUIFont();
	~CUIFont();

	virtual void SetCharSize ( UINT	size ) {m_iCharSize = size;}
	virtual bool SetFont ( UINT	iTextureID ){m_iTextureID = iTextureID;InitLists();return Valid();}

	bool Valid ( void ) { return (m_iTextureID != 0);}

	virtual void DrawString ( trVertex3D pos, char* string ) { DrawString(pos.x,pos.y,pos.z,string);}
	virtual void DrawString ( float x, float y, float z, char* string  );
	virtual void DrawStringFlat ( float x, float y, float z,float rot,float tilt, char* string  );

protected:
	UINT					m_iTextureID;
	UINT					m_aListIDs[128];
	UINT					m_iCharSize;

	void InitLists ( void );
};

class POCKETENGINE_API CUItem
{
public:
	CUItem();
	~CUItem();

	virtual bool SetItemInfo ( UINT	iTextureID, UINT x, UINT y );

	virtual void Draw ( trVertex3D pos ) { Draw(pos.x,pos.y,pos.z);}
	virtual void Draw ( float x, float y, float z );

	bool Valid ( void ) { return (m_iTextureID != 0);}

protected:
	UINT					m_iTextureID;
	UINT					m_iList;
	UINT					m_iWidth,m_iHeight;
};

#endif //_MODEL_H_