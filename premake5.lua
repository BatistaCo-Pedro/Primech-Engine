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
includeDir["glm"] = "PriMech/vendor/glm"
includeDir["stb_image"] = "PriMech/vendor/stb_image"

group "Dependencies"
	-- include the other premake5.lua
	include "PriMech/vendor/GLFW"
	include "PriMech/vendor/glad"
	include "PriMech/vendor/imgui"

group ""

project "PriMech" --Name of SLN
	location "PriMech" --Realtive Folder where to put files
	kind "StaticLib" 
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ppch.h"
	pchsource ("PriMech/src/ppch.cpp")

	files {
		"%{prj.name}/src/**.h", -- ** => recursively, find all files that end wiht .h
		"%{prj.name}/src/**.cpp", --all files that end with .cpp
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.imgui}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}",
	}

	links {
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"PM_PLATFORM_WINDOWS",
			"PRIMECH_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
		}
	
	filter "configurations:Debug"
		defines "PM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PM_Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PM_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "consoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
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
		systemversion "latest"

		defines {
			"PM_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "PM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PM_Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PM_DIST"
		runtime "Release"
		optimize "on"