

project "omnibot_et"
	targetname  "omnibot_et"
	language    "C++"
	kind        "SharedLib"
	flags       { "ExtraWarnings" }
	files
	{
		"libs/gmsrc_ex/src/gm/*.cpp", "libs/gmsrc_ex/src/gm/*.h",
		--"libs/gmsrc_ex/src/platform/win32gcc/*.cpp", "libs/gmsrc_ex/src/platform/win32gcc/*.h",
		"libs/gmsrc_ex/src/binds/*.cpp", "libs/gmsrc_ex/src/binds/*.h",
		--"libs/gmsrc_ex/src/binds/gmSqliteLib.cpp",
		"libs/gmsrc_ex/src/3rdParty/mathlib/*.cpp", "libs/gmsrc_ex/src/3rdParty/mathlib/*.h",
		"libs/gmsrc_ex/src/3rdParty/gmbinder2/*.cpp", "libs/gmsrc_ex/src/3rdParty/gmbinder2/*.h",
		
		"libs/wildmagic/*.cpp", "libs/wildmagic/*.h",
		
		"libs/physfs/*.c", "libs/physfs/*.h", 
		"libs/physfs/archivers/*.c", 
		"libs/physfs/lzma/C/*.c", 
		"libs/physfs/lzma/C/Archive/7z/*.c", "libs/physfs/lzma/C/Archive/7z/*.h", 
		"libs/physfs/lzma/C/Compress/Lzma/*.c", "libs/physfs/lzma/C/Compress/Lzma/*.h", 
		"libs/physfs/lzma/C/Compress/Branch/*.c", "libs/physfs/lzma/C/Compress/Branch/*.h", 
		"libs/physfs/zlib123/*.c", "libs/physfs/zlib123/*.h", 
		
		"libs/recast/Recast/Source/*.cpp", "libs/recast/Recast/Source/*.h", 
		"libs/recast/Detour/Source/*.cpp", "libs/recast/Detour/Source/*.h",
		"libs/recast/DebugUtils/Source/*.cpp", "libs/recast/DebugUtils/Source/*.h",
		
		"libs/iprof/prof_draw.c",
		"libs/iprof/prof_gather.c",
		"libs/iprof/prof_process.c",
		"libs/iprof/prof_gather.h", 
		"libs/iprof/prof_gather.h", 
		
		--"libs/pcre-8.10/*.c", 
		--"libs/pcre-8.10/*.cpp", 		  
		--"libs/sqlite3/*.c",
		
		--"Common/ObstacleManager.cpp",
		--"Common/RecastInterfaces.cpp",
		--"Common/BatchBuild.cpp", 
		--"ET/ET_BatchBuild.cpp",
		
		"Common/*.cpp", "Common/*.h",
		"ET/*.cpp", "ET/*.h",
	}
	excludes
	{
		"Common/BatchBuild.cpp",
		"ET/ET_BatchBuild.cpp",
	}
	includedirs
	{
		"Common",
		"ET",
		
		"libs/iprof",
		"libs/wildmagic",
		"libs/gmsrc_ex/src/3rdParty/mathlib",
		"libs/gmsrc_ex/src/gm",
		"libs/gmsrc_ex/src/binds",
		"libs/gmsrc_ex/src/3rdParty/gmbinder2",
		
		"libs/recast/Recast/Include",
		"libs/recast/DebugUtils/Include",
		"libs/recast/Detour/Include",
		"libs/physfs",
		"libs/physfs/lzma/C",
		"libs/physfs/zlib123",
		"libs/physfs/lzma/C/Archive/7z",
		"libs/physfs/lzma/C/Compress/Lzma",
		"libs/physfs/lzma/C/Compress/Branch",
	}
	defines
	{ 
		"PHYSFS_SUPPORTS_ZIP=1",
		"PHYSFS_SUPPORTS_7Z=1",
		"PHYSFS_NO_CDROM_SUPPORT",
	}
	
	--
	-- Platform Configurations
	--
	--configuration "x32"
	--	targetdir 	"../../bin32"
	
	--configuration "x64"
	--	targetdir 	"../../bin64"
				
	-- 
	-- Project Configurations
	-- 
	configuration "vs*"
		files
		{
			"libs/gmsrc_ex/src/platform/win32msvc/*.cpp", "libs/gmsrc_ex/src/platform/win32msvc/*.h",
			
			"libs/physfs/platform/windows.c", 
			
			"libs/iprof/prof_win32.c", 
			"libs/iprof/prof_win32.h",
		}
		includedirs
		{
			"C:/boost/boost_1_44_0",
			"libs/gmsrc_ex/src/platform/win32msvc"
		}
		libdirs
		{
			"C:/boost/boost_1_44_0/stage/lib"
		}
		--buildoptions
		--{
		--	"/fp:fast",
		--	"/arch:SSE"
		--}
		defines
		{
			"WIN32",
			"_CRT_SECURE_NO_WARNINGS",
		}
		
	configuration { "vs*", "x32" }
		targetdir 	"../../bin/win32"
		
	configuration { "vs*", "x64" }
		targetdir 	"../../bin/win64"
	
	configuration "linux"
		targetprefix ""
		files
		{
			"libs/gmsrc_ex/src/platform/win32gcc/*.cpp",
			"libs/gmsrc_ex/src/platform/win32gcc/*.h",
			
			"libs/physfs/platform/unix.c",
			"libs/physfs/platform/posix.c",
			
			"libs/iprof/prof_unix.c",
			"libs/iprof/prof_unix.h",
		}
		includedirs
		{
			--"/projects/boost/boost_1_44_0",
			"libs/gmsrc_ex/src/platform/win32gcc",
		}
		--libdirs
		--{
		--	"/projects/boost/boost_1_44_0/stage/lib"
		--}
		buildoptions
		{
			"-pthread",
			"-lrt",
		}
		links
		{
			--"boost_date_time",
			"boost_filesystem",
			--"boost_regex",
			"boost_system",
			--"boost_thread",
		}	

	configuration { "linux", "x32" }
		targetdir 	"../../bin/linux-x86"
		
	configuration { "linux", "x64" }
		targetdir 	"../../bin/linux-x86_64"
	
