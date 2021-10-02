#include "SoundManager.h"
#include <iostream>
#include <Windows.h>
using namespace std;

/*!
	* \brief the SoundManager default constructor.
	*
	* The constructor will ask for the audio driver and load the sounds from the fev file.
*/
SoundManager::SoundManager(void)
{

	FMOD_SYSTEM    *systeme;
    int            cpt, numdrivers, driver;

    ERRCHECK(result = FMOD_EventSystem_Create(&eventsystem));
    ERRCHECK(result = FMOD_EventSystem_GetSystemObject(eventsystem,&systeme));
    ERRCHECK(result =  FMOD_System_GetNumDrivers(systeme, &numdrivers));
	AllocConsole(); 
	freopen("CONIN$", "rb", stdin);   // reopen stdin handle as console window input
	freopen("CONOUT$","wb", stdout);  // reopen stout handle as console window output
	freopen("CONOUT$", "wb", stderr); // reopen stderr handle as console window output	
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");    
	printf("~~~~~~~~~~~~~~~~ BADASS MADNESS by F & Z ~~~~~~~~~~~~~~~~\n");  
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");      
	printf("\n Dirigez BubbleBall vers la sortie avec les fleches directionnelles.");
	printf("\n Reprenez votre souffle avec la touche ESPACE.");
	printf("\n\n");
    printf("---------------------------------------------------------\n"); 
    printf("-------------------------------- Choisir une sortie audio\n");
    printf("---------------------------------------------------------\n"); 
	for (cpt = 0; cpt < numdrivers; cpt++){
        char nom[256];
        ERRCHECK(result = FMOD_System_GetDriverInfo(systeme, cpt, nom, 256, 0));
        printf("%d : %s\n", cpt + 1, nom);
    }

	printf("---------------------------------------------------------\n");
    printf("Entrer le numero de la sortie audio : \n");
    scanf("%d",&driver);
    driver--;
	//driver = 0;
	ERRCHECK(result = FMOD_System_SetDriver(systeme, driver));
    ERRCHECK(result = FMOD_EventSystem_Init(eventsystem,64, FMOD_INIT_NORMAL, 0, FMOD_EVENT_INIT_NORMAL));
    ERRCHECK(result = FMOD_EventSystem_SetMediaPath(eventsystem,".\\media\\"));
    ERRCHECK(result = FMOD_EventSystem_Load(eventsystem,"MarbleMadness.fev", 0, 0));
	ERRCHECK(result = FMOD_EventSystem_GetEvent(eventsystem,"MarbleMadness/MarbleMadness/Music/MainTheme", FMOD_EVENT_DEFAULT, &events["MUSIC"]));
	ERRCHECK(result = FMOD_EventSystem_GetEvent(eventsystem,"MarbleMadness/MarbleMadness/Music/Credits", FMOD_EVENT_DEFAULT, &events["CREDITS"]));
	ERRCHECK(result = FMOD_EventSystem_GetEvent(eventsystem,"MarbleMadness/MarbleMadness/Music/Pause", FMOD_EVENT_DEFAULT, &events["PAUSE"]));
	ERRCHECK(result = FMOD_EventSystem_GetEvent(eventsystem,"MarbleMadness/MarbleMadness/Sounds/Falling", FMOD_EVENT_DEFAULT, &events["FALL"]));
	ERRCHECK(result = FMOD_EventSystem_GetEvent(eventsystem,"MarbleMadness/MarbleMadness/Sounds/Slip", FMOD_EVENT_DEFAULT, &events["SLIP"]));
	ERRCHECK(result = FMOD_EventSystem_GetEvent(eventsystem,"MarbleMadness/MarbleMadness/Sounds/Brake", FMOD_EVENT_DEFAULT, &events["BRAKE"]));
	ERRCHECK(result = FMOD_EventSystem_GetEvent(eventsystem,"MarbleMadness/MarbleMadness/Sounds/HitWall", FMOD_EVENT_DEFAULT, &events["HITWALL"]));
	ERRCHECK(result = FMOD_EventSystem_GetEvent(eventsystem,"MarbleMadness/MarbleMadness/Sounds/Move", FMOD_EVENT_DEFAULT, &events["MOVE"]));
	ERRCHECK(result = FMOD_EventSystem_GetEvent(eventsystem,"MarbleMadness/MarbleMadness/Sounds/Gems", FMOD_EVENT_DEFAULT, &events["GEMS"]));
	ERRCHECK(result = FMOD_EventSystem_GetEvent(eventsystem,"MarbleMadness/MarbleMadness/Sounds/Checkpoint", FMOD_EVENT_DEFAULT, &events["CHECKPOINT"]));
	//SetAllParametersValue(0.7f);
	//SetParameterValue("MUSIC",0.6f);
	//SetParameterValue("FALL",1.f);
	//FreeConsole();
}

/*!
	* \brief The SoundManager destructor
*/
SoundManager::~SoundManager(void)
{
}

/*!
	* \brief Plays a sound.
	*
	* Method that will play a sound according the the parameter.
	* \param e : the event that must be played.
*/
void SoundManager::Play(char* e){
	ERRCHECK(result = FMOD_Event_Start(events[e]));
	
}

/*!
	* \brief Stops a sound.
	*
	* Method that will stop a sound according the the parameter.
	* \param e : the event that must be stopped.
*/
void SoundManager::Stop(char* e){
	ERRCHECK(result = FMOD_Event_Stop(events[e],1));
}

/*!
	* \brief Pauses a sound.
	*
	* Method that will pause a sound according the the parameter.
	* \param e : the event that must be paused.
*/
void SoundManager::Pause(char* e){
	ERRCHECK(result = FMOD_Event_GetState(events[e],&eventState));
	if(eventState && FMOD_EVENT_STATE_PLAYING){
		ERRCHECK(result = FMOD_Event_GetPaused(events[e],&eventBool));
		if(!eventBool)
			ERRCHECK(result = FMOD_Event_SetPaused(events[e],1));
	}
}

/*!
	* \brief Resumes a sound.
	*
	* Method that will resume a sound according the the parameter.
	* \param e : the event that must be resumed.
*/
void SoundManager::Resume(char* e){
	//Resumes only if the sound has been paused.
	ERRCHECK(result = FMOD_Event_GetPaused(events[e],&eventBool));
	if(eventBool)
		ERRCHECK(result = FMOD_Event_SetPaused(events[e],0));
}

/*!
	* \brief Checks for error.
	*
	* Method that will check if an error occured when using FMOD.
	* \param result : a pointer to a result variable.
*/
void SoundManager::ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

/*!
	* \brief Change an event parameter value.
	*
	* Method that change the value of an event parameter.
	* \param e : the event to change the parameter of.
	* \parem value : the new parameter value.
*/
void SoundManager::SetParameterValue(char* e, float value)
{
	ERRCHECK(result = FMOD_Event_GetParameterByIndex(events[e],0,&parameter));
	ERRCHECK(result = FMOD_EventParameter_SetValue(parameter,value));
}

/*!
	* \brief Change all events parameter value.
	*
	* Method that change the value of all events parameter.
	* \parem value : the new parameter value.
*/
void SoundManager::SetAllParametersValue(float value)
{
	std::map<char*,FMOD_EVENT*>::iterator it;
	for(it= events.begin();it!=events.end();++it){
		SetParameterValue(it->first,value);
	}
}