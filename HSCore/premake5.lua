project "HSCore"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"

    targetdir "%{OutputDir.Binaries}/%{prj.name}"
    objdir    "%{OutputDir.Intermediates}/%{prj.name}"

    files
    {
        "Source/**.h",
        "Source/**.cpp"
    }

    includedirs
    {
        "Source"
    }

    filter "system:windows"
        systemversion "latest"
        defines
        {
            "HS_PLATFORM_WINDOWS",
            "HS_STABLE_PLATFORM"
        }

    filter "system:linux"
        defines
        {
            "HS_PLATFORM_LINUX",
            "HS_EXPERIMENTAL_PLATFORM"
        }

    filter "platforms:x64"
        defines
        {
            "HS_ARCH_X64",
            "HS_BITNESS=64"
        }

    filter "platforms:ARM64"
        defines
        {
            "HS_ARCH_ARM64",
            "HS_BITNESS=64"
        }
    
    filter "configurations:Development"
        runtime "Debug"
        symbols "On"
        defines
        {
            "HS_DEVELOPMENT",
            "HS_TEST_BUILD"
        }
    
    filter "configurations:Preview"
        runtime "Release"
        optimize "Speed"
        defines
        {
            "HS_PREVIEW",
            "HS_TEST_BUILD"
        }
    
    filter "configurations:Shipping"
        runtime "Release"
        optimize "Speed"
        defines
        {
            "HS_SHIPPING",
            "HS_PRODUCTION_BUILD"
        }
