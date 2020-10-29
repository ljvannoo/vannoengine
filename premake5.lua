workspace "vannoengine"
	architecture "x64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "VannoEngine"
	location "VannoEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-temp/" .. outputdir .. "/%{prj.name}")

	files { 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp" 
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/freetype/include",
		"%{prj.name}/vendor/glew/include",
		"%{prj.name}/vendor/sdl/include",
		"%{prj.name}/vendor/stb_image/include",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/../vendor/rapidjson/include",
		"%{prj.name}/..//vendor/glm/include"
	}

	libdirs {
		"%{prj.name}/vendor/freetype/win64",
		"%{prj.name}/vendor/sdl/lib/x64",
		"%{prj.name}/vendor/glew/lib/Release/x64"
	}

	links {
		"SDL2",
		"opengl32",
		"user32",
		"gdi32",
		"shell32",
		"glew32s",
		"freetype"
	}

	filter "system:windows"
		staticruntime "on"
		systemversion "latest"
		
		defines { 
			"GLEW_STATIC",
			"ENGINE_PLATFORM_WINDOWS"
		}

		postbuildcommands {
			("xcopy /y \"%{wks.location}\\%{prj.name}\\vendor\\sdl\\lib\\x64\\SDL2.dll\" \"%{wks.location}\\bin\\" .. outputdir .. "\\Sandbox\\\""),
			("xcopy /y \"%{wks.location}\\%{prj.name}\\vendor\\freetype\\win64\\freetype.dll\" \"%{wks.location}\\bin\\" .. outputdir .. "\\Sandbox\\\""),
		}


	filter "configurations:Debug"
		defines "ENGINE_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "ENGINE_RELEASE"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-temp/" .. outputdir .. "/%{prj.name}")

	files { 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp" 
	}

	includedirs {
		"%{prj.name}/src",
		"VannoEngine/vendor/spdlog/include",
		"VannoEngine/src",
		"%{prj.name}/../vendor/rapidjson/include",
		"%{prj.name}/..//vendor/glm/include"
	}

	links {
		"VannoEngine"
	}

	filter "system:windows"
		staticruntime "on"
		systemversion "latest"
		
		defines { 
			"ENGINE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "ENGINE_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "ENGINE_RELEASE"
		optimize "on"