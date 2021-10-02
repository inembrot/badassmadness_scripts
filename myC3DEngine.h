#ifndef __MYC3DENGINE_H
#define __MYC3DENGINE_H

#pragma once
#include "3dengine.h"
#include "Game.h"

/*! \class myC3DEngine
 * \brief the 3DEngine class.
 *
 * Class that will manage rendering based on the data from the game.
*/

class myC3DEngine :
	public C3DEngine
{
private :
	Game* myGame;
public:
	myC3DEngine();
	void Setup(HWND hWnd);
	void Render(unsigned int u32Width, unsigned int u32Height);
	void KeyDown(int s32VirtualKey);
	void MouseWheel(float fIncrement);
	void MouseMove(POINT Pos);
	void Update(float fDT);
	~myC3DEngine();
};

#endif