# Microsoft Developer Studio Project File - Name="q3_ui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=q3_ui - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "q3_ui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "q3_ui.mak" CFG="q3_ui - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "q3_ui - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "q3_ui - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "q3_ui - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Q3_UI_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Q3_UI_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../Release/uix86.dll"

!ELSEIF  "$(CFG)" == "q3_ui - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "q3_ui___Win32_Debug"
# PROP BASE Intermediate_Dir "q3_ui___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "q3_ui___Win32_Debug"
# PROP Intermediate_Dir "q3_ui___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Q3_UI_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /FR"Debug/" /Fp"Debug/q3_ui.pch" /YX /Fo"Debug/" /Fd"Debug/" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /base:"0x40000000" /dll /profile /map:"../../uix86.map" /debug /machine:I386 /def:".\ui.def" /out:"../../uix86.dll" /implib:"Debug/uix86.lib"

!ENDIF 

# Begin Target

# Name "q3_ui - Win32 Release"
# Name "q3_ui - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\game\bg_misc.c
# End Source File
# Begin Source File

SOURCE=..\game\q_math.c
# End Source File
# Begin Source File

SOURCE=..\game\q_shared.c
# End Source File
# Begin Source File

SOURCE=.\ui.def

!IF  "$(CFG)" == "q3_ui - Win32 Release"

!ELSEIF  "$(CFG)" == "q3_ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ui_addbots.c
# End Source File
# Begin Source File

SOURCE=.\ui_atoms.c
# End Source File
# Begin Source File

SOURCE=.\ui_cdkey.c
# End Source File
# Begin Source File

SOURCE=.\ui_cinematics.c
# End Source File
# Begin Source File

SOURCE=.\ui_confirm.c
# End Source File
# Begin Source File

SOURCE=.\ui_connect.c
# End Source File
# Begin Source File

SOURCE=.\ui_credits.c
# End Source File
# Begin Source File

SOURCE=.\ui_demo2.c
# End Source File
# Begin Source File

SOURCE=.\ui_display.c
# End Source File
# Begin Source File

SOURCE=.\ui_gameinfo.c
# End Source File
# Begin Source File

SOURCE=.\ui_ingame.c
# End Source File
# Begin Source File

SOURCE=.\ui_loadconfig.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ui_main.c
# End Source File
# Begin Source File

SOURCE=.\ui_menu.c
# End Source File
# Begin Source File

SOURCE=.\ui_mfield.c
# End Source File
# Begin Source File

SOURCE=.\ui_mods.c
# End Source File
# Begin Source File

SOURCE=.\ui_network.c
# End Source File
# Begin Source File

SOURCE=.\ui_options.c
# End Source File
# Begin Source File

SOURCE=.\ui_playermodel.c
# End Source File
# Begin Source File

SOURCE=.\ui_players.c
# End Source File
# Begin Source File

SOURCE=.\ui_playersettings.c
# End Source File
# Begin Source File

SOURCE=.\ui_preferences.c
# End Source File
# Begin Source File

SOURCE=.\ui_qmenu.c
# End Source File
# Begin Source File

SOURCE=.\ui_rally_controls.c
# End Source File
# Begin Source File

SOURCE=.\ui_rally_credits.c
# End Source File
# Begin Source File

SOURCE=.\ui_rally_favorites.c
# End Source File
# Begin Source File

SOURCE=.\ui_rally_gfxloading.c
# End Source File
# Begin Source File

SOURCE=.\ui_rally_options.c
# End Source File
# Begin Source File

SOURCE=.\ui_rally_servers.c
# End Source File
# Begin Source File

SOURCE=.\ui_rally_startserver.c
# End Source File
# Begin Source File

SOURCE=.\ui_rally_tools.c
# End Source File
# Begin Source File

SOURCE=.\ui_removebots.c
# End Source File
# Begin Source File

SOURCE=.\ui_saveconfig.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ui_serverinfo.c
# End Source File
# Begin Source File

SOURCE=.\ui_setup.c
# End Source File
# Begin Source File

SOURCE=.\ui_sound.c
# End Source File
# Begin Source File

SOURCE=.\ui_sparena.c
# End Source File
# Begin Source File

SOURCE=.\ui_specifyserver.c
# End Source File
# Begin Source File

SOURCE=.\ui_splevel.c
# End Source File
# Begin Source File

SOURCE=.\ui_sppostgame.c
# End Source File
# Begin Source File

SOURCE=.\ui_spreset.c
# End Source File
# Begin Source File

SOURCE=.\ui_spskill.c
# End Source File
# Begin Source File

SOURCE=.\ui_syscalls.c
# End Source File
# Begin Source File

SOURCE=.\ui_team.c
# End Source File
# Begin Source File

SOURCE=.\ui_teamorders.c
# End Source File
# Begin Source File

SOURCE=.\ui_video.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\keycodes.h
# End Source File
# Begin Source File

SOURCE=..\game\q_shared.h
# End Source File
# Begin Source File

SOURCE=.\ui_local.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
