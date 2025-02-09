io.stdout:setvbuf('no')

love.filesystem.setCRequirePath("fmod/lib/??")

---@type FmodLib
local fmod = require "libfmodlua"

local FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER = 0x00000800
local FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT = 0x00001000

function love.load()
    if fmod.init() then print("FMOD INITIALIZED") end
    fmod.load_bank("fmod/banks/Desktop/Master.bank")
    fmod.load_bank("fmod/banks/Desktop/Master.strings.bank")
    fmod.load_bank("fmod/banks/Desktop/Music.bank")
    local instance = fmod.get_event("event:/music_test")
    fmod.play_event(instance)
    fmod.set_event_callback(instance, function(type, param1, param2)
        if type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER then
            print(string.format("Marker callback: %s, position: %d", param1, param2))
        elseif type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT then
            print(string.format("Bar: %d, beat: %d", param1, param2))
        end
    end)
end

function love.quit()
    if fmod.release() then print("FMOD RELEASED") end
end

function love.update()
    fmod.update()
end
