----------------------------------------
UI Enhanced 1.2 for Quake 3 release 1.32
----------------------------------------


* Introduction
   - assistance given
* Installation
   - installing over previous versions (IMPORTANT!)
* Running UI Enhanced (IMPORTANT!)
* Features
* Known issues
* Some Questions Answered
* Redistribution
* Legal


------------
Introduction
------------

UI Enhanced adds to the user interface of Quake 3 by providing a more
flexible and powerful way of setting up single player and server games.
It replaces only the user interface, leaving the server and client
code untouched.

You can create a rotation of up to 32 maps, or a random selection from
your map collection. In addition to selecting your own bots, they can
be randomly picked and replaced on each level change. The bot skill can
also be set or randomly picked from a range of values.

Many server parameters can be customized. A script can be saved for
loading back in at a later time, or for use with a dedicated server.

In addition to the new map and bot game generation, there are numerous
improvements to the rest of the UI interface. An better model/skin
selection page (along with a model animator), an Unreal Tournament style
bot control menu, better add and remove menus for bots in play, as well
as many other small tweaks and bug fixes.

This release works with Quake 3 version 1.32, and includes support for
specific new features: old/new weapon graphics, 1.17 or 1.27 mouse input
style, and much more. It also works with 1.31 and 1.30 versions.

If you're new to UI Enhanced, then enjoy the features. If you're upgrading
from an earlier version then take a look around and see what's changed.

Source code is also available for use in other mods, visit my UIE homepage
for more information.

Your feedback, ideas, and criticisms are, as always, welcome.


Enjoy!

HypoThermia (uie@planetquake.com)


UI Enhanced: http://www.planetquake.com/uie/
 Code3Arena: http://www.planetquake.com/code3arena/
   Homepage: http://www.eudoxus.demon.co.uk/


All code modifications to the Q3 User Interface are
Copyright (c) 2000-2002 Ian "HypoThermia" Jefferies
where this does not conflict with the Quake 3 source
code licensing agreement.



Assistance given
----------------

These people have aided UI Enhanced by providing valuable help
and improvements. They've made my work that much easier.

Thank you!


Source code for an essential bug fix:
Joel Baxter (aka Johnny Law), author of MoreMaps


Enthusiasm for helping diagnose bugs:
Sascha Lichtenstein (alpha tested 0.97)
Kevin Knight        (alpha tested 0.97) 
Dagwit


Map scripts:
BFG-Berater
Joel "Johnny Law" Baxter
Kevin Knight
DeSiRe
Sascha Lichtenstein

Installation suggestions:
Rogue13

Additional help
Michael "Sludge" Labbe

Some quotes taken from Sam Spade suite of internet tools. Recommended! 
http://www.samspade.org/ssw/

Source code base:
All at Id software

And everyone else who's sent an e-mail with praise and/or a bug report!


------------
Installation
------------

Unzip the contents of this zip archive file into your Quake 3
directory, making sure that directories are restored. You should
have a new directory called "uie", and it should be peer (i.e.
at the same dir depth) to "baseq3".


Installing over previous versions (IMPORTANT!)
----------------------------------------------

The files in this archive replace and override all previous versions
of UI Enhanced. To keep your directory as clean as possible you can
remove older UIE related files that aren't in this archive.

One important change made to version 1.1 onward concerns data stored in 
q3config.cfg. The first time you open the Skirmish menu this data is
exported into uieSkirmish.dat, and after that the UIE skirmish data in
q3config.cfg is ignored.

You can safely clean this data out of q3config.cfg with the following 
procedure:

1) Open q3config.cfg in a text editor (neither notepad or write works
properly). I'd recommend a good shareware or freeware text editor like 
Programmer's File Editor.

2) Remove any lines of this form:

seta uie_*

The following values may be safely left behind, they control
other parts of the interface and aren't used to create a skirmish 
game. Deleting them won't do any harm though.

seta uie_map_multisel
seta uie_map_list
seta uie_bot_multisel
seta uie_bot_list
seta uie_ingame_dynamicmenu
seta uie_s_animsfx
seta uie_mapicons
seta uie_autoclosebotmenu
seta uie_olditemmenu


I was hoping to do this cleanup from within UIE itself, but there 
doesn't appear to be any support for it in the cvar management.



--------------------------------
Running UI Enhanced (IMPORTANT!)
--------------------------------

There are three methods of running UI Enhanced:

1) (Recommended) From the command line
--------------------------------------

Create a shortcut on your desktop to the Quake 3 executable. Right click
on this shortcut, select "Properties...". Then select the "Shortcut" tab.

Modify the target in the following way. If it originally looked like this:

"C:\Quake3\quake3.exe"

then change it so it looks like this:

"C:\Quake3\quake3.exe" +set fs_game uie

The quotes may or may not be present, there might be a need to add them
depending on which version of Windows you're running. Thanks go 
to "rogue13" for the tip.

You will also need to use the correct directory for the Quake3 executable,
I've given C:\Quake3 as an example.

If there is already a part of the target line that includes
"+set fs_game XXX" (where XXX is a directory, and no quotes), then 
remove that part completely before making the addition above.

You may also find it beneficial to run as in impure mode. Your command
line would then look something like this:

"C:\Quake3\quake3.exe" +set fs_game uie +set sv_pure 0


Use the shortcut in the normal way.


2) Copying UIE into baseq3
--------------------------

If you want you can use UI Enhanced on a more permanent basis: copy the
pk3 files in the "uie" directory into the "baseq3" directory. You should also
copy over usermaps.txt if you've made any use of this file.

So long as there isn't a pk3 file in "baseq3" that supplies its own
replacement of the user interface (after UI Enhanced) then this should
work. Run Quake 3 in the normal way.

If you do find that UI Enhanced isn't running, then check the pk3 files that
follow on from UI Enhanced (vm_uie_*.pk3) in alphabetical order. Since
they're just zip files you can examine the contents and locate the ui.qvm file
that's over-riding UI Enhanced.

The new pure protection code in Quake3 version 1.30 onward now allows this 
to work. When you connect to a public server the only pk3 files loaded are
the ones needed by the server, and UI Enhanced is safely bypassed.



3) (Not recommended) From the MODS menu
---------------------------------------

Start Quake 3 and select the Mods menu (release 1.16n and later). Locate UI
Enhanced and load it.

At the time of writing (Quake 3 release 1.27), there is a bug that prevents
the mod loading the correct variables from q3config.cfg. You will lose all
the map rotations and bot configuration previously set up.

Useful to reset UI Enhanced, but not recommended for general use until this
bug is fixed.




--------
Features
--------

See the history.txt document for a more detailed look at features and
bugs that have been fixed.


General improvements
--------------------

* Support for the new 1.32 options, including Punkbuster when creating
  server scripts in multiplayer mode.

* Support for the new options in the Quake3 1.30 release. New weapon graphics,
  core/outer railgun colours etc.

* Improved model and skin chooser: separating out model types from the
  skins that go with them.

* Selection of separate head and team models in the model/skin browser.

* Model selection includes rotation controls for better range of 
  viewing angles. 

* A model animator for preview of animations and sound fx.

* In game menu now includes a full callvote implementation. For those who
  prefer the old style menu, there's an option to use it in the preferences.

* Enable or disable map items from the in-game menu.

* Current player model now shown on the startup menu page, and tracks
  the cursor for better preview. Also an animated preview of the model
  after a short idle period. Direct link to player stats from the main
  menu page.

* Bot command menu system for use in Team DM and CTF gametypes. In the
  style of the Unreal Tournament equivalent. A key binding has been added
  to the setup menu, and menu auto-close to the game options.

* Improvements to the addbot and removebot in-game menus: a picture of
  the selected bot is shown, along with its skill icon, and team colour if
  appropriate.

* In-game menu adds a Next Map command, intended for any map type, and
  particularly tournaments. It also tries to put the tournament player(s)
  back into a game, rather than let them wait for the spectator list to
  rotate.

* In-game menu now has the option of using a dynamic version: more
  commands, including a complete callvote system (like TA). 

* More options in the setup game options (FOV, timer, FPS, shadows, blood,
  gibs). Now also uses a tabbed page system - better grouping of controls.

* Demos are now properly recognized and loaded. Timedemo can be enabled
  from the menu too! 

* Software fix for the single player and bot duplication bugs. 

* Includes fixes for some maps without arena scripts. Users are 
  encouraged to send in their fixes for redistribution in a central
  archive from this site.


Enhanced Skirmish and Multiplayer game setup
--------------------------------------------

* Disable individual or groups of items on maps.

* Specified up to 32 maps in order or random rotation. Or have a random
  set of maps chosen for you. A unique maplist for each game type.

* Control of map duration, frag, or cap limit - on a per map basis. Or
  the application of one set of parameters to all maps.

* Frag limits can now be set to those recommended by the map arena script.

* Custom maps types can be added, and used to create random map lists.
  RailArena, BigMaps, and SmallMaps are included to get you started.

* Randomly generated maps can have named maps excluded: never play the
  maps you don't like!

* Custom type for maps that don't support bots. Generated automatically
  from your map collection. Randomly generated map lists exclude maps of
  this type.

* Enhanced map selection: maps pictures are named, and 12 per page. Can
  add maps that don't normally support a game type (e.g. tournament or
  team deathmatch).

* Multiple selection of maps, and a list mode, to help with large map
  collections. 

* Maps that don't have an arena script can now be viewed in the selection
  page. 

* Map selection now includes icons for each map that tell you what
  gametype it supports: including your custom map types. Restrict yourself
  to Id or non-Id maps.

* Selection of bots: one fixed list of bots for all maps, or a random
  selection. The random choice of bots can be changed every map, or up to
  every 4th map.

* Skill levels for bots can be chosen individually or set from a global
  value. Skill can take either a single fixed value, or a min/max style
  range which is chosen when the server script is generated. The min/max
  value can be biased high or low for variation.

* Fractional bot skill levels,can be enabled when using a skill range. 
  Makes full use of the skill bias feature to give smoother variation 
  in bot skill.

* Randomly generated bot lists can now exclude named bots. Eliminate
  bots you don't want to play.

* Maps can now be played with the recommended bot selection for that map.
  Or maps can be played with the same number of random bots (with an
  exclusion list). You can now mix large and small maps, and get a
  playable number of bots for each! 

* Maps can play the arena script recommended number of bots, selecting 
  from a user defined pool (an inclusion list).

* Multiple selection of bots, and a list mode, to help with large
  collections. 

* Several server attributes can be setup and modified, including pure,
  dedicated, team balance and auto join etc.

* More server attributes can be setup, including password, allow download,
  min and max ping. Also includes a fix for the broken fraglimit and
  timelimit problems. 

* Setup parameters can be saved as a script for later use in setting up
  a server map rotation, or execution from the console.

* A script saved by UIE can be reloaded, restoring the parameters. Move
  quickly between previously used map and bot configurations! 

* All parameters are saved between sessions on a per-game-type basis.

  Setup a complicated map rotation or bot list and it will be there for
  you the next time you play.


------------
Known Issues
------------

Several known issues at the moment:

1) If the bot command menu used in CTF or TDM is left open after a command
is issued, the response from the bot won't be visible until the menu is closed.
The text in this region might also scroll up out of view before the command menu 
is closed.

No known work around at this time, though the response is visible in the
team message overlay.


2) Disabling one or more powerups may disable all powerups or crash the
map. This occurs when there are several powerups available for a given
spot (e.g. The Nameless Place). 

Disabling one of those powerups will cause a crash during gameplay with
the DLL version of the server at the point when it would have first
spawned. In the VM, all powerups spawning at that point are disabled 
in the game from then on.


No known workaround from UIE. Requires an updated server code to fix this.


3) UIE refuses to run pure. When run impure you get a console message like:

"WARNING: bad command byte for client 0"

This is caused by having too large a map/bot collection. Actually it's the
count of PK3 files within baseq3 and the mod directory that reaches an
internal limit, overflowing the data and giving the "bad command byte" error
message.

This doesn't happen when you connect to a public server because they have
a much smaller set of files that you need to verify (generally speaking,
server operators don't have large rambling collections of files on their
machines :)

Possible solutions:

  i) Clean out your map collection
 ii) Clean out your bot collection
iii) Consolidate your maps/bots into new PK3 files
 iv) Make sure that the pure server option in the skirmish menu is off
 
Step iii) will have implications with purity if you play online (only
if that server uses a map you've modified). As 1.30 prevents custom 
models from being used on servers, consolidating your model collection
is probably a good first thing to try.



-----------------------
Some Questions Answered
-----------------------

Q: Why don't some maps show up?

A: THIS PROBLEM WAS BYPASSED IN 0.99a: maps that don't have arena scripts
   can still be selected. Obviously the best result is to still produce the
   arena script: you can recommend bots and gametypes for the map.


   Map properties need to be announced to Quake 3 by some means, something
   that isn't done from within the map file itself.

   The old way of doing things was to provide a replacement file called
   "arenas.txt" that contained map name information, bots, and game type(s)
   supported by that map. Only one of these files can be active at any one
   time, so this causes a conflict between map makers as they don't know about
   other released maps.

   The new way of doing things is co-operative and doesn't cause conflict.
   Each map can create it's own script that announces itself to Quake 3. I'll
   take you through the steps for creating one of these files (it's very
   easy!)

   For a map in an archive called testmap.pk3, loaded from the console
   by typing "map testmap2b", and called "Test map - version 2b" you would do
   the following:

   1) Create a directory called "scripts" (no quotes) in your Quake 3 baseq3
   directory (you've probably already got one).

   2) Create a file in that directory called "testmap.arena" (no quotes). It's
   best to match the name of the .pk3 file that contains the map. At the very
   least it must be unique.

   3) Put the following into the testmap.arena file:

	 {
	 map             "testmap2b"
	 bots            "grunt keel visor"
	 longname        "Test map - version 2b"
	 fraglimit       20
	 type            "ffa tourney team ctf"
	 }

   Replace these example values with the appropriate values for your map file.

   Make sure that "type" describes the gametypes supported by that map, so
   remove the ones that aren't needed.

   You can put entries for multiple maps in each .arena file, so long as each
   is contained within its own bracket pair {}.

   4) If you find an arenas.txt file in that map .pk3 then consider removing
   it. You can also use the entry in that file for the map to create the
   .arena file. Just make sure that you take out "single" from the "type"
   line, its used as part of the single player game and only map packs that
   provide a replacement tier should really use it.

   Don't remove the arenas.txt files from your original Quake 3 files,
   nor from the ThreeWave CTF add-on pak!

   5) Restart Quake 3, the map should appear in the map selection page.

   

Q: How can I setup a LAN party using UI Enhanced?
A: Make sure that each of the clients that will connect to your server have
   the same version of UI Enhanced on their machines. It should be installed
   on their machine in the same way it is installed on the server.

   This only really applies if you're running a pure server.

   Make sure you allow enough open slots for everyone to join in.

   You can also save scripts generated by UIE, put them in baseq3, and run
   a normal Q3 server.


Q: Can I get UI Enhanced in DLL form?
A: There's little or no performance advantage to using binaries of the
   ui.qvm. The performance benefit comes from the client and server.
   Besides, the only time you're using the user interface is when you're
   in a menu... just how fast can you type or use a mouse?


Q: How do I un-install UI Enhanced?
A: If you installed UI Enhanced into it's own directory, just remove
   that directory and any shortcuts that start up UI Enhanced.

   If you installed into "baseq3" or another mod directory then remove
   the .pk3 file that contains UI Enhanced only. Some values are also
   stored in the "q3config.cfg" file in that directory, they won't affect
   gameplay in any way. For completeness they can be removed by deleting
   all lines that set a variable that begins "uie_" (no quotes). You should
   also remove UIE added PK3 files that begin "zzz_uie_mapdata" (if present).

   Some additional data is stored in text files: description.txt, usermaps.txt,
   history.txt, readme.txt.


Q: Can I use scripts created by UI Enhanced with other mods or original Q3?
A: Yes, but there are several things you should be aware of:

   a) UIE won't know about any custom cvars set by a mod. It might be possible
   that UIE uses a cvar or command in a way that the mod has "modified", and
   hence broken. The only way to find out is to try it!

   b) UIE won't know about any custom maps that the mod has created either.
   You can manually modify the script by replacing map names if you wish.

   c) The script can *only* be created in the directory where UIE is
   installed. You'll have to copy the script into the mod directory or
   baseq3 yourself.

   This is because of a security loop-hole in the VM code that Id
   had to plug. This fix limits the creation of new files to the mod
   directory itself. If the loop-hole had been left open then a
   malicious VM could have modified files in use by another mod, or
   even files outside the Q3 directory.



--------------
Redistribution
--------------

UI Enhanced may be distributed freely via any electronic medium (including but
not limited to http or ftp) provided such distribution is of the original
archive, intact and unchanged.

UI Enhanced may be distributed on the front cover disk for any magazine
provided such distribution is of the original archive, intact and unchanged.
THE AUTHOR REQUESTS NOTIFICATION OF THIS DISTRIBUTION. A sample copy of
the magazine would also be appreciated, or a URL link to the magazine
web site.

If a physical media is required for the distribution of UI Enhanced then
the only charge that can be made is for media cost.

Any person or body using any form of Internet distribution may not limit
or control access by requiring a download or copying "fee" to gain access
to the archive.

Any distribution using a particular medium shall not restrict my rights to
distribute UI Enhanced using that or any other medium.



Please contact me with a description of your situation if it isn't covered.



-----
Legal
-----

THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
THE AUTHOR FURTHER DISCLAIMS ALL IMPLIED WARRANTIES INCLUDING WITHOUT
LIMITATION ANY IMPLIED WARRANTIES OF MERCHANTABILITY OR OF FITNESS
FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK ARISING OUT OF THE USE OR
PERFORMANCE OF THE SOFTWARE AND DOCUMENTATION REMAINS WITH YOU.

IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DAMAGES WHATSOEVER
(INCLUDING WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY
LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THE SOFTWARE OR
DOCUMENTATION EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY
OF SUCH DAMAGES.
