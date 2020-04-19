/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "client_render.h"

glconfig_t glConfig;

void RE_AddRefEntityToScene( const refEntity_t *ent ) {
#ifdef EMSCRIPTEN
	EM_ASM_({
	RE_AddRefEntityToScene($0, $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15);
	},
		ent->reType,
		ent->hModel,
		ent->origin[0], ent->origin[1], ent->origin[2],
		ent->axis[0][0], ent->axis[0][1], ent->axis[0][2],
		ent->axis[1][0], ent->axis[1][1], ent->axis[1][2],
		ent->axis[2][0], ent->axis[2][1], ent->axis[2][2],
		ent->renderfx,
		ent->frame
	);
#endif
}

void RE_AddDynamicLightToScene( const vec3_t org, float intensity, float r, float g, float b, int additive ) {

}

void RE_AddLightToScene( const vec3_t org, float intensity, float r, float g, float b ) {
	//RE_AddDynamicLightToScene( org, intensity, r, g, b, qfalse );
}

void RE_AddAdditiveLightToScene( const vec3_t org, float intensity, float r, float g, float b ) {
	RE_AddDynamicLightToScene( org, intensity, r, g, b, qtrue );
}


void RE_RenderScene( const refdef_t *fd ) {
}

#if 0
void R_AddBrushModelSurfaces ( trRefEntity_t *ent ) {

	bmodel_t	*bmodel;
	int			clip;
	model_t		*pModel;
	int			i;

	pModel = R_GetModelByHandle( ent->e.hModel );

	bmodel = pModel->bmodel;

	clip = R_CullLocalBox( bmodel->bounds );
	if ( clip == CULL_OUT ) {
		return;
	}
	
	R_SetupEntityLighting( &tr.refdef, ent );
	R_DlightBmodel( bmodel );

	for ( i = 0 ; i < bmodel->numSurfaces ; i++ ) {
		int surf = bmodel->firstSurface + i;

		if (tr.world->surfacesViewCount[surf] != tr.viewCount)
		{
			tr.world->surfacesViewCount[surf] = tr.viewCount;
			R_AddWorldSurface( tr.world->surfaces + surf, tr.currentEntity->needDlights, 0 );
		}
	}
}
#endif


qboolean R_inPVS( const vec3_t p1, const vec3_t p2 ) {
	return qtrue;
}

void R_AddWorldSurfaces (void) {

}

qhandle_t RE_RegisterModel( const char *name ) {
	qhandle_t ret = 0;

#ifdef EMSCRIPTEN
	ret = EM_ASM_INT({
		return RE_RegisterModel_callback($0);
	}, name);
#endif
	//Com_Printf("name=%s id=%d", name, ret);
	return ret;
}

// obsolete...
CCALL qhandle_t R_CustomMD3(char *name, int numVertices, int numTriangles, int numFrames, int tagsPerFrame) {
	return RE_RegisterModel(name);
}
