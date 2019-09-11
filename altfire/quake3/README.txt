How to Build and Use Alternate Fire Source
------------------------------------------

This requires about 600MB of free space and MS Visual C++ 6.0.

1) You've already done this - unzipped altfire_code.zip into the root of a drive.  We'll assume it's drive C: for the rest of the README.

2) Copy pak0.pk3 to pak6.pk3 from "C:\Program Files\Quake III Arena\baseq3" into "C:\quake3\code\baseq3".  This will take a while, since pak0.pk3 is 457MB.

3) Copy your q3config.cfg (usually in "Quake III Arena\baseq3") into "C:\quake3\code\baseq3".

4) Find vcvars32.bat.  It should be in your "D:\Program Files\Microsoft Visual Studio\VC98\bin" directory.  Copy it into "C:\quake3\code" for convenience.

5) Locate "C:\quake3\code\setpath.bat".  Edit it (right-click and choose "Edit") to reflect the real path to your Alternate Fire source.  (If your stuff is in "C:\quake3" you shouldn't have to do anything.)

6) Browse to "C:\quake3\code\baseq3".  Assuming you have WinZip (and you should have either that or PowerArchiver if you want to make pk3's), right-click on z-alternatefire-02 and choose "Compress to z-alternatefire-02.zip".  Rename the new zip file to "z-alternatefire-02.pk3".

7) Find "C:\quake3\code\alternatefire.dsw" and double-click it.  This should open MS Visual C++ and load the Alternate Fire project.  Go to Project->Settings.  Make sure that "Win32 Debug" is selected from the drop-down and that "All" is selected from the list on the left.  ("All" is a dummy project that does nothing but depend on the other projects - so when you compile "All", you compile every DLL.)  Click on the "Debug" tag.  Under "Executable for debug session:", put in the path to quake3.exe or browse to it by clicking on the arrow to the right of the text box.

8) Choose Build->Rebuild All.  This should build everything in Debug mode.  (You shouldn't have to change the active configuration from Debug, ever.)  Run it.


So far, we've built the DDL's, but if you want to build a VM file for distribution you have to take a slightly different route.  Happily, everything's set up to do that now.

9) Open a DOS prompt.

10) Change directory to "C:\quake3\code".

11) Type "setpath", hit ENTER.  Type "vcvars32", hit ENTER.

12) If you got a message saying "Out of environment space" after issuing either command in step 11, you can resolve it by following the directions on this web page: http://support.microsoft.com/support/kb/articles/Q230/2/05.ASP

13) If you followed the instructions in step 12, go back to step 11.

14) Type "compile", hit ENTER.


This should build VM files into the directory "C:\quake3\baseq3\vm".  You still need to package it, and here are the steps:

15) Copy everything from "C:\quake3\code\baseq3\z-alternatefire-02" into "C:\quake3\z-alternatefire-02j".

16) Copy every ".qvm" file from "C:\quake3\baseq3\vm" into "C:\quake3\z-alternatefire-02j\vm".

17) Go to "C:\quake3", right-click on directory "z-alternatefire-02j" and choose "Compress to z-alternatefire-02j.zip".

18) Rename z-alternatefire-02j.zip to z-alternatefire-02j.pk3.


Ta da!  You're done!  You should be able to copy that pk3 into "C:\Program Files\Quake III Arena\alternatefire" and play it.  Though in functionality it is identical to version 1.52's pk3 (02i), alphabetically, it comes AFTER it.  So you should be playing it if you load up Quake, go to MODS, and choose "Alterante Fire".

IF YOU HOST A SERVER, MAKE SURE YOU DELETE THIS NEW FILE FIRST.  Nobody will have it but you.

IF YOU JOIN AN ALTERNATE FIRE SERVER, YOU WILL NOT BE PLAYING THIS NEW VERSION.  The server will tell your client that you'll be playing off of 02i, so this pk3 will not be loaded.

Now let's change something.



How to Make Evil Homing Missiles
--------------------------------

1) Open the Alternate Fire workspace in MS Visual C++ 6.0.

2) Open the file "g_missile.c".

3) At the top of the file, define HOMING_THINK as 1 (instead of 60), HOMING_MISSILE_SPEED as 1800 (instead of 400), and FIRST_HOMING_THINK as 1 (instead of 500).

4) Find the G_HomingMissile function.

5) Comment out this line:

	if (cs > cos(120.0f * M_PI / 180.0f))

6) Change this line:

	VectorScale(dir, 0.5, dir);

   to this:

	VectorScale(dir, 50, dir);

7) Run it.  Add a bot, and have fun.