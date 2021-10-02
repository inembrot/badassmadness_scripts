#ifndef __GAME_H
#define __GAME_H

#pragma once
#include "Character.h"
#include "SoundManager.h"
#include "OBJLoader.h"
#include <queue>

/*! \class Game
 * \brief the game class.
 *
 * Class that will manage the collisions, game date (score,checkpoints..) and interact with the SoundManager.
*/
class Game
{
private:
	SCENE *scene;				/*!< a pointer to the scene, in order to get the property of faces.*/
	Character ball;				/*!< the Character of the game.*/
	SoundManager soundManager;	/*!< the SoundManager of the game.*/
	int first;					/*!< a property checker that checks if the game has already began.*/
	int score;					/*!< the player's score.*/
	float time;					/*!< the ellapsed time since the game began.*/
	bool isGame;				/*!< property that defines if the game is paused.*/
	int nb_chutes;				/*!< the number of player's fall.*/
	int nb_letters;				/*!< the number the player has collected.*/
	float resetTime;			/*!< defines how much time the ball falls before resetting the game.*/
	bool checkpoint;			/*!< defines if the checkpoint has been reached.*/
	queue<FACE*> collidingFaces;/*!< the faces that are currently colliding the character.*/

public:
	Game(void);
	Game(SCENE *scene, float resetTime);
	~Game(void);
	void Update(float fDT);
	void Colliding(Character c);
	bool IsNotFlat(FACE *f);
	void KeyDown(int s32VirtualKey);
	bool isPaused();
	Character* getBall();
};

#endif