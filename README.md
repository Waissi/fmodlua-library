# fmodluajit
A dynamic library in C for using [FmodStudio](https://www.fmod.com/resources/documentation-api?version=2.02&page=welcome.html) with the [Lua JIT ffi library](https://luajit.org/ext_ffi.html).

It's a minimalistic implementation of Fmod calling only essential functions, aimed for 2d projects (no 3D attributes implementation yet). Feel free to expand it as per your needs.

This repository provides the source code, a lua module and binaires for Windows, MacOS and Linux in the releases section.
I don't provide the fmod libraries as it's not under the same licence, so you still have to download the fmod API for your platform on their [website](https://www.fmod.com/download). 

## Usage
This library should work with any game engine using LuaJIT.

In your project, add the fmod libraries(fmod.dll + fmodstudio.dll for Windows, libfmod.dylib + libfmodstudio.dylib for MacOS, libfmod.so + libfmodstudio.so + symlinks for Linux) and fmodluajit(fmodluajit.dll for Windows, libfmodluajit.dylib for MacOS, libfmodluajit.so for Linux) in the same folder, plus the fmodManager.lua wherever you want.

Then require fmodManager.lua, init the module and call its functions.

Don't forget to update and to release(the rest is garbage collected).

Here is a small example with [Löve2d](https://love2d.org/wiki/Main_Page)

```lua
local fmodMan = require"fmodManager"

function love.load()
	fmodMan.init()
	fmodMan.load_bank("banks/Desktop/Master.bank")
	fmodMan.load_bank("banks/Desktop/Master.strings.bank")
	fmodMan.play_one_shot_event("event:/Test")
end

function love.update(dt) fmodMan.update() end

function love.quit() fmodMan.release() end
```

### Licence

[MIT](https://mit-license.org/)
