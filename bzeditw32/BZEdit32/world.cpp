#ifdef WIN32
	#pragma warning( disable : 4786 )  // Disable warning message for stl maps
	
#endif
#include "stdafx.h"
#include "resource.h"

#include <stdio.h>
#include <math.h>
#include <fstream>
#include "Utils.h"
#include "objects.h"
#include "world.h"
#include "StdObjectInfoPanel.h"
#include "TextureMngr.h"
#include "Model.h"


// object case class
CBaseObject::CBaseObject()
{
	SetDefalutData();
}

CBaseObject::~CBaseObject()
{
}

CBaseObject::CBaseObject(const CBaseObject &r)
{
	Init();
	(*this) = r;
}

void CBaseObject::CopyFrom (CBaseObject *r)
{
	if (!r)
		return;
	
	m_rPosition = r->m_rPosition;
	m_rScale = r->m_rScale;
	m_fAngle = r->m_fAngle;

	m_sTypeName = r->m_sTypeName;
	m_sItemName = r->m_sItemName;

	m_iGroupID = r->m_iGroupID;
	m_sGroupName = r->m_sGroupName;

	m_bVisible = r->m_bVisible;
	m_bSelected = r->m_bSelected;

	m_pTextureMan =r->m_pTextureMan;
	m_pModelMan = r->m_pModelMan;
}

void CBaseObject::SetDefalutData ( void )
{
	m_pInterface = NULL;
	m_pTextureMan = NULL;
	m_pModelMan = NULL;

	m_rPosition.x = m_rPosition.y = m_rPosition.z = 0;
	m_rScale.x = m_rScale.y = m_rScale.z = 10;
	m_fAngle = 0;

	m_sTypeName.empty();
	m_sItemName.empty();

	m_iGroupID = -1;
	m_iTypeID = -1;
	m_iListID = -1;

	m_bVisible = true;
	m_bSelected = false;

	m_bDriveThru = false;
	m_bShootThru = false;
	m_bFlipZ = false;
}

bool CBaseObject::Init ( void )
{
	return false;
}

void CBaseObject::Render(bool transparent, bool bSelected, int iName  )
{
	// just stubed
}

const char*	CBaseObject::GetListName ( void )
{
	m_sListName.empty();

	char	szId[255];

	sprintf(szId,"%d-",m_iListID);

	m_sListName = szId;

	if (m_sItemName.size()>0)
		m_sListName += m_sItemName;
	else
		m_sListName += m_sTypeName;

	return m_sListName.c_str();
}

const char*	CBaseObject::GetTypeName ( void )
{
	return m_sTypeName.c_str();
}

bool CBaseObject::StdDataField ( char *line )
{
	char	name[256],temp[256],*pNameEnd;

	sscanf(line,"%s",name);
	pNameEnd = line+strlen(name);

	if (stricmp(name,"position") ==0)
	{
		sscanf(pNameEnd," %f %f %f",&m_rPosition.x,&m_rPosition.y,&m_rPosition.z);
	}
	else if (stricmp(name,"rotation") ==0)
	{
		sscanf(pNameEnd," %f",&m_fAngle);
	}
	else if (stricmp(name,"size") ==0)
	{
		sscanf(pNameEnd," %f %f %f",&m_rScale.x,&m_rScale.y,&m_rScale.z);
	}
	else if (stricmp(name,"name") ==0)
	{
	//	sscanf(pNameEnd," %s",&temp);

		char	szTempString[512];
		strcpy(szTempString,pNameEnd);

		char *pStringStart = szTempString;

		while ((*pStringStart != NULL) && isspace(*pStringStart))
			pStringStart++;

		if (*pStringStart == NULL)
			return true;

		char * pStringEnd = pStringStart;

		while ( (*pStringEnd != NULL) && (*pStringEnd != '/') )
			pStringEnd++;

		if (*pStringEnd != NULL)
		{
			if (*pStringEnd == '/')
			{
				pStringEnd--;

				while ( (pStringEnd >pStringStart) && isspace(*pStringEnd) )
					pStringEnd--;

				*pStringEnd = NULL;
			}
		}
		m_sItemName = pStringStart;
	}
	else if (stricmp(name,"#!group") ==0)
	{
		sscanf(pNameEnd," %d %s",&m_iGroupID,&temp);
		m_sGroupName = temp;
	}
        else if (stricmp(name,"passable") ==0)
            m_bDriveThru = m_bShootThru = true;
        else if (stricmp(name,"drivethrough") ==0)
            m_bDriveThru = true;
        else if (stricmp(name,"shoothrough") ==0)
            m_bShootThru = true;
	else
		return false;

	return true;
}

void CBaseObject::WriteStdData ( std::ostream &stream )
{
	char	szName[512];
	strcpy(szName,m_sTypeName.c_str());
	strlwr(szName);
	stream << szName << std::endl;

	if (m_sItemName.size()>0)
		stream << "name " << m_sItemName.c_str() << std::endl;

	if (m_iGroupID != -1)
		stream << "#!group " <<  m_iGroupID << " " << m_sGroupName.c_str() << std::endl;

	stream << "	position " << m_rPosition.x << ' ' << m_rPosition.y << ' ' << m_rPosition.z << std::endl;
	stream << "	rotation " << m_fAngle << std::endl;
}

void CBaseObject::WriteStdData ( char *data )
{
	char	szTemp[1024];

	char	szName[512];
	strcpy(szName,m_sTypeName.c_str());
	strlwr(szName);
	//	sprintf(data,"pyramid\nposition %f %f %f\nrotation %f\nsize %f %f %f\nend\n\n",

	sprintf(szTemp,"%s\n",szName);
	strcat(data,szTemp);

	if (m_sItemName.size()>0)
	{
		sprintf(szTemp,"name %s\n",m_sItemName.c_str());
		strcat(data,szTemp);
	}

	if (m_iGroupID != -1)
	{
		sprintf(szTemp,"#!group %d %s\n",m_iGroupID,m_sGroupName.c_str());
		strcat(data,szTemp);
	}

	sprintf(szTemp,"\tposition  %f %f %f\n\trotation  %f\n",m_rPosition.x,m_rPosition.y,m_rPosition.z,m_fAngle);
	strcat(data,szTemp);
}

bool CBaseObject::ShowInInterface ( void )
{
	return true;
}

bool CBaseObject::ForcedAdd ( void )
{
	return false;
}

bool CBaseObject::OnlyOne ( void )
{
	return false;
}
	// to be called by the editor
void* CBaseObject::GetEditInterface ( void *pParam )
{
	if (m_pInterface)
		delete((CStdObjectInfoPanel*)m_pInterface);

	m_pInterface = new CStdObjectInfoPanel();

	if (!m_pInterface)
		return NULL;

	((CStdObjectInfoPanel*)m_pInterface)->Create(IDD_STD_INFO_DLOG,(CWnd*)pParam);
	((CStdObjectInfoPanel*)m_pInterface)->ShowWindow(SW_NORMAL);

	return m_pInterface;
}

void CBaseObject::CloseEditInterface ( void )
{
	if (!m_pInterface)
		return;

	((CStdObjectInfoPanel*)m_pInterface)->EndDialog(IDOK);
	delete((CStdObjectInfoPanel*)m_pInterface);

	m_pInterface = NULL;
}

void CBaseObject::UpdateEditInfo ( void )
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

        ((CStdObjectInfoPanel*)m_pInterface)->m_bShootThru = m_bShootThru;
        ((CStdObjectInfoPanel*)m_pInterface)->m_bDriveThru = m_bDriveThru;

	((CStdObjectInfoPanel*)m_pInterface)->UpdateData(false);
	((CStdObjectInfoPanel*)m_pInterface)->UpdateWindow();
}

void CBaseObject::ApplyEdit ( void )
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
	m_bShootThru = ((CStdObjectInfoPanel*)m_pInterface)->m_bShootThru != 0;
	m_bDriveThru = ((CStdObjectInfoPanel*)m_pInterface)->m_bDriveThru != 0;

	Init();
}

void CBaseObject::RevertEdit ( void )
{
	UpdateEditInfo();
}


void* CBaseObject::GetIcon ( void )
{
	return NULL;
}	


// world class
CWorld::CWorld()
{
	// list gui stuff outside of this class
	m_pTextureMan = NULL;
	m_bInited = false;
	m_bFadeNonSelect = true;

	m_bDrawAll = false;
	m_iSelItem = -1;
	m_pModelMan = new CModelManager;

	m_iNextGUID = 0;
}

CWorld::CWorld(CWorld &rWorld)
{
	m_pTextureMan = rWorld.m_pTextureMan;
	m_vObjList = rWorld.m_vObjList;

	m_bInited = false;
	m_bFadeNonSelect = true;
	
	m_iSelItem = -1;
	m_pModelMan = new CModelManager;
}

CWorld::~CWorld()
{
	Clear();
}

bool CWorld::Inited ( void )
{
	return m_bInited;
}

void CWorld::Clear ( void )
{
	trObjectMap::iterator itr = m_vObjList.begin();
	while(itr != m_vObjList.end())
	{
		if(itr->second)
			delete(itr->second);
		itr++;
	}
	m_vObjList.clear();

	m_iNextGUID = 0;
}

void CWorld::Init ( void )
{
	if (!m_pTextureMan)
		return;

	m_pModelMan->Init();
	m_pModelMan->SetTextureMan(m_pTextureMan);
	
	m_pGround = (CBaseObject*)new Ground;
	m_pGround->SetTextureMan(m_pTextureMan);
	m_pGround->SetModelMan(m_pModelMan);
	m_pGround->Init();

	m_pWalls = (CBaseObject*)new Walls;
	m_pWalls->SetTextureMan(m_pTextureMan);
	m_pWalls->SetModelMan(m_pModelMan);
	m_pWalls->Init();

	m_iNextGUID = 0;
	m_bInited = true;
}

void CWorld::Render ( void )
{
	if (!Inited())
		return;

	m_pModelMan->ClearDrawList();

	m_pGround->Render();
	m_pWalls->Render();

	CBaseObject	*pSelected = NULL;

	trObjectMap::iterator itr = m_vObjList.begin();
	while(itr != m_vObjList.end())
	{
		if (m_iSelItem != (itr->second)->GetID())
			(itr->second)->Render(m_bFadeNonSelect && (m_iSelItem != -1),false);
		else
			pSelected = (itr->second);
		itr++;
	}

	if (pSelected)
		pSelected->Render(false,true);

	m_pModelMan->DrawList();
}

void CWorld::RenderTargets ( void )
{
	if (!Inited())
		return;

	m_pModelMan->ClearDrawList();

	trObjectMap::iterator	itr = m_vObjList.begin();
	while(itr !=m_vObjList.end())
	{
		(itr->second)->Render(false,false,(itr->second)->GetID());
		itr++;
	}
	m_pModelMan->DrawList();
}

bool CWorld::Load ( const char *szFileName )
{	
	Clear();
	return Append(szFileName);
}

bool CWorld::Append ( const char *szFileName )
{
	if (!szFileName)
		return false;

	FILE *fp = fopen(szFileName,"rt");

	if (!fp)
		return false;

	int		iCount = 0;
	char	temp;

	while (fread(&temp,1,1,fp) != 0)
		iCount++;

	fseek(fp,0,SEEK_SET);

	char*	pData = (char*)malloc(iCount+1);

	if (!pData)
	{
		fclose(fp);
		return FALSE;
	}

	fread(pData,iCount,1,fp);
	pData[iCount] = NULL;
	fclose(fp);

	// now we got the data

	char* pPtr = pData, element[1024],name[255];
	bool	bDone = false;

	CBaseObject	*Item;
	char	Name[255];
	int		iNewItem;

	while (!bDone)
	{
		bDone = GetNextElement(&pPtr,element,Name);

		if (element[0])
		{
			sscanf(element,"%s",name);

			for ( int i = strlen(name)-1; i >= 0;i--)
			{
				if (isspace(name[i]))
					name[i] = NULL;
				else
					i = -1;
			}

			iNewItem = AddObject(name);

			Item = GetObject(iNewItem);

			if (Item)
			{
				Item->Read(element);
				Item->Init();
				Item = NULL;
			}
		}
	}
	free(pData);
	return true;
}

bool CWorld::Write(const char *szFileName)
{
	if (m_vObjList.size() <1) // never ever ever save a blank map, it's bad
		return false;

	std::fstream file;

	file.open(szFileName, std::ios::out);

	if(!file)
		return false;

	file << "# World built with bzedit32 world class, available at\n";
	file << "# http://www.artemisgames.com/bzedit\n\n";

	trObjectMap::iterator	itr = m_vObjList.begin();
	while(itr !=m_vObjList.end())
	{
		itr->second->Write(file);
		itr++;
	}

	file.close();
  return true;
}

void CWorld::Sort()
{
/*	trObjectList NewList;

	trObjectList::iterator	itr;

	for(int eType = eBOX; eType < eNONE; eType++)
	{
		itr = m_vObjList.begin();

		while (itr != m_vObjList.end())
		{
			if ((*itr)->GetType() == eType)
				NewList.push_back(*itr);
			
			itr++;
		}
	}
	m_vObjList.clear();
	m_vObjList = NewList; */
}

CBaseObject* CWorld::GetObject( char* name )
{
	trObjectMap::iterator itr = m_vObjList.begin();
	while(itr != m_vObjList.end())
	{
		if(stricmp((itr->second)->GetItemName(),name)==0)
			return itr->second;
		itr++;
	}
	return NULL;
}

CBaseObject* CWorld::GetObject( int item )
{
	trObjectMap::iterator	itr = m_vObjList.find(item);

	if  (itr == m_vObjList.end())
		return NULL;

	return itr->second;
}

CBaseObject* CWorld::GetNthObjectOfType ( const char *szType, int n )
{
	if (!szType)
		return NULL;
	
	int iCount = 0;

	char	szUperName[512];
	strcpy(szUperName,szType);
	strupr(szUperName);

	std::string	str = szUperName;

	tmFacotryNameMap::iterator	itr = m_vFactoryNameMap.find(str);

	if (itr == m_vFactoryNameMap.end())
		return NULL;

	int iType = itr->second->GetTypeID();
	
	trObjectMap::iterator ObjItr = m_vObjList.begin();

	while(ObjItr != m_vObjList.end())
	{
		if(iType == ObjItr->second->GetTypeID())
		{
			if (iCount == n )
				return ObjItr->second;

			iCount++;
		}
		ObjItr++;
	}
	return NULL;
}

int CWorld::GetObjectListOfType ( const char *szType, trObjectList	&rList )
{
	if (!szType)
		return NULL;
	
	int iCount = 0;
	
	char	szUperName[512];
	strcpy(szUperName,szType);
	strupr(szUperName);

	std::string	str = szUperName;

	tmFacotryNameMap::iterator	itr = m_vFactoryNameMap.find(str);

	if (itr == m_vFactoryNameMap.end())
		return NULL;

	int iType = itr->second->GetTypeID();
	
	trObjectMap::iterator ObjItr = m_vObjList.begin();

	rList.clear();
	while(ObjItr != m_vObjList.end())
	{
		if(iType == ObjItr->second->GetTypeID())
		{
			rList.push_back(ObjItr->second);
			iCount++;
		}
		ObjItr++;
	}
	return rList.size();
}

int	CWorld::AddObject( const char * szType )
{
	if (!szType)
		return -1;
	
	char	szUperName[512];
	strcpy(szUperName,szType);
	strupr(szUperName);

	std::string str = szUperName;

	tmFacotryNameMap::iterator itr = m_vFactoryNameMap.find(str);

	if (itr == m_vFactoryNameMap.end())
		return -1;

	CBaseObject	*Item = itr->second->New();

	if (!Item)
		return -1;

	Item->SetTypeID(itr->second->GetTypeID());

	Item->SetTextureMan(m_pTextureMan);
	Item->SetModelMan(m_pModelMan);
	Item->SetWorld(this);

	Item->Init();

	Item->SetID(m_iNextGUID);

	m_vObjList[m_iNextGUID] = Item;

	m_iNextGUID ++;

	return Item->GetID();
}

int	CWorld::AddObject( int iTypeID )
{
	return AddObject(GetFactoryName(iTypeID));
}

int	CWorld::AddObject( CBaseObject *pToCopy )
{
	if (!pToCopy)
		return -1;

	return AddObject(pToCopy->GetTypeID());
}

int	CWorld::DupeObject( int iID )
{
	return DupeObject(GetObject(iID));
}

int	CWorld::DupeObject( CBaseObject *pToCopy )
{
	if (!pToCopy)
		return -1;

	int iCopy = AddObject(pToCopy->GetTypeName());
	CBaseObject *pCopy = GetObject(iCopy);
		
	if (!pCopy)
		return -1;

	pCopy->CopyFrom(pToCopy);

	std::string	temp = pToCopy->GetItemName();
	
	char	*pCopyChar = strstr(temp.c_str(),"copy");

	if (pCopyChar)
	{
		pCopyChar += 5;
		int iNum = 0;

		iNum = atoi(pCopyChar);
		iNum++;

		*pCopyChar = NULL;

		char	temp2[512];
		sprintf(temp2,"%d",iNum);
		temp += temp2;
	}
	else
	{
		temp += " copy";
	}

	pCopy->SetItemName((const char*)temp.c_str());
	return iCopy;
}

bool CWorld::RemoveObject( int item )
{
	trObjectMap::iterator itr = m_vObjList.find(item);

	if (itr == m_vObjList.end())
		return false;

	m_vObjList.erase(itr);
	return true;
	
}

void CWorld::RegisterFactory ( const char *szName, CBaseObjectFactory	*pFactory )
{
	if (!szName || !pFactory)
		return;

	pFactory->SetTypeID(m_vFactoryList.size());
	pFactory->SetName(szName);
	m_vFactoryList.push_back(pFactory);

	char	szUperName[512];
	strcpy(szUperName,szName);
	strupr(szUperName);

	std::string	str = szUperName;

	m_vFactoryNameMap[str] = pFactory;
}

int CWorld::GetFactoryCount ( void )
{
	return m_vFactoryList.size();
}

const char* CWorld::GetFactoryName ( int iFactory )
{
	if ( (iFactory < 0) || (iFactory >= GetFactoryCount()) )
		return NULL;

	CBaseObjectFactory	*pFactory = m_vFactoryList[iFactory];

	if (!pFactory)
		return NULL;

	return pFactory->GetName();
}

CBaseObjectFactory*	CWorld::GetFactory ( int iFactory )
{
	if ( (iFactory < 0) || (iFactory >= GetFactoryCount()) )
		return NULL;

	return m_vFactoryList[iFactory];
}

CBaseObjectFactory*	CWorld::GetFactory ( const char *szName )
{
	if (!szName)
		return NULL;

	char	szUperName[512];
	strcpy(szUperName,szName);
	strupr(szUperName);

	std::string str = szUperName;

	tmFacotryNameMap::iterator itr = m_vFactoryNameMap.find(str);

	if (itr == m_vFactoryNameMap.end())
		return NULL;

	return itr->second;
}



