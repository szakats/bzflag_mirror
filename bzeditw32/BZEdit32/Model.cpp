// model.cpp

#ifdef WIN32
	#pragma warning( disable : 4786 )  // Disable warning message for stl maps
	#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/GLU.h>
#include <MAP>
#include "model.h"

#include "world.h"
#include "Utils.h"

#define _SELECT_LINE_WIDTH	2.5f

void DrawSelectGrid ( float fOffset, float fLen, trVertex3D &rScale )
{
	// X+ Y+ Z+ Corner;
	glVertex3f(rScale.x+fOffset, rScale.y+fOffset, rScale.z+fOffset);
	glVertex3f(rScale.x+fOffset-fLen, rScale.y+fOffset, rScale.z+fOffset);

	glVertex3f(rScale.x+fOffset, rScale.y+fOffset, rScale.z+fOffset);
	glVertex3f(rScale.x+fOffset, rScale.y+fOffset-fLen, rScale.z+fOffset);

	glVertex3f(rScale.x+fOffset, rScale.y+fOffset, rScale.z+fOffset);
	glVertex3f(rScale.x+fOffset, rScale.y+fOffset, rScale.z+fOffset-fLen);

	// X- Y+ Z+ Corner;
	glVertex3f(-rScale.x-fOffset, rScale.y+fOffset, rScale.z+fOffset);
	glVertex3f(-rScale.x-fOffset+fLen, rScale.y+fOffset, rScale.z+fOffset);

	glVertex3f(-rScale.x-fOffset, rScale.y+fOffset, rScale.z+fOffset);
	glVertex3f(-rScale.x-fOffset, rScale.y+fOffset-fLen, rScale.z+fOffset);

	glVertex3f(-rScale.x-fOffset, rScale.y+fOffset, rScale.z+fOffset);
	glVertex3f(-rScale.x-fOffset, rScale.y+fOffset, rScale.z+fOffset-fLen);

	// X- Y- Z+ Corner;
	glVertex3f(-rScale.x-fOffset, -rScale.y-fOffset, rScale.z+fOffset);
	glVertex3f(-rScale.x-fOffset+fLen, -rScale.y-fOffset, rScale.z+fOffset);

	glVertex3f(-rScale.x-fOffset, -rScale.y-fOffset, rScale.z+fOffset);
	glVertex3f(-rScale.x-fOffset, -rScale.y-fOffset+fLen, rScale.z+fOffset);

	glVertex3f(-rScale.x-fOffset, -rScale.y-fOffset, rScale.z+fOffset);
	glVertex3f(-rScale.x-fOffset, -rScale.y-fOffset, rScale.z+fOffset-fLen);

	// X+ Y- Z+ Corner;
	glVertex3f(rScale.x+fOffset, -rScale.y-fOffset, rScale.z+fOffset);
	glVertex3f(rScale.x+fOffset-fLen, -rScale.y-fOffset, rScale.z+fOffset);

	glVertex3f(rScale.x+fOffset, -rScale.y-fOffset, rScale.z+fOffset);
	glVertex3f(rScale.x+fOffset, -rScale.y-fOffset+fLen, rScale.z+fOffset);

	glVertex3f(rScale.x+fOffset, -rScale.y-fOffset, rScale.z+fOffset);
	glVertex3f(rScale.x+fOffset, -rScale.y-fOffset, rScale.z+fOffset-fLen);

	// X+ Y+ Z- Corner;
	glVertex3f(rScale.x+fOffset, rScale.y+fOffset, -fOffset);
	glVertex3f(rScale.x+fOffset-fLen, rScale.y+fOffset, -fOffset);

	glVertex3f(rScale.x+fOffset, rScale.y+fOffset, -fOffset);
	glVertex3f(rScale.x+fOffset, rScale.y+fOffset-fLen, -fOffset);

	glVertex3f(rScale.x+fOffset, rScale.y+fOffset, -fOffset);
	glVertex3f(rScale.x+fOffset, rScale.y+fOffset, -fOffset+fLen);

	// X- Y+ Z+ Corner;
	glVertex3f(-rScale.x-fOffset, rScale.y+fOffset, -fOffset);
	glVertex3f(-rScale.x-fOffset+fLen, rScale.y+fOffset, -fOffset);

	glVertex3f(-rScale.x-fOffset, rScale.y+fOffset, -fOffset);
	glVertex3f(-rScale.x-fOffset, rScale.y+fOffset-fLen, -fOffset);

	glVertex3f(-rScale.x-fOffset, rScale.y+fOffset, -fOffset);
	glVertex3f(-rScale.x-fOffset, rScale.y+fOffset, -fOffset+fLen);

	// X- Y- Z+ Corner;
	glVertex3f(-rScale.x-fOffset, -rScale.y-fOffset, -fOffset);
	glVertex3f(-rScale.x-fOffset+fLen, -rScale.y-fOffset, -fOffset);

	glVertex3f(-rScale.x-fOffset, -rScale.y-fOffset, -fOffset);
	glVertex3f(-rScale.x-fOffset, -rScale.y-fOffset+fLen, -fOffset);

	glVertex3f(-rScale.x-fOffset, -rScale.y-fOffset, -fOffset);
	glVertex3f(-rScale.x-fOffset, -rScale.y-fOffset, -fOffset+fLen);

	// X+ Y- Z+ Corner;
	glVertex3f(rScale.x+fOffset, -rScale.y-fOffset, -fOffset);
	glVertex3f(rScale.x+fOffset-fLen, -rScale.y-fOffset, -fOffset);

	glVertex3f(rScale.x+fOffset, -rScale.y-fOffset, -fOffset);
	glVertex3f(rScale.x+fOffset, -rScale.y-fOffset+fLen, -fOffset);

	glVertex3f(rScale.x+fOffset, -rScale.y-fOffset, -fOffset);
	glVertex3f(rScale.x+fOffset, -rScale.y-fOffset, -fOffset+fLen);
}


CModelManager::CModelManager()
{
	aDrawList.clear();
	aModelList.clear();

	pTextureManager = NULL;
}

CModelManager::~CModelManager()
{
	Clear();
	
	aDrawList.clear();
}

void CModelManager::Clear ( void )
{
	trModelListItr	itr = aModelList.begin();

	while (itr != aModelList.end())
	{
		if (*itr)
			delete(*itr);
		itr++;
	}
	aModelList.clear();
}

CModelManager::CModelManager(CTextureManager *pTexMan)
{
	aDrawList.clear();
	aModelList.clear();
	SetTextureMan(pTexMan);
}

void CModelManager::Init ( void )
{
}

int	CModelManager::NewModel ( char *szType )
{
	CBaseModel	*pModel = NULL;

	// here are all the models supported

	if (stricmp(szType,"Ground")==0)
		pModel = (CBaseModel*)new CGroundPlaneModel;
	else if (stricmp(szType,"Walls")==0)
		pModel = (CBaseModel*)new CWallsModel;
	else if (stricmp(szType,"BoxSides")==0)
		pModel = (CBaseModel*)new CBoxSidesModel;
	else if (stricmp(szType,"BoxTops")==0)
		pModel = (CBaseModel*)new CBoxTopsModel;
	else if (stricmp(szType,"Pyramid")==0)
		pModel = (CBaseModel*)new CPyramidModel;
	else if (stricmp(szType,"TPortField")==0)
		pModel = (CBaseModel*)new CTPortFieldModel;
	else if (stricmp(szType,"TPortFrame")==0)
		pModel = (CBaseModel*)new CTPortFrameModel;
	else if (stricmp(szType,"BaseTops")==0)
		pModel = (CBaseModel*)new CBaseTopsModel;
	else if (stricmp(szType,"Link")==0)
		pModel = (CBaseModel*)new CLinkModel;

	if (!pModel)
		return -1;

	int id = aModelList.size();
	aModelList.push_back(pModel);
	return id;
}

void CModelManager::KillModel ( int iItem )
{
	ClearDrawList();
}

CBaseModel	*CModelManager::GetModel ( int iItem )
{
	if ( (iItem < 0) && (iItem >= aModelList.size()) )
		return NULL;

	return aModelList[iItem];
}

void CModelManager::AddDrawItem ( int iItem, bool bTransparent, bool bSelected, int iName, teDrawPriority ePriority )
{
	if ( (iItem < 0) && (iItem >= aModelList.size()) )
		return;

	trDrawItem	rDrawItem;

	rDrawItem.bSel = bSelected;
	rDrawItem.bTrans = bTransparent;
	rDrawItem.iID = iItem;
	rDrawItem.iName = iName;
	rDrawItem.ePriority = ePriority;

	aDrawList.push_back(rDrawItem);
}

void CModelManager::ClearDrawList ( void )
{
	aDrawList.clear();
}

void CModelManager::DrawList ( void )
{
	trDrawListItr modelItr = aDrawList.begin();
	modelItr = aDrawList.begin();

	trDrawItem	item;
	CBaseModel	*pModel;

	std::map <int,trDrawList> TextureList;

	while ( modelItr != aDrawList.end())
	{
		item = (*modelItr);
		pModel = GetModel(item.iID);

		TextureList[pModel->GetTexuture()].push_back(item);
		modelItr++;
	}

	glEnable(GL_TEXTURE_2D);

	std::map <int,trDrawList>::iterator	TextureItr = TextureList.begin();

	for (int i = 0; i <=eLast; i++)
	{
		TextureItr = TextureList.begin();

		if (i == eAlways)	// for funcky transparent stuff
		{
		//	glDepthMask(false);
			glDepthFunc(GL_ALWAYS);
		//	glPolygonOffset (-4,-4.0f);
		//	glEnable(GL_POLYGON_OFFSET_FILL);
		}

		if (i == eTransNoZ)
			glDepthMask(false);

		if (i == eLast)
			glDepthFunc(GL_LEQUAL);

		while (TextureItr != TextureList.end())
		{	
			modelItr = (*TextureItr).second.begin();
			pTextureManager->Bind((*TextureItr).first);

			while ( modelItr != (*TextureItr).second.end())
			{
				item = (*modelItr);
				pModel = GetModel(item.iID);

				if (pModel && (i == item.ePriority) )
				{
					if (item.iName != -1)
						glLoadName(item.iName+1);
						
					pModel->Render(item.bTrans,item.bSel);
				}
				modelItr++;
			}
			TextureItr++;
		}

		if ( (i == eAlways) || (i == eTransNoZ) || (i == eLast))
		{
			glDepthMask(true);
			glDisable(GL_POLYGON_OFFSET_FILL);
			glDepthFunc(GL_LESS);
		}

	}
}

CBaseModel::CBaseModel()
{
	iGLListID = -1;
	iSelList = -1;
	iTexture = -1;
}

CBaseModel::~CBaseModel()
{

}

void CBaseModel::Init( trVertex3D rRot, trVertex3D rScale, float fRot  )
{
	iGLListID = -1;
	iSelList = -1;
	iTexture = -1;
}

void CBaseModel::Render( bool bTrans, bool bSelected )
{
	if (iGLListID ==-1)
		return;
		
	float	fAlphaValue = bTrans?0.35f:1.0f;

	if (bSelected)
		glColor4f(0.5f,1.0f,1.0f,fAlphaValue);
	else
		glColor4f(1,1,1,fAlphaValue);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glCallList(iGLListID);

	// draw the bb thing
	if ( bSelected && (iSelList != -1) )
	{
		glColor4f( 0,1,0,1);
		glLineWidth(_SELECT_LINE_WIDTH);
		glCallList(iSelList);
		glLineWidth(1.0f);
	}

	glColor4f(1,1,1,1);
}

void CBaseModel::SetItemInfo ( char* szItem, float *fData )
{

}

void CGroundPlaneModel::Init ( trVertex3D rPos, trVertex3D rScale, float fRot )
{
	if (iGLListID != -1)
		glDeleteLists(iGLListID,1);

	iGLListID = glGenLists(1);
	
	glNewList(iGLListID,GL_COMPILE);

		float		TextureScale = 50.0f;

		float		WallSize = 400.0f;

	//	glDisable(GL_CULL_FACE);

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);

			glTexCoord2f(0.0,TextureScale);
			glVertex3f(WallSize, WallSize, 0);

			glTexCoord2f(TextureScale, TextureScale);
			glVertex3f(-WallSize, WallSize, 0);

			glTexCoord2f(TextureScale, 0.0);
			glVertex3f(-WallSize, -WallSize, 0);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(WallSize, -WallSize, 0);
			
		glEnd();

	//	glEnable(GL_CULL_FACE);

	glEndList();
}
	

void CWallsModel::Init ( trVertex3D rPos, trVertex3D rScale, float fRot )
{
	if (iGLListID != -1)
		glDeleteLists(iGLListID,1);

	iGLListID = glGenLists(1);
	
	glNewList(iGLListID,GL_COMPILE);

		glDisable(GL_CULL_FACE);

		float		WallSize = 400.0f;
		float		fWallZ = 20.0f;

		glBegin(GL_TRIANGLE_STRIP);
			glNormal3f(0,-1,0);

			glTexCoord2f(0,0);
			glVertex3f(-WallSize,WallSize,0);

			glTexCoord2f(0,1);
			glVertex3f(-WallSize,WallSize,fWallZ);

			glTexCoord2f(((WallSize*2)/fWallZ),0);
			glVertex3f(WallSize,WallSize,0);
			
			glTexCoord2f(((WallSize*2)/fWallZ),1);
			glVertex3f(WallSize,WallSize,fWallZ);
		
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
			// x+ side

			glNormal3f(-1,0,0);

			glTexCoord2f(((WallSize*2)/fWallZ),0);
			glVertex3f(WallSize,WallSize,0);
			
			glTexCoord2f(((WallSize*2)/fWallZ),1);
			glVertex3f(WallSize,WallSize,fWallZ);

			glTexCoord2f(((WallSize*2)/fWallZ)*2,0);
			glVertex3f(WallSize,-WallSize,0);

			glTexCoord2f(((WallSize*2)/fWallZ)*2,1);
			glVertex3f(WallSize,-WallSize,fWallZ);

		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
			// y side

			glNormal3f(0,1,0);

			glTexCoord2f(((WallSize*2)/fWallZ)*2,0);
			glVertex3f(WallSize,-WallSize,0);

			glTexCoord2f(((WallSize*2)/fWallZ)*2,1);
			glVertex3f(WallSize,-WallSize,fWallZ);
			
			glTexCoord2f(((WallSize*2)/fWallZ)*3,0);
			glVertex3f(-WallSize,-WallSize,0);

			glTexCoord2f(((WallSize*2)/fWallZ)*3,1);
			glVertex3f(-WallSize,-WallSize,fWallZ);

		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
			// x- side

			glNormal3f(1,1,0);
			
			glTexCoord2f(((WallSize*2)/fWallZ)*3,0);
			glVertex3f(-WallSize,-WallSize,0);

			glTexCoord2f(((WallSize*2)/fWallZ)*3,1);
			glVertex3f(-WallSize,-WallSize,fWallZ);
			
			glTexCoord2f(((WallSize*2)/fWallZ)*4,0);
			glVertex3f(-WallSize,WallSize,0);

			glTexCoord2f(((WallSize*2)/fWallZ)*4,1);
			glVertex3f(-WallSize,WallSize,fWallZ);

		glEnd();

		glEnable(GL_CULL_FACE);

	glEndList();
}


void CBoxSidesModel::Init ( trVertex3D rPos, trVertex3D rScale, float fRot)
{
	if (iGLListID != -1)
		glDeleteLists(iGLListID,1);

	iGLListID = glGenLists(1);

	float	fTexureUnits = 4.0f;
	
	glNewList(iGLListID,GL_COMPILE);

		glPushMatrix();
		glTranslatef(rPos.x, rPos.y, rPos.z);
		glRotatef(fRot, 0, 0, 1);
		
		glBegin(GL_QUADS);	// lets hope they make them quads
		
			// Y+ side
			glNormal3f(0,1,0);
			glTexCoord2f(0,0);
			glVertex3f(-rScale.x,rScale.y,0);

			glTexCoord2f(0,rScale.z/fTexureUnits);
			glVertex3f(-rScale.x,rScale.y,rScale.z);

			glTexCoord2f(rScale.x/fTexureUnits*2,rScale.z/fTexureUnits);
			glVertex3f(rScale.x,rScale.y,rScale.z);

			glTexCoord2f(rScale.x/fTexureUnits*2,0);
			glVertex3f(rScale.x,rScale.y,0);

			// x+ side
			glNormal3f(1,0,0);
			glTexCoord2f(0,0);
			glVertex3f(rScale.x,rScale.y,0);

			glTexCoord2f(0,rScale.z/fTexureUnits);
			glVertex3f(rScale.x,rScale.y,rScale.z);

			glTexCoord2f(rScale.y/fTexureUnits*2,rScale.z/fTexureUnits);
			glVertex3f(rScale.x,-rScale.y,rScale.z);

			glTexCoord2f(rScale.y/fTexureUnits*2,0);
			glVertex3f(rScale.x,-rScale.y,0);

			// y- side
			glNormal3f(0,-1,0);
			glTexCoord2f(0,0);
			glVertex3f(rScale.x,-rScale.y,0);

			glTexCoord2f(0,rScale.z/fTexureUnits);
			glVertex3f(rScale.x,-rScale.y,rScale.z);

			glTexCoord2f(rScale.x/fTexureUnits*2,rScale.z/fTexureUnits);
			glVertex3f(-rScale.x,-rScale.y,rScale.z);

			glTexCoord2f(rScale.x/fTexureUnits*2,0);
			glVertex3f(-rScale.x,-rScale.y,0);

			// x- side
			glNormal3f(0,-1,0);
			glTexCoord2f(0,0);
			glVertex3f(-rScale.x,-rScale.y,0);

			glTexCoord2f(0,rScale.z/fTexureUnits);
			glVertex3f(-rScale.x,-rScale.y,rScale.z);

			glTexCoord2f(rScale.y/fTexureUnits*2,rScale.z/fTexureUnits);
			glVertex3f(-rScale.x,rScale.y,rScale.z);

			glTexCoord2f(rScale.y/fTexureUnits*2,0);
			glVertex3f(-rScale.x,rScale.y,0);

		glEnd();

		glPopMatrix();

	glEndList();

	if (iSelList != -1)
		glDeleteLists(iSelList,1);

	iSelList = glGenLists(1);
	
	glNewList(iSelList,GL_COMPILE);

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();
		glTranslatef(rPos.x, rPos.y, rPos.z);
		glRotatef(fRot, 0, 0, 1);

		float	fOffset = 2.5f;
		float	fLen = 5.0f;

		glBegin(GL_LINES);
		
			DrawSelectGrid(fOffset,fLen,rScale);

		glEnd();

		glPopMatrix();
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

	glEndList();
}

void CBoxTopsModel::Init ( trVertex3D rPos, trVertex3D rScale, float fRot)
{
	if (iGLListID != -1)
		glDeleteLists(iGLListID,1);

	iGLListID = glGenLists(1);

	float	fTexureUnits = 4.0f;
	
	glNewList(iGLListID,GL_COMPILE);

		glPushMatrix();
		glTranslatef(rPos.x, rPos.y, rPos.z);
		glRotatef(fRot, 0, 0, 1);
		
		glBegin(GL_QUADS);	// lets hope they make them quads
		
			// z+ side
			glNormal3f(0,0,1);
			glTexCoord2f(rScale.x/fTexureUnits*2,0);
			glVertex3f(rScale.x,-rScale.y,rScale.z);

			glTexCoord2f(rScale.x/fTexureUnits*2,rScale.y/fTexureUnits*2);
			glVertex3f(rScale.x,rScale.y,rScale.z);

			glTexCoord2f(0,rScale.y/fTexureUnits*2);
			glVertex3f(-rScale.x,rScale.y,rScale.z);

			glTexCoord2f(0,0);
			glVertex3f(-rScale.x,-rScale.y,rScale.z);

			// z+ side
			glNormal3f(0,0,-1);
			glTexCoord2f(0,0);
			glVertex3f(-rScale.x,-rScale.y,0);

			glTexCoord2f(0,rScale.y/fTexureUnits*2);
			glVertex3f(-rScale.x,rScale.y,0);

			glTexCoord2f(rScale.x/fTexureUnits*2,rScale.y/fTexureUnits*2);
			glVertex3f(rScale.x,rScale.y,0);

			glTexCoord2f(rScale.x/fTexureUnits*2,0);
			glVertex3f(rScale.x,-rScale.y,0);

		glEnd();

		glPopMatrix();

	glEndList();

	// tops don't have select models
}

void CPyramidModel::Init ( trVertex3D rPos, trVertex3D rScale, float fRot)
{
	if (iGLListID != -1)
		glDeleteLists(iGLListID,1);

	iGLListID = glGenLists(1);

	float	fTexureUnits = 4.0f;
	
	glNewList(iGLListID,GL_COMPILE);

		glPushMatrix();
		glTranslatef(rPos.x, rPos.y, rPos.z);
		glRotatef(fRot, 0, 0, 1);
		
		glBegin(GL_QUADS);	// lets hope they make them quads into strips
			// z+ side
			glNormal3f(0,0,-1);
			glTexCoord2f(0,0);
			glVertex3f(-rScale.x,-rScale.y,0);

			glTexCoord2f(0,rScale.y/fTexureUnits*2);
			glVertex3f(-rScale.x,rScale.y,0);

			glTexCoord2f(rScale.x/fTexureUnits*2,rScale.y/fTexureUnits*2);
			glVertex3f(rScale.x,rScale.y,0);

			glTexCoord2f(rScale.x/fTexureUnits*2,0);
			glVertex3f(rScale.x,-rScale.y,0);

		glEnd();

		trVertex3D	rVec1,rVec2,rNorm;

		glBegin(GL_TRIANGLES);
			// y+ Side
			// compute a normal
			rVec1.x = rScale.x; rVec1.y = -rScale.y; rVec1.z = rScale.z;
			rVec2.x = rScale.x*2; rVec2.y = 0; rVec2.z = 0;
			cross(rVec1,rVec2,rNorm);
			set_mag(rNorm,1.0f);

			glNormal3f(rNorm.x,rNorm.y,rNorm.z);
			glTexCoord2f(0,0);
			glVertex3f(-rScale.x,rScale.y,0);

			glTexCoord2f(rScale.x/fTexureUnits,rScale.z/fTexureUnits);
			glVertex3f(0,0,rScale.z);

			glTexCoord2f(rScale.x/fTexureUnits*2,0);
			glVertex3f(rScale.x,rScale.y,0);

			// x+ Side
			// compute a normal
			rVec1.x = rScale.x; rVec1.y = -rScale.y; rVec1.z = rScale.z;
			rVec2.x = 0; rVec2.y = -rScale.y*2; rVec2.z = 0;
			
			cross(rVec2,rVec1,rNorm);
			set_mag(rNorm,1.0f);

			glNormal3f(rNorm.x,rNorm.y,rNorm.z);
			glTexCoord2f(0,0);
			glVertex3f(rScale.x,rScale.y,0);

			glTexCoord2f(rScale.x/fTexureUnits,rScale.z/fTexureUnits);
			glVertex3f(0,0,rScale.z);

			glTexCoord2f(rScale.x/fTexureUnits*2,0);
			glVertex3f(rScale.x,-rScale.y,0);

			// y- Side
			// compute a normal
			rVec1.x = -rScale.x; rVec1.y = rScale.y; rVec1.z = rScale.z;
			rVec2.x = -rScale.x*2; rVec2.y = 0; rVec2.z = 0;
			cross(rVec1,rVec2,rNorm);
			set_mag(rNorm,1.0f);

			glNormal3f(rNorm.x,rNorm.y,rNorm.z);
			glTexCoord2f(0,0);
			glVertex3f(rScale.x,-rScale.y,0);

			glTexCoord2f(rScale.x/fTexureUnits,rScale.z/fTexureUnits);
			glVertex3f(0,0,rScale.z);

			glTexCoord2f(rScale.x/fTexureUnits*2,0);
			glVertex3f(-rScale.x,-rScale.y,0);
				
			// x- Side
			// compute a normal
			rVec1.x = -rScale.x; rVec1.y = rScale.y; rVec1.z = rScale.z;
			rVec2.x = 0; rVec2.y = rScale.y*2; rVec2.z = 0;
			cross(rVec2,rVec1,rNorm);
			set_mag(rNorm,1.0f);

			glNormal3f(rNorm.x,rNorm.y,rNorm.z);
			glTexCoord2f(0,0);
			glVertex3f(-rScale.x,-rScale.y,0);

			glTexCoord2f(rScale.x/fTexureUnits,rScale.z/fTexureUnits);
			glVertex3f(0,0,rScale.z);

			glTexCoord2f(rScale.x/fTexureUnits*2,0);
			glVertex3f(-rScale.x,rScale.y,0);

		glEnd();

		glPopMatrix();

	glEndList();

	if (iSelList != -1)
		glDeleteLists(iSelList,1);

	iSelList = glGenLists(1);
	
	glNewList(iSelList,GL_COMPILE);

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();
		glTranslatef(rPos.x, rPos.y, rPos.z);
		glRotatef(fRot, 0, 0, 1);

		float	fOffset = 2.5f;
		float	fLen = 5.0f;

		glBegin(GL_LINES);
		
			DrawSelectGrid(fOffset,fLen,rScale);

		glEnd();

		glPopMatrix();
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

	glEndList();
}

void CTPortFieldModel::Render ( bool bTrans, bool bSelected )
{
	if (iGLListID ==-1)
		return;
		
	float	fAlphaValue = bTrans?0.5f:1.0f;

	if (bSelected)
		glColor4f(1,0,0,fAlphaValue);
	else
		glColor4f(1,1,1,fAlphaValue);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glCallList(iGLListID);

	glColor4f(1,1,1,1);
}

void CTPortFieldModel::SetItemInfo ( char* szItem, float *fData )
{
	if (stricmp(szItem,"Border")==0)
		fBorder = *fData;
}

void CTPortFieldModel::Init ( trVertex3D rPos, trVertex3D rScale, float fRot )
{
	if (iGLListID != -1)
		glDeleteLists(iGLListID,1);

	iGLListID = glGenLists(1);
	
	glNewList(iGLListID,GL_COMPILE);

		glPushMatrix();
		glTranslatef(rPos.x, rPos.y, rPos.z);
		glRotatef(fRot, 0, 0, 1);
		
		float fStupidY = rScale.y +  + fBorder;

		float fStupidX = rScale.x/4.0f;

		glBegin(GL_QUADS);	// lets hope they make them quads
			//	x pos side
			glNormal3f(1,0,0);

			glTexCoord2f(0.5f,0);
			glVertex3f(fStupidX,fStupidY,0);

			glTexCoord2f(0.5f,1);
			glVertex3f(fStupidX,fStupidY,rScale.z);

			glTexCoord2f(0,1);
			glVertex3f(fStupidX,-fStupidY,rScale.z);

			glTexCoord2f(0,0);
			glVertex3f(fStupidX,-fStupidY,0);

			// x neg side
			glNormal3f(-1,0,0);

			glTexCoord2f(1,0);
			glVertex3f(-fStupidX,-fStupidY,0);

			glTexCoord2f(1,1);
			glVertex3f(-fStupidX,-fStupidY,rScale.z);

			glTexCoord2f(0.5f,1);
			glVertex3f(-fStupidX,fStupidY,rScale.z);
			
			glTexCoord2f(0.5f,0);
			glVertex3f(-fStupidX,fStupidY,0);

		glEnd();


		glPopMatrix();

	glEndList();

	// you can't select fields
}

void CTPortFrameModel::SetItemInfo ( char* szItem, float *fData )
{
	if (stricmp(szItem,"Border")==0)
		fBorder = *fData;
}

void CTPortFrameModel::Init ( trVertex3D rPos, trVertex3D rScale, float fRot )
{
	if (iGLListID != -1)
		glDeleteLists(iGLListID,1);

	iGLListID = glGenLists(1);
	
	glNewList(iGLListID,GL_COMPILE);

		glPushMatrix();
		glTranslatef(rPos.x, rPos.y, rPos.z);
		glRotatef(fRot, 0, 0, 1);

		float	fTextureSize = 1.0f;

		float	fShift = 0.0f;

		float	fStupidY = rScale.y + fBorder;
		
		glBegin(GL_TRIANGLE_STRIP);	// lets hope they make them quads
			//	X neg side
			glNormal3f(-1,0,0);

			glTexCoord2f(0,0);
			glVertex3f(-fBorder/2,fStupidY+fBorder,0);

			glTexCoord2f(fBorder/fTextureSize,0);
			glVertex3f(-fBorder/2,fStupidY,0);

			glTexCoord2f(0,(rScale.z+fBorder)/fTextureSize);
			glVertex3f(-fBorder/2,fStupidY+fBorder,rScale.z+fBorder);

			glTexCoord2f(fBorder/fTextureSize,rScale.z/fTextureSize);
			glVertex3f(-fBorder/2,fStupidY,rScale.z);

			glTexCoord2f(((2*fBorder)+(2*fStupidY))/fTextureSize,(rScale.z+fBorder)/fTextureSize);
			glVertex3f(-fBorder/2,-fStupidY-fBorder,rScale.z+fBorder);

			glTexCoord2f(((fBorder)+(2*fStupidY))/fTextureSize,(rScale.z)/fTextureSize);
			glVertex3f(-fBorder/2,-fStupidY,rScale.z);

			glTexCoord2f(((2*fBorder)+(2*fStupidY))/fTextureSize,0);
			glVertex3f(-fBorder/2,-fStupidY-fBorder,0);

			glTexCoord2f(((fBorder)+(2*fStupidY))/fTextureSize,0);
			glVertex3f(-fBorder/2,-fStupidY,0);

		glEnd();

		glFrontFace(GL_CW);

		float fShift2 = fShift;

		glBegin(GL_TRIANGLE_STRIP);	// lets hope they make them quads
			//	X Pos side
			glNormal3f(1,0,0);

			glTexCoord2f(0,fShift2);
			glVertex3f(fBorder/2,fStupidY+fBorder,0);

			glTexCoord2f(fBorder/fTextureSize,fShift2);
			glVertex3f(fBorder/2,fStupidY,0);

			glTexCoord2f(0,(rScale.z+fBorder)/fTextureSize+fShift2);
			glVertex3f(fBorder/2,fStupidY+fBorder,rScale.z+fBorder);

			glTexCoord2f(fBorder/fTextureSize,rScale.z/fTextureSize+fShift2);
			glVertex3f(fBorder/2,fStupidY,rScale.z);

			glTexCoord2f(((2*fBorder)+(2*fStupidY))/fTextureSize,(rScale.z+fBorder)/fTextureSize+fShift2);
			glVertex3f(fBorder/2,-fStupidY-fBorder,rScale.z+fBorder);

			glTexCoord2f(((fBorder)+(2*fStupidY))/fTextureSize,(rScale.z)/fTextureSize+fShift2);
			glVertex3f(fBorder/2,-fStupidY,rScale.z);

			glTexCoord2f(((2*fBorder)+(2*fStupidY))/fTextureSize,fShift2);
			glVertex3f(fBorder/2,-fStupidY-fBorder,0);

			glTexCoord2f(((fBorder)+(2*fStupidY))/fTextureSize,fShift2);
			glVertex3f(fBorder/2,-fStupidY,0);

		glEnd();

		glFrontFace(GL_CCW);

		
		glBegin(GL_QUADS);

			float	fTopShift = 0.5f;
			//top
			glNormal3f(fTopShift,0,1);

			glTexCoord2f(fTopShift,0.0001f);
			glVertex3f(fBorder/2,-fStupidY-fBorder,rScale.z+fBorder);

			glTexCoord2f(((2*fBorder)+(2*fStupidY))/fTextureSize+fTopShift,0.0001f); // just a sliver
			glVertex3f(fBorder/2,fStupidY+fBorder,rScale.z+fBorder);

			glTexCoord2f(((2*fBorder)+(2*fStupidY))/fTextureSize+fTopShift,0);
			glVertex3f(-fBorder/2,fStupidY+fBorder,rScale.z+fBorder);
		
			glTexCoord2f(fTopShift,0);
			glVertex3f(-fBorder/2,-fStupidY-fBorder,rScale.z+fBorder);

			// underside of top
			glNormal3f(0,0,-1);

			glTexCoord2f(fTopShift,0);
			glVertex3f(-fBorder/2,-fStupidY-fBorder,rScale.z);

			glTexCoord2f(((2*fBorder)+(2*fStupidY))/fTextureSize+fTopShift,0);
			glVertex3f(-fBorder/2,fStupidY+fBorder,rScale.z);

			glTexCoord2f(((2*fBorder)+(2*fStupidY))/fTextureSize+fTopShift,0.0001f); // just a sliver
			glVertex3f(fBorder/2,fStupidY+fBorder,rScale.z);

			glTexCoord2f(fTopShift,0.0001f);
			glVertex3f(fBorder/2,-fStupidY-fBorder,rScale.z);

			// y plus outside
			glNormal3f(0,+1,0);

			glTexCoord2f(fBorder/fTextureSize,fShift);
			glVertex3f(-fBorder/2,fStupidY+fBorder,0);

			glTexCoord2f(fBorder/fTextureSize,(rScale.z+fBorder)/fTextureSize+fShift);
			glVertex3f(-fBorder/2,fStupidY+fBorder,rScale.z+fBorder);

			glTexCoord2f(0,(rScale.z+fBorder)/fTextureSize+fShift);
			glVertex3f(fBorder/2,fStupidY+fBorder,rScale.z+fBorder);

			glTexCoord2f(0,fShift);
			glVertex3f(fBorder/2,fStupidY+fBorder,0);

			// y minus inside
			glNormal3f(0,-1,0);

			glTexCoord2f(0,0);
			glVertex3f(fBorder/2,fStupidY,0);

			glTexCoord2f(0,(rScale.z)/fTextureSize);
			glVertex3f(fBorder/2,fStupidY,rScale.z);

			glTexCoord2f(fBorder/fTextureSize,(rScale.z)/fTextureSize);
			glVertex3f(-fBorder/2,fStupidY,rScale.z);

			glTexCoord2f(fBorder/fTextureSize,0);
			glVertex3f(-fBorder/2,fStupidY,0);

			// y minus outside
			glNormal3f(0,1,0);

			glTexCoord2f(0,fShift);
			glVertex3f(fBorder/2,-fStupidY-fBorder,0);

			glTexCoord2f(0,(rScale.z+fBorder)/fTextureSize+fShift);
			glVertex3f(fBorder/2,-fStupidY-fBorder,rScale.z+fBorder);

			glTexCoord2f(fBorder/fTextureSize,(rScale.z+fBorder)/fTextureSize+fShift);
			glVertex3f(-fBorder/2,-fStupidY-fBorder,rScale.z+fBorder);

			glTexCoord2f(fBorder/fTextureSize,fShift);
			glVertex3f(-fBorder/2,-fStupidY-fBorder,0);

			// y minus inside
			glNormal3f(0,1,0);

			glTexCoord2f(fBorder/fTextureSize,0);
			glVertex3f(-fBorder/2,-fStupidY,0);

			glTexCoord2f(fBorder/fTextureSize,(rScale.z)/fTextureSize);
			glVertex3f(-fBorder/2,-fStupidY,rScale.z);

			glTexCoord2f(0,(rScale.z)/fTextureSize);
			glVertex3f(fBorder/2,-fStupidY,rScale.z);

			glTexCoord2f(0,0);
			glVertex3f(fBorder/2,-fStupidY,0);


			// y plus Leg bottom
			glNormal3f(0,0,-1);

			glTexCoord2f(fBorder/fTextureSize,fBorder/fTextureSize);
			glVertex3f(fBorder/2,fStupidY+fBorder,0);

			glTexCoord2f(0,fBorder/fTextureSize);
			glVertex3f(fBorder/2,fStupidY,0);

			glTexCoord2f(0,0);
			glVertex3f(-fBorder/2,fStupidY,0);

			glTexCoord2f(fBorder/fTextureSize,0);
			glVertex3f(-fBorder/2,fStupidY+fBorder,0);

			// y minus Leg bottom
			glNormal3f(0,0,-1);

			glTexCoord2f(fBorder/fTextureSize,fBorder/fTextureSize);
			glVertex3f(-fBorder/2,-fStupidY-fBorder,0);

			glTexCoord2f(fBorder/fTextureSize,0);
			glVertex3f(-fBorder/2,-fStupidY,0);

			glTexCoord2f(0,0);
			glVertex3f(fBorder/2,-fStupidY,0);

			glTexCoord2f(0,fBorder/fTextureSize);
			glVertex3f(fBorder/2,-fStupidY-fBorder,0);

		glEnd();

		glPopMatrix();

	glEndList();

	if (iSelList != -1)
		glDeleteLists(iSelList,1);

	iSelList = glGenLists(1);
	
	glNewList(iSelList,GL_COMPILE);

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();
		glTranslatef(rPos.x, rPos.y, rPos.z);
		glRotatef(fRot, 0, 0, 1);

		float	fOffset = 2.5f;
		float	fLen = 5.0f;

		glBegin(GL_LINES);
		
			DrawSelectGrid(fOffset,fLen,rScale);

		glEnd();

		glPopMatrix();
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

	glEndList();
}

void CBaseTopsModel::SetItemInfo ( char* szItem, float *fData )
{
	if (stricmp(szItem,"Color")==0)
		iColor = (int)(*fData);
}

void CBaseTopsModel::Init ( trVertex3D rPos, trVertex3D rScale, float fRot)
{
	if (iGLListID != -1)
		glDeleteLists(iGLListID,1);

	iGLListID = glGenLists(1);

	float	fTexureUnits = 4.0f;
	
	glNewList(iGLListID,GL_COMPILE);

		glPushMatrix();
		glTranslatef(rPos.x, rPos.y, rPos.z);
		glRotatef(fRot, 0, 0, 1);
	
		glDisable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);	// lets hope they make them quads
		
			float	fTopZ = 0.0f;

			if (rPos.z > 0.0f)
				fTopZ = 1.0f;

			// z+ side
			glNormal3f(0,0,1);
			glVertex3f(rScale.x,-rScale.y,fTopZ);

			glVertex3f(rScale.x,rScale.y,fTopZ);

			glVertex3f(-rScale.x,rScale.y,fTopZ);

			glVertex3f(-rScale.x,-rScale.y,fTopZ);

			if (rPos.z > 0.0f)
			{
				// z+ side
				glNormal3f(0,0,-1);
				glVertex3f(-rScale.x,-rScale.y,0);

				glVertex3f(-rScale.x,rScale.y,0);

				glVertex3f(rScale.x,rScale.y,0);

				glVertex3f(rScale.x,-rScale.y,0);
			}

		glEnd();
		
		glEnable(GL_TEXTURE_2D);

		glPopMatrix();

	glEndList();

	if (iSelList != -1)
		glDeleteLists(iSelList,1);

	iSelList = glGenLists(1);
	
	glNewList(iSelList,GL_COMPILE);

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();
		glTranslatef(rPos.x, rPos.y, rPos.z);
		glRotatef(fRot, 0, 0, 1);

		float	fOffset = 2.5f;
		float	fLen = 5.0f;

		glBegin(GL_LINES);
		
			DrawSelectGrid(fOffset,fLen,rScale);

		glEnd();

		glPopMatrix();
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

	glEndList();
}

void CBaseTopsModel::Render ( bool bTrans, bool bSelected )
{
	if (iGLListID ==-1)
		return;
		
	float	fAlphaValue = bTrans?0.25f:1.0f;

	float	afColor[3] = {0.0f,0.0f,0.0f};

	switch(iColor)
	{
		default:
		case 1:
			afColor[0] = 1.0f;
			break;
		case 3:
			afColor[2] = 1.0f;
			break;
		case 2:
			afColor[1] = 1.0f;
			break;
		case 4:
			afColor[0] = afColor[2] = 1.0f;
			break;	
	}

	if (!bSelected)
	{
		afColor[0] *= 0.75f;
		afColor[1] *= 0.75f;
		afColor[2] *= 0.75f;
	}
		
	glColor4f(afColor[0],afColor[1],afColor[2],fAlphaValue);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glCallList(iGLListID);

	glColor4f(1,1,1,1);	
}

CLinkModel::CLinkModel()
{
	void*	pWorld = NULL;
	iFrom = -2;
	iTo = -2;
}

void CLinkModel::Render ( bool bTrans, bool bSelected)
{
	if (!pWorld)
		return;

	struct 
	{
		CBaseObject	*ent;
		int		iItem;
		int		iSide;
	}rTo,rFrom;

	rTo.iItem = iTo/2;
	rTo.iSide = iTo - (rTo.iItem*2);
	rTo.ent = ((CWorld*)pWorld)->GetNthObjectOfType("Teleporter",rTo.iItem);

	rFrom.iItem = iFrom/2;
	rFrom.iSide = iFrom - (rFrom.iItem*2);
	rFrom.ent = ((CWorld*)pWorld)->GetNthObjectOfType("Teleporter",rFrom.iItem);

	float fColor,fAlpha,fBorder,afXSise[2];
	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	if (rFrom.ent)
	{
		// from side
		glPushMatrix();

		glTranslatef(rFrom.ent->GetXPos(), rFrom.ent->GetYPos(), rFrom.ent->GetZPos());
		glRotatef(rFrom.ent->GetRotAngle(), 0, 0, 1);

		fColor = 0.5f;
		if (bSelected)
			fColor = 1.0f;

		fAlpha = 1.0f;
		if (bTrans)
			fAlpha = 0.5f;

		rFrom.ent->GetItemInfo("Border",fBorder);

		if (rFrom.iSide)
		{
			afXSise[0] = -(fBorder/2.0f);
			afXSise[1] = -(fBorder/2.0f)-5.0f;
		}
		else
		{
			afXSise[0] = (fBorder/2.0f);
			afXSise[1] = (fBorder/2.0f)+5.0f;
		}

		glColor4f(0,fColor,0,fAlpha);

		glBegin(GL_QUADS);
			
			glTexCoord2f(0,0.5f);
			glVertex3f(afXSise[0],0,(rFrom.ent->GetZScale()/2-1.25f)-1.25f);

			glTexCoord2f(1.0f,0.5f);
			glVertex3f(afXSise[1],0,(rFrom.ent->GetZScale()/2-1.25f)-1.25f);

			glTexCoord2f(1.0f,1.0f);
			glVertex3f(afXSise[1],0,(rFrom.ent->GetZScale()/2-1.25f)+1.25f);

			glTexCoord2f(0,1.0f);
			glVertex3f(afXSise[0],0,(rFrom.ent->GetZScale()/2-1.25f)+1.25f);

		glEnd();

		glColor4f(1,1,1,1);

		glPopMatrix();
	}

	if (rTo.ent)
	{
		// from side
		glPushMatrix();

		glTranslatef(rTo.ent->GetXPos(), rTo.ent->GetYPos(), rTo.ent->GetZPos());
		glRotatef(rTo.ent->GetRotAngle(), 0, 0, 1);

		fColor = 0.5f;
		if (bSelected)
			fColor = 1.0f;

		fAlpha = 1.0f;
		if (bTrans)
			fAlpha = 0.5f;

		rTo.ent->GetItemInfo("Border",fBorder);

		if (rTo.iSide)
		{
			afXSise[0] = -(fBorder/2.0f);
			afXSise[1] = -(fBorder/2.0f)-5.0f;
		}
		else
		{
			afXSise[0] = (fBorder/2.0f);
			afXSise[1] = (fBorder/2.0f)+5.0f;
		}

		glColor4f(fColor,0,0,fAlpha);

		glBegin(GL_QUADS);
			
			glTexCoord2f(0,0);
			glVertex3f(afXSise[0],0,(rTo.ent->GetZScale()/2+1.25f)-1.25f);

			glTexCoord2f(1.0f,0);
			glVertex3f(afXSise[1],0,(rTo.ent->GetZScale()/2+1.25f)-1.25f);

			glTexCoord2f(1.0f,0.5f);
			glVertex3f(afXSise[1],0,(rTo.ent->GetZScale()/2+1.25f)+1.25f);

			glTexCoord2f(0,0.5f);
			glVertex3f(afXSise[0],0,(rTo.ent->GetZScale()/2+1.25f)+1.25f);

		glEnd();

		glColor4f(1,1,1,1);

		glPopMatrix();
	}

	if (rTo.ent && rFrom.ent && (((CWorld*)pWorld)->GetDrawAll() || bSelected) )
	{
		glDisable(GL_TEXTURE_2D);
		glLineWidth(10.0f);

		glBegin(GL_LINES);
		
			glColor4f(1,0,0,0.5f);
			glVertex3f(rTo.ent->GetXPos(),rTo.ent->GetYPos(),rTo.ent->GetZPos()+rTo.ent->GetZScale());

			glColor4f(0,1,0,0.5f);
			glVertex3f(rFrom.ent->GetXPos(),rFrom.ent->GetYPos(),rFrom.ent->GetZPos()+rFrom.ent->GetZScale());

		glEnd();

		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}

void CLinkModel::SetItemInfo ( char* szItem, float *fData )
{
	if (stricmp(szItem,"World")==0)
		pWorld = (void*)(fData);
	else if (stricmp(szItem,"From")==0)
		iFrom = *(int*)(fData);
	else if (stricmp(szItem,"To")==0)
		iTo = *(int*)(fData);
}


