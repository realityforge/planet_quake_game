export CG_InitLocalEntities
code
proc CG_InitLocalEntities 12 12
file "../cg_localents.c"
line 21
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:
;4:// cg_localents.c -- every frame, generate renderer commands for locally
;5:// processed entities, like smoke puffs, gibs, shells, etc.
;6:
;7:#include "cg_local.h"
;8:
;9:#define	MAX_LOCAL_ENTITIES	512
;10:localEntity_t	cg_localEntities[MAX_LOCAL_ENTITIES];
;11:localEntity_t	cg_activeLocalEntities;		// double linked list
;12:localEntity_t	*cg_freeLocalEntities;		// single linked list
;13:
;14:/*
;15:===================
;16:CG_InitLocalEntities
;17:
;18:This is called at startup and for tournement restarts
;19:===================
;20:*/
;21:void	CG_InitLocalEntities( void ) {
line 24
;22:	int		i;
;23:
;24:	memset( cg_localEntities, 0, sizeof( cg_localEntities ) );
ADDRGP4 cg_localEntities
ARGP4
CNSTI4 0
ARGI4
CNSTI4 149504
ARGI4
ADDRGP4 memset
CALLP4
pop
line 25
;25:	cg_activeLocalEntities.next = &cg_activeLocalEntities;
ADDRGP4 cg_activeLocalEntities+4
ADDRGP4 cg_activeLocalEntities
ASGNP4
line 26
;26:	cg_activeLocalEntities.prev = &cg_activeLocalEntities;
ADDRLP4 4
ADDRGP4 cg_activeLocalEntities
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
ASGNP4
line 27
;27:	cg_freeLocalEntities = cg_localEntities;
ADDRGP4 cg_freeLocalEntities
ADDRGP4 cg_localEntities
ASGNP4
line 28
;28:	for ( i = 0 ; i < MAX_LOCAL_ENTITIES - 1 ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $72
line 29
;29:		cg_localEntities[i].next = &cg_localEntities[i+1];
ADDRLP4 8
CNSTI4 292
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 8
INDIRI4
ADDRGP4 cg_localEntities+4
ADDP4
ADDRLP4 8
INDIRI4
ADDRGP4 cg_localEntities+292
ADDP4
ASGNP4
line 30
;30:	}
LABELV $73
line 28
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 511
LTI4 $72
line 31
;31:}
LABELV $70
endproc CG_InitLocalEntities 12 12
export CG_FreeLocalEntity
proc CG_FreeLocalEntity 12 4
line 39
;32:
;33:
;34:/*
;35:==================
;36:CG_FreeLocalEntity
;37:==================
;38:*/
;39:void CG_FreeLocalEntity( localEntity_t *le ) {
line 40
;40:	if ( !le->prev ) {
ADDRFP4 0
INDIRP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $79
line 41
;41:		CG_Error( "CG_FreeLocalEntity: not active" );
ADDRGP4 $81
ARGP4
ADDRGP4 CG_Error
CALLV
pop
line 42
;42:	}
LABELV $79
line 45
;43:
;44:	// remove from the doubly linked active list
;45:	le->prev->next = le->next;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTI4 4
ASGNI4
ADDRLP4 0
INDIRP4
INDIRP4
ADDRLP4 4
INDIRI4
ADDP4
ADDRLP4 0
INDIRP4
ADDRLP4 4
INDIRI4
ADDP4
INDIRP4
ASGNP4
line 46
;46:	le->next->prev = le->prev;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
ADDRLP4 8
INDIRP4
INDIRP4
ASGNP4
line 49
;47:
;48:	// the free list is only singly linked
;49:	le->next = cg_freeLocalEntities;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRGP4 cg_freeLocalEntities
INDIRP4
ASGNP4
line 50
;50:	cg_freeLocalEntities = le;
ADDRGP4 cg_freeLocalEntities
ADDRFP4 0
INDIRP4
ASGNP4
line 51
;51:}
LABELV $78
endproc CG_FreeLocalEntity 12 4
export CG_AllocLocalEntity
proc CG_AllocLocalEntity 8 12
line 60
;52:
;53:/*
;54:===================
;55:CG_AllocLocalEntity
;56:
;57:Will allways succeed, even if it requires freeing an old active entity
;58:===================
;59:*/
;60:localEntity_t	*CG_AllocLocalEntity( void ) {
line 63
;61:	localEntity_t	*le;
;62:
;63:	if ( !cg_freeLocalEntities ) {
ADDRGP4 cg_freeLocalEntities
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $83
line 66
;64:		// no free entities, so free the one at the end of the chain
;65:		// remove the oldest active entity
;66:		CG_FreeLocalEntity( cg_activeLocalEntities.prev );
ADDRGP4 cg_activeLocalEntities
INDIRP4
ARGP4
ADDRGP4 CG_FreeLocalEntity
CALLV
pop
line 67
;67:	}
LABELV $83
line 69
;68:
;69:	le = cg_freeLocalEntities;
ADDRLP4 0
ADDRGP4 cg_freeLocalEntities
INDIRP4
ASGNP4
line 70
;70:	cg_freeLocalEntities = cg_freeLocalEntities->next;
ADDRLP4 4
ADDRGP4 cg_freeLocalEntities
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
ASGNP4
line 72
;71:
;72:	memset( le, 0, sizeof( *le ) );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 292
ARGI4
ADDRGP4 memset
CALLP4
pop
line 75
;73:
;74:	// link into the active list
;75:	le->next = cg_activeLocalEntities.next;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRGP4 cg_activeLocalEntities+4
INDIRP4
ASGNP4
line 76
;76:	le->prev = &cg_activeLocalEntities;
ADDRLP4 0
INDIRP4
ADDRGP4 cg_activeLocalEntities
ASGNP4
line 77
;77:	cg_activeLocalEntities.next->prev = le;
ADDRGP4 cg_activeLocalEntities+4
INDIRP4
ADDRLP4 0
INDIRP4
ASGNP4
line 78
;78:	cg_activeLocalEntities.next = le;
ADDRGP4 cg_activeLocalEntities+4
ADDRLP4 0
INDIRP4
ASGNP4
line 79
;79:	return le;
ADDRLP4 0
INDIRP4
RETP4
LABELV $82
endproc CG_AllocLocalEntity 8 12
export CG_BloodTrail
proc CG_BloodTrail 48 48
line 101
;80:}
;81:
;82:
;83:/*
;84:====================================================================================
;85:
;86:FRAGMENT PROCESSING
;87:
;88:A fragment localentity interacts with the environment in some way (hitting walls),
;89:or generates more localentities along a trail.
;90:
;91:====================================================================================
;92:*/
;93:
;94:/*
;95:================
;96:CG_BloodTrail
;97:
;98:Leave expanding blood puffs behind gibs
;99:================
;100:*/
;101:void CG_BloodTrail( localEntity_t *le ) {
line 108
;102:	int		t;
;103:	int		t2;
;104:	int		step;
;105:	vec3_t	newOrigin;
;106:	localEntity_t	*blood;
;107:
;108:	step = 150;
ADDRLP4 20
CNSTI4 150
ASGNI4
line 109
;109:	t = step * ( (cg.time - cg.frametime + step ) / step );
ADDRLP4 0
ADDRLP4 20
INDIRI4
ADDRGP4 cg+107604
INDIRI4
ADDRGP4 cg+107600
INDIRI4
SUBI4
ADDRLP4 20
INDIRI4
ADDI4
ADDRLP4 20
INDIRI4
DIVI4
MULI4
ASGNI4
line 110
;110:	t2 = step * ( cg.time / step );
ADDRLP4 24
ADDRLP4 20
INDIRI4
ADDRGP4 cg+107604
INDIRI4
ADDRLP4 20
INDIRI4
DIVI4
MULI4
ASGNI4
line 112
;111:
;112:	for ( ; t <= t2; t += step ) {
ADDRGP4 $95
JUMPV
LABELV $92
line 113
;113:		BG_EvaluateTrajectory( &le->pos, t, newOrigin );
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
ADDRGP4 BG_EvaluateTrajectory
CALLV
pop
line 115
;114:
;115:		blood = CG_SmokePuff( newOrigin, vec3_origin, 
ADDRLP4 8
ARGP4
ADDRGP4 vec3_origin
ARGP4
CNSTF4 1101004800
ARGF4
ADDRLP4 36
CNSTF4 1065353216
ASGNF4
ADDRLP4 36
INDIRF4
ARGF4
ADDRLP4 36
INDIRF4
ARGF4
ADDRLP4 36
INDIRF4
ARGF4
ADDRLP4 36
INDIRF4
ARGF4
CNSTF4 1157234688
ARGF4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 40
CNSTI4 0
ASGNI4
ADDRLP4 40
INDIRI4
ARGI4
ADDRLP4 40
INDIRI4
ARGI4
ADDRGP4 cgs+152340+296
INDIRI4
ARGI4
ADDRLP4 44
ADDRGP4 CG_SmokePuff
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 44
INDIRP4
ASGNP4
line 124
;116:					  20,		// radius
;117:					  1, 1, 1, 1,	// color
;118:					  2000,		// trailTime
;119:					  t,		// startTime
;120:					  0,		// fadeInTime
;121:					  0,		// flags
;122:					  cgs.media.bloodTrailShader );
;123:		// use the optimized version
;124:		blood->leType = LE_FALL_SCALE_FADE;
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 5
ASGNI4
line 126
;125:		// drop a total of 40 units over its lifetime
;126:		blood->pos.trDelta[2] = 40;
ADDRLP4 4
INDIRP4
CNSTI4 64
ADDP4
CNSTF4 1109393408
ASGNF4
line 127
;127:	}
LABELV $93
line 112
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 20
INDIRI4
ADDI4
ASGNI4
LABELV $95
ADDRLP4 0
INDIRI4
ADDRLP4 24
INDIRI4
LEI4 $92
line 128
;128:}
LABELV $88
endproc CG_BloodTrail 48 48
export CG_FragmentBounceMark
proc CG_FragmentBounceMark 20 44
line 136
;129:
;130:
;131:/*
;132:================
;133:CG_FragmentBounceMark
;134:================
;135:*/
;136:void CG_FragmentBounceMark( localEntity_t *le, trace_t *trace ) {
line 139
;137:	int			radius;
;138:
;139:	if ( le->leMarkType == LEMT_BLOOD ) {
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
CNSTI4 2
NEI4 $99
line 141
;140:
;141:		radius = 16 + (rand()&31);
ADDRLP4 4
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
CNSTI4 31
BANDI4
CNSTI4 16
ADDI4
ASGNI4
line 142
;142:		CG_ImpactMark( cgs.media.bloodMarkShader, trace->endpos, trace->plane.normal, random()*360,
ADDRLP4 8
ADDRGP4 rand
CALLI4
ASGNI4
ADDRGP4 cgs+152340+372
INDIRI4
ARGI4
ADDRLP4 12
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRLP4 12
INDIRP4
CNSTI4 24
ADDP4
ARGP4
CNSTF4 1135869952
ADDRLP4 8
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ARGF4
ADDRLP4 16
CNSTF4 1065353216
ASGNF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
CNSTI4 1
ARGI4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
CNSTI4 0
ARGI4
ADDRGP4 CG_ImpactMark
CALLV
pop
line 144
;143:			1,1,1,1, qtrue, radius, qfalse );
;144:	} else if ( le->leMarkType == LEMT_BURN ) {
ADDRGP4 $100
JUMPV
LABELV $99
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
CNSTI4 1
NEI4 $103
line 146
;145:
;146:		radius = 8 + (rand()&15);
ADDRLP4 4
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
CNSTI4 15
BANDI4
CNSTI4 8
ADDI4
ASGNI4
line 147
;147:		CG_ImpactMark( cgs.media.burnMarkShader, trace->endpos, trace->plane.normal, random()*360,
ADDRLP4 8
ADDRGP4 rand
CALLI4
ASGNI4
ADDRGP4 cgs+152340+380
INDIRI4
ARGI4
ADDRLP4 12
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRLP4 12
INDIRP4
CNSTI4 24
ADDP4
ARGP4
CNSTF4 1135869952
ADDRLP4 8
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ARGF4
ADDRLP4 16
CNSTF4 1065353216
ASGNF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
CNSTI4 1
ARGI4
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
CNSTI4 0
ARGI4
ADDRGP4 CG_ImpactMark
CALLV
pop
line 149
;148:			1,1,1,1, qtrue, radius, qfalse );
;149:	}
LABELV $103
LABELV $100
line 154
;150:
;151:
;152:	// don't allow a fragment to make multiple marks, or they
;153:	// pile up while settling
;154:	le->leMarkType = LEMT_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 0
ASGNI4
line 155
;155:}
LABELV $98
endproc CG_FragmentBounceMark 20 44
export CG_FragmentBounceSound
proc CG_FragmentBounceSound 16 16
line 162
;156:
;157:/*
;158:================
;159:CG_FragmentBounceSound
;160:================
;161:*/
;162:void CG_FragmentBounceSound( localEntity_t *le, trace_t *trace ) {
line 163
;163:	if ( le->leBounceSoundType == LEBS_BLOOD ) {
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 1
NEI4 $108
line 165
;164:		// half the gibs will make splat sounds
;165:		if ( rand() & 1 ) {
ADDRLP4 0
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $109
line 166
;166:			int r = rand()&3;
ADDRLP4 12
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 12
INDIRI4
CNSTI4 3
BANDI4
ASGNI4
line 169
;167:			sfxHandle_t	s;
;168:
;169:			if ( r == 0 ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $112
line 170
;170:				s = cgs.media.gibBounce1Sound;
ADDRLP4 8
ADDRGP4 cgs+152340+700
INDIRI4
ASGNI4
line 171
;171:			} else if ( r == 1 ) {
ADDRGP4 $113
JUMPV
LABELV $112
ADDRLP4 4
INDIRI4
CNSTI4 1
NEI4 $116
line 172
;172:				s = cgs.media.gibBounce2Sound;
ADDRLP4 8
ADDRGP4 cgs+152340+704
INDIRI4
ASGNI4
line 173
;173:			} else {
ADDRGP4 $117
JUMPV
LABELV $116
line 174
;174:				s = cgs.media.gibBounce3Sound;
ADDRLP4 8
ADDRGP4 cgs+152340+708
INDIRI4
ASGNI4
line 175
;175:			}
LABELV $117
LABELV $113
line 176
;176:			trap_S_StartSound( trace->endpos, ENTITYNUM_WORLD, CHAN_AUTO, s );
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
ARGP4
CNSTI4 1022
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRGP4 trap_S_StartSound
CALLV
pop
line 177
;177:		}
line 178
;178:	} else if ( le->leBounceSoundType == LEBS_BRASS ) {
ADDRGP4 $109
JUMPV
LABELV $108
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 2
NEI4 $122
line 180
;179:
;180:	}
LABELV $122
LABELV $109
line 184
;181:
;182:	// don't allow a fragment to make multiple bounce sounds,
;183:	// or it gets too noisy as they settle
;184:	le->leBounceSoundType = LEBS_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 0
ASGNI4
line 185
;185:}
LABELV $107
endproc CG_FragmentBounceSound 16 16
export CG_ReflectVelocity
proc CG_ReflectVelocity 56 12
line 193
;186:
;187:
;188:/*
;189:================
;190:CG_ReflectVelocity
;191:================
;192:*/
;193:void CG_ReflectVelocity( localEntity_t *le, trace_t *trace ) {
line 199
;194:	vec3_t	velocity;
;195:	float	dot;
;196:	int		hitTime;
;197:
;198:	// reflect the velocity on the trace plane
;199:	hitTime = cg.time - cg.frametime + cg.frametime * trace->fraction;
ADDRLP4 16
ADDRGP4 cg+107604
INDIRI4
ADDRGP4 cg+107600
INDIRI4
SUBI4
CVIF4 4
ADDRGP4 cg+107600
INDIRI4
CVIF4 4
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 200
;200:	BG_EvaluateTrajectoryDelta( &le->pos, hitTime, velocity );
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRLP4 16
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BG_EvaluateTrajectoryDelta
CALLV
pop
line 201
;201:	dot = DotProduct( velocity, trace->plane.normal );
ADDRLP4 20
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 0
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
MULF4
ADDRLP4 0+4
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 0+8
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 202
;202:	VectorMA( velocity, -2*dot, trace->plane.normal, le->pos.trDelta );
ADDRFP4 0
INDIRP4
CNSTI4 56
ADDP4
ADDRLP4 0
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
CNSTF4 3221225472
ADDRLP4 12
INDIRF4
MULF4
MULF4
ADDF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 0+4
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 3221225472
ADDRLP4 12
INDIRF4
MULF4
MULF4
ADDF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDRLP4 0+8
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
CNSTF4 3221225472
ADDRLP4 12
INDIRF4
MULF4
MULF4
ADDF4
ASGNF4
line 204
;203:
;204:	VectorScale( le->pos.trDelta, le->bounceFactor, le->pos.trDelta );
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
ADDRLP4 24
INDIRP4
CNSTI4 56
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRF4
ADDRLP4 24
INDIRP4
CNSTI4 104
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
ADDRLP4 32
INDIRP4
CNSTI4 60
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRF4
ADDRLP4 32
INDIRP4
CNSTI4 104
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
ADDRLP4 40
INDIRP4
CNSTI4 64
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRF4
ADDRLP4 40
INDIRP4
CNSTI4 104
ADDP4
INDIRF4
MULF4
ASGNF4
line 206
;205:
;206:	VectorCopy( trace->endpos, le->pos.trBase );
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRB
ASGNB 12
line 207
;207:	le->pos.trTime = cg.time;
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRGP4 cg+107604
INDIRI4
ASGNI4
line 211
;208:
;209:
;210:	// check for stop, making sure that even on low FPS systems it doesn't bobble
;211:	if ( trace->allsolid || 
ADDRLP4 48
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $137
ADDRLP4 48
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
CNSTF4 0
LEF4 $133
ADDRLP4 52
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ASGNF4
ADDRLP4 52
INDIRF4
CNSTF4 1109393408
LTF4 $137
ADDRLP4 52
INDIRF4
ADDRGP4 cg+107600
INDIRI4
NEGI4
CVIF4 4
ADDRLP4 52
INDIRF4
MULF4
GEF4 $133
LABELV $137
line 213
;212:		( trace->plane.normal[2] > 0 && 
;213:		( le->pos.trDelta[2] < 40 || le->pos.trDelta[2] < -cg.frametime * le->pos.trDelta[2] ) ) ) {
line 214
;214:		le->pos.trType = TR_STATIONARY;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
CNSTI4 0
ASGNI4
line 215
;215:	} else {
LABELV $133
line 217
;216:
;217:	}
LABELV $134
line 218
;218:}
LABELV $124
endproc CG_ReflectVelocity 56 12
export CG_AddFragment
proc CG_AddFragment 88 28
line 225
;219:
;220:/*
;221:================
;222:CG_AddFragment
;223:================
;224:*/
;225:void CG_AddFragment( localEntity_t *le ) {
line 229
;226:	vec3_t	newOrigin;
;227:	trace_t	trace;
;228:
;229:	if ( le->pos.trType == TR_STATIONARY ) {
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 0
NEI4 $139
line 234
;230:		// sink into the ground if near the removal time
;231:		int		t;
;232:		float	oldZ;
;233:		
;234:		t = le->endTime - cg.time;
ADDRLP4 68
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRGP4 cg+107604
INDIRI4
SUBI4
ASGNI4
line 235
;235:		if ( t < SINK_TIME ) {
ADDRLP4 68
INDIRI4
CNSTI4 1000
GEI4 $142
line 239
;236:			// we must use an explicit lighting origin, otherwise the
;237:			// lighting would be lost as soon as the origin went
;238:			// into the ground
;239:			VectorCopy( le->refEntity.origin, le->refEntity.lightingOrigin );
ADDRLP4 76
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 76
INDIRP4
CNSTI4 164
ADDP4
ADDRLP4 76
INDIRP4
CNSTI4 220
ADDP4
INDIRB
ASGNB 12
line 240
;240:			le->refEntity.renderfx |= RF_LIGHTING_ORIGIN;
ADDRLP4 80
ADDRFP4 0
INDIRP4
CNSTI4 156
ADDP4
ASGNP4
ADDRLP4 80
INDIRP4
ADDRLP4 80
INDIRP4
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 241
;241:			oldZ = le->refEntity.origin[2];
ADDRLP4 72
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRF4
ASGNF4
line 242
;242:			le->refEntity.origin[2] -= 16 * ( 1.0 - (float)t / SINK_TIME );
ADDRLP4 84
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRF4
CNSTF4 1098907648
CNSTF4 1065353216
ADDRLP4 68
INDIRI4
CVIF4 4
CNSTF4 1148846080
DIVF4
SUBF4
MULF4
SUBF4
ASGNF4
line 243
;243:			trap_R_AddRefEntityToScene( &le->refEntity );
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
ARGP4
ADDRGP4 trap_R_AddRefEntityToScene
CALLV
pop
line 244
;244:			le->refEntity.origin[2] = oldZ;
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
ADDRLP4 72
INDIRF4
ASGNF4
line 245
;245:		} else {
ADDRGP4 $138
JUMPV
LABELV $142
line 246
;246:			trap_R_AddRefEntityToScene( &le->refEntity );
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
ARGP4
ADDRGP4 trap_R_AddRefEntityToScene
CALLV
pop
line 247
;247:		}
line 249
;248:
;249:		return;
ADDRGP4 $138
JUMPV
LABELV $139
line 253
;250:	}
;251:
;252:	// calculate new position
;253:	BG_EvaluateTrajectory( &le->pos, cg.time, newOrigin );
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRGP4 cg+107604
INDIRI4
ARGI4
ADDRLP4 56
ARGP4
ADDRGP4 BG_EvaluateTrajectory
CALLV
pop
line 256
;254:
;255:	// trace a line from previous position to new position
;256:	CG_Trace( &trace, le->refEntity.origin, NULL, NULL, newOrigin, -1, CONTENTS_SOLID );
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
ARGP4
ADDRLP4 68
CNSTP4 0
ASGNP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 56
ARGP4
CNSTI4 -1
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 CG_Trace
CALLV
pop
line 257
;257:	if ( trace.fraction == 1.0 ) {
ADDRLP4 0+8
INDIRF4
CNSTF4 1065353216
NEF4 $145
line 259
;258:		// still in free fall
;259:		VectorCopy( newOrigin, le->refEntity.origin );
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
ADDRLP4 56
INDIRB
ASGNB 12
line 261
;260:
;261:		if ( le->leFlags & LEF_TUMBLE ) {
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $148
line 264
;262:			vec3_t angles;
;263:
;264:			BG_EvaluateTrajectory( &le->angles, cg.time, angles );
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ARGP4
ADDRGP4 cg+107604
INDIRI4
ARGI4
ADDRLP4 72
ARGP4
ADDRGP4 BG_EvaluateTrajectory
CALLV
pop
line 265
;265:			AnglesToAxis( angles, le->refEntity.axis );
ADDRLP4 72
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
ARGP4
ADDRGP4 AnglesToAxis
CALLV
pop
line 266
;266:		}
LABELV $148
line 268
;267:
;268:		trap_R_AddRefEntityToScene( &le->refEntity );
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
ARGP4
ADDRGP4 trap_R_AddRefEntityToScene
CALLV
pop
line 271
;269:
;270:		// add a blood trail
;271:		if ( le->leBounceSoundType == LEBS_BLOOD ) {
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 1
NEI4 $138
line 272
;272:			CG_BloodTrail( le );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CG_BloodTrail
CALLV
pop
line 273
;273:		}
line 275
;274:
;275:		return;
ADDRGP4 $138
JUMPV
LABELV $145
line 281
;276:	}
;277:
;278:	// if it is in a nodrop zone, remove it
;279:	// this keeps gibs from waiting at the bottom of pits of death
;280:	// and floating levels
;281:	if ( trap_CM_PointContents( trace.endpos, 0 ) & CONTENTS_NODROP ) {
ADDRLP4 0+12
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 72
ADDRGP4 trap_CM_PointContents
CALLI4
ASGNI4
ADDRLP4 72
INDIRI4
CVIU4 4
CNSTU4 2147483648
BANDU4
CNSTU4 0
EQU4 $153
line 282
;282:		CG_FreeLocalEntity( le );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CG_FreeLocalEntity
CALLV
pop
line 283
;283:		return;
ADDRGP4 $138
JUMPV
LABELV $153
line 287
;284:	}
;285:
;286:	// leave a mark
;287:	CG_FragmentBounceMark( le, &trace );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 CG_FragmentBounceMark
CALLV
pop
line 290
;288:
;289:	// do a bouncy sound
;290:	CG_FragmentBounceSound( le, &trace );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 CG_FragmentBounceSound
CALLV
pop
line 293
;291:
;292:	// reflect the velocity on the trace plane
;293:	CG_ReflectVelocity( le, &trace );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 CG_ReflectVelocity
CALLV
pop
line 295
;294:
;295:	trap_R_AddRefEntityToScene( &le->refEntity );
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
ARGP4
ADDRGP4 trap_R_AddRefEntityToScene
CALLV
pop
line 296
;296:}
LABELV $138
endproc CG_AddFragment 88 28
export CG_AddFadeRGB
proc CG_AddFadeRGB 60 4
line 312
;297:
;298:/*
;299:=====================================================================
;300:
;301:TRIVIAL LOCAL ENTITIES
;302:
;303:These only do simple scaling or modulation before passing to the renderer
;304:=====================================================================
;305:*/
;306:
;307:/*
;308:====================
;309:CG_AddFadeRGB
;310:====================
;311:*/
;312:void CG_AddFadeRGB( localEntity_t *le ) {
line 316
;313:	refEntity_t *re;
;314:	float c;
;315:
;316:	re = &le->refEntity;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
line 318
;317:
;318:	c = ( le->endTime - cg.time ) * le->lifeRate;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRGP4 cg+107604
INDIRI4
SUBI4
CVIF4 4
ADDRLP4 8
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
MULF4
ASGNF4
line 319
;319:	c *= 0xff;
ADDRLP4 4
CNSTF4 1132396544
ADDRLP4 4
INDIRF4
MULF4
ASGNF4
line 321
;320:
;321:	re->shaderRGBA[0] = le->color[0] * c;
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
ASGNF4
ADDRLP4 20
CNSTF4 1325400064
ASGNF4
ADDRLP4 16
INDIRF4
ADDRLP4 20
INDIRF4
LTF4 $159
ADDRLP4 12
ADDRLP4 16
INDIRF4
ADDRLP4 20
INDIRF4
SUBF4
CVFI4 4
CVIU4 4
CNSTU4 2147483648
ADDU4
ASGNU4
ADDRGP4 $160
JUMPV
LABELV $159
ADDRLP4 12
ADDRLP4 16
INDIRF4
CVFI4 4
CVIU4 4
ASGNU4
LABELV $160
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRLP4 12
INDIRU4
CVUU1 4
ASGNU1
line 322
;322:	re->shaderRGBA[1] = le->color[1] * c;
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
ASGNF4
ADDRLP4 32
CNSTF4 1325400064
ASGNF4
ADDRLP4 28
INDIRF4
ADDRLP4 32
INDIRF4
LTF4 $162
ADDRLP4 24
ADDRLP4 28
INDIRF4
ADDRLP4 32
INDIRF4
SUBF4
CVFI4 4
CVIU4 4
CNSTU4 2147483648
ADDU4
ASGNU4
ADDRGP4 $163
JUMPV
LABELV $162
ADDRLP4 24
ADDRLP4 28
INDIRF4
CVFI4 4
CVIU4 4
ASGNU4
LABELV $163
ADDRLP4 0
INDIRP4
CNSTI4 117
ADDP4
ADDRLP4 24
INDIRU4
CVUU1 4
ASGNU1
line 323
;323:	re->shaderRGBA[2] = le->color[2] * c;
ADDRLP4 40
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
ASGNF4
ADDRLP4 44
CNSTF4 1325400064
ASGNF4
ADDRLP4 40
INDIRF4
ADDRLP4 44
INDIRF4
LTF4 $165
ADDRLP4 36
ADDRLP4 40
INDIRF4
ADDRLP4 44
INDIRF4
SUBF4
CVFI4 4
CVIU4 4
CNSTU4 2147483648
ADDU4
ASGNU4
ADDRGP4 $166
JUMPV
LABELV $165
ADDRLP4 36
ADDRLP4 40
INDIRF4
CVFI4 4
CVIU4 4
ASGNU4
LABELV $166
ADDRLP4 0
INDIRP4
CNSTI4 118
ADDP4
ADDRLP4 36
INDIRU4
CVUU1 4
ASGNU1
line 324
;324:	re->shaderRGBA[3] = le->color[3] * c;
ADDRLP4 52
ADDRFP4 0
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
ASGNF4
ADDRLP4 56
CNSTF4 1325400064
ASGNF4
ADDRLP4 52
INDIRF4
ADDRLP4 56
INDIRF4
LTF4 $168
ADDRLP4 48
ADDRLP4 52
INDIRF4
ADDRLP4 56
INDIRF4
SUBF4
CVFI4 4
CVIU4 4
CNSTU4 2147483648
ADDU4
ASGNU4
ADDRGP4 $169
JUMPV
LABELV $168
ADDRLP4 48
ADDRLP4 52
INDIRF4
CVFI4 4
CVIU4 4
ASGNU4
LABELV $169
ADDRLP4 0
INDIRP4
CNSTI4 119
ADDP4
ADDRLP4 48
INDIRU4
CVUU1 4
ASGNU1
line 326
;325:
;326:	trap_R_AddRefEntityToScene( re );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_R_AddRefEntityToScene
CALLV
pop
line 327
;327:}
LABELV $156
endproc CG_AddFadeRGB 60 4
proc CG_AddMoveScaleFade 52 12
line 334
;328:
;329:/*
;330:==================
;331:CG_AddMoveScaleFade
;332:==================
;333:*/
;334:static void CG_AddMoveScaleFade( localEntity_t *le ) {
line 340
;335:	refEntity_t	*re;
;336:	float		c;
;337:	vec3_t		delta;
;338:	float		len;
;339:
;340:	re = &le->refEntity;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
line 342
;341:
;342:	if ( le->fadeInTime > le->startTime && cg.time < le->fadeInTime ) {
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
ADDRLP4 24
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
ADDRLP4 24
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
LEI4 $171
ADDRGP4 cg+107604
INDIRI4
ADDRLP4 28
INDIRI4
GEI4 $171
line 344
;343:		// fade / grow time
;344:		c = 1.0 - (float) ( le->fadeInTime - cg.time ) / ( le->fadeInTime - le->startTime );
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
ADDRLP4 32
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
ASGNI4
ADDRLP4 16
CNSTF4 1065353216
ADDRLP4 36
INDIRI4
ADDRGP4 cg+107604
INDIRI4
SUBI4
CVIF4 4
ADDRLP4 36
INDIRI4
ADDRLP4 32
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
SUBI4
CVIF4 4
DIVF4
SUBF4
ASGNF4
line 345
;345:	}
ADDRGP4 $172
JUMPV
LABELV $171
line 346
;346:	else {
line 348
;347:		// fade / grow time
;348:		c = ( le->endTime - cg.time ) * le->lifeRate;
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 32
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRGP4 cg+107604
INDIRI4
SUBI4
CVIF4 4
ADDRLP4 32
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
MULF4
ASGNF4
line 349
;349:	}
LABELV $172
line 351
;350:
;351:	re->shaderRGBA[3] = 0xff * c * le->color[3];
ADDRLP4 36
CNSTF4 1132396544
ADDRLP4 16
INDIRF4
MULF4
ADDRFP4 0
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 40
CNSTF4 1325400064
ASGNF4
ADDRLP4 36
INDIRF4
ADDRLP4 40
INDIRF4
LTF4 $177
ADDRLP4 32
ADDRLP4 36
INDIRF4
ADDRLP4 40
INDIRF4
SUBF4
CVFI4 4
CVIU4 4
CNSTU4 2147483648
ADDU4
ASGNU4
ADDRGP4 $178
JUMPV
LABELV $177
ADDRLP4 32
ADDRLP4 36
INDIRF4
CVFI4 4
CVIU4 4
ASGNU4
LABELV $178
ADDRLP4 0
INDIRP4
CNSTI4 119
ADDP4
ADDRLP4 32
INDIRU4
CVUU1 4
ASGNU1
line 353
;352:
;353:	if ( !( le->leFlags & LEF_PUFF_DONT_SCALE ) ) {
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $179
line 354
;354:		re->radius = le->radius * ( 1.0 - c ) + 8;
ADDRLP4 0
INDIRP4
CNSTI4 132
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
CNSTF4 1065353216
ADDRLP4 16
INDIRF4
SUBF4
MULF4
CNSTF4 1090519040
ADDF4
ASGNF4
line 355
;355:	}
LABELV $179
line 357
;356:
;357:	BG_EvaluateTrajectory( &le->pos, cg.time, re->origin );
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRGP4 cg+107604
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ARGP4
ADDRGP4 BG_EvaluateTrajectory
CALLV
pop
line 361
;358:
;359:	// if the view would be "inside" the sprite, kill the sprite
;360:	// so it doesn't add too much overdraw
;361:	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRF4
ADDRGP4 cg+109044+24
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
INDIRF4
ADDRGP4 cg+109044+24+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
INDIRF4
ADDRGP4 cg+109044+24+8
INDIRF4
SUBF4
ASGNF4
line 362
;362:	len = VectorLength( delta );
ADDRLP4 4
ARGP4
ADDRLP4 48
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 20
ADDRLP4 48
INDIRF4
ASGNF4
line 363
;363:	if ( len < le->radius ) {
ADDRLP4 20
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
GEF4 $192
line 364
;364:		CG_FreeLocalEntity( le );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CG_FreeLocalEntity
CALLV
pop
line 365
;365:		return;
ADDRGP4 $170
JUMPV
LABELV $192
line 368
;366:	}
;367:
;368:	trap_R_AddRefEntityToScene( re );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_R_AddRefEntityToScene
CALLV
pop
line 369
;369:}
LABELV $170
endproc CG_AddMoveScaleFade 52 12
proc CG_AddScaleFade 48 4
line 381
;370:
;371:
;372:/*
;373:===================
;374:CG_AddScaleFade
;375:
;376:For rocket smokes that hang in place, fade out, and are
;377:removed if the view passes through them.
;378:There are often many of these, so it needs to be simple.
;379:===================
;380:*/
;381:static void CG_AddScaleFade( localEntity_t *le ) {
line 387
;382:	refEntity_t	*re;
;383:	float		c;
;384:	vec3_t		delta;
;385:	float		len;
;386:
;387:	re = &le->refEntity;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
line 390
;388:
;389:	// fade / grow time
;390:	c = ( le->endTime - cg.time ) * le->lifeRate;
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 24
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRGP4 cg+107604
INDIRI4
SUBI4
CVIF4 4
ADDRLP4 24
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
MULF4
ASGNF4
line 392
;391:
;392:	re->shaderRGBA[3] = 0xff * c * le->color[3];
ADDRLP4 32
CNSTF4 1132396544
ADDRLP4 16
INDIRF4
MULF4
ADDRFP4 0
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 36
CNSTF4 1325400064
ASGNF4
ADDRLP4 32
INDIRF4
ADDRLP4 36
INDIRF4
LTF4 $197
ADDRLP4 28
ADDRLP4 32
INDIRF4
ADDRLP4 36
INDIRF4
SUBF4
CVFI4 4
CVIU4 4
CNSTU4 2147483648
ADDU4
ASGNU4
ADDRGP4 $198
JUMPV
LABELV $197
ADDRLP4 28
ADDRLP4 32
INDIRF4
CVFI4 4
CVIU4 4
ASGNU4
LABELV $198
ADDRLP4 0
INDIRP4
CNSTI4 119
ADDP4
ADDRLP4 28
INDIRU4
CVUU1 4
ASGNU1
line 393
;393:	re->radius = le->radius * ( 1.0 - c ) + 8;
ADDRLP4 0
INDIRP4
CNSTI4 132
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
CNSTF4 1065353216
ADDRLP4 16
INDIRF4
SUBF4
MULF4
CNSTF4 1090519040
ADDF4
ASGNF4
line 397
;394:
;395:	// if the view would be "inside" the sprite, kill the sprite
;396:	// so it doesn't add too much overdraw
;397:	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRF4
ADDRGP4 cg+109044+24
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
INDIRF4
ADDRGP4 cg+109044+24+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
INDIRF4
ADDRGP4 cg+109044+24+8
INDIRF4
SUBF4
ASGNF4
line 398
;398:	len = VectorLength( delta );
ADDRLP4 4
ARGP4
ADDRLP4 44
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 20
ADDRLP4 44
INDIRF4
ASGNF4
line 399
;399:	if ( len < le->radius ) {
ADDRLP4 20
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
GEF4 $209
line 400
;400:		CG_FreeLocalEntity( le );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CG_FreeLocalEntity
CALLV
pop
line 401
;401:		return;
ADDRGP4 $194
JUMPV
LABELV $209
line 404
;402:	}
;403:
;404:	trap_R_AddRefEntityToScene( re );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_R_AddRefEntityToScene
CALLV
pop
line 405
;405:}
LABELV $194
endproc CG_AddScaleFade 48 4
proc CG_AddFallScaleFade 52 4
line 418
;406:
;407:
;408:/*
;409:=================
;410:CG_AddFallScaleFade
;411:
;412:This is just an optimized CG_AddMoveScaleFade
;413:For blood mists that drift down, fade out, and are
;414:removed if the view passes through them.
;415:There are often 100+ of these, so it needs to be simple.
;416:=================
;417:*/
;418:static void CG_AddFallScaleFade( localEntity_t *le ) {
line 424
;419:	refEntity_t	*re;
;420:	float		c;
;421:	vec3_t		delta;
;422:	float		len;
;423:
;424:	re = &le->refEntity;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
line 427
;425:
;426:	// fade time
;427:	c = ( le->endTime - cg.time ) * le->lifeRate;
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 24
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRGP4 cg+107604
INDIRI4
SUBI4
CVIF4 4
ADDRLP4 24
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
MULF4
ASGNF4
line 429
;428:
;429:	re->shaderRGBA[3] = 0xff * c * le->color[3];
ADDRLP4 32
CNSTF4 1132396544
ADDRLP4 4
INDIRF4
MULF4
ADDRFP4 0
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 36
CNSTF4 1325400064
ASGNF4
ADDRLP4 32
INDIRF4
ADDRLP4 36
INDIRF4
LTF4 $214
ADDRLP4 28
ADDRLP4 32
INDIRF4
ADDRLP4 36
INDIRF4
SUBF4
CVFI4 4
CVIU4 4
CNSTU4 2147483648
ADDU4
ASGNU4
ADDRGP4 $215
JUMPV
LABELV $214
ADDRLP4 28
ADDRLP4 32
INDIRF4
CVFI4 4
CVIU4 4
ASGNU4
LABELV $215
ADDRLP4 0
INDIRP4
CNSTI4 119
ADDP4
ADDRLP4 28
INDIRU4
CVUU1 4
ASGNU1
line 431
;430:
;431:	re->origin[2] = le->pos.trBase[2] - ( 1.0 - c ) * le->pos.trDelta[2];
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
ADDRLP4 40
INDIRP4
CNSTI4 52
ADDP4
INDIRF4
CNSTF4 1065353216
ADDRLP4 4
INDIRF4
SUBF4
ADDRLP4 40
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
MULF4
SUBF4
ASGNF4
line 433
;432:
;433:	re->radius = le->radius * ( 1.0 - c ) + 16;
ADDRLP4 0
INDIRP4
CNSTI4 132
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
CNSTF4 1065353216
ADDRLP4 4
INDIRF4
SUBF4
MULF4
CNSTF4 1098907648
ADDF4
ASGNF4
line 437
;434:
;435:	// if the view would be "inside" the sprite, kill the sprite
;436:	// so it doesn't add too much overdraw
;437:	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRF4
ADDRGP4 cg+109044+24
INDIRF4
SUBF4
ASGNF4
ADDRLP4 8+4
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
INDIRF4
ADDRGP4 cg+109044+24+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 8+8
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
INDIRF4
ADDRGP4 cg+109044+24+8
INDIRF4
SUBF4
ASGNF4
line 438
;438:	len = VectorLength( delta );
ADDRLP4 8
ARGP4
ADDRLP4 48
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 20
ADDRLP4 48
INDIRF4
ASGNF4
line 439
;439:	if ( len < le->radius ) {
ADDRLP4 20
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
GEF4 $226
line 440
;440:		CG_FreeLocalEntity( le );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CG_FreeLocalEntity
CALLV
pop
line 441
;441:		return;
ADDRGP4 $211
JUMPV
LABELV $226
line 444
;442:	}
;443:
;444:	trap_R_AddRefEntityToScene( re );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_R_AddRefEntityToScene
CALLV
pop
line 445
;445:}
LABELV $211
endproc CG_AddFallScaleFade 52 4
proc CG_AddExplosion 20 20
line 454
;446:
;447:
;448:
;449:/*
;450:================
;451:CG_AddExplosion
;452:================
;453:*/
;454:static void CG_AddExplosion( localEntity_t *ex ) {
line 457
;455:	refEntity_t	*ent;
;456:
;457:	ent = &ex->refEntity;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
line 460
;458:
;459:	// add the entity
;460:	trap_R_AddRefEntityToScene(ent);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_R_AddRefEntityToScene
CALLV
pop
line 463
;461:
;462:	// add the dlight
;463:	if ( ex->light ) {
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
INDIRF4
CNSTF4 0
EQF4 $229
line 466
;464:		float		light;
;465:
;466:		light = (float)( cg.time - ex->startTime ) / ( ex->endTime - ex->startTime );
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 8
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
ADDRGP4 cg+107604
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
CVIF4 4
ADDRLP4 8
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
CVIF4 4
DIVF4
ASGNF4
line 467
;467:		if ( light < 0.5 ) {
ADDRLP4 4
INDIRF4
CNSTF4 1056964608
GEF4 $232
line 468
;468:			light = 1.0;
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
line 469
;469:		} else {
ADDRGP4 $233
JUMPV
LABELV $232
line 470
;470:			light = 1.0 - ( light - 0.5 ) * 2;
ADDRLP4 4
CNSTF4 1065353216
CNSTF4 1073741824
ADDRLP4 4
INDIRF4
CNSTF4 1056964608
SUBF4
MULF4
SUBF4
ASGNF4
line 471
;471:		}
LABELV $233
line 472
;472:		light = ex->light * light;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
ASGNF4
line 473
;473:		trap_R_AddLightToScene(ent->origin, light, ex->lightColor[0], ex->lightColor[1], ex->lightColor[2] );
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ARGP4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 132
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 136
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 140
ADDP4
INDIRF4
ARGF4
ADDRGP4 trap_R_AddLightToScene
CALLV
pop
line 474
;474:	}
LABELV $229
line 475
;475:}
LABELV $228
endproc CG_AddExplosion 20 20
proc CG_AddSpriteExplosion 180 20
line 482
;476:
;477:/*
;478:================
;479:CG_AddSpriteExplosion
;480:================
;481:*/
;482:static void CG_AddSpriteExplosion( localEntity_t *le ) {
line 486
;483:	refEntity_t	re;
;484:	float c;
;485:
;486:	re = le->refEntity;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
INDIRB
ASGNB 140
line 488
;487:
;488:	c = ( le->endTime - cg.time ) / ( float ) ( le->endTime - le->startTime );
ADDRLP4 144
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 148
ADDRLP4 144
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ASGNI4
ADDRLP4 140
ADDRLP4 148
INDIRI4
ADDRGP4 cg+107604
INDIRI4
SUBI4
CVIF4 4
ADDRLP4 148
INDIRI4
ADDRLP4 144
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
SUBI4
CVIF4 4
DIVF4
ASGNF4
line 489
;489:	if ( c > 1 ) {
ADDRLP4 140
INDIRF4
CNSTF4 1065353216
LEF4 $236
line 490
;490:		c = 1.0;	// can happen during connection problems
ADDRLP4 140
CNSTF4 1065353216
ASGNF4
line 491
;491:	}
LABELV $236
line 493
;492:
;493:	re.shaderRGBA[0] = 0xff;
ADDRLP4 0+116
CNSTU1 255
ASGNU1
line 494
;494:	re.shaderRGBA[1] = 0xff;
ADDRLP4 0+116+1
CNSTU1 255
ASGNU1
line 495
;495:	re.shaderRGBA[2] = 0xff;
ADDRLP4 0+116+2
CNSTU1 255
ASGNU1
line 496
;496:	re.shaderRGBA[3] = 0xff * c * 0.33;
ADDRLP4 156
CNSTF4 1051260355
CNSTF4 1132396544
ADDRLP4 140
INDIRF4
MULF4
MULF4
ASGNF4
ADDRLP4 160
CNSTF4 1325400064
ASGNF4
ADDRLP4 156
INDIRF4
ADDRLP4 160
INDIRF4
LTF4 $246
ADDRLP4 152
ADDRLP4 156
INDIRF4
ADDRLP4 160
INDIRF4
SUBF4
CVFI4 4
CVIU4 4
CNSTU4 2147483648
ADDU4
ASGNU4
ADDRGP4 $247
JUMPV
LABELV $246
ADDRLP4 152
ADDRLP4 156
INDIRF4
CVFI4 4
CVIU4 4
ASGNU4
LABELV $247
ADDRLP4 0+116+3
ADDRLP4 152
INDIRU4
CVUU1 4
ASGNU1
line 498
;497:
;498:	re.reType = RT_SPRITE;
ADDRLP4 0
CNSTI4 2
ASGNI4
line 499
;499:	re.radius = 42 * ( 1.0 - c ) + 30;
ADDRLP4 0+132
CNSTF4 1109917696
CNSTF4 1065353216
ADDRLP4 140
INDIRF4
SUBF4
MULF4
CNSTF4 1106247680
ADDF4
ASGNF4
line 501
;500:
;501:	trap_R_AddRefEntityToScene( &re );
ADDRLP4 0
ARGP4
ADDRGP4 trap_R_AddRefEntityToScene
CALLV
pop
line 504
;502:
;503:	// add the dlight
;504:	if ( le->light ) {
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
INDIRF4
CNSTF4 0
EQF4 $249
line 507
;505:		float		light;
;506:
;507:		light = (float)( cg.time - le->startTime ) / ( le->endTime - le->startTime );
ADDRLP4 168
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 172
ADDRLP4 168
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
ADDRLP4 164
ADDRGP4 cg+107604
INDIRI4
ADDRLP4 172
INDIRI4
SUBI4
CVIF4 4
ADDRLP4 168
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRLP4 172
INDIRI4
SUBI4
CVIF4 4
DIVF4
ASGNF4
line 508
;508:		if ( light < 0.5 ) {
ADDRLP4 164
INDIRF4
CNSTF4 1056964608
GEF4 $252
line 509
;509:			light = 1.0;
ADDRLP4 164
CNSTF4 1065353216
ASGNF4
line 510
;510:		} else {
ADDRGP4 $253
JUMPV
LABELV $252
line 511
;511:			light = 1.0 - ( light - 0.5 ) * 2;
ADDRLP4 164
CNSTF4 1065353216
CNSTF4 1073741824
ADDRLP4 164
INDIRF4
CNSTF4 1056964608
SUBF4
MULF4
SUBF4
ASGNF4
line 512
;512:		}
LABELV $253
line 513
;513:		light = le->light * light;
ADDRLP4 164
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
INDIRF4
ADDRLP4 164
INDIRF4
MULF4
ASGNF4
line 514
;514:		trap_R_AddLightToScene(re.origin, light, le->lightColor[0], le->lightColor[1], le->lightColor[2] );
ADDRLP4 0+68
ARGP4
ADDRLP4 164
INDIRF4
ARGF4
ADDRLP4 176
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 176
INDIRP4
CNSTI4 132
ADDP4
INDIRF4
ARGF4
ADDRLP4 176
INDIRP4
CNSTI4 136
ADDP4
INDIRF4
ARGF4
ADDRLP4 176
INDIRP4
CNSTI4 140
ADDP4
INDIRF4
ARGF4
ADDRGP4 trap_R_AddLightToScene
CALLV
pop
line 515
;515:	}
LABELV $249
line 516
;516:}
LABELV $234
endproc CG_AddSpriteExplosion 180 20
lit
align 4
LABELV $256
byte 4 0
byte 4 0
byte 4 1065353216
export CG_AddScorePlum
code
proc CG_AddScorePlum 168 12
line 701
;517:
;518:
;519:#ifdef MISSIONPACK
;520:/*
;521:====================
;522:CG_AddKamikaze
;523:====================
;524:*/
;525:void CG_AddKamikaze( localEntity_t *le ) {
;526:	refEntity_t	*re;
;527:	refEntity_t shockwave;
;528:	float		c;
;529:	vec3_t		test, axis[3];
;530:	int			t;
;531:
;532:	re = &le->refEntity;
;533:
;534:	t = cg.time - le->startTime;
;535:	VectorClear( test );
;536:	AnglesToAxis( test, axis );
;537:
;538:	if (t > KAMI_SHOCKWAVE_STARTTIME && t < KAMI_SHOCKWAVE_ENDTIME) {
;539:
;540:		if (!(le->leFlags & LEF_SOUND1)) {
;541://			trap_S_StartSound (re->origin, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.kamikazeExplodeSound );
;542:			trap_S_StartLocalSound(cgs.media.kamikazeExplodeSound, CHAN_AUTO);
;543:			le->leFlags |= LEF_SOUND1;
;544:		}
;545:		// 1st kamikaze shockwave
;546:		memset(&shockwave, 0, sizeof(shockwave));
;547:		shockwave.hModel = cgs.media.kamikazeShockWave;
;548:		shockwave.reType = RT_MODEL;
;549:		shockwave.shaderTime = re->shaderTime;
;550:		VectorCopy(re->origin, shockwave.origin);
;551:
;552:		c = (float)(t - KAMI_SHOCKWAVE_STARTTIME) / (float)(KAMI_SHOCKWAVE_ENDTIME - KAMI_SHOCKWAVE_STARTTIME);
;553:		VectorScale( axis[0], c * KAMI_SHOCKWAVE_MAXRADIUS / KAMI_SHOCKWAVEMODEL_RADIUS, shockwave.axis[0] );
;554:		VectorScale( axis[1], c * KAMI_SHOCKWAVE_MAXRADIUS / KAMI_SHOCKWAVEMODEL_RADIUS, shockwave.axis[1] );
;555:		VectorScale( axis[2], c * KAMI_SHOCKWAVE_MAXRADIUS / KAMI_SHOCKWAVEMODEL_RADIUS, shockwave.axis[2] );
;556:		shockwave.nonNormalizedAxes = qtrue;
;557:
;558:		if (t > KAMI_SHOCKWAVEFADE_STARTTIME) {
;559:			c = (float)(t - KAMI_SHOCKWAVEFADE_STARTTIME) / (float)(KAMI_SHOCKWAVE_ENDTIME - KAMI_SHOCKWAVEFADE_STARTTIME);
;560:		}
;561:		else {
;562:			c = 0;
;563:		}
;564:		c *= 0xff;
;565:		shockwave.shaderRGBA[0] = 0xff - c;
;566:		shockwave.shaderRGBA[1] = 0xff - c;
;567:		shockwave.shaderRGBA[2] = 0xff - c;
;568:		shockwave.shaderRGBA[3] = 0xff - c;
;569:
;570:		trap_R_AddRefEntityToScene( &shockwave );
;571:	}
;572:
;573:	if (t > KAMI_EXPLODE_STARTTIME && t < KAMI_IMPLODE_ENDTIME) {
;574:		// explosion and implosion
;575:		c = ( le->endTime - cg.time ) * le->lifeRate;
;576:		c *= 0xff;
;577:		re->shaderRGBA[0] = le->color[0] * c;
;578:		re->shaderRGBA[1] = le->color[1] * c;
;579:		re->shaderRGBA[2] = le->color[2] * c;
;580:		re->shaderRGBA[3] = le->color[3] * c;
;581:
;582:		if( t < KAMI_IMPLODE_STARTTIME ) {
;583:			c = (float)(t - KAMI_EXPLODE_STARTTIME) / (float)(KAMI_IMPLODE_STARTTIME - KAMI_EXPLODE_STARTTIME);
;584:		}
;585:		else {
;586:			if (!(le->leFlags & LEF_SOUND2)) {
;587://				trap_S_StartSound (re->origin, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.kamikazeImplodeSound );
;588:				trap_S_StartLocalSound(cgs.media.kamikazeImplodeSound, CHAN_AUTO);
;589:				le->leFlags |= LEF_SOUND2;
;590:			}
;591:			c = (float)(KAMI_IMPLODE_ENDTIME - t) / (float) (KAMI_IMPLODE_ENDTIME - KAMI_IMPLODE_STARTTIME);
;592:		}
;593:		VectorScale( axis[0], c * KAMI_BOOMSPHERE_MAXRADIUS / KAMI_BOOMSPHEREMODEL_RADIUS, re->axis[0] );
;594:		VectorScale( axis[1], c * KAMI_BOOMSPHERE_MAXRADIUS / KAMI_BOOMSPHEREMODEL_RADIUS, re->axis[1] );
;595:		VectorScale( axis[2], c * KAMI_BOOMSPHERE_MAXRADIUS / KAMI_BOOMSPHEREMODEL_RADIUS, re->axis[2] );
;596:		re->nonNormalizedAxes = qtrue;
;597:
;598:		trap_R_AddRefEntityToScene( re );
;599:		// add the dlight
;600:		trap_R_AddLightToScene( re->origin, c * 1000.0, 1.0, 1.0, c );
;601:	}
;602:
;603:	if (t > KAMI_SHOCKWAVE2_STARTTIME && t < KAMI_SHOCKWAVE2_ENDTIME) {
;604:		// 2nd kamikaze shockwave
;605:		if (le->angles.trBase[0] == 0 &&
;606:			le->angles.trBase[1] == 0 &&
;607:			le->angles.trBase[2] == 0) {
;608:			le->angles.trBase[0] = random() * 360;
;609:			le->angles.trBase[1] = random() * 360;
;610:			le->angles.trBase[2] = random() * 360;
;611:		}
;612:		else {
;613:			c = 0;
;614:		}
;615:		memset(&shockwave, 0, sizeof(shockwave));
;616:		shockwave.hModel = cgs.media.kamikazeShockWave;
;617:		shockwave.reType = RT_MODEL;
;618:		shockwave.shaderTime = re->shaderTime;
;619:		VectorCopy(re->origin, shockwave.origin);
;620:
;621:		test[0] = le->angles.trBase[0];
;622:		test[1] = le->angles.trBase[1];
;623:		test[2] = le->angles.trBase[2];
;624:		AnglesToAxis( test, axis );
;625:
;626:		c = (float)(t - KAMI_SHOCKWAVE2_STARTTIME) / (float)(KAMI_SHOCKWAVE2_ENDTIME - KAMI_SHOCKWAVE2_STARTTIME);
;627:		VectorScale( axis[0], c * KAMI_SHOCKWAVE2_MAXRADIUS / KAMI_SHOCKWAVEMODEL_RADIUS, shockwave.axis[0] );
;628:		VectorScale( axis[1], c * KAMI_SHOCKWAVE2_MAXRADIUS / KAMI_SHOCKWAVEMODEL_RADIUS, shockwave.axis[1] );
;629:		VectorScale( axis[2], c * KAMI_SHOCKWAVE2_MAXRADIUS / KAMI_SHOCKWAVEMODEL_RADIUS, shockwave.axis[2] );
;630:		shockwave.nonNormalizedAxes = qtrue;
;631:
;632:		if (t > KAMI_SHOCKWAVE2FADE_STARTTIME) {
;633:			c = (float)(t - KAMI_SHOCKWAVE2FADE_STARTTIME) / (float)(KAMI_SHOCKWAVE2_ENDTIME - KAMI_SHOCKWAVE2FADE_STARTTIME);
;634:		}
;635:		else {
;636:			c = 0;
;637:		}
;638:		c *= 0xff;
;639:		shockwave.shaderRGBA[0] = 0xff - c;
;640:		shockwave.shaderRGBA[1] = 0xff - c;
;641:		shockwave.shaderRGBA[2] = 0xff - c;
;642:		shockwave.shaderRGBA[3] = 0xff - c;
;643:
;644:		trap_R_AddRefEntityToScene( &shockwave );
;645:	}
;646:}
;647:
;648:/*
;649:===================
;650:CG_AddInvulnerabilityImpact
;651:===================
;652:*/
;653:void CG_AddInvulnerabilityImpact( localEntity_t *le ) {
;654:	trap_R_AddRefEntityToScene( &le->refEntity );
;655:}
;656:
;657:/*
;658:===================
;659:CG_AddInvulnerabilityJuiced
;660:===================
;661:*/
;662:void CG_AddInvulnerabilityJuiced( localEntity_t *le ) {
;663:	int t;
;664:
;665:	t = cg.time - le->startTime;
;666:	if ( t > 3000 ) {
;667:		le->refEntity.axis[0][0] = (float) 1.0 + 0.3 * (t - 3000) / 2000;
;668:		le->refEntity.axis[1][1] = (float) 1.0 + 0.3 * (t - 3000) / 2000;
;669:		le->refEntity.axis[2][2] = (float) 0.7 + 0.3 * (2000 - (t - 3000)) / 2000;
;670:	}
;671:	if ( t > 5000 ) {
;672:		le->endTime = 0;
;673:		CG_GibPlayer( le->refEntity.origin );
;674:	}
;675:	else {
;676:		trap_R_AddRefEntityToScene( &le->refEntity );
;677:	}
;678:}
;679:
;680:/*
;681:===================
;682:CG_AddRefEntity
;683:===================
;684:*/
;685:void CG_AddRefEntity( localEntity_t *le ) {
;686:	if (le->endTime < cg.time) {
;687:		CG_FreeLocalEntity( le );
;688:		return;
;689:	}
;690:	trap_R_AddRefEntityToScene( &le->refEntity );
;691:}
;692:
;693:#endif
;694:/*
;695:===================
;696:CG_AddScorePlum
;697:===================
;698:*/
;699:#define NUMBER_SIZE		8
;700:
;701:void CG_AddScorePlum( localEntity_t *le ) {
line 703
;702:	refEntity_t	*re;
;703:	vec3_t		origin, delta, dir, vec, up = {0, 0, 1};
ADDRLP4 112
ADDRGP4 $256
INDIRB
ASGNB 12
line 707
;704:	float		c, len;
;705:	int			i, score, digits[10], numdigits, negative;
;706:
;707:	re = &le->refEntity;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
line 709
;708:
;709:	c = ( le->endTime - cg.time ) * le->lifeRate;
ADDRLP4 128
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 80
ADDRLP4 128
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRGP4 cg+107604
INDIRI4
SUBI4
CVIF4 4
ADDRLP4 128
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
MULF4
ASGNF4
line 711
;710:
;711:	score = le->radius;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
CVFI4 4
ASGNI4
line 712
;712:	if (score < 0) {
ADDRLP4 12
INDIRI4
CNSTI4 0
GEI4 $258
line 713
;713:		re->shaderRGBA[0] = 0xff;
ADDRLP4 8
INDIRP4
CNSTI4 116
ADDP4
CNSTU1 255
ASGNU1
line 714
;714:		re->shaderRGBA[1] = 0x11;
ADDRLP4 8
INDIRP4
CNSTI4 117
ADDP4
CNSTU1 17
ASGNU1
line 715
;715:		re->shaderRGBA[2] = 0x11;
ADDRLP4 8
INDIRP4
CNSTI4 118
ADDP4
CNSTU1 17
ASGNU1
line 716
;716:	}
ADDRGP4 $259
JUMPV
LABELV $258
line 717
;717:	else {
line 718
;718:		re->shaderRGBA[0] = 0xff;
ADDRLP4 8
INDIRP4
CNSTI4 116
ADDP4
CNSTU1 255
ASGNU1
line 719
;719:		re->shaderRGBA[1] = 0xff;
ADDRLP4 8
INDIRP4
CNSTI4 117
ADDP4
CNSTU1 255
ASGNU1
line 720
;720:		re->shaderRGBA[2] = 0xff;
ADDRLP4 8
INDIRP4
CNSTI4 118
ADDP4
CNSTU1 255
ASGNU1
line 721
;721:		if (score >= 50) {
ADDRLP4 12
INDIRI4
CNSTI4 50
LTI4 $260
line 722
;722:			re->shaderRGBA[1] = 0;
ADDRLP4 8
INDIRP4
CNSTI4 117
ADDP4
CNSTU1 0
ASGNU1
line 723
;723:		} else if (score >= 20) {
ADDRGP4 $261
JUMPV
LABELV $260
ADDRLP4 12
INDIRI4
CNSTI4 20
LTI4 $262
line 724
;724:			re->shaderRGBA[0] = re->shaderRGBA[1] = 0;
ADDRLP4 136
CNSTU1 0
ASGNU1
ADDRLP4 8
INDIRP4
CNSTI4 117
ADDP4
ADDRLP4 136
INDIRU1
ASGNU1
ADDRLP4 8
INDIRP4
CNSTI4 116
ADDP4
ADDRLP4 136
INDIRU1
ASGNU1
line 725
;725:		} else if (score >= 10) {
ADDRGP4 $263
JUMPV
LABELV $262
ADDRLP4 12
INDIRI4
CNSTI4 10
LTI4 $264
line 726
;726:			re->shaderRGBA[2] = 0;
ADDRLP4 8
INDIRP4
CNSTI4 118
ADDP4
CNSTU1 0
ASGNU1
line 727
;727:		} else if (score >= 2) {
ADDRGP4 $265
JUMPV
LABELV $264
ADDRLP4 12
INDIRI4
CNSTI4 2
LTI4 $266
line 728
;728:			re->shaderRGBA[0] = re->shaderRGBA[2] = 0;
ADDRLP4 136
CNSTU1 0
ASGNU1
ADDRLP4 8
INDIRP4
CNSTI4 118
ADDP4
ADDRLP4 136
INDIRU1
ASGNU1
ADDRLP4 8
INDIRP4
CNSTI4 116
ADDP4
ADDRLP4 136
INDIRU1
ASGNU1
line 729
;729:		}
LABELV $266
LABELV $265
LABELV $263
LABELV $261
line 731
;730:
;731:	}
LABELV $259
line 732
;732:	if (c < 0.25)
ADDRLP4 80
INDIRF4
CNSTF4 1048576000
GEF4 $268
line 733
;733:		re->shaderRGBA[3] = 0xff * 4 * c;
ADDRLP4 136
CNSTF4 1149173760
ADDRLP4 80
INDIRF4
MULF4
ASGNF4
ADDRLP4 140
CNSTF4 1325400064
ASGNF4
ADDRLP4 136
INDIRF4
ADDRLP4 140
INDIRF4
LTF4 $271
ADDRLP4 132
ADDRLP4 136
INDIRF4
ADDRLP4 140
INDIRF4
SUBF4
CVFI4 4
CVIU4 4
CNSTU4 2147483648
ADDU4
ASGNU4
ADDRGP4 $272
JUMPV
LABELV $271
ADDRLP4 132
ADDRLP4 136
INDIRF4
CVFI4 4
CVIU4 4
ASGNU4
LABELV $272
ADDRLP4 8
INDIRP4
CNSTI4 119
ADDP4
ADDRLP4 132
INDIRU4
CVUU1 4
ASGNU1
ADDRGP4 $269
JUMPV
LABELV $268
line 735
;734:	else
;735:		re->shaderRGBA[3] = 0xff;
ADDRLP4 8
INDIRP4
CNSTI4 119
ADDP4
CNSTU1 255
ASGNU1
LABELV $269
line 737
;736:
;737:	re->radius = NUMBER_SIZE / 2;
ADDRLP4 8
INDIRP4
CNSTI4 132
ADDP4
CNSTF4 1082130432
ASGNF4
line 739
;738:
;739:	VectorCopy(le->pos.trBase, origin);
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRB
ASGNB 12
line 740
;740:	origin[2] += 110 - c * 100;
ADDRLP4 16+8
ADDRLP4 16+8
INDIRF4
CNSTF4 1121714176
CNSTF4 1120403456
ADDRLP4 80
INDIRF4
MULF4
SUBF4
ADDF4
ASGNF4
line 742
;741:
;742:	VectorSubtract(cg.refdef.vieworg, origin, dir);
ADDRLP4 96
ADDRGP4 cg+109044+24
INDIRF4
ADDRLP4 16
INDIRF4
SUBF4
ASGNF4
ADDRLP4 96+4
ADDRGP4 cg+109044+24+4
INDIRF4
ADDRLP4 16+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 96+8
ADDRGP4 cg+109044+24+8
INDIRF4
ADDRLP4 16+8
INDIRF4
SUBF4
ASGNF4
line 743
;743:	CrossProduct(dir, up, vec);
ADDRLP4 96
ARGP4
ADDRLP4 112
ARGP4
ADDRLP4 28
ARGP4
ADDRGP4 CrossProduct
CALLV
pop
line 744
;744:	VectorNormalize(vec);
ADDRLP4 28
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 746
;745:
;746:	VectorMA(origin, -10 + 20 * sin(c * 2 * M_PI), vec, origin);
CNSTF4 1078530011
CNSTF4 1073741824
ADDRLP4 80
INDIRF4
MULF4
MULF4
ARGF4
ADDRLP4 144
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 16
INDIRF4
ADDRLP4 28
INDIRF4
CNSTF4 1101004800
ADDRLP4 144
INDIRF4
MULF4
CNSTF4 3240099840
ADDF4
MULF4
ADDF4
ASGNF4
CNSTF4 1078530011
CNSTF4 1073741824
ADDRLP4 80
INDIRF4
MULF4
MULF4
ARGF4
ADDRLP4 148
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 16+4
ADDRLP4 16+4
INDIRF4
ADDRLP4 28+4
INDIRF4
CNSTF4 1101004800
ADDRLP4 148
INDIRF4
MULF4
CNSTF4 3240099840
ADDF4
MULF4
ADDF4
ASGNF4
CNSTF4 1078530011
CNSTF4 1073741824
ADDRLP4 80
INDIRF4
MULF4
MULF4
ARGF4
ADDRLP4 152
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 16+8
ADDRLP4 16+8
INDIRF4
ADDRLP4 28+8
INDIRF4
CNSTF4 1101004800
ADDRLP4 152
INDIRF4
MULF4
CNSTF4 3240099840
ADDF4
MULF4
ADDF4
ASGNF4
line 750
;747:
;748:	// if the view would be "inside" the sprite, kill the sprite
;749:	// so it doesn't add too much overdraw
;750:	VectorSubtract( origin, cg.refdef.vieworg, delta );
ADDRLP4 84
ADDRLP4 16
INDIRF4
ADDRGP4 cg+109044+24
INDIRF4
SUBF4
ASGNF4
ADDRLP4 84+4
ADDRLP4 16+4
INDIRF4
ADDRGP4 cg+109044+24+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 84+8
ADDRLP4 16+8
INDIRF4
ADDRGP4 cg+109044+24+8
INDIRF4
SUBF4
ASGNF4
line 751
;751:	len = VectorLength( delta );
ADDRLP4 84
ARGP4
ADDRLP4 156
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 124
ADDRLP4 156
INDIRF4
ASGNF4
line 752
;752:	if ( len < 20 ) {
ADDRLP4 124
INDIRF4
CNSTF4 1101004800
GEF4 $304
line 753
;753:		CG_FreeLocalEntity( le );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CG_FreeLocalEntity
CALLV
pop
line 754
;754:		return;
ADDRGP4 $255
JUMPV
LABELV $304
line 757
;755:	}
;756:
;757:	negative = qfalse;
ADDRLP4 108
CNSTI4 0
ASGNI4
line 758
;758:	if (score < 0) {
ADDRLP4 12
INDIRI4
CNSTI4 0
GEI4 $306
line 759
;759:		negative = qtrue;
ADDRLP4 108
CNSTI4 1
ASGNI4
line 760
;760:		score = -score;
ADDRLP4 12
ADDRLP4 12
INDIRI4
NEGI4
ASGNI4
line 761
;761:	}
LABELV $306
line 763
;762:
;763:	for (numdigits = 0; !(numdigits && !score); numdigits++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $311
JUMPV
LABELV $308
line 764
;764:		digits[numdigits] = score % 10;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 40
ADDP4
ADDRLP4 12
INDIRI4
CNSTI4 10
MODI4
ASGNI4
line 765
;765:		score = score / 10;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 10
DIVI4
ASGNI4
line 766
;766:	}
LABELV $309
line 763
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $311
ADDRLP4 160
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 160
INDIRI4
EQI4 $308
ADDRLP4 12
INDIRI4
ADDRLP4 160
INDIRI4
NEI4 $308
line 768
;767:
;768:	if (negative) {
ADDRLP4 108
INDIRI4
CNSTI4 0
EQI4 $312
line 769
;769:		digits[numdigits] = 10;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 40
ADDP4
CNSTI4 10
ASGNI4
line 770
;770:		numdigits++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 771
;771:	}
LABELV $312
line 773
;772:
;773:	for (i = 0; i < numdigits; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $317
JUMPV
LABELV $314
line 774
;774:		VectorMA(origin, (float) (((float) numdigits / 2) - i) * NUMBER_SIZE, vec, re->origin);
ADDRLP4 8
INDIRP4
CNSTI4 68
ADDP4
ADDRLP4 16
INDIRF4
ADDRLP4 28
INDIRF4
CNSTF4 1090519040
ADDRLP4 0
INDIRI4
CVIF4 4
CNSTF4 1073741824
DIVF4
ADDRLP4 4
INDIRI4
CVIF4 4
SUBF4
MULF4
MULF4
ADDF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 72
ADDP4
ADDRLP4 16+4
INDIRF4
ADDRLP4 28+4
INDIRF4
CNSTF4 1090519040
ADDRLP4 0
INDIRI4
CVIF4 4
CNSTF4 1073741824
DIVF4
ADDRLP4 4
INDIRI4
CVIF4 4
SUBF4
MULF4
MULF4
ADDF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 76
ADDP4
ADDRLP4 16+8
INDIRF4
ADDRLP4 28+8
INDIRF4
CNSTF4 1090519040
ADDRLP4 0
INDIRI4
CVIF4 4
CNSTF4 1073741824
DIVF4
ADDRLP4 4
INDIRI4
CVIF4 4
SUBF4
MULF4
MULF4
ADDF4
ASGNF4
line 775
;775:		re->customShader = cgs.media.numberShaders[digits[numdigits-1-i]];
ADDRLP4 164
CNSTI4 2
ASGNI4
ADDRLP4 8
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ADDRLP4 4
INDIRI4
SUBI4
ADDRLP4 164
INDIRI4
LSHI4
ADDRLP4 40
ADDP4
INDIRI4
ADDRLP4 164
INDIRI4
LSHI4
ADDRGP4 cgs+152340+300
ADDP4
INDIRI4
ASGNI4
line 776
;776:		trap_R_AddRefEntityToScene( re );
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_R_AddRefEntityToScene
CALLV
pop
line 777
;777:	}
LABELV $315
line 773
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $317
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRI4
LTI4 $314
line 778
;778:}
LABELV $255
endproc CG_AddScorePlum 168 12
export CG_AddLocalEntities
proc CG_AddLocalEntities 16 8
line 791
;779:
;780:
;781:
;782:
;783://==============================================================================
;784:
;785:/*
;786:===================
;787:CG_AddLocalEntities
;788:
;789:===================
;790:*/
;791:void CG_AddLocalEntities( void ) {
line 796
;792:	localEntity_t	*le, *next;
;793:
;794:	// walk the list backwards, so any new local entities generated
;795:	// (trails, marks, etc) will be present this frame
;796:	le = cg_activeLocalEntities.prev;
ADDRLP4 0
ADDRGP4 cg_activeLocalEntities
INDIRP4
ASGNP4
line 797
;797:	for ( ; le != &cg_activeLocalEntities ; le = next ) {
ADDRGP4 $328
JUMPV
LABELV $325
line 800
;798:		// grab next now, so if the local entity is freed we
;799:		// still have it
;800:		next = le->prev;
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRP4
ASGNP4
line 802
;801:
;802:		if ( cg.time >= le->endTime ) {
ADDRGP4 cg+107604
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
LTI4 $329
line 803
;803:			CG_FreeLocalEntity( le );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_FreeLocalEntity
CALLV
pop
line 804
;804:			continue;
ADDRGP4 $326
JUMPV
LABELV $329
line 806
;805:		}
;806:		switch ( le->leType ) {
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
LTI4 $332
ADDRLP4 8
INDIRI4
CNSTI4 8
GTI4 $332
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $345
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $345
address $333
address $338
address $337
address $339
address $340
address $342
address $341
address $343
address $344
code
LABELV $332
line 808
;807:		default:
;808:			CG_Error( "Bad leType: %i", le->leType );
ADDRGP4 $335
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 CG_Error
CALLV
pop
line 809
;809:			break;
ADDRGP4 $333
JUMPV
line 812
;810:
;811:		case LE_MARK:
;812:			break;
LABELV $337
line 815
;813:
;814:		case LE_SPRITE_EXPLOSION:
;815:			CG_AddSpriteExplosion( le );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_AddSpriteExplosion
CALLV
pop
line 816
;816:			break;
ADDRGP4 $333
JUMPV
LABELV $338
line 819
;817:
;818:		case LE_EXPLOSION:
;819:			CG_AddExplosion( le );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_AddExplosion
CALLV
pop
line 820
;820:			break;
ADDRGP4 $333
JUMPV
LABELV $339
line 823
;821:
;822:		case LE_FRAGMENT:			// gibs and brass
;823:			CG_AddFragment( le );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_AddFragment
CALLV
pop
line 824
;824:			break;
ADDRGP4 $333
JUMPV
LABELV $340
line 827
;825:
;826:		case LE_MOVE_SCALE_FADE:		// water bubbles
;827:			CG_AddMoveScaleFade( le );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_AddMoveScaleFade
CALLV
pop
line 828
;828:			break;
ADDRGP4 $333
JUMPV
LABELV $341
line 831
;829:
;830:		case LE_FADE_RGB:				// teleporters, railtrails
;831:			CG_AddFadeRGB( le );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_AddFadeRGB
CALLV
pop
line 832
;832:			break;
ADDRGP4 $333
JUMPV
LABELV $342
line 835
;833:
;834:		case LE_FALL_SCALE_FADE: // gib blood trails
;835:			CG_AddFallScaleFade( le );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_AddFallScaleFade
CALLV
pop
line 836
;836:			break;
ADDRGP4 $333
JUMPV
LABELV $343
line 839
;837:
;838:		case LE_SCALE_FADE:		// rocket trails
;839:			CG_AddScaleFade( le );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_AddScaleFade
CALLV
pop
line 840
;840:			break;
ADDRGP4 $333
JUMPV
LABELV $344
line 843
;841:
;842:		case LE_SCOREPLUM:
;843:			CG_AddScorePlum( le );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 CG_AddScorePlum
CALLV
pop
line 844
;844:			break;
LABELV $333
line 861
;845:
;846:#ifdef MISSIONPACK
;847:		case LE_KAMIKAZE:
;848:			CG_AddKamikaze( le );
;849:			break;
;850:		case LE_INVULIMPACT:
;851:			CG_AddInvulnerabilityImpact( le );
;852:			break;
;853:		case LE_INVULJUICED:
;854:			CG_AddInvulnerabilityJuiced( le );
;855:			break;
;856:		case LE_SHOWREFENTITY:
;857:			CG_AddRefEntity( le );
;858:			break;
;859:#endif
;860:		}
;861:	}
LABELV $326
line 797
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
LABELV $328
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRGP4 cg_activeLocalEntities
CVPU4 4
NEU4 $325
line 862
;862:}
LABELV $324
endproc CG_AddLocalEntities 16 8
bss
export cg_freeLocalEntities
align 4
LABELV cg_freeLocalEntities
skip 4
export cg_activeLocalEntities
align 4
LABELV cg_activeLocalEntities
skip 292
export cg_localEntities
align 4
LABELV cg_localEntities
skip 149504
import CG_NewParticleArea
import initparticles
import CG_ParticleExplosion
import CG_ParticleMisc
import CG_ParticleDust
import CG_ParticleSparks
import CG_ParticleBulletDebris
import CG_ParticleSnowFlurry
import CG_AddParticleShrapnel
import CG_ParticleSmoke
import CG_ParticleSnow
import CG_AddParticles
import CG_ClearParticles
import trap_GetEntityToken
import trap_getCameraInfo
import trap_startCamera
import trap_loadCamera
import trap_SnapVector
import trap_CIN_SetExtents
import trap_CIN_DrawCinematic
import trap_CIN_RunCinematic
import trap_CIN_StopCinematic
import trap_CIN_PlayCinematic
import trap_Key_GetKey
import trap_Key_SetCatcher
import trap_Key_GetCatcher
import trap_Key_IsDown
import trap_R_RegisterFont
import trap_MemoryRemaining
import testPrintFloat
import testPrintInt
import trap_SetUserCmdValue
import trap_GetUserCmd
import trap_GetCurrentCmdNumber
import trap_GetServerCommand
import trap_GetSnapshot
import trap_GetCurrentSnapshotNumber
import trap_GetGameState
import trap_GetGlconfig
import trap_R_RemapShader
import trap_R_LerpTag
import trap_R_ModelBounds
import trap_R_DrawStretchPic
import trap_R_SetColor
import trap_R_RenderScene
import trap_R_LightForPoint
import trap_R_AddLightToScene
import trap_R_AddPolysToScene
import trap_R_AddPolyToScene
import trap_R_AddRefEntityToScene
import trap_R_ClearScene
import trap_R_RegisterShaderNoMip
import trap_R_RegisterShader
import trap_R_RegisterSkin
import trap_R_RegisterModel
import trap_R_LoadWorldMap
import trap_S_StopBackgroundTrack
import trap_S_StartBackgroundTrack
import trap_S_RegisterSound
import trap_S_Respatialize
import trap_S_UpdateEntityPosition
import trap_S_AddRealLoopingSound
import trap_S_AddLoopingSound
import trap_S_ClearLoopingSounds
import trap_S_StartLocalSound
import trap_S_StopLoopingSound
import trap_S_StartSound
import trap_CM_MarkFragments
import trap_CM_TransformedBoxTrace
import trap_CM_BoxTrace
import trap_CM_TransformedPointContents
import trap_CM_PointContents
import trap_CM_TempBoxModel
import trap_CM_InlineModel
import trap_CM_NumInlineModels
import trap_CM_LoadMap
import trap_UpdateScreen
import trap_SendClientCommand
import trap_AddCommand
import trap_SendConsoleCommand
import trap_FS_Seek
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import trap_Args
import trap_Argv
import trap_Argc
import trap_Cvar_VariableStringBuffer
import trap_Cvar_Set
import trap_Cvar_Update
import trap_Cvar_Register
import trap_Milliseconds
import trap_Error
import trap_Print
import CG_CheckChangedPredictableEvents
import CG_TransitionPlayerState
import CG_Respawn
import CG_PlayBufferedVoiceChats
import CG_VoiceChatLocal
import CG_ShaderStateChanged
import CG_LoadVoiceChats
import CG_SetConfigValues
import CG_ParseServerinfo
import CG_ExecuteNewServerCommands
import CG_InitConsoleCommands
import CG_ConsoleCommand
import CG_DrawOldTourneyScoreboard
import CG_DrawOldScoreboard
import CG_DrawInformation
import CG_LoadingClient
import CG_LoadingItem
import CG_LoadingString
import CG_ProcessSnapshots
import CG_MakeExplosion
import CG_Bleed
import CG_BigExplode
import CG_GibPlayer
import CG_ScorePlum
import CG_SpawnEffect
import CG_BubbleTrail
import CG_SmokePuff
import CG_ImpactMark
import CG_AddMarks
import CG_InitMarkPolys
import CG_OutOfAmmoChange
import CG_DrawWeaponSelect
import CG_AddPlayerWeapon
import CG_AddViewWeapon
import CG_GrappleTrail
import CG_RailTrail
import CG_Bullet
import CG_ShotgunFire
import CG_MissileHitPlayer
import CG_MissileHitWall
import CG_FireWeapon
import CG_RegisterItemVisuals
import CG_RegisterWeapon
import CG_Weapon_f
import CG_PrevWeapon_f
import CG_NextWeapon_f
import CG_PositionRotatedEntityOnTag
import CG_PositionEntityOnTag
import CG_AdjustPositionForMover
import CG_Beam
import CG_AddPacketEntities
import CG_SetEntitySoundPosition
import CG_PainEvent
import CG_EntityEvent
import CG_PlaceString
import CG_CheckEvents
import CG_LoadDeferredPlayers
import CG_PredictPlayerState
import CG_Trace
import CG_PointContents
import CG_BuildSolidList
import CG_CustomSound
import CG_NewClientInfo
import CG_AddRefEntityWithPowerups
import CG_ResetPlayerEntity
import CG_Player
import CG_StatusHandle
import CG_OtherTeamHasFlag
import CG_YourTeamHasFlag
import CG_GameTypeString
import CG_CheckOrderPending
import CG_Text_PaintChar
import CG_Draw3DModel
import CG_GetKillerText
import CG_GetGameStatusText
import CG_GetTeamColor
import CG_InitTeamChat
import CG_SetPrintString
import CG_ShowResponseHead
import CG_RunMenuScript
import CG_OwnerDrawVisible
import CG_GetValue
import CG_SelectNextPlayer
import CG_SelectPrevPlayer
import CG_Text_Height
import CG_Text_Width
import CG_Text_Paint
import CG_OwnerDraw
import CG_DrawTeamBackground
import CG_DrawFlagModel
import CG_DrawActive
import CG_DrawHead
import CG_CenterPrint
import CG_AddLagometerSnapshotInfo
import CG_AddLagometerFrameInfo
import teamChat2
import teamChat1
import systemChat
import drawTeamOverlayModificationCount
import numSortedTeamPlayers
import sortedTeamPlayers
import CG_DrawTopBottom
import CG_DrawSides
import CG_DrawRect
import UI_DrawProportionalString
import CG_GetColorForHealth
import CG_ColorForHealth
import CG_TileClear
import CG_TeamColor
import CG_FadeColor
import CG_DrawStrlen
import CG_DrawSmallStringColor
import CG_DrawSmallString
import CG_DrawBigStringColor
import CG_DrawBigString
import CG_DrawStringExt
import CG_DrawString
import CG_DrawPic
import CG_FillRect
import CG_AdjustFrom640
import CG_DrawActiveFrame
import CG_AddBufferedSound
import CG_ZoomUp_f
import CG_ZoomDown_f
import CG_TestModelPrevSkin_f
import CG_TestModelNextSkin_f
import CG_TestModelPrevFrame_f
import CG_TestModelNextFrame_f
import CG_TestGun_f
import CG_TestModel_f
import CG_BuildSpectatorString
import CG_GetSelectedScore
import CG_SetScoreSelection
import CG_RankRunFrame
import CG_EventHandling
import CG_MouseEvent
import CG_KeyEvent
import CG_LoadMenus
import CG_LastAttacker
import CG_CrosshairPlayer
import CG_UpdateCvars
import CG_StartMusic
import CG_Error
import CG_Printf
import CG_Argv
import CG_ConfigString
import cg_trueLightning
import cg_oldPlasma
import cg_oldRocket
import cg_oldRail
import cg_noProjectileTrail
import cg_noTaunt
import cg_bigFont
import cg_smallFont
import cg_cameraMode
import cg_timescale
import cg_timescaleFadeSpeed
import cg_timescaleFadeEnd
import cg_cameraOrbitDelay
import cg_cameraOrbit
import pmove_msec
import pmove_fixed
import cg_smoothClients
import cg_scorePlum
import cg_noVoiceText
import cg_noVoiceChats
import cg_teamChatsOnly
import cg_drawFriend
import cg_deferPlayers
import cg_predictItems
import cg_blood
import cg_paused
import cg_buildScript
import cg_forceModel
import cg_stats
import cg_teamChatHeight
import cg_teamChatTime
import cg_synchronousClients
import cg_drawAttacker
import cg_lagometer
import cg_stereoSeparation
import cg_thirdPerson
import cg_thirdPersonAngle
import cg_thirdPersonRange
import cg_zoomFov
import cg_fov
import cg_simpleItems
import cg_ignore
import cg_autoswitch
import cg_tracerLength
import cg_tracerWidth
import cg_tracerChance
import cg_viewsize
import cg_drawGun
import cg_gun_z
import cg_gun_y
import cg_gun_x
import cg_gun_frame
import cg_brassTime
import cg_addMarks
import cg_footsteps
import cg_showmiss
import cg_noPlayerAnims
import cg_nopredict
import cg_errorDecay
import cg_railTrailTime
import cg_debugEvents
import cg_debugPosition
import cg_debugAnim
import cg_animSpeed
import cg_draw2D
import cg_drawStatus
import cg_crosshairHealth
import cg_crosshairSize
import cg_crosshairY
import cg_crosshairX
import cg_teamOverlayUserinfo
import cg_drawTeamOverlay
import cg_drawRewards
import cg_drawCrosshairNames
import cg_drawCrosshair
import cg_drawAmmoWarning
import cg_drawIcons
import cg_draw3dIcons
import cg_drawSnapshot
import cg_drawFPS
import cg_drawTimer
import cg_gibs
import cg_shadows
import cg_swingSpeed
import cg_bobroll
import cg_bobpitch
import cg_bobup
import cg_runroll
import cg_runpitch
import cg_centertime
import cg_markPolys
import cg_items
import cg_weapons
import cg_entities
import cg
import cgs
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
LABELV $335
byte 1 66
byte 1 97
byte 1 100
byte 1 32
byte 1 108
byte 1 101
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $81
byte 1 67
byte 1 71
byte 1 95
byte 1 70
byte 1 114
byte 1 101
byte 1 101
byte 1 76
byte 1 111
byte 1 99
byte 1 97
byte 1 108
byte 1 69
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 101
byte 1 0
