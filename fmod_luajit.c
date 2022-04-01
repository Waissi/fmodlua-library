#include "fmod_luajit.h"

/*
---------------------------------             FMOD STUDIO SYSTEM         --------------------------------
*/

bool Init()
{
	FMOD_RESULT result = FMOD_Studio_System_Create(&studioSystem, FMOD_VERSION);
    if(result != FMOD_OK){return false;}

    result = FMOD_Studio_System_Initialize(studioSystem, 128, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL);
    if(result != FMOD_OK){return false;}
	
    return true;
}

bool Update()
{
    FMOD_RESULT result = FMOD_Studio_System_Update(studioSystem);
    if(result != FMOD_OK){return false;}
    return true;
}

bool Release()
{
    FMOD_RESULT result = FMOD_Studio_System_Release(studioSystem);
    if(result != FMOD_OK){return false;}
    return true;
}

/*
--------------------------------------             BANKS         --------------------------------
*/

int LoadBank(const char* path)
{
    struct bankNode *link = (struct bankNode*) malloc(sizeof(struct bankNode));
    FMOD_RESULT result = FMOD_Studio_System_LoadBankFile(studioSystem, path, FMOD_STUDIO_LOAD_BANK_NORMAL, &link->bank);
    if(result != FMOD_OK){return -1;}

    numBanks += 1;
    link->key = numBanks;
    link->next = headBank;
    headBank = link;
    return numBanks;
}

/*
--------------------------------------             BUSES         --------------------------------
*/

int GetBus(const char* path)
{
    struct busNode *link = (struct busNode*) malloc(sizeof(struct busNode));
    FMOD_RESULT result = FMOD_Studio_System_GetBus(studioSystem, path, &link->bus);
    if(result != FMOD_OK){return -1;}

    numBus += 1;
    link->key = numBus;
    link->next = headBus;
    headBus = link;
    return numBus;
}

bool SetBusVolume(int key, float volume)
{
    FMOD_STUDIO_BUS* bus;
    bus = RetrieveBus(key);
    if (!bus){return false;}
    
    FMOD_RESULT result = FMOD_Studio_Bus_SetVolume(bus, volume);
    if(result != FMOD_OK){return false;}
    return true;
}

float GetBusVolume(int key)
{
    FMOD_STUDIO_BUS* bus;
    bus = RetrieveBus(key);
    if (!bus){return -1.0f;}

    float volume;
    float finalVolume;
    FMOD_RESULT result = FMOD_Studio_Bus_GetVolume(bus, &volume, &finalVolume);
    if(result != FMOD_OK){return -1.0f;}
    return finalVolume;
}

bool SetBusMute(int key, FMOD_BOOL mute)
{
    FMOD_STUDIO_BUS* bus;
    bus = RetrieveBus(key);
    if (!bus){return false;}

    FMOD_RESULT result = FMOD_Studio_Bus_SetMute(bus, mute);
    if(result != FMOD_OK){return false;}
    return true;
}

bool GetBusMute(int key)
{
    FMOD_STUDIO_BUS* bus;
    bus = RetrieveBus(key);
    if (!bus){return false;}

    FMOD_BOOL mute;
    FMOD_RESULT result = FMOD_Studio_Bus_GetMute(bus, &mute);
    if(result != FMOD_OK){return false;}
    return mute == 1;
}

/*
-------------------------------------             VCAs         --------------------------------
*/

int GetVCA(const char* path)
{
    struct vcaNode *link = (struct vcaNode*) malloc(sizeof(struct vcaNode));
    FMOD_RESULT result = FMOD_Studio_System_GetVCA(studioSystem, path, &link->vca);
    if(result != FMOD_OK){return -1;}

    numVCA += 1;
    link->key = numVCA;
    link->next = headVCA;
    headVCA = link;
    return numVCA;
}

bool SetVcaVolume(int key, float volume)
{
    FMOD_STUDIO_VCA* vca;
    vca = RetrieveVca(key);
    if (!vca){return false;}

    FMOD_RESULT result = FMOD_Studio_VCA_SetVolume(vca, volume);
    if(result != FMOD_OK){return false;}
    return true;
}

float GetVcaVolume(int key)
{
    FMOD_STUDIO_VCA* vca;
    vca = RetrieveVca(key);
    if (!vca){return -1.0f;}

    float volume;
    float finalVolume;
    FMOD_RESULT result = FMOD_Studio_VCA_GetVolume(vca, &volume, &finalVolume);
    if(result != FMOD_OK){return -1.0f;}
    return finalVolume;
}

/*
---------------------------------------             EVENTS         --------------------------------
*/

int CreateInstance(const char* path)
{
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription;
    FMOD_RESULT result = FMOD_Studio_System_GetEvent(studioSystem, path, &eventDescription);
    if(result != FMOD_OK){return -1;}

    struct eventNode *link = (struct eventNode*) malloc(sizeof(struct eventNode));
    result = FMOD_Studio_EventDescription_CreateInstance(eventDescription, &link->event);
    if(result != FMOD_OK){return -1;}

    numEvents += 1;
    link->key = numEvents;
    link->next = headEvent;
    headEvent = link;
    return numEvents;
}

bool PlayOneShotEvent(const char* path)
{
    FMOD_STUDIO_EVENTINSTANCE* event;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription;
    FMOD_RESULT result = FMOD_Studio_System_GetEvent(studioSystem, path, &eventDescription);
    if (result != FMOD_OK) {return false;}

    result = FMOD_Studio_EventDescription_CreateInstance(eventDescription, &event);
    if(result != FMOD_OK){return false;}

    result = FMOD_Studio_EventInstance_Start(event);
    if(result != FMOD_OK){return false;}

    result = FMOD_Studio_EventInstance_Release(event);
    if(result != FMOD_OK){return false;}
    return true;
}

bool PlayEvent(int key)
{
    FMOD_STUDIO_EVENTINSTANCE* event;
    event = RetrieveEvent(key);
    if (!event){return false;}

    FMOD_RESULT result = FMOD_Studio_EventInstance_Start(event);
    if(result != FMOD_OK){return false;}
    return true;
}

bool StopEvent(int key)
{
    FMOD_STUDIO_EVENTINSTANCE* event;
    event = RetrieveEvent(key);
    if (!event){return false;}

    FMOD_RESULT result = FMOD_Studio_EventInstance_Stop(event, FMOD_STUDIO_STOP_ALLOWFADEOUT);
    if(result != FMOD_OK){return false;}
    return true;
}

bool PauseEvent(int key, FMOD_BOOL paused)
{
    FMOD_STUDIO_EVENTINSTANCE* event;
    event = RetrieveEvent(key);
    if (!event){return false;}

    FMOD_RESULT result = FMOD_Studio_EventInstance_SetPaused(event, paused);
    if(result != FMOD_OK){return false;}
    return true;
}

bool EventIsPlaying(int key)
{
    FMOD_STUDIO_EVENTINSTANCE* event;
    event = RetrieveEvent(key);
    if (!event){return false;}

    FMOD_STUDIO_PLAYBACK_STATE state;
    FMOD_RESULT result = FMOD_Studio_EventInstance_GetPlaybackState(event, &state);
    if(result != FMOD_OK){return false;}

    FMOD_BOOL paused;
    result = FMOD_Studio_EventInstance_GetPaused(event, &paused);
    if(result != FMOD_OK){return false;}
    return state == FMOD_STUDIO_PLAYBACK_PLAYING && paused == 0;
}

/*
-----------------------------------             PARAMETERS         --------------------------------
*/

float GetParamaterByName(int key, const char* name)
{
    FMOD_STUDIO_EVENTINSTANCE* event;
    event = RetrieveEvent(key);
    if (!event){return -1.0f;}

    float value;
    float finalValue;
    FMOD_RESULT result = FMOD_Studio_EventInstance_GetParameterByName(event, name, &value, &finalValue);
    if(result != FMOD_OK){return -1.0f;}
    return finalValue;
}

bool SetParamaterByName(int key, const char* name, float value)
{
    FMOD_STUDIO_EVENTINSTANCE* event;
    event = RetrieveEvent(key);
    if (!event){return false;}

    FMOD_RESULT result = FMOD_Studio_EventInstance_SetParameterByName(event, name, value, 1);
    if(result != FMOD_OK){return false;}
    return true;
}

float GetGlobalParamaterByName(const char* name)
{
    float value;
    float finalValue;
    FMOD_RESULT result = FMOD_Studio_System_GetParameterByName(studioSystem, name, &value, &finalValue);
    if(result != FMOD_OK){return -1.0f;}
    return finalValue;
}

bool SetGlobalParamaterByName(const char* name, float value)
{
    FMOD_RESULT result = FMOD_Studio_System_SetParameterByName(studioSystem, name, value, 1);
    if(result != FMOD_OK){return false;}
    return true;
}