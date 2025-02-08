local ffi = require "ffi"
local audioPlayer = require "fmodInterface"

function love.load()
    if audioPlayer.init_fmod() then print "FMOD STUDIO Init" end
    audioPlayer.load_bank("fmod/banks/Desktop/Master.bank")
    audioPlayer.load_bank("fmod/banks/Desktop/Master.strings.bank")
    audioPlayer.load_bank("fmod/banks/Desktop/Music.bank")
    local event = audioPlayer.get_event("event:/music_test")
    audioPlayer.set_event_marker_callback(event, function(callbackInfo) print("callback!", ffi.string(callbackInfo)) end)
    --audioPlayer.set_event_beat_callback(event,
    --function(bar, beat) print(string.format("Bar: %d, Beat: %d", bar, beat)) end)
    audioPlayer.play_event(event)
end

function love.update()
    audioPlayer.update_fmod()
end

function love.quit()
    if audioPlayer.release_fmod() then print "FMOD STUDIO Release" end
end
