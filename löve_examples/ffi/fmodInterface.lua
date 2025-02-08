local ffi = require "ffi"
ffi.cdef [[
    typedef void* FMOD_STUDIO_BANK;
    typedef void* FMOD_STUDIO_BUS;
    typedef void* FMOD_STUDIO_VCA;
    typedef void* FMOD_STUDIO_EVENTINSTANCE;

    bool init_fmod();
    bool update_fmod();
    bool release_fmod();

    FMOD_STUDIO_BANK load_bank(const char* path);
    bool unload_bank(FMOD_STUDIO_BANK bank);

    FMOD_STUDIO_BUS get_bus(const char* path);
    bool set_bus_volume(FMOD_STUDIO_BUS bus, float value);
    float get_bus_volume(FMOD_STUDIO_BUS bus);
    bool mute_bus(FMOD_STUDIO_BUS bus, bool mute);
    bool is_bus_muted(FMOD_STUDIO_BUS bus);

    FMOD_STUDIO_VCA get_vca(const char* path);
    bool set_vca_volume(FMOD_STUDIO_VCA vca, float value);
    float get_vca_volume(FMOD_STUDIO_VCA vca);

    FMOD_STUDIO_EVENTINSTANCE get_event(const char* path);
    bool play_one_shot_event(const char* path);
    bool play_event(FMOD_STUDIO_EVENTINSTANCE event);
    bool stop_event(FMOD_STUDIO_EVENTINSTANCE event);
    bool pause_event(FMOD_STUDIO_EVENTINSTANCE event, bool paused);
    bool is_event_playing(FMOD_STUDIO_EVENTINSTANCE event);
    bool set_event_marker_callback(FMOD_STUDIO_EVENTINSTANCE event, void(*callback)(const char* name));
    bool set_event_beat_callback(FMOD_STUDIO_EVENTINSTANCE event, void(*callback)(int bar, int beat));

    bool set_parameter_by_name(FMOD_STUDIO_EVENTINSTANCE event, const char* name, float value);
    float get_paramater_by_name(FMOD_STUDIO_EVENTINSTANCE event, const char* name);
    bool set_global_paramter_by_name(const char* name, float value);
    float get_global_parameter_by_name(const char* name);
]]

if ffi.os == "Windows" then
    return ffi.load("fmodluajitffi.dll")
elseif ffi.os == "OSX" then
    return ffi.load("fmod/lib/libfmodluajitffi.dylib")
else
    return ffi.load("fmod/lib/libfmodluajitffi.so")
end
