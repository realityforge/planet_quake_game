data
align 4
LABELV playerMins
byte 4 3245342720
byte 4 3245342720
byte 4 3250585600
align 4
LABELV playerMaxs
byte 4 1097859072
byte 4 1097859072
byte 4 1107296256
export SP_info_player_deathmatch
code
proc SP_info_player_deathmatch 8 12
file "../g_client.c"
line 17
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:#include "g_local.h"
;4:
;5:// g_client.c -- client functions that don't happen every frame
;6:
;7:static vec3_t	playerMins = {-15, -15, -24};
;8:static vec3_t	playerMaxs = {15, 15, 32};
;9:
;10:/*QUAKED info_player_deathmatch (1 0 1) (-16 -16 -24) (16 16 32) initial
;11:potential spawning position for deathmatch games.
;12:The first time a player enters the game, they will be at an 'initial' spot.
;13:Targets will be fired when someone spawns in on them.
;14:"nobots" will prevent bots from using this spot.
;15:"nohumans" will prevent non-bots from using this spot.
;16:*/
;17:void SP_info_player_deathmatch( gentity_t *ent ) {
line 20
;18:	int		i;
;19:
;20:	G_SpawnInt( "nobots", "0", &i);
ADDRGP4 $53
ARGP4
ADDRGP4 $54
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 21
;21:	if ( i ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $55
line 22
;22:		ent->flags |= FL_NO_BOTS;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 8192
BORI4
ASGNI4
line 23
;23:	}
LABELV $55
line 24
;24:	G_SpawnInt( "nohumans", "0", &i );
ADDRGP4 $57
ARGP4
ADDRGP4 $54
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 25
;25:	if ( i ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $58
line 26
;26:		ent->flags |= FL_NO_HUMANS;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
line 27
;27:	}
LABELV $58
line 28
;28:}
LABELV $52
endproc SP_info_player_deathmatch 8 12
export SP_info_player_start
proc SP_info_player_start 0 4
line 33
;29:
;30:/*QUAKED info_player_start (1 0 0) (-16 -16 -24) (16 16 32)
;31:equivelant to info_player_deathmatch
;32:*/
;33:void SP_info_player_start(gentity_t *ent) {
line 34
;34:	ent->classname = "info_player_deathmatch";
ADDRFP4 0
INDIRP4
CNSTI4 524
ADDP4
ADDRGP4 $61
ASGNP4
line 35
;35:	SP_info_player_deathmatch( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 SP_info_player_deathmatch
CALLV
pop
line 36
;36:}
LABELV $60
endproc SP_info_player_start 0 4
export SP_info_player_intermission
proc SP_info_player_intermission 0 0
line 41
;37:
;38:/*QUAKED info_player_intermission (1 0 1) (-16 -16 -24) (16 16 32)
;39:The intermission will be viewed from this point.  Target an info_notnull for the view direction.
;40:*/
;41:void SP_info_player_intermission( gentity_t *ent ) {
line 43
;42:
;43:}
LABELV $62
endproc SP_info_player_intermission 0 0
export SpotWouldTelefrag
proc SpotWouldTelefrag 4144 16
line 61
;44:
;45:
;46:
;47:/*
;48:=======================================================================
;49:
;50:  SelectSpawnPoint
;51:
;52:=======================================================================
;53:*/
;54:
;55:/*
;56:================
;57:SpotWouldTelefrag
;58:
;59:================
;60:*/
;61:qboolean SpotWouldTelefrag( gentity_t *spot ) {
line 67
;62:	int			i, num;
;63:	int			touch[MAX_GENTITIES];
;64:	gentity_t	*hit;
;65:	vec3_t		mins, maxs;
;66:
;67:	VectorAdd( spot->s.origin, playerMins, mins );
ADDRLP4 4132
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4108
ADDRLP4 4132
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRGP4 playerMins
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4108+4
ADDRLP4 4132
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRGP4 playerMins+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4108+8
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRGP4 playerMins+8
INDIRF4
ADDF4
ASGNF4
line 68
;68:	VectorAdd( spot->s.origin, playerMaxs, maxs );
ADDRLP4 4136
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4120
ADDRLP4 4136
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRGP4 playerMaxs
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4120+4
ADDRLP4 4136
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRGP4 playerMaxs+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4120+8
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRGP4 playerMaxs+8
INDIRF4
ADDF4
ASGNF4
line 69
;69:	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );
ADDRLP4 4108
ARGP4
ADDRLP4 4120
ARGP4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4140
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 4140
INDIRI4
ASGNI4
line 71
;70:
;71:	for (i=0 ; i<num ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $75
JUMPV
LABELV $72
line 72
;72:		hit = &g_entities[touch[i]];
ADDRLP4 4
CNSTI4 808
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 12
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 74
;73:		//if ( hit->client && hit->client->ps.stats[STAT_HEALTH] > 0 ) {
;74:		if ( hit->client) {
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $76
line 75
;75:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $63
JUMPV
LABELV $76
line 78
;76:		}
;77:
;78:	}
LABELV $73
line 71
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $75
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $72
line 80
;79:
;80:	return qfalse;
CNSTI4 0
RETI4
LABELV $63
endproc SpotWouldTelefrag 4144 16
export SelectNearestDeathmatchSpawnPoint
proc SelectNearestDeathmatchSpawnPoint 40 12
line 91
;81:}
;82:
;83:/*
;84:================
;85:SelectNearestDeathmatchSpawnPoint
;86:
;87:Find the spot that we DON'T want to use
;88:================
;89:*/
;90:#define	MAX_SPAWN_POINTS	128
;91:gentity_t *SelectNearestDeathmatchSpawnPoint( vec3_t from ) {
line 97
;92:	gentity_t	*spot;
;93:	vec3_t		delta;
;94:	float		dist, nearestDist;
;95:	gentity_t	*nearestSpot;
;96:
;97:	nearestDist = 999999;
ADDRLP4 20
CNSTF4 1232348144
ASGNF4
line 98
;98:	nearestSpot = NULL;
ADDRLP4 24
CNSTP4 0
ASGNP4
line 99
;99:	spot = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $80
JUMPV
LABELV $79
line 101
;100:
;101:	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
line 103
;102:
;103:		VectorSubtract( spot->s.origin, from, delta );
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 32
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
SUBF4
ASGNF4
line 104
;104:		dist = VectorLength( delta );
ADDRLP4 4
ARGP4
ADDRLP4 36
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 36
INDIRF4
ASGNF4
line 105
;105:		if ( dist < nearestDist ) {
ADDRLP4 16
INDIRF4
ADDRLP4 20
INDIRF4
GEF4 $84
line 106
;106:			nearestDist = dist;
ADDRLP4 20
ADDRLP4 16
INDIRF4
ASGNF4
line 107
;107:			nearestSpot = spot;
ADDRLP4 24
ADDRLP4 0
INDIRP4
ASGNP4
line 108
;108:		}
LABELV $84
line 109
;109:	}
LABELV $80
line 101
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 524
ARGI4
ADDRGP4 $61
ARGP4
ADDRLP4 28
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 28
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $79
line 111
;110:
;111:	return nearestSpot;
ADDRLP4 24
INDIRP4
RETP4
LABELV $78
endproc SelectNearestDeathmatchSpawnPoint 40 12
export SelectRandomDeathmatchSpawnPoint
proc SelectRandomDeathmatchSpawnPoint 532 12
line 123
;112:}
;113:
;114:
;115:/*
;116:================
;117:SelectRandomDeathmatchSpawnPoint
;118:
;119:go to a random point that doesn't telefrag
;120:================
;121:*/
;122:#define	MAX_SPAWN_POINTS	128
;123:gentity_t *SelectRandomDeathmatchSpawnPoint( void ) {
line 129
;124:	gentity_t	*spot;
;125:	int			count;
;126:	int			selection;
;127:	gentity_t	*spots[MAX_SPAWN_POINTS];
;128:
;129:	count = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 130
;130:	spot = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $88
JUMPV
LABELV $87
line 132
;131:
;132:	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
line 133
;133:		if ( SpotWouldTelefrag( spot ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 524
ADDRGP4 SpotWouldTelefrag
CALLI4
ASGNI4
ADDRLP4 524
INDIRI4
CNSTI4 0
EQI4 $90
line 134
;134:			continue;
ADDRGP4 $88
JUMPV
LABELV $90
line 136
;135:		}
;136:		spots[ count ] = spot;
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 137
;137:		count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 138
;138:	}
LABELV $88
line 132
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 524
ARGI4
ADDRGP4 $61
ARGP4
ADDRLP4 524
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 524
INDIRP4
ASGNP4
ADDRLP4 524
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $87
line 140
;139:
;140:	if ( !count ) {	// no spots that won't telefrag
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $92
line 141
;141:		return G_Find( NULL, FOFS(classname), "info_player_deathmatch");
CNSTP4 0
ARGP4
CNSTI4 524
ARGI4
ADDRGP4 $61
ARGP4
ADDRLP4 528
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 528
INDIRP4
RETP4
ADDRGP4 $86
JUMPV
LABELV $92
line 144
;142:	}
;143:
;144:	selection = rand() % count;
ADDRLP4 528
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 520
ADDRLP4 528
INDIRI4
ADDRLP4 4
INDIRI4
MODI4
ASGNI4
line 145
;145:	return spots[ selection ];
ADDRLP4 520
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRP4
RETP4
LABELV $86
endproc SelectRandomDeathmatchSpawnPoint 532 12
export SelectRandomFurthestSpawnPoint
proc SelectRandomFurthestSpawnPoint 572 12
line 155
;146:}
;147:
;148:/*
;149:===========
;150:SelectRandomFurthestSpawnPoint
;151:
;152:Chooses a player start, deathmatch start, etc
;153:============
;154:*/
;155:gentity_t *SelectRandomFurthestSpawnPoint ( vec3_t avoidPoint, vec3_t origin, vec3_t angles ) {
line 163
;156:	gentity_t	*spot;
;157:	vec3_t		delta;
;158:	float		dist;
;159:	float		list_dist[64];
;160:	gentity_t	*list_spot[64];
;161:	int			numSpots, rnd, i, j;
;162:
;163:	numSpots = 0;
ADDRLP4 520
CNSTI4 0
ASGNI4
line 164
;164:	spot = NULL;
ADDRLP4 528
CNSTP4 0
ASGNP4
ADDRGP4 $96
JUMPV
LABELV $95
line 166
;165:
;166:	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
line 167
;167:		if ( SpotWouldTelefrag( spot ) ) {
ADDRLP4 528
INDIRP4
ARGP4
ADDRLP4 548
ADDRGP4 SpotWouldTelefrag
CALLI4
ASGNI4
ADDRLP4 548
INDIRI4
CNSTI4 0
EQI4 $98
line 168
;168:			continue;
ADDRGP4 $96
JUMPV
LABELV $98
line 170
;169:		}
;170:		VectorSubtract( spot->s.origin, avoidPoint, delta );
ADDRLP4 556
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 532
ADDRLP4 528
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 556
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 532+4
ADDRLP4 528
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 556
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 532+8
ADDRLP4 528
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
SUBF4
ASGNF4
line 171
;171:		dist = VectorLength( delta );
ADDRLP4 532
ARGP4
ADDRLP4 560
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 524
ADDRLP4 560
INDIRF4
ASGNF4
line 172
;172:		for (i = 0; i < numSpots; i++) {
ADDRLP4 516
CNSTI4 0
ASGNI4
ADDRGP4 $105
JUMPV
LABELV $102
line 173
;173:			if ( dist > list_dist[i] ) {
ADDRLP4 524
INDIRF4
ADDRLP4 516
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRF4
LEF4 $106
line 174
;174:				if ( numSpots >= 64 )
ADDRLP4 520
INDIRI4
CNSTI4 64
LTI4 $108
line 175
;175:					numSpots = 64-1;
ADDRLP4 520
CNSTI4 63
ASGNI4
LABELV $108
line 176
;176:				for (j = numSpots; j > i; j--) {
ADDRLP4 0
ADDRLP4 520
INDIRI4
ASGNI4
ADDRGP4 $113
JUMPV
LABELV $110
line 177
;177:					list_dist[j] = list_dist[j-1];
ADDRLP4 564
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 564
INDIRI4
ADDRLP4 4
ADDP4
ADDRLP4 564
INDIRI4
ADDRLP4 4-4
ADDP4
INDIRF4
ASGNF4
line 178
;178:					list_spot[j] = list_spot[j-1];
ADDRLP4 568
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 568
INDIRI4
ADDRLP4 260
ADDP4
ADDRLP4 568
INDIRI4
ADDRLP4 260-4
ADDP4
INDIRP4
ASGNP4
line 179
;179:				}
LABELV $111
line 176
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
LABELV $113
ADDRLP4 0
INDIRI4
ADDRLP4 516
INDIRI4
GTI4 $110
line 180
;180:				list_dist[i] = dist;
ADDRLP4 516
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
ADDRLP4 524
INDIRF4
ASGNF4
line 181
;181:				list_spot[i] = spot;
ADDRLP4 516
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
ADDRLP4 528
INDIRP4
ASGNP4
line 182
;182:				numSpots++;
ADDRLP4 520
ADDRLP4 520
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 183
;183:				if (numSpots > 64)
ADDRLP4 520
INDIRI4
CNSTI4 64
LEI4 $104
line 184
;184:					numSpots = 64;
ADDRLP4 520
CNSTI4 64
ASGNI4
line 185
;185:				break;
ADDRGP4 $104
JUMPV
LABELV $106
line 187
;186:			}
;187:		}
LABELV $103
line 172
ADDRLP4 516
ADDRLP4 516
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $105
ADDRLP4 516
INDIRI4
ADDRLP4 520
INDIRI4
LTI4 $102
LABELV $104
line 188
;188:		if (i >= numSpots && numSpots < 64) {
ADDRLP4 516
INDIRI4
ADDRLP4 520
INDIRI4
LTI4 $118
ADDRLP4 520
INDIRI4
CNSTI4 64
GEI4 $118
line 189
;189:			list_dist[numSpots] = dist;
ADDRLP4 520
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
ADDRLP4 524
INDIRF4
ASGNF4
line 190
;190:			list_spot[numSpots] = spot;
ADDRLP4 520
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
ADDRLP4 528
INDIRP4
ASGNP4
line 191
;191:			numSpots++;
ADDRLP4 520
ADDRLP4 520
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 192
;192:		}
LABELV $118
line 193
;193:	}
LABELV $96
line 166
ADDRLP4 528
INDIRP4
ARGP4
CNSTI4 524
ARGI4
ADDRGP4 $61
ARGP4
ADDRLP4 548
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 528
ADDRLP4 548
INDIRP4
ASGNP4
ADDRLP4 548
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $95
line 194
;194:	if (!numSpots) {
ADDRLP4 520
INDIRI4
CNSTI4 0
NEI4 $120
line 195
;195:		spot = G_Find( NULL, FOFS(classname), "info_player_deathmatch");
CNSTP4 0
ARGP4
CNSTI4 524
ARGI4
ADDRGP4 $61
ARGP4
ADDRLP4 552
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 528
ADDRLP4 552
INDIRP4
ASGNP4
line 196
;196:		if (!spot)
ADDRLP4 528
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $122
line 197
;197:			G_Error( "Couldn't find a spawn point" );
ADDRGP4 $124
ARGP4
ADDRGP4 G_Error
CALLV
pop
LABELV $122
line 198
;198:		VectorCopy (spot->s.origin, origin);
ADDRFP4 4
INDIRP4
ADDRLP4 528
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 199
;199:		origin[2] += 9;
ADDRLP4 556
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 556
INDIRP4
ADDRLP4 556
INDIRP4
INDIRF4
CNSTF4 1091567616
ADDF4
ASGNF4
line 200
;200:		VectorCopy (spot->s.angles, angles);
ADDRFP4 8
INDIRP4
ADDRLP4 528
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 201
;201:		return spot;
ADDRLP4 528
INDIRP4
RETP4
ADDRGP4 $94
JUMPV
LABELV $120
line 205
;202:	}
;203:
;204:	// select a random spot from the spawn points furthest away
;205:	rnd = random() * (numSpots / 2);
ADDRLP4 552
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 544
ADDRLP4 552
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 520
INDIRI4
CNSTI4 2
DIVI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 207
;206:
;207:	VectorCopy (list_spot[rnd]->s.origin, origin);
ADDRFP4 4
INDIRP4
ADDRLP4 544
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 208
;208:	origin[2] += 9;
ADDRLP4 556
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 556
INDIRP4
ADDRLP4 556
INDIRP4
INDIRF4
CNSTF4 1091567616
ADDF4
ASGNF4
line 209
;209:	VectorCopy (list_spot[rnd]->s.angles, angles);
ADDRFP4 8
INDIRP4
ADDRLP4 544
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 211
;210:
;211:	return list_spot[rnd];
ADDRLP4 544
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
INDIRP4
RETP4
LABELV $94
endproc SelectRandomFurthestSpawnPoint 572 12
export SelectSpawnPoint
proc SelectSpawnPoint 4 12
line 221
;212:}
;213:
;214:/*
;215:===========
;216:SelectSpawnPoint
;217:
;218:Chooses a player start, deathmatch start, etc
;219:============
;220:*/
;221:gentity_t *SelectSpawnPoint ( vec3_t avoidPoint, vec3_t origin, vec3_t angles ) {
line 222
;222:	return SelectRandomFurthestSpawnPoint( avoidPoint, origin, angles );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 SelectRandomFurthestSpawnPoint
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
RETP4
LABELV $125
endproc SelectSpawnPoint 4 12
export SelectInitialSpawnPoint
proc SelectInitialSpawnPoint 20 12
line 261
;223:
;224:	/*
;225:	gentity_t	*spot;
;226:	gentity_t	*nearestSpot;
;227:
;228:	nearestSpot = SelectNearestDeathmatchSpawnPoint( avoidPoint );
;229:
;230:	spot = SelectRandomDeathmatchSpawnPoint ( );
;231:	if ( spot == nearestSpot ) {
;232:		// roll again if it would be real close to point of death
;233:		spot = SelectRandomDeathmatchSpawnPoint ( );
;234:		if ( spot == nearestSpot ) {
;235:			// last try
;236:			spot = SelectRandomDeathmatchSpawnPoint ( );
;237:		}		
;238:	}
;239:
;240:	// find a single player start spot
;241:	if (!spot) {
;242:		G_Error( "Couldn't find a spawn point" );
;243:	}
;244:
;245:	VectorCopy (spot->s.origin, origin);
;246:	origin[2] += 9;
;247:	VectorCopy (spot->s.angles, angles);
;248:
;249:	return spot;
;250:	*/
;251:}
;252:
;253:/*
;254:===========
;255:SelectInitialSpawnPoint
;256:
;257:Try to find a spawn point marked 'initial', otherwise
;258:use normal spawn selection.
;259:============
;260:*/
;261:gentity_t *SelectInitialSpawnPoint( vec3_t origin, vec3_t angles ) {
line 264
;262:	gentity_t	*spot;
;263:
;264:	spot = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $128
JUMPV
LABELV $127
line 265
;265:	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
line 266
;266:		if ( spot->spawnflags & 1 ) {
ADDRLP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $130
line 267
;267:			break;
ADDRGP4 $129
JUMPV
LABELV $130
line 269
;268:		}
;269:	}
LABELV $128
line 265
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 524
ARGI4
ADDRGP4 $61
ARGP4
ADDRLP4 4
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $127
LABELV $129
line 271
;270:
;271:	if ( !spot || SpotWouldTelefrag( spot ) ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $134
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 SpotWouldTelefrag
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $132
LABELV $134
line 272
;272:		return SelectSpawnPoint( vec3_origin, origin, angles );
ADDRGP4 vec3_origin
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 SelectSpawnPoint
CALLP4
ASGNP4
ADDRLP4 16
INDIRP4
RETP4
ADDRGP4 $126
JUMPV
LABELV $132
line 275
;273:	}
;274:
;275:	VectorCopy (spot->s.origin, origin);
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 276
;276:	origin[2] += 9;
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRF4
CNSTF4 1091567616
ADDF4
ASGNF4
line 277
;277:	VectorCopy (spot->s.angles, angles);
ADDRFP4 4
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 279
;278:
;279:	return spot;
ADDRLP4 0
INDIRP4
RETP4
LABELV $126
endproc SelectInitialSpawnPoint 20 12
export SelectSpectatorSpawnPoint
proc SelectSpectatorSpawnPoint 0 0
line 288
;280:}
;281:
;282:/*
;283:===========
;284:SelectSpectatorSpawnPoint
;285:
;286:============
;287:*/
;288:gentity_t *SelectSpectatorSpawnPoint( vec3_t origin, vec3_t angles ) {
line 289
;289:	FindIntermissionPoint();
ADDRGP4 FindIntermissionPoint
CALLV
pop
line 291
;290:
;291:	VectorCopy( level.intermission_origin, origin );
ADDRFP4 0
INDIRP4
ADDRGP4 level+9148
INDIRB
ASGNB 12
line 292
;292:	VectorCopy( level.intermission_angle, angles );
ADDRFP4 4
INDIRP4
ADDRGP4 level+9160
INDIRB
ASGNB 12
line 294
;293:
;294:	return NULL;
CNSTP4 0
RETP4
LABELV $135
endproc SelectSpectatorSpawnPoint 0 0
export InitBodyQue
proc InitBodyQue 12 0
line 310
;295:}
;296:
;297:/*
;298:=======================================================================
;299:
;300:BODYQUE
;301:
;302:=======================================================================
;303:*/
;304:
;305:/*
;306:===============
;307:InitBodyQue
;308:===============
;309:*/
;310:void InitBodyQue (void) {
line 314
;311:	int		i;
;312:	gentity_t	*ent;
;313:
;314:	level.bodyQueIndex = 0;
ADDRGP4 level+9180
CNSTI4 0
ASGNI4
line 315
;315:	for (i=0; i<BODY_QUEUE_SIZE ; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $140
line 316
;316:		ent = G_Spawn();
ADDRLP4 8
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 317
;317:		ent->classname = "bodyque";
ADDRLP4 0
INDIRP4
CNSTI4 524
ADDP4
ADDRGP4 $144
ASGNP4
line 318
;318:		ent->neverFree = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 532
ADDP4
CNSTI4 1
ASGNI4
line 319
;319:		level.bodyQue[i] = ent;
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 level+9184
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 320
;320:	}
LABELV $141
line 315
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 8
LTI4 $140
line 321
;321:}
LABELV $138
endproc InitBodyQue 12 0
export BodySink
proc BodySink 4 4
line 330
;322:
;323:/*
;324:=============
;325:BodySink
;326:
;327:After sitting around for five seconds, fall into the ground and dissapear
;328:=============
;329:*/
;330:void BodySink( gentity_t *ent ) {
line 331
;331:	if ( level.time - ent->timestamp > 6500 ) {
ADDRGP4 level+36
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 640
ADDP4
INDIRI4
SUBI4
CNSTI4 6500
LEI4 $147
line 333
;332:		// the body ques are never actually freed, they are just unlinked
;333:		trap_UnlinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 334
;334:		ent->physicsObject = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 564
ADDP4
CNSTI4 0
ASGNI4
line 335
;335:		return;	
ADDRGP4 $146
JUMPV
LABELV $147
line 337
;336:	}
;337:	ent->nextthink = level.time + 100;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 338
;338:	ent->s.pos.trBase[2] -= 1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
CNSTF4 1065353216
SUBF4
ASGNF4
line 339
;339:}
LABELV $146
endproc BodySink 4 4
export CopyToBodyQue
proc CopyToBodyQue 48 8
line 349
;340:
;341:/*
;342:=============
;343:CopyToBodyQue
;344:
;345:A player is respawning, so make an entity that looks
;346:just like the existing corpse to leave behind.
;347:=============
;348:*/
;349:void CopyToBodyQue( gentity_t *ent ) {
line 357
;350:#ifdef MISSIONPACK
;351:	gentity_t	*e;
;352:	int i;
;353:#endif
;354:	gentity_t		*body;
;355:	int			contents;
;356:
;357:	trap_UnlinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 360
;358:
;359:	// if client is in a nodrop area, don't leave the body
;360:	contents = trap_PointContents( ent->s.origin, -1 );
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 8
ADDRGP4 trap_PointContents
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRI4
ASGNI4
line 361
;361:	if ( contents & CONTENTS_NODROP ) {
ADDRLP4 4
INDIRI4
CVIU4 4
CNSTU4 2147483648
BANDU4
CNSTU4 0
EQU4 $152
line 362
;362:		return;
ADDRGP4 $151
JUMPV
LABELV $152
line 366
;363:	}
;364:
;365:	// grab a body que and cycle to the next one
;366:	body = level.bodyQue[ level.bodyQueIndex ];
ADDRLP4 0
ADDRGP4 level+9180
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 level+9184
ADDP4
INDIRP4
ASGNP4
line 367
;367:	level.bodyQueIndex = (level.bodyQueIndex + 1) % BODY_QUEUE_SIZE;
ADDRGP4 level+9180
ADDRGP4 level+9180
INDIRI4
CNSTI4 1
ADDI4
CNSTI4 8
MODI4
ASGNI4
line 369
;368:
;369:	trap_UnlinkEntity (body);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 371
;370:
;371:	body->s = ent->s;
ADDRLP4 0
INDIRP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 208
line 372
;372:	body->s.eFlags = EF_DEAD;		// clear EF_TALK, etc
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 1
ASGNI4
line 391
;373:#ifdef MISSIONPACK
;374:	if ( ent->s.eFlags & EF_KAMIKAZE ) {
;375:		body->s.eFlags |= EF_KAMIKAZE;
;376:
;377:		// check if there is a kamikaze timer around for this owner
;378:		for (i = 0; i < MAX_GENTITIES; i++) {
;379:			e = &g_entities[i];
;380:			if (!e->inuse)
;381:				continue;
;382:			if (e->activator != ent)
;383:				continue;
;384:			if (strcmp(e->classname, "kamikaze timer"))
;385:				continue;
;386:			e->activator = body;
;387:			break;
;388:		}
;389:	}
;390:#endif
;391:	body->s.powerups = 0;	// clear powerups
ADDRLP4 0
INDIRP4
CNSTI4 188
ADDP4
CNSTI4 0
ASGNI4
line 392
;392:	body->s.loopSound = 0;	// clear lava burning
ADDRLP4 0
INDIRP4
CNSTI4 156
ADDP4
CNSTI4 0
ASGNI4
line 393
;393:	body->s.number = body - g_entities;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 808
DIVI4
ASGNI4
line 394
;394:	body->timestamp = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 640
ADDP4
ADDRGP4 level+36
INDIRI4
ASGNI4
line 395
;395:	body->physicsObject = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 564
ADDP4
CNSTI4 1
ASGNI4
line 396
;396:	body->physicsBounce = 0;		// don't bounce
ADDRLP4 0
INDIRP4
CNSTI4 568
ADDP4
CNSTF4 0
ASGNF4
line 397
;397:	if ( body->s.groundEntityNum == ENTITYNUM_NONE ) {
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 1023
NEI4 $159
line 398
;398:		body->s.pos.trType = TR_GRAVITY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 399
;399:		body->s.pos.trTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+36
INDIRI4
ASGNI4
line 400
;400:		VectorCopy( ent->client->ps.velocity, body->s.pos.trDelta );
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 401
;401:	} else {
ADDRGP4 $160
JUMPV
LABELV $159
line 402
;402:		body->s.pos.trType = TR_STATIONARY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 403
;403:	}
LABELV $160
line 404
;404:	body->s.event = 0;
ADDRLP4 0
INDIRP4
CNSTI4 180
ADDP4
CNSTI4 0
ASGNI4
line 408
;405:
;406:	// change the animation to the last-frame only, so the sequence
;407:	// doesn't repeat anew for the body
;408:	switch ( body->s.legsAnim & ~ANIM_TOGGLEBIT ) {
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 -129
BANDI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
LTI4 $162
ADDRLP4 16
INDIRI4
CNSTI4 5
GTI4 $162
ADDRLP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $168
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $168
address $165
address $165
address $166
address $166
address $167
address $167
code
LABELV $165
line 411
;409:	case BOTH_DEATH1:
;410:	case BOTH_DEAD1:
;411:		body->s.torsoAnim = body->s.legsAnim = BOTH_DEAD1;
ADDRLP4 28
CNSTI4 1
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 412
;412:		break;
ADDRGP4 $163
JUMPV
LABELV $166
line 415
;413:	case BOTH_DEATH2:
;414:	case BOTH_DEAD2:
;415:		body->s.torsoAnim = body->s.legsAnim = BOTH_DEAD2;
ADDRLP4 36
CNSTI4 3
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 416
;416:		break;
ADDRGP4 $163
JUMPV
LABELV $167
LABELV $162
line 420
;417:	case BOTH_DEATH3:
;418:	case BOTH_DEAD3:
;419:	default:
;420:		body->s.torsoAnim = body->s.legsAnim = BOTH_DEAD3;
ADDRLP4 44
CNSTI4 5
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
line 421
;421:		break;
LABELV $163
line 424
;422:	}
;423:
;424:	body->r.svFlags = ent->r.svFlags;
ADDRLP4 24
CNSTI4 424
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 425
;425:	VectorCopy (ent->r.mins, body->r.mins);
ADDRLP4 28
CNSTI4 436
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 426
;426:	VectorCopy (ent->r.maxs, body->r.maxs);
ADDRLP4 32
CNSTI4 448
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 427
;427:	VectorCopy (ent->r.absmin, body->r.absmin);
ADDRLP4 36
CNSTI4 464
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 428
;428:	VectorCopy (ent->r.absmax, body->r.absmax);
ADDRLP4 40
CNSTI4 476
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 430
;429:
;430:	body->clipmask = CONTENTS_SOLID | CONTENTS_PLAYERCLIP;
ADDRLP4 0
INDIRP4
CNSTI4 572
ADDP4
CNSTI4 65537
ASGNI4
line 431
;431:	body->r.contents = CONTENTS_CORPSE;
ADDRLP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 67108864
ASGNI4
line 432
;432:	body->r.ownerNum = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 434
;433:
;434:	body->nextthink = level.time + 5000;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 5000
ADDI4
ASGNI4
line 435
;435:	body->think = BodySink;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRGP4 BodySink
ASGNP4
line 437
;436:
;437:	body->die = body_die;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
ADDRGP4 body_die
ASGNP4
line 440
;438:
;439:	// don't take more damage if already gibbed
;440:	if ( ent->health <= GIB_HEALTH ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 -40
GTI4 $170
line 441
;441:		body->takedamage = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 736
ADDP4
CNSTI4 0
ASGNI4
line 442
;442:	} else {
ADDRGP4 $171
JUMPV
LABELV $170
line 443
;443:		body->takedamage = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 736
ADDP4
CNSTI4 1
ASGNI4
line 444
;444:	}
LABELV $171
line 447
;445:
;446:
;447:	VectorCopy ( body->s.pos.trBase, body->r.currentOrigin );
ADDRLP4 0
INDIRP4
CNSTI4 488
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 448
;448:	trap_LinkEntity (body);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 449
;449:}
LABELV $151
endproc CopyToBodyQue 48 8
export SetClientViewAngle
proc SetClientViewAngle 16 0
line 460
;450:
;451://======================================================================
;452:
;453:
;454:/*
;455:==================
;456:SetClientViewAngle
;457:
;458:==================
;459:*/
;460:void SetClientViewAngle( gentity_t *ent, vec3_t angle ) {
line 464
;461:	int			i;
;462:
;463:	// set the delta angle
;464:	for (i=0 ; i<3 ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $173
line 467
;465:		int		cmdAngle;
;466:
;467:		cmdAngle = ANGLE2SHORT(angle[i]);
ADDRLP4 4
CNSTF4 1199570944
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRF4
MULF4
CNSTF4 1135869952
DIVF4
CVFI4 4
CNSTI4 65535
BANDI4
ASGNI4
line 468
;468:		ent->client->ps.delta_angles[i] = cmdAngle - ent->client->pers.cmd.angles[i];
ADDRLP4 8
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRP4
CNSTI4 56
ADDP4
ADDP4
ADDRLP4 4
INDIRI4
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRP4
CNSTI4 476
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 469
;469:	}
LABELV $174
line 464
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $173
line 470
;470:	VectorCopy( angle, ent->s.angles );
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 471
;471:	VectorCopy (ent->s.angles, ent->client->ps.viewangles);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 152
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 472
;472:}
LABELV $172
endproc SetClientViewAngle 16 0
export respawn
proc respawn 12 8
line 479
;473:
;474:/*
;475:================
;476:respawn
;477:================
;478:*/
;479:void respawn( gentity_t *ent ) {
line 482
;480:	gentity_t	*tent;
;481:
;482:	CopyToBodyQue (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CopyToBodyQue
CALLV
pop
line 483
;483:	ClientSpawn(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ClientSpawn
CALLV
pop
line 486
;484:
;485:	// add a teleportation effect
;486:	tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN );
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 42
ARGI4
ADDRLP4 4
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 487
;487:	tent->s.clientNum = ent->s.clientNum;
ADDRLP4 8
CNSTI4 168
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 488
;488:}
LABELV $177
endproc respawn 12 8
export TeamCount
proc TeamCount 8 0
line 497
;489:
;490:/*
;491:================
;492:TeamCount
;493:
;494:Returns number of players on a team
;495:================
;496:*/
;497:team_t TeamCount( int ignoreClientNum, int team ) {
line 499
;498:	int		i;
;499:	int		count = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 501
;500:
;501:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $182
JUMPV
LABELV $179
line 502
;502:		if ( i == ignoreClientNum ) {
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $184
line 503
;503:			continue;
ADDRGP4 $180
JUMPV
LABELV $184
line 505
;504:		}
;505:		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
CNSTI4 788
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 0
NEI4 $186
line 506
;506:			continue;
ADDRGP4 $180
JUMPV
LABELV $186
line 508
;507:		}
;508:		if ( level.clients[i].sess.sessionTeam == team ) {
CNSTI4 788
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 616
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $188
line 509
;509:			count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 510
;510:		}
LABELV $188
line 511
;511:	}
LABELV $180
line 501
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $182
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $179
line 513
;512:
;513:	return count;
ADDRLP4 4
INDIRI4
RETI4
LABELV $178
endproc TeamCount 8 0
export TeamLeader
proc TeamLeader 4 0
line 523
;514:}
;515:
;516:/*
;517:================
;518:TeamLeader
;519:
;520:Returns the client number of the team leader
;521:================
;522:*/
;523:int TeamLeader( int team ) {
line 526
;524:	int		i;
;525:
;526:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $194
JUMPV
LABELV $191
line 527
;527:		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
CNSTI4 788
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 0
NEI4 $196
line 528
;528:			continue;
ADDRGP4 $192
JUMPV
LABELV $196
line 530
;529:		}
;530:		if ( level.clients[i].sess.sessionTeam == team ) {
CNSTI4 788
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 616
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $198
line 531
;531:			if ( level.clients[i].sess.teamLeader )
CNSTI4 788
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 640
ADDP4
INDIRI4
CNSTI4 0
EQI4 $200
line 532
;532:				return i;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $190
JUMPV
LABELV $200
line 533
;533:		}
LABELV $198
line 534
;534:	}
LABELV $192
line 526
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $194
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $191
line 536
;535:
;536:	return -1;
CNSTI4 -1
RETI4
LABELV $190
endproc TeamLeader 4 0
export PickTeam
proc PickTeam 24 8
line 546
;537:}
;538:
;539:
;540:/*
;541:================
;542:PickTeam
;543:
;544:================
;545:*/
;546:team_t PickTeam( int ignoreClientNum ) {
line 549
;547:	int		counts[TEAM_NUM_TEAMS];
;548:
;549:	counts[TEAM_BLUE] = TeamCount( ignoreClientNum, TEAM_BLUE );
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 16
ADDRGP4 TeamCount
CALLI4
ASGNI4
ADDRLP4 0+8
ADDRLP4 16
INDIRI4
ASGNI4
line 550
;550:	counts[TEAM_RED] = TeamCount( ignoreClientNum, TEAM_RED );
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 20
ADDRGP4 TeamCount
CALLI4
ASGNI4
ADDRLP4 0+4
ADDRLP4 20
INDIRI4
ASGNI4
line 552
;551:
;552:	if ( counts[TEAM_BLUE] > counts[TEAM_RED] ) {
ADDRLP4 0+8
INDIRI4
ADDRLP4 0+4
INDIRI4
LEI4 $205
line 553
;553:		return TEAM_RED;
CNSTI4 1
RETI4
ADDRGP4 $202
JUMPV
LABELV $205
line 555
;554:	}
;555:	if ( counts[TEAM_RED] > counts[TEAM_BLUE] ) {
ADDRLP4 0+4
INDIRI4
ADDRLP4 0+8
INDIRI4
LEI4 $209
line 556
;556:		return TEAM_BLUE;
CNSTI4 2
RETI4
ADDRGP4 $202
JUMPV
LABELV $209
line 559
;557:	}
;558:	// equal team count, so join the team with the lowest score
;559:	if ( level.teamScores[TEAM_BLUE] > level.teamScores[TEAM_RED] ) {
ADDRGP4 level+48+8
INDIRI4
ADDRGP4 level+48+4
INDIRI4
LEI4 $213
line 560
;560:		return TEAM_RED;
CNSTI4 1
RETI4
ADDRGP4 $202
JUMPV
LABELV $213
line 562
;561:	}
;562:	return TEAM_BLUE;
CNSTI4 2
RETI4
LABELV $202
endproc PickTeam 24 8
proc ClientCleanName 40 12
line 590
;563:}
;564:
;565:/*
;566:===========
;567:ForceClientSkin
;568:
;569:Forces a client's skin (for teamplay)
;570:===========
;571:*/
;572:/*
;573:static void ForceClientSkin( gclient_t *client, char *model, const char *skin ) {
;574:	char *p;
;575:
;576:	if ((p = Q_strrchr(model, '/')) != 0) {
;577:		*p = 0;
;578:	}
;579:
;580:	Q_strcat(model, MAX_QPATH, "/");
;581:	Q_strcat(model, MAX_QPATH, skin);
;582:}
;583:*/
;584:
;585:/*
;586:===========
;587:ClientCheckName
;588:============
;589:*/
;590:static void ClientCleanName( const char *in, char *out, int outSize ) {
line 597
;591:	int		len, colorlessLen;
;592:	char	ch;
;593:	char	*p;
;594:	int		spaces;
;595:
;596:	//save room for trailing null byte
;597:	outSize--;
ADDRFP4 8
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 599
;598:
;599:	len = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 600
;600:	colorlessLen = 0;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 601
;601:	p = out;
ADDRLP4 12
ADDRFP4 4
INDIRP4
ASGNP4
line 602
;602:	*p = 0;
ADDRLP4 12
INDIRP4
CNSTI1 0
ASGNI1
line 603
;603:	spaces = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $221
JUMPV
LABELV $220
line 605
;604:
;605:	while( 1 ) {
line 606
;606:		ch = *in++;
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 20
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
INDIRI1
ASGNI1
line 607
;607:		if( !ch ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $223
line 608
;608:			break;
ADDRGP4 $222
JUMPV
LABELV $223
line 612
;609:		}
;610:
;611:		// don't allow leading spaces
;612:		if( !*p && ch == ' ' ) {
ADDRLP4 12
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $225
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 32
NEI4 $225
line 613
;613:			continue;
ADDRGP4 $221
JUMPV
LABELV $225
line 617
;614:		}
;615:
;616:		// check colors
;617:		if( ch == Q_COLOR_ESCAPE ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 94
NEI4 $227
line 619
;618:			// solo trailing carat is not a color prefix
;619:			if( !*in ) {
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $229
line 620
;620:				break;
ADDRGP4 $222
JUMPV
LABELV $229
line 624
;621:			}
;622:
;623:			// don't allow black in a name, period
;624:			if( ColorIndex(*in) == 0 ) {
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 48
SUBI4
CNSTI4 7
BANDI4
CNSTI4 0
NEI4 $231
line 625
;625:				in++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 626
;626:				continue;
ADDRGP4 $221
JUMPV
LABELV $231
line 630
;627:			}
;628:
;629:			// make sure room in dest for both chars
;630:			if( len > outSize - 2 ) {
ADDRLP4 4
INDIRI4
ADDRFP4 8
INDIRI4
CNSTI4 2
SUBI4
LEI4 $233
line 631
;631:				break;
ADDRGP4 $222
JUMPV
LABELV $233
line 634
;632:			}
;633:
;634:			*out++ = ch;
ADDRLP4 24
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 4
ADDRLP4 24
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 0
INDIRI1
ASGNI1
line 635
;635:			*out++ = *in++;
ADDRLP4 28
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 36
CNSTI4 1
ASGNI4
ADDRFP4 4
ADDRLP4 28
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
ASGNP4
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 32
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI1
ASGNI1
line 636
;636:			len += 2;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 2
ADDI4
ASGNI4
line 637
;637:			continue;
ADDRGP4 $221
JUMPV
LABELV $227
line 641
;638:		}
;639:
;640:		// don't allow too many consecutive spaces
;641:		if( ch == ' ' ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 32
NEI4 $235
line 642
;642:			spaces++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 643
;643:			if( spaces > 3 ) {
ADDRLP4 8
INDIRI4
CNSTI4 3
LEI4 $236
line 644
;644:				continue;
ADDRGP4 $221
JUMPV
line 646
;645:			}
;646:		}
LABELV $235
line 647
;647:		else {
line 648
;648:			spaces = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 649
;649:		}
LABELV $236
line 651
;650:
;651:		if( len > outSize - 1 ) {
ADDRLP4 4
INDIRI4
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
LEI4 $239
line 652
;652:			break;
ADDRGP4 $222
JUMPV
LABELV $239
line 655
;653:		}
;654:
;655:		*out++ = ch;
ADDRLP4 24
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 4
ADDRLP4 24
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 0
INDIRI1
ASGNI1
line 656
;656:		colorlessLen++;
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 657
;657:		len++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 658
;658:	}
LABELV $221
line 605
ADDRGP4 $220
JUMPV
LABELV $222
line 659
;659:	*out = 0;
ADDRFP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 662
;660:
;661:	// don't allow empty names
;662:	if( *p == 0 || colorlessLen == 0 ) {
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 12
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 20
INDIRI4
EQI4 $243
ADDRLP4 16
INDIRI4
ADDRLP4 20
INDIRI4
NEI4 $241
LABELV $243
line 663
;663:		Q_strncpyz( p, "UnnamedPlayer", outSize );
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 $244
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 664
;664:	}
LABELV $241
line 665
;665:}
LABELV $219
endproc ClientCleanName 40 12
export ClientUserinfoChanged
proc ClientUserinfoChanged 6388 56
line 679
;666:
;667:
;668:/*
;669:===========
;670:ClientUserInfoChanged
;671:
;672:Called from ClientConnect when the player first connects and
;673:directly by the server system when the player updates a userinfo variable.
;674:
;675:The game can override any of the settings and call trap_SetUserinfo
;676:if desired.
;677:============
;678:*/
;679:void ClientUserinfoChanged( int clientNum ) {
line 693
;680:	gentity_t *ent;
;681:	int		teamTask, teamLeader, team, health;
;682:	char	*s;
;683:	char	model[MAX_QPATH];
;684:	char	headModel[MAX_QPATH];
;685:	char	oldname[MAX_STRING_CHARS];
;686:	gclient_t	*client;
;687:	char	c1[MAX_INFO_STRING];
;688:	char	c2[MAX_INFO_STRING];
;689:	char	redTeam[MAX_INFO_STRING];
;690:	char	blueTeam[MAX_INFO_STRING];
;691:	char	userinfo[MAX_INFO_STRING];
;692:
;693:	ent = g_entities + clientNum;
ADDRLP4 1032
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 694
;694:	client = ent->client;
ADDRLP4 0
ADDRLP4 1032
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 696
;695:
;696:	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 699
;697:
;698:	// check for malformed or illegal info strings
;699:	if ( !Info_Validate(userinfo) ) {
ADDRLP4 4
ARGP4
ADDRLP4 6300
ADDRGP4 Info_Validate
CALLI4
ASGNI4
ADDRLP4 6300
INDIRI4
CNSTI4 0
NEI4 $246
line 700
;700:		strcpy (userinfo, "\\name\\badinfo");
ADDRLP4 4
ARGP4
ADDRGP4 $248
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 701
;701:	}
LABELV $246
line 704
;702:
;703:	// check for local client
;704:	s = Info_ValueForKey( userinfo, "ip" );
ADDRLP4 4
ARGP4
ADDRGP4 $249
ARGP4
ADDRLP4 6304
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6304
INDIRP4
ASGNP4
line 705
;705:	if ( !strcmp( s, "localhost" ) ) {
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $252
ARGP4
ADDRLP4 6308
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 6308
INDIRI4
CNSTI4 0
NEI4 $250
line 706
;706:		client->pers.localClient = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 496
ADDP4
CNSTI4 1
ASGNI4
line 707
;707:	}
LABELV $250
line 710
;708:
;709:	// check the item prediction
;710:	s = Info_ValueForKey( userinfo, "cg_predictItems" );
ADDRLP4 4
ARGP4
ADDRGP4 $253
ARGP4
ADDRLP4 6312
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6312
INDIRP4
ASGNP4
line 711
;711:	if ( !atoi( s ) ) {
ADDRLP4 1028
INDIRP4
ARGP4
ADDRLP4 6316
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 6316
INDIRI4
CNSTI4 0
NEI4 $254
line 712
;712:		client->pers.predictItemPickup = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 504
ADDP4
CNSTI4 0
ASGNI4
line 713
;713:	} else {
ADDRGP4 $255
JUMPV
LABELV $254
line 714
;714:		client->pers.predictItemPickup = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 504
ADDP4
CNSTI4 1
ASGNI4
line 715
;715:	}
LABELV $255
line 718
;716:
;717:	// set name
;718:	Q_strncpyz ( oldname, client->pers.netname, sizeof( oldname ) );
ADDRLP4 1164
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 719
;719:	s = Info_ValueForKey (userinfo, "name");
ADDRLP4 4
ARGP4
ADDRGP4 $256
ARGP4
ADDRLP4 6320
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6320
INDIRP4
ASGNP4
line 720
;720:	ClientCleanName( s, client->pers.netname, sizeof(client->pers.netname) );
ADDRLP4 1028
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientCleanName
CALLV
pop
line 722
;721:
;722:	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $257
line 723
;723:		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD ) {
ADDRLP4 0
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 3
NEI4 $259
line 724
;724:			Q_strncpyz( client->pers.netname, "scoreboard", sizeof(client->pers.netname) );
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRGP4 $261
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 725
;725:		}
LABELV $259
line 726
;726:	}
LABELV $257
line 728
;727:
;728:	if ( client->pers.connected == CON_CONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
NEI4 $262
line 729
;729:		if ( strcmp( oldname, client->pers.netname ) ) {
ADDRLP4 1164
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 6324
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 6324
INDIRI4
CNSTI4 0
EQI4 $264
line 730
;730:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " renamed to %s\n\"", oldname, 
ADDRGP4 $266
ARGP4
ADDRLP4 1164
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 6328
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 6328
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 732
;731:				client->pers.netname) );
;732:		}
LABELV $264
line 733
;733:	}
LABELV $262
line 747
;734:
;735:	// set max health
;736:#ifdef MISSIONPACK
;737:	if (client->ps.powerups[PW_GUARD]) {
;738:		client->pers.maxHealth = 200;
;739:	} else {
;740:		health = atoi( Info_ValueForKey( userinfo, "handicap" ) );
;741:		client->pers.maxHealth = health;
;742:		if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
;743:			client->pers.maxHealth = 100;
;744:		}
;745:	}
;746:#else
;747:	health = atoi( Info_ValueForKey( userinfo, "handicap" ) );
ADDRLP4 4
ARGP4
ADDRGP4 $267
ARGP4
ADDRLP4 6324
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 6324
INDIRP4
ARGP4
ADDRLP4 6328
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 2196
ADDRLP4 6328
INDIRI4
ASGNI4
line 748
;748:	client->pers.maxHealth = health;
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
ADDRLP4 2196
INDIRI4
ASGNI4
line 749
;749:	if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
ADDRLP4 6332
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ASGNI4
ADDRLP4 6332
INDIRI4
CNSTI4 1
LTI4 $270
ADDRLP4 6332
INDIRI4
CNSTI4 100
LEI4 $268
LABELV $270
line 750
;750:		client->pers.maxHealth = 100;
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
CNSTI4 100
ASGNI4
line 751
;751:	}
LABELV $268
line 753
;752:#endif
;753:	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ASGNI4
line 756
;754:
;755:	// set model
;756:	if( g_gametype.integer >= GT_TEAM ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
LTI4 $271
line 757
;757:		Q_strncpyz( model, Info_ValueForKey (userinfo, "team_model"), sizeof( model ) );
ADDRLP4 4
ARGP4
ADDRGP4 $274
ARGP4
ADDRLP4 6340
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1036
ARGP4
ADDRLP4 6340
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 758
;758:		Q_strncpyz( headModel, Info_ValueForKey (userinfo, "team_headmodel"), sizeof( headModel ) );
ADDRLP4 4
ARGP4
ADDRGP4 $275
ARGP4
ADDRLP4 6344
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1100
ARGP4
ADDRLP4 6344
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 759
;759:	} else {
ADDRGP4 $272
JUMPV
LABELV $271
line 760
;760:		Q_strncpyz( model, Info_ValueForKey (userinfo, "model"), sizeof( model ) );
ADDRLP4 4
ARGP4
ADDRGP4 $276
ARGP4
ADDRLP4 6340
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1036
ARGP4
ADDRLP4 6340
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 761
;761:		Q_strncpyz( headModel, Info_ValueForKey (userinfo, "headmodel"), sizeof( headModel ) );
ADDRLP4 4
ARGP4
ADDRGP4 $277
ARGP4
ADDRLP4 6344
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1100
ARGP4
ADDRLP4 6344
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 762
;762:	}
LABELV $272
line 765
;763:
;764:	// bots set their team a few frames later
;765:	if (g_gametype.integer >= GT_TEAM && g_entities[clientNum].r.svFlags & SVF_BOT) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
LTI4 $278
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities+208+216
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $278
line 766
;766:		s = Info_ValueForKey( userinfo, "team" );
ADDRLP4 4
ARGP4
ADDRGP4 $283
ARGP4
ADDRLP4 6340
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6340
INDIRP4
ASGNP4
line 767
;767:		if ( !Q_stricmp( s, "red" ) || !Q_stricmp( s, "r" ) ) {
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $286
ARGP4
ADDRLP4 6344
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 6344
INDIRI4
CNSTI4 0
EQI4 $288
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $287
ARGP4
ADDRLP4 6348
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 6348
INDIRI4
CNSTI4 0
NEI4 $284
LABELV $288
line 768
;768:			team = TEAM_RED;
ADDRLP4 4248
CNSTI4 1
ASGNI4
line 769
;769:		} else if ( !Q_stricmp( s, "blue" ) || !Q_stricmp( s, "b" ) ) {
ADDRGP4 $279
JUMPV
LABELV $284
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $291
ARGP4
ADDRLP4 6352
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 6352
INDIRI4
CNSTI4 0
EQI4 $293
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $292
ARGP4
ADDRLP4 6356
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 6356
INDIRI4
CNSTI4 0
NEI4 $289
LABELV $293
line 770
;770:			team = TEAM_BLUE;
ADDRLP4 4248
CNSTI4 2
ASGNI4
line 771
;771:		} else {
ADDRGP4 $279
JUMPV
LABELV $289
line 773
;772:			// pick the team with the least number of players
;773:			team = PickTeam( clientNum );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 6360
ADDRGP4 PickTeam
CALLI4
ASGNI4
ADDRLP4 4248
ADDRLP4 6360
INDIRI4
ASGNI4
line 774
;774:		}
line 775
;775:	}
ADDRGP4 $279
JUMPV
LABELV $278
line 776
;776:	else {
line 777
;777:		team = client->sess.sessionTeam;
ADDRLP4 4248
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ASGNI4
line 778
;778:	}
LABELV $279
line 814
;779:
;780:/*	NOTE: all client side now
;781:
;782:	// team
;783:	switch( team ) {
;784:	case TEAM_RED:
;785:		ForceClientSkin(client, model, "red");
;786://		ForceClientSkin(client, headModel, "red");
;787:		break;
;788:	case TEAM_BLUE:
;789:		ForceClientSkin(client, model, "blue");
;790://		ForceClientSkin(client, headModel, "blue");
;791:		break;
;792:	}
;793:	// don't ever use a default skin in teamplay, it would just waste memory
;794:	// however bots will always join a team but they spawn in as spectator
;795:	if ( g_gametype.integer >= GT_TEAM && team == TEAM_SPECTATOR) {
;796:		ForceClientSkin(client, model, "red");
;797://		ForceClientSkin(client, headModel, "red");
;798:	}
;799:*/
;800:
;801:#ifdef MISSIONPACK
;802:	if (g_gametype.integer >= GT_TEAM) {
;803:		client->pers.teamInfo = qtrue;
;804:	} else {
;805:		s = Info_ValueForKey( userinfo, "teamoverlay" );
;806:		if ( ! *s || atoi( s ) != 0 ) {
;807:			client->pers.teamInfo = qtrue;
;808:		} else {
;809:			client->pers.teamInfo = qfalse;
;810:		}
;811:	}
;812:#else
;813:	// teamInfo
;814:	s = Info_ValueForKey( userinfo, "teamoverlay" );
ADDRLP4 4
ARGP4
ADDRGP4 $294
ARGP4
ADDRLP4 6340
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6340
INDIRP4
ASGNP4
line 815
;815:	if ( ! *s || atoi( s ) != 0 ) {
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $297
ADDRLP4 1028
INDIRP4
ARGP4
ADDRLP4 6348
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 6348
INDIRI4
CNSTI4 0
EQI4 $295
LABELV $297
line 816
;816:		client->pers.teamInfo = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 612
ADDP4
CNSTI4 1
ASGNI4
line 817
;817:	} else {
ADDRGP4 $296
JUMPV
LABELV $295
line 818
;818:		client->pers.teamInfo = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 612
ADDP4
CNSTI4 0
ASGNI4
line 819
;819:	}
LABELV $296
line 832
;820:#endif
;821:	/*
;822:	s = Info_ValueForKey( userinfo, "cg_pmove_fixed" );
;823:	if ( !*s || atoi( s ) == 0 ) {
;824:		client->pers.pmoveFixed = qfalse;
;825:	}
;826:	else {
;827:		client->pers.pmoveFixed = qtrue;
;828:	}
;829:	*/
;830:
;831:	// team task (0 = none, 1 = offence, 2 = defence)
;832:	teamTask = atoi(Info_ValueForKey(userinfo, "teamtask"));
ADDRLP4 4
ARGP4
ADDRGP4 $298
ARGP4
ADDRLP4 6352
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 6352
INDIRP4
ARGP4
ADDRLP4 6356
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 2188
ADDRLP4 6356
INDIRI4
ASGNI4
line 834
;833:	// team Leader (1 = leader, 0 is normal player)
;834:	teamLeader = client->sess.teamLeader;
ADDRLP4 2192
ADDRLP4 0
INDIRP4
CNSTI4 640
ADDP4
INDIRI4
ASGNI4
line 837
;835:
;836:	// colors
;837:	strcpy(c1, Info_ValueForKey( userinfo, "color1" ));
ADDRLP4 4
ARGP4
ADDRGP4 $299
ARGP4
ADDRLP4 6360
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 2200
ARGP4
ADDRLP4 6360
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 838
;838:	strcpy(c2, Info_ValueForKey( userinfo, "color2" ));
ADDRLP4 4
ARGP4
ADDRGP4 $300
ARGP4
ADDRLP4 6364
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 3224
ARGP4
ADDRLP4 6364
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 840
;839:
;840:	strcpy(redTeam, Info_ValueForKey( userinfo, "g_redteam" ));
ADDRLP4 4
ARGP4
ADDRGP4 $301
ARGP4
ADDRLP4 6368
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4252
ARGP4
ADDRLP4 6368
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 841
;841:	strcpy(blueTeam, Info_ValueForKey( userinfo, "g_blueteam" ));
ADDRLP4 4
ARGP4
ADDRGP4 $302
ARGP4
ADDRLP4 6372
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 5276
ARGP4
ADDRLP4 6372
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 845
;842:
;843:	// send over a subset of the userinfo keys so other clients can
;844:	// print scoreboards, display models, and play custom sounds
;845:	if ( ent->r.svFlags & SVF_BOT ) {
ADDRLP4 1032
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $303
line 846
;846:		s = va("n\\%s\\t\\%i\\model\\%s\\hmodel\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\skill\\%s\\tt\\%d\\tl\\%d",
ADDRLP4 4
ARGP4
ADDRGP4 $306
ARGP4
ADDRLP4 6376
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 $305
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 4248
INDIRI4
ARGI4
ADDRLP4 1036
ARGP4
ADDRLP4 1100
ARGP4
ADDRLP4 2200
ARGP4
ADDRLP4 3224
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
INDIRI4
ARGI4
ADDRLP4 6376
INDIRP4
ARGP4
ADDRLP4 2188
INDIRI4
ARGI4
ADDRLP4 2192
INDIRI4
ARGI4
ADDRLP4 6384
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6384
INDIRP4
ASGNP4
line 850
;847:			client->pers.netname, team, model, headModel, c1, c2, 
;848:			client->pers.maxHealth, client->sess.wins, client->sess.losses,
;849:			Info_ValueForKey( userinfo, "skill" ), teamTask, teamLeader );
;850:	} else {
ADDRGP4 $304
JUMPV
LABELV $303
line 851
;851:		s = va("n\\%s\\t\\%i\\model\\%s\\hmodel\\%s\\g_redteam\\%s\\g_blueteam\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\tt\\%d\\tl\\%d",
ADDRGP4 $307
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ARGI4
ADDRLP4 1036
ARGP4
ADDRLP4 1100
ARGP4
ADDRLP4 4252
ARGP4
ADDRLP4 5276
ARGP4
ADDRLP4 2200
ARGP4
ADDRLP4 3224
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
INDIRI4
ARGI4
ADDRLP4 2188
INDIRI4
ARGI4
ADDRLP4 2192
INDIRI4
ARGI4
ADDRLP4 6380
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6380
INDIRP4
ASGNP4
line 854
;852:			client->pers.netname, client->sess.sessionTeam, model, headModel, redTeam, blueTeam, c1, c2, 
;853:			client->pers.maxHealth, client->sess.wins, client->sess.losses, teamTask, teamLeader);
;854:	}
LABELV $304
line 856
;855:
;856:	trap_SetConfigstring( CS_PLAYERS+clientNum, s );
ADDRFP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 859
;857:
;858:	// this is not the userinfo, more like the configstring actually
;859:	G_LogPrintf( "ClientUserinfoChanged: %i %s\n", clientNum, s );
ADDRGP4 $308
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 860
;860:}
LABELV $245
endproc ClientUserinfoChanged 6388 56
export ClientConnect
proc ClientConnect 1064 12
line 883
;861:
;862:
;863:/*
;864:===========
;865:ClientConnect
;866:
;867:Called when a player begins connecting to the server.
;868:Called again for every map change or tournement restart.
;869:
;870:The session information will be valid after exit.
;871:
;872:Return NULL if the client should be allowed, otherwise return
;873:a string with the reason for denial.
;874:
;875:Otherwise, the client will be sent the current gamestate
;876:and will eventually get to ClientBegin.
;877:
;878:firstTime will be qtrue the very first time a client connects
;879:to the server machine, but qfalse on map changes and tournement
;880:restarts.
;881:============
;882:*/
;883:char *ClientConnect( int clientNum, qboolean firstTime, qboolean isBot ) {
line 890
;884:	char		*value;
;885://	char		*areabits;
;886:	gclient_t	*client;
;887:	char		userinfo[MAX_INFO_STRING];
;888:	gentity_t	*ent;
;889:
;890:	ent = &g_entities[ clientNum ];
ADDRLP4 4
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 892
;891:
;892:	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 898
;893:
;894: 	// IP filtering
;895: 	// https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=500
;896: 	// recommanding PB based IP / GUID banning, the builtin system is pretty limited
;897: 	// check to see if they are on the banned IP list
;898:	value = Info_ValueForKey (userinfo, "ip");
ADDRLP4 12
ARGP4
ADDRGP4 $249
ARGP4
ADDRLP4 1036
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 1036
INDIRP4
ASGNP4
line 899
;899:	if ( G_FilterPacket( value ) ) {
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1040
ADDRGP4 G_FilterPacket
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
EQI4 $310
line 900
;900:		return "You are banned from this server.";
ADDRGP4 $312
RETP4
ADDRGP4 $309
JUMPV
LABELV $310
line 906
;901:	}
;902:
;903:  // we don't check password for bots and local client
;904:  // NOTE: local client <-> "ip" "localhost"
;905:  //   this means this client is not running in our current process
;906:	if ( !( ent->r.svFlags & SVF_BOT ) && (strcmp(value, "localhost") != 0)) {
ADDRLP4 4
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $313
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $252
ARGP4
ADDRLP4 1044
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
EQI4 $313
line 908
;907:		// check for a password
;908:		value = Info_ValueForKey (userinfo, "password");
ADDRLP4 12
ARGP4
ADDRGP4 $315
ARGP4
ADDRLP4 1048
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 1048
INDIRP4
ASGNP4
line 909
;909:		if ( g_password.string[0] && Q_stricmp( g_password.string, "none" ) &&
ADDRGP4 g_password+16
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $316
ADDRGP4 g_password+16
ARGP4
ADDRGP4 $320
ARGP4
ADDRLP4 1052
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 0
EQI4 $316
ADDRGP4 g_password+16
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1056
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
EQI4 $316
line 910
;910:			strcmp( g_password.string, value) != 0) {
line 911
;911:			return "Invalid password";
ADDRGP4 $322
RETP4
ADDRGP4 $309
JUMPV
LABELV $316
line 913
;912:		}
;913:	}
LABELV $313
line 916
;914:
;915:	// they can connect
;916:	ent->client = level.clients + clientNum;
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
CNSTI4 788
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 917
;917:	client = ent->client;
ADDRLP4 0
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 921
;918:
;919://	areabits = client->areabits;
;920:
;921:	memset( client, 0, sizeof(*client) );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 788
ARGI4
ADDRGP4 memset
CALLP4
pop
line 923
;922:
;923:	client->pers.connected = CON_CONNECTING;
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
CNSTI4 1
ASGNI4
line 926
;924:
;925:	// read or initialize the session data
;926:	if ( firstTime || level.newSession ) {
ADDRLP4 1048
CNSTI4 0
ASGNI4
ADDRFP4 4
INDIRI4
ADDRLP4 1048
INDIRI4
NEI4 $326
ADDRGP4 level+68
INDIRI4
ADDRLP4 1048
INDIRI4
EQI4 $323
LABELV $326
line 927
;927:		G_InitSessionData( client, userinfo );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRGP4 G_InitSessionData
CALLV
pop
line 928
;928:	}
LABELV $323
line 929
;929:	G_ReadSessionData( client );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_ReadSessionData
CALLV
pop
line 931
;930:
;931:	if( isBot ) {
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $327
line 932
;932:		ent->r.svFlags |= SVF_BOT;
ADDRLP4 1052
ADDRLP4 4
INDIRP4
CNSTI4 424
ADDP4
ASGNP4
ADDRLP4 1052
INDIRP4
ADDRLP4 1052
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 933
;933:		ent->inuse = qtrue;
ADDRLP4 4
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 1
ASGNI4
line 934
;934:		if( !G_BotConnect( clientNum, !firstTime ) ) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $332
ADDRLP4 1056
CNSTI4 1
ASGNI4
ADDRGP4 $333
JUMPV
LABELV $332
ADDRLP4 1056
CNSTI4 0
ASGNI4
LABELV $333
ADDRLP4 1056
INDIRI4
ARGI4
ADDRLP4 1060
ADDRGP4 G_BotConnect
CALLI4
ASGNI4
ADDRLP4 1060
INDIRI4
CNSTI4 0
NEI4 $329
line 935
;935:			return "BotConnectfailed";
ADDRGP4 $334
RETP4
ADDRGP4 $309
JUMPV
LABELV $329
line 937
;936:		}
;937:	}
LABELV $327
line 940
;938:
;939:	// get and distribute relevent paramters
;940:	G_LogPrintf( "ClientConnect: %i\n", clientNum );
ADDRGP4 $335
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_LogPrintf
CALLV
pop
line 941
;941:	ClientUserinfoChanged( clientNum );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 ClientUserinfoChanged
CALLV
pop
line 944
;942:
;943:	// don't do the "xxx connected" messages if they were caried over from previous level
;944:	if ( firstTime ) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $336
line 945
;945:		trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " connected\n\"", client->pers.netname) );
ADDRGP4 $338
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 1052
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 1052
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 946
;946:	}
LABELV $336
line 948
;947:
;948:	if ( g_gametype.integer >= GT_TEAM &&
ADDRLP4 1052
CNSTI4 3
ASGNI4
ADDRGP4 g_gametype+12
INDIRI4
ADDRLP4 1052
INDIRI4
LTI4 $339
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ADDRLP4 1052
INDIRI4
EQI4 $339
line 949
;949:		client->sess.sessionTeam != TEAM_SPECTATOR ) {
line 950
;950:		BroadcastTeamChange( client, -1 );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 BroadcastTeamChange
CALLV
pop
line 951
;951:	}
LABELV $339
line 954
;952:
;953:	// count current clients and rank for scoreboard
;954:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 961
;955:
;956:	// for statistics
;957://	client->areabits = areabits;
;958://	if ( !client->areabits )
;959://		client->areabits = G_Alloc( (trap_AAS_PointReachabilityAreaIndex( NULL ) + 7) / 8 );
;960:
;961:	return NULL;
CNSTP4 0
RETP4
LABELV $309
endproc ClientConnect 1064 12
export ClientBegin
proc ClientBegin 32 12
line 973
;962:}
;963:
;964:/*
;965:===========
;966:ClientBegin
;967:
;968:called when a client has finished connecting, and is ready
;969:to be placed into the level.  This will happen every level load,
;970:and on transition between teams, but doesn't happen on respawns
;971:============
;972:*/
;973:void ClientBegin( int clientNum ) {
line 980
;974:	gentity_t	*ent;
;975:	gclient_t	*client;
;976:	gentity_t	*tent;
;977:	int			flags;
;978:	int			time;			//Declares time integer
;979:
;980:	ent = g_entities + clientNum;
ADDRLP4 4
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 982
;981:
;982:	client = level.clients + clientNum;
ADDRLP4 0
CNSTI4 788
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 984
;983:
;984:	if ( ent->r.linked ) {
ADDRLP4 4
INDIRP4
CNSTI4 416
ADDP4
INDIRI4
CNSTI4 0
EQI4 $343
line 985
;985:		trap_UnlinkEntity( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 986
;986:	}
LABELV $343
line 987
;987:	G_InitGentity( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 G_InitGentity
CALLV
pop
line 988
;988:	ent->touch = 0;
ADDRLP4 4
INDIRP4
CNSTI4 704
ADDP4
CNSTP4 0
ASGNP4
line 989
;989:	ent->pain = 0;
ADDRLP4 4
INDIRP4
CNSTI4 712
ADDP4
CNSTP4 0
ASGNP4
line 990
;990:	ent->client = client;
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 992
;991:
;992:	client->pers.connected = CON_CONNECTED;
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
CNSTI4 2
ASGNI4
line 993
;993:	client->pers.enterTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 552
ADDP4
ADDRGP4 level+36
INDIRI4
ASGNI4
line 994
;994:	client->pers.teamState.state = TEAM_BEGIN;
ADDRLP4 0
INDIRP4
CNSTI4 556
ADDP4
CNSTI4 0
ASGNI4
line 1001
;995:
;996:	// save eflags around this, because changing teams will
;997:	// cause this to happen with a valid entity, and we
;998:	// want to make sure the teleport bit is set right
;999:	// so the viewpoint doesn't interpolate through the
;1000:	// world to the new position
;1001:	flags = client->ps.eFlags;
ADDRLP4 12
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
INDIRI4
ASGNI4
line 1002
;1002:	memset( &client->ps, 0, sizeof( client->ps ) );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 468
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1003
;1003:	client->ps.eFlags = flags;
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 1006
;1004:
;1005:	// locate ent at a spawn point
;1006:	ClientSpawn( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 ClientSpawn
CALLV
pop
line 1008
;1007:
;1008:	if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
EQI4 $346
line 1010
;1009:		// send event
;1010:		tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN );
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 42
ARGI4
ADDRLP4 20
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 16
ADDRLP4 20
INDIRP4
ASGNP4
line 1011
;1011:		tent->s.clientNum = ent->s.clientNum;
ADDRLP4 24
CNSTI4 168
ASGNI4
ADDRLP4 16
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
ADDRLP4 4
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 1013
;1012:
;1013:		if ( g_gametype.integer != GT_TOURNAMENT  ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 1
EQI4 $348
line 1014
;1014:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " entered the game\n\"", client->pers.netname) );
ADDRGP4 $351
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 28
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 28
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1015
;1015:		}
LABELV $348
line 1016
;1016:	}
LABELV $346
line 1017
;1017:	G_LogPrintf( "ClientBegin: %i\n", clientNum );
ADDRGP4 $352
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_LogPrintf
CALLV
pop
line 1020
;1018:
;1019:	// count current clients and rank for scoreboard
;1020:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 1023
;1021:
;1022:	// ^^^
;1023:		time = level.time - level.startTime;
ADDRLP4 8
ADDRGP4 level+36
INDIRI4
ADDRGP4 level+44
INDIRI4
SUBI4
ASGNI4
line 1024
;1024:		client->jailedHits = 0;
ADDRLP4 0
INDIRP4
CNSTI4 740
ADDP4
CNSTI4 0
ASGNI4
line 1026
;1025:
;1026:		if ( time <= 0 ) {
ADDRLP4 8
INDIRI4
CNSTI4 0
GTI4 $355
line 1027
;1027:			time = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 1028
;1028:		}
LABELV $355
line 1029
;1029:		if ( time >= 60000 ) {											//See if they entered after a minute
ADDRLP4 8
INDIRI4
CNSTI4 60000
LTI4 $357
line 1030
;1030:			client->ps.persistant[PERS_SCORE] = 6 - time / 60000;		//If so, Score = 6 - the total level time in minutes
ADDRLP4 0
INDIRP4
CNSTI4 248
ADDP4
CNSTI4 6
ADDRLP4 8
INDIRI4
CNSTI4 60000
DIVI4
SUBI4
ASGNI4
line 1031
;1031:			if (client->ps.persistant[PERS_SCORE] <= 0) {					//Make sure noone spawns with negative lives
ADDRLP4 0
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
CNSTI4 0
GTI4 $358
line 1032
;1032:				client->ps.persistant[PERS_SCORE] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 248
ADDP4
CNSTI4 0
ASGNI4
line 1033
;1033:			}
line 1034
;1034:		} else {
ADDRGP4 $358
JUMPV
LABELV $357
line 1035
;1035:		client->ps.persistant[PERS_SCORE] = 10;		// ^^^ Players recieve a score of 10 when connected before 1 minute.
ADDRLP4 0
INDIRP4
CNSTI4 248
ADDP4
CNSTI4 10
ASGNI4
line 1036
;1036:		}
LABELV $358
line 1039
;1037:
;1038://	trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " connected\n\"", client->pers.netname) );
;1039:	trap_SendServerCommand( client->ps.clientNum, va("print \"^3Objective is to knock others off the map,\n\"", client->pers.netname) );
ADDRGP4 $361
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 20
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 20
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1040
;1040:	trap_SendServerCommand( client->ps.clientNum, va("print \"^3while staying on the map. 10 lives are given.\n\"", client->pers.netname) );
ADDRGP4 $362
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 24
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1041
;1041:	trap_SendServerCommand( client->ps.clientNum, va("cp \"http://www.planetquake.com/toxinate\n\"", client->pers.netname));
ADDRGP4 $363
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 28
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 28
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1042
;1042:}
LABELV $342
endproc ClientBegin 32 12
export ClientSpawn
proc ClientSpawn 1376 16
line 1053
;1043:
;1044:/*
;1045:===========
;1046:ClientSpawn
;1047:
;1048:Called every time a client is placed fresh in the world:
;1049:after the first ClientBegin, and after each respawn
;1050:Initializes all non-persistant parts of playerState
;1051:============
;1052:*/
;1053:void ClientSpawn(gentity_t *ent) {
line 1069
;1054:	int		index;
;1055:	vec3_t	spawn_origin, spawn_angles;
;1056:	gclient_t	*client;
;1057:	int		i;
;1058:	clientPersistant_t	saved;
;1059:	clientSession_t		savedSess;
;1060:	int		persistant[MAX_PERSISTANT];
;1061:	gentity_t	*spawnPoint;
;1062:	int		flags;
;1063:	int		savedPing;
;1064://	char	*savedAreaBits;
;1065:	int		accuracy_hits, accuracy_shots;
;1066:	int		eventSequence;
;1067:	char	userinfo[MAX_INFO_STRING];
;1068:
;1069:	index = ent - g_entities;
ADDRLP4 100
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 808
DIVI4
ASGNI4
line 1070
;1070:	client = ent->client;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 1075
;1071:
;1072:	// find a spawn point
;1073:	// do it before setting health back up, so farthest
;1074:	// ranging doesn't count this client
;1075:	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRLP4 4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $365
line 1076
;1076:		spawnPoint = SelectSpectatorSpawnPoint ( 
ADDRLP4 76
ARGP4
ADDRLP4 88
ARGP4
ADDRLP4 1324
ADDRGP4 SelectSpectatorSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 1324
INDIRP4
ASGNP4
line 1078
;1077:						spawn_origin, spawn_angles);
;1078:	} else if (g_gametype.integer >= GT_CTF ) {
ADDRGP4 $366
JUMPV
LABELV $365
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 4
LTI4 $367
line 1080
;1079:		// all base oriented team games use the CTF spawn points
;1080:		spawnPoint = SelectCTFSpawnPoint ( 
ADDRLP4 4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 556
ADDP4
INDIRI4
ARGI4
ADDRLP4 76
ARGP4
ADDRLP4 88
ARGP4
ADDRLP4 1328
ADDRGP4 SelectCTFSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 1328
INDIRP4
ASGNP4
line 1084
;1081:						client->sess.sessionTeam, 
;1082:						client->pers.teamState.state, 
;1083:						spawn_origin, spawn_angles);
;1084:	} else {
ADDRGP4 $368
JUMPV
LABELV $367
LABELV $370
line 1085
;1085:		do {
line 1087
;1086:			// the first spawn should be at a good looking spot
;1087:			if ( !client->pers.initialSpawn && client->pers.localClient ) {
ADDRLP4 1328
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 500
ADDP4
INDIRI4
ADDRLP4 1328
INDIRI4
NEI4 $373
ADDRLP4 4
INDIRP4
CNSTI4 496
ADDP4
INDIRI4
ADDRLP4 1328
INDIRI4
EQI4 $373
line 1088
;1088:				client->pers.initialSpawn = qtrue;
ADDRLP4 4
INDIRP4
CNSTI4 500
ADDP4
CNSTI4 1
ASGNI4
line 1089
;1089:				spawnPoint = SelectInitialSpawnPoint( spawn_origin, spawn_angles );
ADDRLP4 76
ARGP4
ADDRLP4 88
ARGP4
ADDRLP4 1332
ADDRGP4 SelectInitialSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 1332
INDIRP4
ASGNP4
line 1090
;1090:			} else {
ADDRGP4 $374
JUMPV
LABELV $373
line 1092
;1091:				// don't spawn near existing origin if possible
;1092:				spawnPoint = SelectSpawnPoint ( 
ADDRLP4 4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 76
ARGP4
ADDRLP4 88
ARGP4
ADDRLP4 1332
ADDRGP4 SelectSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 1332
INDIRP4
ASGNP4
line 1095
;1093:					client->ps.origin, 
;1094:					spawn_origin, spawn_angles);
;1095:			}
LABELV $374
line 1099
;1096:
;1097:			// Tim needs to prevent bots from spawning at the initial point
;1098:			// on q3dm0...
;1099:			if ( ( spawnPoint->flags & FL_NO_BOTS ) && ( ent->r.svFlags & SVF_BOT ) ) {
ADDRLP4 1332
CNSTI4 0
ASGNI4
ADDRLP4 72
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 8192
BANDI4
ADDRLP4 1332
INDIRI4
EQI4 $375
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 1332
INDIRI4
EQI4 $375
line 1100
;1100:				continue;	// try again
ADDRGP4 $371
JUMPV
LABELV $375
line 1103
;1101:			}
;1102:			// just to be symetric, we have a nohumans option...
;1103:			if ( ( spawnPoint->flags & FL_NO_HUMANS ) && !( ent->r.svFlags & SVF_BOT ) ) {
ADDRLP4 1336
CNSTI4 0
ASGNI4
ADDRLP4 72
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 16384
BANDI4
ADDRLP4 1336
INDIRI4
EQI4 $372
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 1336
INDIRI4
NEI4 $372
line 1104
;1104:				continue;	// try again
line 1107
;1105:			}
;1106:
;1107:			break;
LABELV $371
line 1109
;1108:
;1109:		} while ( 1 );
ADDRGP4 $370
JUMPV
LABELV $372
line 1110
;1110:	}
LABELV $368
LABELV $366
line 1111
;1111:	client->pers.teamState.state = TEAM_ACTIVE;
ADDRLP4 4
INDIRP4
CNSTI4 556
ADDP4
CNSTI4 1
ASGNI4
line 1114
;1112:
;1113:	// always clear the kamikaze flag
;1114:	ent->s.eFlags &= ~EF_KAMIKAZE;
ADDRLP4 1324
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 1324
INDIRP4
ADDRLP4 1324
INDIRP4
INDIRI4
CNSTI4 -513
BANDI4
ASGNI4
line 1118
;1115:
;1116:	// toggle the teleport bit so the client knows to not lerp
;1117:	// and never clear the voted flag
;1118:	flags = ent->client->ps.eFlags & (EF_TELEPORT_BIT | EF_VOTED | EF_TEAMVOTED);
ADDRLP4 104
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
INDIRI4
CNSTI4 540676
BANDI4
ASGNI4
line 1119
;1119:	flags ^= EF_TELEPORT_BIT;
ADDRLP4 104
ADDRLP4 104
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 1123
;1120:
;1121:	// clear everything but the persistant data
;1122:
;1123:	saved = client->pers;
ADDRLP4 1132
ADDRLP4 4
INDIRP4
CNSTI4 468
ADDP4
INDIRB
ASGNB 148
line 1124
;1124:	savedSess = client->sess;
ADDRLP4 1280
ADDRLP4 4
INDIRP4
CNSTI4 616
ADDP4
INDIRB
ASGNB 28
line 1125
;1125:	savedPing = client->ps.ping;
ADDRLP4 1308
ADDRLP4 4
INDIRP4
CNSTI4 452
ADDP4
INDIRI4
ASGNI4
line 1127
;1126://	savedAreaBits = client->areabits;
;1127:	accuracy_hits = client->accuracy_hits;
ADDRLP4 1312
ADDRLP4 4
INDIRP4
CNSTI4 716
ADDP4
INDIRI4
ASGNI4
line 1128
;1128:	accuracy_shots = client->accuracy_shots;
ADDRLP4 1316
ADDRLP4 4
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 1129
;1129:	for ( i = 0 ; i < MAX_PERSISTANT ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $379
line 1130
;1130:		persistant[i] = client->ps.persistant[i];
ADDRLP4 1328
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 1328
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 1328
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 248
ADDP4
ADDP4
INDIRI4
ASGNI4
line 1131
;1131:	}
LABELV $380
line 1129
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $379
line 1132
;1132:	eventSequence = client->ps.eventSequence;
ADDRLP4 1320
ADDRLP4 4
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ASGNI4
line 1134
;1133:
;1134:	memset (client, 0, sizeof(*client)); // bk FIXME: Com_Memset?
ADDRLP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 788
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1136
;1135:
;1136:	client->pers = saved;
ADDRLP4 4
INDIRP4
CNSTI4 468
ADDP4
ADDRLP4 1132
INDIRB
ASGNB 148
line 1137
;1137:	client->sess = savedSess;
ADDRLP4 4
INDIRP4
CNSTI4 616
ADDP4
ADDRLP4 1280
INDIRB
ASGNB 28
line 1138
;1138:	client->ps.ping = savedPing;
ADDRLP4 4
INDIRP4
CNSTI4 452
ADDP4
ADDRLP4 1308
INDIRI4
ASGNI4
line 1140
;1139://	client->areabits = savedAreaBits;
;1140:	client->accuracy_hits = accuracy_hits;
ADDRLP4 4
INDIRP4
CNSTI4 716
ADDP4
ADDRLP4 1312
INDIRI4
ASGNI4
line 1141
;1141:	client->accuracy_shots = accuracy_shots;
ADDRLP4 4
INDIRP4
CNSTI4 712
ADDP4
ADDRLP4 1316
INDIRI4
ASGNI4
line 1142
;1142:	client->lastkilled_client = -1;
ADDRLP4 4
INDIRP4
CNSTI4 720
ADDP4
CNSTI4 -1
ASGNI4
line 1144
;1143:
;1144:	for ( i = 0 ; i < MAX_PERSISTANT ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $383
line 1145
;1145:		client->ps.persistant[i] = persistant[i];
ADDRLP4 1328
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 1328
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 248
ADDP4
ADDP4
ADDRLP4 1328
INDIRI4
ADDRLP4 8
ADDP4
INDIRI4
ASGNI4
line 1146
;1146:	}
LABELV $384
line 1144
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $383
line 1147
;1147:	client->ps.eventSequence = eventSequence;
ADDRLP4 4
INDIRP4
CNSTI4 108
ADDP4
ADDRLP4 1320
INDIRI4
ASGNI4
line 1149
;1148:	// increment the spawncount so the client will detect the respawn
;1149:	client->ps.persistant[PERS_SPAWN_COUNT]++;
ADDRLP4 1328
ADDRLP4 4
INDIRP4
CNSTI4 264
ADDP4
ASGNP4
ADDRLP4 1328
INDIRP4
ADDRLP4 1328
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1150
;1150:	client->ps.persistant[PERS_TEAM] = client->sess.sessionTeam;
ADDRLP4 4
INDIRP4
CNSTI4 260
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ASGNI4
line 1152
;1151:
;1152:	client->airOutTime = level.time + 12000;
ADDRLP4 4
INDIRP4
CNSTI4 760
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 12000
ADDI4
ASGNI4
line 1154
;1153:
;1154:	trap_GetUserinfo( index, userinfo, sizeof(userinfo) );
ADDRLP4 100
INDIRI4
ARGI4
ADDRLP4 108
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 1156
;1155:	// set max health
;1156:	client->pers.maxHealth = atoi( Info_ValueForKey( userinfo, "handicap" ) );
ADDRLP4 108
ARGP4
ADDRGP4 $267
ARGP4
ADDRLP4 1336
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1336
INDIRP4
ARGP4
ADDRLP4 1340
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 548
ADDP4
ADDRLP4 1340
INDIRI4
ASGNI4
line 1157
;1157:	if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
ADDRLP4 1344
ADDRLP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1344
INDIRI4
CNSTI4 1
LTI4 $390
ADDRLP4 1344
INDIRI4
CNSTI4 100
LEI4 $388
LABELV $390
line 1158
;1158:		client->pers.maxHealth = 100;
ADDRLP4 4
INDIRP4
CNSTI4 548
ADDP4
CNSTI4 100
ASGNI4
line 1159
;1159:	}
LABELV $388
line 1161
;1160:	// clear entity values
;1161:	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;
ADDRLP4 4
INDIRP4
CNSTI4 208
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ASGNI4
line 1162
;1162:	client->ps.eFlags = flags;
ADDRLP4 4
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 104
INDIRI4
ASGNI4
line 1164
;1163:
;1164:	ent->s.groundEntityNum = ENTITYNUM_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 1023
ASGNI4
line 1165
;1165:	ent->client = &level.clients[index];
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
CNSTI4 788
ADDRLP4 100
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 1166
;1166:	ent->takedamage = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
CNSTI4 1
ASGNI4
line 1167
;1167:	ent->inuse = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 1
ASGNI4
line 1168
;1168:	ent->classname = "player";
ADDRFP4 0
INDIRP4
CNSTI4 524
ADDP4
ADDRGP4 $391
ASGNP4
line 1169
;1169:	ent->r.contents = CONTENTS_BODY;
ADDRFP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 33554432
ASGNI4
line 1170
;1170:	ent->clipmask = MASK_PLAYERSOLID;
ADDRFP4 0
INDIRP4
CNSTI4 572
ADDP4
CNSTI4 33619969
ASGNI4
line 1171
;1171:	ent->die = player_die;
ADDRFP4 0
INDIRP4
CNSTI4 716
ADDP4
ADDRGP4 player_die
ASGNP4
line 1172
;1172:	ent->waterlevel = 0;
ADDRFP4 0
INDIRP4
CNSTI4 788
ADDP4
CNSTI4 0
ASGNI4
line 1173
;1173:	ent->watertype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
CNSTI4 0
ASGNI4
line 1174
;1174:	ent->flags = 0;
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
CNSTI4 0
ASGNI4
line 1176
;1175:	
;1176:	VectorCopy (playerMins, ent->r.mins);
ADDRFP4 0
INDIRP4
CNSTI4 436
ADDP4
ADDRGP4 playerMins
INDIRB
ASGNB 12
line 1177
;1177:	VectorCopy (playerMaxs, ent->r.maxs);
ADDRFP4 0
INDIRP4
CNSTI4 448
ADDP4
ADDRGP4 playerMaxs
INDIRB
ASGNB 12
line 1179
;1178:
;1179:	client->ps.clientNum = index;
ADDRLP4 4
INDIRP4
CNSTI4 140
ADDP4
ADDRLP4 100
INDIRI4
ASGNI4
line 1181
;1180:
;1181:	client->ps.stats[STAT_WEAPONS] = ( 1 << WP_MACHINEGUN );
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
CNSTI4 4
ASGNI4
line 1182
;1182:	if ( g_gametype.integer == GT_TEAM ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
NEI4 $392
line 1183
;1183:		client->ps.ammo[WP_MACHINEGUN] = 50;
ADDRLP4 4
INDIRP4
CNSTI4 384
ADDP4
CNSTI4 50
ASGNI4
line 1184
;1184:	} else {
ADDRGP4 $393
JUMPV
LABELV $392
line 1185
;1185:		client->ps.ammo[WP_MACHINEGUN] = 100;
ADDRLP4 4
INDIRP4
CNSTI4 384
ADDP4
CNSTI4 100
ASGNI4
line 1186
;1186:	}
LABELV $393
line 1188
;1187:
;1188:	client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_GAUNTLET );
ADDRLP4 1352
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
ASGNP4
ADDRLP4 1352
INDIRP4
ADDRLP4 1352
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 1189
;1189:	client->ps.ammo[WP_GAUNTLET] = -1;
ADDRLP4 4
INDIRP4
CNSTI4 380
ADDP4
CNSTI4 -1
ASGNI4
line 1190
;1190:	client->ps.ammo[WP_GRAPPLING_HOOK] = -1;
ADDRLP4 4
INDIRP4
CNSTI4 416
ADDP4
CNSTI4 -1
ASGNI4
line 1192
;1191:
;1192:	if (client->ps.persistant[PERS_SCORE] == 0) {					//If you're on your last life, give all weps!	^^^
ADDRLP4 4
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
CNSTI4 0
NEI4 $395
line 1193
;1193:		client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_SHOTGUN );
ADDRLP4 1356
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
ASGNP4
ADDRLP4 1356
INDIRP4
ADDRLP4 1356
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 1194
;1194:		client->ps.ammo[WP_SHOTGUN] = 50;
ADDRLP4 4
INDIRP4
CNSTI4 388
ADDP4
CNSTI4 50
ASGNI4
line 1195
;1195:		client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_RAILGUN );
ADDRLP4 1360
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
ASGNP4
ADDRLP4 1360
INDIRP4
ADDRLP4 1360
INDIRP4
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 1196
;1196:		client->ps.ammo[WP_RAILGUN] = 50;
ADDRLP4 4
INDIRP4
CNSTI4 404
ADDP4
CNSTI4 50
ASGNI4
line 1197
;1197:		client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_ROCKET_LAUNCHER );
ADDRLP4 1364
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
ASGNP4
ADDRLP4 1364
INDIRP4
ADDRLP4 1364
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 1198
;1198:		client->ps.ammo[WP_ROCKET_LAUNCHER] = 50;
ADDRLP4 4
INDIRP4
CNSTI4 396
ADDP4
CNSTI4 50
ASGNI4
line 1199
;1199:	}
LABELV $395
line 1202
;1200:
;1201:	// health will count down towards max_health
;1202:	ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH] + 25;
ADDRLP4 1360
ADDRLP4 4
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 25
ADDI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 184
ADDP4
ADDRLP4 1360
INDIRI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRLP4 1360
INDIRI4
ASGNI4
line 1204
;1203:
;1204:	G_SetOrigin( ent, spawn_origin );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 76
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1205
;1205:	VectorCopy( spawn_origin, client->ps.origin );
ADDRLP4 4
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 76
INDIRB
ASGNB 12
line 1208
;1206:
;1207:	// the respawned flag will be cleared after the attack and jump keys come up
;1208:	client->ps.pm_flags |= PMF_RESPAWNED;
ADDRLP4 1364
ADDRLP4 4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 1364
INDIRP4
ADDRLP4 1364
INDIRP4
INDIRI4
CNSTI4 512
BORI4
ASGNI4
line 1210
;1209:
;1210:	trap_GetUsercmd( client - level.clients, &ent->client->pers.cmd );
ADDRLP4 4
INDIRP4
CVPU4 4
ADDRGP4 level
INDIRP4
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 788
DIVI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 472
ADDP4
ARGP4
ADDRGP4 trap_GetUsercmd
CALLV
pop
line 1211
;1211:	SetClientViewAngle( ent, spawn_angles );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 88
ARGP4
ADDRGP4 SetClientViewAngle
CALLV
pop
line 1213
;1212:
;1213:	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $397
line 1215
;1214:
;1215:	} else {
ADDRGP4 $398
JUMPV
LABELV $397
line 1216
;1216:		G_KillBox( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_KillBox
CALLV
pop
line 1217
;1217:		trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1220
;1218:
;1219:		// force the base weapon up
;1220:		client->ps.weapon = WP_MACHINEGUN;
ADDRLP4 4
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 2
ASGNI4
line 1221
;1221:		client->ps.weaponstate = WEAPON_READY;
ADDRLP4 4
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 0
ASGNI4
line 1223
;1222:
;1223:	}
LABELV $398
line 1226
;1224:
;1225:	// don't allow full run speed for a bit
;1226:	client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
ADDRLP4 1368
ADDRLP4 4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 1368
INDIRP4
ADDRLP4 1368
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 1227
;1227:	client->ps.pm_time = 100;
ADDRLP4 4
INDIRP4
CNSTI4 16
ADDP4
CNSTI4 100
ASGNI4
line 1229
;1228:
;1229:	client->respawnTime = level.time;
ADDRLP4 4
INDIRP4
CNSTI4 744
ADDP4
ADDRGP4 level+36
INDIRI4
ASGNI4
line 1230
;1230:	client->inactivityTime = level.time + g_inactivity.integer * 1000;
ADDRLP4 4
INDIRP4
CNSTI4 748
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 1000
ADDRGP4 g_inactivity+12
INDIRI4
MULI4
ADDI4
ASGNI4
line 1231
;1231:	client->latched_buttons = 0;
ADDRLP4 4
INDIRP4
CNSTI4 664
ADDP4
CNSTI4 0
ASGNI4
line 1234
;1232:
;1233:	// set default animations
;1234:	client->ps.torsoAnim = TORSO_STAND;
ADDRLP4 4
INDIRP4
CNSTI4 84
ADDP4
CNSTI4 11
ASGNI4
line 1235
;1235:	client->ps.legsAnim = LEGS_IDLE;
ADDRLP4 4
INDIRP4
CNSTI4 76
ADDP4
CNSTI4 22
ASGNI4
line 1237
;1236:
;1237:	if ( level.intermissiontime ) {
ADDRGP4 level+9132
INDIRI4
CNSTI4 0
EQI4 $402
line 1238
;1238:		MoveClientToIntermission( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 MoveClientToIntermission
CALLV
pop
line 1239
;1239:	} else {
ADDRGP4 $403
JUMPV
LABELV $402
line 1241
;1240:		// fire the targets of the spawn point
;1241:		G_UseTargets( spawnPoint, ent );
ADDRLP4 72
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 1245
;1242:
;1243:		// select the highest weapon number available, after any
;1244:		// spawn given items have fired
;1245:		client->ps.weapon = 1;
ADDRLP4 4
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 1
ASGNI4
line 1246
;1246:		for ( i = WP_NUM_WEAPONS - 1 ; i > 0 ; i-- ) {
ADDRLP4 0
CNSTI4 10
ASGNI4
LABELV $405
line 1247
;1247:			if ( client->ps.stats[STAT_WEAPONS] & ( 1 << i ) ) {
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 0
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $409
line 1248
;1248:				client->ps.weapon = i;
ADDRLP4 4
INDIRP4
CNSTI4 144
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1249
;1249:				break;
ADDRGP4 $407
JUMPV
LABELV $409
line 1251
;1250:			}
;1251:		}
LABELV $406
line 1246
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
GTI4 $405
LABELV $407
line 1252
;1252:	}
LABELV $403
line 1256
;1253:
;1254:	// run a client frame to drop exactly to the floor,
;1255:	// initialize animations and other things
;1256:	client->ps.commandTime = level.time - 100;
ADDRLP4 4
INDIRP4
ADDRGP4 level+36
INDIRI4
CNSTI4 100
SUBI4
ASGNI4
line 1257
;1257:	ent->client->pers.cmd.serverTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 472
ADDP4
ADDRGP4 level+36
INDIRI4
ASGNI4
line 1258
;1258:	ClientThink( ent-g_entities );
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 808
DIVI4
ARGI4
ADDRGP4 ClientThink
CALLV
pop
line 1261
;1259:
;1260:	// positively link the client, even if the command times are weird
;1261:	if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
EQI4 $413
line 1262
;1262:		BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 1263
;1263:		VectorCopy( ent->client->ps.origin, ent->r.currentOrigin );
ADDRLP4 1372
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1372
INDIRP4
CNSTI4 488
ADDP4
ADDRLP4 1372
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 1264
;1264:		trap_LinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1265
;1265:	}
LABELV $413
line 1268
;1266:
;1267:	// run the presend to set anything else
;1268:	ClientEndFrame( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ClientEndFrame
CALLV
pop
line 1271
;1269:
;1270:	// clear entity state values
;1271:	BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 1272
;1272:}
LABELV $364
endproc ClientSpawn 1376 16
export ClientDisconnect
proc ClientDisconnect 24 8
line 1287
;1273:
;1274:
;1275:/*
;1276:===========
;1277:ClientDisconnect
;1278:
;1279:Called when a player drops from the server.
;1280:Will not be called between levels.
;1281:
;1282:This should NOT be called directly by any game logic,
;1283:call trap_DropClient(), which will call this and do
;1284:server system housekeeping.
;1285:============
;1286:*/
;1287:void ClientDisconnect( int clientNum ) {
line 1290
;1288:	gentity_t	*ent;
;1289:	gentity_t	*tent;
;1290:	int			i = clientNum;
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
line 1294
;1291:
;1292:	// cleanup if we are kicking a bot that
;1293:	// hasn't spawned yet
;1294:	G_RemoveQueuedBotBegin( clientNum );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_RemoveQueuedBotBegin
CALLV
pop
line 1296
;1295:
;1296:	ent = g_entities + clientNum;
ADDRLP4 4
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1298
;1297:
;1298:	if ( !ent->client ) {
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $416
line 1299
;1299:		return;
ADDRGP4 $415
JUMPV
LABELV $416
line 1302
;1300:	}
;1301:
;1302:	if (ent->client->ps.persistant[PERS_SCORE] == -1) {
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $418
line 1303
;1303:		level.numJailed = level.numJailed - 1;
ADDRGP4 level+28
ADDRGP4 level+28
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1304
;1304:	}
LABELV $418
line 1307
;1305:
;1306:	// stop any following clients
;1307:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $425
JUMPV
LABELV $422
line 1308
;1308:		if ( level.clients[i].sess.sessionTeam == TEAM_SPECTATOR
ADDRLP4 12
CNSTI4 788
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $427
ADDRLP4 12
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 2
NEI4 $427
ADDRLP4 12
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $427
line 1310
;1309:			&& level.clients[i].sess.spectatorState == SPECTATOR_FOLLOW
;1310:			&& level.clients[i].sess.spectatorClient == clientNum ) {
line 1311
;1311:			StopFollowing( &g_entities[i] );
CNSTI4 808
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRGP4 StopFollowing
CALLV
pop
line 1312
;1312:		}
LABELV $427
line 1313
;1313:	}
LABELV $423
line 1307
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $425
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $422
line 1316
;1314:
;1315:	// send effect if they were completely connected
;1316:	if ( ent->client->pers.connected == CON_CONNECTED 
ADDRLP4 12
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
NEI4 $429
ADDRLP4 12
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
EQI4 $429
line 1317
;1317:		&& ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
line 1318
;1318:		tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_OUT );
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 43
ARGI4
ADDRLP4 16
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 16
INDIRP4
ASGNP4
line 1319
;1319:		tent->s.clientNum = ent->s.clientNum;
ADDRLP4 20
CNSTI4 168
ASGNI4
ADDRLP4 8
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
ADDRLP4 4
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 1323
;1320:
;1321:		// They don't get to take powerups with them!
;1322:		// Especially important for stuff like CTF flags
;1323:		TossClientItems( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 TossClientItems
CALLV
pop
line 1331
;1324:#ifdef MISSIONPACK
;1325:		TossClientPersistantPowerups( ent );
;1326:		if( g_gametype.integer == GT_HARVESTER ) {
;1327:			TossClientCubes( ent );
;1328:		}
;1329:#endif
;1330:
;1331:	}
LABELV $429
line 1333
;1332:
;1333:	G_LogPrintf( "ClientDisconnect: %i\n", clientNum );
ADDRGP4 $431
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_LogPrintf
CALLV
pop
line 1336
;1334:
;1335:	// if we are playing in tourney mode and losing, give a win to the other player
;1336:	if ( (g_gametype.integer == GT_TOURNAMENT )
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 1
NEI4 $432
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRGP4 level+9132
INDIRI4
ADDRLP4 16
INDIRI4
NEI4 $432
ADDRGP4 level+16
INDIRI4
ADDRLP4 16
INDIRI4
NEI4 $432
ADDRGP4 level+88+4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $432
line 1338
;1337:		&& !level.intermissiontime
;1338:		&& !level.warmupTime && level.sortedClients[1] == clientNum ) {
line 1339
;1339:		level.clients[ level.sortedClients[0] ].sess.wins++;
ADDRLP4 20
CNSTI4 788
ADDRGP4 level+88
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 632
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1340
;1340:		ClientUserinfoChanged( level.sortedClients[0] );
ADDRGP4 level+88
INDIRI4
ARGI4
ADDRGP4 ClientUserinfoChanged
CALLV
pop
line 1341
;1341:	}
LABELV $432
line 1343
;1342:
;1343:	trap_UnlinkEntity (ent);
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 1344
;1344:	ent->s.modelindex = 0;
ADDRLP4 4
INDIRP4
CNSTI4 160
ADDP4
CNSTI4 0
ASGNI4
line 1345
;1345:	ent->inuse = qfalse;
ADDRLP4 4
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 0
ASGNI4
line 1346
;1346:	ent->classname = "disconnected";
ADDRLP4 4
INDIRP4
CNSTI4 524
ADDP4
ADDRGP4 $441
ASGNP4
line 1347
;1347:	ent->client->pers.connected = CON_DISCONNECTED;
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 468
ADDP4
CNSTI4 0
ASGNI4
line 1348
;1348:	ent->client->ps.persistant[PERS_TEAM] = TEAM_FREE;
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 260
ADDP4
CNSTI4 0
ASGNI4
line 1349
;1349:	ent->client->sess.sessionTeam = TEAM_FREE;
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
CNSTI4 0
ASGNI4
line 1351
;1350:
;1351:	trap_SetConfigstring( CS_PLAYERS + clientNum, "");
ADDRFP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRGP4 $442
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 1353
;1352:
;1353:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 1355
;1354:
;1355:	if ( ent->r.svFlags & SVF_BOT ) {
ADDRLP4 4
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $443
line 1356
;1356:		BotAIShutdownClient( clientNum, qfalse );
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 BotAIShutdownClient
CALLI4
pop
line 1357
;1357:	}
LABELV $443
line 1358
;1358:}
LABELV $415
endproc ClientDisconnect 24 8
import trap_SnapVector
import trap_GeneticParentsAndChildSelection
import trap_BotResetWeaponState
import trap_BotFreeWeaponState
import trap_BotAllocWeaponState
import trap_BotLoadWeaponWeights
import trap_BotGetWeaponInfo
import trap_BotChooseBestFightWeapon
import trap_BotAddAvoidSpot
import trap_BotInitMoveState
import trap_BotFreeMoveState
import trap_BotAllocMoveState
import trap_BotPredictVisiblePosition
import trap_BotMovementViewTarget
import trap_BotReachabilityArea
import trap_BotResetLastAvoidReach
import trap_BotResetAvoidReach
import trap_BotMoveInDirection
import trap_BotMoveToGoal
import trap_BotResetMoveState
import trap_BotFreeGoalState
import trap_BotAllocGoalState
import trap_BotMutateGoalFuzzyLogic
import trap_BotSaveGoalFuzzyLogic
import trap_BotInterbreedGoalFuzzyLogic
import trap_BotFreeItemWeights
import trap_BotLoadItemWeights
import trap_BotUpdateEntityItems
import trap_BotInitLevelItems
import trap_BotSetAvoidGoalTime
import trap_BotAvoidGoalTime
import trap_BotGetLevelItemGoal
import trap_BotGetMapLocationGoal
import trap_BotGetNextCampSpotGoal
import trap_BotItemGoalInVisButNotVisible
import trap_BotTouchingGoal
import trap_BotChooseNBGItem
import trap_BotChooseLTGItem
import trap_BotGetSecondGoal
import trap_BotGetTopGoal
import trap_BotGoalName
import trap_BotDumpGoalStack
import trap_BotDumpAvoidGoals
import trap_BotEmptyGoalStack
import trap_BotPopGoal
import trap_BotPushGoal
import trap_BotResetAvoidGoals
import trap_BotRemoveFromAvoidGoals
import trap_BotResetGoalState
import trap_BotSetChatName
import trap_BotSetChatGender
import trap_BotLoadChatFile
import trap_BotReplaceSynonyms
import trap_UnifyWhiteSpaces
import trap_BotMatchVariable
import trap_BotFindMatch
import trap_StringContains
import trap_BotGetChatMessage
import trap_BotEnterChat
import trap_BotChatLength
import trap_BotReplyChat
import trap_BotNumInitialChats
import trap_BotInitialChat
import trap_BotNumConsoleMessages
import trap_BotNextConsoleMessage
import trap_BotRemoveConsoleMessage
import trap_BotQueueConsoleMessage
import trap_BotFreeChatState
import trap_BotAllocChatState
import trap_Characteristic_String
import trap_Characteristic_BInteger
import trap_Characteristic_Integer
import trap_Characteristic_BFloat
import trap_Characteristic_Float
import trap_BotFreeCharacter
import trap_BotLoadCharacter
import trap_EA_ResetInput
import trap_EA_GetInput
import trap_EA_EndRegular
import trap_EA_View
import trap_EA_Move
import trap_EA_DelayedJump
import trap_EA_Jump
import trap_EA_SelectWeapon
import trap_EA_MoveRight
import trap_EA_MoveLeft
import trap_EA_MoveBack
import trap_EA_MoveForward
import trap_EA_MoveDown
import trap_EA_MoveUp
import trap_EA_Crouch
import trap_EA_Respawn
import trap_EA_Use
import trap_EA_Attack
import trap_EA_Talk
import trap_EA_Gesture
import trap_EA_Action
import trap_EA_Command
import trap_EA_SayTeam
import trap_EA_Say
import trap_AAS_PredictClientMovement
import trap_AAS_Swimming
import trap_AAS_AlternativeRouteGoals
import trap_AAS_PredictRoute
import trap_AAS_EnableRoutingArea
import trap_AAS_AreaTravelTimeToGoalArea
import trap_AAS_AreaReachability
import trap_AAS_IntForBSPEpairKey
import trap_AAS_FloatForBSPEpairKey
import trap_AAS_VectorForBSPEpairKey
import trap_AAS_ValueForBSPEpairKey
import trap_AAS_NextBSPEntity
import trap_AAS_PointContents
import trap_AAS_TraceAreas
import trap_AAS_PointReachabilityAreaIndex
import trap_AAS_PointAreaNum
import trap_AAS_Time
import trap_AAS_PresenceTypeBoundingBox
import trap_AAS_Initialized
import trap_AAS_EntityInfo
import trap_AAS_AreaInfo
import trap_AAS_BBoxAreas
import trap_BotUserCommand
import trap_BotGetServerCommand
import trap_BotGetSnapshotEntity
import trap_BotLibTest
import trap_BotLibUpdateEntity
import trap_BotLibLoadMap
import trap_BotLibStartFrame
import trap_BotLibDefine
import trap_BotLibVarGet
import trap_BotLibVarSet
import trap_BotLibShutdown
import trap_BotLibSetup
import trap_DebugPolygonDelete
import trap_DebugPolygonCreate
import trap_GetEntityToken
import trap_GetUsercmd
import trap_BotFreeClient
import trap_BotAllocateClient
import trap_EntityContact
import trap_EntitiesInBox
import trap_UnlinkEntity
import trap_LinkEntity
import trap_AreasConnected
import trap_AdjustAreaPortalState
import trap_InPVSIgnorePortals
import trap_InPVS
import trap_PointContents
import trap_Trace
import trap_SetBrushModel
import trap_GetServerinfo
import trap_SetUserinfo
import trap_GetUserinfo
import trap_GetConfigstring
import trap_SetConfigstring
import trap_SendServerCommand
import trap_DropClient
import trap_LocateGameData
import trap_Cvar_VariableStringBuffer
import trap_Cvar_VariableValue
import trap_Cvar_VariableIntegerValue
import trap_Cvar_Set
import trap_Cvar_Update
import trap_Cvar_Register
import trap_SendConsoleCommand
import trap_FS_Seek
import trap_FS_GetFileList
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import trap_Args
import trap_Argv
import trap_Argc
import trap_Milliseconds
import trap_Error
import trap_Printf
import g_proxMineTimeout
import g_singlePlayer
import g_enableBreath
import g_enableDust
import g_rankings
import pmove_msec
import pmove_fixed
import g_smoothClients
import g_blueteam
import g_redteam
import g_cubeTimeout
import g_obeliskRespawnDelay
import g_obeliskRegenAmount
import g_obeliskRegenPeriod
import g_obeliskHealth
import g_filterBan
import g_banIPs
import g_teamForceBalance
import g_teamAutoJoin
import g_allowVote
import g_blood
import g_doWarmup
import g_warmup
import g_motd
import g_synchronousClients
import g_weaponTeamRespawn
import g_weaponRespawn
import g_debugDamage
import g_debugAlloc
import g_debugMove
import g_inactivity
import g_forcerespawn
import g_quadfactor
import g_knockback
import g_speed
import g_gravity
import g_needpass
import g_password
import g_friendlyFire
import g_capturelimit
import g_timelimit
import g_fraglimit
import g_dmflags
import g_restarted
import g_maxGameClients
import g_maxclients
import g_cheats
import g_dedicated
import g_gametype
import g_entities
import level
import Pickup_Team
import CheckTeamStatus
import TeamplayInfoMessage
import Team_GetLocationMsg
import Team_GetLocation
import SelectCTFSpawnPoint
import Team_FreeEntity
import Team_ReturnFlag
import Team_InitGame
import Team_CheckHurtCarrier
import Team_FragBonuses
import Team_DroppedFlagThink
import AddTeamScore
import TeamColorString
import OtherTeamName
import TeamName
import OtherTeam
import BotTestAAS
import BotAIStartFrame
import BotAIShutdownClient
import BotAISetupClient
import BotAILoadMap
import BotAIShutdown
import BotAISetup
import BotInterbreedEndMatch
import Svcmd_BotList_f
import Svcmd_AddBot_f
import G_BotConnect
import G_RemoveQueuedBotBegin
import G_CheckBotSpawn
import G_GetBotInfoByName
import G_GetBotInfoByNumber
import G_InitBots
import Svcmd_AbortPodium_f
import SpawnModelsOnVictoryPads
import UpdateTournamentInfo
import G_WriteSessionData
import G_InitWorldSession
import G_InitSessionData
import G_ReadSessionData
import Svcmd_GameMem_f
import G_InitMemory
import G_Alloc
import CheckObeliskAttack
import Team_CheckDroppedItem
import OnSameTeam
import G_RunClient
import ClientEndFrame
import ClientThink
import ClientCommand
import G_Error
import G_Printf
import SendScoreboardMessageToAllClients
import G_LogPrintf
import G_RunThink
import CheckTeamLeader
import SetLeader
import FindIntermissionPoint
import DeathmatchScoreboardMessage
import G_SetStats
import MoveClientToIntermission
import FireWeapon
import G_FilterPacket
import G_ProcessIPBans
import ConsoleCommand
import CalculateRanks
import AddScore
import bash_die
import player_die
import InitClientResp
import InitClientPersistant
import BeginIntermission
import Weapon_HookThink
import Weapon_HookFree
import CheckGauntletAttack
import SnapVectorTowards
import CalcMuzzlePoint
import LogAccuracyHit
import TeleportPlayer
import trigger_teleporter_touch
import Touch_DoorTrigger
import G_RunMover
import fire_grapple
import fire_bfg
import fire_rocket
import fire_grenade
import fire_plasma
import fire_blaster
import G_RunMissile
import TossClientCubes
import TossClientItems
import body_die
import G_InvulnerabilityEffect
import G_RadiusDamage
import G_Damage
import CanDamage
import BuildShaderStateConfig
import AddRemap
import G_SetOrigin
import G_AddEvent
import G_AddPredictableEvent
import vectoyaw
import vtos
import tv
import G_TouchSolids
import G_TouchTriggers
import G_EntitiesFree
import G_FreeEntity
import G_Sound
import G_TempEntity
import G_Spawn
import G_InitGentity
import G_SetMovedir
import G_UseTargets
import G_PickTarget
import G_Find
import G_KillBox
import G_TeamCommand
import G_SoundIndex
import G_ModelIndex
import SaveRegisteredItems
import RegisterItem
import ClearRegisteredItems
import Touch_Item
import Add_Ammo
import ArmorIndex
import Think_Weapon
import FinishSpawningItem
import G_SpawnItem
import SetRespawn
import LaunchItem
import Drop_Item
import PrecacheItem
import UseHoldableItem
import RespawnItem
import G_RunItem
import G_CheckTeamItems
import Cmd_FollowCycle_f
import SetTeam
import BroadcastTeamChange
import StopFollowing
import Cmd_Score_f
import G_NewString
import G_SpawnEntitiesFromString
import G_SpawnVector
import G_SpawnInt
import G_SpawnFloat
import G_SpawnString
import BG_PlayerTouchesItem
import BG_PlayerStateToEntityStateExtraPolate
import BG_PlayerStateToEntityState
import BG_TouchJumpPad
import BG_AddPredictableEventToPlayerstate
import BG_EvaluateTrajectoryDelta
import BG_EvaluateTrajectory
import BG_CanItemBeGrabbed
import BG_FindItemForHoldable
import BG_FindItemForPowerup
import BG_FindItemForWeapon
import BG_FindItem
import bg_numItems
import bg_itemlist
import Pmove
import PM_UpdateViewAngles
import Com_Printf
import Com_Error
import Info_NextPair
import Info_Validate
import Info_SetValueForKey_Big
import Info_SetValueForKey
import Info_RemoveKey_big
import Info_RemoveKey
import Info_ValueForKey
import va
import Q_CleanStr
import Q_PrintStrlen
import Q_strcat
import Q_strncpyz
import Q_strrchr
import Q_strupr
import Q_strlwr
import Q_stricmpn
import Q_strncmp
import Q_stricmp
import Q_isalpha
import Q_isupper
import Q_islower
import Q_isprint
import Com_sprintf
import Parse3DMatrix
import Parse2DMatrix
import Parse1DMatrix
import SkipRestOfLine
import SkipBracedSection
import COM_MatchToken
import COM_ParseWarning
import COM_ParseError
import COM_Compress
import COM_ParseExt
import COM_Parse
import COM_GetCurrentParseLine
import COM_BeginParseSession
import COM_DefaultExtension
import COM_StripExtension
import COM_SkipPath
import Com_Clamp
import PerpendicularVector
import AngleVectors
import MatrixMultiply
import MakeNormalVectors
import RotateAroundDirection
import RotatePointAroundVector
import ProjectPointOnPlane
import PlaneFromPoints
import AngleDelta
import AngleNormalize180
import AngleNormalize360
import AnglesSubtract
import AngleSubtract
import LerpAngle
import AngleMod
import BoxOnPlaneSide
import SetPlaneSignbits
import AxisCopy
import AxisClear
import AnglesToAxis
import vectoangles
import Q_crandom
import Q_random
import Q_rand
import Q_acos
import Q_log2
import VectorRotate
import Vector4Scale
import VectorNormalize2
import VectorNormalize
import CrossProduct
import VectorInverse
import VectorNormalizeFast
import DistanceSquared
import Distance
import VectorLengthSquared
import VectorLength
import VectorCompare
import AddPointToBounds
import ClearBounds
import RadiusFromBounds
import NormalizeColor
import ColorBytes4
import ColorBytes3
import _VectorMA
import _VectorScale
import _VectorCopy
import _VectorAdd
import _VectorSubtract
import _DotProduct
import ByteToDir
import DirToByte
import ClampShort
import ClampChar
import Q_rsqrt
import Q_fabs
import axisDefault
import vec3_origin
import g_color_table
import colorDkGrey
import colorMdGrey
import colorLtGrey
import colorWhite
import colorCyan
import colorMagenta
import colorYellow
import colorBlue
import colorGreen
import colorRed
import colorBlack
import bytedirs
import Com_Memcpy
import Com_Memset
import Hunk_Alloc
import FloatSwap
import LongSwap
import ShortSwap
import acos
import fabs
import abs
import tan
import atan2
import cos
import sin
import sqrt
import floor
import ceil
import memcpy
import memset
import memmove
import sscanf
import vsprintf
import _atoi
import atoi
import _atof
import atof
import toupper
import tolower
import strncpy
import strstr
import strchr
import strcmp
import strcpy
import strcat
import strlen
import rand
import srand
import qsort
lit
align 1
LABELV $442
byte 1 0
align 1
LABELV $441
byte 1 100
byte 1 105
byte 1 115
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $431
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 68
byte 1 105
byte 1 115
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $391
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $363
byte 1 99
byte 1 112
byte 1 32
byte 1 34
byte 1 104
byte 1 116
byte 1 116
byte 1 112
byte 1 58
byte 1 47
byte 1 47
byte 1 119
byte 1 119
byte 1 119
byte 1 46
byte 1 112
byte 1 108
byte 1 97
byte 1 110
byte 1 101
byte 1 116
byte 1 113
byte 1 117
byte 1 97
byte 1 107
byte 1 101
byte 1 46
byte 1 99
byte 1 111
byte 1 109
byte 1 47
byte 1 116
byte 1 111
byte 1 120
byte 1 105
byte 1 110
byte 1 97
byte 1 116
byte 1 101
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $362
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 32
byte 1 34
byte 1 94
byte 1 51
byte 1 119
byte 1 104
byte 1 105
byte 1 108
byte 1 101
byte 1 32
byte 1 115
byte 1 116
byte 1 97
byte 1 121
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 111
byte 1 110
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 109
byte 1 97
byte 1 112
byte 1 46
byte 1 32
byte 1 49
byte 1 48
byte 1 32
byte 1 108
byte 1 105
byte 1 118
byte 1 101
byte 1 115
byte 1 32
byte 1 97
byte 1 114
byte 1 101
byte 1 32
byte 1 103
byte 1 105
byte 1 118
byte 1 101
byte 1 110
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $361
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 32
byte 1 34
byte 1 94
byte 1 51
byte 1 79
byte 1 98
byte 1 106
byte 1 101
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 101
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 107
byte 1 110
byte 1 111
byte 1 99
byte 1 107
byte 1 32
byte 1 111
byte 1 116
byte 1 104
byte 1 101
byte 1 114
byte 1 115
byte 1 32
byte 1 111
byte 1 102
byte 1 102
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 109
byte 1 97
byte 1 112
byte 1 44
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $352
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 66
byte 1 101
byte 1 103
byte 1 105
byte 1 110
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $351
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 101
byte 1 100
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $338
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 32
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $335
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 67
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $334
byte 1 66
byte 1 111
byte 1 116
byte 1 67
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 102
byte 1 97
byte 1 105
byte 1 108
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $322
byte 1 73
byte 1 110
byte 1 118
byte 1 97
byte 1 108
byte 1 105
byte 1 100
byte 1 32
byte 1 112
byte 1 97
byte 1 115
byte 1 115
byte 1 119
byte 1 111
byte 1 114
byte 1 100
byte 1 0
align 1
LABELV $320
byte 1 110
byte 1 111
byte 1 110
byte 1 101
byte 1 0
align 1
LABELV $315
byte 1 112
byte 1 97
byte 1 115
byte 1 115
byte 1 119
byte 1 111
byte 1 114
byte 1 100
byte 1 0
align 1
LABELV $312
byte 1 89
byte 1 111
byte 1 117
byte 1 32
byte 1 97
byte 1 114
byte 1 101
byte 1 32
byte 1 98
byte 1 97
byte 1 110
byte 1 110
byte 1 101
byte 1 100
byte 1 32
byte 1 102
byte 1 114
byte 1 111
byte 1 109
byte 1 32
byte 1 116
byte 1 104
byte 1 105
byte 1 115
byte 1 32
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 46
byte 1 0
align 1
LABELV $308
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 85
byte 1 115
byte 1 101
byte 1 114
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 67
byte 1 104
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 100
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $307
byte 1 110
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 116
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 104
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 103
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 103
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 99
byte 1 49
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 99
byte 1 50
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 104
byte 1 99
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 119
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 108
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 116
byte 1 116
byte 1 92
byte 1 37
byte 1 100
byte 1 92
byte 1 116
byte 1 108
byte 1 92
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $306
byte 1 115
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $305
byte 1 110
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 116
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 104
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 99
byte 1 49
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 99
byte 1 50
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 104
byte 1 99
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 119
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 108
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 115
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 116
byte 1 116
byte 1 92
byte 1 37
byte 1 100
byte 1 92
byte 1 116
byte 1 108
byte 1 92
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $302
byte 1 103
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $301
byte 1 103
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $300
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 50
byte 1 0
align 1
LABELV $299
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 49
byte 1 0
align 1
LABELV $298
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 116
byte 1 97
byte 1 115
byte 1 107
byte 1 0
align 1
LABELV $294
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 108
byte 1 97
byte 1 121
byte 1 0
align 1
LABELV $292
byte 1 98
byte 1 0
align 1
LABELV $291
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $287
byte 1 114
byte 1 0
align 1
LABELV $286
byte 1 114
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $283
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $277
byte 1 104
byte 1 101
byte 1 97
byte 1 100
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $276
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $275
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 104
byte 1 101
byte 1 97
byte 1 100
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $274
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $267
byte 1 104
byte 1 97
byte 1 110
byte 1 100
byte 1 105
byte 1 99
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $266
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 32
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 100
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $261
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 98
byte 1 111
byte 1 97
byte 1 114
byte 1 100
byte 1 0
align 1
LABELV $256
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $253
byte 1 99
byte 1 103
byte 1 95
byte 1 112
byte 1 114
byte 1 101
byte 1 100
byte 1 105
byte 1 99
byte 1 116
byte 1 73
byte 1 116
byte 1 101
byte 1 109
byte 1 115
byte 1 0
align 1
LABELV $252
byte 1 108
byte 1 111
byte 1 99
byte 1 97
byte 1 108
byte 1 104
byte 1 111
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $249
byte 1 105
byte 1 112
byte 1 0
align 1
LABELV $248
byte 1 92
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 92
byte 1 98
byte 1 97
byte 1 100
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 0
align 1
LABELV $244
byte 1 85
byte 1 110
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 100
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $144
byte 1 98
byte 1 111
byte 1 100
byte 1 121
byte 1 113
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $124
byte 1 67
byte 1 111
byte 1 117
byte 1 108
byte 1 100
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 102
byte 1 105
byte 1 110
byte 1 100
byte 1 32
byte 1 97
byte 1 32
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 32
byte 1 112
byte 1 111
byte 1 105
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $61
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 95
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 95
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 109
byte 1 97
byte 1 116
byte 1 99
byte 1 104
byte 1 0
align 1
LABELV $57
byte 1 110
byte 1 111
byte 1 104
byte 1 117
byte 1 109
byte 1 97
byte 1 110
byte 1 115
byte 1 0
align 1
LABELV $54
byte 1 48
byte 1 0
align 1
LABELV $53
byte 1 110
byte 1 111
byte 1 98
byte 1 111
byte 1 116
byte 1 115
byte 1 0
