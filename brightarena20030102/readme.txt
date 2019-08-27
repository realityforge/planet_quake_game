***        BrightArena Mod        *** 
***                               *** 
***  www.planetquake.com/modifia  *** 



This package contains the source code of the Q3A modification
"BrightArena". It's of no use for you unless you're a programmer.

Only those files have been included that are different from the
PR 1.32 source code. All changes have been marked by "JUHOX".
Just search for this string if you want to find all changes.

However, if you're interested in certain features only, there's a
better way. To the beginning of "bg_public.h" there're some
defines tagged as "subsystem control". These defines are:

- MAPLENSFLARES
    Controls whether lens flares for map lights are displayed.

- MISSILELENSFLARES
    Controls whether lens flares for missiles are displayed.

- LFEDITOR
    Controls whether the lens flare editor is included.

You can search for these strings to find all changes needed for
a certain feature. For example, changes needed to display lens
flares for map lights look like this:

#if MAPLENSFLARES
   ... additions needed ...
#endif


#if !MAPLENSFLARES
   ... original code no longer needed ...
#endif


#if !MAPLENSFLARES
   ... original code that needs to be modified ...
#else
   ... replacement ...
#endif


Note that most changes are needed for the lens flare editor.
There's no need to duplicate the lens flare editor in every mod,
so you can save yourself a lot of work by including the
MAPLENSFLARES and MISSILELENSFLARES subsytems only in your mod.


To make your mod completely BrightArena compatible, you also need
to include the files from the "additional_files" directory into
your mod.
