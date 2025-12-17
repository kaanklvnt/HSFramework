workspace "HSFramework"
    platforms
    {
        "x64",
        "ARM64"
    }
    defaultplatform "x64"
    configurations
    {
        "Development",
        "Preview",
        "Shipping"
    }
    startproject "HSEditor"

include "premake5.deps.lua"

include "HSCore"
include "HSEditor"
include "HSRuntime"
