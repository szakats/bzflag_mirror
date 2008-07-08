
#include "worldDrawing.h"
#include <math.h>

float muzleHeight = 1.57f;
float boxWallUVScale = 1.0f/(muzleHeight*6);
float boxRoofUVScale = boxWallUVScale*5.0f;
float pyramidUVScale = 1.0f/(4.8f*muzleHeight);

/*void ClassicBoxDrawInfo::load ( MapItem *item, Editor *editor  )
{
	ClassicBoxItem	*box = (ClassicBoxItem*)item;

	//int stdWallTexture = editor->findTexture(std::string("boxwall.png"));
	//int stdRoofTexture = editor->findTexture(std::string("roof.png"));

	roofTexture = stdRoofTexture;
	wallTexture = stdWallTexture;

	glList[0] = glGenLists(1);
	glNewList(glList[0],GL_COMPILE);

	glEnable(GL_LIGHTING);

	glBegin(GL_QUADS);
		// draw the top
		glNormal3f(0,0,1);

		glTexCoord2f(box->scale[0]*2*boxRoofUVScale,0);
		glVertex3f(-box->scale[0],box->scale[1],box->scale[2]);

		glTexCoord2f(box->scale[0]*2*boxRoofUVScale,box->scale[1]*2*boxRoofUVScale);
		glVertex3f(-box->scale[0],-box->scale[1],box->scale[2]);
		
		glTexCoord2f(0,box->scale[1]*2*boxRoofUVScale);
		glVertex3f(box->scale[0],-box->scale[1],box->scale[2]);

		glTexCoord2f(0,0);
		glVertex3f(box->scale[0],box->scale[1],box->scale[2]);

		// draw the bottom
		glNormal3f(0,0,-1);
		
		glTexCoord2f(0,0);
		glVertex3f(box->scale[0],box->scale[1],0);
		
		glTexCoord2f(0,box->scale[1]*2*boxRoofUVScale);
		glVertex3f(box->scale[0],-box->scale[1],0);
	
		glTexCoord2f(box->scale[0]*2*boxRoofUVScale,box->scale[1]*2*boxRoofUVScale);
		glVertex3f(-box->scale[0],-box->scale[1],0);

		glTexCoord2f(box->scale[0]*2*boxRoofUVScale,0);
		glVertex3f(-box->scale[0],box->scale[1],0);

	glEnd();

	glEndList();

	glList[1] = glGenLists(1);
	glNewList(glList[1],GL_COMPILE);

	glEnable(GL_LIGHTING);

	glBegin(GL_QUADS);
		// Y- side
		glNormal3f(0,-1,0);

		glTexCoord2f(0,0);
		glVertex3f(-box->scale[0],-box->scale[1],0);
		
		glTexCoord2f(box->scale[0]*2*boxWallUVScale,0);
		glVertex3f(box->scale[0],-box->scale[1],0);
		
		glTexCoord2f(box->scale[0]*2*boxWallUVScale,box->scale[2]*boxWallUVScale);
		glVertex3f(box->scale[0],-box->scale[1],box->scale[2]);
		
		glTexCoord2f(0,box->scale[2]*boxWallUVScale);
		glVertex3f(-box->scale[0],-box->scale[1],box->scale[2]);

		// y+ side
		glNormal3f(0,1,0);
		
		glTexCoord2f(box->scale[0]*2*boxWallUVScale,box->scale[2]*boxWallUVScale);
		glVertex3f(-box->scale[0],box->scale[1],box->scale[2]);
		
		glTexCoord2f(0,box->scale[2]*boxWallUVScale);
		glVertex3f(box->scale[0],box->scale[1],box->scale[2]);
		
		glTexCoord2f(0,0);
		glVertex3f(box->scale[0],box->scale[1],0);
		
		glTexCoord2f(box->scale[0]*2*boxWallUVScale,0);
		glVertex3f(-box->scale[0],box->scale[1],0);

		// x- Side
		glNormal3f(-1,0,0);

		glTexCoord2f(box->scale[1]*2*boxWallUVScale,0);
		glVertex3f(-box->scale[0],box->scale[1],0);
	
		glTexCoord2f(0,0);
		glVertex3f(-box->scale[0],-box->scale[1],0);
	
		glTexCoord2f(0,box->scale[2]*boxWallUVScale);
		glVertex3f(-box->scale[0],-box->scale[1],box->scale[2]);
	
		glTexCoord2f(box->scale[1]*2*boxWallUVScale,box->scale[2]*boxWallUVScale);
		glVertex3f(-box->scale[0],box->scale[1],box->scale[2]);

		// x- Side
		glNormal3f(1,0,0);
		
		glTexCoord2f(box->scale[1]*2*boxWallUVScale,box->scale[2]*boxWallUVScale);
		glVertex3f(box->scale[0],box->scale[1],box->scale[2]);
		
		glTexCoord2f(0,box->scale[2]*boxWallUVScale);
		glVertex3f(box->scale[0],-box->scale[1],box->scale[2]);
		
		glTexCoord2f(0,0);
		glVertex3f(box->scale[0],-box->scale[1],0);
		
		glTexCoord2f(box->scale[1]*2*boxWallUVScale,0);
		glVertex3f(box->scale[0],box->scale[1],0);
	glEnd();

	glEndList();
}*/

void ClassicBoxDrawInfo::draw ( void )
{
	//TextureSystem &ts = TextureSystem::Instance();

	//glTexturesOn();
	
//	glColor4f(1,1,1,1.0f);

	//ts.bind(roofTexture);
//	glCallList(glList[0]);
//
	//ts.bind(wallTexture);
//	glCallList(glList[1]);

	//glTexturesOff();
}

/*void ClassicPyramidDrawInfo::load ( MapItem *item, Editor *editor )
{
	ClassicPyramidItem	*pry = (ClassicPyramidItem*)item;

	//int stdpryTexture = editor->findTexture(std::string("pyrwall.png"));

	pryTexture = stdpryTexture;

	glPushMatrix();
	if (pry->flipedZ)
	{
		glTranslatef(0,0,pry->scale[2]*0.5f);
		glRotatef(180,1,0,0);
	}
	glList = glGenLists(1);
	glNewList(glList,GL_COMPILE);
	glEnable(GL_LIGHTING);

	// bottom
	glBegin(GL_QUADS);

		glNormal3f(0,0,-1);

		glTexCoord2f(0,0);
		glVertex3f(pry->scale[0],pry->scale[1],0);

		glTexCoord2f(0,pry->scale[1]*2*pyramidUVScale);
		glVertex3f(pry->scale[0],-pry->scale[1],0);

		glTexCoord2f(pry->scale[0]*2*pyramidUVScale,pry->scale[1]*2*pyramidUVScale);
		glVertex3f(-pry->scale[0],-pry->scale[1],0);

		glTexCoord2f(pry->scale[0]*2*pyramidUVScale,0);
		glVertex3f(-pry->scale[0],pry->scale[1],0);

	glEnd();

	
	// compute the distance along the vector
	float textureDistance = sqrtf(pry->scale[0]*pry->scale[0]+pry->scale[1]*pry->scale[1]+pry->scale[2]*pry->scale[2]);

	glBegin(GL_TRIANGLES);

		// y- side
		float norm[2];
		norm[0] = -pry->scale[2];
		norm[1] = pry->scale[1];

		float dist = sqrtf(norm[0]*norm[0]+norm[1]*norm[1]);
		norm[0] /= dist;
		norm[1] /= dist;

		glNormal3f(0,norm[0],norm[1]);

		glTexCoord2f(pry->scale[0]*2*pyramidUVScale,0);
		glVertex3f(pry->scale[0],-pry->scale[1],0);
		
		glTexCoord2f(0,textureDistance*pyramidUVScale);
		glVertex3f(0,0,pry->scale[2]);

		glTexCoord2f(0,0);
		glVertex3f(-pry->scale[0],-pry->scale[1],0);


		// y+ side
		glNormal3f(0,-norm[0],norm[1]);

		glTexCoord2f(0,0);
		glVertex3f(-pry->scale[0],pry->scale[1],0);
	
		glTexCoord2f(0,textureDistance*pyramidUVScale);
		glVertex3f(0,0,pry->scale[2]);
	
		glTexCoord2f(pry->scale[0]*2*pyramidUVScale,0);
		glVertex3f(pry->scale[0],pry->scale[1],0);

		//x- side
		norm[0] = -pry->scale[2];
		norm[1] = pry->scale[0];

		dist = sqrtf(norm[0]*norm[0]+norm[1]*norm[1]);
		norm[0] /= dist;
		norm[1] /= dist;

		glNormal3f(norm[0],0,norm[1]);

		glTexCoord2f(0,0);
		glVertex3f(-pry->scale[0],pry->scale[1],0);

		glTexCoord2f(pry->scale[0]*2*pyramidUVScale,0);
		glVertex3f(-pry->scale[0],-pry->scale[1],0);

		glTexCoord2f(0,textureDistance*pyramidUVScale);
		glVertex3f(0,0,pry->scale[2]);


		//x+ side
		glNormal3f(-norm[0],0,norm[1]);

		glTexCoord2f(0,textureDistance*pyramidUVScale);
		glVertex3f(0,0,pry->scale[2]);

		glTexCoord2f(pry->scale[0]*2*pyramidUVScale,0);
		glVertex3f(pry->scale[0],-pry->scale[1],0);

		glTexCoord2f(0,0);
		glVertex3f(pry->scale[0],pry->scale[1],0);

	glEnd();

	glPopMatrix();
	glEndList();
}*/

void ClassicPyramidDrawInfo::draw ( void )
{
	//TextureSystem &ts = TextureSystem::Instance();

	//glTexturesOn();

//	glColor4f(1,1,1,1.0f);

	//ts.bind(pryTexture);
//	glCallList(glList);

	//glTexturesOff();
}

