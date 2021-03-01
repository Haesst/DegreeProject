workspace "DegreeProject"
	architecture "x86"

	configurations
	{
		"Debug",
		"Release"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

-- Include directories relative to solution directory
includedir = {}
includedir["SFML"] = "ThirdParty/SFML-2.5.1/include"

libdir = {}
libdir["SFML"] = "ThirdParty/SFML-2.5.1/lib"

sfmlbindir = "ThirdParty/SFML-2.5.1/bin"

project "DegreeProject"
	location "DegreeProject"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
	}

	defines
	{
		"SFML_DYNAMIC"
	}

	includedirs
	{
		"%{includedir.SFML}"
	}

	libdirs
	{
		"%{libdir.SFML}"
	}

	links
	{
		"sfml-graphics.lib",
		"sfml-window.lib",
		"sfml-system.lib",
		"sfml-audio.lib"
	}
	postbuildcommands
	{
		("{COPY} %{wks.location}%{sfmlbindir}/sfml-graphics-2.dll %{wks.location}bin/" .. outputdir .. "/%{prj.name}"),
		("{COPY} %{wks.location}%{sfmlbindir}/sfml-window-2.dll %{wks.location}bin/" .. outputdir .. "/%{prj.name}"),
		("{COPY} %{wks.location}%{sfmlbindir}/sfml-system-2.dll %{wks.location}bin/" .. outputdir .. "/%{prj.name}"),
		("{COPY} %{wks.location}%{sfmlbindir}/sfml-audio-2.dll %{wks.location}bin/" .. outputdir .. "/%{prj.name}"),
		("{COPY} %{wks.location}%{sfmlbindir}/openal32.dll %{wks.location}bin/" .. outputdir .. "/%{prj.name}")
	}