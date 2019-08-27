# Microsoft Developer Studio Project File - Name="cgame" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cgame - Win32 Debug_qvm
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cgame.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cgame.mak" CFG="cgame - Win32 Debug_qvm"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cgame - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cgame - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cgame - Win32 Release TA" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cgame - Win32 Debug TA" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cgame - Win32 Debug_qvm" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cgame - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /base:"0x30000000" /subsystem:windows /dll /map /machine:I386 /out:"../../cgamex86.dll"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CGAME" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /base:"0x30000000" /subsystem:windows /dll /profile /map:"../../cgamex86.map" /debug /machine:I386 /out:"../../cgamex86.dll"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "cgame - Win32 Release TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "cgame___Win32_Release_TA"
# PROP BASE Intermediate_Dir "cgame___Win32_Release_TA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_TA"
# PROP Intermediate_Dir "Release_TA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "MISSIONPACK" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /base:"0x30000000" /subsystem:windows /dll /map /machine:I386 /out:"../Release/cgamex86.dll"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 /nologo /base:"0x30000000" /subsystem:windows /dll /map /machine:I386 /out:"../../cgamex86.dll"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "cgame___Win32_Debug_TA"
# PROP BASE Intermediate_Dir "cgame___Win32_Debug_TA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_TA"
# PROP Intermediate_Dir "Debug_TA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "MISSIONPACK" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /base:"0x30000000" /subsystem:windows /dll /map /debug /machine:I386 /out:"..\Debug/cgamex86.dll"
# SUBTRACT BASE LINK32 /profile /nodefaultlib
# ADD LINK32 /nologo /base:"0x30000000" /subsystem:windows /dll /map /debug /machine:I386 /def:".\cgame.def" /out:"..\..\cgamex86.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug_qvm"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "cgame_qvm"
# PROP BASE Intermediate_Dir "cgame_qvm"
# PROP BASE Cmd_Line "NMAKE /f cgame_qvm.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "cgame_qvm.exe"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "cgame_qvm"
# PROP Intermediate_Dir "cgame_qvm"
# PROP Cmd_Line "vc_qvm cgame_qvm.dsp "../vm/cgame" ../cgame/cg_syscalls cg_main -D_DEBUG -DQ3_VM -DCGAME -S -Wf-target=bytecode -Wf-g -I../cgame -I../game -I../q3_ui -I../ui"
# PROP Rebuild_Opt "/a"
# PROP Target_File "cgame.qvm"
# PROP Bsc_Name ""
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /D "WIN32" /D "DEBUG" /FR
# ADD BASE RSC /l 0x1009
# ADD RSC /l 0x1009
BSC32=bscmake.exe
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 /machine:IX86

!ENDIF 

# Begin Target

# Name "cgame - Win32 Release"
# Name "cgame - Win32 Debug"
# Name "cgame - Win32 Release TA"
# Name "cgame - Win32 Debug TA"
# Name "cgame - Win32 Debug_qvm"
# Begin Group "Source Files"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\game\bg_lib.c

!IF  "$(CFG)" == "cgame - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cgame - Win32 Release TA"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug_qvm"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\game\bg_misc.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_physics.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_pmove.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_slidemove.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_wheel_forces.c
# End Source File
# Begin Source File

SOURCE=.\cg_atmospheric.c
# End Source File
# Begin Source File

SOURCE=.\cg_consolecmds.c
# End Source File
# Begin Source File

SOURCE=.\cg_draw.c
# End Source File
# Begin Source File

SOURCE=.\cg_drawtools.c
# End Source File
# Begin Source File

SOURCE=.\cg_effects.c
# End Source File
# Begin Source File

SOURCE=.\cg_ents.c
# End Source File
# Begin Source File

SOURCE=.\cg_event.c
# End Source File
# Begin Source File

SOURCE=.\cg_info.c
# End Source File
# Begin Source File

SOURCE=.\cg_localents.c
# End Source File
# Begin Source File

SOURCE=.\cg_main.c
# End Source File
# Begin Source File

SOURCE=.\cg_marks.c
# End Source File
# Begin Source File

SOURCE=.\cg_newDraw.c

!IF  "$(CFG)" == "cgame - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cgame - Win32 Release TA"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug_qvm"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_players.c
# End Source File
# Begin Source File

SOURCE=.\cg_playerstate.c
# End Source File
# Begin Source File

SOURCE=.\cg_predict.c
# End Source File
# Begin Source File

SOURCE=.\cg_rally_draw.c
# End Source File
# Begin Source File

SOURCE=.\cg_rally_drawtools.c
# End Source File
# Begin Source File

SOURCE=.\cg_rally_hud.c
# End Source File
# Begin Source File

SOURCE=.\cg_rally_hud2.c
# End Source File
# Begin Source File

SOURCE=.\cg_rally_platetools.c
# End Source File
# Begin Source File

SOURCE=.\cg_rally_racetools.c
# End Source File
# Begin Source File

SOURCE=.\cg_rally_rearweapons.c
# End Source File
# Begin Source File

SOURCE=.\cg_rally_scripted_objects.c
# End Source File
# Begin Source File

SOURCE=.\cg_rally_tools.c
# End Source File
# Begin Source File

SOURCE=.\cg_scoreboard.c
# End Source File
# Begin Source File

SOURCE=.\cg_servercmds.c
# End Source File
# Begin Source File

SOURCE=.\cg_snapshot.c
# End Source File
# Begin Source File

SOURCE=.\cg_syscalls.c
# End Source File
# Begin Source File

SOURCE=.\cg_view.c
# End Source File
# Begin Source File

SOURCE=.\cg_weapons.c
# End Source File
# Begin Source File

SOURCE=..\game\q_math.c
# End Source File
# Begin Source File

SOURCE=..\game\q_shared.c
# End Source File
# Begin Source File

SOURCE=..\ui\ui_shared.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\Game\Bg_lib.h

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cgame - Win32 Release TA"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug_qvm"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\game\bg_physics.h
# End Source File
# Begin Source File

SOURCE=..\game\bg_public.h
# End Source File
# Begin Source File

SOURCE=.\cg_local.h
# End Source File
# Begin Source File

SOURCE=.\cg_public.h
# End Source File
# Begin Source File

SOURCE=..\game\q_shared.h
# End Source File
# Begin Source File

SOURCE=..\game\surfaceflags.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cgame.def

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 Release TA"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug TA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug_qvm"

!ENDIF 

# End Source File
# End Target
# End Project
