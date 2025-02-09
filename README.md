# fmodlua
An [FmodStudio](https://www.fmod.com/resources/documentation-api?version=2.02&page=welcome.html) implementation in C for building a shared library for Lua.

It covers the basic functionalities of Fmod, ideal for 2d projects (no 3D attributes implementation yet). Feel free to extend it as per your needs.

This repository provides the source code in order to build the shared library. However, it doesn't include the fmod libraries as it's not under the same licence, meaning you still have to download the fmod API for your platform on their [website](https://www.fmod.com/download).

This code should work with any game engine using Lua 5.1 and LuaJIT 2.1. For other versions you might need to change the header files in the lua/include folder and the Windows dlls in lua/lib.

## How to build 

Example using GCC on Linux
```
LINUX_API=path/to/fmodstudio/linux/api

gcc -c -fPIC src/fmod_lua.c -I$LINUX_API/core/inc -I$LINUX_API/studio/inc -Isrc
LD_RUN_PATH='$ORIGIN' gcc -o libfmodlua.so -Wl,-undefined,dynamic_lookup -shared fmod_lua.o -L$LINUX_API/studio/lib/x86_64 -L$LINUX_API/core/lib/x86_64 -lfmod -lfmodstudio
```

### Usage

See example using [Löve](https://love2d.org/wiki/Main_Page).

#### Licence

[MIT](https://mit-license.org/)
