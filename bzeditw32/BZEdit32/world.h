#ifndef __WORLD_H__
#define __WORLD_H__

#ifdef WIN32
	#pragma warning( disable : 4786 )  // Disable warning message for stl maps
	#include <windows.h>
#endif

#include <vector>
#include <map>
#include <string>
#include <iostream>

#include "3dtypes.h"

//#include "objects.h"

#define		M_PI		3.141592653589794f
#define		M_RAD2DEG	57.29577951308232087678f
#define		M_DEG2RAD	0.01745329251994329577f

inline  bool operator < (const std::string &s1,const std::string &s2) { return (s1.compare(s2)<0);}

class CWorld;
class CTextureManager;
class CModelManager;

class CBaseObject
{
public:
	CBaseObject();
	~CBaseObject();

	CBaseObject(const CBaseObject &r);

	virtual void CopyFrom (CBaseObject *r);

	virtual bool Init ( void );

	// to be called by world
	virtual bool ShowInInterface ( void );
	virtual bool ForcedAdd ( void );
	virtual bool OnlyOne ( void );

	virtual void Render(bool transparent = false, bool bSelected = false, int iName = -1);

	virtual void Write( std::ostream &Stream ) { return; }
	virtual void Write( char *data ) { return; }
	virtual bool Read( char *data ) { return false; }

	// to be called by the editor
	virtual void* GetEditInterface ( void *pParam );
	virtual void CloseEditInterface ( void );
	
	virtual void UpdateEditInfo ( void );
	virtual void ApplyEdit ( void );
	virtual void RevertEdit ( void );

	virtual void* GetIcon ( void );

	// to be used by some classes
	virtual bool SetItemInfo ( char  *szName, int iData ) {return false;}
	virtual bool SetItemInfo ( char  *szName, float fData ) {return false;}
	virtual bool SetItemInfo ( char  *szName, char *pData ) {return false;}

	virtual bool GetItemInfo ( char  *szName, int &iData ) {return false;}
	virtual bool GetItemInfo ( char  *szName, float &fData ) {return false;}
	virtual bool GetItemInfo ( char  *szName, char *pData ) {return false;}

	// standard data sets
	void SetPos ( trVertex3D rVert ) { m_rPosition = rVert; }
	void SetXPos(float px) { m_rPosition.x = px; }
    void SetYPos(float py) { m_rPosition.y = py; }
    void SetZPos(float pz) { m_rPosition.z = pz; }

	void SetScale ( trVertex3D rVert ) { m_rScale = rVert; }
    void SetXScale(float sx){ m_rScale.x = sx; }
    void SetYScale(float sy){ m_rScale.y = sy; }
    void SetZScale(float sz){ m_rScale.z = sz; }

    void SetRotAngle(float angle){ m_fAngle = angle; }

	void SetTextureMan ( CTextureManager *pMan ) { m_pTextureMan = pMan;}
	void SetModelMan ( CModelManager *pMan ) { m_pModelMan = pMan;}
	void SetWorld ( CWorld *pWorld ) { m_pWorld = pWorld; }

	// standard data gets
	trVertex3D GetPos ( void ) { return m_rPosition; }
    float GetXPos( void ){ return m_rPosition.x; }
    float GetYPos( void ){ return m_rPosition.y; }
    float GetZPos( void ){ return m_rPosition.z; }

	trVertex3D GetScale ( void ) { return m_rScale; }
    float GetXScale( void ) {  return m_rScale.x; }
    float GetYScale( void ){  return m_rScale.y; }
    float GetZScale( void ) {  return m_rScale.z; }

    float GetRotAngle( void ) { return m_fAngle; }
	
	const char*	GetListName ( void );
	const char*	GetTypeName ( void );

	// world data gets and sets
	const char*	GetItemName ( void ) { return m_sItemName.c_str(); }
	void SetItemName ( const char*	name ) { m_sItemName = name; }

	void SetGroupID ( int iNewGroup ) { m_iGroupID = iNewGroup;}
	int GetGroupID ( void ) { return m_iGroupID;}

	void SetGroupName( const char * name ) { m_sGroupName = name; }
	const char* GetGroupName( void ) { return m_sGroupName.c_str(); }

	void SetID ( int ID ) { m_iListID = ID; }
	int GetID ( void ) { return m_iListID; }

	void SetVisible ( bool bVis ) { m_bVisible = bVis; }
	bool GetVisible ( void ) { return m_bVisible; }

	void SetSelected ( bool bSel ) { m_bSelected = bSel; }
	bool GetSelected ( void ) { return m_bSelected; }

	// Util funcs
	void SetDefalutData ( void );

	int GetTypeID ( void ) { return m_iTypeID; }
	void SetTypeID ( int iID ) { m_iTypeID = iID; }

protected:
	CTextureManager	*m_pTextureMan;
	CModelManager	*m_pModelMan;
	CWorld			*m_pWorld;
	
	trVertex3D		m_rPosition;
 	trVertex3D		m_rScale;
    float			m_fAngle;

	std::string		m_sTypeName;
	std::string		m_sItemName;
	std::string		m_sListName;

	int				m_iGroupID;
	std::string		m_sGroupName;

	int				m_iListID;

	bool			m_bVisible;
    bool			m_bSelected;
	int				m_iTypeID;
	void			*m_pInterface;

	bool			m_bDriveThru;
	bool			m_bShootThru;
	bool			m_bFlipZ;
	
	bool StdDataField ( char *data );

	void WriteStdData ( std::ostream &stream );
	void WriteStdData ( char *data );
};

typedef std::vector<CBaseObject*> trObjectList;
typedef std::map<unsigned int,CBaseObject*> trObjectMap;

class CBaseObjectFactory
{
public:
	CBaseObjectFactory() { m_iTypeID = -1; }
	~CBaseObjectFactory(){;}

	virtual CBaseObject* New( void ) { return new CBaseObject; }
	virtual void Delete ( CBaseObject* pObject ) { delete(pObject); }

	int GetTypeID ( void ) { return m_iTypeID; }
	void SetTypeID ( int iID ) { m_iTypeID = iID; }

	const char* GetName ( void ) { return m_szName.c_str(); }
	void SetName ( const char* name ) { m_szName = name; }

protected:
	int				m_iTypeID;
	std::string		m_szName;
};

typedef std::map<std::string,CBaseObjectFactory*> tmFacotryNameMap;
typedef std::vector<CBaseObjectFactory*> tvFacotryList;

class CWorld 
{
public:
	CWorld();
	CWorld(CWorld &rWorld);
	~CWorld();

	void Init ( void );
	void Clear ( void );

	bool Load( const char *szFileName );
	bool Append( const char *szFileName );
	bool Write( const char *szFileName );

	CBaseObject *GetObject( char* szName );
	CBaseObject *GetObject( int iItem );

	CBaseObject *GetNthObjectOfType ( const char *szType, int iN );
	int GetObjectListOfType ( const char *szType, trObjectList	&rList );

	int GetSelItem ( void ) { return m_iSelItem; }
	void SetSelItem ( int iSel ) { m_iSelItem = iSel; }

	bool GetFadeNonSel ( void ) { return m_bFadeNonSelect; }
	void SetFadeNonSel ( bool bSet ) { m_bFadeNonSelect = bSet; }

	bool GetDrawAll ( void ) { return m_bDrawAll; }
	void SetDrawAll ( bool bSet ) { m_bDrawAll = bSet; }

	int	AddObject( const char *szType );
	int	AddObject( int iTypeID );
	int	AddObject( CBaseObject *pToCopy );

	int DupeObject ( int iID );
	int DupeObject ( CBaseObject *pToCopy );

	bool RemoveObject( int item );

	void Sort ( void );
	bool Inited ( void );

	int Size ( void ) { return m_vObjList.size(); }

	void Render( void );
	void RenderTargets( void );
	
	void SetTextureMan ( CTextureManager *pMan ) { m_pTextureMan = pMan;}
	void SetModelMan ( CModelManager *pMan ) { m_pModelMan = pMan;}

	void RegisterFactory ( const char *szName, CBaseObjectFactory	*pFactory );

	int GetFactoryCount ( void );
	const char* GetFactoryName ( int iFactory );

	CBaseObjectFactory*	GetFactory ( int iFactory );
	CBaseObjectFactory*	GetFactory ( const char *szName );

private:		
	CTextureManager	*m_pTextureMan;
	CModelManager	*m_pModelMan;

	CBaseObject		*m_pGround;
	CBaseObject		*m_pWalls;
	bool			m_bInited;

	tmFacotryNameMap	m_vFactoryNameMap;
	tvFacotryList		m_vFactoryList;
//	trObjectList		m_vObjList;
	trObjectMap			m_vObjList;

	int					m_iNextGUID;
	
	int				m_iSelItem;
	bool			m_bFadeNonSelect;
	bool			m_bDrawAll;
};

#endif
