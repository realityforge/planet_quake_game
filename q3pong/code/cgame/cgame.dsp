# Microsoft Developer Studio Project File - Name="cgame" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cgame - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cgame.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cgame.mak" CFG="cgame - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cgame - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cgame - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cgame - Win32 QVM" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MissionPack/code/cgame", NPAAAAAA"
# PROP Scc_LocalPath "."
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
# ADD LINK32 /nologo /base:"0x30000000" /subsystem:windows /dll /map /machine:I386 /out:"../Release/cgamex86.dll"
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
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /base:"0x30000000" /subsystem:windows /dll /map /debug /machine:I386 /out:"..\Debug/cgamex86.dll"
# SUBTRACT LINK32 /profile /nodefaultlib

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "cgame___Win32_QVM"
# PROP BASE Intermediate_Dir "cgame___Win32_QVM"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vm"
# PROP Intermediate_Dir "vm"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
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
# ADD LINK32 /nologo /pdb:none /machine:IX86
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Generating cgame.qvm
PostBuild_Cmds=cd vm	C:\Quake3\bin_nt\q3asm -o "\quake3\q3pong_beta\vm\cgame.qvm" -f ..\cgame
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "cgame - Win32 Release"
# Name "cgame - Win32 Debug"
# Name "cgame - Win32 QVM"
# Begin Group "Source Files"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\game\bg_lib.c

!IF  "$(CFG)" == "cgame - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP BASE Exclude_From_Build 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=..\game\bg_lib.c
InputName=bg_lib

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\game\bg_misc.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=..\game\bg_misc.c
InputName=bg_misc

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\game\bg_pmove.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=..\game\bg_pmove.c
InputName=bg_pmove

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\game\bg_slidemove.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=..\game\bg_slidemove.c
InputName=bg_slidemove

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_consolecmds.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_consolecmds.c
InputName=cg_consolecmds

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_draw.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_draw.c
InputName=cg_draw

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_drawtools.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_drawtools.c
InputName=cg_drawtools

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_effects.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_effects.c
InputName=cg_effects

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_ents.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_ents.c
InputName=cg_ents

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_event.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_event.c
InputName=cg_event

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_info.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_info.c
InputName=cg_info

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_localents.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_localents.c
InputName=cg_localents

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_main.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_main.c
InputName=cg_main

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_marks.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_marks.c
InputName=cg_marks

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_players.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_players.c
InputName=cg_players

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_playerstate.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_playerstate.c
InputName=cg_playerstate

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_predict.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_predict.c
InputName=cg_predict

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_q3pong.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_q3pong.c
InputName=cg_q3pong

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_scoreboard.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_scoreboard.c
InputName=cg_scoreboard

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_servercmds.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_servercmds.c
InputName=cg_servercmds

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_snapshot.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_snapshot.c
InputName=cg_snapshot

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_syscalls.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Exclude_From_Build 1
# PROP Ignore_Default_Tool 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_view.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_view.c
InputName=cg_view

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cg_weapons.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=.\cg_weapons.c
InputName=cg_weapons

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\game\q_math.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=..\game\q_math.c
InputName=q_math

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\game\q_shared.c

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling $(InputPath)...
OutDir=.\vm
InputPath=..\game\q_shared.c
InputName=q_shared

"$(OutDir)\$(InputName).asm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(OutDir) 
	C:\Quake3\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\\..\\cgame -I..\\..\\game -I..\\..\\q3_ui .$(InputPath) 
	C:\Quake3\bin_nt\addtofile ..\cgame.q3asm $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\game\bg_local.h
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

SOURCE=.\cg_q3pong.h
# End Source File
# Begin Source File

SOURCE=..\game\q_shared.h
# End Source File
# Begin Source File

SOURCE=..\game\surfaceflags.h
# End Source File
# Begin Source File

SOURCE=.\tr_types.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cgame.def

!IF  "$(CFG)" == "cgame - Win32 Release"

!ELSEIF  "$(CFG)" == "cgame - Win32 Debug"

!ELSEIF  "$(CFG)" == "cgame - Win32 QVM"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\dummy.c
# End Source File
# End Target
# End Project
