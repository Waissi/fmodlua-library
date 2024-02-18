# fmodlua
An [FmodStudio](https://www.fmod.com/resources/documentation-api?version=2.02&page=welcome.html) implementation in C for building a shared library for Lua or LuaJIT with the [ffi library](https://luajit.org/ext_ffi.html).

It covers the basic functionalities of Fmod, ideal for 2d projects (no 3D attributes implementation yet). Feel free to extend it as per your needs.

This repository provides the source code in order to build the shared library. However, it doesn't include the fmod libraries as it's not under the same licence, meaning you still have to download the fmod API for your platform on their [website](https://www.fmod.com/download).

This code should work with any game engine using Lua 5.1 and LuaJIT 2.1. For other versions you might need to change the header files in the lua/include folder and the Windows dlls in lua/lib.

## How to build 

Here are two examples, using GCC and Zig for building the Lua version. For LuaJIT you can do the same, the only differences are that there is no need to include the lua header file and there is no linking against lua51.dll for Windows.

Using GCC
```
LINUX_API=path/to/fmodstudio/linux/api

gcc -c -fPIC lua/fmod_lua.c -I$LINUX_API/core/inc -I$LINUX_API/studio/inc -Ilua/include
LD_RUN_PATH='$ORIGIN' gcc -o libfmodlua.so -Wl,-undefined,dynamic_lookup -shared fmod_lua.o -L$LINUX_API/studio/lib/x86_64 -L$LINUX_API/core/lib/x86_64 -lfmod -lfmodstudio
```

Using Zig
```zig
const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const libfmodlua_linux = b.addSharedLibrary(.{
        .name = "fmodlua",
        .target = target,
        .optimize = optimize,
    });

    b.installArtifact(libfmodlua_linux);
    libfmodlua_linux.addIncludePath(.{ .path = "include" });
    libfmodlua_linux.addIncludePath(.{ .path = "path/to/fmodstudio/linux/api/core/inc" });
    libfmodlua_linux.addIncludePath(.{ .path = "path/to/fmodstudio/linux/api/studio/inc" });
    libfmodlua_linux.addObjectFile(.{ .path = "path/to/fmodstudio/linux/api/core/lib/x86_64/libfmod.so.13.20" });
    libfmodlua_linux.addObjectFile(.{ .path = "path/to/fmodstudio/linux/api/studio/lib/x86_64/libfmodstudio.so.13.20" });
    libfmodlua_linux.addCSourceFile(.{ .file = .{ .path = "lua/fmod_lua.c" }, .flags = &.{} });
    libfmodlua_linux.linkLibC();
    libfmodlua_linux.root_module.addRPathSpecial("$ORIGIN");

    const libfmodlua_windows = b.addSharedLibrary(.{
        .name = "fmodlua",
        .target = b.resolveTargetQuery(.{ .cpu_arch = .x86_64, .os_tag = .windows }),
        .optimize = optimize,
    });
    b.installArtifact(libfmodlua_windows);
    libfmodlua_windows.addIncludePath(.{ .path = "include" });
    libfmodlua_windows.addIncludePath(.{ .path = "path/to/fmodstudio/windows/api/core/inc" });
    libfmodlua_windows.addIncludePath(.{ .path = "path/to/fmodstudio/windows/api/studio/inc" });
    libfmodlua_windows.addObjectFile(.{ .path = "path/to/fmodstudio/windows/api/core/lib/x64/fmod.dll" });
    libfmodlua_windows.addObjectFile(.{ .path = "path/to/fmodstudio/windows/api/studio/lib/x64/fmodstudio.dll" });
    libfmodlua_windows.addObjectFile(.{ .path = "lua/lib/lua51.dll" });
    libfmodlua_windows.addCSourceFile(.{ .file = .{ .path = "lua/fmod_lua.c" }, .flags = &.{} });
    libfmodlua_windows.linkLibC();
}
```

### Usage

Examples using [Löve](https://love2d.org/wiki/Main_Page) are provided for both version.

#### Licence

[MIT](https://mit-license.org/)
