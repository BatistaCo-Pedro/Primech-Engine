workspace "PriMech"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "PriMech" --Name of SLN
	location "PriMech" --Realtive Folder where to put files
	kind "SharedLib" --DLL
	language "C++"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h", -- ** = recursively, find all files that end wiht .h
		"%{prj.name}/src/**.cpp" --all files that end with .cpp
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines {
			"PM_PLATFORM_WINDOWS",
			"PRIMECH_BUILD_DLL",
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
	
	filter "configurations:Debug"
		defines "PM_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PM_Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PM_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "consoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h", -- ** = recursively, find all files that end wiht .h
		"%{prj.name}/src/**.cpp" --all files that end with .cpp
	}

	includedirs {
		"PriMech/vendor/spdlog/include",
		"PriMech/src"
	}

	links {
		"PriMech"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines {
			"PM_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "PM_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PM_Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PM_DIST"
		optimize "On"