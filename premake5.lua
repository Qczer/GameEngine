workspace "GameEngine"
	architecture "x86_64"
	startproject "GameEngine-Editor"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/GameEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/GameEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/GameEngine/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/GameEngine/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/GameEngine/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/GameEngine/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/GameEngine/vendor/yaml-cpp/include"

group "Dependencies"
	include "GameEngine/vendor/GLFW"
	include "GameEngine/vendor/Glad"
	include "GameEngine/vendor/imgui"
	include "GameEngine/vendor/yaml-cpp"
group ""

project "GameEngine"
	location "GameEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"


	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gepch.h"
	pchsource "GameEngine/src/gepch.cpp"

	files
	{
		"%{prj.name}/src/*.cpp",
		"%{prj.name}/src/*.h",
		"%{prj.name}/src/GameEngine/**.h",
		"%{prj.name}/src/GameEngine/**.cpp",
		"%{prj.name}/src/Platform/OpenGL/**.h",
		"%{prj.name}/src/Platform/OpenGL/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "system:windows"
		systemversion "latest"

		links
		{
			"opengl32.lib"
		}

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

		files
		{
			"%{prj.name}/src/Platform/Windows/**.h",
			"%{prj.name}/src/Platform/Windows/**.cpp",
		}

	filter "system:linux"
		links
		{
			"GL",
			"pthread",
			"dl",
			"GLFW",
			"Glad",
			"ImGui",
            "X11",
            "Xrandr",
            "Xi",
            "Xxf86vm",
            "Xinerama",
            "Xcursor"
		}

		files
		{
			"%{prj.name}/src/Platform/Linux/**.h",
			"%{prj.name}/src/Platform/Linux/**.cpp",
		}

	filter "configurations:Debug"
		defines "GE_BUILD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GE_BUILD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GE_BUILD_DIST"
		runtime "Release"
		optimize "on"

	filter "action:vs*"
		buildoptions { "/utf-8" }

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"GameEngine/vendor/spdlog/include",
		"GameEngine/src",
		"GameEngine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"GameEngine",
		"GLFW",
        "Glad",
        "ImGui",
		"yaml-cpp"
	}

	filter "system:windows"
		systemversion "latest"
        postbuildcommands
        {
            "{COPYDIR} \"assets\" \"%{cfg.targetdir}/assets\""
        }

    filter "system:linux"
		links
		{
			"X11",
			"Xrandr",
			"Xi",
			"Xxf86vm",
			"Xinerama",
			"Xcursor"
		}
        postbuildcommands
        {
            "mkdir -p %{cfg.targetdir}/assets && cp -r assets/* %{cfg.targetdir}/assets/"
        }

	filter "configurations:Debug"
		defines "GE_BUILD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GE_BUILD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GE_BUILD_DIST"
		runtime "Release"
		optimize "on"

	filter "action:vs*"
		buildoptions { "/utf-8" }

project "GameEngine-Editor"
	location "GameEngine-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"GameEngine/vendor/spdlog/include",
		"GameEngine/src",
		"GameEngine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"GameEngine",
		"GLFW",
        "Glad",
        "ImGui",
		"yaml-cpp"
	}

	filter "system:windows"
		systemversion "latest"
		postbuildcommands
        {
            "{COPYDIR} \"assets\" \"%{cfg.targetdir}/assets\""
        }

    filter "system:linux"
		links
		{
			"X11",
			"Xrandr",
			"Xi",
			"Xxf86vm",
			"Xinerama",
			"Xcursor"
		}
        postbuildcommands
        {
            "mkdir -p %{cfg.targetdir}/assets && cp -r assets/* %{cfg.targetdir}/assets/"
        }

	filter "configurations:Debug"
		defines "GE_BUILD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GE_BUILD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GE_BUILD_DIST"
		runtime "Release"
		optimize "on"

	filter "action:vs*"
		buildoptions { "/utf-8" }
