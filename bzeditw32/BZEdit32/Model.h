// model.h

#ifndef _MODEL_H_
#define _MODEL_H_

#include "TextureMngr.h"
#include <VECTOR>

#include "3dtypes.h"


// place holder for modifying world size
#define M_WORLDSIZE 400.0f

typedef enum
{
	eFirst = 0,
	eHigh,
	eNormal,
	eTransNoZ,
	eTranspaent,
	eAlways,
	eLast
}teDrawPriority;

typedef struct 
{
	int		iID;
	bool	bTrans;
	bool	bSel;
	int		iName;
	teDrawPriority	ePriority;
}trDrawItem;

class CBaseModel
{
	public:
		CBaseModel();
		~CBaseModel();

		virtual void Init ( trVertex3D rRot, trVertex3D rScale, float fRot = 0.0f );
		virtual void Render ( bool bTrans = false, bool bSelected = false );
		virtual void SetItemInfo ( char* szItem, float *fData );

		int GetTexuture ( void ) { return iTexture; } 
		void SetTexuture ( int iTex  ) { iTexture  = iTex; } 

	protected:
		int	iGLListID;
		int	iTexture;
		int	iSelList;
};

class CGroundPlaneModel :public CBaseModel
{
	public:
		virtual void Init ( trVertex3D rPos, trVertex3D rScale, float fRot = 0.0f );
};

class CWallsModel :public CBaseModel
{
	public:
		virtual void Init ( trVertex3D rPos, trVertex3D rScale, float fRot = 0.0f );
};

class CBoxSidesModel :public CBaseModel
{
	public:
		virtual void Init ( trVertex3D rPos, trVertex3D rScale, float fRot = 0.0f );
};

class CBoxTopsModel :public CBaseModel
{
	public:
		virtual void Init ( trVertex3D rPos, trVertex3D rScale, float fRot = 0.0f );
};

class CPyramidModel :public CBaseModel
{
	public:
		virtual void Init ( trVertex3D rPos, trVertex3D rScale, float fRot = 0.0f );
};

class CTPortFieldModel :public CBaseModel
{
	public:
		virtual void Init ( trVertex3D rPos, trVertex3D rScale, float fRot = 0.0f );
		virtual void Render ( bool bTrans = false, bool bSelected = false );
		virtual void SetItemInfo ( char* szItem, float *fData );

	protected:
		float	fBorder;
};

class CTPortFrameModel :public CBaseModel
{
	public:
		virtual void Init ( trVertex3D rPos, trVertex3D rScale, float fRot = 0.0f );
		virtual void SetItemInfo ( char* szItem, float *fData );

	protected:
		float	fBorder;
};

class CBaseTopsModel :public CBaseModel
{
	public:
		virtual void Init ( trVertex3D rPos, trVertex3D rScale, float fRot = 0.0f );
		virtual void Render ( bool bTrans = false, bool bSelected = false );
		virtual void SetItemInfo ( char* szItem, float *fData );
	protected:
		int		iColor;
};

class CLinkModel :public CBaseModel
{
	public:
		CLinkModel();

		virtual void Render ( bool bTrans = false, bool bSelected = false );
		virtual void SetItemInfo ( char* szItem, float *fData );
	protected:
		void*	pWorld;
		int		iFrom;
		int		iTo;
};

typedef std::vector<trDrawItem>				trDrawList;
typedef std::vector<trDrawItem>::iterator	trDrawListItr;

typedef std::vector<CBaseModel*>			trModelList;
typedef std::vector<CBaseModel*>::iterator	trModelListItr;

class CModelManager
{
public:
	CModelManager();
	~CModelManager();

	CModelManager(CTextureManager *pTexMan);

	void SetTextureMan ( CTextureManager *pTexMan ) { pTextureManager = pTexMan;}

	void Init ( void );
	void Clear ( void );

	int	NewModel ( char *szType );
	void KillModel ( int iItem );

	CBaseModel	*GetModel ( int iItem );

	void AddDrawItem ( int iItem, bool bTransparent = false, bool bSelected = false, int iName = -1, teDrawPriority	ePriority = eNormal );
	void ClearDrawList ( void );
	void DrawList ( void );

private:
	
	trDrawList			aDrawList;
	trModelList			aModelList;
	CTextureManager	*pTextureManager;
};

#endif//_MODEL_H_ 