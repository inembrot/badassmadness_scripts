#include "Game.h"

#define BEGIN_TO_CHECKPOINT 33
#define CHECKPOINT_TO_END 33
#define MAX_SPEED 4

/*!
	* \brief Game default constructor.
*/
Game::Game(void)
{
}

/*!
	* \brief Game constructor.
	* \param scene : a pointer to the scene.
	* \param resetTime : the fall time allowed before resetting the game.
*/
Game::Game(SCENE *scene, float resetTime) {
	this->time = 0;
	this->first = 0;
	this->score = 0;
	this->nb_chutes = 0;
	this->nb_letters = 0;
	this->isGame = false;
	this->checkpoint = false;

	this->scene = scene;
	this->resetTime = resetTime;
}

/*!
	* \brief Game default destructor.
*/
Game::~Game(void)
{
}

/*!
	* \brief Checks if a face is not a flat ground.
	*
	* Method checking if a face is not a flat ground : a wall or an inclined face.
	* \return true is the face is not a flat ground,
	* false otherwise.
*/
bool Game::IsNotFlat(FACE *f){
	float ay = scene->pVertices[f->pu32Vertices[0]].fY;
	float by = scene->pVertices[f->pu32Vertices[1]].fY;
	float cy = scene->pVertices[f->pu32Vertices[2]].fY;
	if (abs(scene->pNormals[f->pu32Normals[0]].fX) > 0.2f || (scene->pNormals[f->pu32Normals[0]].fZ) > 0.2f) {
		return true;	
	}
	return false;
}

/*!
	* \brief Stores the character colliders in the queue.
	* \param c : the caracter we want the colliders of.
*/
void Game::Colliding(Character c){
	//Gets the character position
	Vecteur4D point = c.getPosition();
	//Empty the colliders queue
	for (int i = 0; i < collidingFaces.size(); i++) {
		collidingFaces.pop();
	}
	//Computes the sphere radius
	float r2 = pow(c.getRadius(), 2);
	if (scene) {
		//Browsing all faces for a collision with our character
		for(int i = 0; i < scene->u32FacesCount; i++) {
			FACE* f = &scene->pFaces[i];
			VERTEX* A = &scene->pVertices[f->pu32Vertices[0]];
			VERTEX* B = &scene->pVertices[f->pu32Vertices[1]];
			VERTEX* C = &scene->pVertices[f->pu32Vertices[2]];
			Vecteur4D VA = Vecteur4D(A->fX,A->fY,A->fZ,0) - point;
			Vecteur4D VB = Vecteur4D(B->fX,B->fY,B->fZ,0) - point;
			Vecteur4D VC = Vecteur4D(C->fX,C->fY,C->fZ,0) - point;
			Vecteur4D V = (VB-VA).produitVectoriel(VC-VA);
			float d = VA.produitScalaire(V);
			float e = V.produitScalaire(V);
			bool sep1 = d*d > r2*e;
			float aa = VA.produitScalaire(VA);
			float ab = VA.produitScalaire(VB);
			float ac = VA.produitScalaire(VC);
			float bb = VB.produitScalaire(VB);
			float bc = VB.produitScalaire(VC);
			float cc = VC.produitScalaire(VC);
			bool sep2 = (aa > r2) & (ab > aa) & (ac > aa);
			bool sep3 = (bb > r2) & (ab > bb) & (bc > bb);
			bool sep4 = (cc > r2) & (ac > cc) & (bc > cc);
			Vecteur4D AB = VB - VA;
			Vecteur4D BC = VC - VB;
			Vecteur4D CA = VA - VC;
			float d1 = ab-aa;
			float d2 = bc-bb;
			float d3 = ac-cc;
			float e1 = AB.produitScalaire(AB);
			float e2 = BC.produitScalaire(BC);
			float e3 = CA.produitScalaire(CA);
			Vecteur4D Q1 = (VA*e1) - (AB*d1);
			Vecteur4D Q2 = (VB*e2) - (BC*d2);
			Vecteur4D Q3 = (VC*e3) - (CA*d3);
			Vecteur4D QC = (VC*e1) - Q1;
			Vecteur4D QA = (VA*e2) - Q2;
			Vecteur4D QB = (VB*e3) - Q3;
			bool sep5 = (Q1.produitScalaire(Q1) > r2*e1*e1) && (Q1.produitScalaire(QC) > 0);
			bool sep6 =	(Q2.produitScalaire(Q2) > r2*e2*e2) && (Q2.produitScalaire(QA) > 0);
			bool sep7 =	(Q3.produitScalaire(Q3) > r2*e3*e3) && (Q3.produitScalaire(QB) > 0);
			bool separated = sep1 || sep2 || sep3 || sep4 || sep5 || sep6 || sep7;
			//Adds a face to the character colliders if there's no separating axis.
			if (!separated) {
				collidingFaces.push(f);
			}
		}
	}
}

/*!
	* \brief updates the Game.
	*
	* Method that updates the game : applies collisions forces to the character,
	* plays matching sounds, and checks if the checkpoint or win place have been reached.
	* \param fDT : the time ellapsed since the last update.
*/
void Game::Update(float fDT) {
	if (this->isGame) {
		//Updating game time
		this->time += fDT;
		//soundManager.SetParameterValue("MUSIC",time/100);
		//Updating ball colliders.
		Colliding(ball);
		//Getting the speed of the ball in order to keep it and apply the colliders forces.
		Vecteur4D orSpeed = ball.getSpeed();
		FACE* collidingFace;
		//Makes the ball fall if there's no faces colliding it
		if (collidingFaces.empty()) {
			ball.fall(fDT);
			soundManager.Stop("MOVE");
		} else {
			if((abs(orSpeed[0])>=0.1 || abs(orSpeed[2])>=0.1)){
			soundManager.Play("MOVE");
			if(abs(orSpeed[0]) > abs(orSpeed[2])){
				//cout << abs(orSpeed[0])/MAX_SPEED << endl;
				soundManager.SetParameterValue("MOVE",abs(orSpeed[0])/MAX_SPEED);
			}else
				soundManager.SetParameterValue("MOVE",abs(orSpeed[2])/MAX_SPEED);
		}
			//Otherwise, applies a force on the ball, based on the face collided.
			for (int i = 0; i < collidingFaces.size(); i++) {
				collidingFace = collidingFaces.front();
				//There's a face colliding, the ball is not falling anymore.
				ball.resetFallTime();
				orSpeed[1] = 0;
				ball.setSpeed(orSpeed);
				//If it's not the flat ground, we're applying a force to the ball
				if (IsNotFlat(collidingFace)) {
					float mx = scene->pNormals[collidingFace->pu32Normals[0]].fX;
					float my = scene->pNormals[collidingFace->pu32Normals[0]].fY;
					float mz = scene->pNormals[collidingFace->pu32Normals[0]].fZ;
					orSpeed[0] = mx;
					orSpeed[2] = mz;
					//If it's only a wall we're reducing the force value.
					if (my <= 0.1f) {
						orSpeed[0] *= 0.15;
						orSpeed[2] *= 0.15;
						soundManager.Stop("HITWALL");
						soundManager.Play("HITWALL");
					//Otherwise it's an inclined face
					} else {
						ball.setIsOnFlatFace(false);
						soundManager.Play("SLIP");
					}
					//Applying the new speed to the ball
					ball.setSpeed(orSpeed);
					ball.move(mx*0.1, 0, 0.1*mz);
				} else {
					ball.setIsOnFlatFace(true);
					soundManager.Stop("SLIP");
				}
				collidingFaces.pop();
			}
		}
		//Calls for the ball update, so that it can move
		ball.Update();

		//If the ball fall time is over the reset time, we're resetting the game and set that the player has failed.
		if (ball.getFallTime() >= resetTime) {
			//soundManager.Stop("MUSIC");
			ball.reset(this->checkpoint);
			//soundManager.Play("MUSIC);
			printf(" You just die...\n");
			this->nb_chutes++;
			this->score -= 100;
		}
		if (ball.isFalling())
			soundManager.Play("FALL");
		else {
			soundManager.Stop("FALL");
		}
		
		//Checking if the checkpoint has been reached.
		Vecteur4D Position = ball.getPosition();
		if (Position.getX() <= 8 && Position.getX() > 0 && Position.getY() <= 5.8 && Position.getZ() <= 35.0) {;
			if (this->first == 1) {
				printf(" CHECKPOINT reached in %f seconds ! \n", this->time);
				this->checkpoint = true;
				soundManager.Play("CHECKPOINT");

				this->first++;
				this->score += (int) (10000 * 1.0/this->time);
			}
		}
		//Checking if the win checkpoint has been reached.
		if (Position.getX() <= -2.5 && Position.getY() <= -4 && Position.getZ() <= 66.0 && Position.getZ() > 35.0) {
			this->isGame = false;
			this->score += (int) (1000000 * 1.0/this->time);
			printf(" YOU WON !!! Score : %d in %f seconds \n", this->score, this->time);
			soundManager.Stop("MUSIC");
			soundManager.Play("CREDITS");
		}

		if(!checkpoint){
			//Computes the distance between the ball position and the checkpoint
			float distToCheckpoint = sqrt((Position.getX()-8)*(Position.getX()-8) + 
										   (Position.getY()-5.8)*(Position.getY()-5.8) +
										    (Position.getZ()-35.0)*(Position.getZ()-35.0));
			soundManager.SetParameterValue("MUSIC",((distToCheckpoint/BEGIN_TO_CHECKPOINT)+0.5));
		}else{
			float distToEnd = sqrt((Position.getX()+2.5)*(Position.getX()+2.5) + 
										   (Position.getY()+4)*(Position.getY()+4) +
										    (Position.getZ()-66.0)*(Position.getZ()-66.0));
			soundManager.SetParameterValue("MUSIC",((distToEnd/CHECKPOINT_TO_END)-0.5));
		}
	}
}

/*!
	* \brief Game keyboard input manager.
	*
	* Method that will handle the keyboard inputs.
	* \param s32VirtualKey : the key pressed.
*/
void Game::KeyDown(int s32VirtualKey) {
	//We're getting the ball current, speed and position
	Vecteur4D speed = ball.getSpeed();
	Vecteur4D Position = ball.getPosition();

	switch (s32VirtualKey) {
		case VK_UP :
			//Allows the player to move the ball only if the game is not paused
			if (this->isGame) {
				//Applying the new speed to the current one
				speed[0] +=0.1;
				ball.setSpeed(speed);
				//Playing brake sound, and turn sound only if the ball is not on an inclined face.
				if(ball.isOnFlatFace() && !ball.isFalling()){
					if(speed[0] < 0)
						soundManager.Play("BRAKE");
					//if(speed[2] < 0 || speed[2] > 0)
						//soundManager.Play("TURN");
				}	
			}
		break;

		case VK_DOWN :
			if (this->isGame) {
				speed[0] += -0.1;
				ball.setSpeed(speed);
				if(ball.isOnFlatFace()){
					if(speed[0] > 0 && !ball.isFalling())
						soundManager.Play("BRAKE");
					//if(speed[2] < 0 || speed[2] > 0)
						//soundManager.Play("TURN");
				}
			}
		break;

		case VK_LEFT :
			if (this->isGame) {
				speed[2] += -0.1;
				ball.setSpeed(speed);
				if(ball.isOnFlatFace() && !ball.isFalling()){
					if(speed[2] > 0)
						soundManager.Play("BRAKE");
					//if(speed[0] < 0 || speed[0] > 0)
						//soundManager.Play("TURN");
				}
			}
		break;
		
		case VK_RIGHT :
			if (this->isGame) {
				speed[2] += 0.1;
				ball.setSpeed(speed);
				if (ball.isOnFlatFace() && !ball.isFalling()) {
					if(speed[2] < 0)
						soundManager.Play("BRAKE");
					//if(speed[0] < 0 || speed[0] > 0)
						//soundManager.Play("TURN");
				}
			}
		break;

		//Pauses the game
		case VK_SPACE:
			if (this->first++ == 0){
				printf(" Game STARTED !\n");
				soundManager.Play("MUSIC");
			}
			else {
				if (this->isGame){
					printf(" Game PAUSED...\n");
					soundManager.Pause("MUSIC");
					soundManager.Play("PAUSE");
				}
				else{
					printf(" Game ON. Let's roll !\n");
					soundManager.Stop("PAUSE");
					soundManager.Resume("MUSIC");
				}
			}
			this->isGame = (this->isGame) ? false : true;
		break;
		
		//Displays the player's current score and time
		case VK_NUMPAD5 : 
			printf("%d in %f seconds \n", this->score, this->time);
		break;
		
		//Displays the ball current position
		case VK_NUMPAD8 : 
			printf("x:%f y:%f z:%f\n", Position.getX(), Position.getY(), Position.getZ());
		break;

		//Displays the credits
		case VK_NUMPAD0 : 
//*
			printf("\n\n");
			printf("---------------------------------------------------------\n"); 
			printf("------------------------------------------------- Credits\n");
			printf("---------------------------------------------------------\n"); 
			printf("\n~~~~~~~~~~ Direction et Realisation\n");
			printf("\tFranck BENARD\n");
			printf("\tIsabelle NEMBROT\n");
			printf("\n~~~~~~~~~~ Equipe musicale\n");
			printf("\tFranck BENARD\n");
			printf("\tIsabelle NEMBROT\n");
			printf("\tThibault CATTELIN\n");
			printf("\n\tAvec la participation de \n\t\tNajat A., Camille P. et Jahnai M.");
			printf("\n\n");
			printf("---------------------------------------------------------\n"); 
			printf("---------------------------------------------------------\n"); 
			printf("\n\n");
//*/
		break;

		//Simulates the sound of a letter
		case VK_DELETE:
			this->nb_letters++;
			float parameter = ((float)this->nb_letters*(1+0.6))/10.0;	//6 is the number max of letters

			printf(" Letter collected ! [%f] \n", parameter);
			soundManager.Play("GEMS");
			soundManager.SetParameterValue("GEMS", parameter);

			//Reinit the number of letters collected (can't be more than 6 for FFVIII)
			this->nb_letters = this->nb_letters >= 6 ? 0 : this->nb_letters;
		break;

	}
}

/*!
	* \brief Ball accessor.
	* \return a pointer to the ball of the game.
*/
Character* Game::getBall(){
	return &ball;
}

/*!
	* \brief Ball accessor.
	* \return a bool if the game is paused
*/
bool Game::isPaused(){
	return isGame;
}