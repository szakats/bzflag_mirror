
#include "stdafx.h"
#include "resource.h"
#include "objects.h"
#include "world.h"
//#include "textures.h"
#include <windows.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "StdObjectInfoPanel.h"
#include "LinkDlog.h"

#include <stdio.h>
#include <string.h>

#include "Utils.h"

GroundFactory		GroundFac;
WallsFactory		WallsFac;
BoxFactory			BoxFac;
PyramidFactory		PyramidFac;
BaseFactory			BaseFac;
TeleporterFactory	TeleporterFac;
LinkFactory			LinkFac;

void RegisterClasses ( CWorld *pWorld )
{
	pWorld->RegisterFactory("Walls",&WallsFac);
	pWorld->RegisterFactory("Ground",&GroundFac);
	pWorld->RegisterFactory("Box",&BoxFac);
	pWorld->RegisterFactory("Pyramid",&PyramidFac);
	pWorld->RegisterFactory("Base",&BaseFac);
	pWorld->RegisterFactory("Teleporter",&TeleporterFac);
	pWorld->RegisterFactory("Link",&LinkFac);
}
// gorund plane

bool Ground::ShowInInterface ( void )
{
	return false;
}

bool Ground::Init ( void )
{
	if (!m_pTextureMan || !m_pModelMan )
		return false;
	
	if (!m_pModelMan)
		return false;

	iTexture = m_pTextureMan->GetID("ground");

	iModel = m_pModelMan->NewModel("Ground");

	CBaseModel	*pModel = m_pModelMan->GetModel(iModel);

	trVertex3D	p; // just a dummy
	p.x = p.y = p.z = 0;

	pModel->SetTexuture(iTexture);
	pModel->Init(p,p);

	m_sTypeName = "ground";

	return true;
}

void Ground::Render( bool transparent, bool bSelected, int iName )
{
	if (m_pModelMan)
		m_pModelMan->AddDrawItem(iModel,transparent,bSelected,iName,eFirst);
}

// walls
bool Walls::ShowInInterface ( void )
{
	return false;
}

bool Walls::Init ( void )
{
	iTexture = m_pTextureMan->GetID("wall");

	if (!m_pModelMan)
		return false;

	iModel = m_pModelMan->NewModel("Walls");

	CBaseModel	*pModel = m_pModelMan->GetModel(iModel);

	trVertex3D	p; // just a dummy
	p.x = p.y = p.z = 0;

	pModel->SetTexuture(iTexture);
	pModel->Init(p,p);

	m_sTypeName = "walls";

	return true;
}

void Walls::Render( bool transparent, bool bSelected, int iName  )
{
	if (m_pModelMan)
	{
		m_pModelMan->AddDrawItem(iModel,transparent,bSelected,iName,eFirst);
	}
}

Box::Box()
{
	CBaseObject::SetDefalutData();
	
	iSidesModel = -1;
	iTopsModel = -1;

	m_sTypeName = "Box";
}

Box::Box(const Box &r)
{
	CBaseObject::SetDefalutData();
	CBaseObject::CopyFrom(&(CBaseObject)r);

	iWallTexture = r.iWallTexture;
	iRoofTexture = r.iRoofTexture;

	iSidesModel = -1;
	iTopsModel = -1;
}

Box Box::operator = (const Box &r)
{
	CBaseObject::CopyFrom(&(CBaseObject)r);

	iWallTexture = r.iWallTexture;
	iRoofTexture = r.iRoofTexture;

	Init();
	return *this;
}

void Box::CopyFrom (CBaseObject *r)
{
	CBaseObject::CopyFrom(r);

	iWallTexture = ((Box*)r)->iWallTexture;
	iRoofTexture = ((Box*)r)->iRoofTexture;
	Init();
}

bool Box::Init ( void )
{
	iWallTexture = m_pTextureMan->GetID("boxwall");
	iRoofTexture = m_pTextureMan->GetID("roof");

	if (!m_pModelMan)
		return false;

	if (iSidesModel <0)
		iSidesModel = m_pModelMan->NewModel("BoxSides");

	CBaseModel	*pModel = m_pModelMan->GetModel(iSidesModel);

	pModel->SetTexuture(iWallTexture);
	pModel->Init(m_rPosition,m_rScale,m_fAngle);

	if (iTopsModel <0)
		iTopsModel = m_pModelMan->NewModel("BoxTops");

	pModel = m_pModelMan->GetModel(iTopsModel);
	pModel->SetTexuture(iRoofTexture);

	pModel->Init(m_rPosition,m_rScale,m_fAngle);

	m_sTypeName = "Box";

	return true;
}

void Box::Render( bool transparent, bool bSelected, int iName )
{
	if (m_pModelMan)
	{
		teDrawPriority	eP = eNormal;
		if (transparent)
			eP = eTransNoZ;
		else if (bSelected)
			eP = eLast;

		m_pModelMan->AddDrawItem(iSidesModel,transparent,bSelected,iName,eP);
		m_pModelMan->AddDrawItem(iTopsModel,transparent,bSelected,iName,eP);
	}
}

void Box::Write(ostream &stream)
{
	WriteStdData(stream);
	stream << "size " << m_rScale.x << ' ' << m_rScale.y << ' ' << m_rScale.z << endl;
	stream << "end" << endl;
	stream << endl;
}

void Box::Write( char *data )
{
	char	szTemp[1024] = {0};

	WriteStdData(data);
	sprintf(szTemp,"size %f %f %f\nend\n\n",m_rScale.x,m_rScale.y,m_rScale.z);
	strcat(data,szTemp);
}

bool Box::Read( char *data )
{
	char	line[255];
	char	*pPtr = data;
	char	name[255];

	bool	bDone = false;
	while (!bDone)
	{
		bDone = !GetLine( &pPtr, line );

		if (!StdDataField(line))
		{
			sscanf(line,"%s",name);

			if (stricmp(name,"end") ==0)
				bDone = true;
		}
	}
	return true;
}

void* Box::GetIcon ( void )
{
	return (void*)LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_BOXWALL_ICON));
}

// pyramid

Pyramid::Pyramid()
{
	CBaseObject::SetDefalutData();
	m_rScale.x = m_rScale.y = m_rScale.z = 1.0f;

	iModel = -1;
}

Pyramid::Pyramid(const Pyramid &r)
{
	CBaseObject::CopyFrom(&(CBaseObject)r);
	iTexture = r.iTexture;
}

Pyramid Pyramid::operator = (const Pyramid &r)
{
	CBaseObject::CopyFrom(&(CBaseObject)r);
	iTexture = r.iTexture;
	return *this;
}

void Pyramid::CopyFrom (CBaseObject *r)
{
	CBaseObject::CopyFrom(r);
	iTexture = ((Pyramid*)r)->iTexture;
	Init();
}

bool Pyramid::Init ( void )
{
	if (!m_pTextureMan || !m_pModelMan )
		return false;
	
	if (!m_pModelMan)
		return false;

	iTexture = m_pTextureMan->GetID("pyrwall");

	if (iModel<0)
		iModel = m_pModelMan->NewModel("Pyramid");

	CBaseModel	*pModel = m_pModelMan->GetModel(iModel);


	pModel->SetTexuture(iTexture);
	pModel->Init(m_rPosition,m_rScale,m_fAngle);

	m_sTypeName = "Pyramid";

	return true;
}

void Pyramid::Render( bool transparent, bool bSelected, int iName )
{
	if (m_pModelMan)
	{
		teDrawPriority	eP = eNormal;
		if (transparent)
			eP = eTransNoZ;
		else if (bSelected)
			eP = eLast;

		m_pModelMan->AddDrawItem(iModel,transparent,bSelected,iName,eP);
	}
}

void Pyramid::Write(ostream &stream)
{
	WriteStdData(stream);
	stream << "size " << m_rScale.x << ' ' << m_rScale.y << ' ' << m_rScale.z << endl;
	stream << "end" << endl;
	stream << endl;
}

void Pyramid::Write( char *data )
{
	char	szTemp[1024] = {0};

	WriteStdData(data);
	sprintf(szTemp,"size %f %f %f\nend\n\n",m_rScale.x,m_rScale.y,m_rScale.z);
	strcat(data,szTemp);
}

bool Pyramid::Read( char *data )
{
	char	line[255];
	char	*pPtr = data;
	char	name[255];

	bool	bDone = false;
	while (!bDone)
	{
		bDone = !GetLine( &pPtr, line );

		if (!StdDataField(line))
		{
			sscanf(line,"%s",name);

			if (stricmp(name,"end") ==0)
				bDone = true;
		}
	}
	return true;
}

void* Pyramid::GetIcon ( void )
{
	return (void*)LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_PYRAMID_ICON));
}

// teleporter
Teleporter::Teleporter()
{
	CBaseObject::SetDefalutData();
	m_rScale.x = 0.25;
	m_rScale.x = 0.125f;

	border = 1;
	iFieldModel = -1;
	iFrameModel = -1;
	iFieldTexture = -1;
	iTexture = -1;
}

Teleporter::Teleporter(const Teleporter &r)
{
	CBaseObject::CopyFrom(&(CBaseObject)r);
	iTexture = r.iTexture;
	iFieldTexture = r.iTexture;
}

Teleporter Teleporter::operator = (const Teleporter &r)
{
	CBaseObject::CopyFrom(&(CBaseObject)r);
	iTexture = r.iTexture;
	iFieldTexture = r.iTexture;
	return *this;
}

void Teleporter::CopyFrom (CBaseObject *r)
{
	CBaseObject::CopyFrom(r);
	iTexture = ((Teleporter*)r)->iTexture;
	iFieldTexture = ((Teleporter*)r)->iFieldTexture;
	Init();
}

bool Teleporter::Init ( void )
{
	iTexture = m_pTextureMan->GetID("caution");
	iFieldTexture = m_pTextureMan->GetID("Field");

	if (!m_pModelMan)
		return false;

	if (iFieldModel <0)
		iFieldModel = m_pModelMan->NewModel("TPortField");

	CBaseModel	*pModel = m_pModelMan->GetModel(iFieldModel);

	if (!pModel)
		return false;
	
	
	pModel->SetTexuture(iFieldTexture);
	pModel->SetItemInfo("Border",&border);
	pModel->Init(m_rPosition,m_rScale,m_fAngle);

	if (iFrameModel <0)
		iFrameModel = m_pModelMan->NewModel("TPortFrame");

	pModel = m_pModelMan->GetModel(iFrameModel);

	if (!pModel)
		return false;

	pModel->SetItemInfo("Border",&border);
	pModel->SetTexuture(iTexture);
	pModel->Init(m_rPosition,m_rScale,m_fAngle);

	m_sTypeName = "Teleporter";

	return true;
}

void Teleporter::Render(bool transparent, bool bSelected, int iName )
{
	if (m_pModelMan)
	{
		teDrawPriority	eP = eNormal;
		if (transparent)
			eP = eTransNoZ;
		else if (bSelected)
			eP = eAlways;

		m_pModelMan->AddDrawItem(iFieldModel,transparent,bSelected,iName,eTranspaent);
		m_pModelMan->AddDrawItem(iFrameModel,transparent,bSelected,iName,eP);
	}
}

void Teleporter::Write(ostream &stream)
{
	WriteStdData(stream);
	stream << "size " << m_rScale.x << ' ' << m_rScale.y << ' ' << m_rScale.z << endl;
	stream << "border " << border << endl;
	stream << "end" << endl;
	stream << endl;
}

void Teleporter::Write( char *data )
{
	char	szTemp[1024] = {0};

	WriteStdData(data);
	sprintf(szTemp,"size %f %f %f\nborder %f\nend\n\n",m_rScale.x,m_rScale.y,m_rScale.z,border);
	strcat(data,szTemp);
}

bool Teleporter::Read( char *data )
{
	char	line[255];
	char	*pPtr = data,*pDataStart;
	char	name[255];

	bool	bDone = false;
	while (!bDone)
	{
		bDone = !GetLine( &pPtr, line );

		if (!StdDataField(line))
		{
			sscanf(line,"%s",name);
			pDataStart = line+strlen(name);

			if (stricmp(name,"end") ==0)
				bDone = true;
			else if (stricmp(name,"border") ==0)
				border = (float)atof(pDataStart);
		}
	}
	return true;
}

bool Teleporter::SetItemInfo ( char  *szName, float fData )
{
	if (stricmp(szName,"Border")==0)
	{
		border = fData;
		return true;
	}
	return false;
}

bool Teleporter::GetItemInfo ( char  *szName, float &fData )
{
	if (stricmp(szName,"Border")==0)
	{
		fData = border;
		return true;
	}
	return false;
}

void* Teleporter::GetIcon ( void )
{
	return (void*)LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_TPORT_ICON));
}

	// to be called by the editor
void* Teleporter::GetEditInterface ( void *pParam )
{
	if (m_pInterface)
		delete((CStdObjectInfoPanel*)m_pInterface);

	m_pInterface = new CStdObjectInfoPanel();

	if (!m_pInterface)
		return NULL;

	((CStdObjectInfoPanel*)m_pInterface)->m_bShowBorder = true;
	((CStdObjectInfoPanel*)m_pInterface)->Create(IDD_STD_INFO_DLOG,(CWnd*)pParam);
	((CStdObjectInfoPanel*)m_pInterface)->ShowWindow(SW_NORMAL);

	return m_pInterface;
}

void Teleporter::CloseEditInterface ( void )
{
	if (!m_pInterface)
		return;

	((CStdObjectInfoPanel*)m_pInterface)->EndDialog(IDOK);
	delete((CStdObjectInfoPanel*)m_pInterface);

	m_pInterface = NULL;
}

void Teleporter::UpdateEditInfo ( void )
{
	if (!m_pInterface)
		return;

	((CStdObjectInfoPanel*)m_pInterface)->m_fXPos = m_rPosition.x;
	((CStdObjectInfoPanel*)m_pInterface)->m_fYPos = m_rPosition.y;
	((CStdObjectInfoPanel*)m_pInterface)->m_fZPos = m_rPosition.z;

	((CStdObjectInfoPanel*)m_pInterface)->m_fXScale = m_rScale.x;
	((CStdObjectInfoPanel*)m_pInterface)->m_fYScale = m_rScale.y;
	((CStdObjectInfoPanel*)m_pInterface)->m_fZScale = m_rScale.z;

	((CStdObjectInfoPanel*)m_pInterface)->m_fZRot = m_fAngle;

	((CStdObjectInfoPanel*)m_pInterface)->m_fBorderValue = border;

	((CStdObjectInfoPanel*)m_pInterface)->UpdateData(false);
	((CStdObjectInfoPanel*)m_pInterface)->UpdateWindow();
}

void Teleporter::ApplyEdit ( void )
{
	if (!m_pInterface)
		return;
	((CStdObjectInfoPanel*)m_pInterface)->UpdateData(true);
	
	m_rPosition.x = ((CStdObjectInfoPanel*)m_pInterface)->m_fXPos;
	m_rPosition.y = ((CStdObjectInfoPanel*)m_pInterface)->m_fYPos;
	m_rPosition.z = ((CStdObjectInfoPanel*)m_pInterface)->m_fZPos;

	m_rScale.x = ((CStdObjectInfoPanel*)m_pInterface)->m_fXScale;
	m_rScale.y = ((CStdObjectInfoPanel*)m_pInterface)->m_fYScale;
	m_rScale.z = ((CStdObjectInfoPanel*)m_pInterface)->m_fZScale;

	m_fAngle = ((CStdObjectInfoPanel*)m_pInterface)->m_fZRot;
	border = ((CStdObjectInfoPanel*)m_pInterface)->m_fBorderValue;

	Init();
}

void Teleporter::RevertEdit ( void )
{
	UpdateEditInfo();
}


// base
Base::Base() : CBaseObject()
{
	CBaseObject::SetDefalutData();

	color = 1;

	m_rScale.x = 10;
	m_rScale.y = 10;
	m_rScale.z = 10;

	iBaseModel = -1;
	iWallsModel = -1;
}

Base::Base(const Base &b)
{
	CBaseObject::CopyFrom(&(CBaseObject)b);
	color = b.color;
}

Base Base::operator = (const Base &b)
{
	CBaseObject::CopyFrom(&(CBaseObject)b);
	color = b.color;
	return *this;
}

void Base::CopyFrom (CBaseObject *r)
{
	CBaseObject::CopyFrom(r);

	color = ((Base*)r)->color;
	Init();
}

bool Base::Init( void )
{
	int iWallTexture = m_pTextureMan->GetID("boxwall");

	if (!m_pModelMan)
		return false;

	if (iBaseModel <0)
		iBaseModel = m_pModelMan->NewModel("BaseTops");

	CBaseModel	*pModel = m_pModelMan->GetModel(iBaseModel);

	if (!pModel)
		return false;
		
	pModel->SetTexuture(-1);

	float	fTemp = color;
	pModel->SetItemInfo("Color",&fTemp);

	pModel->Init(m_rPosition,m_rScale,m_fAngle);

	if (iWallsModel <0)
	{
		if (m_rPosition.z >0)
			iWallsModel = m_pModelMan->NewModel("BoxSides");
	}
	else
	{
		if (m_rPosition.z <=0)
		{
			m_pModelMan->KillModel(iWallsModel);
			iWallsModel = -1;
		}
	}

	if (iWallsModel != -1 )
	{
		pModel = m_pModelMan->GetModel(iWallsModel);

		if (!pModel)
			return false;

		m_rScale.z = 1.0f;
		pModel->SetTexuture(iWallTexture);
		pModel->Init(m_rPosition,m_rScale,m_fAngle);
	}

	m_sTypeName = "Base";

	return true;
}

void Base::Render( bool transparent, bool bSelected, int iName )
{
	if (m_pModelMan)
	{
		teDrawPriority	eP = eNormal;
		if (transparent)
			eP = eTransNoZ;
		else if (bSelected)
			eP = eAlways;

		m_pModelMan->AddDrawItem(iBaseModel,transparent,bSelected,iName,eP);
		if (iWallsModel != -1)
			m_pModelMan->AddDrawItem(iWallsModel,transparent,bSelected,iName,eP);
	}
}

bool Base::SetItemInfo ( char  *szName, int iData )
{
	if (stricmp(szName,"Color")==0)
	{
		color = iData;
		return true;
	}
	return false;
}

bool Base::GetItemInfo ( char  *szName, int &iData )
{
	if (stricmp(szName,"Color")==0)
	{
		iData = color;
		return true;
	}
	return false;
}

void Base::Write(ostream &stream)
{
	WriteStdData(stream);
	stream << "size " << m_rScale.x << ' ' << m_rScale.y << ' ' << m_rScale.z << endl;
	stream << "color " << color << endl;
	stream << "end" << endl;
	stream << endl;
}

void Base::Write( char *data )
{
	char	szTemp[1024] = {0};

	WriteStdData(data);
	sprintf(szTemp,"size %f %f %f\ncolor %d\nend\n\n",m_rScale.x,m_rScale.y,m_rScale.z,color);
	strcat(data,szTemp);
}

bool Base::Read( char *data )
{
	char	line[255];
	char	*pPtr = data,*pDataStart;
	char	name[255];

	bool	bDone = false;
	while (!bDone)
	{
		bDone = !GetLine( &pPtr, line );

		if (!StdDataField(line))
		{
			sscanf(line,"%s",name);
			pDataStart = line+strlen(name);

			if (stricmp(name,"end") ==0)
				bDone = true;
			else if (stricmp(name,"color") ==0)
				color = (int)atoi(pDataStart);
		}
	}
	return true;
}

void* Base::GetIcon ( void )
{
	return (void*)LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_BASE_ICON));
}

	// to be called by the editor
void* Base::GetEditInterface ( void *pParam )
{
	if (m_pInterface)
		delete((CStdObjectInfoPanel*)m_pInterface);

	m_pInterface = new CStdObjectInfoPanel();

	if (!m_pInterface)
		return NULL;

	((CStdObjectInfoPanel*)m_pInterface)->m_bShowTeam = true;
	((CStdObjectInfoPanel*)m_pInterface)->Create(IDD_STD_INFO_DLOG,(CWnd*)pParam);
	((CStdObjectInfoPanel*)m_pInterface)->ShowWindow(SW_NORMAL);
	((CStdObjectInfoPanel*)m_pInterface)->m_cTeamList.SetCurSel(color-1);
	return m_pInterface;
}

void Base::CloseEditInterface ( void )
{
	if (!m_pInterface)
		return;

	((CStdObjectInfoPanel*)m_pInterface)->EndDialog(IDOK);
	delete((CStdObjectInfoPanel*)m_pInterface);

	m_pInterface = NULL;
}

void Base::UpdateEditInfo ( void )
{
	if (!m_pInterface)
		return;

	((CStdObjectInfoPanel*)m_pInterface)->m_fXPos = m_rPosition.x;
	((CStdObjectInfoPanel*)m_pInterface)->m_fYPos = m_rPosition.y;
	((CStdObjectInfoPanel*)m_pInterface)->m_fZPos = m_rPosition.z;

	((CStdObjectInfoPanel*)m_pInterface)->m_fXScale = m_rScale.x;
	((CStdObjectInfoPanel*)m_pInterface)->m_fYScale = m_rScale.y;
	((CStdObjectInfoPanel*)m_pInterface)->m_fZScale = m_rScale.z;

	((CStdObjectInfoPanel*)m_pInterface)->m_fZRot = m_fAngle;

	((CStdObjectInfoPanel*)m_pInterface)->m_cTeamList.GetItemData(color);

	((CStdObjectInfoPanel*)m_pInterface)->UpdateData(false);
	((CStdObjectInfoPanel*)m_pInterface)->UpdateWindow();
	
}

void Base::ApplyEdit ( void )
{
	if (!m_pInterface)
		return;
	((CStdObjectInfoPanel*)m_pInterface)->UpdateData(true);
	
	m_rPosition.x = ((CStdObjectInfoPanel*)m_pInterface)->m_fXPos;
	m_rPosition.y = ((CStdObjectInfoPanel*)m_pInterface)->m_fYPos;
	m_rPosition.z = ((CStdObjectInfoPanel*)m_pInterface)->m_fZPos;

	m_rScale.x = ((CStdObjectInfoPanel*)m_pInterface)->m_fXScale;
	m_rScale.y = ((CStdObjectInfoPanel*)m_pInterface)->m_fYScale;
	m_rScale.z = ((CStdObjectInfoPanel*)m_pInterface)->m_fZScale;

	m_fAngle = ((CStdObjectInfoPanel*)m_pInterface)->m_fZRot;
	
	color = ((CStdObjectInfoPanel*)m_pInterface)->m_cTeamList.GetItemData(((CStdObjectInfoPanel*)m_pInterface)->m_cTeamList.GetCurSel());

	Init();
}

void Base::RevertEdit ( void )
{
	UpdateEditInfo();
}


// link
Link::Link()
{
	CBaseObject::SetDefalutData();
	m_pWorld = NULL;
	from_side = -1;
	to_side = -1;
	iLinkModel = -1;
}

Link::Link(const Link &r)
{
	from_side = r.from_side;
	to_side = r.to_side;
}

Link Link::operator = (const Link &r)
{
	from_side = r.from_side;
	to_side = r.to_side;
	return *this;
}

void Link::CopyFrom (CBaseObject *r)
{	
	from_side = ((Link*)r)->from_side;
	to_side = ((Link*)r)->to_side;
	Init();	
}

bool Link::Init ( void )
{
	int iLinkTexture = m_pTextureMan->GetID("Links");

	if (!m_pModelMan)
		return false;

	if (iLinkModel <0)
		iLinkModel = m_pModelMan->NewModel("Link");

	CBaseModel	*pModel = m_pModelMan->GetModel(iLinkModel);

	if (!pModel)
		return false;
	
	pModel->SetTexuture(iLinkTexture);

	pModel->SetItemInfo("From",(float*)&from_side);
	pModel->SetItemInfo("To",(float*)&to_side);
	pModel->SetItemInfo("World",(float*)m_pWorld);

	m_sTypeName = "Link";

	return true;
}

void Link::Render(bool transparent, bool bSelected, int iName)
{
	if (m_pModelMan && ( (from_side != -1) && (to_side != -1) ) )
	{
		m_pModelMan->AddDrawItem(iLinkModel,transparent,bSelected,iName,eTransNoZ);
	}
}

void Link::Write(ostream &stream)
{
	stream << "link" << endl;

	if (m_sItemName.size()>0)
		stream << "#!name " << m_sItemName.c_str() << endl;

	if (m_iGroupID != -1)
		stream << "#!group " <<  m_iGroupID << ' ' << m_sGroupName.c_str() << endl;

	stream << "from " << from_side << endl;
	stream << "to " << to_side << endl;
	stream << "end" << endl;
	stream << endl;
}

void Link::Write( char *data )
{
	sprintf(data,"link\n");
	
	char	szTemp[256];

	if (m_sItemName.size()>0)
	{
		sprintf(szTemp,"#!name %s\n",m_sItemName.c_str());
		strcat(data,szTemp);
	}

	if (m_iGroupID != -1)
	{
		sprintf(szTemp,"#!group %d %s\n",m_iGroupID,m_sGroupName.c_str());
		strcat(data,szTemp);
	}

	char szData[256];
	sprintf(szData,"from %d\nto %d\nend\n\n",from_side,to_side);
	strcat(data,szData);
}

bool Link::Read( char *data )
{
	char	line[255];
	char	*pPtr = data,*pDataStart;
	char	name[255];

	bool	bDone = false;
	while (!bDone)
	{
		bDone = !GetLine( &pPtr, line );
		if (!StdDataField(line))
		{
			sscanf(line,"%s",name);
			pDataStart = line+strlen(name);

			if (stricmp(name,"end") ==0)
				bDone = true;
			else if (stricmp(name,"from") ==0)
				from_side = (int)atoi(pDataStart);
			else if (stricmp(name,"to") ==0)
				to_side = (int)atoi(pDataStart);
		}
	}
	return true;
}

bool Link::SetItemInfo ( char  *szName, int iData )
{
	if (stricmp(szName,"World")==0)
	{
		SetWorld((CWorld*)iData);
		return true;
	}
	else if (stricmp(szName,"FromSide")==0)
	{
		from_side = iData;
		return true;
	}
	else if (stricmp(szName,"ToSide")==0)
	{
		to_side = iData;
		return true;
	}
	return false;
}

bool Link::GetItemInfo ( char  *szName, int &iData )
{
	if (stricmp(szName,"FromSide")==0)
	{
		iData = from_side;
		return true;
	}
	else if (stricmp(szName,"ToSide")==0)
	{
		iData = to_side;
		return true;
	}
	return false;
}

void* Link::GetIcon ( void )
{
	return (void*)LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_LINK_ICON));
}

	// to be called by the editor
void* Link::GetEditInterface ( void *pParam )
{
	if (m_pInterface)
		delete((CLinkDlog*)m_pInterface);

	m_pInterface = new CLinkDlog();

	if (!m_pInterface)
		return NULL;

	((CLinkDlog*)m_pInterface)->m_pWorld = m_pWorld;
	((CLinkDlog*)m_pInterface)->Create(IDD_LINK_INTERFACE,(CWnd*)pParam);
	((CLinkDlog*)m_pInterface)->ShowWindow(SW_NORMAL);
	((CLinkDlog*)m_pInterface)->UpdateWindow();
	return m_pInterface;
}

void Link::CloseEditInterface ( void )
{
	if (!m_pInterface)
		return;

	((CLinkDlog*)m_pInterface)->EndDialog(IDOK);
	delete((CLinkDlog*)m_pInterface);

	m_pInterface = NULL;
}

void Link::UpdateEditInfo ( void )
{
	if (!m_pInterface)
		return;
	((CLinkDlog*)m_pInterface)->SetSel(to_side,from_side);
}

void Link::RevertEdit ( void )
{
	UpdateEditInfo();
}

void Link::ApplyEdit ( void )
{
	if (!m_pInterface)
		return;
	((CLinkDlog*)m_pInterface)->GetSel(to_side,from_side);

	Init();
}
