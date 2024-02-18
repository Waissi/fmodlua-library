local fmod
local ffi = require "ffi"

ffi.cdef [[
    typedef struct {} FMOD_STUDIO_BANK;
    typedef struct {} FMOD_STUDIO_BUS;
    typedef struct {} FMOD_STUDIO_VCA;
    typedef struct {} FMOD_STUDIO_EVENTINSTANCE;

    bool InitFmod();
    bool UpdateFmod();
    bool ReleaseFmod();

    FMOD_STUDIO_BANK* LoadBank(const char* path);
    bool UnloadBank(FMOD_STUDIO_BANK* bank);

    FMOD_STUDIO_BUS* GetBus(const char* path);
    bool SetBusVolume(FMOD_STUDIO_BUS* bus, float value);
    float GetBusVolume(FMOD_STUDIO_BUS* bus);
    bool MuteBus(FMOD_STUDIO_BUS* bus, bool mute);
    bool IsBusMuted(FMOD_STUDIO_BUS* bus);

    FMOD_STUDIO_VCA* GetVCA(const char* path);
    bool SetVcaVolume(FMOD_STUDIO_VCA* vca, float value);
    float GetVcaVolume(FMOD_STUDIO_VCA* vca);

    FMOD_STUDIO_EVENTINSTANCE* GetEvent(const char* path);
    bool PlayOneShotEvent(const char* path);
    bool PlayEvent(FMOD_STUDIO_EVENTINSTANCE* event);
    bool StopEvent(FMOD_STUDIO_EVENTINSTANCE *event);
    bool PauseEvent(FMOD_STUDIO_EVENTINSTANCE *event, bool paused);
    bool IsEventPlaying(FMOD_STUDIO_EVENTINSTANCE *event);
    bool SetEventMarkerCallback(FMOD_STUDIO_EVENTINSTANCE *event, void(*callback)(const char* name));
    bool SetEventBeatCallback(FMOD_STUDIO_EVENTINSTANCE *event, void(*callback)(int bar, int beat));

    bool SetParamaterByName(FMOD_STUDIO_EVENTINSTANCE *event, const char* name, float value);
    float GetParameterByName(FMOD_STUDIO_EVENTINSTANCE *event, const char* name);
    bool SetGlobalParamaterByName(const char* name, float value);
    float GetGlobalParameterByName(const char* name);
    ]]

local init = function()
    if ffi.os == "Windows" then
        fmod = ffi.load("fmodluajit.dll")
    else
        fmod = ffi.load("fmod/libs/libfmodluajit.so")
    end
    return fmod.InitFmod()
end

return {
    init = init,
    update = function() return fmod.UpdateFmod() end,
    release = function() return fmod.ReleaseFmod() end,
    load_bank = function(path) return fmod.LoadBank(path) end,
    unload_bank = function(bank) return fmod.UnloadBank(bank) end,
    get_bus = function(path) return fmod.GetBus(path) end,
    set_bus_volume = function(bus, value) return fmod.SetBusVolume(bus, value) end,
    get_bus_volume = function(bus) return fmod.GetBusVolume(bus) end,
    mute_bus = function(bus, mute) return fmod.MuteBus(bus, mute) end,
    is_bus_muted = function(bus) return fmod.IsBusMuted(bus) end,
    get_vca = function(path) return fmod.GetVCA(path) end,
    set_vca_volume = function(vca, value) return fmod.SetVcaVolume(vca, value) end,
    get_vca_volume = function(vca) return fmod.GetVcaVolume(vca) end,
    get_event = function(path) return fmod.GetEvent(path) end,
    play_one_shot_event = function(path) return fmod.PlayOneShotEvent(path) end,
    play_event = function(event) return fmod.PlayEvent(event) end,
    stop_event = function(event) return fmod.StopEvent(event) end,
    pause_event = function(event, paused) return fmod.PauseEvent(event, paused) end,
    is_event_playing = function(event) return fmod.IsEventPlaying(event) end,
    set_event_marker_callback = function(event, callback) return fmod.SetEventMarkerCallback(event, callback) end,
    set_event_beat_callback = function(event, callback) return fmod.SetEventBeatCallback(event, callback) end,
    set_parameter_by_name = function(event, name, value) return fmod.SetParamaterByName(event, name, value) end,
    get_paramater_by_name = function(event, name) return fmod.GetParameterByName(event, name) end,
    set_global_parameter_by_name = function(name, value) return fmod.SetGlobalParamaterByName(name, value) end,
    get_global_paramater_by_name = function(name) return fmod.GetGlobalParameterByName(name) end
}
