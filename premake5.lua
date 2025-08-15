workspace "NGine"
    configurations {"debug", "release"}

    filter "configurations:debug"
        defines {"DEBUG"}
        symbols "On"
        optimize "Off"

    filter "configurations:release"
        defines {"NDEBUG"}
        symbols "Off"
        optimize "On"

    filter {}

project "ngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetname "ngine_%{cfg.buildcfg}"
    targetdir "ngine/lib"

    filter "system:linux"
        defines {"_GLFW_X11"}
        links {"GL"}

    filter "system:windows"
        defines {"_GLFW_WIN32"}
        links {"opengl32", "gdi32"}

    filter {}

    includedirs {"ngine/ext_include", "ngine/include"}
    objdir "ngine/lib/interms"

    files {"ngine/src/**.cpp", "ngine/ext_src/**.cpp", "ngine/ext_src/**.c"}

project "main"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetname "main_%{cfg.buildcfg}"
    targetdir "main/bin"

    dependson {"ngine"}
    links {"ngine"}

    includedirs {"ngine/include"}
    includedirs {"ngine/ext_include"}
    objdir "main/bin/interms"

    files {"main/src/**.cpp"}