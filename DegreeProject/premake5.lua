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
includedir["SpdLog"] = "ThirdParty/spdlog/include"
includedir["json"] = "ThirdParty/json-develop/include/nlohmann"

libdir = {}
libdir["SFML"] = "ThirdParty/SFML-2.5.1/lib"

sfmlbindir = "ThirdParty/SFML-2.5.1/bin"

project "DegreeProject"
	location "DegreeProject"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	warnings "Extra"
	flags "MultiProcessorCompile"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
	}

	defines
	{
		"SFML_DYNAMIC",
		"ENABLE_ASSERTS",
	}

	includedirs
	{
		"%{includedir.SFML}",
		"%{includedir.SpdLog}",
		"%{includedir.json}",
		"%{prj.location}/Source",
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
		"sfml-audio.lib",
	}
	postbuildcommands
	{
		("{COPY} %{wks.location}%{sfmlbindir}/sfml-graphics-2.dll %{wks.location}bin/" .. outputdir .. "/%{prj.name}"),
		("{COPY} %{wks.location}%{sfmlbindir}/sfml-window-2.dll %{wks.location}bin/" .. outputdir .. "/%{prj.name}"),
		("{COPY} %{wks.location}%{sfmlbindir}/sfml-system-2.dll %{wks.location}bin/" .. outputdir .. "/%{prj.name}"),
		("{COPY} %{wks.location}%{sfmlbindir}/sfml-audio-2.dll %{wks.location}bin/" .. outputdir .. "/%{prj.name}"),
		("{COPY} %{wks.location}%{sfmlbindir}/openal32.dll %{wks.location}bin/" .. outputdir .. "/%{prj.name}")
	}
