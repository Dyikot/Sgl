workspace "Sgl"
    architecture "x64"
    configurations { "Debug", "Release" }

project "Sgl"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    location "%{prj.name}"

    targetdir "bin/%{prj.name}/Output/%{cfg.buildcfg}"
    objdir "bin/%{prj.name}/Intermediate/%{cfg.buildcfg}"

    includedirs {
        "Dependencies/Include"
    }

    files {
        "%{prj.name}/**.cpp",
        "%{prj.name}/**.h"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "on"

project "App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    location "%{prj.name}"

    targetdir "bin/%{prj.name}/Output/%{cfg.buildcfg}"
    objdir "bin/%{prj.name}/Intermediate/%{cfg.buildcfg}"

    files {
        "%{prj.name}/**.cpp"
    }

    includedirs {
        "Dependencies/Include",
        "Sgl"
    }

    libdirs {
        "Dependencies/Libraries"
    }

    links {
        "Sgl",
        "SDL3",
        "SDL3_image",
        "SDL3_ttf"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "on"