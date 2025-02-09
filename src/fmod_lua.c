#include "fmod_lua.h"

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
    ReleaseEventCallbackList();
    FMOD_RESULT result = FMOD_Studio_System_Release(studioSystem);
    return result == FMOD_OK;
}

/*
--------------------------------------             BANKS         --------------------------------
*/

FMOD_STUDIO_BANK *LoadBank(const char *path)
{
    FMOD_STUDIO_BANK *bank;
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
    FMOD_STUDIO_BUS *bus;
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
    return result == FMOD_OK;
}

/*
-------------------------------------             VCAs         --------------------------------
*/

FMOD_STUDIO_VCA *GetVCA(const char *path)
{
    FMOD_STUDIO_VCA *vca;
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
    FMOD_STUDIO_EVENTDESCRIPTION *eventDescription;
    FMOD_RESULT result = FMOD_Studio_System_GetEvent(studioSystem, path, &eventDescription);
    if (result != FMOD_OK)
    {
        return NULL;
    }

    FMOD_STUDIO_EVENTINSTANCE *instance;
    result = FMOD_Studio_EventDescription_CreateInstance(eventDescription, &instance);
    if (result != FMOD_OK)
    {
        return NULL;
    }
    return instance;
}

FMOD_BOOL ReleaseEvent(FMOD_STUDIO_EVENTINSTANCE *event)
{
    FMOD_RESULT result = FMOD_Studio_EventInstance_Stop(event, FMOD_STUDIO_STOP_IMMEDIATE);
    if (result != FMOD_OK)
    {
        return 0;
    }
    struct eventCallback *eventCallback = RetrieveEventCallback(event);
    if (eventCallback)
    {
        luaL_unref(luaState, LUA_REGISTRYINDEX, eventCallback->registryIndex);
        ReleaseEventCallback(eventCallback);
    }
    result = FMOD_Studio_EventInstance_Release(event);
    return result == FMOD_OK;
}

FMOD_BOOL PlayOneShotEvent(const char *path)
{
    FMOD_STUDIO_EVENTINSTANCE *event;
    FMOD_STUDIO_EVENTDESCRIPTION *eventDescription;
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

void F_CALLBACK OnEventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters)
{
    struct eventCallback *eventCallback = RetrieveEventCallback(event);
    if (!eventCallback)
    {
        return;
    }

    union eventCallbackProperties properties;

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

FMOD_BOOL SetEventCallback(FMOD_STUDIO_EVENTINSTANCE *event, int registryIndex)
{
    struct eventCallback *eventCallback = RetrieveEventCallback(event);
    if (eventCallback)
    {
        return 0;
    }

    struct eventCallback *link = malloc(sizeof(struct eventCallback));
    link->event = event;
    link->registryIndex = registryIndex;
    link->next = headCallback;
    headCallback = link;

    FMOD_RESULT result = FMOD_Studio_EventInstance_SetCallback(event, (FMOD_STUDIO_EVENT_CALLBACK)OnEventCallback, FMOD_STUDIO_EVENT_CALLBACK_ALL);
    return result == FMOD_OK;
}

/*
-----------------------------------             PARAMETERS         --------------------------------
*/

float GetParameterByName(FMOD_STUDIO_EVENTINSTANCE *event, const char *name)
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

FMOD_BOOL SetParameterByName(FMOD_STUDIO_EVENTINSTANCE *event, const char *name, float value)
{
    if (!event)
    {
        return 0;
    }
    FMOD_RESULT result = FMOD_Studio_EventInstance_SetParameterByName(event, name, value, 1);
    return result == FMOD_OK;
}

float GetGlobalParameterByName(const char *name)
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

FMOD_BOOL SetGlobalParameterByName(const char *name, float value)
{
    FMOD_RESULT result = FMOD_Studio_System_SetParameterByName(studioSystem, name, value, 1);
    return result == FMOD_OK;
}

/*
-----------------------------------             LUA INTERFACE         --------------------------------
*/

static int init()
{
    FMOD_BOOL result = InitFmod();
    lua_pushboolean(luaState, result);
    return 1;
}

static int update()
{
    FMOD_BOOL result = UpdateFmod();
    lua_pushboolean(luaState, result);
    return 1;
}

static int release()
{
    FMOD_BOOL result = ReleaseFmod();
    lua_pushboolean(luaState, result);
    return 1;
}

static int load_bank()
{
    const char *path = lua_tostring(luaState, 1);
    FMOD_STUDIO_BANK *bank = LoadBank(path);
    lua_pushlightuserdata(luaState, bank);
    return 1;
}

static int unload_bank()
{
    FMOD_STUDIO_BANK *bank = lua_touserdata(luaState, 1);
    FMOD_BOOL result = UnloadBank(bank);
    lua_pushboolean(luaState, result);
    return 1;
}

static int get_bus()
{
    const char *path = lua_tostring(luaState, 1);
    FMOD_STUDIO_BUS *bus = GetBus(path);
    lua_pushlightuserdata(luaState, bus);
    return 1;
}

static int set_bus_volume()
{
    FMOD_STUDIO_BUS *bus = lua_touserdata(luaState, 1);
    float volume = lua_tonumber(luaState, 2);
    FMOD_BOOL result = SetBusVolume(bus, volume);
    lua_pushboolean(luaState, result);
    return 1;
}

static int get_bus_volume()
{
    FMOD_STUDIO_BUS *bus = lua_touserdata(luaState, 1);
    float volume = GetBusVolume(bus);
    lua_Number result = volume;
    lua_pushnumber(luaState, result);
    return 1;
}

static int mute_bus()
{
    FMOD_STUDIO_BUS *bus = lua_touserdata(luaState, 1);
    FMOD_BOOL mute = lua_toboolean(luaState, 2);
    FMOD_BOOL result = MuteBus(bus, mute);
    lua_pushboolean(luaState, result);
    return 1;
}

static int is_bus_muted()
{
    FMOD_STUDIO_BUS *bus = lua_touserdata(luaState, 1);
    FMOD_BOOL result = IsBusMuted(bus);
    lua_pushboolean(luaState, result);
    return 1;
}

static int get_vca()
{
    const char *path = lua_tostring(luaState, 1);
    FMOD_STUDIO_VCA *vca = GetVCA(path);
    lua_pushlightuserdata(luaState, vca);
    return 1;
}

static int set_vca_volume()
{
    FMOD_STUDIO_VCA *vca = lua_touserdata(luaState, 1);
    float volume = lua_tonumber(luaState, 2);
    FMOD_BOOL result = SetVcaVolume(vca, volume);
    lua_pushboolean(luaState, result);
    return 1;
}

static int get_vca_volume()
{
    FMOD_STUDIO_VCA *vca = lua_touserdata(luaState, 1);
    float volume = GetVcaVolume(vca);
    lua_Number result = volume;
    lua_pushnumber(luaState, result);
    return 1;
}

static int get_event()
{
    const char *path = lua_tostring(luaState, 1);
    FMOD_STUDIO_EVENTINSTANCE *instance = GetEvent(path);
    lua_pushlightuserdata(luaState, instance);
    return 1;
}

static int release_event()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    FMOD_BOOL result = ReleaseEvent(event);
    lua_pushboolean(luaState, result);
    return 1;
}

static int play_one_shot_event()
{
    const char *path = lua_tostring(luaState, 1);
    FMOD_BOOL result = PlayOneShotEvent(path);
    lua_pushboolean(luaState, result);
    return 1;
}

static int play_event()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    FMOD_BOOL result = PlayEvent(event);
    lua_pushboolean(luaState, result);
    return 1;
}

static int stop_event()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    FMOD_BOOL result = StopEvent(event);
    lua_pushboolean(luaState, result);
    return 1;
}

static int pause_event()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    FMOD_BOOL pause = lua_toboolean(luaState, 2);
    FMOD_BOOL result = PauseEvent(event, pause);
    lua_pushboolean(luaState, result);
    return 1;
}

static int is_event_playing()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    FMOD_BOOL result = IsEventPlaying(event);
    lua_pushboolean(luaState, result);
    return 1;
}

static int set_parameter_by_name()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    const char *path = lua_tostring(luaState, 2);
    float parameterValue = lua_tonumber(luaState, 3);
    FMOD_BOOL result = SetParameterByName(event, path, parameterValue);
    lua_pushboolean(luaState, result);
    return 1;
}

static int get_parameter_by_name()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    const char *path = lua_tostring(luaState, 2);
    float result = GetParameterByName(event, path);
    lua_pushnumber(luaState, result);
    return 1;
}

static int set_global_parameter_by_name()
{
    const char *path = lua_tostring(luaState, 1);
    float parameterValue = lua_tonumber(luaState, 2);
    FMOD_BOOL result = SetGlobalParameterByName(path, parameterValue);
    lua_pushboolean(luaState, result);
    return 1;
}

static int get_global_parameter_by_name()
{
    const char *path = lua_tostring(luaState, 1);
    float result = GetGlobalParameterByName(path);
    lua_pushnumber(luaState, result);
    return 1;
}

static int set_event_callback()
{
    FMOD_STUDIO_EVENTINSTANCE *event = lua_touserdata(luaState, 1);
    lua_pushvalue(luaState, 2);
    int registryIndex = luaL_ref(luaState, LUA_REGISTRYINDEX);
    FMOD_BOOL result = SetEventCallback(event, registryIndex);
    lua_pushboolean(luaState, result);
    return 1;
}

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
    {"is_event_playing", is_event_playing},
    {"get_global_parameter_by_name", get_global_parameter_by_name},
    {"set_global_parameter_by_name", set_global_parameter_by_name},
    {"get_parameter_by_name", get_parameter_by_name},
    {"set_parameter_by_name", set_parameter_by_name},
    {"set_event_callback", set_event_callback},
    {"get_bus", get_bus},
    {"get_bus_volume", get_bus_volume},
    {"set_bus_volume", set_bus_volume},
    {"is_bus_muted", is_bus_muted},
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