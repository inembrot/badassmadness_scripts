#ifndef __SOUNDMANAGER_H
#define __SOUNDMANAGER_H

#pragma once
#include "fmod.h"
#include "fmod_event.h"
#include "fmod_errors.h"
#include <map>

/*! \class SoundManager
 * \brief class that will manage musics and sounds.
 *
 * Class managing the musics and sounds.
*/
class SoundManager
{
private:
	FMOD_EVENTSYSTEM *eventsystem;	/*!< the sound system that will play the sounds.*/
	FMOD_EVENT_STATE eventState;	/*!< the state of an event.*/
	FMOD_BOOL eventBool;	/*!< an event property checker.*/
	FMOD_RESULT result;		/*!< the result of an FMOD event operation.*/
	FMOD_EVENTPARAMETER *parameter; /*!< parameter of an event.*/
	std::map<char*,FMOD_EVENT*> events; /*!< Events to be played.*/
public:
	SoundManager(void);
	~SoundManager(void);
	void Play(char* e);
	void Pause(char* e);
	void Resume(char* e);
	void Stop(char* e);
	void SetParameterValue(char* e, float value);
	void SetAllParametersValue(float value);
	void ERRCHECK(FMOD_RESULT result);
};
#endif