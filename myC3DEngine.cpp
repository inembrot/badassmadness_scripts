#include "stdafx.h"
#include "myC3DEngine.h"
#include "OBJLoader.h"
using namespace std;

myC3DEngine m;
SCENE* pScene[2];

int objectCount = 2;
float tx = 0, ty = 0, tz = 0, rx = 0, ry = 0;
float rs = 0;


void myC3DEngine::Setup(HWND hWnd){
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR);
	glEnable(GL_TEXTURE_2D);
	
	pScene[0] = ReadOBJFile(".\\..\\BadassMadness.obj");
	pScene[1] = ReadOBJFile(".\\..\\Sphere.obj");
	for (int h = 0; h < objectCount; h++) {
		GLuint u32TexID;
		if (pScene[h]) {
			for (int i = 0; i < pScene[h]->u32MaterialsCount; i++) {
				MATERIAL m = pScene[h]->pMaterials[i];
				if (m.pDiffuse) {
					glGenTextures(1, &u32TexID);
					m.pDiffuse->pUserData = (void *)(DWORD_PTR)u32TexID;
					glBindTexture(GL_TEXTURE_2D, u32TexID);
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					switch (m.pDiffuse->PixelFormat) {
						case PIXELFORMAT_R8G8B8:
							glTexImage2D(GL_TEXTURE_2D, 0, 3, m.pDiffuse->u32Width, m.pDiffuse->u32Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m.pDiffuse->pu8Pixels);
						break;
						case PIXELFORMAT_R8G8B8A8:
							glTexImage2D(GL_TEXTURE_2D,0,4,m.pDiffuse->u32Width, m.pDiffuse->u32Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m.pDiffuse->pu8Pixels);
						break;
					
					}
				}
			}
		}
	}
	myGame = new Game(pScene[0], 3.f);
	//Setting the camera view
	rx = 780;
	ry = 400;
	tz = -30;
}

void show(SCENE* Scene) {
	if (Scene) {
		for (int i = 0; i < Scene->u32ObjectsCount; i++) {
			OBJECT *pO = &Scene->pObjects[i];
				if (pO->u32Material >= 0 && pO->u32Material < Scene->u32MaterialsCount) { 
					MATERIAL* m = &Scene->pMaterials[pO->u32Material];
					if (m->pDiffuse) {
						GLuint texture = (GLuint)m->pDiffuse->pUserData;
						glBindTexture(GL_TEXTURE_2D, texture);
					}
				}
				MATERIAL* m = &Scene->pMaterials[pO->u32Material];
							if (m) {
								glMaterialfv(GL_FRONT, GL_AMBIENT, m->pfAmbient);
								glMaterialfv(GL_FRONT, GL_SPECULAR, m->pfSpecular);
								glMaterialfv(GL_FRONT, GL_DIFFUSE, m->pfDiffuse);
								glMaterialfv(GL_FRONT, GL_EMISSION, m->pfEmission);
								glMaterialf(GL_FRONT, GL_SHININESS, m->fShininess);
							}
				for(int j = pO->u32FirstFace; j < pO->u32FirstFace+pO->u32FacesCount; j++) {
					FACE *f = &Scene->pFaces[j];
					glBegin(GL_TRIANGLES);
						for (int k = 0; k < 3; k++) {
							if (m->pDiffuse) {
								glTexCoord2d(Scene->pUV[f->pu32UV[k]].fU, Scene->pUV[f->pu32UV[k]].fV);
							}
							glNormal3f(Scene->pNormals[f->pu32Normals[k]].fX, Scene->pNormals[f->pu32Normals[k]].fY, Scene->pNormals[f->pu32Normals[k]].fZ);
							glVertex3f(Scene->pVertices[f->pu32Vertices[k]].fX, Scene->pVertices[f->pu32Vertices[k]].fY, Scene->pVertices[f->pu32Vertices[k]].fZ);
						}
					glEnd();
			}
		}
	}
}

void myC3DEngine::Render(unsigned int u32Width, unsigned int u32Height) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(!myGame->isPaused())
		glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	else
		glClearColor(0.6f, 0.68f, 0.77f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(tx, ty, tz);
	glRotatef(rx, 0, 1, 0);
	glRotatef(ry, 1, 0, 0);
	
	//Getting the character position
	Vecteur4D pos = myGame->getBall()->getPosition();
	
	//Translates the scene so the ball is still in the center.
	glPushMatrix();
		glTranslatef(-pos[0], -pos[1], -pos[2]);
		show(pScene[0]);
	glPopMatrix();
	
	//Makes the ball rolling
	glPushMatrix();
		glRotatef(myGame->getBall()->getRotationX(),1,0,0);
		glRotatef(myGame->getBall()->getRotationZ(),0,0,1);
		show(pScene[1]);
	glPopMatrix();


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)u32Width/u32Height, 0.5f, 1000);
	
	

}

void myC3DEngine::KeyDown(int s32VirtualKey) {
	myGame->KeyDown(s32VirtualKey);
}

void myC3DEngine::MouseWheel(float fIncrement) {
	tz += fIncrement;
}

void myC3DEngine::MouseMove(POINT Pos) {
	rx = Pos.x;
	ry = Pos.y;
}


myC3DEngine::myC3DEngine()
{
}


myC3DEngine::~myC3DEngine()
{
}

void myC3DEngine::Update(float fDT) {
	myGame->Update(fDT);
}
