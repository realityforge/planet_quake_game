Title:    Mr. Pants' Excessive Overkill for Quake III Arena Source Release
Author:   Dan "Mr. Pants" Schoenblum <dan@schoenblum.net>
Date:     3-20-04
Homepage: http://killerrobotsfromhell.com/excessive/
Forum:    http://www.forumplanet.com/planetquake/excessive/forum.asp?fid=7306

This is the source code for Mr. Pants' Excessive Overkill for Quake III Arena.
The code may be used for any non-commercial purposes.

No support is offered for this release.  If you have questions, try the
"ExcessiveQ3 Source" forum hosted at forumplanet.com:
http://www.forumplanet.com/planetquake/excessive/forum.asp?fid=7306

This source is for Release 4 of ExcessiveQ3, and is based on the v1.32 Q3
source, which is available here:
http://www.fileplanet.com/files/40000/42054.shtml

This package contains only the files that were changed for Excessive.  All
changes are marked with a "//PANTS" or "// PANTS" comment.  The best way to
see all of the changes is with a "diff" tool.  A good tool for doing this on
Windows is Araxis Merge.  A free, fully functional trial version is availabe
here:
http://www.araxis.com/merge/

The first thing to do when making a custom version of Excessive is to go to
line 206 in g_main.c, which looks like this:
#define EX_CUSTOM_STR    "Custom Excessive"
and change the string to whatever you would like your custom version called.