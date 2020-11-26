workspace "Hazel"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder (solution directory)
IncludeDir = {}
IncludeDir["ImGui"] = "Hazel/vendor/ImGui"

group "Dependencies"
	include "Hazel/vendor/ImGui"

group ""

project "Hazel"
	location "Hazel"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.inl",
		"%{prj.name}/src/**.hlsl",
		"%{prj.name}/resource.h",
		"%{prj.name}/Hazel.rc",
		"%{prj.name}/Hazel.aps"

	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.ImGui}"
	}

	links 
	{
		"ImGui"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox\"")
		}

		filter("files:**PixelShader.hlsl")
			shadertype("Pixel")
			shaderobjectfileoutput("PixelShader.cso")
			shadermodel("4.0")

		filter("files:**VertexShader.hlsl")
			shadertype("Vertex")
			shaderobjectfileoutput("VertexShader.cso")
			shadermodel("4.0")
					
		filter "configurations:Debug"
			defines 
			{
				"HZ_DEBUG"
			}

			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines 
			{
				"HZ_RELEASE",
				"NDEBUG"
			}
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines
			{
				"HZ_DIST",
				"NDEBUG"
			}
			runtime "Release"
			optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/resource.h",
		"%{prj.name}/Sandbox.rc",
		"%{prj.name}/Sandbox.aps"
	}

	includedirs
	{
		"Hazel/vendor/spdlog/include",
		"Hazel/src",
		"Hazel/vendor"
	}

	links 
	{
		"Hazel"
	}

	postbuildcommands
	{
		("{COPY} ../Hazel/PixelShader.cso  ../bin/" .. outputdir .. "/Hazel"),
		("{COPY} ../Hazel/VertexShader.cso ../bin/" .. outputdir .. "/Hazel")
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "HZ_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines
			{
				"HZ_RELEASE",
				"NDEBUG"
			}
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines
			{
				"HZ_DIST",
				"NDEBUG"
			}
			runtime "Release"
			optimize "on"