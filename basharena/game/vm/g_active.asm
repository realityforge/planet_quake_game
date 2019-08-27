export P_DamageFeedback
code
proc P_DamageFeedback 36 12
file "../g_active.c"
line 20
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:
;4:#include "g_local.h"
;5:
;6:float health;			//Declares health to use in formula ^^^
;7:float speed;			//Declares speed for use in forumula
;8:
;9:
;10:/*
;11:===============
;12:G_DamageFeedback
;13:
;14:Called just before a snapshot is sent to the given player.
;15:Totals up all damage and generates both the player_state_t
;16:damage values to that client for pain blends and kicks, and
;17:global pain sound events for all clients.
;18:===============
;19:*/
;20:void P_DamageFeedback( gentity_t *player ) {
line 25
;21:	gclient_t	*client;
;22:	float	count;
;23:	vec3_t	angles;
;24:
;25:	client = player->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 26
;26:	if ( client->ps.pm_type == PM_DEAD ) {
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
NEI4 $53
line 27
;27:		return;
ADDRGP4 $52
JUMPV
LABELV $53
line 31
;28:	}
;29:
;30:	// total points of damage shot at the player this frame
;31:	count = client->damage_blood + client->damage_armor;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 684
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
ADDI4
CVIF4 4
ASGNF4
line 32
;32:	if ( count == 0 ) {
ADDRLP4 4
INDIRF4
CNSTF4 0
NEF4 $55
line 33
;33:		return;		// didn't take any damage
ADDRGP4 $52
JUMPV
LABELV $55
line 36
;34:	}
;35:
;36:	if ( count > 255 ) {
ADDRLP4 4
INDIRF4
CNSTF4 1132396544
LEF4 $57
line 37
;37:		count = 255;
ADDRLP4 4
CNSTF4 1132396544
ASGNF4
line 38
;38:	}
LABELV $57
line 44
;39:
;40:	// send the information to the client
;41:
;42:	// world damage (falling, slime, etc) uses a special code
;43:	// to make the blend blob centered instead of positional
;44:	if ( client->damage_fromWorld ) {
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
INDIRI4
CNSTI4 0
EQI4 $59
line 45
;45:		client->ps.damagePitch = 255;
ADDRLP4 0
INDIRP4
CNSTI4 176
ADDP4
CNSTI4 255
ASGNI4
line 46
;46:		client->ps.damageYaw = 255;
ADDRLP4 0
INDIRP4
CNSTI4 172
ADDP4
CNSTI4 255
ASGNI4
line 48
;47:
;48:		client->damage_fromWorld = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 0
ASGNI4
line 49
;49:	} else {
ADDRGP4 $60
JUMPV
LABELV $59
line 50
;50:		vectoangles( client->damage_from, angles );
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
ARGP4
ADDRLP4 8
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 51
;51:		client->ps.damagePitch = angles[PITCH]/360.0 * 256;
ADDRLP4 0
INDIRP4
CNSTI4 176
ADDP4
CNSTF4 1132462080
ADDRLP4 8
INDIRF4
CNSTF4 1135869952
DIVF4
MULF4
CVFI4 4
ASGNI4
line 52
;52:		client->ps.damageYaw = angles[YAW]/360.0 * 256;
ADDRLP4 0
INDIRP4
CNSTI4 172
ADDP4
CNSTF4 1132462080
ADDRLP4 8+4
INDIRF4
CNSTF4 1135869952
DIVF4
MULF4
CVFI4 4
ASGNI4
line 53
;53:	}
LABELV $60
line 56
;54:
;55:	// play an apropriate pain sound
;56:	if ( (level.time > player->pain_debounce_time) && !(player->flags & FL_GODMODE) ) {
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 level+36
INDIRI4
ADDRLP4 24
INDIRP4
CNSTI4 720
ADDP4
INDIRI4
LEI4 $62
ADDRLP4 24
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
NEI4 $62
line 57
;57:		player->pain_debounce_time = level.time + 700;
ADDRFP4 0
INDIRP4
CNSTI4 720
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 700
ADDI4
ASGNI4
line 58
;58:		G_AddEvent( player, EV_PAIN, player->health );
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
CNSTI4 56
ARGI4
ADDRLP4 28
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 59
;59:		client->ps.damageEvent++;
ADDRLP4 32
ADDRLP4 0
INDIRP4
CNSTI4 168
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 60
;60:	}
LABELV $62
line 63
;61:
;62:
;63:	client->ps.damageCount = count;
ADDRLP4 0
INDIRP4
CNSTI4 180
ADDP4
ADDRLP4 4
INDIRF4
CVFI4 4
ASGNI4
line 68
;64:
;65:	//
;66:	// clear totals
;67:	//
;68:	client->damage_blood = 0;
ADDRLP4 0
INDIRP4
CNSTI4 684
ADDP4
CNSTI4 0
ASGNI4
line 69
;69:	client->damage_armor = 0;
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 70
;70:	client->damage_knockback = 0;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 0
ASGNI4
line 71
;71:}
LABELV $52
endproc P_DamageFeedback 36 12
export P_WorldEffects
proc P_WorldEffects 40 32
line 82
;72:
;73:
;74:
;75:/*
;76:=============
;77:P_WorldEffects
;78:
;79:Check for lava / slime contents and drowning
;80:=============
;81:*/
;82:void P_WorldEffects( gentity_t *ent ) {
line 86
;83:	qboolean	envirosuit;
;84:	int			waterlevel;
;85:
;86:	if ( ent->client->noclip ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 648
ADDP4
INDIRI4
CNSTI4 0
EQI4 $67
line 87
;87:		ent->client->airOutTime = level.time + 12000;	// don't need air
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 760
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 12000
ADDI4
ASGNI4
line 88
;88:		return;
ADDRGP4 $66
JUMPV
LABELV $67
line 91
;89:	}
;90:
;91:	waterlevel = ent->waterlevel;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 788
ADDP4
INDIRI4
ASGNI4
line 93
;92:
;93:	envirosuit = ent->client->ps.powerups[PW_BATTLESUIT] > level.time;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 320
ADDP4
INDIRI4
ADDRGP4 level+36
INDIRI4
LEI4 $72
ADDRLP4 8
CNSTI4 1
ASGNI4
ADDRGP4 $73
JUMPV
LABELV $72
ADDRLP4 8
CNSTI4 0
ASGNI4
LABELV $73
ADDRLP4 4
ADDRLP4 8
INDIRI4
ASGNI4
line 98
;94:
;95:	//
;96:	// check for drowning
;97:	//
;98:	if ( waterlevel == 3 ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
NEI4 $74
line 100
;99:		// envirosuit give air
;100:		if ( envirosuit ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $76
line 101
;101:			ent->client->airOutTime = level.time + 10000;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 760
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 10000
ADDI4
ASGNI4
line 102
;102:		}
LABELV $76
line 105
;103:
;104:		// if out of air, start drowning
;105:		if ( ent->client->airOutTime < level.time) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 760
ADDP4
INDIRI4
ADDRGP4 level+36
INDIRI4
GEI4 $75
line 107
;106:			// drown!
;107:			ent->client->airOutTime += 1000;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 760
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 108
;108:			if ( ent->health > 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 0
LEI4 $75
line 110
;109:				// take more damage the longer underwater
;110:				ent->damage += 2;
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 2
ADDI4
ASGNI4
line 111
;111:				if (ent->damage > 15)
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
INDIRI4
CNSTI4 15
LEI4 $84
line 112
;112:					ent->damage = 15;
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
CNSTI4 15
ASGNI4
LABELV $84
line 115
;113:
;114:				// play a gurp sound instead of a normal pain sound
;115:				if (ent->health <= ent->damage) {
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 20
INDIRP4
CNSTI4 740
ADDP4
INDIRI4
GTI4 $86
line 116
;116:					G_Sound(ent, CHAN_VOICE, G_SoundIndex("*drown.wav"));
ADDRGP4 $88
ARGP4
ADDRLP4 24
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 24
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 117
;117:				} else if (rand()&1) {
ADDRGP4 $87
JUMPV
LABELV $86
ADDRLP4 24
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $89
line 118
;118:					G_Sound(ent, CHAN_VOICE, G_SoundIndex("sound/player/gurp1.wav"));
ADDRGP4 $91
ARGP4
ADDRLP4 28
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 28
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 119
;119:				} else {
ADDRGP4 $90
JUMPV
LABELV $89
line 120
;120:					G_Sound(ent, CHAN_VOICE, G_SoundIndex("sound/player/gurp2.wav"));
ADDRGP4 $92
ARGP4
ADDRLP4 28
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 28
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 121
;121:				}
LABELV $90
LABELV $87
line 124
;122:
;123:				// don't play a normal pain sound
;124:				ent->pain_debounce_time = level.time + 200;
ADDRFP4 0
INDIRP4
CNSTI4 720
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 126
;125:
;126:				G_Damage (ent, NULL, NULL, NULL, NULL, 
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 32
CNSTP4 0
ASGNP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRLP4 36
CNSTP4 0
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 28
INDIRP4
CNSTI4 740
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
CNSTI4 14
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 128
;127:					ent->damage, DAMAGE_NO_ARMOR, MOD_WATER);
;128:			}
line 129
;129:		}
line 130
;130:	} else {
ADDRGP4 $75
JUMPV
LABELV $74
line 131
;131:		ent->client->airOutTime = level.time + 12000;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 760
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 12000
ADDI4
ASGNI4
line 132
;132:		ent->damage = 2;
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
CNSTI4 2
ASGNI4
line 133
;133:	}
LABELV $75
line 138
;134:
;135:	//
;136:	// check for sizzle damage (move to pmove?)
;137:	//
;138:	if (waterlevel && 
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $95
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
CNSTI4 24
BANDI4
ADDRLP4 12
INDIRI4
EQI4 $95
line 139
;139:		(ent->watertype&(CONTENTS_LAVA|CONTENTS_SLIME)) ) {
line 140
;140:		if (ent->health > 0
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 0
LEI4 $97
ADDRLP4 16
INDIRP4
CNSTI4 720
ADDP4
INDIRI4
ADDRGP4 level+36
INDIRI4
GTI4 $97
line 141
;141:			&& ent->pain_debounce_time <= level.time	) {
line 143
;142:
;143:			if ( envirosuit ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $100
line 144
;144:				G_AddEvent( ent, EV_POWERUP_BATTLESUIT, 0 );
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
line 145
;145:			} else {
ADDRGP4 $101
JUMPV
LABELV $100
line 146
;146:				if (ent->watertype & CONTENTS_LAVA) {
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $102
line 147
;147:					G_Damage (ent, NULL, NULL, NULL, NULL, 
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 20
CNSTP4 0
ASGNP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 24
CNSTP4 0
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
CNSTI4 30
ADDRLP4 0
INDIRI4
MULI4
ARGI4
CNSTI4 0
ARGI4
CNSTI4 16
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 149
;148:						30*waterlevel, 0, MOD_LAVA);
;149:				}
LABELV $102
line 151
;150:
;151:				if (ent->watertype & CONTENTS_SLIME) {
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $104
line 152
;152:					G_Damage (ent, NULL, NULL, NULL, NULL, 
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 20
CNSTP4 0
ASGNP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 24
CNSTP4 0
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
CNSTI4 10
ADDRLP4 0
INDIRI4
MULI4
ARGI4
CNSTI4 0
ARGI4
CNSTI4 15
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 154
;153:						10*waterlevel, 0, MOD_SLIME);
;154:				}
LABELV $104
line 155
;155:			}
LABELV $101
line 156
;156:		}
LABELV $97
line 157
;157:	}
LABELV $95
line 158
;158:}
LABELV $66
endproc P_WorldEffects 40 32
export G_SetClientSound
proc G_SetClientSound 8 0
line 167
;159:
;160:
;161:
;162:/*
;163:===============
;164:G_SetClientSound
;165:===============
;166:*/
;167:void G_SetClientSound( gentity_t *ent ) {
line 174
;168:#ifdef MISSIONPACK
;169:	if( ent->s.eFlags & EF_TICKING ) {
;170:		ent->client->ps.loopSound = G_SoundIndex( "sound/weapons/proxmine/wstbtick.wav");
;171:	}
;172:	else
;173:#endif
;174:	if (ent->waterlevel && (ent->watertype&(CONTENTS_LAVA|CONTENTS_SLIME)) ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 788
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $107
ADDRLP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
CNSTI4 24
BANDI4
ADDRLP4 4
INDIRI4
EQI4 $107
line 175
;175:		ent->client->ps.loopSound = level.snd_fry;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 444
ADDP4
ADDRGP4 level+352
INDIRI4
ASGNI4
line 176
;176:	} else {
ADDRGP4 $108
JUMPV
LABELV $107
line 177
;177:		ent->client->ps.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 444
ADDP4
CNSTI4 0
ASGNI4
line 178
;178:	}
LABELV $108
line 179
;179:}
LABELV $106
endproc G_SetClientSound 8 0
export ClientImpacts
proc ClientImpacts 76 12
line 190
;180:
;181:
;182:
;183://==============================================================
;184:
;185:/*
;186:==============
;187:ClientImpacts
;188:==============
;189:*/
;190:void ClientImpacts( gentity_t *ent, pmove_t *pm ) {
line 195
;191:	int		i, j;
;192:	trace_t	trace;
;193:	gentity_t	*other;
;194:
;195:	memset( &trace, 0, sizeof( trace ) );
ADDRLP4 12
ARGP4
CNSTI4 0
ARGI4
CNSTI4 56
ARGI4
ADDRGP4 memset
CALLP4
pop
line 196
;196:	for (i=0 ; i<pm->numtouch ; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $114
JUMPV
LABELV $111
line 197
;197:		for (j=0 ; j<i ; j++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $118
JUMPV
LABELV $115
line 198
;198:			if (pm->touchents[j] == pm->touchents[i] ) {
ADDRLP4 68
CNSTI4 2
ASGNI4
ADDRLP4 72
ADDRFP4 4
INDIRP4
CNSTI4 52
ADDP4
ASGNP4
ADDRLP4 0
INDIRI4
ADDRLP4 68
INDIRI4
LSHI4
ADDRLP4 72
INDIRP4
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDRLP4 68
INDIRI4
LSHI4
ADDRLP4 72
INDIRP4
ADDP4
INDIRI4
NEI4 $119
line 199
;199:				break;
ADDRGP4 $117
JUMPV
LABELV $119
line 201
;200:			}
;201:		}
LABELV $116
line 197
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $118
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $115
LABELV $117
line 202
;202:		if (j != i) {
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $121
line 203
;203:			continue;	// duplicated
ADDRGP4 $112
JUMPV
LABELV $121
line 205
;204:		}
;205:		other = &g_entities[ pm->touchents[i] ];
ADDRLP4 8
CNSTI4 808
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 52
ADDP4
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 207
;206:
;207:		if ( ( ent->r.svFlags & SVF_BOT ) && ( ent->touch ) ) {
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $123
ADDRLP4 68
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $123
line 208
;208:			ent->touch( ent, other, &trace );
ADDRLP4 72
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 72
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CALLV
pop
line 209
;209:		}
LABELV $123
line 211
;210:
;211:		if ( !other->touch ) {
ADDRLP4 8
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $125
line 212
;212:			continue;
ADDRGP4 $112
JUMPV
LABELV $125
line 215
;213:		}
;214:
;215:		other->touch( other, ent, &trace );
ADDRLP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 8
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CALLV
pop
line 216
;216:	}
LABELV $112
line 196
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $114
ADDRLP4 4
INDIRI4
ADDRFP4 4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
LTI4 $111
line 218
;217:
;218:}
LABELV $110
endproc ClientImpacts 76 12
data
align 4
LABELV $128
byte 4 1109393408
byte 4 1109393408
byte 4 1112539136
export G_TouchTriggers
code
proc G_TouchTriggers 4240 16
line 228
;219:
;220:/*
;221:============
;222:G_TouchTriggers
;223:
;224:Find all trigger entities that ent's current position touches.
;225:Spectators will only interact with teleporters.
;226:============
;227:*/
;228:void	G_TouchTriggers( gentity_t *ent ) {
line 236
;229:	int			i, num;
;230:	int			touch[MAX_GENTITIES];
;231:	gentity_t	*hit;
;232:	trace_t		trace;
;233:	vec3_t		mins, maxs;
;234:	static vec3_t	range = { 40, 40, 52 };
;235:
;236:	if ( !ent->client ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $129
line 237
;237:		return;
ADDRGP4 $127
JUMPV
LABELV $129
line 241
;238:	}
;239:
;240:	// dead clients don't activate triggers!
;241:	if ( ent->client->ps.stats[STAT_HEALTH] <= 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 0
GTI4 $131
line 242
;242:		return;
ADDRGP4 $127
JUMPV
LABELV $131
line 245
;243:	}
;244:
;245:	VectorSubtract( ent->client->ps.origin, range, mins );
ADDRLP4 4188
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 64
ADDRLP4 4188
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRGP4 $128
INDIRF4
SUBF4
ASGNF4
ADDRLP4 64+4
ADDRLP4 4188
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRGP4 $128+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 64+8
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRGP4 $128+8
INDIRF4
SUBF4
ASGNF4
line 246
;246:	VectorAdd( ent->client->ps.origin, range, maxs );
ADDRLP4 4192
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 76
ADDRLP4 4192
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRGP4 $128
INDIRF4
ADDF4
ASGNF4
ADDRLP4 76+4
ADDRLP4 4192
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRGP4 $128+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 76+8
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRGP4 $128+8
INDIRF4
ADDF4
ASGNF4
line 248
;247:
;248:	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );
ADDRLP4 64
ARGP4
ADDRLP4 76
ARGP4
ADDRLP4 92
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4196
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 88
ADDRLP4 4196
INDIRI4
ASGNI4
line 251
;249:
;250:	// can't use ent->absmin, because that has a one unit pad
;251:	VectorAdd( ent->client->ps.origin, ent->r.mins, mins );
ADDRLP4 4200
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4204
ADDRLP4 4200
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 64
ADDRLP4 4204
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 4200
INDIRP4
CNSTI4 436
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 64+4
ADDRLP4 4204
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 4200
INDIRP4
CNSTI4 440
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4208
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64+8
ADDRLP4 4208
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 4208
INDIRP4
CNSTI4 444
ADDP4
INDIRF4
ADDF4
ASGNF4
line 252
;252:	VectorAdd( ent->client->ps.origin, ent->r.maxs, maxs );
ADDRLP4 4212
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4216
ADDRLP4 4212
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 76
ADDRLP4 4216
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 4212
INDIRP4
CNSTI4 448
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 76+4
ADDRLP4 4216
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 4212
INDIRP4
CNSTI4 452
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4220
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 76+8
ADDRLP4 4220
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 4220
INDIRP4
CNSTI4 456
ADDP4
INDIRF4
ADDF4
ASGNF4
line 254
;253:
;254:	for ( i=0 ; i<num ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $148
JUMPV
LABELV $145
line 255
;255:		hit = &g_entities[touch[i]];
ADDRLP4 0
CNSTI4 808
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 92
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 257
;256:
;257:		if ( !hit->touch && !ent->touch ) {
ADDRLP4 4224
CNSTI4 704
ASGNI4
ADDRLP4 4228
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
ADDRLP4 4224
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 4228
INDIRU4
NEU4 $149
ADDRFP4 0
INDIRP4
ADDRLP4 4224
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 4228
INDIRU4
NEU4 $149
line 258
;258:			continue;
ADDRGP4 $146
JUMPV
LABELV $149
line 260
;259:		}
;260:		if ( !( hit->r.contents & CONTENTS_TRIGGER ) ) {
ADDRLP4 0
INDIRP4
CNSTI4 460
ADDP4
INDIRI4
CNSTI4 1073741824
BANDI4
CNSTI4 0
NEI4 $151
line 261
;261:			continue;
ADDRGP4 $146
JUMPV
LABELV $151
line 265
;262:		}
;263:
;264:		// ignore most entities if a spectator
;265:		if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $153
line 266
;266:			if ( hit->s.eType != ET_TELEPORT_TRIGGER &&
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 9
EQI4 $155
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CVPU4 4
ADDRGP4 Touch_DoorTrigger
CVPU4 4
EQU4 $155
line 269
;267:				// this is ugly but adding a new ET_? type will
;268:				// most likely cause network incompatibilities
;269:				hit->touch != Touch_DoorTrigger) {
line 270
;270:				continue;
ADDRGP4 $146
JUMPV
LABELV $155
line 272
;271:			}
;272:		}
LABELV $153
line 276
;273:
;274:		// use seperate code for determining if an item is picked up
;275:		// so you don't have to actually contact its bounding box
;276:		if ( hit->s.eType == ET_ITEM ) {
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 2
NEI4 $157
line 277
;277:			if ( !BG_PlayerTouchesItem( &ent->client->ps, &hit->s, level.time ) ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 level+36
INDIRI4
ARGI4
ADDRLP4 4232
ADDRGP4 BG_PlayerTouchesItem
CALLI4
ASGNI4
ADDRLP4 4232
INDIRI4
CNSTI4 0
NEI4 $158
line 278
;278:				continue;
ADDRGP4 $146
JUMPV
line 280
;279:			}
;280:		} else {
LABELV $157
line 281
;281:			if ( !trap_EntityContact( mins, maxs, hit ) ) {
ADDRLP4 64
ARGP4
ADDRLP4 76
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4232
ADDRGP4 trap_EntityContact
CALLI4
ASGNI4
ADDRLP4 4232
INDIRI4
CNSTI4 0
NEI4 $162
line 282
;282:				continue;
ADDRGP4 $146
JUMPV
LABELV $162
line 284
;283:			}
;284:		}
LABELV $158
line 286
;285:
;286:		memset( &trace, 0, sizeof(trace) );
ADDRLP4 8
ARGP4
CNSTI4 0
ARGI4
CNSTI4 56
ARGI4
ADDRGP4 memset
CALLP4
pop
line 288
;287:
;288:		if ( hit->touch ) {
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $164
line 289
;289:			hit->touch (hit, ent, &trace);
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CALLV
pop
line 290
;290:		}
LABELV $164
line 292
;291:
;292:		if ( ( ent->r.svFlags & SVF_BOT ) && ( ent->touch ) ) {
ADDRLP4 4232
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4232
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $166
ADDRLP4 4232
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $166
line 293
;293:			ent->touch( ent, hit, &trace );
ADDRLP4 4236
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4236
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 4236
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CALLV
pop
line 294
;294:		}
LABELV $166
line 295
;295:	}
LABELV $146
line 254
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $148
ADDRLP4 4
INDIRI4
ADDRLP4 88
INDIRI4
LTI4 $145
line 298
;296:
;297:	// if we didn't touch a jump pad this pmove frame
;298:	if ( ent->client->ps.jumppad_frame != ent->client->ps.pmove_framecount ) {
ADDRLP4 4224
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4224
INDIRP4
CNSTI4 460
ADDP4
INDIRI4
ADDRLP4 4224
INDIRP4
CNSTI4 456
ADDP4
INDIRI4
EQI4 $168
line 299
;299:		ent->client->ps.jumppad_frame = 0;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 0
ASGNI4
line 300
;300:		ent->client->ps.jumppad_ent = 0;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 448
ADDP4
CNSTI4 0
ASGNI4
line 301
;301:	}
LABELV $168
line 302
;302:}
LABELV $127
endproc G_TouchTriggers 4240 16
export SpectatorThink
proc SpectatorThink 252 12
line 309
;303:
;304:/*
;305:=================
;306:SpectatorThink
;307:=================
;308:*/
;309:void SpectatorThink( gentity_t *ent, usercmd_t *ucmd ) {
line 313
;310:	pmove_t	pm;
;311:	gclient_t	*client;
;312:
;313:	client = ent->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 315
;314:
;315:	if ( client->sess.spectatorState != SPECTATOR_FOLLOW ) {
ADDRLP4 0
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 2
EQI4 $171
line 316
;316:		client->ps.pm_type = PM_SPECTATOR;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 2
ASGNI4
line 317
;317:		client->ps.speed = 400;	// faster than normal
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
CNSTI4 400
ASGNI4
line 320
;318:
;319:		// set up for pmove
;320:		memset (&pm, 0, sizeof(pm));
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 232
ARGI4
ADDRGP4 memset
CALLP4
pop
line 321
;321:		pm.ps = &client->ps;
ADDRLP4 4
ADDRLP4 0
INDIRP4
ASGNP4
line 322
;322:		pm.cmd = *ucmd;
ADDRLP4 4+4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 24
line 323
;323:		pm.tracemask = MASK_PLAYERSOLID & ~CONTENTS_BODY;	// spectators can fly through bodies
ADDRLP4 4+28
CNSTI4 65537
ASGNI4
line 324
;324:		pm.trace = trap_Trace;
ADDRLP4 4+224
ADDRGP4 trap_Trace
ASGNP4
line 325
;325:		pm.pointcontents = trap_PointContents;
ADDRLP4 4+228
ADDRGP4 trap_PointContents
ASGNP4
line 328
;326:
;327:		// perform a pmove
;328:		Pmove (&pm);
ADDRLP4 4
ARGP4
ADDRGP4 Pmove
CALLV
pop
line 330
;329:		// save results of pmove
;330:		VectorCopy( client->ps.origin, ent->s.origin );
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 332
;331:
;332:		G_TouchTriggers( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_TouchTriggers
CALLV
pop
line 333
;333:		trap_UnlinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 334
;334:	}
LABELV $171
line 336
;335:
;336:	client->oldbuttons = client->buttons;
ADDRLP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
INDIRI4
ASGNI4
line 337
;337:	client->buttons = ucmd->buttons;
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 340
;338:
;339:	// attack button cycles through spectators
;340:	if ( ( client->buttons & BUTTON_ATTACK ) && ! ( client->oldbuttons & BUTTON_ATTACK ) ) {
ADDRLP4 244
CNSTI4 1
ASGNI4
ADDRLP4 248
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
INDIRI4
ADDRLP4 244
INDIRI4
BANDI4
ADDRLP4 248
INDIRI4
EQI4 $177
ADDRLP4 0
INDIRP4
CNSTI4 660
ADDP4
INDIRI4
ADDRLP4 244
INDIRI4
BANDI4
ADDRLP4 248
INDIRI4
NEI4 $177
line 341
;341:		Cmd_FollowCycle_f( ent, 1 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Cmd_FollowCycle_f
CALLV
pop
line 342
;342:	}
LABELV $177
line 343
;343:}
LABELV $170
endproc SpectatorThink 252 12
export ClientInactivityTimer
proc ClientInactivityTimer 12 8
line 354
;344:
;345:
;346:
;347:/*
;348:=================
;349:ClientInactivityTimer
;350:
;351:Returns qfalse if the client is dropped
;352:=================
;353:*/
;354:qboolean ClientInactivityTimer( gclient_t *client ) {
line 355
;355:	if ( ! g_inactivity.integer ) {
ADDRGP4 g_inactivity+12
INDIRI4
CNSTI4 0
NEI4 $180
line 358
;356:		// give everyone some time, so if the operator sets g_inactivity during
;357:		// gameplay, everyone isn't kicked
;358:		client->inactivityTime = level.time + 60 * 1000;
ADDRFP4 0
INDIRP4
CNSTI4 748
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 60000
ADDI4
ASGNI4
line 359
;359:		client->inactivityWarning = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 752
ADDP4
CNSTI4 0
ASGNI4
line 360
;360:	} else if ( client->pers.cmd.forwardmove || 
ADDRGP4 $181
JUMPV
LABELV $180
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 493
ADDP4
INDIRI1
CVII4 1
ADDRLP4 4
INDIRI4
NEI4 $188
ADDRLP4 0
INDIRP4
CNSTI4 494
ADDP4
INDIRI1
CVII4 1
ADDRLP4 4
INDIRI4
NEI4 $188
ADDRLP4 0
INDIRP4
CNSTI4 495
ADDP4
INDIRI1
CVII4 1
ADDRLP4 4
INDIRI4
NEI4 $188
ADDRLP4 0
INDIRP4
CNSTI4 488
ADDP4
INDIRI4
CNSTI4 1
BANDI4
ADDRLP4 4
INDIRI4
EQI4 $184
LABELV $188
line 363
;361:		client->pers.cmd.rightmove || 
;362:		client->pers.cmd.upmove ||
;363:		(client->pers.cmd.buttons & BUTTON_ATTACK) ) {
line 364
;364:		client->inactivityTime = level.time + g_inactivity.integer * 1000;
ADDRFP4 0
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
line 365
;365:		client->inactivityWarning = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 752
ADDP4
CNSTI4 0
ASGNI4
line 366
;366:	} else if ( !client->pers.localClient ) {
ADDRGP4 $185
JUMPV
LABELV $184
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRI4
CNSTI4 0
NEI4 $191
line 367
;367:		if ( level.time > client->inactivityTime ) {
ADDRGP4 level+36
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 748
ADDP4
INDIRI4
LEI4 $193
line 368
;368:			trap_DropClient( client - level.clients, "Dropped due to inactivity" );
ADDRFP4 0
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
ADDRGP4 $196
ARGP4
ADDRGP4 trap_DropClient
CALLV
pop
line 369
;369:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $179
JUMPV
LABELV $193
line 371
;370:		}
;371:		if ( level.time > client->inactivityTime - 10000 && !client->inactivityWarning ) {
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 level+36
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 748
ADDP4
INDIRI4
CNSTI4 10000
SUBI4
LEI4 $197
ADDRLP4 8
INDIRP4
CNSTI4 752
ADDP4
INDIRI4
CNSTI4 0
NEI4 $197
line 372
;372:			client->inactivityWarning = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 752
ADDP4
CNSTI4 1
ASGNI4
line 373
;373:			trap_SendServerCommand( client - level.clients, "cp \"Ten seconds until inactivity drop!\n\"" );
ADDRFP4 0
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
ADDRGP4 $200
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 374
;374:		}
LABELV $197
line 375
;375:	}
LABELV $191
LABELV $185
LABELV $181
line 376
;376:	return qtrue;
CNSTI4 1
RETI4
LABELV $179
endproc ClientInactivityTimer 12 8
export ClientTimerActions
proc ClientTimerActions 20 12
line 386
;377:}
;378:
;379:/*
;380:==================
;381:ClientTimerActions
;382:
;383:Actions that happen once a second
;384:==================
;385:*/
;386:void ClientTimerActions( gentity_t *ent, int msec ) {
line 392
;387:	gclient_t	*client;
;388:#ifdef MISSIONPACK
;389:	int			maxHealth;
;390:#endif
;391:
;392:	client = ent->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 393
;393:	client->timeResidual += msec;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 780
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
ADDRFP4 4
INDIRI4
ADDI4
ASGNI4
ADDRGP4 $203
JUMPV
LABELV $202
line 395
;394:
;395:	while ( client->timeResidual >= 1000 ) {
line 396
;396:		client->timeResidual -= 1000;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 780
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1000
SUBI4
ASGNI4
line 424
;397:
;398:		// regenerate
;399:#ifdef MISSIONPACK
;400:		if( bg_itemlist[client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD ) {
;401:			maxHealth = client->ps.stats[STAT_MAX_HEALTH] / 2;
;402:		}
;403:		else if ( client->ps.powerups[PW_REGEN] ) {
;404:			maxHealth = client->ps.stats[STAT_MAX_HEALTH];
;405:		}
;406:		else {
;407:			maxHealth = 0;
;408:		}
;409:		if( maxHealth ) {
;410:			if ( ent->health < maxHealth ) {
;411:				ent->health += 15;
;412:				if ( ent->health > maxHealth * 1.1 ) {
;413:					ent->health = maxHealth * 1.1;
;414:				}
;415:				G_AddEvent( ent, EV_POWERUP_REGEN, 0 );
;416:			} else if ( ent->health < maxHealth * 2) {
;417:				ent->health += 5;
;418:				if ( ent->health > maxHealth * 2 ) {
;419:					ent->health = maxHealth * 2;
;420:				}
;421:				G_AddEvent( ent, EV_POWERUP_REGEN, 0 );
;422:			}
;423:#else
;424:		if ( client->ps.powerups[PW_REGEN] ) {
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
INDIRI4
CNSTI4 0
EQI4 $205
line 425
;425:			if ( ent->health < client->ps.stats[STAT_MAX_HEALTH]) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
GEI4 $207
line 426
;426:				ent->health += 15;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 15
ADDI4
ASGNI4
line 427
;427:				if ( ent->health > client->ps.stats[STAT_MAX_HEALTH] * 1.1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1066192077
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CVIF4 4
MULF4
LEF4 $209
line 428
;428:					ent->health = client->ps.stats[STAT_MAX_HEALTH] * 1.1;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
CNSTF4 1066192077
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 429
;429:				}
LABELV $209
line 430
;430:				G_AddEvent( ent, EV_POWERUP_REGEN, 0 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 63
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 431
;431:			} else if ( ent->health < client->ps.stats[STAT_MAX_HEALTH] * 2) {
ADDRGP4 $206
JUMPV
LABELV $207
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1
LSHI4
GEI4 $206
line 432
;432:				ent->health += 5;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 5
ADDI4
ASGNI4
line 433
;433:				if ( ent->health > client->ps.stats[STAT_MAX_HEALTH] * 2 ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1
LSHI4
LEI4 $213
line 434
;434:					ent->health = client->ps.stats[STAT_MAX_HEALTH] * 2;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1
LSHI4
ASGNI4
line 435
;435:				}
LABELV $213
line 436
;436:				G_AddEvent( ent, EV_POWERUP_REGEN, 0 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 63
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 437
;437:			}
line 439
;438:#endif
;439:		} else {
ADDRGP4 $206
JUMPV
LABELV $205
line 441
;440:			// count down health when over max
;441:			if ( ent->health > client->ps.stats[STAT_MAX_HEALTH] ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
LEI4 $215
line 442
;442:				ent->health--;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 443
;443:			}
LABELV $215
line 444
;444:		}
LABELV $206
line 447
;445:
;446:		// count down armor when over max
;447:		if ( client->ps.stats[STAT_ARMOR] > client->ps.stats[STAT_MAX_HEALTH] ) {
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
LEI4 $217
line 448
;448:			client->ps.stats[STAT_ARMOR]--;
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 449
;449:		}
LABELV $217
line 450
;450:	}
LABELV $203
line 395
ADDRLP4 0
INDIRP4
CNSTI4 780
ADDP4
INDIRI4
CNSTI4 1000
GEI4 $202
line 489
;451:#ifdef MISSIONPACK
;452:	if( bg_itemlist[client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_AMMOREGEN ) {
;453:		int w, max, inc, t, i;
;454:    int weapList[]={WP_MACHINEGUN,WP_SHOTGUN,WP_GRENADE_LAUNCHER,WP_ROCKET_LAUNCHER,WP_LIGHTNING,WP_RAILGUN,WP_PLASMAGUN,WP_BFG,WP_NAILGUN,WP_PROX_LAUNCHER,WP_CHAINGUN};
;455:    int weapCount = sizeof(weapList) / sizeof(int);
;456:		//
;457:    for (i = 0; i < weapCount; i++) {
;458:		  w = weapList[i];
;459:
;460:		  switch(w) {
;461:			  case WP_MACHINEGUN: max = 50; inc = 4; t = 1000; break;
;462:			  case WP_SHOTGUN: max = 10; inc = 1; t = 1500; break;
;463:			  case WP_GRENADE_LAUNCHER: max = 10; inc = 1; t = 2000; break;
;464:			  case WP_ROCKET_LAUNCHER: max = 10; inc = 1; t = 1750; break;
;465:			  case WP_LIGHTNING: max = 50; inc = 5; t = 1500; break;
;466:			  case WP_RAILGUN: max = 10; inc = 1; t = 1750; break;
;467:			  case WP_PLASMAGUN: max = 50; inc = 5; t = 1500; break;
;468:			  case WP_BFG: max = 10; inc = 1; t = 4000; break;
;469:			  case WP_NAILGUN: max = 10; inc = 1; t = 1250; break;
;470:			  case WP_PROX_LAUNCHER: max = 5; inc = 1; t = 2000; break;
;471:			  case WP_CHAINGUN: max = 100; inc = 5; t = 1000; break;
;472:			  default: max = 0; inc = 0; t = 1000; break;
;473:		  }
;474:		  client->ammoTimes[w] += msec;
;475:		  if ( client->ps.ammo[w] >= max ) {
;476:			  client->ammoTimes[w] = 0;
;477:		  }
;478:		  if ( client->ammoTimes[w] >= t ) {
;479:			  while ( client->ammoTimes[w] >= t )
;480:				  client->ammoTimes[w] -= t;
;481:			  client->ps.ammo[w] += inc;
;482:			  if ( client->ps.ammo[w] > max ) {
;483:				  client->ps.ammo[w] = max;
;484:			  }
;485:		  }
;486:    }
;487:	}
;488:#endif
;489:}
LABELV $201
endproc ClientTimerActions 20 12
export ClientIntermissionThink
proc ClientIntermissionThink 24 0
line 496
;490:
;491:/*
;492:====================
;493:ClientIntermissionThink
;494:====================
;495:*/
;496:void ClientIntermissionThink( gclient_t *client ) {
line 497
;497:	client->ps.eFlags &= ~EF_TALK;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 -4097
BANDI4
ASGNI4
line 498
;498:	client->ps.eFlags &= ~EF_FIRING;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -257
BANDI4
ASGNI4
line 505
;499:
;500://	level.time = 0;		// ^^^ Resetting level time to get an accurate round time result
;501:
;502:	// the level will exit when everyone wants to or after timeouts
;503:
;504:	// swap and latch button actions
;505:	client->oldbuttons = client->buttons;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 660
ADDP4
ADDRLP4 8
INDIRP4
CNSTI4 656
ADDP4
INDIRI4
ASGNI4
line 506
;506:	client->buttons = client->pers.cmd.buttons;
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 656
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 488
ADDP4
INDIRI4
ASGNI4
line 507
;507:	if ( client->buttons & ( BUTTON_ATTACK | BUTTON_USE_HOLDABLE ) & ( client->oldbuttons ^ client->buttons ) ) {
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
ADDRLP4 16
INDIRP4
CNSTI4 656
ADDP4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 5
BANDI4
ADDRLP4 16
INDIRP4
CNSTI4 660
ADDP4
INDIRI4
ADDRLP4 20
INDIRI4
BXORI4
BANDI4
CNSTI4 0
EQI4 $220
line 509
;508:		// this used to be an ^1 but once a player says ready, it should stick
;509:		client->readyToExit = 1;
ADDRFP4 0
INDIRP4
CNSTI4 644
ADDP4
CNSTI4 1
ASGNI4
line 510
;510:	}
LABELV $220
line 511
;511:}
LABELV $219
endproc ClientIntermissionThink 24 0
export ClientEvents
proc ClientEvents 92 32
line 522
;512:
;513:
;514:/*
;515:================
;516:ClientEvents
;517:
;518:Events will be passed on to the clients for presentation,
;519:but any server game effects are handled here
;520:================
;521:*/
;522:void ClientEvents( gentity_t *ent, int oldEventSequence ) {
line 533
;523:	int		i, j;
;524:	int		event;
;525:	gclient_t *client;
;526:	int		damage;
;527:	vec3_t	dir;
;528:	vec3_t	origin, angles;
;529://	qboolean	fired;
;530:	gitem_t *item;
;531:	gentity_t *drop;
;532:
;533:	client = ent->client;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 535
;534:
;535:	if ( oldEventSequence < client->ps.eventSequence - MAX_PS_EVENTS ) {
ADDRFP4 4
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
CNSTI4 2
SUBI4
GEI4 $223
line 536
;536:		oldEventSequence = client->ps.eventSequence - MAX_PS_EVENTS;
ADDRFP4 4
ADDRLP4 8
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
CNSTI4 2
SUBI4
ASGNI4
line 537
;537:	}
LABELV $223
line 538
;538:	for ( i = oldEventSequence ; i < client->ps.eventSequence ; i++ ) {
ADDRLP4 0
ADDRFP4 4
INDIRI4
ASGNI4
ADDRGP4 $228
JUMPV
LABELV $225
line 539
;539:		event = client->ps.events[ i & (MAX_PS_EVENTS-1) ];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRP4
CNSTI4 112
ADDP4
ADDP4
INDIRI4
ASGNI4
line 541
;540:
;541:		switch ( event ) {
ADDRLP4 68
CNSTI4 11
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 68
INDIRI4
EQI4 $231
ADDRLP4 4
INDIRI4
CNSTI4 12
EQI4 $231
ADDRLP4 4
INDIRI4
ADDRLP4 68
INDIRI4
LTI4 $230
LABELV $256
ADDRLP4 4
INDIRI4
CNSTI4 23
EQI4 $242
ADDRLP4 4
INDIRI4
CNSTI4 25
EQI4 $243
ADDRLP4 4
INDIRI4
CNSTI4 26
EQI4 $255
ADDRGP4 $230
JUMPV
LABELV $231
line 544
;542:		case EV_FALL_MEDIUM:
;543:		case EV_FALL_FAR:
;544:			if ( ent->s.eType != ET_PLAYER ) {
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 1
EQI4 $232
line 545
;545:				break;		// not in the player model
ADDRGP4 $230
JUMPV
LABELV $232
line 547
;546:			}
;547:			if ( g_dmflags.integer & DF_NO_FALLING ) {
ADDRGP4 g_dmflags+12
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $234
line 548
;548:				break;
ADDRGP4 $230
JUMPV
LABELV $234
line 550
;549:			}
;550:			if ( event == EV_FALL_FAR ) {
ADDRLP4 4
INDIRI4
CNSTI4 12
NEI4 $237
line 551
;551:				damage = 10;
ADDRLP4 32
CNSTI4 10
ASGNI4
line 552
;552:			} else {
ADDRGP4 $238
JUMPV
LABELV $237
line 553
;553:				damage = 5;
ADDRLP4 32
CNSTI4 5
ASGNI4
line 554
;554:			}
LABELV $238
line 555
;555:			VectorSet (dir, 0, 0, 1);
ADDRLP4 76
CNSTF4 0
ASGNF4
ADDRLP4 16
ADDRLP4 76
INDIRF4
ASGNF4
ADDRLP4 16+4
ADDRLP4 76
INDIRF4
ASGNF4
ADDRLP4 16+8
CNSTF4 1065353216
ASGNF4
line 556
;556:			ent->pain_debounce_time = level.time + 200;	// no normal pain sound
ADDRFP4 0
INDIRP4
CNSTI4 720
ADDP4
ADDRGP4 level+36
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 557
;557:			G_Damage (ent, NULL, NULL, NULL, NULL, damage, 0, MOD_FALLING);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 80
CNSTP4 0
ASGNP4
ADDRLP4 80
INDIRP4
ARGP4
ADDRLP4 80
INDIRP4
ARGP4
ADDRLP4 84
CNSTP4 0
ASGNP4
ADDRLP4 84
INDIRP4
ARGP4
ADDRLP4 84
INDIRP4
ARGP4
ADDRLP4 32
INDIRI4
ARGI4
CNSTI4 0
ARGI4
CNSTI4 19
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 558
;558:			break;
ADDRGP4 $230
JUMPV
LABELV $242
line 561
;559:
;560:		case EV_FIRE_WEAPON:
;561:			FireWeapon( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 FireWeapon
CALLV
pop
line 562
;562:			break;
ADDRGP4 $230
JUMPV
LABELV $243
line 566
;563:
;564:		case EV_USE_ITEM1:		// teleporter
;565:			// drop flags in CTF
;566:			item = NULL;
ADDRLP4 12
CNSTP4 0
ASGNP4
line 567
;567:			j = 0;
ADDRLP4 28
CNSTI4 0
ASGNI4
line 569
;568:
;569:			if ( ent->client->ps.powerups[ PW_REDFLAG ] ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 340
ADDP4
INDIRI4
CNSTI4 0
EQI4 $244
line 570
;570:				item = BG_FindItemForPowerup( PW_REDFLAG );
CNSTI4 7
ARGI4
ADDRLP4 88
ADDRGP4 BG_FindItemForPowerup
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 88
INDIRP4
ASGNP4
line 571
;571:				j = PW_REDFLAG;
ADDRLP4 28
CNSTI4 7
ASGNI4
line 572
;572:			} else if ( ent->client->ps.powerups[ PW_BLUEFLAG ] ) {
ADDRGP4 $245
JUMPV
LABELV $244
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 344
ADDP4
INDIRI4
CNSTI4 0
EQI4 $246
line 573
;573:				item = BG_FindItemForPowerup( PW_BLUEFLAG );
CNSTI4 8
ARGI4
ADDRLP4 88
ADDRGP4 BG_FindItemForPowerup
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 88
INDIRP4
ASGNP4
line 574
;574:				j = PW_BLUEFLAG;
ADDRLP4 28
CNSTI4 8
ASGNI4
line 575
;575:			} else if ( ent->client->ps.powerups[ PW_NEUTRALFLAG ] ) {
ADDRGP4 $247
JUMPV
LABELV $246
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 348
ADDP4
INDIRI4
CNSTI4 0
EQI4 $248
line 576
;576:				item = BG_FindItemForPowerup( PW_NEUTRALFLAG );
CNSTI4 9
ARGI4
ADDRLP4 88
ADDRGP4 BG_FindItemForPowerup
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 88
INDIRP4
ASGNP4
line 577
;577:				j = PW_NEUTRALFLAG;
ADDRLP4 28
CNSTI4 9
ASGNI4
line 578
;578:			}
LABELV $248
LABELV $247
LABELV $245
line 580
;579:
;580:			if ( item ) {
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $250
line 581
;581:				drop = Drop_Item( ent, item, 0 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRLP4 88
ADDRGP4 Drop_Item
CALLP4
ASGNP4
ADDRLP4 60
ADDRLP4 88
INDIRP4
ASGNP4
line 583
;582:				// decide how many seconds it has left
;583:				drop->count = ( ent->client->ps.powerups[ j ] - level.time ) / 1000;
ADDRLP4 60
INDIRP4
CNSTI4 760
ADDP4
ADDRLP4 28
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
line 584
;584:				if ( drop->count < 1 ) {
ADDRLP4 60
INDIRP4
CNSTI4 760
ADDP4
INDIRI4
CNSTI4 1
GEI4 $253
line 585
;585:					drop->count = 1;
ADDRLP4 60
INDIRP4
CNSTI4 760
ADDP4
CNSTI4 1
ASGNI4
line 586
;586:				}
LABELV $253
line 588
;587:
;588:				ent->client->ps.powerups[ j ] = 0;
ADDRLP4 28
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
CNSTI4 0
ASGNI4
line 589
;589:			}
LABELV $250
line 613
;590:
;591:#ifdef MISSIONPACK
;592:			if ( g_gametype.integer == GT_HARVESTER ) {
;593:				if ( ent->client->ps.generic1 > 0 ) {
;594:					if ( ent->client->sess.sessionTeam == TEAM_RED ) {
;595:						item = BG_FindItem( "Blue Cube" );
;596:					} else {
;597:						item = BG_FindItem( "Red Cube" );
;598:					}
;599:					if ( item ) {
;600:						for ( j = 0; j < ent->client->ps.generic1; j++ ) {
;601:							drop = Drop_Item( ent, item, 0 );
;602:							if ( ent->client->sess.sessionTeam == TEAM_RED ) {
;603:								drop->spawnflags = TEAM_BLUE;
;604:							} else {
;605:								drop->spawnflags = TEAM_RED;
;606:							}
;607:						}
;608:					}
;609:					ent->client->ps.generic1 = 0;
;610:				}
;611:			}
;612:#endif
;613:			SelectSpawnPoint( ent->client->ps.origin, origin, angles );
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 36
ARGP4
ADDRLP4 48
ARGP4
ADDRGP4 SelectSpawnPoint
CALLP4
pop
line 614
;614:			TeleportPlayer( ent, origin, angles );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 36
ARGP4
ADDRLP4 48
ARGP4
ADDRGP4 TeleportPlayer
CALLV
pop
line 615
;615:			break;
ADDRGP4 $230
JUMPV
LABELV $255
line 618
;616:
;617:		case EV_USE_ITEM2:		// medkit
;618:			ent->health = ent->client->ps.stats[STAT_MAX_HEALTH] + 25;
ADDRLP4 88
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 88
INDIRP4
CNSTI4 732
ADDP4
ADDRLP4 88
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 25
ADDI4
ASGNI4
line 620
;619:
;620:			break;
line 644
;621:
;622:#ifdef MISSIONPACK
;623:		case EV_USE_ITEM3:		// kamikaze
;624:			// make sure the invulnerability is off
;625:			ent->client->invulnerabilityTime = 0;
;626:			// start the kamikze
;627:			G_StartKamikaze( ent );
;628:			break;
;629:
;630:		case EV_USE_ITEM4:		// portal
;631:			if( ent->client->portalID ) {
;632:				DropPortalSource( ent );
;633:			}
;634:			else {
;635:				DropPortalDestination( ent );
;636:			}
;637:			break;
;638:		case EV_USE_ITEM5:		// invulnerability
;639:			ent->client->invulnerabilityTime = level.time + 10000;
;640:			break;
;641:#endif
;642:
;643:		default:
;644:			break;
LABELV $230
line 646
;645:		}
;646:	}
LABELV $226
line 538
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $228
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
LTI4 $225
line 648
;647:
;648:}
LABELV $222
endproc ClientEvents 92 32
export SendPendingPredictableEvents
proc SendPendingPredictableEvents 44 12
line 700
;649:
;650:#ifdef MISSIONPACK
;651:/*
;652:==============
;653:StuckInOtherClient
;654:==============
;655:*/
;656:static int StuckInOtherClient(gentity_t *ent) {
;657:	int i;
;658:	gentity_t	*ent2;
;659:
;660:	ent2 = &g_entities[0];
;661:	for ( i = 0; i < MAX_CLIENTS; i++, ent2++ ) {
;662:		if ( ent2 == ent ) {
;663:			continue;
;664:		}
;665:		if ( !ent2->inuse ) {
;666:			continue;
;667:		}
;668:		if ( !ent2->client ) {
;669:			continue;
;670:		}
;671:		if ( ent2->health <= 0 ) {
;672:			continue;
;673:		}
;674:		//
;675:		if (ent2->r.absmin[0] > ent->r.absmax[0])
;676:			continue;
;677:		if (ent2->r.absmin[1] > ent->r.absmax[1])
;678:			continue;
;679:		if (ent2->r.absmin[2] > ent->r.absmax[2])
;680:			continue;
;681:		if (ent2->r.absmax[0] < ent->r.absmin[0])
;682:			continue;
;683:		if (ent2->r.absmax[1] < ent->r.absmin[1])
;684:			continue;
;685:		if (ent2->r.absmax[2] < ent->r.absmin[2])
;686:			continue;
;687:		return qtrue;
;688:	}
;689:	return qfalse;
;690:}
;691:#endif
;692:
;693:void BotTestSolid(vec3_t origin);
;694:
;695:/*
;696:==============
;697:SendPendingPredictableEvents
;698:==============
;699:*/
;700:void SendPendingPredictableEvents( playerState_t *ps ) {
line 706
;701:	gentity_t *t;
;702:	int event, seq;
;703:	int extEvent, number;
;704:
;705:	// if there are still events pending
;706:	if ( ps->entityEventSequence < ps->eventSequence ) {
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 464
ADDP4
INDIRI4
ADDRLP4 20
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
GEI4 $258
line 709
;707:		// create a temporary entity for this event which is sent to everyone
;708:		// except the client who generated the event
;709:		seq = ps->entityEventSequence & (MAX_PS_EVENTS-1);
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 464
ADDP4
INDIRI4
CNSTI4 1
BANDI4
ASGNI4
line 710
;710:		event = ps->events[ seq ] | ( ( ps->entityEventSequence & 3 ) << 8 );
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 24
INDIRP4
CNSTI4 112
ADDP4
ADDP4
INDIRI4
ADDRLP4 24
INDIRP4
CNSTI4 464
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 8
LSHI4
BORI4
ASGNI4
line 712
;711:		// set external event to zero before calling BG_PlayerStateToEntityState
;712:		extEvent = ps->externalEvent;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
INDIRI4
ASGNI4
line 713
;713:		ps->externalEvent = 0;
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
CNSTI4 0
ASGNI4
line 715
;714:		// create temporary entity for event
;715:		t = G_TempEntity( ps->origin, event );
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 28
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 28
INDIRP4
ASGNP4
line 716
;716:		number = t->s.number;
ADDRLP4 16
ADDRLP4 0
INDIRP4
INDIRI4
ASGNI4
line 717
;717:		BG_PlayerStateToEntityState( ps, &t->s, qtrue );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 718
;718:		t->s.number = number;
ADDRLP4 0
INDIRP4
ADDRLP4 16
INDIRI4
ASGNI4
line 719
;719:		t->s.eType = ET_EVENTS + event;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 4
INDIRI4
CNSTI4 13
ADDI4
ASGNI4
line 720
;720:		t->s.eFlags |= EF_PLAYER_EVENT;
ADDRLP4 32
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 721
;721:		t->s.otherEntityNum = ps->clientNum;
ADDRLP4 36
CNSTI4 140
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
INDIRI4
ASGNI4
line 723
;722:		// send to everyone except the client who generated the event
;723:		t->r.svFlags |= SVF_NOTSINGLECLIENT;
ADDRLP4 40
ADDRLP4 0
INDIRP4
CNSTI4 424
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 2048
BORI4
ASGNI4
line 724
;724:		t->r.singleClient = ps->clientNum;
ADDRLP4 0
INDIRP4
CNSTI4 428
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ASGNI4
line 726
;725:		// set back external event
;726:		ps->externalEvent = extEvent;
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 727
;727:	}
LABELV $258
line 728
;728:}
LABELV $257
endproc SendPendingPredictableEvents 44 12
export ClientThink_real
proc ClientThink_real 304 16
line 741
;729:
;730:/*
;731:==============
;732:ClientThink
;733:
;734:This will be called once for each client frame, which will
;735:usually be a couple times for each server frame on fast clients.
;736:
;737:If "g_synchronousClients 1" is set, this will be called exactly
;738:once for each server frame, which makes for smooth demo recording.
;739:==============
;740:*/
;741:void ClientThink_real( gentity_t *ent ) {
line 748
;742:	gclient_t	*client;
;743:	pmove_t		pm;
;744:	int			oldEventSequence;
;745:	int			msec;
;746:	usercmd_t	*ucmd;
;747:
;748:	client = ent->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 751
;749:
;750:	// don't think if the client is not yet connected (and thus not yet spawned in)
;751:	if (client->pers.connected != CON_CONNECTED) {
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
EQI4 $261
line 752
;752:		return;
ADDRGP4 $260
JUMPV
LABELV $261
line 755
;753:	}
;754:	// mark the time, so the connection sprite can be removed
;755:	ucmd = &ent->client->pers.cmd;
ADDRLP4 236
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 472
ADDP4
ASGNP4
line 758
;756:
;757:	// sanity check the command time to prevent speedup cheating
;758:	if ( ucmd->serverTime > level.time + 200 ) {
ADDRLP4 236
INDIRP4
INDIRI4
ADDRGP4 level+36
INDIRI4
CNSTI4 200
ADDI4
LEI4 $263
line 759
;759:		ucmd->serverTime = level.time + 200;
ADDRLP4 236
INDIRP4
ADDRGP4 level+36
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 761
;760://		G_Printf("serverTime <<<<<\n" );
;761:	}
LABELV $263
line 762
;762:	if ( ucmd->serverTime < level.time - 1000 ) {
ADDRLP4 236
INDIRP4
INDIRI4
ADDRGP4 level+36
INDIRI4
CNSTI4 1000
SUBI4
GEI4 $267
line 763
;763:		ucmd->serverTime = level.time - 1000;
ADDRLP4 236
INDIRP4
ADDRGP4 level+36
INDIRI4
CNSTI4 1000
SUBI4
ASGNI4
line 765
;764://		G_Printf("serverTime >>>>>\n" );
;765:	} 
LABELV $267
line 767
;766:
;767:	msec = ucmd->serverTime - client->ps.commandTime;
ADDRLP4 240
ADDRLP4 236
INDIRP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
SUBI4
ASGNI4
line 770
;768:	// following others may result in bad times, but we still want
;769:	// to check for follow toggles
;770:	if ( msec < 1 && client->sess.spectatorState != SPECTATOR_FOLLOW ) {
ADDRLP4 240
INDIRI4
CNSTI4 1
GEI4 $271
ADDRLP4 0
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 2
EQI4 $271
line 771
;771:		return;
ADDRGP4 $260
JUMPV
LABELV $271
line 773
;772:	}
;773:	if ( msec > 200 ) {
ADDRLP4 240
INDIRI4
CNSTI4 200
LEI4 $273
line 774
;774:		msec = 200;
ADDRLP4 240
CNSTI4 200
ASGNI4
line 775
;775:	}
LABELV $273
line 777
;776:
;777:	if ( pmove_msec.integer < 8 ) {
ADDRGP4 pmove_msec+12
INDIRI4
CNSTI4 8
GEI4 $275
line 778
;778:		trap_Cvar_Set("pmove_msec", "8");
ADDRGP4 $278
ARGP4
ADDRGP4 $279
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 779
;779:	}
ADDRGP4 $276
JUMPV
LABELV $275
line 780
;780:	else if (pmove_msec.integer > 33) {
ADDRGP4 pmove_msec+12
INDIRI4
CNSTI4 33
LEI4 $280
line 781
;781:		trap_Cvar_Set("pmove_msec", "33");
ADDRGP4 $278
ARGP4
ADDRGP4 $283
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 782
;782:	}
LABELV $280
LABELV $276
line 784
;783:
;784:	if ( pmove_fixed.integer || client->pers.pmoveFixed ) {
ADDRLP4 248
CNSTI4 0
ASGNI4
ADDRGP4 pmove_fixed+12
INDIRI4
ADDRLP4 248
INDIRI4
NEI4 $287
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
INDIRI4
ADDRLP4 248
INDIRI4
EQI4 $284
LABELV $287
line 785
;785:		ucmd->serverTime = ((ucmd->serverTime + pmove_msec.integer-1) / pmove_msec.integer) * pmove_msec.integer;
ADDRLP4 236
INDIRP4
ADDRLP4 236
INDIRP4
INDIRI4
ADDRGP4 pmove_msec+12
INDIRI4
ADDI4
CNSTI4 1
SUBI4
ADDRGP4 pmove_msec+12
INDIRI4
DIVI4
ADDRGP4 pmove_msec+12
INDIRI4
MULI4
ASGNI4
line 788
;786:		//if (ucmd->serverTime - client->ps.commandTime <= 0)
;787:		//	return;
;788:	}
LABELV $284
line 793
;789:
;790:	//
;791:	// check for exiting intermission
;792:	//
;793:	if ( level.intermissiontime ) {
ADDRGP4 level+9132
INDIRI4
CNSTI4 0
EQI4 $291
line 794
;794:		ClientIntermissionThink( client );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 ClientIntermissionThink
CALLV
pop
line 795
;795:		return;
ADDRGP4 $260
JUMPV
LABELV $291
line 799
;796:	}
;797:
;798:	// spectators don't do much
;799:	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $294
line 800
;800:		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD ) {
ADDRLP4 0
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 3
NEI4 $296
line 801
;801:			return;
ADDRGP4 $260
JUMPV
LABELV $296
line 803
;802:		}
;803:		SpectatorThink( ent, ucmd );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 236
INDIRP4
ARGP4
ADDRGP4 SpectatorThink
CALLV
pop
line 804
;804:		return;
ADDRGP4 $260
JUMPV
LABELV $294
line 808
;805:	}
;806:
;807:	// check for inactivity timer, but never drop the local client of a non-dedicated server
;808:	if ( !ClientInactivityTimer( client ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 252
ADDRGP4 ClientInactivityTimer
CALLI4
ASGNI4
ADDRLP4 252
INDIRI4
CNSTI4 0
NEI4 $298
line 809
;809:		return;
ADDRGP4 $260
JUMPV
LABELV $298
line 813
;810:	}
;811:
;812:	// clear the rewards if time
;813:	if ( level.time > client->rewardTime ) {
ADDRGP4 level+36
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 756
ADDP4
INDIRI4
LEI4 $300
line 814
;814:		client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
ADDRLP4 256
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 256
INDIRP4
ADDRLP4 256
INDIRP4
INDIRI4
CNSTI4 -231497
BANDI4
ASGNI4
line 815
;815:	}
LABELV $300
line 817
;816:
;817:	if ( client->noclip ) {
ADDRLP4 0
INDIRP4
CNSTI4 648
ADDP4
INDIRI4
CNSTI4 0
EQI4 $303
line 818
;818:		client->ps.pm_type = PM_NOCLIP;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 1
ASGNI4
line 819
;819:	} else if ( client->ps.stats[STAT_HEALTH] <= 0 ) {
ADDRGP4 $304
JUMPV
LABELV $303
ADDRLP4 0
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 0
GTI4 $305
line 820
;820:		client->ps.pm_type = PM_DEAD;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 3
ASGNI4
line 821
;821:	} else {
ADDRGP4 $306
JUMPV
LABELV $305
line 822
;822:		client->ps.pm_type = PM_NORMAL;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 823
;823:	}
LABELV $306
LABELV $304
line 825
;824:
;825:	client->ps.gravity = g_gravity.value;
ADDRLP4 0
INDIRP4
CNSTI4 48
ADDP4
ADDRGP4 g_gravity+8
INDIRF4
CVFI4 4
ASGNI4
line 827
;826:
;827:	client->ps.speed = g_speed.value;
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
ADDRGP4 g_speed+8
INDIRF4
CVFI4 4
ASGNI4
line 835
;828:
;829:#ifdef MISSIONPACK
;830:	if( bg_itemlist[client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_SCOUT ) {
;831:		client->ps.speed *= 1.5;
;832:	}
;833:	else
;834:#endif
;835:	if ( client->ps.powerups[PW_HASTE] ) {
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
INDIRI4
CNSTI4 0
EQI4 $309
line 836
;836:		client->ps.speed *= 1.3;
ADDRLP4 256
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
ASGNP4
ADDRLP4 256
INDIRP4
CNSTF4 1067869798
ADDRLP4 256
INDIRP4
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 837
;837:	}
LABELV $309
line 840
;838:
;839:	// Let go of the hook if we aren't firing
;840:	if ( client->ps.weapon == WP_GRAPPLING_HOOK &&
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
CNSTI4 10
NEI4 $311
ADDRLP4 0
INDIRP4
CNSTI4 772
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $311
ADDRLP4 236
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $311
line 841
;841:		client->hook && !( ucmd->buttons & BUTTON_ATTACK ) ) {
line 842
;842:		Weapon_HookFree(client->hook);
ADDRLP4 0
INDIRP4
CNSTI4 772
ADDP4
INDIRP4
ARGP4
ADDRGP4 Weapon_HookFree
CALLV
pop
line 843
;843:	}
LABELV $311
line 846
;844:
;845:	// set up for pmove
;846:	oldEventSequence = client->ps.eventSequence;
ADDRLP4 244
ADDRLP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ASGNI4
line 848
;847:
;848:	memset (&pm, 0, sizeof(pm));
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 232
ARGI4
ADDRGP4 memset
CALLP4
pop
line 852
;849:
;850:	// check for the hit-scan gauntlet, don't let the action
;851:	// go through as an attack unless it actually hits something
;852:	if ( client->ps.weapon == WP_GAUNTLET && !( ucmd->buttons & BUTTON_TALK ) &&
ADDRLP4 264
CNSTI4 1
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ADDRLP4 264
INDIRI4
NEI4 $313
ADDRLP4 268
ADDRLP4 236
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
ADDRLP4 272
CNSTI4 0
ASGNI4
ADDRLP4 268
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 272
INDIRI4
NEI4 $313
ADDRLP4 268
INDIRI4
ADDRLP4 264
INDIRI4
BANDI4
ADDRLP4 272
INDIRI4
EQI4 $313
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ADDRLP4 272
INDIRI4
GTI4 $313
line 853
;853:		( ucmd->buttons & BUTTON_ATTACK ) && client->ps.weaponTime <= 0 ) {
line 854
;854:		pm.gauntletHit = CheckGauntletAttack( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 276
ADDRGP4 CheckGauntletAttack
CALLI4
ASGNI4
ADDRLP4 4+40
ADDRLP4 276
INDIRI4
ASGNI4
line 855
;855:	}
LABELV $313
line 857
;856:
;857:	if ( ent->flags & FL_FORCE_GESTURE ) {
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 32768
BANDI4
CNSTI4 0
EQI4 $316
line 858
;858:		ent->flags &= ~FL_FORCE_GESTURE;
ADDRLP4 276
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ASGNP4
ADDRLP4 276
INDIRP4
ADDRLP4 276
INDIRP4
INDIRI4
CNSTI4 -32769
BANDI4
ASGNI4
line 859
;859:		ent->client->pers.cmd.buttons |= BUTTON_GESTURE;
ADDRLP4 280
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 488
ADDP4
ASGNP4
ADDRLP4 280
INDIRP4
ADDRLP4 280
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 860
;860:	}
LABELV $316
line 889
;861:
;862:#ifdef MISSIONPACK
;863:	// check for invulnerability expansion before doing the Pmove
;864:	if (client->ps.powerups[PW_INVULNERABILITY] ) {
;865:		if ( !(client->ps.pm_flags & PMF_INVULEXPAND) ) {
;866:			vec3_t mins = { -42, -42, -42 };
;867:			vec3_t maxs = { 42, 42, 42 };
;868:			vec3_t oldmins, oldmaxs;
;869:
;870:			VectorCopy (ent->r.mins, oldmins);
;871:			VectorCopy (ent->r.maxs, oldmaxs);
;872:			// expand
;873:			VectorCopy (mins, ent->r.mins);
;874:			VectorCopy (maxs, ent->r.maxs);
;875:			trap_LinkEntity(ent);
;876:			// check if this would get anyone stuck in this player
;877:			if ( !StuckInOtherClient(ent) ) {
;878:				// set flag so the expanded size will be set in PM_CheckDuck
;879:				client->ps.pm_flags |= PMF_INVULEXPAND;
;880:			}
;881:			// set back
;882:			VectorCopy (oldmins, ent->r.mins);
;883:			VectorCopy (oldmaxs, ent->r.maxs);
;884:			trap_LinkEntity(ent);
;885:		}
;886:	}
;887:#endif
;888:
;889:	pm.ps = &client->ps;
ADDRLP4 4
ADDRLP4 0
INDIRP4
ASGNP4
line 890
;890:	pm.cmd = *ucmd;
ADDRLP4 4+4
ADDRLP4 236
INDIRP4
INDIRB
ASGNB 24
line 891
;891:	if ( pm.ps->pm_type == PM_DEAD ) {
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
NEI4 $319
line 892
;892:		pm.tracemask = MASK_PLAYERSOLID & ~CONTENTS_BODY;
ADDRLP4 4+28
CNSTI4 65537
ASGNI4
line 893
;893:	}
ADDRGP4 $320
JUMPV
LABELV $319
line 894
;894:	else if ( ent->r.svFlags & SVF_BOT ) {
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $322
line 895
;895:		pm.tracemask = MASK_PLAYERSOLID | CONTENTS_BOTCLIP;
ADDRLP4 4+28
CNSTI4 37814273
ASGNI4
line 896
;896:	}
ADDRGP4 $323
JUMPV
LABELV $322
line 897
;897:	else {
line 898
;898:		pm.tracemask = MASK_PLAYERSOLID;
ADDRLP4 4+28
CNSTI4 33619969
ASGNI4
line 899
;899:	}
LABELV $323
LABELV $320
line 900
;900:	pm.trace = trap_Trace;
ADDRLP4 4+224
ADDRGP4 trap_Trace
ASGNP4
line 901
;901:	pm.pointcontents = trap_PointContents;
ADDRLP4 4+228
ADDRGP4 trap_PointContents
ASGNP4
line 902
;902:	pm.debugLevel = g_debugMove.integer;
ADDRLP4 4+32
ADDRGP4 g_debugMove+12
INDIRI4
ASGNI4
line 903
;903:	pm.noFootsteps = ( g_dmflags.integer & DF_NO_FOOTSTEPS ) > 0;
ADDRGP4 g_dmflags+12
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
LEI4 $333
ADDRLP4 276
CNSTI4 1
ASGNI4
ADDRGP4 $334
JUMPV
LABELV $333
ADDRLP4 276
CNSTI4 0
ASGNI4
LABELV $334
ADDRLP4 4+36
ADDRLP4 276
INDIRI4
ASGNI4
line 905
;904:
;905:	pm.pmove_fixed = pmove_fixed.integer | client->pers.pmoveFixed;
ADDRLP4 4+216
ADDRGP4 pmove_fixed+12
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
INDIRI4
BORI4
ASGNI4
line 906
;906:	pm.pmove_msec = pmove_msec.integer;
ADDRLP4 4+220
ADDRGP4 pmove_msec+12
INDIRI4
ASGNI4
line 908
;907:
;908:	VectorCopy( client->ps.origin, client->oldOrigin );
ADDRLP4 0
INDIRP4
CNSTI4 668
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 925
;909:
;910:#ifdef MISSIONPACK
;911:		if (level.intermissionQueued != 0 && g_singlePlayer.integer) {
;912:			if ( level.time - level.intermissionQueued >= 1000  ) {
;913:				pm.cmd.buttons = 0;
;914:				pm.cmd.forwardmove = 0;
;915:				pm.cmd.rightmove = 0;
;916:				pm.cmd.upmove = 0;
;917:				if ( level.time - level.intermissionQueued >= 2000 && level.time - level.intermissionQueued <= 2500 ) {
;918:					trap_SendConsoleCommand( EXEC_APPEND, "centerview\n");
;919:				}
;920:				ent->client->ps.pm_type = PM_SPINTERMISSION;
;921:			}
;922:		}
;923:		Pmove (&pm);
;924:#else
;925:		Pmove (&pm);
ADDRLP4 4
ARGP4
ADDRGP4 Pmove
CALLV
pop
line 929
;926:#endif
;927:
;928:	// save results of pmove
;929:	if ( ent->client->ps.eventSequence != oldEventSequence ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ADDRLP4 244
INDIRI4
EQI4 $339
line 930
;930:		ent->eventTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 552
ADDP4
ADDRGP4 level+36
INDIRI4
ASGNI4
line 931
;931:	}
LABELV $339
line 932
;932:	if (g_smoothClients.integer) {
ADDRGP4 g_smoothClients+12
INDIRI4
CNSTI4 0
EQI4 $342
line 933
;933:		BG_PlayerStateToEntityStateExtraPolate( &ent->client->ps, &ent->s, ent->client->ps.commandTime, qtrue );
ADDRLP4 284
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 288
ADDRLP4 284
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 284
INDIRP4
ARGP4
ADDRLP4 288
INDIRP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityStateExtraPolate
CALLV
pop
line 934
;934:	}
ADDRGP4 $343
JUMPV
LABELV $342
line 935
;935:	else {
line 936
;936:		BG_PlayerStateToEntityState( &ent->client->ps, &ent->s, qtrue );
ADDRLP4 284
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 284
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
ADDRLP4 284
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 937
;937:	}
LABELV $343
line 938
;938:	SendPendingPredictableEvents( &ent->client->ps );
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
ADDRGP4 SendPendingPredictableEvents
CALLV
pop
line 940
;939:
;940:	if ( !( ent->client->ps.eFlags & EF_FIRING ) ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
NEI4 $345
line 941
;941:		client->fireHeld = qfalse;		// for grapple
ADDRLP4 0
INDIRP4
CNSTI4 768
ADDP4
CNSTI4 0
ASGNI4
line 942
;942:	}
LABELV $345
line 945
;943:
;944:	// use the snapped origin for linking so it matches client predicted versions
;945:	VectorCopy( ent->s.pos.trBase, ent->r.currentOrigin );
ADDRLP4 284
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 284
INDIRP4
CNSTI4 488
ADDP4
ADDRLP4 284
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 947
;946:
;947:	VectorCopy (pm.mins, ent->r.mins);
ADDRFP4 0
INDIRP4
CNSTI4 436
ADDP4
ADDRLP4 4+180
INDIRB
ASGNB 12
line 948
;948:	VectorCopy (pm.maxs, ent->r.maxs);
ADDRFP4 0
INDIRP4
CNSTI4 448
ADDP4
ADDRLP4 4+192
INDIRB
ASGNB 12
line 950
;949:
;950:	ent->waterlevel = pm.waterlevel;
ADDRFP4 0
INDIRP4
CNSTI4 788
ADDP4
ADDRLP4 4+208
INDIRI4
ASGNI4
line 951
;951:	ent->watertype = pm.watertype;
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
ADDRLP4 4+204
INDIRI4
ASGNI4
line 954
;952:
;953:	// execute client events
;954:	ClientEvents( ent, oldEventSequence );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 244
INDIRI4
ARGI4
ADDRGP4 ClientEvents
CALLV
pop
line 957
;955:
;956:	// link entity now, after any personal teleporters have been used
;957:	trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 958
;958:	if ( !ent->client->noclip ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 648
ADDP4
INDIRI4
CNSTI4 0
NEI4 $351
line 959
;959:		G_TouchTriggers( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_TouchTriggers
CALLV
pop
line 960
;960:	}
LABELV $351
line 963
;961:
;962:	// NOTE: now copy the exact origin over otherwise clients can be snapped into solid
;963:	VectorCopy( ent->client->ps.origin, ent->r.currentOrigin );
ADDRLP4 288
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 288
INDIRP4
CNSTI4 488
ADDP4
ADDRLP4 288
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 966
;964:
;965:	//test for solid areas in the AAS file
;966:	BotTestAAS(ent->r.currentOrigin);
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ARGP4
ADDRGP4 BotTestAAS
CALLV
pop
line 969
;967:
;968:	// touch other objects
;969:	ClientImpacts( ent, &pm );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 ClientImpacts
CALLV
pop
line 972
;970:
;971:	// save results of triggers and client events
;972:	if (ent->client->ps.eventSequence != oldEventSequence) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ADDRLP4 244
INDIRI4
EQI4 $353
line 973
;973:		ent->eventTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 552
ADDP4
ADDRGP4 level+36
INDIRI4
ASGNI4
line 974
;974:	}
LABELV $353
line 977
;975:
;976:	// swap and latch button actions
;977:	client->oldbuttons = client->buttons;
ADDRLP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
INDIRI4
ASGNI4
line 978
;978:	client->buttons = ucmd->buttons;
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
ADDRLP4 236
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 979
;979:	client->latched_buttons |= client->buttons & ~client->oldbuttons;
ADDRLP4 300
ADDRLP4 0
INDIRP4
CNSTI4 664
ADDP4
ASGNP4
ADDRLP4 300
INDIRP4
ADDRLP4 300
INDIRP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 660
ADDP4
INDIRI4
BCOMI4
BANDI4
BORI4
ASGNI4
line 982
;980:
;981:	// check for respawning
;982:	if ( client->ps.stats[STAT_HEALTH] <= 0 ) {
ADDRLP4 0
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 0
GTI4 $356
line 984
;983:		// wait for the attack button to be pressed
;984:		if ( level.time > client->respawnTime ) {
ADDRGP4 level+36
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 744
ADDP4
INDIRI4
LEI4 $260
line 986
;985:			// forcerespawn is to prevent users from waiting out powerups
;986:			if ( g_forcerespawn.integer > 0 && 
ADDRGP4 g_forcerespawn+12
INDIRI4
CNSTI4 0
LEI4 $361
ADDRGP4 level+36
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 744
ADDP4
INDIRI4
SUBI4
CNSTI4 1000
ADDRGP4 g_forcerespawn+12
INDIRI4
MULI4
LEI4 $361
line 987
;987:				( level.time - client->respawnTime ) > g_forcerespawn.integer * 1000 ) {
line 988
;988:				respawn( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 respawn
CALLV
pop
line 989
;989:				return;
ADDRGP4 $260
JUMPV
LABELV $361
line 993
;990:			}
;991:		
;992:			// pressing attack or use is the normal respawn method
;993:			if ( ucmd->buttons & ( BUTTON_ATTACK | BUTTON_USE_HOLDABLE ) ) {
ADDRLP4 236
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 5
BANDI4
CNSTI4 0
EQI4 $260
line 994
;994:				respawn( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 respawn
CALLV
pop
line 995
;995:			}
line 996
;996:		}
line 997
;997:		return;
ADDRGP4 $260
JUMPV
LABELV $356
line 1001
;998:	}
;999:
;1000:	// perform once-a-second actions
;1001:	ClientTimerActions( ent, msec );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 240
INDIRI4
ARGI4
ADDRGP4 ClientTimerActions
CALLV
pop
line 1002
;1002:}
LABELV $260
endproc ClientThink_real 304 16
export ClientThink
proc ClientThink 8 8
line 1011
;1003:
;1004:/*
;1005:==================
;1006:ClientThink
;1007:
;1008:A new command has arrived from the client
;1009:==================
;1010:*/
;1011:void ClientThink( int clientNum ) {
line 1014
;1012:	gentity_t *ent;
;1013:
;1014:	ent = g_entities + clientNum;
ADDRLP4 0
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1015
;1015:	trap_GetUsercmd( clientNum, &ent->client->pers.cmd );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
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
line 1019
;1016:
;1017:	// mark the time we got info, so we can display the
;1018:	// phone jack if they don't get any for a while
;1019:	ent->client->lastCmdTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 level+36
INDIRI4
ASGNI4
line 1021
;1020:
;1021:	if ( !(ent->r.svFlags & SVF_BOT) && !g_synchronousClients.integer ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 4
INDIRI4
NEI4 $370
ADDRGP4 g_synchronousClients+12
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $370
line 1022
;1022:		ClientThink_real( ent );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 ClientThink_real
CALLV
pop
line 1023
;1023:	}
LABELV $370
line 1024
;1024:}
LABELV $368
endproc ClientThink 8 8
export G_RunClient
proc G_RunClient 4 4
line 1027
;1025:
;1026:
;1027:void G_RunClient( gentity_t *ent ) {
line 1028
;1028:	if ( !(ent->r.svFlags & SVF_BOT) && !g_synchronousClients.integer ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 0
INDIRI4
NEI4 $374
ADDRGP4 g_synchronousClients+12
INDIRI4
ADDRLP4 0
INDIRI4
NEI4 $374
line 1029
;1029:		return;
ADDRGP4 $373
JUMPV
LABELV $374
line 1031
;1030:	}
;1031:	ent->client->pers.cmd.serverTime = level.time;
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
line 1032
;1032:	ClientThink_real( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ClientThink_real
CALLV
pop
line 1033
;1033:}
LABELV $373
endproc G_RunClient 4 4
export SpectatorClientEndFrame
proc SpectatorClientEndFrame 24 4
line 1042
;1034:
;1035:
;1036:/*
;1037:==================
;1038:SpectatorClientEndFrame
;1039:
;1040:==================
;1041:*/
;1042:void SpectatorClientEndFrame( gentity_t *ent ) {
line 1046
;1043:	gclient_t	*cl;
;1044:
;1045:	// if we are doing a chase cam or a remote view, grab the latest info
;1046:	if ( ent->client->sess.spectatorState == SPECTATOR_FOLLOW ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 2
NEI4 $379
line 1049
;1047:		int		clientNum, flags;
;1048:
;1049:		clientNum = ent->client->sess.spectatorClient;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ASGNI4
line 1052
;1050:
;1051:		// team follow1 and team follow2 go to whatever clients are playing
;1052:		if ( clientNum == -1 ) {
ADDRLP4 4
INDIRI4
CNSTI4 -1
NEI4 $381
line 1053
;1053:			clientNum = level.follow1;
ADDRLP4 4
ADDRGP4 level+344
INDIRI4
ASGNI4
line 1054
;1054:		} else if ( clientNum == -2 ) {
ADDRGP4 $382
JUMPV
LABELV $381
ADDRLP4 4
INDIRI4
CNSTI4 -2
NEI4 $384
line 1055
;1055:			clientNum = level.follow2;
ADDRLP4 4
ADDRGP4 level+348
INDIRI4
ASGNI4
line 1056
;1056:		}
LABELV $384
LABELV $382
line 1057
;1057:		if ( clientNum >= 0 ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $387
line 1058
;1058:			cl = &level.clients[ clientNum ];
ADDRLP4 0
CNSTI4 788
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 1059
;1059:			if ( cl->pers.connected == CON_CONNECTED && cl->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRLP4 12
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
NEI4 $389
ADDRLP4 12
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
EQI4 $389
line 1060
;1060:				flags = (cl->ps.eFlags & ~(EF_VOTED | EF_TEAMVOTED)) | (ent->client->ps.eFlags & (EF_VOTED | EF_TEAMVOTED));
ADDRLP4 16
CNSTI4 104
ASGNI4
ADDRLP4 8
ADDRLP4 0
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRI4
CNSTI4 -540673
BANDI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRI4
CNSTI4 540672
BANDI4
BORI4
ASGNI4
line 1061
;1061:				ent->client->ps = cl->ps;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ADDRLP4 0
INDIRP4
INDIRB
ASGNB 468
line 1062
;1062:				ent->client->ps.pm_flags |= PMF_FOLLOW;
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 1063
;1063:				ent->client->ps.eFlags = flags;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 1064
;1064:				return;
ADDRGP4 $378
JUMPV
LABELV $389
line 1065
;1065:			} else {
line 1067
;1066:				// drop them to free spectators unless they are dedicated camera followers
;1067:				if ( ent->client->sess.spectatorClient >= 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
CNSTI4 0
LTI4 $391
line 1068
;1068:					ent->client->sess.spectatorState = SPECTATOR_FREE;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 624
ADDP4
CNSTI4 1
ASGNI4
line 1069
;1069:					ClientBegin( ent->client - level.clients );
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
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
ADDRGP4 ClientBegin
CALLV
pop
line 1070
;1070:				}
LABELV $391
line 1071
;1071:			}
line 1072
;1072:		}
LABELV $387
line 1073
;1073:	}
LABELV $379
line 1075
;1074:
;1075:	if ( ent->client->sess.spectatorState == SPECTATOR_SCOREBOARD ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 3
NEI4 $393
line 1076
;1076:		ent->client->ps.pm_flags |= PMF_SCOREBOARD;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 12
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
line 1077
;1077:	} else {
ADDRGP4 $394
JUMPV
LABELV $393
line 1078
;1078:		ent->client->ps.pm_flags &= ~PMF_SCOREBOARD;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -8193
BANDI4
ASGNI4
line 1079
;1079:	}
LABELV $394
line 1080
;1080:}
LABELV $378
endproc SpectatorClientEndFrame 24 4
export ClientEndFrame
proc ClientEndFrame 20 16
line 1091
;1081:
;1082:/*
;1083:==============
;1084:ClientEndFrame
;1085:
;1086:Called at the end of each server frame for each connected client
;1087:A fast client will have multiple ClientThink for each ClientEdFrame,
;1088:while a slow client may have multiple ClientEndFrame between ClientThink.
;1089:==============
;1090:*/
;1091:void ClientEndFrame( gentity_t *ent ) {
line 1095
;1092:	int			i;
;1093:	clientPersistant_t	*pers;
;1094:
;1095:	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $396
line 1096
;1096:		SpectatorClientEndFrame( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 SpectatorClientEndFrame
CALLV
pop
line 1097
;1097:		return;
ADDRGP4 $395
JUMPV
LABELV $396
line 1100
;1098:	}
;1099:
;1100:	pers = &ent->client->pers;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 468
ADDP4
ASGNP4
line 1103
;1101:
;1102:	// turn off any expired powerups
;1103:	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $398
line 1104
;1104:		if ( ent->client->ps.powerups[ i ] < level.time ) {
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
GEI4 $402
line 1105
;1105:			ent->client->ps.powerups[ i ] = 0;
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
CNSTI4 0
ASGNI4
line 1106
;1106:		}
LABELV $402
line 1107
;1107:	}
LABELV $399
line 1103
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $398
line 1140
;1108:
;1109:#ifdef MISSIONPACK
;1110:	// set powerup for player animation
;1111:	if( bg_itemlist[ent->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD ) {
;1112:		ent->client->ps.powerups[PW_GUARD] = level.time;
;1113:	}
;1114:	if( bg_itemlist[ent->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_SCOUT ) {
;1115:		ent->client->ps.powerups[PW_SCOUT] = level.time;
;1116:	}
;1117:	if( bg_itemlist[ent->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_DOUBLER ) {
;1118:		ent->client->ps.powerups[PW_DOUBLER] = level.time;
;1119:	}
;1120:	if( bg_itemlist[ent->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_AMMOREGEN ) {
;1121:		ent->client->ps.powerups[PW_AMMOREGEN] = level.time;
;1122:	}
;1123:	if ( ent->client->invulnerabilityTime > level.time ) {
;1124:		ent->client->ps.powerups[PW_INVULNERABILITY] = level.time;
;1125:	}
;1126:#endif
;1127:
;1128:	// save network bandwidth
;1129:#if 0
;1130:	if ( !g_synchronousClients->integer && ent->client->ps.pm_type == PM_NORMAL ) {
;1131:		// FIXME: this must change eventually for non-sync demo recording
;1132:		VectorClear( ent->client->ps.viewangles );
;1133:	}
;1134:#endif
;1135:
;1136:	//
;1137:	// If the end of unit layout is displayed, don't give
;1138:	// the player any normal movement attributes
;1139:	//
;1140:	if ( level.intermissiontime ) {
ADDRGP4 level+9132
INDIRI4
CNSTI4 0
EQI4 $405
line 1141
;1141:		return;
ADDRGP4 $395
JUMPV
LABELV $405
line 1145
;1142:	}
;1143:
;1144:	// burn from lava, etc
;1145:	P_WorldEffects (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 P_WorldEffects
CALLV
pop
line 1148
;1146:
;1147:	// apply all the damage taken this frame
;1148:	P_DamageFeedback (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 P_DamageFeedback
CALLV
pop
line 1151
;1149:
;1150:	// add the EF_CONNECTION flag if we haven't gotten commands recently
;1151:	if ( level.time - ent->client->lastCmdTime > 1000 ) {
ADDRGP4 level+36
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 652
ADDP4
INDIRI4
SUBI4
CNSTI4 1000
LEI4 $408
line 1152
;1152:		ent->s.eFlags |= EF_CONNECTION;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 8192
BORI4
ASGNI4
line 1153
;1153:	} else {
ADDRGP4 $409
JUMPV
LABELV $408
line 1154
;1154:		ent->s.eFlags &= ~EF_CONNECTION;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 -8193
BANDI4
ASGNI4
line 1155
;1155:	}
LABELV $409
line 1157
;1156:
;1157:	ent->client->ps.stats[STAT_HEALTH] = ent->health;	// FIXME: get rid of ent->health...
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 184
ADDP4
ADDRLP4 8
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ASGNI4
line 1159
;1158:
;1159:	G_SetClientSound (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_SetClientSound
CALLV
pop
line 1162
;1160:
;1161:	// set the latest infor
;1162:	if (g_smoothClients.integer) {
ADDRGP4 g_smoothClients+12
INDIRI4
CNSTI4 0
EQI4 $411
line 1163
;1163:		BG_PlayerStateToEntityStateExtraPolate( &ent->client->ps, &ent->s, ent->client->ps.commandTime, qtrue );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 12
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityStateExtraPolate
CALLV
pop
line 1164
;1164:	}
ADDRGP4 $412
JUMPV
LABELV $411
line 1165
;1165:	else {
line 1166
;1166:		BG_PlayerStateToEntityState( &ent->client->ps, &ent->s, qtrue );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 1167
;1167:	}
LABELV $412
line 1168
;1168:	SendPendingPredictableEvents( &ent->client->ps );
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
ADDRGP4 SendPendingPredictableEvents
CALLV
pop
line 1173
;1169:
;1170:	// set the bit for the reachability area the client is currently in
;1171://	i = trap_AAS_PointReachabilityAreaIndex( ent->client->ps.origin );
;1172://	ent->client->areabits[i >> 3] |= 1 << (i & 7);
;1173:}
LABELV $395
endproc ClientEndFrame 20 16
import BotTestSolid
bss
export speed
align 4
LABELV speed
skip 4
export health
align 4
LABELV health
skip 4
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
import AddScore
import bash_die
import player_die
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
LABELV $283
byte 1 51
byte 1 51
byte 1 0
align 1
LABELV $279
byte 1 56
byte 1 0
align 1
LABELV $278
byte 1 112
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 95
byte 1 109
byte 1 115
byte 1 101
byte 1 99
byte 1 0
align 1
LABELV $200
byte 1 99
byte 1 112
byte 1 32
byte 1 34
byte 1 84
byte 1 101
byte 1 110
byte 1 32
byte 1 115
byte 1 101
byte 1 99
byte 1 111
byte 1 110
byte 1 100
byte 1 115
byte 1 32
byte 1 117
byte 1 110
byte 1 116
byte 1 105
byte 1 108
byte 1 32
byte 1 105
byte 1 110
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 105
byte 1 116
byte 1 121
byte 1 32
byte 1 100
byte 1 114
byte 1 111
byte 1 112
byte 1 33
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $196
byte 1 68
byte 1 114
byte 1 111
byte 1 112
byte 1 112
byte 1 101
byte 1 100
byte 1 32
byte 1 100
byte 1 117
byte 1 101
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 105
byte 1 110
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 105
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $92
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 47
byte 1 103
byte 1 117
byte 1 114
byte 1 112
byte 1 50
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $91
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 47
byte 1 103
byte 1 117
byte 1 114
byte 1 112
byte 1 49
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $88
byte 1 42
byte 1 100
byte 1 114
byte 1 111
byte 1 119
byte 1 110
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
