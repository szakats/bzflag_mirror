// BZEdit32Doc.cpp : implementation of the CBZEdit32Doc class
//

#include "stdafx.h"
#include "BZEdit32.h"
#include "api.h"

#include "BZEdit32Doc.h"
#include "Objects.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// API funks
CWorld			*pTheWorld = NULL;
CBZEdit32Doc	*pTheDoc = NULL;

void SetTheWorld ( CWorld *pWorld )
{
	pTheWorld = pWorld;
}

void SetTheDoc ( CBZEdit32Doc *pDoc )
{
	pTheDoc = pDoc;
}


/////////////////////////////////////////////////////////////////////////////
// CBZEdit32Doc

IMPLEMENT_DYNCREATE(CBZEdit32Doc, CDocument)

BEGIN_MESSAGE_MAP(CBZEdit32Doc, CDocument)
	//{{AFX_MSG_MAP(CBZEdit32Doc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	ON_COMMAND(ID_EDIT_MAKEANGLESDEG, OnEditMakeanglesdeg)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MAKEANGLESDEG, OnUpdateEditMakeanglesdeg)
	ON_COMMAND(ID_EDIT_MAKEANGLESROT, OnEditMakeanglesrot)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MAKEANGLESROT, OnUpdateEditMakeanglesrot)
	ON_COMMAND(ID_UTILTY_CONVERTMAPFILETODEG, OnUtiltyConvertmapfiletodeg)
	ON_UPDATE_COMMAND_UI(ID_UTILTY_CONVERTMAPFILETODEG, OnUpdateUtiltyConvertmapfiletodeg)
	ON_COMMAND(ID_UTILTY_CONVERTMAPFILETORAD, OnUtiltyConvertmapfiletorad)
	ON_UPDATE_COMMAND_UI(ID_UTILTY_CONVERTMAPFILETORAD, OnUpdateUtiltyConvertmapfiletorad)
	ON_COMMAND(ID_UTILTY_DUMPMAP, OnUtiltyDumpmap)
	ON_UPDATE_COMMAND_UI(ID_UTILTY_DUMPMAP, OnUpdateUtiltyDumpmap)
	ON_COMMAND(ID_UTILTY_DUPEANDFLIPX, OnUtiltyDupeandflipx)
	ON_UPDATE_COMMAND_UI(ID_UTILTY_DUPEANDFLIPX, OnUpdateUtiltyDupeandflipx)
	ON_COMMAND(ID_UTILTY_DUPEANDFLIPY, OnUtiltyDupeandflipy)
	ON_UPDATE_COMMAND_UI(ID_UTILTY_DUPEANDFLIPY, OnUpdateUtiltyDupeandflipy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBZEdit32Doc construction/destruction

CBZEdit32Doc::CBZEdit32Doc()
{
	// TODO: add one-time construction code here
//	
	m_oTexMan.LoadAll("data\\");
}

CBZEdit32Doc::~CBZEdit32Doc()
{
	m_oTexMan.Flush();
}

BOOL CBZEdit32Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	char	szPath[512];
	GetModuleFileName(AfxGetInstanceHandle(),szPath,512);

	if (strrchr(szPath,'\\') != NULL)
		*(strrchr(szPath,'\\')+1) = NULL;

	chdir(szPath);

	m_oTexMan.LoadAll("data\\");

	m_oWorld.SetTextureMan(&m_oTexMan);
	m_oWorld.Init();

	SetTheWorld ( &m_oWorld );
	SetTheDoc ( this );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBZEdit32Doc serialization

void CBZEdit32Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBZEdit32Doc diagnostics

#ifdef _DEBUG
void CBZEdit32Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBZEdit32Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBZEdit32Doc commands

BOOL CBZEdit32Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	m_oWorld.Load(lpszPathName);

	SetPathName(lpszPathName,true);

	char	filePath[512];

	strcpy(filePath,lpszPathName);

	char	*p = strrchr(filePath,'\\');

	char	szName[255];
	if (p)
		strcpy(szName,p+1);

	p = strrchr(szName,'.');
	if (p)
		*p = NULL;

	m_oFile = lpszPathName;
	
	SetTitle(szName);
	UpdateAllViews(NULL);
	
	return TRUE;
}

void CBZEdit32Doc::OnCloseDocument() 
{
	CDocument::OnCloseDocument();
}

void CBZEdit32Doc::OnFileNew() 
{
	m_oFile = "";
	m_oWorld.Clear();
	CDocument::OnNewDocument();

	SetTitle("Untitled");
	UpdateAllViews(NULL);
}

void CBZEdit32Doc::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
}

void CBZEdit32Doc::OnFileOpen() 
{
	char szFilter[] = "BZFlag Map Files (*.bzw;*.map)|*.bzw;*.map|All Files (*.*)|*.*||";
		
	CFileDialog  oFD(true,".map;.bzw","*.map;*.bzw",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);

	if (oFD.DoModal() == IDOK)
	{
		m_oFile = oFD.GetPathName();
		
		m_oWorld.Load(oFD.GetPathName());

		SetPathName(oFD.GetPathName(),true);
		SetTitle(oFD.GetFileTitle());

		UpdateAllViews(NULL);	
	}
}

void CBZEdit32Doc::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{	
}

void CBZEdit32Doc::OnFileSave() 
{
	if (m_oFile.GetLength()==0)
		OnFileSaveAs();
	else
		m_oWorld.Write(m_oFile);
	
//	UpdateAllViews(NULL);
	SetModifiedFlag(false);
}

void CBZEdit32Doc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_oWorld.Size()>0) && IsModified());
}

void CBZEdit32Doc::OnFileSaveAs() 
{
	char szFilter[] = "BZFlag Map Files (*.bzw;*.map)|*.bzw;*.map|All Files (*.*)|*.*||";

	CFileDialog  oFD(false,".bzw",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);

	if (oFD.DoModal() == IDOK)
	{
		m_oFile = oFD.GetPathName();

		m_oWorld.Write(m_oFile);

		SetPathName(oFD.GetPathName(),true);
		SetTitle(oFD.GetFileTitle());
		
//		UpdateAllViews(NULL);
		SetModifiedFlag(false);
	}
}

void CBZEdit32Doc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_oWorld.Size()>0));
}

void CBZEdit32Doc::OnFileClose() 
{
	DeleteContents();
	m_oWorld.Clear();

	OnNewDocument();

	m_oFile = "";
	SetTitle(NULL);
	UpdateAllViews(NULL);

}

void CBZEdit32Doc::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_oWorld.Size()>0));
}

void CBZEdit32Doc::OnEditMakeanglesdeg() 
{
	for ( int i = 0; i < m_oWorld.Size();i++)
	{
		CBaseObject	*pObject = m_oWorld.GetObject(i);

		pObject->SetRotAngle(pObject->GetRotAngle()*M_RAD2DEG);
		pObject->Init();
	}
	SetModifiedFlag(true);
	UpdateAllViews(NULL);
}

void CBZEdit32Doc::OnUpdateEditMakeanglesdeg(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_oWorld.Size()>0));	
}

void CBZEdit32Doc::OnEditMakeanglesrot() 
{
	for ( int i = 0; i < m_oWorld.Size();i++)
	{
		CBaseObject	*pObject = m_oWorld.GetObject(i);
		pObject->SetRotAngle(pObject->GetRotAngle()*M_DEG2RAD);
		pObject->Init();
	}

	SetModifiedFlag(true);
	UpdateAllViews(NULL);
}

void CBZEdit32Doc::OnUpdateEditMakeanglesrot(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_oWorld.Size()>0));	
}

void CBZEdit32Doc::OnUtiltyConvertmapfiletodeg() 
{
	char szFilter[] = "BZFlag Map Files (*.bzw;*.map)|*.bzw;*.map|All Files (*.*)|*.*||";
		
	CFileDialog  oFD(true,".bzw","*.bzw",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);

	if (oFD.DoModal() == IDOK)
	{
		CWorld	world;

		world.Load(oFD.GetPathName());

		for ( int i = 0; i < world.Size();i++)
		{
			CBaseObject	*pObject = world.GetObject(i);
			pObject->SetRotAngle(pObject->GetRotAngle()*M_RAD2DEG);
			pObject->Init();
		}
		world.Write(oFD.GetPathName());

		MessageBox(NULL,"Map angles converted to DEG","Conversion Complete",MB_OK);
	}
}

void CBZEdit32Doc::OnUpdateUtiltyConvertmapfiletodeg(CCmdUI* pCmdUI) 
{
}

void CBZEdit32Doc::OnUtiltyConvertmapfiletorad() 
{
	char szFilter[] = "BZFlag Map Files (*.bzw;*.map)|*.bzw;*.map|All Files (*.*)|*.*||";
		
	CFileDialog  oFD(true,".bzw","*.bzw",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);

	if (oFD.DoModal() == IDOK)
	{
		CWorld	world;

		world.Load(oFD.GetPathName());

		for ( int i = 0; i < world.Size();i++)
		{
			CBaseObject	*pObject = world.GetObject(i);
			pObject->SetRotAngle(pObject->GetRotAngle()*M_DEG2RAD);
			pObject->Init();

		}
		world.Write(oFD.GetPathName());

		MessageBox(NULL,"Map angles converted to RAD","Conversion Complete",MB_OK);
	}
}

void CBZEdit32Doc::OnUpdateUtiltyConvertmapfiletorad(CCmdUI* pCmdUI) 
{
}

void CBZEdit32Doc::OnUtiltyDumpmap() 
{
	char szFilter[] = "Ogre Test Map Files (*.omap)|*.omap|All Files (*.*)|*.*||";
		
	CFileDialog  oFD(false,".omap","sample.omap",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);

	if (oFD.DoModal() == IDOK)
	{
		FILE	*fp = fopen (oFD.GetPathName(),"wb");
		if (!fp)
			return;

	//	fprintf (fp,"WLDF\n\n");
	//	fprintf (fp,"World: :\nXDim: 400\nYDim: 400\nWallHeigh: 5\nGroundTex:	data/Textures/World/ground\nWallTex:	data/Textures/World/wall\n#End\n\n");
		
		int iSize = m_oWorld.Size();
		fwrite(&iSize,4,1,fp);
		CBaseObject *pObject;
	//	float fTemp;

		int type;
		float pos[3];
		float scale[3];
		float rot;

		for (int iItem = 0; iItem < iSize; iItem++)
		{
			pObject = m_oWorld.GetObject(iItem);

			if (pObject)
			{
				std::string	str = pObject->GetTypeName();
				
				if (str == "Box")
				{
					
					type = 0;
					pos[0] = pObject->GetXPos();
					pos[1] = pObject->GetYPos();
					pos[2] = pObject->GetZPos();

					scale[0] = pObject->GetXScale();
					scale[1] = pObject->GetYScale();
					scale[2] = pObject->GetZScale();

					rot = pObject->GetRotAngle();
					//fprintf(fp,"#Box:\n");
					//fprintf(fp,"Pos: %f %f %f\n",pObject->GetXPos(),pObject->GetYPos(),pObject->GetZPos());
					//fprintf(fp,"Scale: %f %f %f\n",pObject->GetXScale(),pObject->GetYScale(),pObject->GetZScale());
				//	fprintf(fp,"Rot: %f\n",pObject->GetRotAngle());
					//fprintf(fp,"Texture:	data/Textures/World/boxwall\nTexture:	data/Textures/World/roof\n#End:\n\n");
				}

				if (str == "Pyramid")
				{
					type = 1;
					pos[0] = pObject->GetXPos();
					pos[1] = pObject->GetYPos();
					pos[2] = pObject->GetZPos();

					scale[0] = pObject->GetXScale();
					scale[1] = pObject->GetYScale();
					scale[2] = pObject->GetZScale();

					rot = pObject->GetRotAngle();

				/*	fprintf(fp,"#Pyramid:\n");
					fprintf(fp,"Pos: %f %f %f\n",pObject->GetXPos(),pObject->GetYPos(),pObject->GetZPos());
					fprintf(fp,"Scale: %f %f %f\n",pObject->GetXScale(),pObject->GetYScale(),pObject->GetZScale());
					fprintf(fp,"Rot: %f\n",pObject->GetRotAngle());
					fprintf(fp,"Texture:	data/Textures/World/pyrwall\n#End:\n\n"); */
				}

				if (str == "Teleporter")
				{
					type = 0;
					pos[0] = pObject->GetXPos();
					pos[1] = pObject->GetYPos();
					pos[2] = pObject->GetZPos();

					scale[0] = pObject->GetXScale();
					scale[1] = pObject->GetYScale();
					scale[2] = pObject->GetZScale();

					rot = pObject->GetRotAngle();

					/*
					fprintf(fp,"#Teleporter:\n");
					fprintf(fp,"Pos: %f %f %f\n",pObject->GetXPos(),pObject->GetYPos(),pObject->GetZPos());
					fprintf(fp,"Scale: %f %f %f\n",pObject->GetXScale(),pObject->GetYScale(),pObject->GetZScale());
					fprintf(fp,"Rot: %f\n",pObject->GetRotAngle());
					pObject->GetItemInfo("Border",fTemp);
					fprintf(fp,"Frame: %f\n",fTemp);
					fprintf(fp,"Texture:	data/Textures/World/caution\n#End:\n\n"); */
				}

				if (str == "Base")
				{
					type = 0;
					pos[0] = pObject->GetXPos();
					pos[1] = pObject->GetYPos();
					pos[2] = pObject->GetZPos();

					scale[0] = pObject->GetXScale();
					scale[1] = pObject->GetYScale();
					scale[2] = pObject->GetZScale();

					rot = pObject->GetRotAngle();
					
					/*
					fprintf(fp,"#Box:\n");
					fprintf(fp,"Pos: %f %f %f\n",pObject->GetXPos(),pObject->GetYPos(),pObject->GetZPos());
					fprintf(fp,"Scale: %f %f %f\n",pObject->GetXScale(),pObject->GetYScale(),pObject->GetZScale());
					fprintf(fp,"Rot: %f\n",pObject->GetRotAngle());
					fprintf(fp,"Texture:	data/Textures/World/boxwall\nTexture:	data/Textures/World/roof\n#End:\n\n"); */
				}

				fwrite(&type,4,1,fp);
				fwrite(pos,4*3,1,fp);
				fwrite(scale,4*3,1,fp);
				fwrite(&rot,4,1,fp);
			}
		}
		fclose(fp);
	}	
}

void CBZEdit32Doc::OnUpdateUtiltyDumpmap(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_oWorld.Size()>0);
}

void CBZEdit32Doc::OnUtiltyDupeandflipx() 
{
	int iSelItem = m_oWorld.GetSelItem();

	CBaseObject	*pObject = m_oWorld.GetObject(m_oWorld.DupeObject(iSelItem));
	trVertex3D pos = pObject->GetPos();
	pos.x *= -1;
	pObject->SetPos(pos);
	pObject->Init();

	UpdateAllViews(NULL);
}

void CBZEdit32Doc::OnUpdateUtiltyDupeandflipx(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_oWorld.GetSelItem() != -1);
}

void CBZEdit32Doc::OnUtiltyDupeandflipy() 
{
	int iSelItem = m_oWorld.GetSelItem();

	CBaseObject	*pObject = m_oWorld.GetObject(m_oWorld.DupeObject(iSelItem));
	trVertex3D pos = pObject->GetPos();
	pos.y *= -1;
	pObject->SetPos(pos);
	pObject->Init();

	UpdateAllViews(NULL);
}

void CBZEdit32Doc::OnUpdateUtiltyDupeandflipy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_oWorld.GetSelItem() != -1);
	
}

BOOL CBZEdit32Doc::SaveModified() 
{
	if (IsModified())
	{
		if (m_oFile.GetLength()==0)
			OnFileSaveAs();
		else
			m_oWorld.Write(m_oFile);
			
		SetModifiedFlag(false);
	}
	return TRUE;//CDocument::SaveModified();
}

// API
void UpdateWorld ( void )
{
	pTheDoc->UpdateAllViews(NULL);
}

void SetDocDirty ( void )
{
	pTheDoc->SetModifiedFlag(true);
}

int GetObjectCount ( void )
{
	return pTheWorld->Size();
}

int AddObject ( const char *szClass )
{
	return pTheWorld->AddObject(szClass);
}

int DupeObject ( int iID )
{
	return pTheWorld->DupeObject(iID);
}

void RemoveObject ( int iID )
{
	pTheWorld->RemoveObject(iID);
}

int GetSelectedObject ( void )
{
	return pTheWorld->GetSelItem();
}

bool GetObjectClass(  int iID, char *szClass )
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;
	
	strcpy(szClass,pObject->GetTypeName());
	return true;
}

bool GetObjectName ( int iID, char *szName )
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;
	
	strcpy(szName,pObject->GetItemName());
	return true;
}

bool GetObjectPos ( int iID, float *afPos )
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;
	
	afPos[0] = pObject->GetPos().x;
	afPos[1] = pObject->GetPos().y;
	afPos[2] = pObject->GetPos().z;
	return true;
}

bool GetObjectRot ( int iID, float *pfRot )
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;
	
	pfRot[0] = pObject->GetRotAngle();
	return true;
}

bool GetObjectScale ( int iID, float *afScale )
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;
	
	afScale[0] = pObject->GetScale().x;
	afScale[1] = pObject->GetScale().y;
	afScale[2] = pObject->GetScale().z;
	return true;
}

bool SetObjectName ( int iID, char *szName )
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;
	
	pObject->SetItemName(szName);
	return true;
}

bool SetObjectPos ( int iID, float *afPos )
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;
	
	pObject->SetXPos(afPos[0]);
	pObject->SetYPos(afPos[1]);
	pObject->SetZPos(afPos[2]);
	return true;
}

bool SetObjectRot ( int iID, float fRot )
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;
	
	pObject->SetRotAngle(fRot);
	return true;
}

bool SetObjectScale ( int iID, float *afScale )
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;
	
	pObject->SetXScale(afScale[0]);
	pObject->SetXScale(afScale[1]);
	pObject->SetXScale(afScale[2]);
	return true;
}

bool ApplyObjectChanges ( int iID )
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;
	pObject->Init();
	return true;
}

bool GetObjectInfoI ( int iID, char *szLabel, int *data)
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;

	pObject->GetItemInfo(szLabel,*data);
	return true;
}

bool GetObjectInfoF ( int iID, char *szLabel, float *data)
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;

	pObject->GetItemInfo(szLabel,*data);
	return true;
}

bool GetObjectInfoS ( int iID, char *szLabel, char *data)
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;

	pObject->GetItemInfo(szLabel,data);
	return true;
}


bool SetObjectInfoI ( int iID, char *szLabel, int *data)
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;

	pObject->SetItemInfo(szLabel,*data);
	return true;
}

bool SetObjectInfoF ( int iID, char *szLabel, float *data)
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;

	pObject->SetItemInfo(szLabel,*data);
	return true;
}

bool SetObjectInfoS ( int iID, char *szLabel, char *data)
{
	CBaseObject *pObject = pTheWorld->GetObject(iID);
	if (!pObject)
		return false;

	pObject->GetItemInfo(szLabel,data);
	return true;
}
