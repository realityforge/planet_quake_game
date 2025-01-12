export CG_BubbleTrail
code
proc CG_BubbleTrail 96 4
file "../cg_effects.c"
line 16
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:// cg_effects.c -- these functions generate localentities, usually as a result
;4:// of event processing
;5:
;6:#include "cg_local.h"
;7:
;8:
;9:/*
;10:==================
;11:CG_BubbleTrail
;12:
;13:Bullets shot underwater
;14:==================
;15:*/
;16:void CG_BubbleTrail( vec3_t start, vec3_t end, float spacing ) {
line 22
;17:	vec3_t		move;
;18:	vec3_t		vec;
;19:	float		len;
;20:	int			i;
;21:
;22:	if ( cg_noProjectileTrail.integer ) {
ADDRGP4 cg_noProjectileTrail+12
INDIRI4
CNSTI4 0
EQI4 $71
line 23
;23:		return;
ADDRGP4 $70
JUMPV
LABELV $71
line 26
;24:	}
;25:
;26:	VectorCopy (start, move);
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 27
;27:	VectorSubtract (end, start, vec);
ADDRLP4 32
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 32
INDIRP4
INDIRF4
ADDRLP4 36
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 40
CNSTI4 4
ASGNI4
ADDRLP4 12+4
ADDRLP4 32
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
CNSTI4 8
ASGNI4
ADDRLP4 12+8
ADDRFP4 4
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
line 28
;28:	len = VectorNormalize (vec);
ADDRLP4 12
ARGP4
ADDRLP4 48
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 28
ADDRLP4 48
INDIRF4
ASGNF4
line 31
;29:
;30:	// advance a random amount first
;31:	i = rand() % (int)spacing;
ADDRLP4 52
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 24
ADDRLP4 52
INDIRI4
ADDRFP4 8
INDIRF4
CVFI4 4
MODI4
ASGNI4
line 32
;32:	VectorMA( move, i, vec, move );
ADDRLP4 56
ADDRLP4 24
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 12
INDIRF4
ADDRLP4 56
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 12+4
INDIRF4
ADDRLP4 56
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 12+8
INDIRF4
ADDRLP4 24
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
line 34
;33:
;34:	VectorScale (vec, spacing, vec);
ADDRLP4 60
ADDRFP4 8
INDIRF4
ASGNF4
ADDRLP4 12
ADDRLP4 12
INDIRF4
ADDRLP4 60
INDIRF4
MULF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 12+4
INDIRF4
ADDRLP4 60
INDIRF4
MULF4
ASGNF4
ADDRLP4 12+8
ADDRLP4 12+8
INDIRF4
ADDRFP4 8
INDIRF4
MULF4
ASGNF4
line 36
;35:
;36:	for ( ; i < len; i += spacing ) {
ADDRGP4 $89
JUMPV
LABELV $86
line 40
;37:		localEntity_t	*le;
;38:		refEntity_t		*re;
;39:
;40:		le = CG_AllocLocalEntity();
ADDRLP4 72
ADDRGP4 CG_AllocLocalEntity
CALLP4
ASGNP4
ADDRLP4 64
ADDRLP4 72
INDIRP4
ASGNP4
line 41
;41:		le->leFlags = LEF_PUFF_DONT_SCALE;
ADDRLP4 64
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 1
ASGNI4
line 42
;42:		le->leType = LE_MOVE_SCALE_FADE;
ADDRLP4 64
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 4
ASGNI4
line 43
;43:		le->startTime = cg.time;
ADDRLP4 64
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 cg+107604
INDIRI4
ASGNI4
line 44
;44:		le->endTime = cg.time + 1000 + random() * 250;
ADDRLP4 76
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 64
INDIRP4
CNSTI4 20
ADDP4
ADDRGP4 cg+107604
INDIRI4
CNSTI4 1000
ADDI4
CVIF4 4
CNSTF4 1132068864
ADDRLP4 76
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 45
;45:		le->lifeRate = 1.0 / ( le->endTime - le->startTime );
ADDRLP4 64
INDIRP4
CNSTI4 28
ADDP4
CNSTF4 1065353216
ADDRLP4 64
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRLP4 64
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
SUBI4
CVIF4 4
DIVF4
ASGNF4
line 47
;46:
;47:		re = &le->refEntity;
ADDRLP4 68
ADDRLP4 64
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
line 48
;48:		re->shaderTime = cg.time / 1000.0f;
ADDRLP4 68
INDIRP4
CNSTI4 128
ADDP4
ADDRGP4 cg+107604
INDIRI4
CVIF4 4
CNSTF4 1148846080
DIVF4
ASGNF4
line 50
;49:
;50:		re->reType = RT_SPRITE;
ADDRLP4 68
INDIRP4
CNSTI4 2
ASGNI4
line 51
;51:		re->rotation = 0;
ADDRLP4 68
INDIRP4
CNSTI4 136
ADDP4
CNSTF4 0
ASGNF4
line 52
;52:		re->radius = 3;
ADDRLP4 68
INDIRP4
CNSTI4 132
ADDP4
CNSTF4 1077936128
ASGNF4
line 53
;53:		re->customShader = cgs.media.waterBubbleShader;
ADDRLP4 68
INDIRP4
CNSTI4 112
ADDP4
ADDRGP4 cgs+152340+292
INDIRI4
ASGNI4
line 54
;54:		re->shaderRGBA[0] = 0xff;
ADDRLP4 68
INDIRP4
CNSTI4 116
ADDP4
CNSTU1 255
ASGNU1
line 55
;55:		re->shaderRGBA[1] = 0xff;
ADDRLP4 68
INDIRP4
CNSTI4 117
ADDP4
CNSTU1 255
ASGNU1
line 56
;56:		re->shaderRGBA[2] = 0xff;
ADDRLP4 68
INDIRP4
CNSTI4 118
ADDP4
CNSTU1 255
ASGNU1
line 57
;57:		re->shaderRGBA[3] = 0xff;
ADDRLP4 68
INDIRP4
CNSTI4 119
ADDP4
CNSTU1 255
ASGNU1
line 59
;58:
;59:		le->color[3] = 1.0;
ADDRLP4 64
INDIRP4
CNSTI4 120
ADDP4
CNSTF4 1065353216
ASGNF4
line 61
;60:
;61:		le->pos.trType = TR_LINEAR;
ADDRLP4 64
INDIRP4
CNSTI4 32
ADDP4
CNSTI4 2
ASGNI4
line 62
;62:		le->pos.trTime = cg.time;
ADDRLP4 64
INDIRP4
CNSTI4 36
ADDP4
ADDRGP4 cg+107604
INDIRI4
ASGNI4
line 63
;63:		VectorCopy( move, le->pos.trBase );
ADDRLP4 64
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 0
INDIRB
ASGNB 12
line 64
;64:		le->pos.trDelta[0] = crandom()*5;
ADDRLP4 84
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 64
INDIRP4
CNSTI4 56
ADDP4
CNSTF4 1084227584
CNSTF4 1073741824
ADDRLP4 84
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 65
;65:		le->pos.trDelta[1] = crandom()*5;
ADDRLP4 88
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 64
INDIRP4
CNSTI4 60
ADDP4
CNSTF4 1084227584
CNSTF4 1073741824
ADDRLP4 88
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 66
;66:		le->pos.trDelta[2] = crandom()*5 + 6;
ADDRLP4 92
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 64
INDIRP4
CNSTI4 64
ADDP4
CNSTF4 1084227584
CNSTF4 1073741824
ADDRLP4 92
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1086324736
ADDF4
ASGNF4
line 68
;67:
;68:		VectorAdd (move, vec, move);
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 12
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 12+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 12+8
INDIRF4
ADDF4
ASGNF4
line 69
;69:	}
LABELV $87
line 36
ADDRLP4 24
ADDRLP4 24
INDIRI4
CVIF4 4
ADDRFP4 8
INDIRF4
ADDF4
CVFI4 4
ASGNI4
LABELV $89
ADDRLP4 24
INDIRI4
CVIF4 4
ADDRLP4 28
INDIRF4
LTF4 $86
line 70
;70:}
LABELV $70
endproc CG_BubbleTrail 96 4
data
align 4
LABELV $103
byte 4 146
export CG_SmokePuff
code
proc CG_SmokePuff 52 4
line 86
;71:
;72:/*
;73:=====================
;74:CG_SmokePuff
;75:
;76:Adds a smoke puff or blood trail localEntity.
;77:=====================
;78:*/
;79:localEntity_t *CG_SmokePuff( const vec3_t p, const vec3_t vel, 
;80:				   float radius,
;81:				   float r, float g, float b, float a,
;82:				   float duration,
;83:				   int startTime,
;84:				   int fadeInTime,
;85:				   int leFlags,
;86:				   qhandle_t hShader ) {
line 92
;87:	static int	seed = 0x92;
;88:	localEntity_t	*le;
;89:	refEntity_t		*re;
;90://	int fadeInTime = startTime + duration / 2;
;91:
;92:	le = CG_AllocLocalEntity();
ADDRLP4 8
ADDRGP4 CG_AllocLocalEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 93
;93:	le->leFlags = leFlags;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ADDRFP4 40
INDIRI4
ASGNI4
line 94
;94:	le->radius = radius;
ADDRLP4 0
INDIRP4
CNSTI4 124
ADDP4
ADDRFP4 8
INDIRF4
ASGNF4
line 96
;95:
;96:	re = &le->refEntity;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
line 97
;97:	re->rotation = Q_random( &seed ) * 360;
ADDRGP4 $103
ARGP4
ADDRLP4 12
ADDRGP4 Q_random
CALLF4
ASGNF4
ADDRLP4 4
INDIRP4
CNSTI4 136
ADDP4
CNSTF4 1135869952
ADDRLP4 12
INDIRF4
MULF4
ASGNF4
line 98
;98:	re->radius = radius;
ADDRLP4 4
INDIRP4
CNSTI4 132
ADDP4
ADDRFP4 8
INDIRF4
ASGNF4
line 99
;99:	re->shaderTime = startTime / 1000.0f;
ADDRLP4 4
INDIRP4
CNSTI4 128
ADDP4
ADDRFP4 32
INDIRI4
CVIF4 4
CNSTF4 1148846080
DIVF4
ASGNF4
line 101
;100:
;101:	le->leType = LE_MOVE_SCALE_FADE;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 4
ASGNI4
line 102
;102:	le->startTime = startTime;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRFP4 32
INDIRI4
ASGNI4
line 103
;103:	le->fadeInTime = fadeInTime;
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRFP4 36
INDIRI4
ASGNI4
line 104
;104:	le->endTime = startTime + duration;
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ADDRFP4 32
INDIRI4
CVIF4 4
ADDRFP4 28
INDIRF4
ADDF4
CVFI4 4
ASGNI4
line 105
;105:	if ( fadeInTime > startTime ) {
ADDRFP4 36
INDIRI4
ADDRFP4 32
INDIRI4
LEI4 $104
line 106
;106:		le->lifeRate = 1.0 / ( le->endTime - le->fadeInTime );
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
CNSTF4 1065353216
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
SUBI4
CVIF4 4
DIVF4
ASGNF4
line 107
;107:	}
ADDRGP4 $105
JUMPV
LABELV $104
line 108
;108:	else {
line 109
;109:		le->lifeRate = 1.0 / ( le->endTime - le->startTime );
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
CNSTF4 1065353216
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
SUBI4
CVIF4 4
DIVF4
ASGNF4
line 110
;110:	}
LABELV $105
line 111
;111:	le->color[0] = r;
ADDRLP4 0
INDIRP4
CNSTI4 108
ADDP4
ADDRFP4 12
INDIRF4
ASGNF4
line 112
;112:	le->color[1] = g; 
ADDRLP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRFP4 16
INDIRF4
ASGNF4
line 113
;113:	le->color[2] = b;
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRFP4 20
INDIRF4
ASGNF4
line 114
;114:	le->color[3] = a;
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
ADDRFP4 24
INDIRF4
ASGNF4
line 117
;115:
;116:
;117:	le->pos.trType = TR_LINEAR;
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
CNSTI4 2
ASGNI4
line 118
;118:	le->pos.trTime = startTime;
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRFP4 32
INDIRI4
ASGNI4
line 119
;119:	VectorCopy( vel, le->pos.trDelta );
ADDRLP4 0
INDIRP4
CNSTI4 56
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 120
;120:	VectorCopy( p, le->pos.trBase );
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 122
;121:
;122:	VectorCopy( p, re->origin );
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 123
;123:	re->customShader = hShader;
ADDRLP4 4
INDIRP4
CNSTI4 112
ADDP4
ADDRFP4 44
INDIRI4
ASGNI4
line 126
;124:
;125:	// rage pro can't alpha fade, so use a different shader
;126:	if ( cgs.glconfig.hardwareType == GLHW_RAGEPRO ) {
ADDRGP4 cgs+20100+11288
INDIRI4
CNSTI4 3
NEI4 $106
line 127
;127:		re->customShader = cgs.media.smokePuffRageProShader;
ADDRLP4 4
INDIRP4
CNSTI4 112
ADDP4
ADDRGP4 cgs+152340+280
INDIRI4
ASGNI4
line 128
;128:		re->shaderRGBA[0] = 0xff;
ADDRLP4 4
INDIRP4
CNSTI4 116
ADDP4
CNSTU1 255
ASGNU1
line 129
;129:		re->shaderRGBA[1] = 0xff;
ADDRLP4 4
INDIRP4
CNSTI4 117
ADDP4
CNSTU1 255
ASGNU1
line 130
;130:		re->shaderRGBA[2] = 0xff;
ADDRLP4 4
INDIRP4
CNSTI4 118
ADDP4
CNSTU1 255
ASGNU1
line 131
;131:		re->shaderRGBA[3] = 0xff;
ADDRLP4 4
INDIRP4
CNSTI4 119
ADDP4
CNSTU1 255
ASGNU1
line 132
;132:	} else {
ADDRGP4 $107
JUMPV
LABELV $106
line 133
;133:		re->shaderRGBA[0] = le->color[0] * 0xff;
ADDRLP4 20
CNSTF4 1132396544
ADDRLP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 24
CNSTF4 1325400064
ASGNF4
ADDRLP4 20
INDIRF4
ADDRLP4 24
INDIRF4
LTF4 $113
ADDRLP4 16
ADDRLP4 20
INDIRF4
ADDRLP4 24
INDIRF4
SUBF4
CVFI4 4
CVIU4 4
CNSTU4 2147483648
ADDU4
ASGNU4
ADDRGP4 $114
JUMPV
LABELV $113
ADDRLP4 16
ADDRLP4 20
INDIRF4
CVFI4 4
CVIU4 4
ASGNU4
LABELV $114
ADDRLP4 4
INDIRP4
CNSTI4 116
ADDP4
ADDRLP4 16
INDIRU4
CVUU1 4
ASGNU1
line 134
;134:		re->shaderRGBA[1] = le->color[1] * 0xff;
ADDRLP4 32
CNSTF4 1132396544
ADDRLP4 0
INDIRP4
CNSTI4 112
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
LTF4 $116
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
ADDRGP4 $117
JUMPV
LABELV $116
ADDRLP4 28
ADDRLP4 32
INDIRF4
CVFI4 4
CVIU4 4
ASGNU4
LABELV $117
ADDRLP4 4
INDIRP4
CNSTI4 117
ADDP4
ADDRLP4 28
INDIRU4
CVUU1 4
ASGNU1
line 135
;135:		re->shaderRGBA[2] = le->color[2] * 0xff;
ADDRLP4 44
CNSTF4 1132396544
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 48
CNSTF4 1325400064
ASGNF4
ADDRLP4 44
INDIRF4
ADDRLP4 48
INDIRF4
LTF4 $119
ADDRLP4 40
ADDRLP4 44
INDIRF4
ADDRLP4 48
INDIRF4
SUBF4
CVFI4 4
CVIU4 4
CNSTU4 2147483648
ADDU4
ASGNU4
ADDRGP4 $120
JUMPV
LABELV $119
ADDRLP4 40
ADDRLP4 44
INDIRF4
CVFI4 4
CVIU4 4
ASGNU4
LABELV $120
ADDRLP4 4
INDIRP4
CNSTI4 118
ADDP4
ADDRLP4 40
INDIRU4
CVUU1 4
ASGNU1
line 136
;136:		re->shaderRGBA[3] = 0xff;
ADDRLP4 4
INDIRP4
CNSTI4 119
ADDP4
CNSTU1 255
ASGNU1
line 137
;137:	}
LABELV $107
line 139
;138:
;139:	re->reType = RT_SPRITE;
ADDRLP4 4
INDIRP4
CNSTI4 2
ASGNI4
line 140
;140:	re->radius = le->radius;
ADDRLP4 4
INDIRP4
CNSTI4 132
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
ASGNF4
line 142
;141:
;142:	return le;
ADDRLP4 0
INDIRP4
RETP4
LABELV $102
endproc CG_SmokePuff 52 4
export CG_SpawnEffect
proc CG_SpawnEffect 28 4
line 152
;143:}
;144:
;145:/*
;146:==================
;147:CG_SpawnEffect
;148:
;149:Player teleporting in or out
;150:==================
;151:*/
;152:void CG_SpawnEffect( vec3_t org ) {
line 156
;153:	localEntity_t	*le;
;154:	refEntity_t		*re;
;155:
;156:	le = CG_AllocLocalEntity();
ADDRLP4 8
ADDRGP4 CG_AllocLocalEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 157
;157:	le->leFlags = 0;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 158
;158:	le->leType = LE_FADE_RGB;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 6
ASGNI4
line 159
;159:	le->startTime = cg.time;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 cg+107604
INDIRI4
ASGNI4
line 160
;160:	le->endTime = cg.time + 500;
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ADDRGP4 cg+107604
INDIRI4
CNSTI4 500
ADDI4
ASGNI4
line 161
;161:	le->lifeRate = 1.0 / ( le->endTime - le->startTime );
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
CNSTF4 1065353216
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
SUBI4
CVIF4 4
DIVF4
ASGNF4
line 163
;162:
;163:	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;
ADDRLP4 20
CNSTF4 1065353216
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 108
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
line 165
;164:
;165:	re = &le->refEntity;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
line 167
;166:
;167:	re->reType = RT_MODEL;
ADDRLP4 4
INDIRP4
CNSTI4 0
ASGNI4
line 168
;168:	re->shaderTime = cg.time / 1000.0f;
ADDRLP4 4
INDIRP4
CNSTI4 128
ADDP4
ADDRGP4 cg+107604
INDIRI4
CVIF4 4
CNSTF4 1148846080
DIVF4
ASGNF4
line 171
;169:
;170:#ifndef MISSIONPACK
;171:	re->customShader = cgs.media.teleportEffectShader;
ADDRLP4 4
INDIRP4
CNSTI4 112
ADDP4
ADDRGP4 cgs+152340+480
INDIRI4
ASGNI4
line 173
;172:#endif
;173:	re->hModel = cgs.media.teleportEffectModel;
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
ADDRGP4 cgs+152340+476
INDIRI4
ASGNI4
line 174
;174:	AxisClear( re->axis );
ADDRLP4 4
INDIRP4
CNSTI4 28
ADDP4
ARGP4
ADDRGP4 AxisClear
CALLV
pop
line 176
;175:
;176:	VectorCopy( org, re->origin );
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 180
;177:#ifdef MISSIONPACK
;178:	re->origin[2] += 16;
;179:#else
;180:	re->origin[2] -= 24;
ADDRLP4 24
ADDRLP4 4
INDIRP4
CNSTI4 76
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRF4
CNSTF4 1103101952
SUBF4
ASGNF4
line 182
;181:#endif
;182:}
LABELV $121
endproc CG_SpawnEffect 28 4
bss
align 4
LABELV $130
skip 12
export CG_ScorePlum
code
proc CG_ScorePlum 52 8
line 367
;183:
;184:
;185:#ifdef MISSIONPACK
;186:/*
;187:===============
;188:CG_LightningBoltBeam
;189:===============
;190:*/
;191:void CG_LightningBoltBeam( vec3_t start, vec3_t end ) {
;192:	localEntity_t	*le;
;193:	refEntity_t		*beam;
;194:
;195:	le = CG_AllocLocalEntity();
;196:	le->leFlags = 0;
;197:	le->leType = LE_SHOWREFENTITY;
;198:	le->startTime = cg.time;
;199:	le->endTime = cg.time + 50;
;200:
;201:	beam = &le->refEntity;
;202:
;203:	VectorCopy( start, beam->origin );
;204:	// this is the end point
;205:	VectorCopy( end, beam->oldorigin );
;206:
;207:	beam->reType = RT_LIGHTNING;
;208:	beam->customShader = cgs.media.lightningShader;
;209:}
;210:
;211:/*
;212:==================
;213:CG_KamikazeEffect
;214:==================
;215:*/
;216:void CG_KamikazeEffect( vec3_t org ) {
;217:	localEntity_t	*le;
;218:	refEntity_t		*re;
;219:
;220:	le = CG_AllocLocalEntity();
;221:	le->leFlags = 0;
;222:	le->leType = LE_KAMIKAZE;
;223:	le->startTime = cg.time;
;224:	le->endTime = cg.time + 3000;//2250;
;225:	le->lifeRate = 1.0 / ( le->endTime - le->startTime );
;226:
;227:	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;
;228:
;229:	VectorClear(le->angles.trBase);
;230:
;231:	re = &le->refEntity;
;232:
;233:	re->reType = RT_MODEL;
;234:	re->shaderTime = cg.time / 1000.0f;
;235:
;236:	re->hModel = cgs.media.kamikazeEffectModel;
;237:
;238:	VectorCopy( org, re->origin );
;239:
;240:}
;241:
;242:/*
;243:==================
;244:CG_ObeliskExplode
;245:==================
;246:*/
;247:void CG_ObeliskExplode( vec3_t org, int entityNum ) {
;248:	localEntity_t	*le;
;249:	vec3_t origin;
;250:
;251:	// create an explosion
;252:	VectorCopy( org, origin );
;253:	origin[2] += 64;
;254:	le = CG_MakeExplosion( origin, vec3_origin,
;255:						   cgs.media.dishFlashModel,
;256:						   cgs.media.rocketExplosionShader,
;257:						   600, qtrue );
;258:	le->light = 300;
;259:	le->lightColor[0] = 1;
;260:	le->lightColor[1] = 0.75;
;261:	le->lightColor[2] = 0.0;
;262:}
;263:
;264:/*
;265:==================
;266:CG_ObeliskPain
;267:==================
;268:*/
;269:void CG_ObeliskPain( vec3_t org ) {
;270:	float r;
;271:	sfxHandle_t sfx;
;272:
;273:	// hit sound
;274:	r = rand() & 3;
;275:	if ( r < 2 ) {
;276:		sfx = cgs.media.obeliskHitSound1;
;277:	} else if ( r == 2 ) {
;278:		sfx = cgs.media.obeliskHitSound2;
;279:	} else {
;280:		sfx = cgs.media.obeliskHitSound3;
;281:	}
;282:	trap_S_StartSound ( org, ENTITYNUM_NONE, CHAN_BODY, sfx );
;283:}
;284:
;285:
;286:/*
;287:==================
;288:CG_InvulnerabilityImpact
;289:==================
;290:*/
;291:void CG_InvulnerabilityImpact( vec3_t org, vec3_t angles ) {
;292:	localEntity_t	*le;
;293:	refEntity_t		*re;
;294:	int				r;
;295:	sfxHandle_t		sfx;
;296:
;297:	le = CG_AllocLocalEntity();
;298:	le->leFlags = 0;
;299:	le->leType = LE_INVULIMPACT;
;300:	le->startTime = cg.time;
;301:	le->endTime = cg.time + 1000;
;302:	le->lifeRate = 1.0 / ( le->endTime - le->startTime );
;303:
;304:	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;
;305:
;306:	re = &le->refEntity;
;307:
;308:	re->reType = RT_MODEL;
;309:	re->shaderTime = cg.time / 1000.0f;
;310:
;311:	re->hModel = cgs.media.invulnerabilityImpactModel;
;312:
;313:	VectorCopy( org, re->origin );
;314:	AnglesToAxis( angles, re->axis );
;315:
;316:	r = rand() & 3;
;317:	if ( r < 2 ) {
;318:		sfx = cgs.media.invulnerabilityImpactSound1;
;319:	} else if ( r == 2 ) {
;320:		sfx = cgs.media.invulnerabilityImpactSound2;
;321:	} else {
;322:		sfx = cgs.media.invulnerabilityImpactSound3;
;323:	}
;324:	trap_S_StartSound (org, ENTITYNUM_NONE, CHAN_BODY, sfx );
;325:}
;326:
;327:/*
;328:==================
;329:CG_InvulnerabilityJuiced
;330:==================
;331:*/
;332:void CG_InvulnerabilityJuiced( vec3_t org ) {
;333:	localEntity_t	*le;
;334:	refEntity_t		*re;
;335:	vec3_t			angles;
;336:
;337:	le = CG_AllocLocalEntity();
;338:	le->leFlags = 0;
;339:	le->leType = LE_INVULJUICED;
;340:	le->startTime = cg.time;
;341:	le->endTime = cg.time + 10000;
;342:	le->lifeRate = 1.0 / ( le->endTime - le->startTime );
;343:
;344:	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;
;345:
;346:	re = &le->refEntity;
;347:
;348:	re->reType = RT_MODEL;
;349:	re->shaderTime = cg.time / 1000.0f;
;350:
;351:	re->hModel = cgs.media.invulnerabilityJuicedModel;
;352:
;353:	VectorCopy( org, re->origin );
;354:	VectorClear(angles);
;355:	AnglesToAxis( angles, re->axis );
;356:
;357:	trap_S_StartSound (org, ENTITYNUM_NONE, CHAN_BODY, cgs.media.invulnerabilityJuicedSound );
;358:}
;359:
;360:#endif
;361:
;362:/*
;363:==================
;364:CG_ScorePlum
;365:==================
;366:*/
;367:void CG_ScorePlum( int client, vec3_t org, int score ) {
line 374
;368:	localEntity_t	*le;
;369:	refEntity_t		*re;
;370:	vec3_t			angles;
;371:	static vec3_t lastPos;
;372:
;373:	// only visualize for the client that scored
;374:	if (client != cg.predictedPlayerState.clientNum || cg_scorePlum.integer == 0) {
ADDRFP4 0
INDIRI4
ADDRGP4 cg+107636+140
INDIRI4
NEI4 $136
ADDRGP4 cg_scorePlum+12
INDIRI4
CNSTI4 0
NEI4 $131
LABELV $136
line 375
;375:		return;
ADDRGP4 $129
JUMPV
LABELV $131
line 378
;376:	}
;377:
;378:	le = CG_AllocLocalEntity();
ADDRLP4 20
ADDRGP4 CG_AllocLocalEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 379
;379:	le->leFlags = 0;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 380
;380:	le->leType = LE_SCOREPLUM;
ADDRLP4 24
CNSTI4 8
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
ADDRLP4 24
INDIRI4
ASGNI4
line 381
;381:	le->startTime = cg.time;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 cg+107604
INDIRI4
ASGNI4
line 382
;382:	le->endTime = cg.time + 4000;
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ADDRGP4 cg+107604
INDIRI4
CNSTI4 4000
ADDI4
ASGNI4
line 383
;383:	le->lifeRate = 1.0 / ( le->endTime - le->startTime );
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
CNSTF4 1065353216
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
SUBI4
CVIF4 4
DIVF4
ASGNF4
line 386
;384:
;385:	
;386:	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;
ADDRLP4 36
CNSTF4 1065353216
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
ADDRLP4 36
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRLP4 36
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 36
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 108
ADDP4
ADDRLP4 36
INDIRF4
ASGNF4
line 387
;387:	le->radius = score;
ADDRLP4 0
INDIRP4
CNSTI4 124
ADDP4
ADDRFP4 8
INDIRI4
CVIF4 4
ASGNF4
line 389
;388:	
;389:	VectorCopy( org, le->pos.trBase );
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 390
;390:	if (org[2] >= lastPos[2] - 20 && org[2] <= lastPos[2] + 20) {
ADDRLP4 40
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ASGNF4
ADDRLP4 44
CNSTF4 1101004800
ASGNF4
ADDRLP4 40
INDIRF4
ADDRGP4 $130+8
INDIRF4
ADDRLP4 44
INDIRF4
SUBF4
LTF4 $139
ADDRLP4 40
INDIRF4
ADDRGP4 $130+8
INDIRF4
ADDRLP4 44
INDIRF4
ADDF4
GTF4 $139
line 391
;391:		le->pos.trBase[2] -= 20;
ADDRLP4 48
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRF4
CNSTF4 1101004800
SUBF4
ASGNF4
line 392
;392:	}
LABELV $139
line 395
;393:
;394:	//CG_Printf( "Plum origin %i %i %i -- %i\n", (int)org[0], (int)org[1], (int)org[2], (int)Distance(org, lastPos));
;395:	VectorCopy(org, lastPos);
ADDRGP4 $130
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 398
;396:
;397:
;398:	re = &le->refEntity;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
line 400
;399:
;400:	re->reType = RT_SPRITE;
ADDRLP4 4
INDIRP4
CNSTI4 2
ASGNI4
line 401
;401:	re->radius = 16;
ADDRLP4 4
INDIRP4
CNSTI4 132
ADDP4
CNSTF4 1098907648
ASGNF4
line 403
;402:
;403:	VectorClear(angles);
ADDRLP4 48
CNSTF4 0
ASGNF4
ADDRLP4 8+8
ADDRLP4 48
INDIRF4
ASGNF4
ADDRLP4 8+4
ADDRLP4 48
INDIRF4
ASGNF4
ADDRLP4 8
ADDRLP4 48
INDIRF4
ASGNF4
line 404
;404:	AnglesToAxis( angles, re->axis );
ADDRLP4 8
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 28
ADDP4
ARGP4
ADDRGP4 AnglesToAxis
CALLV
pop
line 405
;405:}
LABELV $129
endproc CG_ScorePlum 52 8
export CG_MakeExplosion
proc CG_MakeExplosion 60 8
line 415
;406:
;407:
;408:/*
;409:====================
;410:CG_MakeExplosion
;411:====================
;412:*/
;413:localEntity_t *CG_MakeExplosion( vec3_t origin, vec3_t dir, 
;414:								qhandle_t hModel, qhandle_t shader,
;415:								int msec, qboolean isSprite ) {
line 421
;416:	float			ang;
;417:	localEntity_t	*ex;
;418:	int				offset;
;419:	vec3_t			tmpVec, newOrigin;
;420:
;421:	if ( msec <= 0 ) {
ADDRFP4 16
INDIRI4
CNSTI4 0
GTI4 $146
line 422
;422:		CG_Error( "CG_MakeExplosion: msec = %i", msec );
ADDRGP4 $148
ARGP4
ADDRFP4 16
INDIRI4
ARGI4
ADDRGP4 CG_Error
CALLV
pop
line 423
;423:	}
LABELV $146
line 426
;424:
;425:	// skew the time a bit so they aren't all in sync
;426:	offset = rand() & 63;
ADDRLP4 36
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 28
ADDRLP4 36
INDIRI4
CNSTI4 63
BANDI4
ASGNI4
line 428
;427:
;428:	ex = CG_AllocLocalEntity();
ADDRLP4 40
ADDRGP4 CG_AllocLocalEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 40
INDIRP4
ASGNP4
line 429
;429:	if ( isSprite ) {
ADDRFP4 20
INDIRI4
CNSTI4 0
EQI4 $149
line 430
;430:		ex->leType = LE_SPRITE_EXPLOSION;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 2
ASGNI4
line 433
;431:
;432:		// randomly rotate sprite orientation
;433:		ex->refEntity.rotation = rand() % 360;
ADDRLP4 44
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 288
ADDP4
ADDRLP4 44
INDIRI4
CNSTI4 360
MODI4
CVIF4 4
ASGNF4
line 434
;434:		VectorScale( dir, 16, tmpVec );
ADDRLP4 48
CNSTF4 1098907648
ASGNF4
ADDRLP4 52
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 48
INDIRF4
ADDRLP4 52
INDIRP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 16+4
ADDRLP4 48
INDIRF4
ADDRLP4 52
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 16+8
CNSTF4 1098907648
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
MULF4
ASGNF4
line 435
;435:		VectorAdd( tmpVec, origin, newOrigin );
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 16
INDIRF4
ADDRLP4 56
INDIRP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 16+4
INDIRF4
ADDRLP4 56
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4+8
ADDRLP4 16+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDF4
ASGNF4
line 436
;436:	} else {
ADDRGP4 $150
JUMPV
LABELV $149
line 437
;437:		ex->leType = LE_EXPLOSION;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 1
ASGNI4
line 438
;438:		VectorCopy( origin, newOrigin );
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 441
;439:
;440:		// set axis with random rotate
;441:		if ( !dir ) {
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $157
line 442
;442:			AxisClear( ex->refEntity.axis );
ADDRLP4 0
INDIRP4
CNSTI4 180
ADDP4
ARGP4
ADDRGP4 AxisClear
CALLV
pop
line 443
;443:		} else {
ADDRGP4 $158
JUMPV
LABELV $157
line 444
;444:			ang = rand() % 360;
ADDRLP4 44
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 32
ADDRLP4 44
INDIRI4
CNSTI4 360
MODI4
CVIF4 4
ASGNF4
line 445
;445:			VectorCopy( dir, ex->refEntity.axis[0] );
ADDRLP4 0
INDIRP4
CNSTI4 180
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 446
;446:			RotateAroundDirection( ex->refEntity.axis, ang );
ADDRLP4 0
INDIRP4
CNSTI4 180
ADDP4
ARGP4
ADDRLP4 32
INDIRF4
ARGF4
ADDRGP4 RotateAroundDirection
CALLV
pop
line 447
;447:		}
LABELV $158
line 448
;448:	}
LABELV $150
line 450
;449:
;450:	ex->startTime = cg.time - offset;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 cg+107604
INDIRI4
ADDRLP4 28
INDIRI4
SUBI4
ASGNI4
line 451
;451:	ex->endTime = ex->startTime + msec;
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ADDRFP4 16
INDIRI4
ADDI4
ASGNI4
line 454
;452:
;453:	// bias the time so all shader effects start correctly
;454:	ex->refEntity.shaderTime = ex->startTime / 1000.0f;
ADDRLP4 0
INDIRP4
CNSTI4 280
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1148846080
DIVF4
ASGNF4
line 456
;455:
;456:	ex->refEntity.hModel = hModel;
ADDRLP4 0
INDIRP4
CNSTI4 160
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 457
;457:	ex->refEntity.customShader = shader;
ADDRLP4 0
INDIRP4
CNSTI4 264
ADDP4
ADDRFP4 12
INDIRI4
ASGNI4
line 460
;458:
;459:	// set origin
;460:	VectorCopy( newOrigin, ex->refEntity.origin );
ADDRLP4 0
INDIRP4
CNSTI4 220
ADDP4
ADDRLP4 4
INDIRB
ASGNB 12
line 461
;461:	VectorCopy( newOrigin, ex->refEntity.oldorigin );
ADDRLP4 0
INDIRP4
CNSTI4 236
ADDP4
ADDRLP4 4
INDIRB
ASGNB 12
line 463
;462:
;463:	ex->color[0] = ex->color[1] = ex->color[2] = 1.0;
ADDRLP4 56
CNSTF4 1065353216
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRLP4 56
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 56
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 108
ADDP4
ADDRLP4 56
INDIRF4
ASGNF4
line 465
;464:
;465:	return ex;
ADDRLP4 0
INDIRP4
RETP4
LABELV $145
endproc CG_MakeExplosion 60 8
export CG_Bleed
proc CG_Bleed 20 0
line 476
;466:}
;467:
;468:
;469:/*
;470:=================
;471:CG_Bleed
;472:
;473:This is the spurt of blood when a character gets hit
;474:=================
;475:*/
;476:void CG_Bleed( vec3_t origin, int entityNum ) {
line 479
;477:	localEntity_t	*ex;
;478:
;479:	if ( !cg_blood.integer ) {
ADDRGP4 cg_blood+12
INDIRI4
CNSTI4 0
NEI4 $161
line 480
;480:		return;
ADDRGP4 $160
JUMPV
LABELV $161
line 483
;481:	}
;482:
;483:	ex = CG_AllocLocalEntity();
ADDRLP4 4
ADDRGP4 CG_AllocLocalEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 484
;484:	ex->leType = LE_EXPLOSION;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 1
ASGNI4
line 486
;485:
;486:	ex->startTime = cg.time;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 cg+107604
INDIRI4
ASGNI4
line 487
;487:	ex->endTime = ex->startTime + 500;
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 500
ADDI4
ASGNI4
line 489
;488:	
;489:	VectorCopy ( origin, ex->refEntity.origin);
ADDRLP4 0
INDIRP4
CNSTI4 220
ADDP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 490
;490:	ex->refEntity.reType = RT_SPRITE;
ADDRLP4 0
INDIRP4
CNSTI4 152
ADDP4
CNSTI4 2
ASGNI4
line 491
;491:	ex->refEntity.rotation = rand() % 360;
ADDRLP4 12
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 288
ADDP4
ADDRLP4 12
INDIRI4
CNSTI4 360
MODI4
CVIF4 4
ASGNF4
line 492
;492:	ex->refEntity.radius = 24;
ADDRLP4 0
INDIRP4
CNSTI4 284
ADDP4
CNSTF4 1103101952
ASGNF4
line 494
;493:
;494:	ex->refEntity.customShader = cgs.media.bloodExplosionShader;
ADDRLP4 0
INDIRP4
CNSTI4 264
ADDP4
ADDRGP4 cgs+152340+472
INDIRI4
ASGNI4
line 497
;495:
;496:	// don't show player's own blood in view
;497:	if ( entityNum == cg.snap->ps.clientNum ) {
ADDRFP4 4
INDIRI4
ADDRGP4 cg+36
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
NEI4 $167
line 498
;498:		ex->refEntity.renderfx |= RF_THIRD_PERSON;
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 156
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 499
;499:	}
LABELV $167
line 500
;500:}
LABELV $160
endproc CG_Bleed 20 0
export CG_LaunchGib
proc CG_LaunchGib 20 8
line 509
;501:
;502:
;503:
;504:/*
;505:==================
;506:CG_LaunchGib
;507:==================
;508:*/
;509:void CG_LaunchGib( vec3_t origin, vec3_t velocity, qhandle_t hModel ) {
line 513
;510:	localEntity_t	*le;
;511:	refEntity_t		*re;
;512:
;513:	le = CG_AllocLocalEntity();
ADDRLP4 8
ADDRGP4 CG_AllocLocalEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 514
;514:	re = &le->refEntity;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
line 516
;515:
;516:	le->leType = LE_FRAGMENT;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 3
ASGNI4
line 517
;517:	le->startTime = cg.time;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 cg+107604
INDIRI4
ASGNI4
line 518
;518:	le->endTime = le->startTime + 5000 + random() * 3000;
ADDRLP4 12
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 5000
ADDI4
CVIF4 4
CNSTF4 1161527296
ADDRLP4 12
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 520
;519:
;520:	VectorCopy( origin, re->origin );
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 521
;521:	AxisCopy( axisDefault, re->axis );
ADDRGP4 axisDefault
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 28
ADDP4
ARGP4
ADDRGP4 AxisCopy
CALLV
pop
line 522
;522:	re->hModel = hModel;
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 524
;523:
;524:	le->pos.trType = TR_GRAVITY;
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
CNSTI4 5
ASGNI4
line 525
;525:	VectorCopy( origin, le->pos.trBase );
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 526
;526:	VectorCopy( velocity, le->pos.trDelta );
ADDRLP4 0
INDIRP4
CNSTI4 56
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 527
;527:	le->pos.trTime = cg.time;
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRGP4 cg+107604
INDIRI4
ASGNI4
line 529
;528:
;529:	le->bounceFactor = 0.6f;
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
CNSTF4 1058642330
ASGNF4
line 531
;530:
;531:	le->leBounceSoundType = LEBS_BLOOD;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 1
ASGNI4
line 532
;532:	le->leMarkType = LEMT_BLOOD;
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 2
ASGNI4
line 533
;533:}
LABELV $170
endproc CG_LaunchGib 20 8
export CG_GibPlayer
proc CG_GibPlayer 148 12
line 544
;534:
;535:/*
;536:===================
;537:CG_GibPlayer
;538:
;539:Generated a bunch of gibs launching out from the bodies location
;540:===================
;541:*/
;542:#define	GIB_VELOCITY	250
;543:#define	GIB_JUMP		250
;544:void CG_GibPlayer( vec3_t playerOrigin ) {
line 547
;545:	vec3_t	origin, velocity;
;546:
;547:	if ( !cg_blood.integer ) {
ADDRGP4 cg_blood+12
INDIRI4
CNSTI4 0
NEI4 $174
line 548
;548:		return;
ADDRGP4 $173
JUMPV
LABELV $174
line 551
;549:	}
;550:
;551:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 552
;552:	velocity[0] = crandom()*GIB_VELOCITY;
ADDRLP4 24
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 24
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 553
;553:	velocity[1] = crandom()*GIB_VELOCITY;
ADDRLP4 28
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 28
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 554
;554:	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
ADDRLP4 32
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 32
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1132068864
ADDF4
ASGNF4
line 555
;555:	if ( rand() & 1 ) {
ADDRLP4 36
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $179
line 556
;556:		CG_LaunchGib( origin, velocity, cgs.media.gibSkull );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+168
INDIRI4
ARGI4
ADDRGP4 CG_LaunchGib
CALLV
pop
line 557
;557:	} else {
ADDRGP4 $180
JUMPV
LABELV $179
line 558
;558:		CG_LaunchGib( origin, velocity, cgs.media.gibBrain );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+172
INDIRI4
ARGI4
ADDRGP4 CG_LaunchGib
CALLV
pop
line 559
;559:	}
LABELV $180
line 562
;560:
;561:	// allow gibs to be turned off for speed
;562:	if ( !cg_gibs.integer ) {
ADDRGP4 cg_gibs+12
INDIRI4
CNSTI4 0
NEI4 $185
line 563
;563:		return;
ADDRGP4 $173
JUMPV
LABELV $185
line 566
;564:	}
;565:
;566:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 567
;567:	velocity[0] = crandom()*GIB_VELOCITY;
ADDRLP4 40
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 40
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 568
;568:	velocity[1] = crandom()*GIB_VELOCITY;
ADDRLP4 44
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 44
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 569
;569:	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
ADDRLP4 48
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 48
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1132068864
ADDF4
ASGNF4
line 570
;570:	CG_LaunchGib( origin, velocity, cgs.media.gibAbdomen );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+136
INDIRI4
ARGI4
ADDRGP4 CG_LaunchGib
CALLV
pop
line 572
;571:
;572:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 573
;573:	velocity[0] = crandom()*GIB_VELOCITY;
ADDRLP4 52
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 52
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 574
;574:	velocity[1] = crandom()*GIB_VELOCITY;
ADDRLP4 56
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 56
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 575
;575:	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
ADDRLP4 60
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 60
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1132068864
ADDF4
ASGNF4
line 576
;576:	CG_LaunchGib( origin, velocity, cgs.media.gibArm );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+140
INDIRI4
ARGI4
ADDRGP4 CG_LaunchGib
CALLV
pop
line 578
;577:
;578:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 579
;579:	velocity[0] = crandom()*GIB_VELOCITY;
ADDRLP4 64
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 64
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 580
;580:	velocity[1] = crandom()*GIB_VELOCITY;
ADDRLP4 68
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 68
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 581
;581:	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
ADDRLP4 72
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 72
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1132068864
ADDF4
ASGNF4
line 582
;582:	CG_LaunchGib( origin, velocity, cgs.media.gibChest );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+144
INDIRI4
ARGI4
ADDRGP4 CG_LaunchGib
CALLV
pop
line 584
;583:
;584:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 585
;585:	velocity[0] = crandom()*GIB_VELOCITY;
ADDRLP4 76
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 76
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 586
;586:	velocity[1] = crandom()*GIB_VELOCITY;
ADDRLP4 80
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 80
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 587
;587:	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
ADDRLP4 84
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 84
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1132068864
ADDF4
ASGNF4
line 588
;588:	CG_LaunchGib( origin, velocity, cgs.media.gibFist );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+148
INDIRI4
ARGI4
ADDRGP4 CG_LaunchGib
CALLV
pop
line 590
;589:
;590:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 591
;591:	velocity[0] = crandom()*GIB_VELOCITY;
ADDRLP4 88
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 88
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 592
;592:	velocity[1] = crandom()*GIB_VELOCITY;
ADDRLP4 92
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 92
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 593
;593:	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
ADDRLP4 96
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 96
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1132068864
ADDF4
ASGNF4
line 594
;594:	CG_LaunchGib( origin, velocity, cgs.media.gibFoot );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+152
INDIRI4
ARGI4
ADDRGP4 CG_LaunchGib
CALLV
pop
line 596
;595:
;596:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 597
;597:	velocity[0] = crandom()*GIB_VELOCITY;
ADDRLP4 100
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 100
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 598
;598:	velocity[1] = crandom()*GIB_VELOCITY;
ADDRLP4 104
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 104
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 599
;599:	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
ADDRLP4 108
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 108
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1132068864
ADDF4
ASGNF4
line 600
;600:	CG_LaunchGib( origin, velocity, cgs.media.gibForearm );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+156
INDIRI4
ARGI4
ADDRGP4 CG_LaunchGib
CALLV
pop
line 602
;601:
;602:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 603
;603:	velocity[0] = crandom()*GIB_VELOCITY;
ADDRLP4 112
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 112
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 604
;604:	velocity[1] = crandom()*GIB_VELOCITY;
ADDRLP4 116
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 116
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 605
;605:	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
ADDRLP4 120
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 120
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1132068864
ADDF4
ASGNF4
line 606
;606:	CG_LaunchGib( origin, velocity, cgs.media.gibIntestine );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+160
INDIRI4
ARGI4
ADDRGP4 CG_LaunchGib
CALLV
pop
line 608
;607:
;608:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 609
;609:	velocity[0] = crandom()*GIB_VELOCITY;
ADDRLP4 124
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 124
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 610
;610:	velocity[1] = crandom()*GIB_VELOCITY;
ADDRLP4 128
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 128
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 611
;611:	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
ADDRLP4 132
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 132
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1132068864
ADDF4
ASGNF4
line 612
;612:	CG_LaunchGib( origin, velocity, cgs.media.gibLeg );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+164
INDIRI4
ARGI4
ADDRGP4 CG_LaunchGib
CALLV
pop
line 614
;613:
;614:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 615
;615:	velocity[0] = crandom()*GIB_VELOCITY;
ADDRLP4 136
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 136
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 616
;616:	velocity[1] = crandom()*GIB_VELOCITY;
ADDRLP4 140
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 140
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 617
;617:	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
ADDRLP4 144
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1132068864
CNSTF4 1073741824
ADDRLP4 144
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1132068864
ADDF4
ASGNF4
line 618
;618:	CG_LaunchGib( origin, velocity, cgs.media.gibLeg );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+164
INDIRI4
ARGI4
ADDRGP4 CG_LaunchGib
CALLV
pop
line 619
;619:}
LABELV $173
endproc CG_GibPlayer 148 12
export CG_LaunchExplode
proc CG_LaunchExplode 20 8
line 626
;620:
;621:/*
;622:==================
;623:CG_LaunchGib
;624:==================
;625:*/
;626:void CG_LaunchExplode( vec3_t origin, vec3_t velocity, qhandle_t hModel ) {
line 630
;627:	localEntity_t	*le;
;628:	refEntity_t		*re;
;629:
;630:	le = CG_AllocLocalEntity();
ADDRLP4 8
ADDRGP4 CG_AllocLocalEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 631
;631:	re = &le->refEntity;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
line 633
;632:
;633:	le->leType = LE_FRAGMENT;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 3
ASGNI4
line 634
;634:	le->startTime = cg.time;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 cg+107604
INDIRI4
ASGNI4
line 635
;635:	le->endTime = le->startTime + 10000 + random() * 6000;
ADDRLP4 12
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 10000
ADDI4
CVIF4 4
CNSTF4 1169915904
ADDRLP4 12
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 637
;636:
;637:	VectorCopy( origin, re->origin );
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 638
;638:	AxisCopy( axisDefault, re->axis );
ADDRGP4 axisDefault
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 28
ADDP4
ARGP4
ADDRGP4 AxisCopy
CALLV
pop
line 639
;639:	re->hModel = hModel;
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 641
;640:
;641:	le->pos.trType = TR_GRAVITY;
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
CNSTI4 5
ASGNI4
line 642
;642:	VectorCopy( origin, le->pos.trBase );
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 643
;643:	VectorCopy( velocity, le->pos.trDelta );
ADDRLP4 0
INDIRP4
CNSTI4 56
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 644
;644:	le->pos.trTime = cg.time;
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRGP4 cg+107604
INDIRI4
ASGNI4
line 646
;645:
;646:	le->bounceFactor = 0.1f;
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
CNSTF4 1036831949
ASGNF4
line 648
;647:
;648:	le->leBounceSoundType = LEBS_BRASS;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 2
ASGNI4
line 649
;649:	le->leMarkType = LEMT_NONE;
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 0
ASGNI4
line 650
;650:}
LABELV $224
endproc CG_LaunchExplode 20 8
export CG_BigExplode
proc CG_BigExplode 92 12
line 661
;651:
;652:#define	EXP_VELOCITY	100
;653:#define	EXP_JUMP		150
;654:/*
;655:===================
;656:CG_GibPlayer
;657:
;658:Generated a bunch of gibs launching out from the bodies location
;659:===================
;660:*/
;661:void CG_BigExplode( vec3_t playerOrigin ) {
line 664
;662:	vec3_t	origin, velocity;
;663:
;664:	if ( !cg_blood.integer ) {
ADDRGP4 cg_blood+12
INDIRI4
CNSTI4 0
NEI4 $228
line 665
;665:		return;
ADDRGP4 $227
JUMPV
LABELV $228
line 668
;666:	}
;667:
;668:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 669
;669:	velocity[0] = crandom()*EXP_VELOCITY;
ADDRLP4 24
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 24
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 670
;670:	velocity[1] = crandom()*EXP_VELOCITY;
ADDRLP4 28
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 28
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 671
;671:	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
ADDRLP4 32
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 32
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1125515264
ADDF4
ASGNF4
line 672
;672:	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+176
INDIRI4
ARGI4
ADDRGP4 CG_LaunchExplode
CALLV
pop
line 674
;673:
;674:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 675
;675:	velocity[0] = crandom()*EXP_VELOCITY;
ADDRLP4 36
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 36
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 676
;676:	velocity[1] = crandom()*EXP_VELOCITY;
ADDRLP4 40
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 40
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ASGNF4
line 677
;677:	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
ADDRLP4 44
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 44
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1125515264
ADDF4
ASGNF4
line 678
;678:	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+176
INDIRI4
ARGI4
ADDRGP4 CG_LaunchExplode
CALLV
pop
line 680
;679:
;680:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 681
;681:	velocity[0] = crandom()*EXP_VELOCITY*1.5;
ADDRLP4 48
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1069547520
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 48
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
MULF4
ASGNF4
line 682
;682:	velocity[1] = crandom()*EXP_VELOCITY*1.5;
ADDRLP4 52
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1069547520
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 52
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
MULF4
ASGNF4
line 683
;683:	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
ADDRLP4 56
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 56
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1125515264
ADDF4
ASGNF4
line 684
;684:	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+176
INDIRI4
ARGI4
ADDRGP4 CG_LaunchExplode
CALLV
pop
line 686
;685:
;686:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 687
;687:	velocity[0] = crandom()*EXP_VELOCITY*2.0;
ADDRLP4 60
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 64
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
ADDRLP4 64
INDIRF4
CNSTF4 1120403456
ADDRLP4 64
INDIRF4
ADDRLP4 60
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
MULF4
ASGNF4
line 688
;688:	velocity[1] = crandom()*EXP_VELOCITY*2.0;
ADDRLP4 68
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 72
CNSTF4 1073741824
ASGNF4
ADDRLP4 0+4
ADDRLP4 72
INDIRF4
CNSTF4 1120403456
ADDRLP4 72
INDIRF4
ADDRLP4 68
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
MULF4
ASGNF4
line 689
;689:	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
ADDRLP4 76
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 76
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1125515264
ADDF4
ASGNF4
line 690
;690:	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+176
INDIRI4
ARGI4
ADDRGP4 CG_LaunchExplode
CALLV
pop
line 692
;691:
;692:	VectorCopy( playerOrigin, origin );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 693
;693:	velocity[0] = crandom()*EXP_VELOCITY*2.5;
ADDRLP4 80
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1075838976
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 80
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
MULF4
ASGNF4
line 694
;694:	velocity[1] = crandom()*EXP_VELOCITY*2.5;
ADDRLP4 84
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1075838976
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 84
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
MULF4
ASGNF4
line 695
;695:	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
ADDRLP4 88
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
CNSTF4 1120403456
CNSTF4 1073741824
ADDRLP4 88
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1125515264
ADDF4
ASGNF4
line 696
;696:	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 cgs+152340+176
INDIRI4
ARGI4
ADDRGP4 CG_LaunchExplode
CALLV
pop
line 697
;697:}
LABELV $227
endproc CG_BigExplode 92 12
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
import CG_AddLocalEntities
import CG_AllocLocalEntity
import CG_InitLocalEntities
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
LABELV $148
byte 1 67
byte 1 71
byte 1 95
byte 1 77
byte 1 97
byte 1 107
byte 1 101
byte 1 69
byte 1 120
byte 1 112
byte 1 108
byte 1 111
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 58
byte 1 32
byte 1 109
byte 1 115
byte 1 101
byte 1 99
byte 1 32
byte 1 61
byte 1 32
byte 1 37
byte 1 105
byte 1 0
