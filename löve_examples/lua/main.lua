io.stdout:setvbuf('no')

love.filesystem.setCRequirePath("fmod/lib/??")
local fmod = require "libfmodlua"

function love.load()
    if fmod.init() then print("FMOD INITIALIZED") end
    fmod.load_bank("fmod/banks/Desktop/Master.bank")
    fmod.load_bank("fmod/banks/Desktop/Master.strings.bank")
    fmod.load_bank("fmod/banks/Desktop/Music.bank")
    local instance = fmod.get_event("event:/music_test")
    fmod.play_event(instance)
    fmod.set_event_marker_callback(instance, function() print "callback!" end)
end

function love.quit()
    if fmod.release() then print("FMOD RELEASED") end
end

function love.update()
    fmod.update()
end
