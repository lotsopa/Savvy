-- premake5.lua
solution "Savvy"
   configurations { "Debug", "Release" }

-- Savvy Library Project
project "Savvy"
    location "Savvy"
    kind "StaticLib"
    language "C++"
	includedirs { "Savvy/inc", "Savvy/inc/internal", "ext/mcpp/include" }
	libdirs { "ext/mcpp/lib" }
    targetdir "Savvy/lib/%{cfg.buildcfg}"

    files { "Savvy/**.h", "Savvy/**.cpp" }

    filter "configurations:Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

    filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
-- Sample Project
project "Sample"
	location "Sample"
	kind "ConsoleApp"
	language "C++"
	links { "Savvy" }
	includedirs { "Savvy/inc", "ext/mcpp/include" }
	libdirs { "ext/mcpp/lib" }
	targetdir "Sample/bin/%{cfg.buildcfg}"
	debugdir "Sample/bin/%{cfg.buildcfg}"
	
	files { "Sample/**.h", "Sample/**.cpp" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		flags { "Symbols" }
	  
-- Savvy Standalone Project
project "Savvy Standalone"
	location "Savvy Standalone"
	kind "WindowedApp"
	language "C++"
	links { "Savvy" }
	includedirs { "Savvy/inc", "Savvy Standalone/inc", "ext/wxWidgets/include", "ext/wxWidgets/include/msvc", "ext/mcpp/include" }
	libdirs { "ext/wxWidgets/lib/vc_lib", "ext/mcpp/lib" }
	targetdir "Savvy Standalone/bin/%{cfg.buildcfg}"
	debugdir "Savvy Standalone/bin/%{cfg.buildcfg}"
	
	files { "Savvy Standalone/**.h", "Savvy Standalone/**.cpp" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		flags { "Symbols", "Unicode", "WinMain" }

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		flags { "Symbols", "Unicode", "WinMain" }