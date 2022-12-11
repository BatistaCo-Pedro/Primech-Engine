workspace "PriMech"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (SLN)
includeDir = {}
includeDir["GLFW"] = "PriMech/vendor/GLFW/include"
includeDir["glad"] = "PriMech/vendor/glad/include"
includeDir["imgui"] = "PriMech/vendor/imgui"

group "Dependencies"
	-- include the other premake5.lua
	include "PriMech/vendor/GLFW"
	include "PriMech/vendor/glad"
	include "PriMech/vendor/imgui"

group ""

project "PriMech" --Name of SLN
	location "PriMech" --Realtive Folder where to put files
	kind "SharedLib" 
	language "C++"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ppch.h"
	pchsource ("PriMech/src/ppch.cpp")

	files {
		"%{prj.name}/src/**.h", -- ** => recursively, find all files that end wiht .h
		"%{prj.name}/src/**.cpp" --all files that end with .cpp
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.imgui}",
	}

	links {
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

		defines {
			"PM_PLATFORM_WINDOWS",
			"PRIMECH_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}
	
	filter "configurations:Debug"
		defines "PM_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PM_Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PM_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "consoleApp"
	language "C++"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h", -- ** = recursively, find all files that end wiht .h
		"%{prj.name}/src/**.cpp" --all files that end with .cpp
	}

	includedirs {
		"PriMech/vendor/spdlog/include",
		"PriMech/src",
		"%{includeDir.glm}",
		"PriMech/vendor",
	}

	links {
		"PriMech"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

		defines {
			"PM_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "PM_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PM_Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PM_DIST"
		runtime "Release"
		optimize "On"