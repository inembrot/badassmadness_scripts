#include "stdafx.h"
#include "myC3DEngine.h"
#include "OBJLoader.h"


using namespace std;

myC3DEngine m;
int objectCount = 2;
SCENE* pScene[2];
float tx=0,ty=0,tz=0, rx=0, ry=0;

bool isColliding(Character c);
Character sphere;

void myC3DEngine::Setup(HWND hWnd){
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR);
	
	glEnable(GL_TEXTURE_2D);
	
	pScene[0] = ReadOBJFile("D:\\Moteur de Jeux\\Plateforme.obj");
	pScene[1] = ReadOBJFile("D:\\Moteur de Jeux\\Sphere.obj");
	for(int h=0;h<objectCount;h++){
		GLuint u32TexID;
		if(pScene[h]){
			for(int i=0;i<pScene[h]->u32MaterialsCount;i++){
				MATERIAL m = pScene[h]->pMaterials[i];
				if(m.pDiffuse){
					glGenTextures(1,&u32TexID);
					m.pDiffuse->pUserData = (void * )(DWORD_PTR)u32TexID;
					glBindTexture(GL_TEXTURE_2D,u32TexID);
					glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
					switch(m.pDiffuse->PixelFormat){
						case PIXELFORMAT_R8G8B8:
							glTexImage2D(GL_TEXTURE_2D,0,3,m.pDiffuse->u32Width, m.pDiffuse->u32Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m.pDiffuse->pu8Pixels);
						break;
						case PIXELFORMAT_R8G8B8A8:
							glTexImage2D(GL_TEXTURE_2D,0,4,m.pDiffuse->u32Width, m.pDiffuse->u32Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m.pDiffuse->pu8Pixels);
						break;
					
					}
				}
			}
		}
	}
}

void show(SCENE* Scene){
	if(Scene){
		for(int i=0;i<Scene->u32ObjectsCount;i++){
			OBJECT *pO = &Scene->pObjects[i];
				if(pO->u32Material >= 0 && pO->u32Material <Scene->u32MaterialsCount){ 
					MATERIAL* m = &Scene->pMaterials[pO->u32Material];
					if(m->pDiffuse){
						GLuint texture = (GLuint)m->pDiffuse->pUserData;
						glBindTexture(GL_TEXTURE_2D,texture);
					}
				}
				MATERIAL* m = &Scene->pMaterials[pO->u32Material];
							if(m){
								glMaterialfv(GL_FRONT, GL_AMBIENT, m->pfAmbient);
								glMaterialfv(GL_FRONT, GL_SPECULAR, m->pfSpecular);
								glMaterialfv(GL_FRONT, GL_DIFFUSE, m->pfDiffuse);
								glMaterialfv(GL_FRONT, GL_EMISSION, m->pfEmission);
								glMaterialf(GL_FRONT, GL_SHININESS, m->fShininess);
							}
				for(int j=pO->u32FirstFace;j<pO->u32FirstFace+pO->u32FacesCount;j++){
					FACE *f = &Scene->pFaces[j];
					glBegin(GL_TRIANGLES);
						for(int k=0;k<3;k++){
							if(m->pDiffuse){
								glTexCoord2d(Scene->pUV[f->pu32UV[k]].fU,Scene->pUV[f->pu32UV[k]].fV);
							}
							glNormal3f(Scene->pNormals[f->pu32Normals[k]].fX,Scene->pNormals[f->pu32Normals[k]].fY,Scene->pNormals[f->pu32Normals[k]].fZ);
							glVertex3f(Scene->pVertices[f->pu32Vertices[k]].fX,Scene->pVertices[f->pu32Vertices[k]].fY,Scene->pVertices[f->pu32Vertices[k]].fZ);
						}
					glEnd();
			}
		}
	}
}

void myC3DEngine::Render(unsigned int u32Width, unsigned int u32Height){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(tx,ty,tz);
	
	glRotatef(rx,0,1,0);
	glRotatef(ry,1,0,0);
	
	
	show(pScene[0]);
	
	glPushMatrix();
		Vecteur4D charPosition = *sphere.getPosition(); 
		glTranslatef(charPosition[0],charPosition[1],charPosition[2]);
		show(pScene[1]);
	glPopMatrix();


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)u32Width/u32Height, 0.5f, 1000);
	
	

}

void myC3DEngine::KeyDown(int s32VirtualKey){
	Vecteur4D Position = *sphere.getPosition();
	switch (s32VirtualKey) {
		case VK_UP :
			//ty--;
			sphere.move(0.1,0,0);
		break;

		case VK_DOWN :
			//ty++;
			sphere.move(-0.1,0,0);
		break;
		case VK_LEFT :
			
			sphere.move(0,0,-0.1);
			//ty+=0.1;
			//tx++;
		break;
		
		case VK_RIGHT :
			sphere.move(0,0,0.1);
			//ty-=0.1;
			//tx--;
		break;
	}
}

void myC3DEngine::MouseWheel(float fIncrement){
	tz+=fIncrement;
}

void myC3DEngine::MouseMove(POINT Pos){
	rx=Pos.x;
	ry=Pos.y;
}


myC3DEngine::myC3DEngine()
{
}


myC3DEngine::~myC3DEngine()
{
}

void myC3DEngine::Update(float fDT) {
	if(!isColliding(sphere)){
		sphere.move(0,-0.02,0);
	} /*else 
		sphere.move(0,0,0.05);*/
}

bool isColliding(Character c){
	SCENE* Scene = pScene[0];
	Vecteur4D point = *c.getPosition();
	if(Scene){
		for(int i=0;i<Scene->u32FacesCount;i++){
			FACE* f = &Scene->pFaces[i];
			VERTEX* A = &Scene->pVertices[f->pu32Vertices[0]];
			VERTEX* B = &Scene->pVertices[f->pu32Vertices[1]];
			VERTEX* C = &Scene->pVertices[f->pu32Vertices[2]];

			Vecteur4D VA = Vecteur4D(A->fX,A->fY,A->fZ,0);
			Vecteur4D VB = Vecteur4D(B->fX,B->fY,B->fZ,0);
			Vecteur4D VC = Vecteur4D(C->fX,C->fY,C->fZ,0);

			Vecteur4D AB = VB - VA;			Vecteur4D BA = VA - VB;
			Vecteur4D AC = VC - VA;			Vecteur4D CA = VA - VC;
			Vecteur4D BC = VC - VB;			Vecteur4D CB = VB - VC;
			Vecteur4D VN = AB.produitVectoriel(AC);
			Vecteur4D AM = Vecteur4D(0,0,0,0)-VA;
			float d = AM.produitScalaire(VN);
			float dist = abs(VN[0]*point[0]+VN[1]*point[1]+VN[2]*point[2] + d)/VN.norme();
			float r = c.getRadius();
			
			
			if (dist <= r) {
				return true;
			}
		}

	}
	return false;
}