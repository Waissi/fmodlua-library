#include <stdlib.h>
#include "fmod_studio.h"

static FMOD_STUDIO_SYSTEM *studioSystem;

struct eventCallback
{
    FMOD_STUDIO_EVENTINSTANCE *event;
    void (*callback)();
    struct eventCallback *next;
};
struct eventCallback *headCallback = NULL;

struct eventCallback *RetrieveCallback(FMOD_STUDIO_EVENTINSTANCE *event)
{
    if (headCallback == NULL)
    {
        return NULL;
    }
    struct eventCallback *current = headCallback;
    while (current->event != event)
    {
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            current = current->next;
        }
    }
    return current;
}

static void ReleaseCallbacks()
{
    while (headCallback)
    {
        struct eventCallback *current = headCallback;
        headCallback = current->next;
        free(current);
    }
}

/*
---------------------------------             FMOD STUDIO SYSTEM         --------------------------------
*/

FMOD_BOOL InitFmod()
{
    FMOD_RESULT result = FMOD_Studio_System_Create(&studioSystem, FMOD_VERSION);
    if (result != FMOD_OK)
    {
        return 0;
    }

    result = FMOD_Studio_System_Initialize(studioSystem, 128, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL);
    return result == FMOD_OK;
}

FMOD_BOOL UpdateFmod()
{
    FMOD_RESULT result = FMOD_Studio_System_Update(studioSystem);
    return result == FMOD_OK;
}

FMOD_BOOL ReleaseFmod()
{
    ReleaseCallbacks();
    FMOD_RESULT result = FMOD_Studio_System_Release(studioSystem);
    return result == FMOD_OK;
}

/*
--------------------------------------             BANKS         --------------------------------
*/

FMOD_STUDIO_BANK *LoadBank(const char *path)
{
    FMOD_STUDIO_BANK *bank = NULL;
    FMOD_RESULT result = FMOD_Studio_System_LoadBankFile(studioSystem, path, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
    if (result != FMOD_OK)
    {
        return NULL;
    }
    return bank;
}

FMOD_BOOL UnloadBank(FMOD_STUDIO_BANK *bank)
{
    FMOD_RESULT result = FMOD_Studio_Bank_Unload(bank);
    return result == FMOD_OK;
}

/*
--------------------------------------             BUSES         --------------------------------
*/

FMOD_STUDIO_BUS *GetBus(const char *path)
{
    FMOD_STUDIO_BUS *bus = NULL;
    FMOD_RESULT result = FMOD_Studio_System_GetBus(studioSystem, path, &bus);
    if (result != FMOD_OK)
    {
        return NULL;
    }
    return bus;
}

FMOD_BOOL SetBusVolume(FMOD_STUDIO_BUS *bus, float volume)
{
    FMOD_RESULT result = FMOD_Studio_Bus_SetVolume(bus, volume);
    return result == FMOD_OK;
}

float GetBusVolume(FMOD_STUDIO_BUS *bus)
{
    float volume;
    float finalVolume;
    FMOD_RESULT result = FMOD_Studio_Bus_GetVolume(bus, &volume, &finalVolume);
    if (result != FMOD_OK)
    {
        return -1.0f;
    }
    return finalVolume;
}

FMOD_BOOL MuteBus(FMOD_STUDIO_BUS *bus, FMOD_BOOL mute)
{
    FMOD_RESULT result = FMOD_Studio_Bus_SetMute(bus, mute);
    return result == FMOD_OK;
}

FMOD_BOOL IsBusMuted(FMOD_STUDIO_BUS *bus)
{
    FMOD_BOOL mute;
    FMOD_RESULT result = FMOD_Studio_Bus_GetMute(bus, &mute);
    return mute;
}

/*
-------------------------------------             VCAs         --------------------------------
*/

FMOD_STUDIO_VCA *GetVCA(const char *path)
{
    FMOD_STUDIO_VCA *vca = NULL;
    FMOD_RESULT result = FMOD_Studio_System_GetVCA(studioSystem, path, &vca);
    if (result != FMOD_OK)
    {
        return NULL;
    }
    return vca;
}

FMOD_BOOL SetVcaVolume(FMOD_STUDIO_VCA *vca, float volume)
{
    FMOD_RESULT result = FMOD_Studio_VCA_SetVolume(vca, volume);
    return result == FMOD_OK;
}

float GetVcaVolume(FMOD_STUDIO_VCA *vca)
{
    float volume;
    float finalVolume;
    FMOD_RESULT result = FMOD_Studio_VCA_GetVolume(vca, &volume, &finalVolume);
    if (result != FMOD_OK)
    {
        return -1.0f;
    }
    return finalVolume;
}

/*
---------------------------------------             EVENTS         --------------------------------
*/

FMOD_STUDIO_EVENTINSTANCE *GetEvent(const char *path)
{
    FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = NULL;
    FMOD_RESULT result = FMOD_Studio_System_GetEvent(studioSystem, path, &eventDescription);
    if (result != FMOD_OK)
    {
        return NULL;
    }

    FMOD_STUDIO_EVENTINSTANCE *event = NULL;
    result = FMOD_Studio_EventDescription_CreateInstance(eventDescription, &event);
    if (result != FMOD_OK)
    {
        return NULL;
    }
    return event;
}

FMOD_BOOL PlayOneShotEvent(const char *path)
{
    FMOD_STUDIO_EVENTINSTANCE *event = NULL;
    FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = NULL;
    FMOD_RESULT result = FMOD_Studio_System_GetEvent(studioSystem, path, &eventDescription);
    if (result != FMOD_OK)
    {
        return 0;
    }

    result = FMOD_Studio_EventDescription_CreateInstance(eventDescription, &event);
    if (result != FMOD_OK)
    {
        return 0;
    }

    result = FMOD_Studio_EventInstance_Start(event);
    if (result != FMOD_OK)
    {
        return 0;
    }

    result = FMOD_Studio_EventInstance_Release(event);
    return result == FMOD_OK;
}

FMOD_BOOL PlayEvent(FMOD_STUDIO_EVENTINSTANCE *event)
{
    FMOD_RESULT result = FMOD_Studio_EventInstance_Start(event);
    return result == FMOD_OK;
}

FMOD_BOOL StopEvent(FMOD_STUDIO_EVENTINSTANCE *event)
{
    FMOD_RESULT result = FMOD_Studio_EventInstance_Stop(event, FMOD_STUDIO_STOP_ALLOWFADEOUT);
    return result == FMOD_OK;
}

FMOD_BOOL PauseEvent(FMOD_STUDIO_EVENTINSTANCE *event, FMOD_BOOL paused)
{
    FMOD_RESULT result = FMOD_Studio_EventInstance_SetPaused(event, paused);
    return result == FMOD_OK;
}

FMOD_BOOL IsEventPlaying(FMOD_STUDIO_EVENTINSTANCE *event)
{
    FMOD_STUDIO_PLAYBACK_STATE state;
    FMOD_RESULT result = FMOD_Studio_EventInstance_GetPlaybackState(event, &state);
    if (result != FMOD_OK)
    {
        return 0;
    }

    FMOD_BOOL paused;
    result = FMOD_Studio_EventInstance_GetPaused(event, &paused);
    if (result != FMOD_OK)
    {
        return 0;
    }
    return state == FMOD_STUDIO_PLAYBACK_PLAYING && paused == 0;
}

FMOD_RESULT F_CALLBACK EventMarkerCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES *parameters)
{
    struct eventCallback *eventCallback = RetrieveCallback(event);
    if (!eventCallback)
    {
        return 0;
    }
    eventCallback->callback(parameters->name);
    return FMOD_OK;
};

FMOD_BOOL SetEventMarkerCallback(FMOD_STUDIO_EVENTINSTANCE *event, void (*callback)(const char *name))
{
    struct eventCallback *eventCallback = RetrieveCallback(event);
    if (eventCallback)
    {
        return 0;
    }

    struct eventCallback *link = (struct eventCallback *)malloc(sizeof(struct eventCallback));
    link->event = event;
    link->callback = callback;
    link->next = headCallback;
    headCallback = link;

    FMOD_RESULT result = FMOD_Studio_EventInstance_SetCallback(event, (FMOD_STUDIO_EVENT_CALLBACK)EventMarkerCallback, FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER);
    return result == FMOD_OK;
}

FMOD_RESULT F_CALLBACK EventBeatCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES *parameters)
{
    struct eventCallback *eventCallback = RetrieveCallback(event);
    if (!eventCallback)
    {
        return 0;
    }
    eventCallback->callback(parameters->bar, parameters->beat);
    return FMOD_OK;
};

FMOD_BOOL SetEventBeatCallback(FMOD_STUDIO_EVENTINSTANCE *event, void (*callback)(int bar, int beat))
{
    struct eventCallback *eventCallback = RetrieveCallback(event);
    if (eventCallback)
    {
        return 0;
    }

    struct eventCallback *link = (struct eventCallback *)malloc(sizeof(struct eventCallback));
    link->event = event;
    link->callback = callback;
    link->next = headCallback;
    headCallback = link;

    FMOD_RESULT result = FMOD_Studio_EventInstance_SetCallback(event, (FMOD_STUDIO_EVENT_CALLBACK)EventBeatCallback, FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT);
    return result == FMOD_OK;
}

/*
-----------------------------------             PARAMETERS         --------------------------------
*/

float GetParamaterByName(FMOD_STUDIO_EVENTINSTANCE *event, const char *name)
{
    if (!event)
    {
        return -1.0f;
    }
    float value;
    float finalValue;
    FMOD_RESULT result = FMOD_Studio_EventInstance_GetParameterByName(event, name, &value, &finalValue);
    if (result != FMOD_OK)
    {
        return -1.0f;
    }
    return finalValue;
}

FMOD_BOOL SetParamaterByName(FMOD_STUDIO_EVENTINSTANCE *event, const char *name, float value)
{
    if (!event)
    {
        return -1.0f;
    }
    FMOD_RESULT result = FMOD_Studio_EventInstance_SetParameterByName(event, name, value, 1);
    return result == FMOD_OK;
}

float GetGlobalParamaterByName(const char *name)
{
    float value;
    float finalValue;
    FMOD_RESULT result = FMOD_Studio_System_GetParameterByName(studioSystem, name, &value, &finalValue);
    if (result != FMOD_OK)
    {
        return -1.0f;
    }
    return finalValue;
}

FMOD_BOOL SetGlobalParamaterByName(const char *name, float value)
{
    FMOD_RESULT result = FMOD_Studio_System_SetParameterByName(studioSystem, name, value, 1);
    return result == FMOD_OK;
}