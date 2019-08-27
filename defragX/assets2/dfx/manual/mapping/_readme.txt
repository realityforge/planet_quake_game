important informations for mappers:

to use the entities of defrag x:
	-place entities.ent where your radiant is installed under the subfolder ..\q3.game\baseq3\
	-you should rename the original file to entities.ent.bak for backup
	
to let radiant include/use PNG images:
	-place q3.game where your radiant is installed under the subfolder ..\games\
	
to use the new surfaceparms:
	-put custinfoparms.txt into your mapping quake to ..\scripts\ folder
	-either add "-custinfoparms" to the q3map command line when compiling the map...example: ! "D:/q3/gtk/q3map2" -v # -game quake3 -fs_basepath "d:/q3//" -meta -custinfoparms $
	-or simply use the build_menu.xml preset, copy this one into your radiant program folder in subdir ..\settings\1.5.0\q3.game\
	