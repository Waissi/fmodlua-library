local audioPlayer = require "fmodInterface"

function love.load()
    if audioPlayer.init() then print "FMOD STUDIO Init" end
    audioPlayer.load_bank("fmod/banks/Desktop/Master.bank")
    audioPlayer.load_bank("fmod/banks/Desktop/Master.strings.bank")
    audioPlayer.load_bank("fmod/banks/Desktop/Music.bank")
    local event = audioPlayer.get_event("event:/Test")
    audioPlayer.play_event(event)
end

function love.update()
    audioPlayer.update()
end

function love.quit()
    if audioPlayer.release() then print "FMOD STUDIO Release" end
end
