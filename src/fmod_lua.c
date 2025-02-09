#include "lua.h"
#include "lauxlib.h"
#include "fmod.h"
#include "fmod_studio.h"
#include <stdlib.h>

lua_State *luaState = NULL;
FMOD_STUDIO_SYSTEM *studioSystem = NULL;

union EventCallbackProperties
{
    FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES *marker;
    FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES *beat;
};

struct eventCallback
{
    FMOD_STUDIO_EVENTINSTANCE *event;
    int registryIndex;
    struct eventCallback *next;
};
struct eventCallback *headCallback = NULL;

static void release_event_callback_list()
{
    while (headCallback)
    {
        struct eventCallback *current = headCallback;
        headCallback = current->next;
        free(current);
    }
}

static struct eventCallback *retrieve_event_callback(FMOD_STUDIO_EVENTINSTANCE *event)
{
    struct eventCallback *current = headCallback;
    if (headCallback == NULL)
    {
        return NULL;
    }
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

static void release_event_callback(struct eventCallback *eventCallback)
{
    struct eventCallback *current = headCallback;
    if (headCallback == eventCallback)
    {
        headCallback = NULL;
        free(eventCallback);
        return;
    }
    while (current->next != eventCallback)
    {
        if (current->next == NULL)
        {
            return;
        }
        else
        {
            current = current->next;
        }
    }
    current->next = current->next->next;
    free(current->next);
}

static void F_CALLBACK on_event_callback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters)
{
    struct eventCallback *eventCallback = retrieve_event_callback(event);
    if (!eventCallback)
    {
        return;
    }

    union EventCallbackProperties properties;

    switch (type)
    {
    case FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER:
        lua_rawgeti(luaState, LUA_REGISTRYINDEX, eventCallback->registryIndex);
        properties.marker = parameters;
        lua_pushinteger(luaState, type);
        lua_pushstring(luaState, properties.marker->name);
        lua_pushinteger(luaState, properties.marker->position);
        lua_call(luaState, 3, 0);
        break;

    case FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT:
        lua_rawgeti(luaState, LUA_REGISTRYINDEX, eventCallback->registryIndex);
        properties.beat = parameters;
        lua_pushinteger(luaState, type);
        lua_pushinteger(luaState, properties.beat->bar);
        lua_pushinteger(luaState, properties.beat->beat);
        lua_call(luaState, 3, 0);
        break;
    }
};

/*
---------------------------------             FMOD STUDIO SYSTEM         --------------------------------
*/

static int init()
{
    FMOD_RESULT result = FMOD_Studio_System_Create(&studioSystem, FMOD_VERSION);
    if (result != FMOD_OK)
    {
        lua_pushboolean(luaState, 0);
        return 1;
    }
    result = FMOD_Studio_System_Initialize(studioSystem, 128, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

static int update()
{
    FMOD_RESULT result = FMOD_Studio_System_Update(studioSystem);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

static int release()
{
    release_event_callback_list();
    FMOD_RESULT result = FMOD_Studio_System_Release(studioSystem);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

/*
--------------------------------------             BANKS         --------------------------------
*/

static int load_bank()
{
    const char *path = lua_tostring(luaState, 1);
    FMOD_STUDIO_BANK *bank = NULL;
    FMOD_RESULT result = FMOD_Studio_System_LoadBankFile(studioSystem, path, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
    lua_pushlightuserdata(luaState, bank);
    return 1;
}

static int unload_bank()
{
    FMOD_STUDIO_BANK *bank = lua_touserdata(luaState, 1);
    FMOD_BOOL result = FMOD_Studio_Bank_Unload(bank);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

/*
--------------------------------------             BUSES         --------------------------------
*/

static int get_bus()
{
    const char *path = lua_tostring(luaState, 1);
    FMOD_STUDIO_BUS *bus = NULL;
    FMOD_RESULT result = FMOD_Studio_System_GetBus(studioSystem, path, &bus);
    lua_pushlightuserdata(luaState, bus);
    return 1;
}

static int set_bus_volume()
{
    FMOD_STUDIO_BUS *bus = lua_touserdata(luaState, 1);
    float volume = lua_tonumber(luaState, 2);
    FMOD_RESULT result = FMOD_Studio_Bus_SetVolume(bus, volume);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

static int get_bus_volume()
{
    FMOD_STUDIO_BUS *bus = lua_touserdata(luaState, 1);
    float volume, finalVolume;
    FMOD_RESULT result = FMOD_Studio_Bus_GetVolume(bus, &volume, &finalVolume);
    lua_Number returnValue = result == FMOD_OK ? finalVolume : -1.0f;
    lua_pushnumber(luaState, returnValue);
    return 1;
}

static int mute_bus()
{
    FMOD_STUDIO_BUS *bus = lua_touserdata(luaState, 1);
    FMOD_BOOL mute = lua_toboolean(luaState, 2);
    FMOD_RESULT result = FMOD_Studio_Bus_SetMute(bus, mute);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

static int bus_is_muted()
{
    FMOD_STUDIO_BUS *bus = lua_touserdata(luaState, 1);
    FMOD_BOOL mute;
    FMOD_RESULT result = FMOD_Studio_Bus_GetMute(bus, &mute);
    lua_pushboolean(luaState, result == FMOD_OK ? mute : 0);
    return 1;
}

/*
-------------------------------------             VCAs         --------------------------------
*/

static int get_vca()
{
    const char *path = lua_tostring(luaState, 1);
    FMOD_STUDIO_VCA *vca = NULL;
    FMOD_RESULT result = FMOD_Studio_System_GetVCA(studioSystem, path, &vca);
    lua_pushlightuserdata(luaState, vca);
    return 1;
}

static int set_vca_volume()
{
    FMOD_STUDIO_VCA *vca = lua_touserdata(luaState, 1);
    float volume = lua_tonumber(luaState, 2);
    FMOD_RESULT result = FMOD_Studio_VCA_SetVolume(vca, volume);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

static int get_vca_volume()
{
    FMOD_STUDIO_VCA *vca = lua_touserdata(luaState, 1);
    float volume, finalVolume;
    FMOD_RESULT result = FMOD_Studio_VCA_GetVolume(vca, &volume, &finalVolume);
    lua_Number returnValue = result == FMOD_OK ? finalVolume : -1.0f;
    lua_pushnumber(luaState, returnValue);
    return 1;
}

/*
---------------------------------------             EVENTS         --------------------------------
*/

static int get_event()
{
    const char *path = lua_tostring(luaState, 1);
    FMOD_STUDIO_EVENTINSTANCE *instance = NULL;
    FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = NULL;
    FMOD_RESULT result = FMOD_Studio_System_GetEvent(studioSystem, path, &eventDescription);
    if (result != FMOD_OK)
    {
        lua_pushlightuserdata(luaState, instance);
        return 1;
    }
    result = FMOD_Studio_EventDescription_CreateInstance(eventDescription, &instance);
    lua_pushlightuserdata(luaState, instance);
    return 1;
}

static int release_event()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    FMOD_RESULT result = FMOD_Studio_EventInstance_Stop(event, FMOD_STUDIO_STOP_IMMEDIATE);
    if (result != FMOD_OK)
    {
        lua_pushboolean(luaState, 0);
        return 1;
    }
    struct eventCallback *eventCallback = retrieve_event_callback(event);
    if (eventCallback)
    {
        luaL_unref(luaState, LUA_REGISTRYINDEX, eventCallback->registryIndex);
        release_event_callback(eventCallback);
    }
    result = FMOD_Studio_EventInstance_Release(event);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

static int play_one_shot_event()
{
    const char *path = lua_tostring(luaState, 1);
    FMOD_STUDIO_EVENTINSTANCE *event = NULL;
    FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = NULL;
    FMOD_RESULT result = FMOD_Studio_System_GetEvent(studioSystem, path, &eventDescription);
    if (result != FMOD_OK)
    {
        lua_pushboolean(luaState, 0);
        return 1;
    }
    result = FMOD_Studio_EventDescription_CreateInstance(eventDescription, &event);
    if (result != FMOD_OK)
    {
        lua_pushboolean(luaState, 0);
        return 1;
    }
    result = FMOD_Studio_EventInstance_Start(event);
    if (result != FMOD_OK)
    {
        lua_pushboolean(luaState, 0);
        return 1;
    }
    result = FMOD_Studio_EventInstance_Release(event);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

static int play_event()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    FMOD_RESULT result = FMOD_Studio_EventInstance_Start(event);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

static int stop_event()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    FMOD_RESULT result = FMOD_Studio_EventInstance_Stop(event, FMOD_STUDIO_STOP_ALLOWFADEOUT);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

static int pause_event()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    FMOD_BOOL pause = lua_toboolean(luaState, 2);
    FMOD_RESULT result = FMOD_Studio_EventInstance_SetPaused(event, pause);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

static int event_is_playing()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    FMOD_STUDIO_PLAYBACK_STATE state;
    FMOD_RESULT result = FMOD_Studio_EventInstance_GetPlaybackState(event, &state);
    if (result != FMOD_OK)
    {
        lua_pushboolean(luaState, 0);
        return 1;
    }
    FMOD_BOOL paused;
    result = FMOD_Studio_EventInstance_GetPaused(event, &paused);
    if (result != FMOD_OK)
    {
        lua_pushboolean(luaState, 0);
        return 1;
    }
    lua_pushboolean(luaState, state == FMOD_STUDIO_PLAYBACK_PLAYING && paused == 0);
    return 1;
}

static int set_event_callback()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    if (retrieve_event_callback(event))
    {
        lua_pushboolean(luaState, 0);
        return 1;
    }
    lua_pushvalue(luaState, 2);
    int registryIndex = luaL_ref(luaState, LUA_REGISTRYINDEX);
    struct eventCallback *eventCallback = malloc(sizeof(struct eventCallback));
    eventCallback->event = event;
    eventCallback->registryIndex = registryIndex;
    eventCallback->next = headCallback;
    headCallback = eventCallback;
    FMOD_RESULT result = FMOD_Studio_EventInstance_SetCallback(event, (FMOD_STUDIO_EVENT_CALLBACK)on_event_callback, FMOD_STUDIO_EVENT_CALLBACK_ALL);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

/*
-----------------------------------             PARAMETERS         --------------------------------
*/

static int get_parameter_by_name()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    const char *name = lua_tostring(luaState, 2);
    float value, finalValue;
    FMOD_RESULT result = FMOD_Studio_EventInstance_GetParameterByName(event, name, &value, &finalValue);
    lua_Number returnValue = result == FMOD_OK ? finalValue : -1.0f;
    lua_pushnumber(luaState, returnValue);
    return 1;
}

static int set_parameter_by_name()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    const char *name = lua_tostring(luaState, 2);
    float value = lua_tonumber(luaState, 3);
    FMOD_BOOL result = FMOD_Studio_EventInstance_SetParameterByName(event, name, value, 1);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

static int set_global_parameter_by_name()
{
    const char *name = lua_tostring(luaState, 1);
    float value = lua_tonumber(luaState, 2);
    FMOD_RESULT result = FMOD_Studio_System_SetParameterByName(studioSystem, name, value, 1);
    lua_pushboolean(luaState, result == FMOD_OK);
    return 1;
}

static int get_global_parameter_by_name()
{
    const char *name = lua_tostring(luaState, 1);
    float value, finalValue;
    FMOD_RESULT result = FMOD_Studio_System_GetParameterByName(studioSystem, name, &value, &finalValue);
    lua_Number returnValue = result == FMOD_OK ? finalValue : -1.0f;
    lua_pushnumber(luaState, returnValue);
    return 1;
}

/*
-----------------------------------             LUA INTERFACE         --------------------------------
*/

static const struct luaL_Reg interface[] = {
    {"init", init},
    {"update", update},
    {"release", release},
    {"load_bank", load_bank},
    {"unload_bank", unload_bank},
    {"get_event", get_event},
    {"release_event", release_event},
    {"play_event", play_event},
    {"stop_event", stop_event},
    {"pause_event", pause_event},
    {"play_one_shot_event", play_one_shot_event},
    {"event_is_playing", event_is_playing},
    {"get_global_parameter_by_name", get_global_parameter_by_name},
    {"set_global_parameter_by_name", set_global_parameter_by_name},
    {"get_parameter_by_name", get_parameter_by_name},
    {"set_parameter_by_name", set_parameter_by_name},
    {"set_event_callback", set_event_callback},
    {"get_bus", get_bus},
    {"get_bus_volume", get_bus_volume},
    {"set_bus_volume", set_bus_volume},
    {"bus_is_muted", bus_is_muted},
    {"mute_bus", mute_bus},
    {"get_vca", get_vca},
    {"get_vca_volume", get_vca_volume},
    {"set_vca_volume", set_vca_volume},
    {NULL, NULL}};

int luaopen_libfmodlua(lua_State *L)
{
    luaState = L;
    luaL_register(luaState, "libfmodlua", interface);
    return 1;
}