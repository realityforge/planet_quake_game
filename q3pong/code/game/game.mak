# Microsoft Developer Studio Generated NMAKE File, Based on game.dsp
!IF "$(CFG)" == ""
CFG=game - Win32 Debug
!MESSAGE No configuration specified. Defaulting to game - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "game - Win32 Release" && "$(CFG)" != "game - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "game.mak" CFG="game - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "game - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "game - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "game - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\Release\qagamex86.dll"


CLEAN :
	-@erase "$(INTDIR)\ai_chat.obj"
	-@erase "$(INTDIR)\ai_cmd.obj"
	-@erase "$(INTDIR)\ai_dmnet.obj"
	-@erase "$(INTDIR)\ai_dmq3.obj"
	-@erase "$(INTDIR)\ai_main.obj"
	-@erase "$(INTDIR)\ai_team.obj"
	-@erase "$(INTDIR)\ai_vcmd.obj"
	-@erase "$(INTDIR)\bg_misc.obj"
	-@erase "$(INTDIR)\bg_pmove.obj"
	-@erase "$(INTDIR)\bg_slidemove.obj"
	-@erase "$(INTDIR)\g_active.obj"
	-@erase "$(INTDIR)\g_arenas.obj"
	-@erase "$(INTDIR)\g_bot.obj"
	-@erase "$(INTDIR)\g_client.obj"
	-@erase "$(INTDIR)\g_cmds.obj"
	-@erase "$(INTDIR)\g_combat.obj"
	-@erase "$(INTDIR)\g_items.obj"
	-@erase "$(INTDIR)\g_main.obj"
	-@erase "$(INTDIR)\g_mem.obj"
	-@erase "$(INTDIR)\g_misc.obj"
	-@erase "$(INTDIR)\g_missile.obj"
	-@erase "$(INTDIR)\g_mover.obj"
	-@erase "$(INTDIR)\g_q3pong.obj"
	-@erase "$(INTDIR)\g_session.obj"
	-@erase "$(INTDIR)\g_spawn.obj"
	-@erase "$(INTDIR)\g_svcmds.obj"
	-@erase "$(INTDIR)\g_syscalls.obj"
	-@erase "$(INTDIR)\g_target.obj"
	-@erase "$(INTDIR)\g_team.obj"
	-@erase "$(INTDIR)\g_trigger.obj"
	-@erase "$(INTDIR)\g_utils.obj"
	-@erase "$(INTDIR)\g_weapon.obj"
	-@erase "$(INTDIR)\q_math.obj"
	-@erase "$(INTDIR)\q_shared.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\qagamex86.exp"
	-@erase "$(OUTDIR)\qagamex86.lib"
	-@erase "$(OUTDIR)\qagamex86.map"
	-@erase "..\Release\qagamex86.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /ML /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\game.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\game.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\qagamex86.pdb" /map:"$(INTDIR)\qagamex86.map" /machine:I386 /def:".\game.def" /out:"..\Release/qagamex86.dll" /implib:"$(OUTDIR)\qagamex86.lib" 
DEF_FILE= \
	".\game.def"
LINK32_OBJS= \
	"$(INTDIR)\ai_chat.obj" \
	"$(INTDIR)\ai_cmd.obj" \
	"$(INTDIR)\ai_dmnet.obj" \
	"$(INTDIR)\ai_dmq3.obj" \
	"$(INTDIR)\ai_main.obj" \
	"$(INTDIR)\ai_team.obj" \
	"$(INTDIR)\bg_misc.obj" \
	"$(INTDIR)\bg_pmove.obj" \
	"$(INTDIR)\bg_slidemove.obj" \
	"$(INTDIR)\g_active.obj" \
	"$(INTDIR)\g_arenas.obj" \
	"$(INTDIR)\g_bot.obj" \
	"$(INTDIR)\g_client.obj" \
	"$(INTDIR)\g_cmds.obj" \
	"$(INTDIR)\g_combat.obj" \
	"$(INTDIR)\g_items.obj" \
	"$(INTDIR)\g_main.obj" \
	"$(INTDIR)\g_mem.obj" \
	"$(INTDIR)\g_misc.obj" \
	"$(INTDIR)\g_missile.obj" \
	"$(INTDIR)\g_mover.obj" \
	"$(INTDIR)\g_q3pong.obj" \
	"$(INTDIR)\g_session.obj" \
	"$(INTDIR)\g_spawn.obj" \
	"$(INTDIR)\g_svcmds.obj" \
	"$(INTDIR)\g_syscalls.obj" \
	"$(INTDIR)\g_target.obj" \
	"$(INTDIR)\g_team.obj" \
	"$(INTDIR)\g_trigger.obj" \
	"$(INTDIR)\g_utils.obj" \
	"$(INTDIR)\g_weapon.obj" \
	"$(INTDIR)\q_math.obj" \
	"$(INTDIR)\q_shared.obj" \
	"$(INTDIR)\ai_vcmd.obj"

"..\Release\qagamex86.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\Debug\qagamex86.dll" "$(OUTDIR)\game.bsc"


CLEAN :
	-@erase "$(INTDIR)\ai_chat.obj"
	-@erase "$(INTDIR)\ai_chat.sbr"
	-@erase "$(INTDIR)\ai_cmd.obj"
	-@erase "$(INTDIR)\ai_cmd.sbr"
	-@erase "$(INTDIR)\ai_dmnet.obj"
	-@erase "$(INTDIR)\ai_dmnet.sbr"
	-@erase "$(INTDIR)\ai_dmq3.obj"
	-@erase "$(INTDIR)\ai_dmq3.sbr"
	-@erase "$(INTDIR)\ai_main.obj"
	-@erase "$(INTDIR)\ai_main.sbr"
	-@erase "$(INTDIR)\ai_team.obj"
	-@erase "$(INTDIR)\ai_team.sbr"
	-@erase "$(INTDIR)\ai_vcmd.obj"
	-@erase "$(INTDIR)\ai_vcmd.sbr"
	-@erase "$(INTDIR)\bg_misc.obj"
	-@erase "$(INTDIR)\bg_misc.sbr"
	-@erase "$(INTDIR)\bg_pmove.obj"
	-@erase "$(INTDIR)\bg_pmove.sbr"
	-@erase "$(INTDIR)\bg_slidemove.obj"
	-@erase "$(INTDIR)\bg_slidemove.sbr"
	-@erase "$(INTDIR)\g_active.obj"
	-@erase "$(INTDIR)\g_active.sbr"
	-@erase "$(INTDIR)\g_arenas.obj"
	-@erase "$(INTDIR)\g_arenas.sbr"
	-@erase "$(INTDIR)\g_bot.obj"
	-@erase "$(INTDIR)\g_bot.sbr"
	-@erase "$(INTDIR)\g_client.obj"
	-@erase "$(INTDIR)\g_client.sbr"
	-@erase "$(INTDIR)\g_cmds.obj"
	-@erase "$(INTDIR)\g_cmds.sbr"
	-@erase "$(INTDIR)\g_combat.obj"
	-@erase "$(INTDIR)\g_combat.sbr"
	-@erase "$(INTDIR)\g_items.obj"
	-@erase "$(INTDIR)\g_items.sbr"
	-@erase "$(INTDIR)\g_main.obj"
	-@erase "$(INTDIR)\g_main.sbr"
	-@erase "$(INTDIR)\g_mem.obj"
	-@erase "$(INTDIR)\g_mem.sbr"
	-@erase "$(INTDIR)\g_misc.obj"
	-@erase "$(INTDIR)\g_misc.sbr"
	-@erase "$(INTDIR)\g_missile.obj"
	-@erase "$(INTDIR)\g_missile.sbr"
	-@erase "$(INTDIR)\g_mover.obj"
	-@erase "$(INTDIR)\g_mover.sbr"
	-@erase "$(INTDIR)\g_q3pong.obj"
	-@erase "$(INTDIR)\g_q3pong.sbr"
	-@erase "$(INTDIR)\g_session.obj"
	-@erase "$(INTDIR)\g_session.sbr"
	-@erase "$(INTDIR)\g_spawn.obj"
	-@erase "$(INTDIR)\g_spawn.sbr"
	-@erase "$(INTDIR)\g_svcmds.obj"
	-@erase "$(INTDIR)\g_svcmds.sbr"
	-@erase "$(INTDIR)\g_syscalls.obj"
	-@erase "$(INTDIR)\g_syscalls.sbr"
	-@erase "$(INTDIR)\g_target.obj"
	-@erase "$(INTDIR)\g_target.sbr"
	-@erase "$(INTDIR)\g_team.obj"
	-@erase "$(INTDIR)\g_team.sbr"
	-@erase "$(INTDIR)\g_trigger.obj"
	-@erase "$(INTDIR)\g_trigger.sbr"
	-@erase "$(INTDIR)\g_utils.obj"
	-@erase "$(INTDIR)\g_utils.sbr"
	-@erase "$(INTDIR)\g_weapon.obj"
	-@erase "$(INTDIR)\g_weapon.sbr"
	-@erase "$(INTDIR)\q_math.obj"
	-@erase "$(INTDIR)\q_math.sbr"
	-@erase "$(INTDIR)\q_shared.obj"
	-@erase "$(INTDIR)\q_shared.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\game.bsc"
	-@erase "$(OUTDIR)\qagamex86.exp"
	-@erase "$(OUTDIR)\qagamex86.lib"
	-@erase "$(OUTDIR)\qagamex86.map"
	-@erase "$(OUTDIR)\qagamex86.pdb"
	-@erase "..\Debug\qagamex86.dll"
	-@erase "..\Debug\qagamex86.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "BUILDING_REF_GL" /D "DEBUG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\game.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\game.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ai_chat.sbr" \
	"$(INTDIR)\ai_cmd.sbr" \
	"$(INTDIR)\ai_dmnet.sbr" \
	"$(INTDIR)\ai_dmq3.sbr" \
	"$(INTDIR)\ai_main.sbr" \
	"$(INTDIR)\ai_team.sbr" \
	"$(INTDIR)\bg_misc.sbr" \
	"$(INTDIR)\bg_pmove.sbr" \
	"$(INTDIR)\bg_slidemove.sbr" \
	"$(INTDIR)\g_active.sbr" \
	"$(INTDIR)\g_arenas.sbr" \
	"$(INTDIR)\g_bot.sbr" \
	"$(INTDIR)\g_client.sbr" \
	"$(INTDIR)\g_cmds.sbr" \
	"$(INTDIR)\g_combat.sbr" \
	"$(INTDIR)\g_items.sbr" \
	"$(INTDIR)\g_main.sbr" \
	"$(INTDIR)\g_mem.sbr" \
	"$(INTDIR)\g_misc.sbr" \
	"$(INTDIR)\g_missile.sbr" \
	"$(INTDIR)\g_mover.sbr" \
	"$(INTDIR)\g_q3pong.sbr" \
	"$(INTDIR)\g_session.sbr" \
	"$(INTDIR)\g_spawn.sbr" \
	"$(INTDIR)\g_svcmds.sbr" \
	"$(INTDIR)\g_syscalls.sbr" \
	"$(INTDIR)\g_target.sbr" \
	"$(INTDIR)\g_team.sbr" \
	"$(INTDIR)\g_trigger.sbr" \
	"$(INTDIR)\g_utils.sbr" \
	"$(INTDIR)\g_weapon.sbr" \
	"$(INTDIR)\q_math.sbr" \
	"$(INTDIR)\q_shared.sbr" \
	"$(INTDIR)\ai_vcmd.sbr"

"$(OUTDIR)\game.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib winmm.lib /nologo /base:"0x20000000" /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\qagamex86.pdb" /map:"$(INTDIR)\qagamex86.map" /debug /machine:I386 /def:".\game.def" /out:"..\Debug/qagamex86.dll" /implib:"$(OUTDIR)\qagamex86.lib" 
DEF_FILE= \
	".\game.def"
LINK32_OBJS= \
	"$(INTDIR)\ai_chat.obj" \
	"$(INTDIR)\ai_cmd.obj" \
	"$(INTDIR)\ai_dmnet.obj" \
	"$(INTDIR)\ai_dmq3.obj" \
	"$(INTDIR)\ai_main.obj" \
	"$(INTDIR)\ai_team.obj" \
	"$(INTDIR)\bg_misc.obj" \
	"$(INTDIR)\bg_pmove.obj" \
	"$(INTDIR)\bg_slidemove.obj" \
	"$(INTDIR)\g_active.obj" \
	"$(INTDIR)\g_arenas.obj" \
	"$(INTDIR)\g_bot.obj" \
	"$(INTDIR)\g_client.obj" \
	"$(INTDIR)\g_cmds.obj" \
	"$(INTDIR)\g_combat.obj" \
	"$(INTDIR)\g_items.obj" \
	"$(INTDIR)\g_main.obj" \
	"$(INTDIR)\g_mem.obj" \
	"$(INTDIR)\g_misc.obj" \
	"$(INTDIR)\g_missile.obj" \
	"$(INTDIR)\g_mover.obj" \
	"$(INTDIR)\g_q3pong.obj" \
	"$(INTDIR)\g_session.obj" \
	"$(INTDIR)\g_spawn.obj" \
	"$(INTDIR)\g_svcmds.obj" \
	"$(INTDIR)\g_syscalls.obj" \
	"$(INTDIR)\g_target.obj" \
	"$(INTDIR)\g_team.obj" \
	"$(INTDIR)\g_trigger.obj" \
	"$(INTDIR)\g_utils.obj" \
	"$(INTDIR)\g_weapon.obj" \
	"$(INTDIR)\q_math.obj" \
	"$(INTDIR)\q_shared.obj" \
	"$(INTDIR)\ai_vcmd.obj"

"..\Debug\qagamex86.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("game.dep")
!INCLUDE "game.dep"
!ELSE 
!MESSAGE Warning: cannot find "game.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "game - Win32 Release" || "$(CFG)" == "game - Win32 Debug"
SOURCE=.\ai_chat.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\ai_chat.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\ai_chat.obj"	"$(INTDIR)\ai_chat.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ai_cmd.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\ai_cmd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\ai_cmd.obj"	"$(INTDIR)\ai_cmd.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ai_dmnet.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\ai_dmnet.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\ai_dmnet.obj"	"$(INTDIR)\ai_dmnet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ai_dmq3.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\ai_dmq3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\ai_dmq3.obj"	"$(INTDIR)\ai_dmq3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ai_main.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\ai_main.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\ai_main.obj"	"$(INTDIR)\ai_main.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ai_team.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\ai_team.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\ai_team.obj"	"$(INTDIR)\ai_team.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ai_vcmd.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\ai_vcmd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\ai_vcmd.obj"	"$(INTDIR)\ai_vcmd.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bg_lib.c
SOURCE=.\bg_misc.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\bg_misc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\bg_misc.obj"	"$(INTDIR)\bg_misc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bg_pmove.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\bg_pmove.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\bg_pmove.obj"	"$(INTDIR)\bg_pmove.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bg_slidemove.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\bg_slidemove.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\bg_slidemove.obj"	"$(INTDIR)\bg_slidemove.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_active.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_active.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_active.obj"	"$(INTDIR)\g_active.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_arenas.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_arenas.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_arenas.obj"	"$(INTDIR)\g_arenas.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_bot.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_bot.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_bot.obj"	"$(INTDIR)\g_bot.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_client.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_client.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_client.obj"	"$(INTDIR)\g_client.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_cmds.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_cmds.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_cmds.obj"	"$(INTDIR)\g_cmds.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_combat.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_combat.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_combat.obj"	"$(INTDIR)\g_combat.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_items.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_items.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_items.obj"	"$(INTDIR)\g_items.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_main.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_main.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_main.obj"	"$(INTDIR)\g_main.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_mem.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_mem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_mem.obj"	"$(INTDIR)\g_mem.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_misc.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_misc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_misc.obj"	"$(INTDIR)\g_misc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_missile.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_missile.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_missile.obj"	"$(INTDIR)\g_missile.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_mover.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_mover.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_mover.obj"	"$(INTDIR)\g_mover.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_q3pong.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_q3pong.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_q3pong.obj"	"$(INTDIR)\g_q3pong.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_session.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_session.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_session.obj"	"$(INTDIR)\g_session.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_spawn.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_spawn.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_spawn.obj"	"$(INTDIR)\g_spawn.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_svcmds.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_svcmds.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_svcmds.obj"	"$(INTDIR)\g_svcmds.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_syscalls.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_syscalls.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_syscalls.obj"	"$(INTDIR)\g_syscalls.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_target.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_target.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_target.obj"	"$(INTDIR)\g_target.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_team.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_team.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_team.obj"	"$(INTDIR)\g_team.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_trigger.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_trigger.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_trigger.obj"	"$(INTDIR)\g_trigger.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_utils.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_utils.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_utils.obj"	"$(INTDIR)\g_utils.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\g_weapon.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\g_weapon.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\g_weapon.obj"	"$(INTDIR)\g_weapon.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\q_math.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\q_math.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\q_math.obj"	"$(INTDIR)\q_math.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\q_shared.c

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\q_shared.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\q_shared.obj"	"$(INTDIR)\q_shared.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

