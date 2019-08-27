// Copyright (C) 1999-2000 Id Software, Inc.
//
// This file must be identical in the quake and utils directories

// contents flags are seperate bits
// a given brush can contribute multiple content bits

// these definitions also need to be in q_shared.h!

#define	CONTENTS_SOLID			1		// an eye is never valid in a solid
#define	CONTENTS_LAVA			8
#define	CONTENTS_SLIME			16
#define	CONTENTS_WATER			32
#define	CONTENTS_FOG			64

#define CONTENTS_NOTTEAM1		0x0080
#define CONTENTS_NOTTEAM2		0x0100
#define CONTENTS_NOBOTCLIP		0x0200

#define	CONTENTS_AREAPORTAL		0x8000

#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_SPAWNAREA		0x20000     //can spawn anywhere inside this brush [original MONSTERCLIP]

#define	CONTENTS_TELEPORTER		0x40000     //bot specific contents types
#define	CONTENTS_JUMPPAD		0x80000
#define CONTENTS_CLUSTERPORTAL	0x100000
#define CONTENTS_GRAVITY		0x200000	//gravity  [original DONOTENTER]
#define CONTENTS_BOTCLIP		0x400000
#define CONTENTS_MOVER			0x800000

#define	CONTENTS_ORIGIN			0x1000000	// removed before bsping an entity

#define	CONTENTS_BODY			0x2000000	// should never be on a brush, only in game
#define	CONTENTS_CORPSE			0x4000000
#define	CONTENTS_DETAIL			0x8000000	// brushes not used for the bsp
#define	CONTENTS_STRUCTURAL		0x10000000	// brushes used for the bsp
#define	CONTENTS_TRANSLUCENT	0x20000000	// don't consume surface fragments inside
#define	CONTENTS_TRIGGER		0x40000000
#define	CONTENTS_NODROP			0x80000000	// don't leave bodies or items (death fog, lava)
#define	CONTENTS_NODE			0xFFFFFFFF

#define	SURF_NODAMAGE			0x1		 // never give falling damage
#define	SURF_SLICK				0x2		 // effects game physics
#define	SURF_SKY				0x4		 // lighting from environment map
#define	SURF_LADDER				0x8      // climbable surface, like a ladder
#define	SURF_NOIMPACT			0x10	 // don't make missile explosions
#define	SURF_NOMARKS			0x20	 // don't leave missile marks
#define	SURF_FLESH				0x40	 // make flesh sounds and effects
#define	SURF_NODRAW				0x80	 // don't generate a drawsurface at all
#define	SURF_HINT				0x100	 // make a primary bsp splitter
#define	SURF_SKIP				0x200	 // completely ignore, allowing non-closed brushes
#define	SURF_NOLIGHTMAP			0x400	 // surface doesn't need a lightmap
#define	SURF_POINTLIGHT			0x800	 // generate lighting info at vertexes
#define	SURF_METALSTEPS			0x1000	 // clanking footsteps
#define	SURF_NOSTEPS			0x2000	 // no footstep sounds
#define	SURF_NONSOLID			0x4000	 // don't collide against curves with this set
#define	SURF_LIGHTFILTER		0x8000	 // act as a light filter during q3map -light
#define	SURF_ALPHASHADOW		0x10000	 // do per-pixel light shadow casting in q3map
#define	SURF_NODLIGHT			0x20000	 // don't dlight even if solid (solid lava, skies)
#define SURF_SAND              	0x40000  // was SURF_DUST, sand sounds, also show dust particles
#define SURF_NOOB				0x80000  // don't allow overbounces on this surface
#define SURF_WOOD               0x100000 // make wood step sounds, only can use grapple on this one in GT_ADVENTURE
#define SURF_GRASS              0x200000 // make grass Step sounds
#define SURF_STONE              0x400000 // make rock sounds
#define SURF_TALLGRASS          0x800000 // make tall grass sounds
#define SURF_HOT                0x1000000// make burn sounds
#define SURF_ICE                0x2000000// make ice sounds
