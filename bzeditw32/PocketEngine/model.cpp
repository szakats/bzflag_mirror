// model.cpp

#include "model.h"

#include<VECTOR>

void WriteLine ( FILE* fp, char *szText );
char* GetNextObjLine ( char* data, char* szLine );

void CModel::Init ( void )
{
	m_bTextured = false;
	m_bLit = true;

	m_iNorms = 0;
	m_iVerts = 0;
	m_iTCs = 0;
	m_iTris = 0;
	m_pVerts = NULL;
	m_pNorms = NULL;
	m_pTCs = NULL;
	m_pTris = NULL;

	m_rColor.r = m_rColor.g = m_rColor.b = 0.5f;
	m_rColor.b = 1.0;

	m_iList = 0;
	m_iTexture = 0;

	memset(m_szTexture,0,255);
	
	m_aMatrix[0] = 1; m_aMatrix[1] = 0; m_aMatrix[2] = 0; m_aMatrix[3] = 0;
	m_aMatrix[4] = 0; m_aMatrix[5] = 1; m_aMatrix[6] = 0; m_aMatrix[7] = 0;
	m_aMatrix[8] = 0; m_aMatrix[9] = 0; m_aMatrix[10] = 1; m_aMatrix[11] = 0;
	m_aMatrix[12] = 0; m_aMatrix[13] = 0; m_aMatrix[14] = 0; m_aMatrix[15] = 1;

	m_aEulers[0] = m_aEulers[1] = m_aEulers[2] = m_aEulers[3] = m_aEulers[4] = m_aEulers[5] = 0;
	m_szFilePath[0] = NULL;

}

CModel::CModel()
{
	Init();
}

CModel::~CModel()
{
	Clear();
}


bool CModel::Load ( const char* fileName )
{
	FILE *fp = fopen (fileName,"rb");
	if (!fp)
		return false;

	// save the folder
	strcpy(m_szFilePath,fileName);
	char*	temp = strrchr(m_szFilePath,'\\');
	if (temp)
		*(temp+1) = NULL;
	else
		m_szFilePath[0] = NULL;
	
	if (Valid())
		Clear();

	m_eInputType = ClasifyFile((char*)fileName);

	switch (m_eInputType)
	{
		case eUnknownFile:
			return false;
		break;

		case eFBFile:
			LoadFB(fp);
		break;

		case eOBJFile:
			LoadOBJ(fp);
		break;

		case eTMFFile:
			LoadTMF(fp);
		break;

		case eBMFFile:
			LoadBMF(fp);
		break;

		case eRawFile:
			LoadRAW(fp);
		break;

		case eSTLFile:
			LoadSTL(fp);
		break;
	}
	fclose(fp);

	BuildList();
	return Valid();
}

void CModel::LoadFB ( FILE *fp )
{
	int	iSize;

	fread(&iSize,4,1,fp);

	if (iSize <= 0)
		return;

	m_iTris = iSize;
	m_iVerts = m_iNorms = iSize*3;
	m_iTCs = 0;

	m_pVerts = (trVertex3D*) malloc (sizeof(trVertex3D) * m_iVerts);
	m_pNorms = (trVertex3D*) malloc (sizeof(trVertex3D) * m_iNorms);
	m_pTCs = NULL;
	m_pTris = (trTriangle*) malloc(sizeof(trTriangle) * m_iTris);

	double v[3],n[3];

	int	i,t = 0;
	for ( i = 0; i < iSize; i++)
	{
		fread(v,sizeof(double)*3,1,fp);
		fread(n,sizeof(double)*3,1,fp);
		m_pVerts[t].x = (float)v[0]; m_pVerts[t].y = (float)v[1]; m_pVerts[t].z = (float)v[2];
		m_pNorms[t].x = (float)n[0]; m_pNorms[t].y = (float)n[1]; m_pNorms[t].z = (float)n[2];
		
		m_pTris[i].aVerts[0] = t;
		m_pTris[i].aNorms[0] = t;
		m_pTris[i].aTCs[0] = -1;
		t++;

		fread(v,sizeof(double)*3,1,fp);
		fread(n,sizeof(double)*3,1,fp);
		m_pVerts[t].x = (float)v[0]; m_pVerts[t].y = (float)v[1]; m_pVerts[t].z = (float)v[2];
		m_pNorms[t].x = (float)n[0]; m_pNorms[t].y = (float)n[1]; m_pNorms[t].z = (float)n[2];
		
		m_pTris[i].aVerts[1] = t;
		m_pTris[i].aNorms[1] = t;
		m_pTris[i].aTCs[1] = -1;
		t++;

		fread(v,sizeof(double)*3,1,fp);
		fread(n,sizeof(double)*3,1,fp);
		m_pVerts[t].x = (float)v[0]; m_pVerts[t].y = (float)v[1]; m_pVerts[t].z = (float)v[2];
		m_pNorms[t].x = (float)n[0]; m_pNorms[t].y = (float)n[1]; m_pNorms[t].z = (float)n[2];
		
		m_pTris[i].aVerts[2] = t;
		m_pTris[i].aNorms[2] = t;
		m_pTris[i].aTCs[2] = -1;
		t++;

		m_pTris[i].rFaceNormal.x = m_pTris[i].rFaceNormal.y = m_pTris[i].rFaceNormal.z = 0;
	}
	
	m_bTextured = false;
	m_bLit = true;

	// make the face normals
	for ( i = 0; i < m_iTris; i++)
	{
		trVertex3D	p1,p2;

		p1 = m_pVerts[m_pTris[i].aVerts[1]] - m_pVerts[m_pTris[i].aVerts[0]];
		p2 = m_pVerts[m_pTris[i].aVerts[2]] - m_pVerts[m_pTris[i].aVerts[0]];

		cross(p1,p2,m_pTris[i].rFaceNormal);

		m_pTris[i].rFaceNormal /= get_mag(m_pTris[i].rFaceNormal);
	}

	// put it back to the begining just to be nice
	fseek(fp,0,SEEK_SET);
}

void CModel::LoadOBJ ( FILE *fp  )
{
	char	*data,*cur,line[255],*mtlData;
	unsigned int	len;

	// use STLverctors to hold the data till the end
	std::vector<trVertex3D>		verts,norms;
	std::vector<trVertex2D>		tcs;
	std::vector<trTriangle>		tris;

	std::vector<trVertex3D>::iterator	v3ittr;
	std::vector<trVertex2D>::iterator	v2ittr;
	std::vector<trTriangle>::iterator	tittr;

	int	i;

	char	szMatFileName[255],*temp,szObjectName[255],num[32];

	trVertex3D		vert,norm;
	trVertex2D		tc;
	trTriangle		tri;

	// get the data into a string+
	fseek(fp,0,SEEK_END);
	len = ftell(fp);
	fseek(fp,0,SEEK_SET);

	data = (char*)malloc(len+4);

	fread(data,len,1,fp);
	fseek(fp,0,SEEK_SET);

	m_szTexture[0] = NULL;

	// terminate data with space CR and NULL
	data[len] = ' ';
	data[len+1] = 13;
	data[len+2] = 10;
	data[len+3] = NULL;

	cur = data;
	cur = GetNextObjLine(cur,line);
	while (cur != NULL)
	{
		switch (line[0])
		{
			case 'm':
				temp = strchr(line,'b');
				if (temp)
				{
					temp+=2;
					strcpy(szMatFileName,m_szFilePath);
					strcat(szMatFileName,temp);
					FILE	*matFB;
					matFB = fopen (szMatFileName,"rb");
					if (!matFB)
						break;

					fseek(matFB,0,SEEK_END);
					len = ftell(matFB);
					fseek(matFB,0,SEEK_SET);

					mtlData = (char*)malloc(len+1);

					fread(mtlData,len,1,matFB);
					fclose(matFB);

					mtlData[len] = NULL;

					// find the texture
					temp = strstr(mtlData,"map_");
					if (temp)
					{
						temp = strchr(temp,'d');
						if (temp)
						{
							temp+=2;
							strcpy(m_szTexture,temp);

							temp = &m_szTexture[strlen(m_szTexture)-1];

							while (isspace(*temp))
								(*temp--) = NULL;
						}
					}

					// find the material
					temp = strstr(mtlData,"Kd");
					if (temp)
						sscanf(temp,"Kd %f %f %f",&m_rColor.r, &m_rColor.b,&m_rColor.g);
				}
			break;

			case 'v':
				switch(line[1])
				{
					case 't':
						sscanf(line,"vt %f %f",&tc.s,&tc.t);
						tc.t = -tc.t;
						tcs.push_back(tc);
					break;

					case 'n':
						sscanf(line,"vn %f %f %f",&norm.x,&norm.y,&norm.z);
						norms.push_back(norm);
					break;

					default:
						sscanf(line,"v %f %f %f",&vert.x,&vert.y,&vert.z);
						verts.push_back(vert);
					break;
				}
			break;

			case 'g':
				temp = line;
				temp+=2;
				strcpy(szObjectName,temp);	
			break;

			case 's':
				// smothing group
			break;

			case 'u':
				/// material
			break;

			case 'f':
				// face
				temp = line;
				temp += 2;
				char *slash;

				for (i = 0; i < 3; i ++)
				{
					// vert ID
					slash = strchr(temp,'/');
					if (slash != temp)
					{
						strncpy(num,temp,slash-temp);
						num[slash-temp] = NULL;
						tri.aVerts[i] = atoi(num) - 1;
					}
					else
						tri.aVerts[i] = 0; // this would be bad

					temp = slash+1;
					// see if theres a texture ID
					if (*temp != '/')
					{ // got somethin
						slash = strchr(temp,'/');
						strncpy(num,temp,slash-temp);
						num[slash-temp] = NULL;
						tri.aTCs[i] = atoi(num) - 1;
					}
					else
						tri.aTCs[i] = -1;

					temp = slash+1;

					if (isdigit(*temp) != 0)
						tri.aNorms[i] = atoi(temp)-1;
					else
						tri.aNorms[i] = 0;

					if (i != 2)
					{ //cus the next one will end it
						slash = strchr(temp,' ');
						if (slash)
							temp = slash+1;
						else
							i = 3;
					}
				}
				tris.push_back(tri);
			break;
		}
		cur = GetNextObjLine(cur,line);
	}

	m_iTris = tris.size();
	if (m_iTris)
	{
		m_pTris = (trTriangle*) malloc(sizeof(trTriangle) * m_iTris);
		tittr = tris.begin();
		i = 0;
		while (tittr != tris.end())
			m_pTris[i++] = (*tittr++);
	}

	m_iVerts = verts.size();
	if (m_iVerts)
	{
		m_pVerts = (trVertex3D*) malloc (sizeof(trVertex3D) * m_iVerts);
		v3ittr = verts.begin();
		i = 0;
		while (v3ittr != verts.end())
			m_pVerts[i++] = (*v3ittr++);
	}

	m_iNorms = norms.size();
	if (m_iNorms)
	{
		m_pNorms = (trVertex3D*) malloc (sizeof(trVertex3D) * m_iNorms);
		v3ittr = norms.begin();
		i = 0;
		while (v3ittr != norms.end())
			m_pNorms[i++] = (*v3ittr++);
	}

	m_iTCs = tcs.size();
	if (m_iTCs)
	{
		m_pTCs = (trVertex2D*) malloc (sizeof(trVertex2D) * m_iTCs);
		v2ittr = tcs.begin();
		i = 0;
		while (v2ittr != tcs.end())
			m_pTCs[i++] = (*v2ittr++);
	}

	if (m_szTexture[0])
		m_bTextured = true;
	else
		m_bTextured = false;

	// make the face normals
	for ( i = 0; i < m_iTris; i++)
	{
		trVertex3D	p1,p2;

		p1 = m_pVerts[m_pTris[i].aVerts[1]] - m_pVerts[m_pTris[i].aVerts[0]];
		p2 = m_pVerts[m_pTris[i].aVerts[2]] - m_pVerts[m_pTris[i].aVerts[0]];

		cross(p1,p2,m_pTris[i].rFaceNormal);

		m_pTris[i].rFaceNormal /= get_mag(m_pTris[i].rFaceNormal);
	}
}

void CModel::LoadTMF ( FILE *fp  )
{
	unsigned int		i,t;

	fscanf(fp,"VERTs: %d\n",&m_iVerts);
	m_pVerts = (trVertex3D*) malloc (sizeof(trVertex3D) * m_iVerts);

	for (i = 0; i < m_iVerts; i++)
		fscanf(fp,"%f %f %f\n",&m_pVerts[i].x,&m_pVerts[i].y,&m_pVerts[i].z);


	fscanf(fp,"NORMs: %d\n",&m_iNorms);
	m_pNorms = (trVertex3D*) malloc (sizeof(trVertex3D) * m_iNorms);

	for (i = 0; i < m_iNorms; i++)
		fscanf(fp,"%f %f %f\n",&m_pNorms[i].x,&m_pNorms[i].y,&m_pNorms[i].z);


	fscanf(fp,"TCs: %d\n",&m_iTCs);
	m_pTCs = (trVertex2D*) malloc (sizeof(trVertex2D) * m_iTCs);

	for (i = 0; i < m_iTCs; i++)
		fscanf(fp,"%f %f\n",&m_pTCs[i].s,&m_pTCs[i].t);


	fscanf(fp,"TRIs: %d\n",&m_iTris);
	m_pTris = (trTriangle*) malloc(sizeof(trTriangle) * m_iTris);

	for (i = 0; i < m_iTris; i++)
	{
		for (t = 0; t < 3; t++)
			fscanf(fp,"%d,%d,%d ",&m_pTris[i].aVerts[t],&m_pTris[i].aNorms[t], &m_pTris[i].aTCs[t]);
		fscanf(fp,"%f,%f,%f\n",&m_pTris[i].rFaceNormal.x,&m_pTris[i].rFaceNormal.y, &m_pTris[i].rFaceNormal.z);
	}

	fscanf(fp,"TEXTURE: %s\n",m_szTexture);

	fscanf(fp,"COLOR: %f %f %f\n",&m_rColor.r,&m_rColor.g,&m_rColor.b);

	if (m_szTexture[0])
		m_bTextured = true;
	else
		m_bTextured = false;
}

void CModel::LoadMemBMF ( char*	pData )
{
	char		*p = pData;

	if (!p)
		return;
	
	unsigned int		i;

	memcpy(&m_iVerts,p,sizeof(unsigned int));
	p+=sizeof(unsigned int);
	
	m_pVerts = (trVertex3D*) malloc (sizeof(trVertex3D) * m_iVerts);
	
	for (i = 0; i < m_iVerts; i++)
	{
		memcpy(&m_pVerts[i],p,sizeof(float)*3);
		p+=sizeof(float)*3;
	}

	memcpy(&m_iNorms,p,sizeof(unsigned int));
	p+=sizeof(unsigned int);
	
	m_pNorms = (trVertex3D*) malloc (sizeof(trVertex3D) * m_iNorms);

	for (i = 0; i < m_iNorms; i++)
	{
		memcpy(&m_pNorms[i],p,sizeof(float)*3);
		p+=sizeof(float)*3;
	}

	memcpy(&m_iTCs,p,sizeof(unsigned int));
	p+=sizeof(unsigned int);

	m_pTCs = (trVertex2D*) malloc (sizeof(trVertex2D) * m_iTCs);

	for (i = 0; i < m_iTCs; i++)
	{
		memcpy(&m_pTCs[i],p,sizeof(float)*3);
		p+=sizeof(float)*3;
	}

	memcpy(&m_iTris,p,sizeof(unsigned int));
	p+=sizeof(unsigned int);

	m_pTris = (trTriangle*) malloc(sizeof(trTriangle) * m_iTris);

	for (i = 0; i < m_iTris; i++)
	{
		memcpy(&m_pTris[i],p,sizeof(float)*3);
		p+=sizeof(float)*3;
	}

	memcpy(m_szTexture,p,255);

	if (m_szTexture[0])
	{
		m_iTexture = 0;
		m_bTextured = true;
	}
	else
	{
		m_iTexture = 0;
		m_bTextured = false;
	}

	memcpy(&m_rColor,p,sizeof(trColor));
}

void CModel::LoadBMF ( FILE *fp  )
{
	unsigned int		i;
	char				temp[255];

	fread(&m_iVerts,sizeof(unsigned int),1,fp);
	m_pVerts = (trVertex3D*) malloc (sizeof(trVertex3D) * m_iVerts);
	
	for (i = 0; i < m_iVerts; i++)
		fread(&m_pVerts[i],sizeof(float)*3,1,fp);


	fread(&m_iNorms,sizeof(unsigned int),1,fp);
	m_pNorms = (trVertex3D*) malloc (sizeof(trVertex3D) * m_iNorms);

	for (i = 0; i < m_iNorms; i++)
		fread(&m_pNorms[i],sizeof(float)*3,1,fp);


	fread(&m_iTCs,sizeof(unsigned int),1,fp);
	m_pTCs = (trVertex2D*) malloc (sizeof(trVertex2D) * m_iTCs);

	for (i = 0; i < m_iTCs; i++)
		fread(&m_pTCs[i],sizeof(float)*2,1,fp);


	fread(&m_iTris,sizeof(unsigned int),1,fp);
	m_pTris = (trTriangle*) malloc(sizeof(trTriangle) * m_iTris);

	for (i = 0; i < m_iTris; i++)
		fread(&m_pTris[i],sizeof(trTriangle),1,fp);


	fread(m_szTexture,255,1,fp);
	if (m_szTexture[0])
	{
		strcpy(temp,m_szFilePath);
		strcat(temp,m_szTexture);
		m_bTextured = true;
	}
	else
	{
		m_iTexture = 0;
		m_bTextured = false;
	}

	fread(&m_rColor,sizeof(trColor),1,fp);
}

void CModel::LoadRAW ( FILE *fp  )
{

}

void CModel::LoadSTL ( FILE *fp  )
{

}

bool CModel::Save ( const char* fileName, teFileType fileType )
{
	FILE	*fp = fopen(fileName,"wb");

	if (!fp || !m_iTris)
		return false;

	switch (fileType)
	{
		case eUnknownFile:
			return false;
		break;

		case eFBFile:
			SaveFB(fp);
		break;

		case eOBJFile:
			SaveOBJ(fp);
		break;

		case eTMFFile:
			SaveTMF(fp);
		break;

		case eBMFFile:
			SaveBMF(fp);
		break;

		case eRawFile:
			SaveRAW(fp);
		break;

		case eSTLFile:
			SaveSTL(fp);
		break;

		default:
			return false;
	}
	fclose(fp);
	return true;
}

void CModel::SaveFB ( FILE *fp )
{
	int	iSize = m_iTris;

	fwrite(&iSize,4,1,fp);

	double v[3],n[3];

	for ( int i = 0; i < iSize; i++)
	{
		v[0] = m_pVerts[m_pTris[i].aVerts[0]].x; v[1] = m_pVerts[m_pTris[i].aVerts[0]].y; v[2] = m_pVerts[m_pTris[i].aVerts[0]].z;
		n[0] = m_pNorms[m_pTris[i].aNorms[0]].x; n[1] = m_pNorms[m_pTris[i].aNorms[0]].y; n[2] = m_pNorms[m_pTris[i].aNorms[0]].z;

		fwrite(v,sizeof(double)*3,1,fp);
		fwrite(n,sizeof(double)*3,1,fp);


		v[0] = m_pVerts[m_pTris[i].aVerts[1]].x; v[1] = m_pVerts[m_pTris[i].aVerts[1]].y; v[2] = m_pVerts[m_pTris[i].aVerts[1]].z;
		n[0] = m_pNorms[m_pTris[i].aNorms[1]].x; n[1] = m_pNorms[m_pTris[i].aNorms[1]].y; n[2] = m_pNorms[m_pTris[i].aNorms[1]].z;

		fwrite(v,sizeof(double)*3,1,fp);
		fwrite(n,sizeof(double)*3,1,fp);


		v[0] = m_pVerts[m_pTris[i].aVerts[2]].x; v[1] = m_pVerts[m_pTris[i].aVerts[2]].y; v[2] = m_pVerts[m_pTris[i].aVerts[2]].z;
		n[0] = m_pNorms[m_pTris[i].aNorms[2]].x; n[1] = m_pNorms[m_pTris[i].aNorms[2]].y; n[2] = m_pNorms[m_pTris[i].aNorms[2]].z;

		fwrite(v,sizeof(double)*3,1,fp);
		fwrite(n,sizeof(double)*3,1,fp);
	}
}

void CModel::SaveOBJ ( FILE *fp  )
{

}

void CModel::SaveTMF ( FILE *fp  )
{
	char	line[255],temp[255];
	unsigned int		i,t;

	sprintf(line,"VERTs: %d",m_iVerts);
	WriteLine(fp,line);

	for (i = 0; i < m_iVerts; i++)
	{
		sprintf(line,"%f %f %f",m_pVerts[i].x,m_pVerts[i].y,m_pVerts[i].z);
		WriteLine(fp,line);
	}

	sprintf(line,"NORMs: %d",m_iNorms);
	WriteLine(fp,line);

	for (i = 0; i < m_iNorms; i++)
	{
		sprintf(line,"%f %f %f",m_pNorms[i].x,m_pNorms[i].y,m_pNorms[i].z);
		WriteLine(fp,line);
	}

	sprintf(line,"TCs: %d",m_iTCs);
	WriteLine(fp,line);

	for (i = 0; i < m_iTCs; i++)
	{
		sprintf(line,"%f %f",m_pTCs[i].s,m_pTCs[i].t);
		WriteLine(fp,line);
	}

	sprintf(line,"TRIs: %d",m_iTris);
	WriteLine(fp,line);

	for (i = 0; i < m_iTris; i++)
	{
		line[0] = NULL;
		for (t = 0; t < 3; t++)
		{
			sprintf(temp,"%d,%d,%d ",m_pTris[i].aVerts[t],m_pTris[i].aNorms[t], m_pTris[i].aTCs[t]);
			strcat(line,temp);
		}
		sprintf(temp,"%f,%f,%f",m_pTris[i].rFaceNormal.x,m_pTris[i].rFaceNormal.y, m_pTris[i].rFaceNormal.z);
		strcat(line,temp);
		WriteLine(fp,line);
	}

	if (strrchr(m_szTexture,'\\'))
		strcpy(temp,strrchr(m_szTexture,'\\')+1);
	else
		strcpy(temp,m_szTexture);

	sprintf(line,"TEXTURE: %s",temp);
	WriteLine(fp,line);

	sprintf(line,"COLOR: %f %f %f",m_rColor.r,m_rColor.g,m_rColor.b);
	WriteLine(fp,line);
}

void CModel::SaveBMF ( FILE *fp  )
{
	unsigned int		i;
	char				szTemp[255],*p;

	fwrite(&m_iVerts,sizeof(unsigned int),1,fp);
	for (i = 0; i < m_iVerts; i++)
		fwrite(&m_pVerts[i],sizeof(float)*3,1,fp);

	fwrite(&m_iNorms,sizeof(unsigned int),1,fp);
	for (i = 0; i < m_iNorms; i++)
		fwrite(&m_pNorms[i],sizeof(float)*3,1,fp);

	fwrite(&m_iTCs,sizeof(unsigned int),1,fp);
	for (i = 0; i < m_iTCs; i++)
		fwrite(&m_pTCs[i],sizeof(float)*2,1,fp);

	fwrite(&m_iTris,sizeof(unsigned int),1,fp);

	for (i = 0; i < m_iTris; i++)
		fwrite(&m_pTris[i],sizeof(trTriangle),1,fp);


	p = strrchr(m_szTexture,'\\');
	if (p)
		strcpy(szTemp,p+1);
	else
		strcpy(szTemp,m_szTexture);

	fwrite(&szTemp,255,1,fp);
	fwrite(&m_rColor,sizeof(trColor),1,fp);
}

void CModel::SaveRAW ( FILE *fp  )
{
}

void CModel::SaveSTL ( FILE *fp  )
{

}

void CModel::Clear ( void )
{
	if (m_pVerts)
		free (m_pVerts);

	if (m_pNorms)
		free (m_pNorms);

	if (m_pTCs)
		free (m_pTCs);

	if (m_pTris)
		free (m_pTris);

	Init();
}

void CModel::Draw ( void )
{
	if (!m_iList)
		return;

	glPushMatrix();
	glTranslatef(m_aEulers[0],m_aEulers[1],m_aEulers[2]);
	glRotatef(m_aEulers[3],1,0,0);
	glRotatef(m_aEulers[4],0,1,0);
	glRotatef(m_aEulers[5],0,0,1);

	glColor3f(m_rColor.r,m_rColor.g,m_rColor.b);

	if (m_bTextured)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_iTexture);
	}
	else
		glDisable(GL_TEXTURE_2D);

	if (!m_bLit)
		glDisable(GL_LIGHTING);
	else
		glEnable(GL_LIGHTING);

	if (m_bLit)
		glCallList(m_iList);

		glColor3f(m_rColor.r,m_rColor.g,m_rColor.b);

		glBegin(GL_TRIANGLES );
		for (unsigned int i = 0; i< m_iTris; i++)
		{
			if (m_bTextured)
				glTexCoord2f(m_pTCs[m_pTris[i].aTCs[0]].s,m_pTCs[m_pTris[i].aTCs[0]].t);
			glNormal3f(m_pNorms[m_pTris[i].aNorms[0]].x,m_pNorms[m_pTris[i].aNorms[0]].y,m_pNorms[m_pTris[i].aNorms[0]].z);
			glVertex3f(m_pVerts[m_pTris[i].aVerts[0]].x,m_pVerts[m_pTris[i].aVerts[0]].y,m_pVerts[m_pTris[i].aVerts[0]].z);

			if (m_bTextured)
				glTexCoord2f(m_pTCs[m_pTris[i].aTCs[1]].s,m_pTCs[m_pTris[i].aTCs[1]].t);
			glNormal3f(m_pNorms[m_pTris[i].aNorms[1]].x,m_pNorms[m_pTris[i].aNorms[1]].y,m_pNorms[m_pTris[i].aNorms[1]].z);
			glVertex3f(m_pVerts[m_pTris[i].aVerts[1]].x,m_pVerts[m_pTris[i].aVerts[1]].y,m_pVerts[m_pTris[i].aVerts[1]].z);

			if (m_bTextured)
				glTexCoord2f(m_pTCs[m_pTris[i].aTCs[2]].s,m_pTCs[m_pTris[i].aTCs[2]].t);
			glNormal3f(m_pNorms[m_pTris[i].aNorms[2]].x,m_pNorms[m_pTris[i].aNorms[2]].y,m_pNorms[m_pTris[i].aNorms[2]].z);
			glVertex3f(m_pVerts[m_pTris[i].aVerts[2]].x,m_pVerts[m_pTris[i].aVerts[2]].y,m_pVerts[m_pTris[i].aVerts[2]].z);
		}
		glEnd();

	glPopMatrix();
}

void CModel::BuildList ( void )
{
	unsigned int i;

	if (m_iList>0)
		glDeleteLists(m_iList,1);
		
	m_iList = glGenLists(1);

	glNewList(m_iList,GL_COMPILE);
	
		if (m_bTextured)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_iTexture);
		}
		else
			glDisable(GL_TEXTURE_2D);

		if (!m_bLit)
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);

		glColor3f(m_rColor.r,m_rColor.g,m_rColor.b);

		glBegin(GL_TRIANGLES );
		for ( i = 0; i< m_iTris; i++)
		{
			if (m_bTextured)
				glTexCoord2f(m_pTCs[m_pTris[i].aTCs[0]].s,m_pTCs[m_pTris[i].aTCs[0]].t);
			glNormal3f(m_pNorms[m_pTris[i].aNorms[0]].x,m_pNorms[m_pTris[i].aNorms[0]].y,m_pNorms[m_pTris[i].aNorms[0]].z);
			glVertex3f(m_pVerts[m_pTris[i].aVerts[0]].x,m_pVerts[m_pTris[i].aVerts[0]].y,m_pVerts[m_pTris[i].aVerts[0]].z);

			if (m_bTextured)
				glTexCoord2f(m_pTCs[m_pTris[i].aTCs[1]].s,m_pTCs[m_pTris[i].aTCs[1]].t);
			glNormal3f(m_pNorms[m_pTris[i].aNorms[1]].x,m_pNorms[m_pTris[i].aNorms[1]].y,m_pNorms[m_pTris[i].aNorms[1]].z);
			glVertex3f(m_pVerts[m_pTris[i].aVerts[1]].x,m_pVerts[m_pTris[i].aVerts[1]].y,m_pVerts[m_pTris[i].aVerts[1]].z);

			if (m_bTextured)
				glTexCoord2f(m_pTCs[m_pTris[i].aTCs[2]].s,m_pTCs[m_pTris[i].aTCs[2]].t);
			glNormal3f(m_pNorms[m_pTris[i].aNorms[2]].x,m_pNorms[m_pTris[i].aNorms[2]].y,m_pNorms[m_pTris[i].aNorms[2]].z);
			glVertex3f(m_pVerts[m_pTris[i].aVerts[2]].x,m_pVerts[m_pTris[i].aVerts[2]].y,m_pVerts[m_pTris[i].aVerts[2]].z);
		}
		glEnd();

	glEndList();
}

teFileType CModel::ClasifyFile ( char* szFileName )
{
	if (!szFileName)
		return eUnknownFile;

	char	*ptr = strrchr(szFileName,'.');
	if (!ptr)
		return eUnknownFile;

	if (stricmp(ptr+1,"FB") == 0)
		return eFBFile;

	if (stricmp(ptr+1,"OBJ") == 0)
		return eOBJFile;

	if (stricmp(ptr+1,"TMF") == 0)
		return eTMFFile;

	if (stricmp(ptr+1,"BMF") == 0)
		return eBMFFile;

	if (stricmp(ptr+1,"RAW") == 0)
		return eRawFile;

	if (stricmp(ptr+1,"TXT") == 0)
		return eRawFile;
	
	if (stricmp(ptr+1,"STL") == 0)
		return eSTLFile;

	return eUnknownFile;
}

void WriteLine ( FILE* fp, char *szText )
{
	char cr[2] = {13,10};

	fwrite(szText,strlen(szText),1,fp);
	fwrite(cr,2,1,fp);
}

// just easyer this way
#define _BAIL_ON_NULL if (*current == NULL)return NULL

char* GetNextObjLine ( char* data, char* szLine )
{
	char *current = data, *lineHead,*lineEnd,*outPointer;

	_BAIL_ON_NULL;

	// get us past any spaces
	while (isspace(*current) != 0)
	{
		_BAIL_ON_NULL;
		current++;
	}
	lineHead = current;
	
	// get us to the CR
	while ((*current) != 10)
	{
		_BAIL_ON_NULL;
		current++;
	}

	outPointer = lineEnd = current;

	while (isspace(*lineEnd) != 0)
		lineEnd--;

	strncpy(szLine,lineHead,lineEnd-lineHead+1);
	szLine[lineEnd-lineHead+1] = NULL;

	return outPointer;
}	

// UI classes
// UI sprite Item
CUItem::CUItem()
{
	m_iTextureID = 0;
	m_iList = 0;
	m_iWidth = 0;
	m_iHeight = 0;
}

CUItem::~CUItem()
{
	if (m_iList>0)
		glDeleteLists(m_iList,1);
}

bool CUItem::SetItemInfo ( UINT	iTextureID, UINT x, UINT y )
{
	if (Valid())
		return false;

	m_iTextureID = iTextureID;

	m_iWidth = x;
	m_iHeight = y;

	if (m_iTextureID<1)
		return false;

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, m_iTextureID);

	m_iList = glGenLists(1);

	glPushMatrix();
	glLoadIdentity();
		glNewList(m_iList,GL_COMPILE);
			glColor4f(1,1,1,1);

			glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex3f(0,0,0);

				glTexCoord2f(1,0);
				glVertex3f((float)x,0,0);

				glTexCoord2f(1,1);
				glVertex3f((float)x,(float)y,0);

				glTexCoord2f(0,1);
				glVertex3f(0,(float)y,0);

			glEnd();
		glEndList();
	glPopMatrix(); 

	return true;
}

void CUItem::Draw ( float x, float y, float z )
{
	if (!Valid())
		return;

	glBindTexture(GL_TEXTURE_2D, m_iTextureID);

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glLoadIdentity();

	glPushMatrix();
		glTranslatef(x,y,-z);
		glColor4f(1,1,1,1);
		glCallList(m_iList);
	glPopMatrix();
}

// font class
CUIFont::CUIFont()
{
	m_iTextureID = NULL;

	memset(m_aListIDs,0,sizeof(UINT)*128);
}

CUIFont::~CUIFont()
{
	for (int i =0 ; i<128;i++)
	{
		if (m_aListIDs[i]>0)
			glDeleteLists(m_aListIDs[i],1);
	}
}

void CUIFont::InitLists ( void )
{
	UINT	CharPos;
	float	TexStartH,TextStartV;
	UINT	TexRow,TexCol;
	float	fTextureInc = 1.0f/16.0f;
	float	fTextureVStep = 1.0f/8.0f;

	glBindTexture(GL_TEXTURE_2D, m_iTextureID);

	for (int i = 33; i < 126; i ++)
	{
		m_aListIDs[i] = glGenLists(1);

		glLoadIdentity();
		glNewList(m_aListIDs[i],GL_COMPILE);

			CharPos = i-33;

			TexCol = CharPos/16;
			TexRow = CharPos - (TexCol*16);

			TexStartH = TexRow*fTextureInc;
			TextStartV = (TexCol*fTextureVStep)+fTextureVStep;

			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);

			glColor4f(1,1,1,1);
			glBegin(GL_QUADS);
			
				glTexCoord2f(TexStartH,TextStartV);
				glVertex3f(0,0,0);
				
				glTexCoord2f(TexStartH+fTextureInc,TextStartV);
				glVertex3f((float)m_iCharSize,0,0);
				
				glTexCoord2f(TexStartH+fTextureInc,TextStartV-fTextureVStep);
				glVertex3f((float)m_iCharSize,(float)m_iCharSize,0);

				glTexCoord2f(TexStartH,TextStartV-fTextureVStep);
				glVertex3f(0,(float)m_iCharSize,0);

			glEnd();
			glTranslatef((float)m_iCharSize,0,0);

		glEndList();
	} 
}

void CUIFont::DrawString (  float x, float y, float z, char* string )
{
	char*	ptr = string;
	if (!string || !Valid())
		return;

	glBindTexture(GL_TEXTURE_2D, m_iTextureID);

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);

	glLoadIdentity();
	glPushMatrix();
	glTranslatef(x,y,-z);
	while (*ptr != NULL)
	{
		if (*ptr == 32)
			glTranslatef((float)m_iCharSize,0,0);
		else
			glCallList(m_aListIDs[*ptr]);

		ptr++;
	}
	glPopMatrix();
}

void CUIFont::DrawStringFlat (  float x, float y, float z, float rot,float tilt, char* string )
{
	char*	ptr = string;
	if (!string || !Valid())
		return;

	UINT	CharPos;
	float	TexStartH,TextStartV;
	UINT	TexRow,TexCol;
	float	fTextureInc = (float)m_iCharSize/(m_iCharSize*16.0f);
	float	fTextureVStep = (float)m_iCharSize/(m_iCharSize*8.0f);

	glBindTexture(GL_TEXTURE_2D, m_iTextureID);

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
	glTranslatef(x,y,z);
	glRotatef(-rot,0,0,1);
	glRotatef(-tilt,1,0,0);
	glTranslatef(-((strlen(string)*(m_iCharSize*0.25f))/2.0f),0,0);
	glScalef(0.25f,0.25f,0.25f);
	while (*ptr != NULL)
	{
		if (*ptr == 32)
			glTranslatef((float)m_iCharSize,0,0);
		else
		{
			CharPos = *ptr-33;

			TexCol = CharPos/16;
			TexRow = CharPos - (TexCol*16);

			TexStartH = TexRow*fTextureInc;
			TextStartV = (TexCol*fTextureVStep)+fTextureVStep;

			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);

			glBegin(GL_QUADS);
			
				glTexCoord2f(TexStartH,TextStartV);
				glVertex3f(0,0,0);
				
				glTexCoord2f(TexStartH+fTextureInc,TextStartV);
				glVertex3f((float)m_iCharSize,0,0);
				
				glTexCoord2f(TexStartH+fTextureInc,TextStartV-fTextureVStep);
				glVertex3f((float)m_iCharSize,0,(float)m_iCharSize);

				glTexCoord2f(TexStartH,TextStartV-fTextureVStep);
				glVertex3f(0,0,(float)m_iCharSize);

			glEnd();
			glTranslatef((float)m_iCharSize,0,0);
		}

		ptr++;
	}
	glPopMatrix();
}

	
