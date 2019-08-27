export ScorePlum
code
proc ScorePlum 12 8
file "../g_combat.c"
line 13
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:// g_combat.c
;4:
;5:#include "g_local.h"
;6:
;7:
;8:/*
;9:============
;10:ScorePlum
;11:============
;12:*/
;13:void ScorePlum( gentity_t *ent, vec3_t origin, int score ) {
line 16
;14:	gentity_t *plum;
;15:
;16:	plum = G_TempEntity( origin, EV_SCOREPLUM );
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 65
ARGI4
ADDRLP4 4
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 18
;17:	// only send this temp entity to a single client
;18:	plum->r.svFlags |= SVF_SINGLECLIENT;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 424
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 256
BORI4
ASGNI4
line 19
;19:	plum->r.singleClient = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 428
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 21
;20:	//
;21:	plum->s.otherEntityNum = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 22
;22:	plum->s.time = score;
ADDRLP4 0
INDIRP4
CNSTI4 84
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 23
;23:}
LABELV $52
endproc ScorePlum 12 8
export AddScore
proc AddScore 8 12
line 32
;24:
;25:/*
;26:============
;27:AddScore
;28:
;29:Adds score to both the client and his team
;30:============
;31:*/
;32:void AddScore( gentity_t *ent, vec3_t origin, int score ) {
line 33
;33:	if ( !ent->client ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $54
line 34
;34:		return;
ADDRGP4 $53
JUMPV
LABELV $54
line 37
;35:	}
;36:	// no scoring during pre-match warmup
;37:	if ( level.warmupTime ) {
ADDRGP4 level+16
INDIRI4
CNSTI4 0
EQI4 $56
line 38
;38:		return;
ADDRGP4 $53
JUMPV
LABELV $56
line 41
;39:	}
;40:	// show score plum
;41:	ScorePlum(ent, origin, score);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 ScorePlum
CALLV
pop
line 43
;42:	//
;43:	ent->client->ps.persistant[PERS_SCORE] += score;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 248
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRFP4 8
INDIRI4
ADDI4
ASGNI4
line 44
;44:	if ( g_gametype.integer == GT_TEAM )
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
NEI4 $59
line 45
;45:		level.teamScores[ ent->client->ps.persistant[PERS_TEAM] ] += score;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 level+48
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
ADDRFP4 8
INDIRI4
ADDI4
ASGNI4
LABELV $59
line 46
;46:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 47
;47:}
LABELV $53
endproc AddScore 8 12
export TossClientItems
proc TossClientItems 40 12
line 56
;48:
;49:/*
;50:=================
;51:TossClientItems
;52:
;53:Toss the weapon and powerups for the killed player
;54:=================
;55:*/
;56:void TossClientItems( gentity_t *self ) {
line 64
;57:	gitem_t		*item;
;58:	int			weapon;
;59:	float		angle;
;60:	int			i;
;61:	gentity_t	*drop;
;62:
;63:	// drop the weapon if not a gauntlet or machinegun
;64:	weapon = self->s.weapon;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
ASGNI4
line 70
;65:
;66:	// make a special check to see if they are changing to a new
;67:	// weapon that isn't the mg or gauntlet.  Without this, a client
;68:	// can pick up a weapon, be killed, and not drop the weapon because
;69:	// their weapon change hasn't completed yet and they are still holding the MG.
;70:	if ( weapon == WP_MACHINEGUN || weapon == WP_GRAPPLING_HOOK ) {
ADDRLP4 12
INDIRI4
CNSTI4 2
EQI4 $66
ADDRLP4 12
INDIRI4
CNSTI4 10
NEI4 $64
LABELV $66
line 71
;71:		if ( self->client->ps.weaponstate == WEAPON_DROPPING ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 2
NEI4 $67
line 72
;72:			weapon = self->client->pers.cmd.weapon;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 492
ADDP4
INDIRU1
CVUI4 1
ASGNI4
line 73
;73:		}
LABELV $67
line 74
;74:		if ( !( self->client->ps.stats[STAT_WEAPONS] & ( 1 << weapon ) ) ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 12
INDIRI4
LSHI4
BANDI4
CNSTI4 0
NEI4 $69
line 75
;75:			weapon = WP_NONE;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 76
;76:		}
LABELV $69
line 77
;77:	}
LABELV $64
line 79
;78:
;79:	if ( weapon > WP_MACHINEGUN && weapon != WP_GRAPPLING_HOOK && 
ADDRLP4 28
CNSTI4 2
ASGNI4
ADDRLP4 12
INDIRI4
ADDRLP4 28
INDIRI4
LEI4 $71
ADDRLP4 12
INDIRI4
CNSTI4 10
EQI4 $71
ADDRLP4 12
INDIRI4
ADDRLP4 28
INDIRI4
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 376
ADDP4
ADDP4
INDIRI4
CNSTI4 0
EQI4 $71
line 80
;80:		self->client->ps.ammo[ weapon ] ) {
line 82
;81:		// find the item type for this weapon
;82:		item = BG_FindItemForWeapon( weapon );
ADDRLP4 12
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 BG_FindItemForWeapon
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 32
INDIRP4
ASGNP4
line 85
;83:
;84:		// spawn the item
;85:		Drop_Item( self, item, 0 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Drop_Item
CALLP4
pop
line 86
;86:	}
LABELV $71
line 89
;87:
;88:	// drop all the powerups if not in teamplay
;89:	if ( g_gametype.integer != GT_TEAM ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
EQI4 $73
line 90
;90:		angle = 45;
ADDRLP4 16
CNSTF4 1110704128
ASGNF4
line 91
;91:		for ( i = 1 ; i < PW_NUM_POWERUPS ; i++ ) {
ADDRLP4 0
CNSTI4 1
ASGNI4
LABELV $76
line 92
;92:			if ( self->client->ps.powerups[ i ] > level.time ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 312
ADDP4
ADDP4
INDIRI4
ADDRGP4 level+36
INDIRI4
LEI4 $80
line 93
;93:				item = BG_FindItemForPowerup( i );
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 BG_FindItemForPowerup
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 32
INDIRP4
ASGNP4
line 94
;94:				if ( !item ) {
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $83
line 95
;95:					continue;
ADDRGP4 $77
JUMPV
LABELV $83
line 97
;96:				}
;97:				drop = Drop_Item( self, item, angle );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 36
ADDRGP4 Drop_Item
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 36
INDIRP4
ASGNP4
line 99
;98:				// decide how many seconds it has left
;99:				drop->count = ( self->client->ps.powerups[ i ] - level.time ) / 1000;
ADDRLP4 4
INDIRP4
CNSTI4 760
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 312
ADDP4
ADDP4
INDIRI4
ADDRGP4 level+36
INDIRI4
SUBI4
CNSTI4 1000
DIVI4
ASGNI4
line 100
;100:				if ( drop->count < 1 ) {
ADDRLP4 4
INDIRP4
CNSTI4 760
ADDP4
INDIRI4
CNSTI4 1
GEI4 $86
line 101
;101:					drop->count = 1;
ADDRLP4 4
INDIRP4
CNSTI4 760
ADDP4
CNSTI4 1
ASGNI4
line 102
;102:				}
LABELV $86
line 103
;103:				angle += 45;
ADDRLP4 16
ADDRLP4 16
INDIRF4
CNSTF4 1110704128
ADDF4
ASGNF4
line 104
;104:			}
LABELV $80
line 105
;105:		}
LABELV $77
line 91
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 15
LTI4 $76
line 106
;106:	}
LABELV $73
line 107
;107:}
LABELV $63
endproc TossClientItems 40 12
export LookAtKiller
proc LookAtKiller 52 4
line 197
;108:
;109:#ifdef MISSIONPACK
;110:
;111:/*
;112:=================
;113:TossClientCubes
;114:=================
;115:*/
;116:extern gentity_t	*neutralObelisk;
;117:
;118:void TossClientCubes( gentity_t *self ) {
;119:	gitem_t		*item;
;120:	gentity_t	*drop;
;121:	vec3_t		velocity;
;122:	vec3_t		angles;
;123:	vec3_t		origin;
;124:
;125:	self->client->ps.generic1 = 0;
;126:
;127:	// this should never happen but we should never
;128:	// get the server to crash due to skull being spawned in
;129:	if (!G_EntitiesFree()) {
;130:		return;
;131:	}
;132:
;133:	if( self->client->sess.sessionTeam == TEAM_RED ) {
;134:		item = BG_FindItem( "Red Cube" );
;135:	}
;136:	else {
;137:		item = BG_FindItem( "Blue Cube" );
;138:	}
;139:
;140:	angles[YAW] = (float)(level.time % 360);
;141:	angles[PITCH] = 0;	// always forward
;142:	angles[ROLL] = 0;
;143:
;144:	AngleVectors( angles, velocity, NULL, NULL );
;145:	VectorScale( velocity, 150, velocity );
;146:	velocity[2] += 200 + crandom() * 50;
;147:
;148:	if( neutralObelisk ) {
;149:		VectorCopy( neutralObelisk->s.pos.trBase, origin );
;150:		origin[2] += 44;
;151:	} else {
;152:		VectorClear( origin ) ;
;153:	}
;154:
;155:	drop = LaunchItem( item, origin, velocity );
;156:
;157:	drop->nextthink = level.time + g_cubeTimeout.integer * 1000;
;158:	drop->think = G_FreeEntity;
;159:	drop->spawnflags = self->client->sess.sessionTeam;
;160:}
;161:
;162:
;163:/*
;164:=================
;165:TossClientPersistantPowerups
;166:=================
;167:*/
;168:void TossClientPersistantPowerups( gentity_t *ent ) {
;169:	gentity_t	*powerup;
;170:
;171:	if( !ent->client ) {
;172:		return;
;173:	}
;174:
;175:	if( !ent->client->persistantPowerup ) {
;176:		return;
;177:	}
;178:
;179:	powerup = ent->client->persistantPowerup;
;180:
;181:	powerup->r.svFlags &= ~SVF_NOCLIENT;
;182:	powerup->s.eFlags &= ~EF_NODRAW;
;183:	powerup->r.contents = CONTENTS_TRIGGER;
;184:	trap_LinkEntity( powerup );
;185:
;186:	ent->client->ps.stats[STAT_PERSISTANT_POWERUP] = 0;
;187:	ent->client->persistantPowerup = NULL;
;188:}
;189:#endif
;190:
;191:
;192:/*
;193:==================
;194:LookAtKiller
;195:==================
;196:*/
;197:void LookAtKiller( gentity_t *self, gentity_t *inflictor, gentity_t *attacker ) {
line 201
;198:	vec3_t		dir;
;199:	vec3_t		angles;
;200:
;201:	if ( attacker && attacker != self ) {
ADDRLP4 24
ADDRFP4 8
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 24
INDIRU4
CNSTU4 0
EQU4 $89
ADDRLP4 24
INDIRU4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $89
line 202
;202:		VectorSubtract (attacker->s.pos.trBase, self->s.pos.trBase, dir);
ADDRLP4 28
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 32
CNSTI4 24
ASGNI4
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 28
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDRLP4 36
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 40
CNSTI4 28
ASGNI4
ADDRLP4 0+4
ADDRLP4 28
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRF4
ADDRLP4 36
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 44
CNSTI4 32
ASGNI4
ADDRLP4 0+8
ADDRFP4 8
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 203
;203:	} else if ( inflictor && inflictor != self ) {
ADDRGP4 $90
JUMPV
LABELV $89
ADDRLP4 28
ADDRFP4 4
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 28
INDIRU4
CNSTU4 0
EQU4 $93
ADDRLP4 28
INDIRU4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $93
line 204
;204:		VectorSubtract (inflictor->s.pos.trBase, self->s.pos.trBase, dir);
ADDRLP4 32
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 36
CNSTI4 24
ASGNI4
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 32
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 44
CNSTI4 28
ASGNI4
ADDRLP4 0+4
ADDRLP4 32
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 48
CNSTI4 32
ASGNI4
ADDRLP4 0+8
ADDRFP4 4
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 205
;205:	} else {
ADDRGP4 $94
JUMPV
LABELV $93
line 206
;206:		self->client->ps.stats[STAT_DEAD_YAW] = self->s.angles[YAW];
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 32
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
CVFI4 4
ASGNI4
line 207
;207:		return;
ADDRGP4 $88
JUMPV
LABELV $94
LABELV $90
line 210
;208:	}
;209:
;210:	self->client->ps.stats[STAT_DEAD_YAW] = vectoyaw ( dir );
ADDRLP4 0
ARGP4
ADDRLP4 32
ADDRGP4 vectoyaw
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 32
INDIRF4
CVFI4 4
ASGNI4
line 212
;211:
;212:	angles[YAW] = vectoyaw ( dir );
ADDRLP4 0
ARGP4
ADDRLP4 36
ADDRGP4 vectoyaw
CALLF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 36
INDIRF4
ASGNF4
line 213
;213:	angles[PITCH] = 0; 
ADDRLP4 12
CNSTF4 0
ASGNF4
line 214
;214:	angles[ROLL] = 0;
ADDRLP4 12+8
CNSTF4 0
ASGNF4
line 215
;215:}
LABELV $88
endproc LookAtKiller 52 4
export GibEntity
proc GibEntity 12 12
line 222
;216:
;217:/*
;218:==================
;219:GibEntity
;220:==================
;221:*/
;222:void GibEntity( gentity_t *self, int killer ) {
line 227
;223:	gentity_t *ent;
;224:	int i;
;225:
;226:	//if this entity still has kamikaze
;227:	if (self->s.eFlags & EF_KAMIKAZE) {
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 512
BANDI4
CNSTI4 0
EQI4 $100
line 229
;228:		// check if there is a kamikaze timer around for this owner
;229:		for (i = 0; i < MAX_GENTITIES; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $102
line 230
;230:			ent = &g_entities[i];
ADDRLP4 0
CNSTI4 808
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 231
;231:			if (!ent->inuse)
ADDRLP4 0
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $106
line 232
;232:				continue;
ADDRGP4 $103
JUMPV
LABELV $106
line 233
;233:			if (ent->activator != self)
ADDRLP4 0
INDIRP4
CNSTI4 772
ADDP4
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $108
line 234
;234:				continue;
ADDRGP4 $103
JUMPV
LABELV $108
line 235
;235:			if (strcmp(ent->classname, "kamikaze timer"))
ADDRLP4 0
INDIRP4
CNSTI4 524
ADDP4
INDIRP4
ARGP4
ADDRGP4 $112
ARGP4
ADDRLP4 8
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $110
line 236
;236:				continue;
ADDRGP4 $103
JUMPV
LABELV $110
line 237
;237:			G_FreeEntity(ent);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 238
;238:			break;
ADDRGP4 $104
JUMPV
LABELV $103
line 229
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 1024
LTI4 $102
LABELV $104
line 240
;239:		}
;240:	}
LABELV $100
line 241
;241:	G_AddEvent( self, EV_GIB_PLAYER, killer );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 242
;242:	self->takedamage = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
CNSTI4 0
ASGNI4
line 243
;243:	self->s.eType = ET_INVISIBLE;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 10
ASGNI4
line 244
;244:	self->r.contents = 0;
ADDRFP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 0
ASGNI4
line 245
;245:}
LABELV $99
endproc GibEntity 12 12
export body_die
proc body_die 0 8
line 252
;246:
;247:/*
;248:==================
;249:body_die
;250:==================
;251:*/
;252:void body_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath ) {
line 253
;253:	if ( self->health > GIB_HEALTH ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 -40
LEI4 $114
line 254
;254:		return;
ADDRGP4 $113
JUMPV
LABELV $114
line 256
;255:	}
;256:	if ( !g_blood.integer ) {
ADDRGP4 g_blood+12
INDIRI4
CNSTI4 0
NEI4 $116
line 257
;257:		self->health = GIB_HEALTH+1;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
CNSTI4 -39
ASGNI4
line 258
;258:		return;
ADDRGP4 $113
JUMPV
LABELV $116
line 261
;259:	}
;260:
;261:	GibEntity( self, 0 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 GibEntity
CALLV
pop
line 262
;262:}
LABELV $113
endproc body_die 0 8
data
export modNames
align 4
LABELV modNames
address $119
address $120
address $121
address $122
address $123
address $124
address $125
address $126
address $127
address $128
address $129
address $130
address $131
address $132
address $133
address $134
address $135
address $136
address $137
address $138
address $139
address $140
address $141
address $142
address $143
export CheckAlmostCapture
code
proc CheckAlmostCapture 56 12
line 337
;263:
;264:
;265:// these are just for logging, the client prints its own messages
;266:char	*modNames[] = {
;267:	"MOD_UNKNOWN",
;268:	"MOD_SHOTGUN",
;269:	"MOD_GAUNTLET",
;270:	"MOD_MACHINEGUN",
;271:	"MOD_GRENADE",
;272:	"MOD_GRENADE_SPLASH",
;273:	"MOD_ROCKET",
;274:	"MOD_ROCKET_SPLASH",
;275:	"MOD_PLASMA",
;276:	"MOD_PLASMA_SPLASH",
;277:	"MOD_RAILGUN",
;278:	"MOD_LIGHTNING",
;279:	"MOD_BFG",
;280:	"MOD_BFG_SPLASH",
;281:	"MOD_WATER",
;282:	"MOD_SLIME",
;283:	"MOD_LAVA",
;284:	"MOD_CRUSH",
;285:	"MOD_TELEFRAG",
;286:	"MOD_FALLING",
;287:	"MOD_SUICIDE",
;288:	"MOD_TARGET_LASER",
;289:	"MOD_TRIGGER_HURT",
;290:	"MOD_FALL",
;291:#ifdef MISSIONPACK
;292:	"MOD_NAIL",
;293:	"MOD_CHAINGUN",
;294:	"MOD_PROXIMITY_MINE",
;295:	"MOD_KAMIKAZE",
;296:	"MOD_JUICED",
;297:#endif
;298:	"MOD_GRAPPLE"
;299:};
;300:
;301:#ifdef MISSIONPACK
;302:/*
;303:==================
;304:Kamikaze_DeathActivate
;305:==================
;306:*/
;307:void Kamikaze_DeathActivate( gentity_t *ent ) {
;308:	G_StartKamikaze(ent);
;309:	G_FreeEntity(ent);
;310:}
;311:
;312:/*
;313:==================
;314:Kamikaze_DeathTimer
;315:==================
;316:*/
;317:void Kamikaze_DeathTimer( gentity_t *self ) {
;318:	gentity_t *ent;
;319:
;320:	ent = G_Spawn();
;321:	ent->classname = "kamikaze timer";
;322:	VectorCopy(self->s.pos.trBase, ent->s.pos.trBase);
;323:	ent->r.svFlags |= SVF_NOCLIENT;
;324:	ent->think = Kamikaze_DeathActivate;
;325:	ent->nextthink = level.time + 5 * 1000;
;326:
;327:	ent->activator = self;
;328:}
;329:
;330:#endif
;331:
;332:/*
;333:==================
;334:CheckAlmostCapture
;335:==================
;336:*/
;337:void CheckAlmostCapture( gentity_t *self, gentity_t *attacker ) {
line 343
;338:	gentity_t	*ent;
;339:	vec3_t		dir;
;340:	char		*classname;
;341:
;342:	// if this player was carrying a flag
;343:	if ( self->client->ps.powerups[PW_REDFLAG] ||
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 24
CNSTI4 0
ASGNI4
ADDRLP4 20
INDIRP4
CNSTI4 340
ADDP4
INDIRI4
ADDRLP4 24
INDIRI4
NEI4 $148
ADDRLP4 20
INDIRP4
CNSTI4 344
ADDP4
INDIRI4
ADDRLP4 24
INDIRI4
NEI4 $148
ADDRLP4 20
INDIRP4
CNSTI4 348
ADDP4
INDIRI4
ADDRLP4 24
INDIRI4
EQI4 $145
LABELV $148
line 345
;344:		self->client->ps.powerups[PW_BLUEFLAG] ||
;345:		self->client->ps.powerups[PW_NEUTRALFLAG] ) {
line 347
;346:		// get the goal flag this player should have been going for
;347:		if ( g_gametype.integer == GT_CTF ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 4
NEI4 $149
line 348
;348:			if ( self->client->sess.sessionTeam == TEAM_BLUE ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 2
NEI4 $152
line 349
;349:				classname = "team_CTF_blueflag";
ADDRLP4 4
ADDRGP4 $154
ASGNP4
line 350
;350:			}
ADDRGP4 $150
JUMPV
LABELV $152
line 351
;351:			else {
line 352
;352:				classname = "team_CTF_redflag";
ADDRLP4 4
ADDRGP4 $155
ASGNP4
line 353
;353:			}
line 354
;354:		}
ADDRGP4 $150
JUMPV
LABELV $149
line 355
;355:		else {
line 356
;356:			if ( self->client->sess.sessionTeam == TEAM_BLUE ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 2
NEI4 $156
line 357
;357:				classname = "team_CTF_redflag";
ADDRLP4 4
ADDRGP4 $155
ASGNP4
line 358
;358:			}
ADDRGP4 $157
JUMPV
LABELV $156
line 359
;359:			else {
line 360
;360:				classname = "team_CTF_blueflag";
ADDRLP4 4
ADDRGP4 $154
ASGNP4
line 361
;361:			}
LABELV $157
line 362
;362:		}
LABELV $150
line 363
;363:		ent = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
LABELV $158
line 365
;364:		do
;365:		{
line 366
;366:			ent = G_Find(ent, FOFS(classname), classname);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 524
ARGI4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 28
INDIRP4
ASGNP4
line 367
;367:		} while (ent && (ent->flags & FL_DROPPED_ITEM));
LABELV $159
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $161
ADDRLP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
NEI4 $158
LABELV $161
line 369
;368:		// if we found the destination flag and it's not picked up
;369:		if (ent && !(ent->r.svFlags & SVF_NOCLIENT) ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $162
ADDRLP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $162
line 371
;370:			// if the player was *very* close
;371:			VectorSubtract( self->client->ps.origin, ent->s.origin, dir );
ADDRLP4 36
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 8
ADDRLP4 36
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 8+4
ADDRLP4 36
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 8+8
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
SUBF4
ASGNF4
line 372
;372:			if ( VectorLength(dir) < 200 ) {
ADDRLP4 8
ARGP4
ADDRLP4 44
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 44
INDIRF4
CNSTF4 1128792064
GEF4 $166
line 373
;373:				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 374
;374:				if ( attacker->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $168
line 375
;375:					attacker->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
ADDRLP4 52
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 376
;376:				}
LABELV $168
line 377
;377:			}
LABELV $166
line 378
;378:		}
LABELV $162
line 379
;379:	}
LABELV $145
line 380
;380:}
LABELV $144
endproc CheckAlmostCapture 56 12
export CheckAlmostScored
proc CheckAlmostScored 44 12
line 387
;381:
;382:/*
;383:==================
;384:CheckAlmostScored
;385:==================
;386:*/
;387:void CheckAlmostScored( gentity_t *self, gentity_t *attacker ) {
line 393
;388:	gentity_t	*ent;
;389:	vec3_t		dir;
;390:	char		*classname;
;391:
;392:	// if the player was carrying cubes
;393:	if ( self->client->ps.generic1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 440
ADDP4
INDIRI4
CNSTI4 0
EQI4 $171
line 394
;394:		if ( self->client->sess.sessionTeam == TEAM_BLUE ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 2
NEI4 $173
line 395
;395:			classname = "team_redobelisk";
ADDRLP4 16
ADDRGP4 $175
ASGNP4
line 396
;396:		}
ADDRGP4 $174
JUMPV
LABELV $173
line 397
;397:		else {
line 398
;398:			classname = "team_blueobelisk";
ADDRLP4 16
ADDRGP4 $176
ASGNP4
line 399
;399:		}
LABELV $174
line 400
;400:		ent = G_Find(NULL, FOFS(classname), classname);
CNSTP4 0
ARGP4
CNSTI4 524
ARGI4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 402
;401:		// if we found the destination obelisk
;402:		if ( ent ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $177
line 404
;403:			// if the player was *very* close
;404:			VectorSubtract( self->client->ps.origin, ent->s.origin, dir );
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 28
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 24
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 28
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 24
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 28
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
SUBF4
ASGNF4
line 405
;405:			if ( VectorLength(dir) < 200 ) {
ADDRLP4 4
ARGP4
ADDRLP4 32
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 32
INDIRF4
CNSTF4 1128792064
GEF4 $181
line 406
;406:				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
ADDRLP4 36
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 407
;407:				if ( attacker->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $183
line 408
;408:					attacker->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
ADDRLP4 40
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 409
;409:				}
LABELV $183
line 410
;410:			}
LABELV $181
line 411
;411:		}
LABELV $177
line 412
;412:	}
LABELV $171
line 413
;413:}
LABELV $170
endproc CheckAlmostScored 44 12
bss
align 4
LABELV $254
skip 4
export player_die
code
proc player_die 96 28
line 420
;414:
;415:/*
;416:==================
;417:player_die
;418:==================
;419:*/
;420:void player_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath ) {
line 428
;421:	gentity_t	*ent;
;422:	int			anim;
;423:	int			contents;
;424:	int			killer;
;425:	int			i;
;426:	char		*killerName, *obit;
;427:
;428:	if ( self->client->ps.pm_type == PM_DEAD ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
NEI4 $186
line 429
;429:		return;
ADDRGP4 $185
JUMPV
LABELV $186
line 432
;430:	}
;431:
;432:	if ( level.intermissiontime ) {
ADDRGP4 level+9132
INDIRI4
CNSTI4 0
EQI4 $188
line 433
;433:		return;
ADDRGP4 $185
JUMPV
LABELV $188
line 437
;434:	}
;435:
;436:	// check for an almost capture
;437:	CheckAlmostCapture( self, attacker );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 CheckAlmostCapture
CALLV
pop
line 439
;438:	// check for a player that almost brought in cubes
;439:	CheckAlmostScored( self, attacker );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 CheckAlmostScored
CALLV
pop
line 441
;440:
;441:	if (self->client && self->client->hook) {
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 32
CNSTU4 0
ASGNU4
ADDRLP4 28
INDIRP4
CVPU4 4
ADDRLP4 32
INDIRU4
EQU4 $191
ADDRLP4 28
INDIRP4
CNSTI4 772
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 32
INDIRU4
EQU4 $191
line 442
;442:		Weapon_HookFree(self->client->hook);
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 772
ADDP4
INDIRP4
ARGP4
ADDRGP4 Weapon_HookFree
CALLV
pop
line 443
;443:	}
LABELV $191
line 451
;444:#ifdef MISSIONPACK
;445:	if ((self->client->ps.eFlags & EF_TICKING) && self->activator) {
;446:		self->client->ps.eFlags &= ~EF_TICKING;
;447:		self->activator->think = G_FreeEntity;
;448:		self->activator->nextthink = level.time;
;449:	}
;450:#endif
;451:	self->client->ps.pm_type = PM_DEAD;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 3
ASGNI4
line 453
;452:
;453:	if ( attacker ) {
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $193
line 454
;454:		killer = attacker->s.number;
ADDRLP4 4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 455
;455:		if ( attacker->client ) {
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $195
line 456
;456:			killerName = attacker->client->pers.netname;
ADDRLP4 16
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 512
ADDP4
ASGNP4
line 457
;457:		} else {
ADDRGP4 $194
JUMPV
LABELV $195
line 458
;458:			killerName = "<non-client>";
ADDRLP4 16
ADDRGP4 $197
ASGNP4
line 459
;459:		}
line 460
;460:	} else {
ADDRGP4 $194
JUMPV
LABELV $193
line 461
;461:		killer = ENTITYNUM_WORLD;
ADDRLP4 4
CNSTI4 1022
ASGNI4
line 462
;462:		killerName = "<world>";
ADDRLP4 16
ADDRGP4 $198
ASGNP4
line 463
;463:	}
LABELV $194
line 465
;464:
;465:	if ( killer < 0 || killer >= MAX_CLIENTS ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $201
ADDRLP4 4
INDIRI4
CNSTI4 64
LTI4 $199
LABELV $201
line 466
;466:		killer = ENTITYNUM_WORLD;
ADDRLP4 4
CNSTI4 1022
ASGNI4
line 467
;467:		killerName = "<world>";
ADDRLP4 16
ADDRGP4 $198
ASGNP4
line 468
;468:	}
LABELV $199
line 470
;469:
;470:	if ( meansOfDeath < 0 || meansOfDeath >= sizeof( modNames ) / sizeof( modNames[0] ) ) {
ADDRLP4 40
ADDRFP4 16
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
LTI4 $204
ADDRLP4 40
INDIRI4
CVIU4 4
CNSTU4 25
LTU4 $202
LABELV $204
line 471
;471:		obit = "<bad obituary>";
ADDRLP4 20
ADDRGP4 $205
ASGNP4
line 472
;472:	} else {
ADDRGP4 $203
JUMPV
LABELV $202
line 473
;473:		obit = modNames[ meansOfDeath ];
ADDRLP4 20
ADDRFP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 modNames
ADDP4
INDIRP4
ASGNP4
line 474
;474:	}
LABELV $203
line 476
;475:
;476:	G_LogPrintf("Kill: %i %i %i: %s killed %s by %s\n", 
ADDRGP4 $206
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
INDIRI4
ARGI4
ADDRFP4 16
INDIRI4
ARGI4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 44
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 481
;477:		killer, self->s.number, meansOfDeath, killerName, 
;478:		self->client->pers.netname, obit );
;479:
;480:	// broadcast the death event to everyone
;481:	ent = G_TempEntity( self->r.currentOrigin, EV_OBITUARY );
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ARGP4
CNSTI4 60
ARGI4
ADDRLP4 48
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 48
INDIRP4
ASGNP4
line 482
;482:	ent->s.eventParm = meansOfDeath;
ADDRLP4 8
INDIRP4
CNSTI4 184
ADDP4
ADDRFP4 16
INDIRI4
ASGNI4
line 483
;483:	ent->s.otherEntityNum = self->s.number;
ADDRLP4 8
INDIRP4
CNSTI4 140
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 484
;484:	ent->s.otherEntityNum2 = killer;
ADDRLP4 8
INDIRP4
CNSTI4 144
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 485
;485:	ent->r.svFlags = SVF_BROADCAST;	// send to everyone
ADDRLP4 8
INDIRP4
CNSTI4 424
ADDP4
CNSTI4 32
ASGNI4
line 487
;486:
;487:	self->enemy = attacker;
ADDRFP4 0
INDIRP4
CNSTI4 768
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 489
;488:
;489:	self->client->ps.persistant[PERS_KILLED]++;
ADDRLP4 52
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 280
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 491
;490:
;491:	if (attacker && attacker->client) {
ADDRLP4 56
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 60
CNSTU4 0
ASGNU4
ADDRLP4 56
INDIRP4
CVPU4 4
ADDRLP4 60
INDIRU4
EQU4 $207
ADDRLP4 56
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 60
INDIRU4
EQU4 $207
line 492
;492:		attacker->client->lastkilled_client = self->s.number;
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 720
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 494
;493:
;494:		if ( attacker == self || OnSameTeam (self, attacker ) ) {
ADDRLP4 64
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CVPU4 4
ADDRLP4 68
INDIRP4
CVPU4 4
EQU4 $211
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 72
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
EQI4 $209
LABELV $211
line 495
;495:			AddScore( attacker, self->r.currentOrigin, -1 );
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 496
;496:		} else {
ADDRGP4 $208
JUMPV
LABELV $209
line 497
;497:			AddScore( attacker, self->r.currentOrigin, 1 );
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 499
;498:
;499:			if( meansOfDeath == MOD_GAUNTLET ) {
ADDRFP4 16
INDIRI4
CNSTI4 2
NEI4 $212
line 502
;500:				
;501:				// play humiliation on player
;502:				attacker->client->ps.persistant[PERS_GAUNTLET_FRAG_COUNT]++;
ADDRLP4 76
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 300
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 505
;503:
;504:				// add the sprite over the player's head
;505:				attacker->client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
ADDRLP4 80
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 80
INDIRP4
ADDRLP4 80
INDIRP4
INDIRI4
CNSTI4 -231497
BANDI4
ASGNI4
line 506
;506:				attacker->client->ps.eFlags |= EF_AWARD_GAUNTLET;
ADDRLP4 84
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 507
;507:				attacker->client->rewardTime = level.time + REWARD_SPRITE_TIME;
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 756
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 2000
ADDI4
ASGNI4
line 510
;508:
;509:				// also play humiliation on target
;510:				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_GAUNTLETREWARD;
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 88
INDIRP4
ADDRLP4 88
INDIRP4
INDIRI4
CNSTI4 2
BXORI4
ASGNI4
line 511
;511:			}
LABELV $212
line 515
;512:
;513:			// check for two kills in a short amount of time
;514:			// if this is close enough to the last kill, give a reward sound
;515:			if ( level.time - attacker->client->lastKillTime < CARNAGE_REWARD_TIME ) {
ADDRGP4 level+36
INDIRI4
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 764
ADDP4
INDIRI4
SUBI4
CNSTI4 3000
GEI4 $215
line 517
;516:				// play excellent on player
;517:				attacker->client->ps.persistant[PERS_EXCELLENT_COUNT]++;
ADDRLP4 76
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 288
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 520
;518:
;519:				// add the sprite over the player's head
;520:				attacker->client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
ADDRLP4 80
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 80
INDIRP4
ADDRLP4 80
INDIRP4
INDIRI4
CNSTI4 -231497
BANDI4
ASGNI4
line 521
;521:				attacker->client->ps.eFlags |= EF_AWARD_EXCELLENT;
ADDRLP4 84
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 522
;522:				attacker->client->rewardTime = level.time + REWARD_SPRITE_TIME;
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 756
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 2000
ADDI4
ASGNI4
line 523
;523:			}
LABELV $215
line 524
;524:			attacker->client->lastKillTime = level.time;
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 764
ADDP4
ADDRGP4 level+36
INDIRI4
ASGNI4
line 526
;525:
;526:		}
line 527
;527:	} else {
ADDRGP4 $208
JUMPV
LABELV $207
line 528
;528:		AddScore( self, self->r.currentOrigin, -1 );
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 64
INDIRP4
CNSTI4 488
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 529
;529:	}
LABELV $208
line 532
;530:
;531:	// Add team bonuses
;532:	Team_FragBonuses(self, inflictor, attacker);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Team_FragBonuses
CALLV
pop
line 535
;533:
;534:	// if I committed suicide, the flag does not fall, it returns.
;535:	if (meansOfDeath == MOD_SUICIDE) {
ADDRFP4 16
INDIRI4
CNSTI4 20
NEI4 $220
line 536
;536:		if ( self->client->ps.powerups[PW_NEUTRALFLAG] ) {		// only happens in One Flag CTF
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 348
ADDP4
INDIRI4
CNSTI4 0
EQI4 $222
line 537
;537:			Team_ReturnFlag( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 538
;538:			self->client->ps.powerups[PW_NEUTRALFLAG] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 348
ADDP4
CNSTI4 0
ASGNI4
line 539
;539:		}
ADDRGP4 $223
JUMPV
LABELV $222
line 540
;540:		else if ( self->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 340
ADDP4
INDIRI4
CNSTI4 0
EQI4 $224
line 541
;541:			Team_ReturnFlag( TEAM_RED );
CNSTI4 1
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 542
;542:			self->client->ps.powerups[PW_REDFLAG] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 0
ASGNI4
line 543
;543:		}
ADDRGP4 $225
JUMPV
LABELV $224
line 544
;544:		else if ( self->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 344
ADDP4
INDIRI4
CNSTI4 0
EQI4 $226
line 545
;545:			Team_ReturnFlag( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 546
;546:			self->client->ps.powerups[PW_BLUEFLAG] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 344
ADDP4
CNSTI4 0
ASGNI4
line 547
;547:		}
LABELV $226
LABELV $225
LABELV $223
line 548
;548:	}
LABELV $220
line 551
;549:
;550:	// if client is in a nodrop area, don't drop anything (but return CTF flags!)
;551:	contents = trap_PointContents( self->r.currentOrigin, -1 );
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 64
ADDRGP4 trap_PointContents
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 64
INDIRI4
ASGNI4
line 552
;552:	if ( !( contents & CONTENTS_NODROP )) {
ADDRLP4 12
INDIRI4
CVIU4 4
CNSTU4 2147483648
BANDU4
CNSTU4 0
NEU4 $228
line 553
;553:		TossClientItems( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 TossClientItems
CALLV
pop
line 554
;554:	}
ADDRGP4 $229
JUMPV
LABELV $228
line 555
;555:	else {
line 556
;556:		if ( self->client->ps.powerups[PW_NEUTRALFLAG] ) {		// only happens in One Flag CTF
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 348
ADDP4
INDIRI4
CNSTI4 0
EQI4 $230
line 557
;557:			Team_ReturnFlag( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 558
;558:		}
ADDRGP4 $231
JUMPV
LABELV $230
line 559
;559:		else if ( self->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 340
ADDP4
INDIRI4
CNSTI4 0
EQI4 $232
line 560
;560:			Team_ReturnFlag( TEAM_RED );
CNSTI4 1
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 561
;561:		}
ADDRGP4 $233
JUMPV
LABELV $232
line 562
;562:		else if ( self->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 344
ADDP4
INDIRI4
CNSTI4 0
EQI4 $234
line 563
;563:			Team_ReturnFlag( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 564
;564:		}
LABELV $234
LABELV $233
LABELV $231
line 565
;565:	}
LABELV $229
line 573
;566:#ifdef MISSIONPACK
;567:	TossClientPersistantPowerups( self );
;568:	if( g_gametype.integer == GT_HARVESTER ) {
;569:		TossClientCubes( self );
;570:	}
;571:#endif
;572:
;573:	Cmd_Score_f( self );		// show scores
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Cmd_Score_f
CALLV
pop
line 576
;574:	// send updated scores to any clients that are following this one,
;575:	// or they would get stale scoreboards
;576:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $239
JUMPV
LABELV $236
line 579
;577:		gclient_t	*client;
;578:
;579:		client = &level.clients[i];
ADDRLP4 68
CNSTI4 788
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 580
;580:		if ( client->pers.connected != CON_CONNECTED ) {
ADDRLP4 68
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
EQI4 $241
line 581
;581:			continue;
ADDRGP4 $237
JUMPV
LABELV $241
line 583
;582:		}
;583:		if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRLP4 68
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
EQI4 $243
line 584
;584:			continue;
ADDRGP4 $237
JUMPV
LABELV $243
line 586
;585:		}
;586:		if ( client->sess.spectatorClient == self->s.number ) {
ADDRLP4 68
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $245
line 587
;587:			Cmd_Score_f( g_entities + i );
CNSTI4 808
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRGP4 Cmd_Score_f
CALLV
pop
line 588
;588:		}
LABELV $245
line 589
;589:	}
LABELV $237
line 576
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $239
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $236
line 591
;590:
;591:	self->takedamage = qtrue;	// can still be gibbed
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
CNSTI4 1
ASGNI4
line 593
;592:
;593:	self->s.weapon = WP_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
CNSTI4 0
ASGNI4
line 594
;594:	self->s.powerups = 0;
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
CNSTI4 0
ASGNI4
line 595
;595:	self->r.contents = CONTENTS_CORPSE;
ADDRFP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 67108864
ASGNI4
line 597
;596:
;597:	self->s.angles[0] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
CNSTF4 0
ASGNF4
line 598
;598:	self->s.angles[2] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
CNSTF4 0
ASGNF4
line 599
;599:	LookAtKiller (self, inflictor, attacker);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 LookAtKiller
CALLV
pop
line 601
;600:
;601:	VectorCopy( self->s.angles, self->client->ps.viewangles );
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 152
ADDP4
ADDRLP4 68
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 603
;602:
;603:	self->s.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 156
ADDP4
CNSTI4 0
ASGNI4
line 605
;604:
;605:	self->r.maxs[2] = -8;
ADDRFP4 0
INDIRP4
CNSTI4 456
ADDP4
CNSTF4 3238002688
ASGNF4
line 609
;606:
;607:	// don't allow respawn until the death anim is done
;608:	// g_forcerespawn may force spawning at some later time
;609:	self->client->respawnTime = level.time + 1700;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 744
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 1700
ADDI4
ASGNI4
line 612
;610:
;611:	// remove powerups
;612:	memset( self->client->ps.powerups, 0, sizeof(self->client->ps.powerups) );
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 312
ADDP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 615
;613:
;614:	// never gib in a nodrop
;615:	if ( (self->health <= GIB_HEALTH && !(contents & CONTENTS_NODROP) && g_blood.integer) || meansOfDeath == MOD_SUICIDE) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 -40
GTI4 $253
ADDRLP4 12
INDIRI4
CVIU4 4
CNSTU4 2147483648
BANDU4
CNSTU4 0
NEU4 $253
ADDRGP4 g_blood+12
INDIRI4
CNSTI4 0
NEI4 $251
LABELV $253
ADDRFP4 16
INDIRI4
CNSTI4 20
NEI4 $248
LABELV $251
line 617
;616:		// gib death
;617:		GibEntity( self, killer );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 GibEntity
CALLV
pop
line 618
;618:	} else {
ADDRGP4 $249
JUMPV
LABELV $248
line 622
;619:		// normal death
;620:		static int i;
;621:
;622:		switch ( i ) {
ADDRLP4 72
ADDRGP4 $254
INDIRI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
EQI4 $257
ADDRLP4 72
INDIRI4
CNSTI4 1
EQI4 $258
ADDRLP4 72
INDIRI4
CNSTI4 2
EQI4 $259
ADDRGP4 $255
JUMPV
LABELV $257
line 624
;623:		case 0:
;624:			anim = BOTH_DEATH1;
ADDRLP4 24
CNSTI4 0
ASGNI4
line 625
;625:			break;
ADDRGP4 $256
JUMPV
LABELV $258
line 627
;626:		case 1:
;627:			anim = BOTH_DEATH2;
ADDRLP4 24
CNSTI4 2
ASGNI4
line 628
;628:			break;
ADDRGP4 $256
JUMPV
LABELV $259
LABELV $255
line 631
;629:		case 2:
;630:		default:
;631:			anim = BOTH_DEATH3;
ADDRLP4 24
CNSTI4 4
ASGNI4
line 632
;632:			break;
LABELV $256
line 637
;633:		}
;634:
;635:		// for the no-blood option, we need to prevent the health
;636:		// from going to gib level
;637:		if ( self->health <= GIB_HEALTH ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 -40
GTI4 $260
line 638
;638:			self->health = GIB_HEALTH+1;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
CNSTI4 -39
ASGNI4
line 639
;639:		}
LABELV $260
line 641
;640:
;641:		self->client->ps.legsAnim = 
ADDRLP4 76
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 76
ADDP4
ASGNP4
ADDRLP4 80
CNSTI4 128
ASGNI4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
ADDRLP4 80
INDIRI4
BANDI4
ADDRLP4 80
INDIRI4
BXORI4
ADDRLP4 24
INDIRI4
BORI4
ASGNI4
line 643
;642:			( ( self->client->ps.legsAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;
;643:		self->client->ps.torsoAnim = 
ADDRLP4 84
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 84
ADDP4
ASGNP4
ADDRLP4 88
CNSTI4 128
ASGNI4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
ADDRLP4 88
INDIRI4
BANDI4
ADDRLP4 88
INDIRI4
BXORI4
ADDRLP4 24
INDIRI4
BORI4
ASGNI4
line 646
;644:			( ( self->client->ps.torsoAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;
;645:
;646:		G_AddEvent( self, EV_DEATH1 + i, killer );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $254
INDIRI4
CNSTI4 57
ADDI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 649
;647:
;648:		// the body can still be gibbed
;649:		self->die = body_die;
ADDRFP4 0
INDIRP4
CNSTI4 716
ADDP4
ADDRGP4 body_die
ASGNP4
line 652
;650:
;651:		// globally cycle through the different death animations
;652:		i = ( i + 1 ) % 3;
ADDRLP4 92
ADDRGP4 $254
ASGNP4
ADDRLP4 92
INDIRP4
ADDRLP4 92
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
CNSTI4 3
MODI4
ASGNI4
line 659
;653:
;654:#ifdef MISSIONPACK
;655:		if (self->s.eFlags & EF_KAMIKAZE) {
;656:			Kamikaze_DeathTimer( self );
;657:		}
;658:#endif
;659:	}
LABELV $249
line 661
;660:
;661:	trap_LinkEntity (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 663
;662:
;663:}
LABELV $185
endproc player_die 96 28
export bash_die
proc bash_die 808 12
line 670
;664:
;665:/*
;666:================
;667:Custom Bash Die			^^^
;668:================
;669:*/
;670:void bash_die (gentity_t *self, int attackNum, int take, gclient_t frozen, gclient_t frozenAttack) {
line 674
;671:	gclient_t *targ;
;672:	gclient_t attack;
;673:
;674:	targ = self->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 675
;675:	attack = level.clients[attackNum];
ADDRLP4 4
CNSTI4 788
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
INDIRB
ASGNB 788
line 677
;676:
;677:	targ->ps.persistant[PERS_SCORE] -= 1;
ADDRLP4 792
ADDRLP4 0
INDIRP4
CNSTI4 248
ADDP4
ASGNP4
ADDRLP4 792
INDIRP4
ADDRLP4 792
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 678
;678:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 680
;679:
;680:	G_LogPrintf( "Die: %s fell off the map.\n", targ->pers.netname );
ADDRGP4 $263
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 682
;681:
;682:	if (attackNum == self->client->ps.clientNum) {					// If player knocked himself off:
ADDRFP4 4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
NEI4 $264
line 683
;683:		if (frozen.ps.stats[STAT_ARMOR] > 150) {
ADDRFP4 12
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 150
LEI4 $266
line 684
;684:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE "'s armor was too heavy.\n\"", targ->pers.netname) );
ADDRGP4 $268
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 796
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 796
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 685
;685:		} else if (frozen.ps.powerups[PW_QUAD] ) {
ADDRGP4 $265
JUMPV
LABELV $266
ADDRFP4 12
INDIRP4
CNSTI4 316
ADDP4
INDIRI4
CNSTI4 0
EQI4 $269
line 686
;686:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " didn't know how to use the quad.\n\"", targ->pers.netname) );
ADDRGP4 $271
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 796
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 796
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 687
;687:		} else if (frozen.ps.stats[STAT_ARMOR] > 100) {
ADDRGP4 $265
JUMPV
LABELV $269
ADDRFP4 12
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 100
LEI4 $272
line 688
;688:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " was wearing TOO much armor.\n\"", targ->pers.netname) );
ADDRGP4 $274
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 796
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 796
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 689
;689:		} else if (frozen.ps.stats[STAT_HEALTH] == 1) {
ADDRGP4 $265
JUMPV
LABELV $272
ADDRFP4 12
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 1
NEI4 $275
line 690
;690:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " forgot how to fly.\n\"", targ->pers.netname) );
ADDRGP4 $277
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 796
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 796
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 691
;691:		} else if (frozen.ps.stats[STAT_HEALTH] < 25) {
ADDRGP4 $265
JUMPV
LABELV $275
ADDRFP4 12
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 25
GEI4 $278
line 692
;692:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " saw it useless to continue.\n\"", targ->pers.netname) );
ADDRGP4 $280
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 796
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 796
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 693
;693:		} else if (frozen.ps.stats[STAT_HEALTH] > 100) {
ADDRGP4 $265
JUMPV
LABELV $278
ADDRFP4 12
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 100
LEI4 $281
line 694
;694:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE "'s %d health didn't reflect his skill.\n\"", targ->pers.netname, frozen.ps.stats[STAT_HEALTH]) );
ADDRGP4 $283
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRFP4 12
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ARGI4
ADDRLP4 796
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 796
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 695
;695:		} else {
ADDRGP4 $265
JUMPV
LABELV $281
line 696
;696:            trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " couldn't handle the pressure.\n\"", targ->pers.netname) );
ADDRGP4 $284
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 796
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 796
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 697
;697:		}
line 698
;698:	} else {														// Else it has to be another player:
ADDRGP4 $265
JUMPV
LABELV $264
line 699
;699:		if (frozenAttack.ps.stats[STAT_HEALTH] > 100 && frozenAttack.ps.stats[STAT_ARMOR] > 100) {
ADDRLP4 796
ADDRFP4 16
INDIRP4
ASGNP4
ADDRLP4 800
CNSTI4 100
ASGNI4
ADDRLP4 796
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ADDRLP4 800
INDIRI4
LEI4 $285
ADDRLP4 796
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ADDRLP4 800
INDIRI4
LEI4 $285
line 700
;700:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " is kicking ass and taking %s" S_COLOR_WHITE "'s name.\n\"", attack.pers.netname, targ->pers.netname) );
ADDRGP4 $287
ARGP4
ADDRLP4 4+468+44
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 804
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 804
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 701
;701:		} else if (frozenAttack.ps.powerups[PW_QUAD] ) {
ADDRGP4 $286
JUMPV
LABELV $285
ADDRFP4 16
INDIRP4
CNSTI4 316
ADDP4
INDIRI4
CNSTI4 0
EQI4 $290
line 702
;702:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " knew how to use the quad against %s" S_COLOR_WHITE ".\n\"", attack.pers.netname, targ->pers.netname) );
ADDRGP4 $292
ARGP4
ADDRLP4 4+468+44
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 804
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 804
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 703
;703:		} else if (frozen.ps.powerups[PW_QUAD] ) {
ADDRGP4 $291
JUMPV
LABELV $290
ADDRFP4 12
INDIRP4
CNSTI4 316
ADDP4
INDIRI4
CNSTI4 0
EQI4 $295
line 704
;704:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " turned off %s" S_COLOR_WHITE "'s bright, blue light.\n\"", attack.pers.netname, targ->pers.netname) );
ADDRGP4 $297
ARGP4
ADDRLP4 4+468+44
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 804
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 804
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 705
;705:		} else if (frozen.ps.stats[STAT_ARMOR] > 100) {
ADDRGP4 $296
JUMPV
LABELV $295
ADDRFP4 12
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 100
LEI4 $300
line 706
;706:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE "'s armor was useless against %s" S_COLOR_WHITE ".\n\"", targ->pers.netname, attack.pers.netname) );
ADDRGP4 $302
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 4+468+44
ARGP4
ADDRLP4 804
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 804
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 707
;707:		} else if (frozen.ps.stats[STAT_HEALTH] == 1) {
ADDRGP4 $301
JUMPV
LABELV $300
ADDRFP4 12
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 1
NEI4 $305
line 708
;708:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " made %s" S_COLOR_WHITE " fly.\n\"", attack.pers.netname, targ->pers.netname) );
ADDRGP4 $307
ARGP4
ADDRLP4 4+468+44
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 804
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 804
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 709
;709:		} else if (frozen.ps.stats[STAT_HEALTH] < 25) {
ADDRGP4 $306
JUMPV
LABELV $305
ADDRFP4 12
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 25
GEI4 $310
line 710
;710:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " won the fire fight against %s" S_COLOR_WHITE ".\n\"", attack.pers.netname, targ->pers.netname) );
ADDRGP4 $312
ARGP4
ADDRLP4 4+468+44
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 804
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 804
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 711
;711:		} else if (frozen.ps.stats[STAT_HEALTH] > 100) {
ADDRGP4 $311
JUMPV
LABELV $310
ADDRFP4 12
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 100
LEI4 $315
line 712
;712:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " thought he had the advantage over %s" S_COLOR_WHITE ".\n\"", targ->pers.netname, attack.pers.netname) );
ADDRGP4 $317
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 4+468+44
ARGP4
ADDRLP4 804
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 804
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 713
;713:		} else {
ADDRGP4 $316
JUMPV
LABELV $315
line 714
;714:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " knocked %s" S_COLOR_WHITE " off the map.\n\"", attack.pers.netname, self->client->pers.netname) );
ADDRGP4 $320
ARGP4
ADDRLP4 4+468+44
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 804
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 804
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 715
;715:		}
LABELV $316
LABELV $311
LABELV $306
LABELV $301
LABELV $296
LABELV $291
LABELV $286
line 716
;716:	}
LABELV $265
line 718
;717:
;718:	respawn (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 respawn
CALLV
pop
line 719
;719:}
LABELV $262
endproc bash_die 808 12
export CheckArmor
proc CheckArmor 20 4
line 727
;720:
;721:/*
;722:================
;723:CheckArmor
;724:================
;725:*/
;726:int CheckArmor (gentity_t *ent, int damage, int dflags)
;727:{
line 732
;728:	gclient_t	*client;
;729:	int			save;
;730:	int			count;
;731:
;732:	if (!damage)
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $324
line 733
;733:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $323
JUMPV
LABELV $324
line 735
;734:
;735:	client = ent->client;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 737
;736:
;737:	if (!client)
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $326
line 738
;738:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $323
JUMPV
LABELV $326
line 740
;739:
;740:	if (dflags & DAMAGE_NO_ARMOR)
ADDRFP4 8
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $328
line 741
;741:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $323
JUMPV
LABELV $328
line 744
;742:
;743:	// armor
;744:	count = client->ps.stats[STAT_ARMOR];
ADDRLP4 8
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
line 745
;745:	save = ceil( damage * ARMOR_PROTECTION );
CNSTF4 1062836634
ADDRFP4 4
INDIRI4
CVIF4 4
MULF4
ARGF4
ADDRLP4 12
ADDRGP4 ceil
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 12
INDIRF4
CVFI4 4
ASGNI4
line 746
;746:	if (save >= count)
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $330
line 747
;747:		save = count;
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
LABELV $330
line 749
;748:
;749:	if (!save)
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $332
line 750
;750:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $323
JUMPV
LABELV $332
line 752
;751:
;752:	client->ps.stats[STAT_ARMOR] -= save;
ADDRLP4 16
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
ASGNI4
line 754
;753:
;754:	return save;
ADDRLP4 0
INDIRI4
RETI4
LABELV $323
endproc CheckArmor 20 4
export RaySphereIntersections
proc RaySphereIntersections 96 4
line 762
;755:}
;756:
;757:/*
;758:================
;759:RaySphereIntersections
;760:================
;761:*/
;762:int RaySphereIntersections( vec3_t origin, float radius, vec3_t point, vec3_t dir, vec3_t intersections[2] ) {
line 771
;763:	float b, c, d, t;
;764:
;765:	//	| origin - (point + t * dir) | = radius
;766:	//	a = dir[0]^2 + dir[1]^2 + dir[2]^2;
;767:	//	b = 2 * (dir[0] * (point[0] - origin[0]) + dir[1] * (point[1] - origin[1]) + dir[2] * (point[2] - origin[2]));
;768:	//	c = (point[0] - origin[0])^2 + (point[1] - origin[1])^2 + (point[2] - origin[2])^2 - radius^2;
;769:
;770:	// normalize dir so a = 1
;771:	VectorNormalize(dir);
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 772
;772:	b = 2 * (dir[0] * (point[0] - origin[0]) + dir[1] * (point[1] - origin[1]) + dir[2] * (point[2] - origin[2]));
ADDRLP4 16
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 20
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
CNSTI4 4
ASGNI4
ADDRLP4 32
CNSTI4 8
ASGNI4
ADDRLP4 4
CNSTF4 1073741824
ADDRLP4 16
INDIRP4
INDIRF4
ADDRLP4 20
INDIRP4
INDIRF4
ADDRLP4 24
INDIRP4
INDIRF4
SUBF4
MULF4
ADDRLP4 16
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRF4
ADDRLP4 20
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRF4
ADDRLP4 24
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRF4
SUBF4
MULF4
ADDF4
ADDRLP4 16
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDRLP4 20
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDRLP4 24
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
SUBF4
MULF4
ADDF4
MULF4
ASGNF4
line 773
;773:	c = (point[0] - origin[0]) * (point[0] - origin[0]) +
ADDRLP4 36
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
ADDRLP4 36
INDIRP4
INDIRF4
ADDRLP4 40
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 48
CNSTI4 4
ASGNI4
ADDRLP4 52
ADDRLP4 36
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56
CNSTI4 8
ASGNI4
ADDRLP4 60
ADDRLP4 36
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 64
ADDRFP4 4
INDIRF4
ASGNF4
ADDRLP4 12
ADDRLP4 44
INDIRF4
ADDRLP4 44
INDIRF4
MULF4
ADDRLP4 52
INDIRF4
ADDRLP4 52
INDIRF4
MULF4
ADDF4
ADDRLP4 60
INDIRF4
ADDRLP4 60
INDIRF4
MULF4
ADDF4
ADDRLP4 64
INDIRF4
ADDRLP4 64
INDIRF4
MULF4
SUBF4
ASGNF4
line 778
;774:		(point[1] - origin[1]) * (point[1] - origin[1]) +
;775:		(point[2] - origin[2]) * (point[2] - origin[2]) -
;776:		radius * radius;
;777:
;778:	d = b * b - 4 * c;
ADDRLP4 8
ADDRLP4 4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
CNSTF4 1082130432
ADDRLP4 12
INDIRF4
MULF4
SUBF4
ASGNF4
line 779
;779:	if (d > 0) {
ADDRLP4 8
INDIRF4
CNSTF4 0
LEF4 $335
line 780
;780:		t = (- b + sqrt(d)) / 2;
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 72
ADDRGP4 sqrt
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
NEGF4
ADDRLP4 72
INDIRF4
ADDF4
CNSTF4 1073741824
DIVF4
ASGNF4
line 781
;781:		VectorMA(point, t, dir, intersections[0]);
ADDRFP4 16
INDIRP4
ADDRFP4 8
INDIRP4
INDIRF4
ADDRFP4 12
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 76
CNSTI4 4
ASGNI4
ADDRFP4 16
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 80
CNSTI4 8
ASGNI4
ADDRFP4 16
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 782
;782:		t = (- b - sqrt(d)) / 2;
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 84
ADDRGP4 sqrt
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
NEGF4
ADDRLP4 84
INDIRF4
SUBF4
CNSTF4 1073741824
DIVF4
ASGNF4
line 783
;783:		VectorMA(point, t, dir, intersections[1]);
ADDRFP4 16
INDIRP4
CNSTI4 12
ADDP4
ADDRFP4 8
INDIRP4
INDIRF4
ADDRFP4 12
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 88
CNSTI4 4
ASGNI4
ADDRFP4 16
INDIRP4
CNSTI4 16
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 88
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 88
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 92
CNSTI4 8
ASGNI4
ADDRFP4 16
INDIRP4
CNSTI4 20
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 92
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 92
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 784
;784:		return 2;
CNSTI4 2
RETI4
ADDRGP4 $334
JUMPV
LABELV $335
line 786
;785:	}
;786:	else if (d == 0) {
ADDRLP4 8
INDIRF4
CNSTF4 0
NEF4 $337
line 787
;787:		t = (- b ) / 2;
ADDRLP4 0
ADDRLP4 4
INDIRF4
NEGF4
CNSTF4 1073741824
DIVF4
ASGNF4
line 788
;788:		VectorMA(point, t, dir, intersections[0]);
ADDRFP4 16
INDIRP4
ADDRFP4 8
INDIRP4
INDIRF4
ADDRFP4 12
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 72
CNSTI4 4
ASGNI4
ADDRFP4 16
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 76
CNSTI4 8
ASGNI4
ADDRFP4 16
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 789
;789:		return 1;
CNSTI4 1
RETI4
ADDRGP4 $334
JUMPV
LABELV $337
line 791
;790:	}
;791:	return 0;
CNSTI4 0
RETI4
LABELV $334
endproc RaySphereIntersections 96 4
export G_Damage
proc G_Damage 4028 24
line 858
;792:}
;793:
;794:#ifdef MISSIONPACK
;795:/*
;796:================
;797:G_InvulnerabilityEffect
;798:================
;799:*/
;800:int G_InvulnerabilityEffect( gentity_t *targ, vec3_t dir, vec3_t point, vec3_t impactpoint, vec3_t bouncedir ) {
;801:	gentity_t	*impact;
;802:	vec3_t		intersections[2], vec;
;803:	int			n;
;804:
;805:	if ( !targ->client ) {
;806:		return qfalse;
;807:	}
;808:	VectorCopy(dir, vec);
;809:	VectorInverse(vec);
;810:	// sphere model radius = 42 units
;811:	n = RaySphereIntersections( targ->client->ps.origin, 42, point, vec, intersections);
;812:	if (n > 0) {
;813:		impact = G_TempEntity( targ->client->ps.origin, EV_INVUL_IMPACT );
;814:		VectorSubtract(intersections[0], targ->client->ps.origin, vec);
;815:		vectoangles(vec, impact->s.angles);
;816:		impact->s.angles[0] += 90;
;817:		if (impact->s.angles[0] > 360)
;818:			impact->s.angles[0] -= 360;
;819:		if ( impactpoint ) {
;820:			VectorCopy( intersections[0], impactpoint );
;821:		}
;822:		if ( bouncedir ) {
;823:			VectorCopy( vec, bouncedir );
;824:			VectorNormalize( bouncedir );
;825:		}
;826:		return qtrue;
;827:	}
;828:	else {
;829:		return qfalse;
;830:	}
;831:}
;832:#endif
;833:/*
;834:============
;835:T_Damage
;836:
;837:targ		entity that is being damaged
;838:inflictor	entity that is causing the damage
;839:attacker	entity that caused the inflictor to damage targ
;840:	example: targ=monster, inflictor=rocket, attacker=player
;841:
;842:dir			direction of the attack for knockback
;843:point		point at which the damage is being inflicted, used for headshots
;844:damage		amount of damage being inflicted
;845:knockback	force to be applied against targ as a result of the damage
;846:
;847:inflictor, attacker, dir, and point can be NULL for environmental effects
;848:
;849:dflags		these flags are used to control how T_Damage works
;850:	DAMAGE_RADIUS			damage was indirect (from a nearby explosion)
;851:	DAMAGE_NO_ARMOR			armor does not protect from this damage
;852:	DAMAGE_NO_KNOCKBACK		do not affect velocity, just view angles
;853:	DAMAGE_NO_PROTECTION	kills godmode, armor, everything
;854:============
;855:*/
;856:
;857:void G_Damage( gentity_t *targ, gentity_t *inflictor, gentity_t *attacker,
;858:			   vec3_t dir, vec3_t point, int damage, int dflags, int mod ) {
line 877
;859:	gclient_t	*client;
;860:	gclient_t	frozenClient;
;861:	gclient_t	humanAttacker;
;862:	gclient_t	frozenAttacker;
;863:	int			attackNum;
;864:	int			lastHurtNum;
;865:	int			take;
;866:	int			save;
;867:	int			asave;
;868:	int			knockback;
;869:	int			max;
;870:	int			i;
;871:	int			prevhealth;
;872:	float			knockbackTrue;
;873:#ifdef MISSIONPACK
;874:	vec3_t		bouncedir, impactpoint;
;875:#endif
;876:
;877:	if (!targ->takedamage) {
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
INDIRI4
CNSTI4 0
NEI4 $340
line 878
;878:		return;
ADDRGP4 $339
JUMPV
LABELV $340
line 883
;879:	}
;880:
;881:	// the intermission has allready been qualified for, so don't
;882:	// allow any extra scoring
;883:	if ( level.intermissionQueued ) {
ADDRGP4 level+9128
INDIRI4
CNSTI4 0
EQI4 $342
line 884
;884:		return;
ADDRGP4 $339
JUMPV
LABELV $342
line 897
;885:	}
;886:#ifdef MISSIONPACK
;887:	if ( targ->client && mod != MOD_JUICED) {
;888:		if ( targ->client->invulnerabilityTime > level.time) {
;889:			if ( dir && point ) {
;890:				G_InvulnerabilityEffect( targ, dir, point, impactpoint, bouncedir );
;891:			}
;892:			return;
;893:		}
;894:	}
;895:#endif
;896:
;897:	if ( !inflictor ) {
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $345
line 898
;898:		inflictor = &g_entities[ENTITYNUM_WORLD];
ADDRFP4 4
ADDRGP4 g_entities+825776
ASGNP4
line 899
;899:	}
LABELV $345
line 900
;900:	if ( !attacker ) {
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $348
line 901
;901:		attacker = &g_entities[ENTITYNUM_WORLD];
ADDRFP4 8
ADDRGP4 g_entities+825776
ASGNP4
line 902
;902:	}
LABELV $348
line 905
;903:
;904:	// shootable doors / buttons don't actually have any health
;905:	if ( targ->s.eType == ET_MOVER ) {
ADDRLP4 2408
CNSTI4 4
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 2408
INDIRI4
ADDP4
INDIRI4
ADDRLP4 2408
INDIRI4
NEI4 $351
line 906
;906:		if ( targ->use && targ->moverState == MOVER_POS1 ) {
ADDRLP4 2412
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2412
INDIRP4
CNSTI4 708
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $339
ADDRLP4 2412
INDIRP4
CNSTI4 576
ADDP4
INDIRI4
CNSTI4 0
NEI4 $339
line 907
;907:			targ->use( targ, inflictor, attacker );
ADDRLP4 2416
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2416
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 2416
INDIRP4
CNSTI4 708
ADDP4
INDIRP4
CALLV
pop
line 908
;908:		}
line 909
;909:		return;
ADDRGP4 $339
JUMPV
LABELV $351
line 918
;910:	}
;911:#ifdef MISSIONPACK
;912:	if( g_gametype.integer == GT_OBELISK && CheckObeliskAttack( targ, attacker ) ) {
;913:		return;
;914:	}
;915:#endif
;916:	// reduce damage by the attacker's handicap value
;917:	// unless they are rocket jumping
;918:	if ( attacker->client && attacker != targ ) {
ADDRLP4 2412
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 2412
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $355
ADDRLP4 2412
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $355
line 919
;919:		max = attacker->client->ps.stats[STAT_MAX_HEALTH];
ADDRLP4 1600
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ASGNI4
line 925
;920:#ifdef MISSIONPACK
;921:		if( bg_itemlist[attacker->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD ) {
;922:			max /= 2;
;923:		}
;924:#endif
;925:		damage = damage * max / 100;
ADDRFP4 20
ADDRFP4 20
INDIRI4
ADDRLP4 1600
INDIRI4
MULI4
CNSTI4 100
DIVI4
ASGNI4
line 926
;926:	}
LABELV $355
line 928
;927:
;928:	client = targ->client;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 930
;929:
;930:	if ( client ) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $357
line 931
;931:		if ( client->noclip ) {
ADDRLP4 4
INDIRP4
CNSTI4 648
ADDP4
INDIRI4
CNSTI4 0
EQI4 $359
line 932
;932:			return;
ADDRGP4 $339
JUMPV
LABELV $359
line 934
;933:		}
;934:	}
LABELV $357
line 936
;935:
;936:	if ( !dir ) {
ADDRFP4 12
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $361
line 937
;937:		dflags |= DAMAGE_NO_KNOCKBACK;
ADDRFP4 24
ADDRFP4 24
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 938
;938:	} else {
ADDRGP4 $362
JUMPV
LABELV $361
line 939
;939:		VectorNormalize(dir);
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 940
;940:	}
LABELV $362
line 942
;941:
;942:	knockback = damage;
ADDRLP4 8
ADDRFP4 20
INDIRI4
ASGNI4
line 949
;943:
;944:	//Get rid of this... ^^^
;945://	if ( knockback > 200 ) {
;946://		knockback = 200;
;947://	}
;948:
;949:	if ( targ->flags & FL_NO_KNOCKBACK ) {
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $363
line 950
;950:		knockback = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 951
;951:	}
LABELV $363
line 952
;952:	if ( dflags & DAMAGE_NO_KNOCKBACK ) {
ADDRFP4 24
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $365
line 953
;953:		knockback = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 954
;954:	}
LABELV $365
line 957
;955:
;956:	// figure momentum add, even if the damage won't be taken
;957:	if ( knockback && targ->client ) {
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $367
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $367
line 965
;958:		vec3_t	kvel;
;959:		float	mass;
;960:
;961:		// ex. o Armor = normal knockback
;962:		//	 100 Armor = Quarter Knockback
;963:		//   200 Armor = Half Knockback
;964:		//	equation: y = -.0025(X) + 1
;965:		knockbackTrue = knockback;
ADDRLP4 1604
ADDRLP4 8
INDIRI4
CVIF4 4
ASGNF4
line 966
;966:		if (targ != attacker) {			
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRFP4 8
INDIRP4
CVPU4 4
EQU4 $369
line 967
;967:			knockback = damage * (-.0025 * targ->client->ps.stats[STAT_ARMOR] + 1);	//^^^ Knockback value is affected by armor
ADDRLP4 8
ADDRFP4 20
INDIRI4
CVIF4 4
CNSTF4 3139688202
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CVIF4 4
MULF4
CNSTF4 1065353216
ADDF4
MULF4
CVFI4 4
ASGNI4
line 968
;968:		} else {
ADDRGP4 $370
JUMPV
LABELV $369
line 969
;969:			knockback = damage;
ADDRLP4 8
ADDRFP4 20
INDIRI4
ASGNI4
line 970
;970:		}
LABELV $370
line 972
;971:
;972:		mass = 200;
ADDRLP4 2428
CNSTF4 1128792064
ASGNF4
line 974
;973:
;974:		VectorScale (dir, g_knockback.value * (float)knockback / mass, kvel);
ADDRLP4 2432
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 2436
ADDRLP4 8
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 2440
ADDRLP4 2428
INDIRF4
ASGNF4
ADDRLP4 2416
ADDRLP4 2432
INDIRP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 2436
INDIRF4
MULF4
ADDRLP4 2440
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 2416+4
ADDRLP4 2432
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 2436
INDIRF4
MULF4
ADDRLP4 2440
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 2416+8
ADDRFP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 8
INDIRI4
CVIF4 4
MULF4
ADDRLP4 2428
INDIRF4
DIVF4
MULF4
ASGNF4
line 975
;975:		VectorAdd (targ->client->ps.velocity, kvel, targ->client->ps.velocity);
ADDRLP4 2444
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 2444
INDIRP4
ADDRLP4 2444
INDIRP4
INDIRF4
ADDRLP4 2416
INDIRF4
ADDF4
ASGNF4
ADDRLP4 2448
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 2448
INDIRP4
ADDRLP4 2448
INDIRP4
INDIRF4
ADDRLP4 2416+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 2452
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 2452
INDIRP4
ADDRLP4 2452
INDIRP4
INDIRF4
ADDRLP4 2416+8
INDIRF4
ADDF4
ASGNF4
line 979
;976:
;977:		// set the timer so that the other client can't cancel
;978:		// out the movement immediately
;979:		if ( !targ->client->ps.pm_time ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 0
NEI4 $378
line 982
;980:			int		t;
;981:
;982:			t = knockback * 2;
ADDRLP4 2456
ADDRLP4 8
INDIRI4
CNSTI4 1
LSHI4
ASGNI4
line 983
;983:			if ( t < 50 ) {
ADDRLP4 2456
INDIRI4
CNSTI4 50
GEI4 $380
line 984
;984:				t = 50;
ADDRLP4 2456
CNSTI4 50
ASGNI4
line 985
;985:			}
LABELV $380
line 986
;986:			if ( t > 200 ) {
ADDRLP4 2456
INDIRI4
CNSTI4 200
LEI4 $382
line 987
;987:				t = 200;
ADDRLP4 2456
CNSTI4 200
ASGNI4
line 988
;988:			}
LABELV $382
line 989
;989:			targ->client->ps.pm_time = t;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 16
ADDP4
ADDRLP4 2456
INDIRI4
ASGNI4
line 990
;990:			targ->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
ADDRLP4 2460
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 2460
INDIRP4
ADDRLP4 2460
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 991
;991:		}
LABELV $378
line 992
;992:	}
LABELV $367
line 995
;993:
;994:	// check for completely getting out of the damage
;995:	if ( !(dflags & DAMAGE_NO_PROTECTION) ) {
ADDRFP4 24
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $384
line 1002
;996:
;997:		// if TF_NO_FRIENDLY_FIRE is set, don't do damage to the target
;998:		// if the attacker was on the same team
;999:#ifdef MISSIONPACK
;1000:		if ( mod != MOD_JUICED && targ != attacker && !(dflags & DAMAGE_NO_TEAM_PROTECTION) && OnSameTeam (targ, attacker)  ) {
;1001:#else	
;1002:		if ( targ != attacker && OnSameTeam (targ, attacker)  ) {
ADDRLP4 2416
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2420
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 2416
INDIRP4
CVPU4 4
ADDRLP4 2420
INDIRP4
CVPU4 4
EQU4 $386
ADDRLP4 2416
INDIRP4
ARGP4
ADDRLP4 2420
INDIRP4
ARGP4
ADDRLP4 2424
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 2424
INDIRI4
CNSTI4 0
EQI4 $386
line 1004
;1003:#endif
;1004:			if ( !g_friendlyFire.integer ) {
ADDRGP4 g_friendlyFire+12
INDIRI4
CNSTI4 0
NEI4 $388
line 1005
;1005:				return;
ADDRGP4 $339
JUMPV
LABELV $388
line 1007
;1006:			}
;1007:		}
LABELV $386
line 1020
;1008:#ifdef MISSIONPACK
;1009:		if (mod == MOD_PROXIMITY_MINE) {
;1010:			if (inflictor && inflictor->parent && OnSameTeam(targ, inflictor->parent)) {
;1011:				return;
;1012:			}
;1013:			if (targ == attacker) {
;1014:				return;
;1015:			}
;1016:		}
;1017:#endif
;1018:
;1019:		// check for godmode
;1020:		if ( targ->flags & FL_GODMODE ) {
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $391
line 1021
;1021:			return;
ADDRGP4 $339
JUMPV
LABELV $391
line 1023
;1022:		}
;1023:	}
LABELV $384
line 1027
;1024:
;1025:	// battlesuit protects from all radius damage (but takes knockback)
;1026:	// and protects 50% against all damage
;1027:	if ( client && client->ps.powerups[PW_BATTLESUIT] ) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $393
ADDRLP4 4
INDIRP4
CNSTI4 320
ADDP4
INDIRI4
CNSTI4 0
EQI4 $393
line 1028
;1028:		G_AddEvent( targ, EV_POWERUP_BATTLESUIT, 0 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 62
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 1029
;1029:		if ( ( dflags & DAMAGE_RADIUS ) || ( mod == MOD_FALLING ) ) {
ADDRFP4 24
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $397
ADDRFP4 28
INDIRI4
CNSTI4 19
NEI4 $395
LABELV $397
line 1030
;1030:			return;
ADDRGP4 $339
JUMPV
LABELV $395
line 1032
;1031:		}
;1032:		damage *= 0.5;
ADDRFP4 20
CNSTF4 1056964608
ADDRFP4 20
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 1033
;1033:	}
LABELV $393
line 1036
;1034:
;1035:	// add to the attacker's hit counter (if the target isn't a general entity like a prox mine)
;1036:	if ( attacker->client && targ != attacker && targ->health > 0
ADDRLP4 2420
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 2420
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $398
ADDRLP4 2424
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2424
INDIRP4
CVPU4 4
ADDRLP4 2420
INDIRP4
CVPU4 4
EQU4 $398
ADDRLP4 2428
CNSTI4 0
ASGNI4
ADDRLP4 2424
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 2428
INDIRI4
LEI4 $398
ADDRLP4 2432
ADDRLP4 2424
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 2432
INDIRI4
CNSTI4 3
EQI4 $398
ADDRLP4 2432
INDIRI4
ADDRLP4 2428
INDIRI4
EQI4 $398
line 1038
;1037:			&& targ->s.eType != ET_MISSILE
;1038:			&& targ->s.eType != ET_GENERAL) {
line 1039
;1039:		if ( OnSameTeam( targ, attacker ) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 2436
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 2436
INDIRI4
CNSTI4 0
EQI4 $400
line 1040
;1040:			attacker->client->ps.persistant[PERS_HITS]--;
ADDRLP4 2440
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 252
ADDP4
ASGNP4
ADDRLP4 2440
INDIRP4
ADDRLP4 2440
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1041
;1041:		} else {
ADDRGP4 $401
JUMPV
LABELV $400
line 1042
;1042:			attacker->client->ps.persistant[PERS_HITS]++;
ADDRLP4 2440
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 252
ADDP4
ASGNP4
ADDRLP4 2440
INDIRP4
ADDRLP4 2440
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1043
;1043:		}
LABELV $401
line 1044
;1044:		attacker->client->ps.persistant[PERS_ATTACKEE_ARMOR] = (targ->health<<8)|(client->ps.stats[STAT_ARMOR]);
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 276
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 8
LSHI4
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
BORI4
ASGNI4
line 1045
;1045:	}
LABELV $398
line 1049
;1046:
;1047:	// always give half damage if hurting self
;1048:	// calculated after knockback, so rocket jumping works
;1049:	if ( targ == attacker) {
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRFP4 8
INDIRP4
CVPU4 4
NEU4 $402
line 1050
;1050:		damage = 0;			//^^^ Changed 0.5 to 0.0 for no self-damage
ADDRFP4 20
CNSTI4 0
ASGNI4
line 1051
;1051:	}
LABELV $402
line 1053
;1052:
;1053:	if ( damage < 1 ) {
ADDRFP4 20
INDIRI4
CNSTI4 1
GEI4 $404
line 1054
;1054:		damage = 1;
ADDRFP4 20
CNSTI4 1
ASGNI4
line 1055
;1055:	}
LABELV $404
line 1056
;1056:	take = damage/4;			//^^^ Divide damage by 4
ADDRLP4 12
ADDRFP4 20
INDIRI4
CNSTI4 4
DIVI4
ASGNI4
line 1057
;1057:	save = 0;
ADDRLP4 1596
CNSTI4 0
ASGNI4
line 1060
;1058:
;1059:	// save some from armor
;1060:	asave = CheckArmor (targ, take, dflags);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
INDIRI4
ARGI4
ADDRFP4 24
INDIRI4
ARGI4
ADDRLP4 2436
ADDRGP4 CheckArmor
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 2436
INDIRI4
ASGNI4
line 1061
;1061:	take -= asave;
ADDRLP4 12
ADDRLP4 12
INDIRI4
ADDRLP4 16
INDIRI4
SUBI4
ASGNI4
line 1063
;1062:
;1063:	if ( g_debugDamage.integer ) {
ADDRGP4 g_debugDamage+12
INDIRI4
CNSTI4 0
EQI4 $406
line 1064
;1064:		G_Printf( "%i: client:%i health:%i damage:%i armor:%i\n", level.time, targ->s.number,
ADDRGP4 $409
ARGP4
ADDRGP4 level+36
INDIRI4
ARGI4
ADDRLP4 2440
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2440
INDIRP4
INDIRI4
ARGI4
ADDRLP4 2440
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
INDIRI4
ARGI4
ADDRLP4 16
INDIRI4
ARGI4
ADDRGP4 G_Printf
CALLV
pop
line 1066
;1065:			targ->health, take, asave );
;1066:	}
LABELV $406
line 1071
;1067:
;1068:	// add to the damage inflicted on a player this frame
;1069:	// the total will be turned into screen blends and view angle kicks
;1070:	// at the end of the frame
;1071:	if ( client ) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $411
line 1072
;1072:		if ( attacker ) {
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $413
line 1073
;1073:			client->ps.persistant[PERS_ATTACKER] = attacker->s.number;
ADDRLP4 4
INDIRP4
CNSTI4 272
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 1074
;1074:		} else {
ADDRGP4 $414
JUMPV
LABELV $413
line 1075
;1075:			client->ps.persistant[PERS_ATTACKER] = ENTITYNUM_WORLD;
ADDRLP4 4
INDIRP4
CNSTI4 272
ADDP4
CNSTI4 1022
ASGNI4
line 1076
;1076:		}
LABELV $414
line 1077
;1077:		client->damage_armor += asave;
ADDRLP4 2440
ADDRLP4 4
INDIRP4
CNSTI4 680
ADDP4
ASGNP4
ADDRLP4 2440
INDIRP4
ADDRLP4 2440
INDIRP4
INDIRI4
ADDRLP4 16
INDIRI4
ADDI4
ASGNI4
line 1078
;1078:		client->damage_blood += take;
ADDRLP4 2444
ADDRLP4 4
INDIRP4
CNSTI4 684
ADDP4
ASGNP4
ADDRLP4 2444
INDIRP4
ADDRLP4 2444
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
ADDI4
ASGNI4
line 1079
;1079:		client->damage_knockback += knockback;
ADDRLP4 2448
ADDRLP4 4
INDIRP4
CNSTI4 688
ADDP4
ASGNP4
ADDRLP4 2448
INDIRP4
ADDRLP4 2448
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
ADDI4
ASGNI4
line 1080
;1080:		if ( dir ) {
ADDRFP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $415
line 1081
;1081:			VectorCopy ( dir, client->damage_from );
ADDRLP4 4
INDIRP4
CNSTI4 692
ADDP4
ADDRFP4 12
INDIRP4
INDIRB
ASGNB 12
line 1082
;1082:			client->damage_fromWorld = qfalse;
ADDRLP4 4
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 0
ASGNI4
line 1083
;1083:		} else {
ADDRGP4 $416
JUMPV
LABELV $415
line 1084
;1084:			VectorCopy ( targ->r.currentOrigin, client->damage_from );
ADDRLP4 4
INDIRP4
CNSTI4 692
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
INDIRB
ASGNB 12
line 1085
;1085:			client->damage_fromWorld = qtrue;
ADDRLP4 4
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 1
ASGNI4
line 1086
;1086:		}
LABELV $416
line 1087
;1087:	}
LABELV $411
line 1093
;1088:
;1089:	// See if it's the player hurting the emeny flag carrier
;1090:#ifdef MISSIONPACK
;1091:	if( g_gametype.integer == GT_CTF || g_gametype.integer == GT_1FCTF ) {
;1092:#else	
;1093:	if( g_gametype.integer == GT_CTF) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 4
NEI4 $417
line 1095
;1094:#endif
;1095:		Team_CheckHurtCarrier(targ, attacker);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Team_CheckHurtCarrier
CALLV
pop
line 1096
;1096:	}
LABELV $417
line 1099
;1097:	
;1098:
;1099:	if (targ->client) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $420
line 1101
;1100:		// set the last client who damaged the target
;1101:		if ( attacker->s.number < 0 || attacker->s.number >= MAX_CLIENTS ){
ADDRLP4 2440
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 2440
INDIRI4
CNSTI4 0
LTI4 $424
ADDRLP4 2440
INDIRI4
CNSTI4 64
LTI4 $422
LABELV $424
line 1102
;1102:			targ->client->lasthurt_client = attacker->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 724
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 1103
;1103:			targ->client->lasthurt_mod = mod;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 732
ADDP4
ADDRFP4 28
INDIRI4
ASGNI4
line 1105
;1104:		//	trap_SendServerCommand( client->ps.clientNum, va("print \"Not storing Human variables.\n\"") );
;1105:		} else {
ADDRGP4 $423
JUMPV
LABELV $422
line 1106
;1106:			if ( attacker->s.number != targ->s.number ) {
ADDRFP4 8
INDIRP4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
EQI4 $425
line 1107
;1107:				targ->client->lasthurt_other = attacker->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 736
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 1108
;1108:			}
LABELV $425
line 1109
;1109:			targ->client->lasthurt_client = attacker->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 724
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 1110
;1110:			targ->client->lasthurt_humclient = attacker->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 728
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 1111
;1111:			targ->client->lasthurt_mod = mod;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 732
ADDP4
ADDRFP4 28
INDIRI4
ASGNI4
line 1112
;1112:		}
LABELV $423
line 1113
;1113:	}
LABELV $420
line 1115
;1114:
;1115:	frozenClient = level.clients[targ->client->ps.clientNum];	// ^^^^^^
ADDRLP4 20
CNSTI4 788
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
INDIRB
ASGNB 788
line 1116
;1116:	frozenAttacker = level.clients[targ->client->lasthurt_humclient];
ADDRLP4 808
CNSTI4 788
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 728
ADDP4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
INDIRB
ASGNB 788
line 1119
;1117:
;1118:	//Give 50 ammo to the player
;1119:	for (i = 0; i < MAX_WEAPONS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $427
line 1120
;1120:		client->ps.ammo[i] = 50;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
INDIRP4
CNSTI4 376
ADDP4
ADDP4
CNSTI4 50
ASGNI4
line 1121
;1121:	}
LABELV $428
line 1119
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $427
line 1124
;1122:
;1123:	// do the damage
;1124:	if (take) {	
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $431
line 1126
;1125:	//		client->ps.persistant[PERS_SCORE] = 0;								// Debugging Code
;1126:		if ( attacker->s.number < 0 || attacker->s.number >= MAX_CLIENTS ){		// ^^^ If the attacker does not have a identifier number equal to a player's
ADDRLP4 2440
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 2440
INDIRI4
CNSTI4 0
LTI4 $435
ADDRLP4 2440
INDIRI4
CNSTI4 64
LTI4 $433
LABELV $435
line 1127
;1127:			prevhealth = targ->client->ps.stats[STAT_HEALTH];
ADDRLP4 1608
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ASGNI4
line 1128
;1128:			targ->health = targ->health - take;										//  Then it must be the world.
ADDRLP4 2444
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ASGNP4
ADDRLP4 2444
INDIRP4
ADDRLP4 2444
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
ASGNI4
line 1129
;1129:			targ->client->ps.stats[STAT_HEALTH] = targ->health;						// Since the world is allowed to kill, deal all damage.
ADDRLP4 2448
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2448
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 184
ADDP4
ADDRLP4 2448
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ASGNI4
line 1130
;1130:		} else if (targ->health - take <= 1) {									// But if it's not the world, see if the resulting damage will be lower
ADDRGP4 $434
JUMPV
LABELV $433
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
CNSTI4 1
GTI4 $436
line 1131
;1131:			targ->health = 1;													// then 1, and if so, set the target's health at 1.
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
CNSTI4 1
ASGNI4
line 1132
;1132:			targ->client->ps.stats[STAT_HEALTH] = targ->health;					// This line set's the target's health stats to the designated integer
ADDRLP4 2444
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2444
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 184
ADDP4
ADDRLP4 2444
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ASGNI4
line 1133
;1133:		} else {																// For everything else, deal damage.
ADDRGP4 $437
JUMPV
LABELV $436
line 1134
;1134:			targ->health = targ->health - take;
ADDRLP4 2444
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ASGNP4
ADDRLP4 2444
INDIRP4
ADDRLP4 2444
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
ASGNI4
line 1135
;1135:			targ->client->ps.stats[STAT_HEALTH] += targ->health;
ADDRLP4 2448
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2452
ADDRLP4 2448
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 184
ADDP4
ASGNP4
ADDRLP4 2452
INDIRP4
ADDRLP4 2452
INDIRP4
INDIRI4
ADDRLP4 2448
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1136
;1136:		}																		// End Custom Addition ^^^
LABELV $437
LABELV $434
line 1139
;1137:
;1138:	
;1139:		if (client->ps.persistant[PERS_SCORE] <= 0) {						//^^^
ADDRLP4 4
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
CNSTI4 0
GTI4 $438
line 1140
;1140:			if (targ->health < 1) {									//If player should die
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 1
GEI4 $440
line 1141
;1141:				targ->health = 1;									//Set health to 1
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
CNSTI4 1
ASGNI4
line 1142
;1142:				client->ps.persistant[PERS_SCORE] = -1;				//Set jailed player's score to -1
ADDRLP4 4
INDIRP4
CNSTI4 248
ADDP4
CNSTI4 -1
ASGNI4
line 1143
;1143:				targ->takedamage = qfalse;							//No longer take damage
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
CNSTI4 0
ASGNI4
line 1144
;1144:				client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_DENIEDREWARD;
ADDRLP4 2444
ADDRLP4 4
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 2444
INDIRP4
ADDRLP4 2444
INDIRP4
INDIRI4
CNSTI4 1
BXORI4
ASGNI4
line 1145
;1145:				targ->client->ps.stats[STAT_HEALTH] = targ->health;
ADDRLP4 2448
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2448
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 184
ADDP4
ADDRLP4 2448
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ASGNI4
line 1146
;1146:				trap_SendServerCommand( client->ps.clientNum, va("print \"^3You are now in jail!\n\"", client->pers.netname) );
ADDRGP4 $442
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 2452
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 2452
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1148
;1147:
;1148:				G_LogPrintf( "Lose: %s is now in jail.\n", client->pers.netname );
ADDRGP4 $443
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 1150
;1149:
;1150:				if (client->lasthurt_humclient == client->ps.clientNum) {
ADDRLP4 4
INDIRP4
CNSTI4 728
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
NEI4 $444
line 1151
;1151:					trap_SendServerCommand( client->ps.clientNum, va("print \"You knocked yourself off the map!\n\"") );
ADDRGP4 $446
ARGP4
ADDRLP4 2460
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 2460
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1152
;1152:					trap_SendServerCommand( -1, va("cp \"%s" S_COLOR_WHITE " slipped into the abyss!\n\"", client->pers.netname) );
ADDRGP4 $447
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 2464
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 2464
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1153
;1153:				} else {
ADDRGP4 $445
JUMPV
LABELV $444
line 1154
;1154:					trap_SendServerCommand( -1, va("cp \"%s" S_COLOR_WHITE " knocked %s" S_COLOR_WHITE " out!\n\"", level.clients[targ->client->lasthurt_humclient].pers.netname, client->pers.netname) );
ADDRGP4 $448
ARGP4
ADDRLP4 2460
CNSTI4 512
ASGNI4
CNSTI4 788
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 728
ADDP4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ADDRLP4 2460
INDIRI4
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
ADDRLP4 2460
INDIRI4
ADDP4
ARGP4
ADDRLP4 2464
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 2464
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1155
;1155:					trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " sent %s" S_COLOR_WHITE " to the big, black void.\n\"", level.clients[targ->client->lasthurt_humclient].pers.netname, client->pers.netname) );
ADDRGP4 $449
ARGP4
ADDRLP4 2468
CNSTI4 512
ASGNI4
CNSTI4 788
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 728
ADDP4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ADDRLP4 2468
INDIRI4
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
ADDRLP4 2468
INDIRI4
ADDP4
ARGP4
ADDRLP4 2472
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 2472
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1156
;1156:					level.clients[targ->client->lasthurt_humclient].ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
ADDRLP4 2476
CNSTI4 788
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 728
ADDP4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 2476
INDIRP4
ADDRLP4 2476
INDIRP4
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 1157
;1157:					trap_SendServerCommand( client->ps.clientNum, va("print \"%s" S_COLOR_WHITE " knocked you out!\n\"", level.clients[targ->client->lasthurt_humclient].pers.netname, client->pers.netname) );
ADDRGP4 $450
ARGP4
ADDRLP4 2480
CNSTI4 512
ASGNI4
CNSTI4 788
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 728
ADDP4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ADDRLP4 2480
INDIRI4
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
ADDRLP4 2480
INDIRI4
ADDP4
ARGP4
ADDRLP4 2484
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 2484
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1158
;1158:			}
LABELV $445
line 1159
;1159:		}
LABELV $440
line 1160
;1160:	}
LABELV $438
line 1162
;1161:
;1162:	if ( targ->health <= 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 0
GTI4 $451
line 1163
;1163:		client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_GAUNTLETREWARD;
ADDRLP4 2444
ADDRLP4 4
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 2444
INDIRP4
ADDRLP4 2444
INDIRP4
INDIRI4
CNSTI4 2
BXORI4
ASGNI4
line 1164
;1164:		if ( client )
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $453
line 1165
;1165:			targ->flags |= FL_NO_KNOCKBACK;
ADDRLP4 2448
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ASGNP4
ADDRLP4 2448
INDIRP4
ADDRLP4 2448
INDIRP4
INDIRI4
CNSTI4 2048
BORI4
ASGNI4
LABELV $453
line 1166
;1166:		if (targ->health < -999)
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 -999
GEI4 $455
line 1167
;1167:				targ->health = -999;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
CNSTI4 -999
ASGNI4
LABELV $455
line 1168
;1168:		targ->enemy = attacker;
ADDRFP4 0
INDIRP4
CNSTI4 768
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 1169
;1169:		bash_die (targ, client->lasthurt_humclient, take, frozenClient, frozenAttacker);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 728
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
INDIRI4
ARGI4
ADDRLP4 2452
ADDRLP4 20
INDIRB
ASGNB 788
ADDRLP4 2452
ARGP4
ADDRLP4 3240
ADDRLP4 808
INDIRB
ASGNB 788
ADDRLP4 3240
ARGP4
ADDRGP4 bash_die
CALLV
pop
line 1170
;1170:		return;
ADDRGP4 $339
JUMPV
LABELV $451
line 1171
;1171:		} else if ( targ->pain ) {
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $457
line 1172
;1172:			targ->pain (targ, attacker, take);
ADDRLP4 2444
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2444
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 12
INDIRI4
ARGI4
ADDRLP4 2444
INDIRP4
CNSTI4 712
ADDP4
INDIRP4
CALLV
pop
line 1173
;1173:		}
LABELV $457
line 1174
;1174:	}
LABELV $431
line 1175
;1175:}
LABELV $339
endproc G_Damage 4028 24
export CanDamage
proc CanDamage 112 28
line 1186
;1176:
;1177:
;1178:/*
;1179:============
;1180:CanDamage
;1181:
;1182:Returns qtrue if the inflictor can directly damage the target.  Used for
;1183:explosions and melee attacks.
;1184:============
;1185:*/
;1186:qboolean CanDamage (gentity_t *targ, vec3_t origin) {
line 1193
;1187:	vec3_t	dest;
;1188:	trace_t	tr;
;1189:	vec3_t	midpoint;
;1190:
;1191:	// use the midpoint of the bounds instead of the origin, because
;1192:	// bmodels may have their origin is 0,0,0
;1193:	VectorAdd (targ->r.absmin, targ->r.absmax, midpoint);
ADDRLP4 80
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 80
INDIRP4
CNSTI4 464
ADDP4
INDIRF4
ADDRLP4 80
INDIRP4
CNSTI4 476
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 80
INDIRP4
CNSTI4 468
ADDP4
INDIRF4
ADDRLP4 80
INDIRP4
CNSTI4 480
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 84
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12+8
ADDRLP4 84
INDIRP4
CNSTI4 472
ADDP4
INDIRF4
ADDRLP4 84
INDIRP4
CNSTI4 484
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1194
;1194:	VectorScale (midpoint, 0.5, midpoint);
ADDRLP4 88
CNSTF4 1056964608
ASGNF4
ADDRLP4 12
ADDRLP4 88
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 88
INDIRF4
ADDRLP4 12+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 12+8
CNSTF4 1056964608
ADDRLP4 12+8
INDIRF4
MULF4
ASGNF4
line 1196
;1195:
;1196:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 1197
;1197:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 92
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 92
INDIRP4
ARGP4
ADDRLP4 92
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1198
;1198:	if (tr.fraction == 1.0 || tr.entityNum == targ->s.number)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
EQF4 $470
ADDRLP4 24+52
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $466
LABELV $470
line 1199
;1199:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $459
JUMPV
LABELV $466
line 1203
;1200:
;1201:	// this should probably check in the plane of projection, 
;1202:	// rather than in world coordinate, and also include Z
;1203:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 1204
;1204:	dest[0] += 15.0;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 1205
;1205:	dest[1] += 15.0;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 1206
;1206:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 96
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 96
INDIRP4
ARGP4
ADDRLP4 96
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1207
;1207:	if (tr.fraction == 1.0)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
NEF4 $472
line 1208
;1208:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $459
JUMPV
LABELV $472
line 1210
;1209:
;1210:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 1211
;1211:	dest[0] += 15.0;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 1212
;1212:	dest[1] -= 15.0;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 1213
;1213:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 100
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 100
INDIRP4
ARGP4
ADDRLP4 100
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1214
;1214:	if (tr.fraction == 1.0)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
NEF4 $476
line 1215
;1215:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $459
JUMPV
LABELV $476
line 1217
;1216:
;1217:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 1218
;1218:	dest[0] -= 15.0;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 1219
;1219:	dest[1] += 15.0;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 1220
;1220:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 104
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 104
INDIRP4
ARGP4
ADDRLP4 104
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1221
;1221:	if (tr.fraction == 1.0)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
NEF4 $480
line 1222
;1222:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $459
JUMPV
LABELV $480
line 1224
;1223:
;1224:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 1225
;1225:	dest[0] -= 15.0;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 1226
;1226:	dest[1] -= 15.0;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 1227
;1227:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 108
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 108
INDIRP4
ARGP4
ADDRLP4 108
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1228
;1228:	if (tr.fraction == 1.0)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
NEF4 $484
line 1229
;1229:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $459
JUMPV
LABELV $484
line 1232
;1230:
;1231:
;1232:	return qfalse;
CNSTI4 0
RETI4
LABELV $459
endproc CanDamage 112 28
export G_RadiusDamage
proc G_RadiusDamage 4196 32
line 1242
;1233:}
;1234:
;1235:
;1236:/*
;1237:============
;1238:G_RadiusDamage
;1239:============
;1240:*/
;1241:qboolean G_RadiusDamage ( vec3_t origin, gentity_t *attacker, float damage, float radius,
;1242:					 gentity_t *ignore, int mod) {
line 1251
;1243:	float		points, dist;
;1244:	gentity_t	*ent;
;1245:	int			entityList[MAX_GENTITIES];
;1246:	int			numListedEntities;
;1247:	vec3_t		mins, maxs;
;1248:	vec3_t		v;
;1249:	vec3_t		dir;
;1250:	int			i, e;
;1251:	qboolean	hitClient = qfalse;
ADDRLP4 4168
CNSTI4 0
ASGNI4
line 1253
;1252:
;1253:	if ( radius < 1 ) {
ADDRFP4 12
INDIRF4
CNSTF4 1065353216
GEF4 $488
line 1254
;1254:		radius = 1;
ADDRFP4 12
CNSTF4 1065353216
ASGNF4
line 1255
;1255:	}
LABELV $488
line 1257
;1256:
;1257:	for ( i = 0 ; i < 3 ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $490
line 1258
;1258:		mins[i] = origin[i] - radius;
ADDRLP4 4172
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4172
INDIRI4
ADDRLP4 4144
ADDP4
ADDRLP4 4172
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRFP4 12
INDIRF4
SUBF4
ASGNF4
line 1259
;1259:		maxs[i] = origin[i] + radius;
ADDRLP4 4176
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4176
INDIRI4
ADDRLP4 4156
ADDP4
ADDRLP4 4176
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRFP4 12
INDIRF4
ADDF4
ASGNF4
line 1260
;1260:	}
LABELV $491
line 1257
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $490
line 1262
;1261:
;1262:	numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );
ADDRLP4 4144
ARGP4
ADDRLP4 4156
ARGP4
ADDRLP4 44
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4172
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 4140
ADDRLP4 4172
INDIRI4
ASGNI4
line 1264
;1263:
;1264:	for ( e = 0 ; e < numListedEntities ; e++ ) {
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRGP4 $497
JUMPV
LABELV $494
line 1265
;1265:		ent = &g_entities[entityList[ e ]];
ADDRLP4 4
CNSTI4 808
ADDRLP4 20
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 44
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1267
;1266:
;1267:		if (ent == ignore)
ADDRLP4 4
INDIRP4
CVPU4 4
ADDRFP4 16
INDIRP4
CVPU4 4
NEU4 $498
line 1268
;1268:			continue;
ADDRGP4 $495
JUMPV
LABELV $498
line 1269
;1269:		if (!ent->takedamage)
ADDRLP4 4
INDIRP4
CNSTI4 736
ADDP4
INDIRI4
CNSTI4 0
NEI4 $500
line 1270
;1270:			continue;
ADDRGP4 $495
JUMPV
LABELV $500
line 1273
;1271:
;1272:		// find the distance from the edge of the bounding box
;1273:		for ( i = 0 ; i < 3 ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $502
line 1274
;1274:			if ( origin[i] < ent->r.absmin[i] ) {
ADDRLP4 4176
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4176
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4176
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 464
ADDP4
ADDP4
INDIRF4
GEF4 $506
line 1275
;1275:				v[i] = ent->r.absmin[i] - origin[i];
ADDRLP4 4180
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4180
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 4180
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 464
ADDP4
ADDP4
INDIRF4
ADDRLP4 4180
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1276
;1276:			} else if ( origin[i] > ent->r.absmax[i] ) {
ADDRGP4 $507
JUMPV
LABELV $506
ADDRLP4 4180
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4180
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4180
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 476
ADDP4
ADDP4
INDIRF4
LEF4 $508
line 1277
;1277:				v[i] = origin[i] - ent->r.absmax[i];
ADDRLP4 4184
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4184
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 4184
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4184
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 476
ADDP4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1278
;1278:			} else {
ADDRGP4 $509
JUMPV
LABELV $508
line 1279
;1279:				v[i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
CNSTF4 0
ASGNF4
line 1280
;1280:			}
LABELV $509
LABELV $507
line 1281
;1281:		}
LABELV $503
line 1273
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $502
line 1283
;1282:
;1283:		dist = VectorLength( v );
ADDRLP4 8
ARGP4
ADDRLP4 4176
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 24
ADDRLP4 4176
INDIRF4
ASGNF4
line 1284
;1284:		if ( dist >= radius ) {
ADDRLP4 24
INDIRF4
ADDRFP4 12
INDIRF4
LTF4 $510
line 1285
;1285:			continue;
ADDRGP4 $495
JUMPV
LABELV $510
line 1288
;1286:		}
;1287:
;1288:		points = damage * ( 1.0 - dist / radius );
ADDRLP4 40
ADDRFP4 8
INDIRF4
CNSTF4 1065353216
ADDRLP4 24
INDIRF4
ADDRFP4 12
INDIRF4
DIVF4
SUBF4
MULF4
ASGNF4
line 1290
;1289:
;1290:		if( CanDamage (ent, origin) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4180
ADDRGP4 CanDamage
CALLI4
ASGNI4
ADDRLP4 4180
INDIRI4
CNSTI4 0
EQI4 $512
line 1291
;1291:			if( LogAccuracyHit( ent, attacker ) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4184
ADDRGP4 LogAccuracyHit
CALLI4
ASGNI4
ADDRLP4 4184
INDIRI4
CNSTI4 0
EQI4 $514
line 1292
;1292:				hitClient = qtrue;
ADDRLP4 4168
CNSTI4 1
ASGNI4
line 1293
;1293:			}
LABELV $514
line 1294
;1294:			VectorSubtract (ent->r.currentOrigin, origin, dir);
ADDRLP4 4192
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
ADDRLP4 4
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
ADDRLP4 4192
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 28+4
ADDRLP4 4
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
ADDRLP4 4192
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 28+8
ADDRLP4 4
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1297
;1295:			// push the center of mass higher than the origin so players
;1296:			// get knocked into the air more
;1297:			dir[2] += 24;
ADDRLP4 28+8
ADDRLP4 28+8
INDIRF4
CNSTF4 1103101952
ADDF4
ASGNF4
line 1298
;1298:			G_Damage (ent, NULL, attacker, dir, origin, (int)points, DAMAGE_RADIUS, mod);
ADDRLP4 4
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 28
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 40
INDIRF4
CVFI4 4
ARGI4
CNSTI4 1
ARGI4
ADDRFP4 20
INDIRI4
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 1299
;1299:		}
LABELV $512
line 1300
;1300:	}
LABELV $495
line 1264
ADDRLP4 20
ADDRLP4 20
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $497
ADDRLP4 20
INDIRI4
ADDRLP4 4140
INDIRI4
LTI4 $494
line 1302
;1301:
;1302:	return hitClient;
ADDRLP4 4168
INDIRI4
RETI4
LABELV $487
endproc G_RadiusDamage 4196 32
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
import ClientBegin
import ClientDisconnect
import ClientUserinfoChanged
import ClientConnect
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
import SpotWouldTelefrag
import CalculateRanks
import ClientSpawn
import InitBodyQue
import InitClientResp
import InitClientPersistant
import BeginIntermission
import respawn
import CopyToBodyQue
import SelectSpawnPoint
import SetClientViewAngle
import PickTeam
import TeamLeader
import TeamCount
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
import G_InvulnerabilityEffect
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
LABELV $450
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
byte 1 107
byte 1 110
byte 1 111
byte 1 99
byte 1 107
byte 1 101
byte 1 100
byte 1 32
byte 1 121
byte 1 111
byte 1 117
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 33
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $449
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
byte 1 115
byte 1 101
byte 1 110
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 98
byte 1 105
byte 1 103
byte 1 44
byte 1 32
byte 1 98
byte 1 108
byte 1 97
byte 1 99
byte 1 107
byte 1 32
byte 1 118
byte 1 111
byte 1 105
byte 1 100
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $448
byte 1 99
byte 1 112
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 32
byte 1 107
byte 1 110
byte 1 111
byte 1 99
byte 1 107
byte 1 101
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 33
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $447
byte 1 99
byte 1 112
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 32
byte 1 115
byte 1 108
byte 1 105
byte 1 112
byte 1 112
byte 1 101
byte 1 100
byte 1 32
byte 1 105
byte 1 110
byte 1 116
byte 1 111
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 97
byte 1 98
byte 1 121
byte 1 115
byte 1 115
byte 1 33
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $446
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 32
byte 1 34
byte 1 89
byte 1 111
byte 1 117
byte 1 32
byte 1 107
byte 1 110
byte 1 111
byte 1 99
byte 1 107
byte 1 101
byte 1 100
byte 1 32
byte 1 121
byte 1 111
byte 1 117
byte 1 114
byte 1 115
byte 1 101
byte 1 108
byte 1 102
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
byte 1 33
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $443
byte 1 76
byte 1 111
byte 1 115
byte 1 101
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 110
byte 1 111
byte 1 119
byte 1 32
byte 1 105
byte 1 110
byte 1 32
byte 1 106
byte 1 97
byte 1 105
byte 1 108
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $442
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 32
byte 1 34
byte 1 94
byte 1 51
byte 1 89
byte 1 111
byte 1 117
byte 1 32
byte 1 97
byte 1 114
byte 1 101
byte 1 32
byte 1 110
byte 1 111
byte 1 119
byte 1 32
byte 1 105
byte 1 110
byte 1 32
byte 1 106
byte 1 97
byte 1 105
byte 1 108
byte 1 33
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $409
byte 1 37
byte 1 105
byte 1 58
byte 1 32
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 58
byte 1 37
byte 1 105
byte 1 32
byte 1 104
byte 1 101
byte 1 97
byte 1 108
byte 1 116
byte 1 104
byte 1 58
byte 1 37
byte 1 105
byte 1 32
byte 1 100
byte 1 97
byte 1 109
byte 1 97
byte 1 103
byte 1 101
byte 1 58
byte 1 37
byte 1 105
byte 1 32
byte 1 97
byte 1 114
byte 1 109
byte 1 111
byte 1 114
byte 1 58
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $320
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
byte 1 107
byte 1 110
byte 1 111
byte 1 99
byte 1 107
byte 1 101
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 94
byte 1 55
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
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $317
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
byte 1 116
byte 1 104
byte 1 111
byte 1 117
byte 1 103
byte 1 104
byte 1 116
byte 1 32
byte 1 104
byte 1 101
byte 1 32
byte 1 104
byte 1 97
byte 1 100
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 97
byte 1 100
byte 1 118
byte 1 97
byte 1 110
byte 1 116
byte 1 97
byte 1 103
byte 1 101
byte 1 32
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 32
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $312
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
byte 1 119
byte 1 111
byte 1 110
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 114
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 32
byte 1 97
byte 1 103
byte 1 97
byte 1 105
byte 1 110
byte 1 115
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $307
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
byte 1 109
byte 1 97
byte 1 100
byte 1 101
byte 1 32
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 32
byte 1 102
byte 1 108
byte 1 121
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $302
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
byte 1 39
byte 1 115
byte 1 32
byte 1 97
byte 1 114
byte 1 109
byte 1 111
byte 1 114
byte 1 32
byte 1 119
byte 1 97
byte 1 115
byte 1 32
byte 1 117
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 115
byte 1 115
byte 1 32
byte 1 97
byte 1 103
byte 1 97
byte 1 105
byte 1 110
byte 1 115
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $297
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
byte 1 116
byte 1 117
byte 1 114
byte 1 110
byte 1 101
byte 1 100
byte 1 32
byte 1 111
byte 1 102
byte 1 102
byte 1 32
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 39
byte 1 115
byte 1 32
byte 1 98
byte 1 114
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 44
byte 1 32
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 32
byte 1 108
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $292
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
byte 1 107
byte 1 110
byte 1 101
byte 1 119
byte 1 32
byte 1 104
byte 1 111
byte 1 119
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 117
byte 1 115
byte 1 101
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 113
byte 1 117
byte 1 97
byte 1 100
byte 1 32
byte 1 97
byte 1 103
byte 1 97
byte 1 105
byte 1 110
byte 1 115
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $287
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
byte 1 105
byte 1 115
byte 1 32
byte 1 107
byte 1 105
byte 1 99
byte 1 107
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 97
byte 1 115
byte 1 115
byte 1 32
byte 1 97
byte 1 110
byte 1 100
byte 1 32
byte 1 116
byte 1 97
byte 1 107
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 39
byte 1 115
byte 1 32
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $284
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
byte 1 117
byte 1 108
byte 1 100
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 104
byte 1 97
byte 1 110
byte 1 100
byte 1 108
byte 1 101
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 112
byte 1 114
byte 1 101
byte 1 115
byte 1 115
byte 1 117
byte 1 114
byte 1 101
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $283
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
byte 1 39
byte 1 115
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 104
byte 1 101
byte 1 97
byte 1 108
byte 1 116
byte 1 104
byte 1 32
byte 1 100
byte 1 105
byte 1 100
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 114
byte 1 101
byte 1 102
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 32
byte 1 104
byte 1 105
byte 1 115
byte 1 32
byte 1 115
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $280
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
byte 1 115
byte 1 97
byte 1 119
byte 1 32
byte 1 105
byte 1 116
byte 1 32
byte 1 117
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 115
byte 1 115
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 99
byte 1 111
byte 1 110
byte 1 116
byte 1 105
byte 1 110
byte 1 117
byte 1 101
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $277
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
byte 1 102
byte 1 111
byte 1 114
byte 1 103
byte 1 111
byte 1 116
byte 1 32
byte 1 104
byte 1 111
byte 1 119
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 102
byte 1 108
byte 1 121
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $274
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
byte 1 119
byte 1 97
byte 1 115
byte 1 32
byte 1 119
byte 1 101
byte 1 97
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 84
byte 1 79
byte 1 79
byte 1 32
byte 1 109
byte 1 117
byte 1 99
byte 1 104
byte 1 32
byte 1 97
byte 1 114
byte 1 109
byte 1 111
byte 1 114
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $271
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
byte 1 100
byte 1 105
byte 1 100
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 32
byte 1 104
byte 1 111
byte 1 119
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 117
byte 1 115
byte 1 101
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 113
byte 1 117
byte 1 97
byte 1 100
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $268
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
byte 1 39
byte 1 115
byte 1 32
byte 1 97
byte 1 114
byte 1 109
byte 1 111
byte 1 114
byte 1 32
byte 1 119
byte 1 97
byte 1 115
byte 1 32
byte 1 116
byte 1 111
byte 1 111
byte 1 32
byte 1 104
byte 1 101
byte 1 97
byte 1 118
byte 1 121
byte 1 46
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $263
byte 1 68
byte 1 105
byte 1 101
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 102
byte 1 101
byte 1 108
byte 1 108
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
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $206
byte 1 75
byte 1 105
byte 1 108
byte 1 108
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 101
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 98
byte 1 121
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $205
byte 1 60
byte 1 98
byte 1 97
byte 1 100
byte 1 32
byte 1 111
byte 1 98
byte 1 105
byte 1 116
byte 1 117
byte 1 97
byte 1 114
byte 1 121
byte 1 62
byte 1 0
align 1
LABELV $198
byte 1 60
byte 1 119
byte 1 111
byte 1 114
byte 1 108
byte 1 100
byte 1 62
byte 1 0
align 1
LABELV $197
byte 1 60
byte 1 110
byte 1 111
byte 1 110
byte 1 45
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 62
byte 1 0
align 1
LABELV $176
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 111
byte 1 98
byte 1 101
byte 1 108
byte 1 105
byte 1 115
byte 1 107
byte 1 0
align 1
LABELV $175
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 111
byte 1 98
byte 1 101
byte 1 108
byte 1 105
byte 1 115
byte 1 107
byte 1 0
align 1
LABELV $155
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 67
byte 1 84
byte 1 70
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $154
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 67
byte 1 84
byte 1 70
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $143
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 71
byte 1 82
byte 1 65
byte 1 80
byte 1 80
byte 1 76
byte 1 69
byte 1 0
align 1
LABELV $142
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 70
byte 1 65
byte 1 76
byte 1 76
byte 1 0
align 1
LABELV $141
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 84
byte 1 82
byte 1 73
byte 1 71
byte 1 71
byte 1 69
byte 1 82
byte 1 95
byte 1 72
byte 1 85
byte 1 82
byte 1 84
byte 1 0
align 1
LABELV $140
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 84
byte 1 65
byte 1 82
byte 1 71
byte 1 69
byte 1 84
byte 1 95
byte 1 76
byte 1 65
byte 1 83
byte 1 69
byte 1 82
byte 1 0
align 1
LABELV $139
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 83
byte 1 85
byte 1 73
byte 1 67
byte 1 73
byte 1 68
byte 1 69
byte 1 0
align 1
LABELV $138
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 70
byte 1 65
byte 1 76
byte 1 76
byte 1 73
byte 1 78
byte 1 71
byte 1 0
align 1
LABELV $137
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 84
byte 1 69
byte 1 76
byte 1 69
byte 1 70
byte 1 82
byte 1 65
byte 1 71
byte 1 0
align 1
LABELV $136
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 67
byte 1 82
byte 1 85
byte 1 83
byte 1 72
byte 1 0
align 1
LABELV $135
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 76
byte 1 65
byte 1 86
byte 1 65
byte 1 0
align 1
LABELV $134
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 83
byte 1 76
byte 1 73
byte 1 77
byte 1 69
byte 1 0
align 1
LABELV $133
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 87
byte 1 65
byte 1 84
byte 1 69
byte 1 82
byte 1 0
align 1
LABELV $132
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 66
byte 1 70
byte 1 71
byte 1 95
byte 1 83
byte 1 80
byte 1 76
byte 1 65
byte 1 83
byte 1 72
byte 1 0
align 1
LABELV $131
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 66
byte 1 70
byte 1 71
byte 1 0
align 1
LABELV $130
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 76
byte 1 73
byte 1 71
byte 1 72
byte 1 84
byte 1 78
byte 1 73
byte 1 78
byte 1 71
byte 1 0
align 1
LABELV $129
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 82
byte 1 65
byte 1 73
byte 1 76
byte 1 71
byte 1 85
byte 1 78
byte 1 0
align 1
LABELV $128
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 80
byte 1 76
byte 1 65
byte 1 83
byte 1 77
byte 1 65
byte 1 95
byte 1 83
byte 1 80
byte 1 76
byte 1 65
byte 1 83
byte 1 72
byte 1 0
align 1
LABELV $127
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 80
byte 1 76
byte 1 65
byte 1 83
byte 1 77
byte 1 65
byte 1 0
align 1
LABELV $126
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 82
byte 1 79
byte 1 67
byte 1 75
byte 1 69
byte 1 84
byte 1 95
byte 1 83
byte 1 80
byte 1 76
byte 1 65
byte 1 83
byte 1 72
byte 1 0
align 1
LABELV $125
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 82
byte 1 79
byte 1 67
byte 1 75
byte 1 69
byte 1 84
byte 1 0
align 1
LABELV $124
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 71
byte 1 82
byte 1 69
byte 1 78
byte 1 65
byte 1 68
byte 1 69
byte 1 95
byte 1 83
byte 1 80
byte 1 76
byte 1 65
byte 1 83
byte 1 72
byte 1 0
align 1
LABELV $123
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 71
byte 1 82
byte 1 69
byte 1 78
byte 1 65
byte 1 68
byte 1 69
byte 1 0
align 1
LABELV $122
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 77
byte 1 65
byte 1 67
byte 1 72
byte 1 73
byte 1 78
byte 1 69
byte 1 71
byte 1 85
byte 1 78
byte 1 0
align 1
LABELV $121
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 71
byte 1 65
byte 1 85
byte 1 78
byte 1 84
byte 1 76
byte 1 69
byte 1 84
byte 1 0
align 1
LABELV $120
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 83
byte 1 72
byte 1 79
byte 1 84
byte 1 71
byte 1 85
byte 1 78
byte 1 0
align 1
LABELV $119
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 85
byte 1 78
byte 1 75
byte 1 78
byte 1 79
byte 1 87
byte 1 78
byte 1 0
align 1
LABELV $112
byte 1 107
byte 1 97
byte 1 109
byte 1 105
byte 1 107
byte 1 97
byte 1 122
byte 1 101
byte 1 32
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 114
byte 1 0
