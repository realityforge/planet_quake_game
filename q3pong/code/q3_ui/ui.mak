# Microsoft Developer Studio Generated NMAKE File, Based on ui.dsp
!IF "$(CFG)" == ""
CFG=ui - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ui - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ui - Win32 Release" && "$(CFG)" != "ui - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ui.mak" CFG="ui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ui - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ui - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "ui - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\Release\uix86.dll"


CLEAN :
	-@erase "$(INTDIR)\bg_misc.obj"
	-@erase "$(INTDIR)\q_math.obj"
	-@erase "$(INTDIR)\q_shared.obj"
	-@erase "$(INTDIR)\ui_addbots.obj"
	-@erase "$(INTDIR)\ui_arcade.obj"
	-@erase "$(INTDIR)\ui_atoms.obj"
	-@erase "$(INTDIR)\ui_cdkey.obj"
	-@erase "$(INTDIR)\ui_cinematics.obj"
	-@erase "$(INTDIR)\ui_confirm.obj"
	-@erase "$(INTDIR)\ui_connect.obj"
	-@erase "$(INTDIR)\ui_controls2.obj"
	-@erase "$(INTDIR)\ui_credits.obj"
	-@erase "$(INTDIR)\ui_demo2.obj"
	-@erase "$(INTDIR)\ui_display.obj"
	-@erase "$(INTDIR)\ui_gameinfo.obj"
	-@erase "$(INTDIR)\ui_ingame.obj"
	-@erase "$(INTDIR)\ui_loadconfig.obj"
	-@erase "$(INTDIR)\ui_main.obj"
	-@erase "$(INTDIR)\ui_menu.obj"
	-@erase "$(INTDIR)\ui_mfield.obj"
	-@erase "$(INTDIR)\ui_mods.obj"
	-@erase "$(INTDIR)\ui_network.obj"
	-@erase "$(INTDIR)\ui_playermodel.obj"
	-@erase "$(INTDIR)\ui_players.obj"
	-@erase "$(INTDIR)\ui_playersettings.obj"
	-@erase "$(INTDIR)\ui_preferences.obj"
	-@erase "$(INTDIR)\ui_qmenu.obj"
	-@erase "$(INTDIR)\ui_removebots.obj"
	-@erase "$(INTDIR)\ui_saveconfig.obj"
	-@erase "$(INTDIR)\ui_serverinfo.obj"
	-@erase "$(INTDIR)\ui_servers2.obj"
	-@erase "$(INTDIR)\ui_setup.obj"
	-@erase "$(INTDIR)\ui_sound.obj"
	-@erase "$(INTDIR)\ui_sparena.obj"
	-@erase "$(INTDIR)\ui_specifyserver.obj"
	-@erase "$(INTDIR)\ui_splevel.obj"
	-@erase "$(INTDIR)\ui_sppostgame.obj"
	-@erase "$(INTDIR)\ui_spskill.obj"
	-@erase "$(INTDIR)\ui_startserver.obj"
	-@erase "$(INTDIR)\ui_syscalls.obj"
	-@erase "$(INTDIR)\ui_team.obj"
	-@erase "$(INTDIR)\ui_teamorders.obj"
	-@erase "$(INTDIR)\ui_video.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\uix86.exp"
	-@erase "$(OUTDIR)\uix86.lib"
	-@erase "$(OUTDIR)\uix86.map"
	-@erase "..\Release\uix86.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /Fp"$(INTDIR)\ui.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /base:"0x40000000" /dll /incremental:no /pdb:"$(OUTDIR)\uix86.pdb" /map:"$(INTDIR)\uix86.map" /machine:I386 /def:".\ui.def" /out:"../Release/uix86.dll" /implib:"$(OUTDIR)\uix86.lib" 
DEF_FILE= \
	".\ui.def"
LINK32_OBJS= \
	"$(INTDIR)\bg_misc.obj" \
	"$(INTDIR)\q_math.obj" \
	"$(INTDIR)\q_shared.obj" \
	"$(INTDIR)\ui_addbots.obj" \
	"$(INTDIR)\ui_arcade.obj" \
	"$(INTDIR)\ui_atoms.obj" \
	"$(INTDIR)\ui_cdkey.obj" \
	"$(INTDIR)\ui_cinematics.obj" \
	"$(INTDIR)\ui_confirm.obj" \
	"$(INTDIR)\ui_connect.obj" \
	"$(INTDIR)\ui_controls2.obj" \
	"$(INTDIR)\ui_credits.obj" \
	"$(INTDIR)\ui_demo2.obj" \
	"$(INTDIR)\ui_display.obj" \
	"$(INTDIR)\ui_gameinfo.obj" \
	"$(INTDIR)\ui_ingame.obj" \
	"$(INTDIR)\ui_loadconfig.obj" \
	"$(INTDIR)\ui_main.obj" \
	"$(INTDIR)\ui_menu.obj" \
	"$(INTDIR)\ui_mfield.obj" \
	"$(INTDIR)\ui_mods.obj" \
	"$(INTDIR)\ui_network.obj" \
	"$(INTDIR)\ui_playermodel.obj" \
	"$(INTDIR)\ui_players.obj" \
	"$(INTDIR)\ui_playersettings.obj" \
	"$(INTDIR)\ui_preferences.obj" \
	"$(INTDIR)\ui_qmenu.obj" \
	"$(INTDIR)\ui_removebots.obj" \
	"$(INTDIR)\ui_saveconfig.obj" \
	"$(INTDIR)\ui_serverinfo.obj" \
	"$(INTDIR)\ui_servers2.obj" \
	"$(INTDIR)\ui_setup.obj" \
	"$(INTDIR)\ui_sound.obj" \
	"$(INTDIR)\ui_sparena.obj" \
	"$(INTDIR)\ui_specifyserver.obj" \
	"$(INTDIR)\ui_splevel.obj" \
	"$(INTDIR)\ui_sppostgame.obj" \
	"$(INTDIR)\ui_spskill.obj" \
	"$(INTDIR)\ui_startserver.obj" \
	"$(INTDIR)\ui_syscalls.obj" \
	"$(INTDIR)\ui_team.obj" \
	"$(INTDIR)\ui_teamorders.obj" \
	"$(INTDIR)\ui_video.obj"

"..\Release\uix86.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ui - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\Debug\uix86.dll" "$(OUTDIR)\ui.bsc"


CLEAN :
	-@erase "$(INTDIR)\bg_misc.obj"
	-@erase "$(INTDIR)\bg_misc.sbr"
	-@erase "$(INTDIR)\q_math.obj"
	-@erase "$(INTDIR)\q_math.sbr"
	-@erase "$(INTDIR)\q_shared.obj"
	-@erase "$(INTDIR)\q_shared.sbr"
	-@erase "$(INTDIR)\ui_addbots.obj"
	-@erase "$(INTDIR)\ui_addbots.sbr"
	-@erase "$(INTDIR)\ui_arcade.obj"
	-@erase "$(INTDIR)\ui_arcade.sbr"
	-@erase "$(INTDIR)\ui_atoms.obj"
	-@erase "$(INTDIR)\ui_atoms.sbr"
	-@erase "$(INTDIR)\ui_cdkey.obj"
	-@erase "$(INTDIR)\ui_cdkey.sbr"
	-@erase "$(INTDIR)\ui_cinematics.obj"
	-@erase "$(INTDIR)\ui_cinematics.sbr"
	-@erase "$(INTDIR)\ui_confirm.obj"
	-@erase "$(INTDIR)\ui_confirm.sbr"
	-@erase "$(INTDIR)\ui_connect.obj"
	-@erase "$(INTDIR)\ui_connect.sbr"
	-@erase "$(INTDIR)\ui_controls2.obj"
	-@erase "$(INTDIR)\ui_controls2.sbr"
	-@erase "$(INTDIR)\ui_credits.obj"
	-@erase "$(INTDIR)\ui_credits.sbr"
	-@erase "$(INTDIR)\ui_demo2.obj"
	-@erase "$(INTDIR)\ui_demo2.sbr"
	-@erase "$(INTDIR)\ui_display.obj"
	-@erase "$(INTDIR)\ui_display.sbr"
	-@erase "$(INTDIR)\ui_gameinfo.obj"
	-@erase "$(INTDIR)\ui_gameinfo.sbr"
	-@erase "$(INTDIR)\ui_ingame.obj"
	-@erase "$(INTDIR)\ui_ingame.sbr"
	-@erase "$(INTDIR)\ui_loadconfig.obj"
	-@erase "$(INTDIR)\ui_loadconfig.sbr"
	-@erase "$(INTDIR)\ui_main.obj"
	-@erase "$(INTDIR)\ui_main.sbr"
	-@erase "$(INTDIR)\ui_menu.obj"
	-@erase "$(INTDIR)\ui_menu.sbr"
	-@erase "$(INTDIR)\ui_mfield.obj"
	-@erase "$(INTDIR)\ui_mfield.sbr"
	-@erase "$(INTDIR)\ui_mods.obj"
	-@erase "$(INTDIR)\ui_mods.sbr"
	-@erase "$(INTDIR)\ui_network.obj"
	-@erase "$(INTDIR)\ui_network.sbr"
	-@erase "$(INTDIR)\ui_playermodel.obj"
	-@erase "$(INTDIR)\ui_playermodel.sbr"
	-@erase "$(INTDIR)\ui_players.obj"
	-@erase "$(INTDIR)\ui_players.sbr"
	-@erase "$(INTDIR)\ui_playersettings.obj"
	-@erase "$(INTDIR)\ui_playersettings.sbr"
	-@erase "$(INTDIR)\ui_preferences.obj"
	-@erase "$(INTDIR)\ui_preferences.sbr"
	-@erase "$(INTDIR)\ui_qmenu.obj"
	-@erase "$(INTDIR)\ui_qmenu.sbr"
	-@erase "$(INTDIR)\ui_removebots.obj"
	-@erase "$(INTDIR)\ui_removebots.sbr"
	-@erase "$(INTDIR)\ui_saveconfig.obj"
	-@erase "$(INTDIR)\ui_saveconfig.sbr"
	-@erase "$(INTDIR)\ui_serverinfo.obj"
	-@erase "$(INTDIR)\ui_serverinfo.sbr"
	-@erase "$(INTDIR)\ui_servers2.obj"
	-@erase "$(INTDIR)\ui_servers2.sbr"
	-@erase "$(INTDIR)\ui_setup.obj"
	-@erase "$(INTDIR)\ui_setup.sbr"
	-@erase "$(INTDIR)\ui_sound.obj"
	-@erase "$(INTDIR)\ui_sound.sbr"
	-@erase "$(INTDIR)\ui_sparena.obj"
	-@erase "$(INTDIR)\ui_sparena.sbr"
	-@erase "$(INTDIR)\ui_specifyserver.obj"
	-@erase "$(INTDIR)\ui_specifyserver.sbr"
	-@erase "$(INTDIR)\ui_splevel.obj"
	-@erase "$(INTDIR)\ui_splevel.sbr"
	-@erase "$(INTDIR)\ui_sppostgame.obj"
	-@erase "$(INTDIR)\ui_sppostgame.sbr"
	-@erase "$(INTDIR)\ui_spskill.obj"
	-@erase "$(INTDIR)\ui_spskill.sbr"
	-@erase "$(INTDIR)\ui_startserver.obj"
	-@erase "$(INTDIR)\ui_startserver.sbr"
	-@erase "$(INTDIR)\ui_syscalls.obj"
	-@erase "$(INTDIR)\ui_syscalls.sbr"
	-@erase "$(INTDIR)\ui_team.obj"
	-@erase "$(INTDIR)\ui_team.sbr"
	-@erase "$(INTDIR)\ui_teamorders.obj"
	-@erase "$(INTDIR)\ui_teamorders.sbr"
	-@erase "$(INTDIR)\ui_video.obj"
	-@erase "$(INTDIR)\ui_video.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ui.bsc"
	-@erase "$(OUTDIR)\uix86.exp"
	-@erase "$(OUTDIR)\uix86.lib"
	-@erase "$(OUTDIR)\uix86.map"
	-@erase "..\Debug\ui.pdb"
	-@erase "..\Debug\uix86.dll"
	-@erase "..\Debug\uix86.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ui.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ui.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bg_misc.sbr" \
	"$(INTDIR)\q_math.sbr" \
	"$(INTDIR)\q_shared.sbr" \
	"$(INTDIR)\ui_addbots.sbr" \
	"$(INTDIR)\ui_arcade.sbr" \
	"$(INTDIR)\ui_atoms.sbr" \
	"$(INTDIR)\ui_cdkey.sbr" \
	"$(INTDIR)\ui_cinematics.sbr" \
	"$(INTDIR)\ui_confirm.sbr" \
	"$(INTDIR)\ui_connect.sbr" \
	"$(INTDIR)\ui_controls2.sbr" \
	"$(INTDIR)\ui_credits.sbr" \
	"$(INTDIR)\ui_demo2.sbr" \
	"$(INTDIR)\ui_display.sbr" \
	"$(INTDIR)\ui_gameinfo.sbr" \
	"$(INTDIR)\ui_ingame.sbr" \
	"$(INTDIR)\ui_loadconfig.sbr" \
	"$(INTDIR)\ui_main.sbr" \
	"$(INTDIR)\ui_menu.sbr" \
	"$(INTDIR)\ui_mfield.sbr" \
	"$(INTDIR)\ui_mods.sbr" \
	"$(INTDIR)\ui_network.sbr" \
	"$(INTDIR)\ui_playermodel.sbr" \
	"$(INTDIR)\ui_players.sbr" \
	"$(INTDIR)\ui_playersettings.sbr" \
	"$(INTDIR)\ui_preferences.sbr" \
	"$(INTDIR)\ui_qmenu.sbr" \
	"$(INTDIR)\ui_removebots.sbr" \
	"$(INTDIR)\ui_saveconfig.sbr" \
	"$(INTDIR)\ui_serverinfo.sbr" \
	"$(INTDIR)\ui_servers2.sbr" \
	"$(INTDIR)\ui_setup.sbr" \
	"$(INTDIR)\ui_sound.sbr" \
	"$(INTDIR)\ui_sparena.sbr" \
	"$(INTDIR)\ui_specifyserver.sbr" \
	"$(INTDIR)\ui_splevel.sbr" \
	"$(INTDIR)\ui_sppostgame.sbr" \
	"$(INTDIR)\ui_spskill.sbr" \
	"$(INTDIR)\ui_startserver.sbr" \
	"$(INTDIR)\ui_syscalls.sbr" \
	"$(INTDIR)\ui_team.sbr" \
	"$(INTDIR)\ui_teamorders.sbr" \
	"$(INTDIR)\ui_video.sbr"

"$(OUTDIR)\ui.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /base:"0x40000000" /dll /incremental:yes /pdb:"../Debug/ui.pdb" /map:"$(INTDIR)\uix86.map" /debug /machine:I386 /def:".\ui.def" /out:"../Debug/uix86.dll" /implib:"$(OUTDIR)\uix86.lib" /pdbtype:sept 
DEF_FILE= \
	".\ui.def"
LINK32_OBJS= \
	"$(INTDIR)\bg_misc.obj" \
	"$(INTDIR)\q_math.obj" \
	"$(INTDIR)\q_shared.obj" \
	"$(INTDIR)\ui_addbots.obj" \
	"$(INTDIR)\ui_arcade.obj" \
	"$(INTDIR)\ui_atoms.obj" \
	"$(INTDIR)\ui_cdkey.obj" \
	"$(INTDIR)\ui_cinematics.obj" \
	"$(INTDIR)\ui_confirm.obj" \
	"$(INTDIR)\ui_connect.obj" \
	"$(INTDIR)\ui_controls2.obj" \
	"$(INTDIR)\ui_credits.obj" \
	"$(INTDIR)\ui_demo2.obj" \
	"$(INTDIR)\ui_display.obj" \
	"$(INTDIR)\ui_gameinfo.obj" \
	"$(INTDIR)\ui_ingame.obj" \
	"$(INTDIR)\ui_loadconfig.obj" \
	"$(INTDIR)\ui_main.obj" \
	"$(INTDIR)\ui_menu.obj" \
	"$(INTDIR)\ui_mfield.obj" \
	"$(INTDIR)\ui_mods.obj" \
	"$(INTDIR)\ui_network.obj" \
	"$(INTDIR)\ui_playermodel.obj" \
	"$(INTDIR)\ui_players.obj" \
	"$(INTDIR)\ui_playersettings.obj" \
	"$(INTDIR)\ui_preferences.obj" \
	"$(INTDIR)\ui_qmenu.obj" \
	"$(INTDIR)\ui_removebots.obj" \
	"$(INTDIR)\ui_saveconfig.obj" \
	"$(INTDIR)\ui_serverinfo.obj" \
	"$(INTDIR)\ui_servers2.obj" \
	"$(INTDIR)\ui_setup.obj" \
	"$(INTDIR)\ui_sound.obj" \
	"$(INTDIR)\ui_sparena.obj" \
	"$(INTDIR)\ui_specifyserver.obj" \
	"$(INTDIR)\ui_splevel.obj" \
	"$(INTDIR)\ui_sppostgame.obj" \
	"$(INTDIR)\ui_spskill.obj" \
	"$(INTDIR)\ui_startserver.obj" \
	"$(INTDIR)\ui_syscalls.obj" \
	"$(INTDIR)\ui_team.obj" \
	"$(INTDIR)\ui_teamorders.obj" \
	"$(INTDIR)\ui_video.obj"

"..\Debug\uix86.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("ui.dep")
!INCLUDE "ui.dep"
!ELSE 
!MESSAGE Warning: cannot find "ui.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ui - Win32 Release" || "$(CFG)" == "ui - Win32 Debug"
SOURCE=..\game\bg_lib.c
SOURCE=..\game\bg_misc.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\bg_misc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\bg_misc.obj"	"$(INTDIR)\bg_misc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\game\q_math.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\q_math.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\q_math.obj"	"$(INTDIR)\q_math.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\game\q_shared.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\q_shared.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\q_shared.obj"	"$(INTDIR)\q_shared.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\ui_addbots.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_addbots.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_addbots.obj"	"$(INTDIR)\ui_addbots.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_arcade.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_arcade.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_arcade.obj"	"$(INTDIR)\ui_arcade.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_atoms.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_atoms.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_atoms.obj"	"$(INTDIR)\ui_atoms.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_cdkey.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_cdkey.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_cdkey.obj"	"$(INTDIR)\ui_cdkey.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_cinematics.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_cinematics.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_cinematics.obj"	"$(INTDIR)\ui_cinematics.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_confirm.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_confirm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_confirm.obj"	"$(INTDIR)\ui_confirm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_connect.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_connect.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_connect.obj"	"$(INTDIR)\ui_connect.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_controls2.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_controls2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_controls2.obj"	"$(INTDIR)\ui_controls2.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_credits.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_credits.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_credits.obj"	"$(INTDIR)\ui_credits.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_demo2.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_demo2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_demo2.obj"	"$(INTDIR)\ui_demo2.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_display.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_display.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_display.obj"	"$(INTDIR)\ui_display.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_gameinfo.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_gameinfo.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_gameinfo.obj"	"$(INTDIR)\ui_gameinfo.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_ingame.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_ingame.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_ingame.obj"	"$(INTDIR)\ui_ingame.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_loadconfig.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_loadconfig.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_loadconfig.obj"	"$(INTDIR)\ui_loadconfig.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_main.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_main.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_main.obj"	"$(INTDIR)\ui_main.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_menu.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_menu.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_menu.obj"	"$(INTDIR)\ui_menu.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_mfield.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_mfield.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_mfield.obj"	"$(INTDIR)\ui_mfield.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_mods.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_mods.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_mods.obj"	"$(INTDIR)\ui_mods.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_network.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_network.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_network.obj"	"$(INTDIR)\ui_network.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_playermodel.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_playermodel.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_playermodel.obj"	"$(INTDIR)\ui_playermodel.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_players.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_players.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_players.obj"	"$(INTDIR)\ui_players.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_playersettings.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_playersettings.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_playersettings.obj"	"$(INTDIR)\ui_playersettings.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_preferences.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_preferences.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_preferences.obj"	"$(INTDIR)\ui_preferences.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_qmenu.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_qmenu.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_qmenu.obj"	"$(INTDIR)\ui_qmenu.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_removebots.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_removebots.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_removebots.obj"	"$(INTDIR)\ui_removebots.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_saveconfig.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_saveconfig.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_saveconfig.obj"	"$(INTDIR)\ui_saveconfig.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_serverinfo.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_serverinfo.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_serverinfo.obj"	"$(INTDIR)\ui_serverinfo.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_servers2.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_servers2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_servers2.obj"	"$(INTDIR)\ui_servers2.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_setup.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_setup.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_setup.obj"	"$(INTDIR)\ui_setup.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_sound.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_sound.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_sound.obj"	"$(INTDIR)\ui_sound.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_sparena.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_sparena.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_sparena.obj"	"$(INTDIR)\ui_sparena.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_specifyserver.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_specifyserver.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_specifyserver.obj"	"$(INTDIR)\ui_specifyserver.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_splevel.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_splevel.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_splevel.obj"	"$(INTDIR)\ui_splevel.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_sppostgame.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_sppostgame.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_sppostgame.obj"	"$(INTDIR)\ui_sppostgame.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_spskill.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_spskill.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_spskill.obj"	"$(INTDIR)\ui_spskill.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_startserver.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_startserver.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_startserver.obj"	"$(INTDIR)\ui_startserver.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_syscalls.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_syscalls.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_syscalls.obj"	"$(INTDIR)\ui_syscalls.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_team.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_team.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_team.obj"	"$(INTDIR)\ui_team.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_teamorders.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_teamorders.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_teamorders.obj"	"$(INTDIR)\ui_teamorders.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui_video.c

!IF  "$(CFG)" == "ui - Win32 Release"


"$(INTDIR)\ui_video.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ui - Win32 Debug"


"$(INTDIR)\ui_video.obj"	"$(INTDIR)\ui_video.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

