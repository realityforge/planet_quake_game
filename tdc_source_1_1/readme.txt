The Dark Conjunction
--------------------

(C) 2001-2002 ACE TEAM

Ace Team is (in short):

Andres & Carlos Bordeu: Game Design, Level Design, 2d & 3d Art, and more! (yes, they worked hard)
Juan Pablo Lastra: Lead Programmer (NPC Code, Save/Load, MD3 anim, weapons, and other little stuff)
Edmundo Bordeu: Level Design, 2d & 3d Art
Esteban Hurtado: Programmer (in-game special FX & gui, and other misc stuff)
Ana Maria Albornoz: Programmer (menus & helper gui)
Kevin Riepl: Music
Klaus Reiter: Sound Fx


PLEASE READ & FOLLOW THE LICENSE.TXT FILE. DO NOT MAKE US REGRET RELEASING THE TDC CODE.

Message to the programmers who downloaded this code:
----------------------------------------------------

First of all, read some tutorials (like the ones from code3arena in planetquake), and other things to gain a general understanding of how q3 code works. When you have finished such job and you feel comfortable with the code, go on looking the TDC code.

There are basically 6 things we have done to the q3 code to get TDC working:

1. Change the HUD & menu look to get the TDC mood
2. Add new weapons & "erase" the q3 ones... many q3 original code was reused, for example, the plasma gun was converted to the bat fire shot.
3. Add entities for special FX & single player management code, animated md3, like earthquakes, level-ends, and stuff like that
4. Remove some q3 code, such as the bot AI code, and much more... any case, there is a lot of code that is not used but was not removed
5. the hardest... the NPC code, there are three files involved in the server code, and one in the client one... the most important one is the g_npcthink.c, it has most of the AI of the NPCs.. g_npc.c has the entity code and some general stuff, and g_npcmove.c has the moving and physics code, almost a copy of the player movement code of original q3, but with some changes for the special movements of the bat, metlar or hulk..
6. other stuff like save/load states, music managment, etc.

JPL - Lead Programmer
