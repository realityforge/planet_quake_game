/*
===========================================================================

Wolfenstein: Enemy Territory GPL Source Code
Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company. 

This file is part of the Wolfenstein: Enemy Territory GPL Source Code (Wolf ET Source Code).  

Wolf ET Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Wolf ET Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Wolf ET Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Wolf: ET Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Wolf ET Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/


#include "tr_local.h"

int r_firstSceneDrawSurf;
#ifdef USE_PMLIGHT
int			r_firstSceneLitSurf;
#endif

int r_numdlights;
int r_firstSceneDlight;

int r_numcoronas;
int r_firstSceneCorona;

int r_numentities;
int r_firstSceneEntity;

int r_numpolys;
int r_firstScenePoly;

int r_numpolyverts;

// Gordon: TESTING
int r_firstScenePolybuffer;
int r_numpolybuffers;

// ydnar: decals
int r_firstSceneDecalProjector;
int r_numDecalProjectors;
int r_firstSceneDecal;

int skyboxportal;
/*
====================
R_InitNextFrame

====================
*/
void R_InitNextFrame( void ) {

	backEndData->commands.used = 0;

	r_firstSceneDrawSurf = 0;
#ifdef USE_PMLIGHT
	r_firstSceneLitSurf = 0;
#endif

	r_numdlights = 0;
	r_firstSceneDlight = 0;

	r_numcoronas = 0;
	r_firstSceneCorona = 0;

	r_numentities = 0;
	r_firstSceneEntity = 0;

	r_numpolys = 0;
	r_firstScenePoly = 0;

	r_numpolyverts = 0;

	// Gordon: TESTING
	r_numpolybuffers = 0;
	r_firstScenePolybuffer = 0;

	// ydnar: decals
	r_numDecalProjectors = 0;
	r_firstSceneDecalProjector = 0;
	r_firstSceneDecal = 0;
}


/*
====================
RE_ClearScene

====================
*/
void RE_ClearScene( void ) {
	int i;


	// ydnar: clear model stuff for dynamic fog
	if ( tr.world != NULL ) {
		for ( i = 0; i < tr.world->numBModels; i++ )
			tr.world->bmodels[ i ].visible = qfalse;
	}

	// everything else
	r_firstSceneDlight = r_numdlights;
	r_firstSceneCorona = r_numcoronas;
	r_firstSceneEntity = r_numentities;
	r_firstScenePoly = r_numpolys;
}

/*
===========================================================================

DISCRETE POLYS

===========================================================================
*/

/*
=====================
R_AddPolygonSurfaces

Adds all the scene's polys into this view's drawsurf list
=====================
*/
void R_AddPolygonSurfaces( void ) {
	int			i;
	shader_t	*sh;
	srfPoly_t	*poly;

	tr.currentEntityNum = REFENTITYNUM_WORLD;
	tr.shiftedEntityNum = tr.currentEntityNum << QSORT_REFENTITYNUM_SHIFT;

	for ( i = 0, poly = tr.refdef.polys; i < tr.refdef.numPolys ; i++, poly++ ) {
		sh = R_GetShaderByHandle( poly->hShader );
		R_AddDrawSurf( ( void * )poly, sh, poly->fogIndex, 0 );
	}
}

/*
=====================
RE_AddPolyToScene

=====================
*/
void RE_AddPolyToScene( qhandle_t hShader, int numVerts, const polyVert_t *verts ) {
	srfPoly_t   *poly;
	int i;
	int fogIndex;
	fog_t       *fog;
	vec3_t bounds[2];

	if ( !tr.registered ) {
		return;
	}
#if 0
	if ( !hShader ) {
		ri.Printf( PRINT_WARNING, "WARNING: RE_AddPolyToScene: NULL poly shader\n" );
		return;
	}
#endif
	if ( ( ( r_numpolyverts + numVerts ) > max_polyverts ) || ( r_numpolys >= max_polys ) ) {
		return;
	}

	poly = &backEndData->polys[r_numpolys];
	poly->surfaceType = SF_POLY;
	poly->hShader = hShader;
	poly->numVerts = numVerts;
	poly->verts = &backEndData->polyVerts[r_numpolyverts];

	memcpy( poly->verts, verts, numVerts * sizeof( *verts ) );
#if 0
	// Ridah
	if ( glConfig.hardwareType == GLHW_RAGEPRO ) {
		poly->verts->modulate[0] = 255;
		poly->verts->modulate[1] = 255;
		poly->verts->modulate[2] = 255;
		poly->verts->modulate[3] = 255;
	}
#endif
	// done.
	r_numpolys++;
	r_numpolyverts += numVerts;

	// see if it is in a fog volume
	if ( tr.world->numfogs == 1 ) {
		fogIndex = 0;
	} else {
		// find which fog volume the poly is in
		VectorCopy( poly->verts[0].xyz, bounds[0] );
		VectorCopy( poly->verts[0].xyz, bounds[1] );
		for ( i = 1 ; i < poly->numVerts ; i++ ) {
			AddPointToBounds( poly->verts[i].xyz, bounds[0], bounds[1] );
		}
		for ( fogIndex = 1 ; fogIndex < tr.world->numfogs ; fogIndex++ ) {
			fog = &tr.world->fogs[fogIndex];
			if ( bounds[1][0] >= fog->bounds[0][0]
				 && bounds[1][1] >= fog->bounds[0][1]
				 && bounds[1][2] >= fog->bounds[0][2]
				 && bounds[0][0] <= fog->bounds[1][0]
				 && bounds[0][1] <= fog->bounds[1][1]
				 && bounds[0][2] <= fog->bounds[1][2] ) {
				break;
			}
		}
		if ( fogIndex == tr.world->numfogs ) {
			fogIndex = 0;
		}
	}
	poly->fogIndex = fogIndex;
}

// Ridah
/*
=====================
RE_AddPolysToScene

=====================
*/
void RE_AddPolysToScene( qhandle_t hShader, int numVerts, const polyVert_t *verts, int numPolys ) {
	srfPoly_t   *poly;
	int i;
	int fogIndex;
	fog_t       *fog;
	vec3_t bounds[2];
	int j;

	if ( !tr.registered ) {
		return;
	}
#if 0
	if ( !hShader ) {
		ri.Printf( PRINT_WARNING, "WARNING: RE_AddPolysToScene: NULL poly shader\n" );
		return;
	}
#endif
	for ( j = 0; j < numPolys; j++ ) {
		if ( r_numpolyverts + numVerts > max_polyverts || r_numpolys >= max_polys ) {
//			ri.Printf( PRINT_WARNING, "WARNING: RE_AddPolysToScene: MAX_POLYS or MAX_POLYVERTS reached\n");
			return;
		}

		poly = &backEndData->polys[r_numpolys];
		poly->surfaceType = SF_POLY;
		poly->hShader = hShader;
		poly->numVerts = numVerts;
		poly->verts = &backEndData->polyVerts[r_numpolyverts];

		memcpy( poly->verts, &verts[numVerts * j], numVerts * sizeof( *verts ) );
#if 0
		// Ridah
		if ( glConfig.hardwareType == GLHW_RAGEPRO ) {
			poly->verts->modulate[0] = 255;
			poly->verts->modulate[1] = 255;
			poly->verts->modulate[2] = 255;
			poly->verts->modulate[3] = 255;
		}
#endif
		// done.
		r_numpolys++;
		r_numpolyverts += numVerts;

		// if no world is loaded
		if ( tr.world == NULL ) {
			fogIndex = 0;
		}
		// see if it is in a fog volume
		else if ( tr.world->numfogs == 1 ) {
			fogIndex = 0;
		} else {
			// find which fog volume the poly is in
			VectorCopy( poly->verts[0].xyz, bounds[0] );
			VectorCopy( poly->verts[0].xyz, bounds[1] );
			for ( i = 1 ; i < poly->numVerts ; i++ ) {
				AddPointToBounds( poly->verts[i].xyz, bounds[0], bounds[1] );
			}
			for ( fogIndex = 1 ; fogIndex < tr.world->numfogs ; fogIndex++ ) {
				fog = &tr.world->fogs[fogIndex];
				if ( bounds[1][0] >= fog->bounds[0][0]
					 && bounds[1][1] >= fog->bounds[0][1]
					 && bounds[1][2] >= fog->bounds[0][2]
					 && bounds[0][0] <= fog->bounds[1][0]
					 && bounds[0][1] <= fog->bounds[1][1]
					 && bounds[0][2] <= fog->bounds[1][2] ) {
					break;
				}
			}
			if ( fogIndex == tr.world->numfogs ) {
				fogIndex = 0;
			}
		}
		poly->fogIndex = fogIndex;
	}
}
// done.


/*
=====================
R_AddPolygonSurfaces

Adds all the scene's polys into this view's drawsurf list
=====================
*/
void R_AddPolygonBufferSurfaces( void ) {
	int i;
	shader_t        *sh;
	srfPolyBuffer_t *polybuffer;

	tr.currentEntityNum = REFENTITYNUM_WORLD;
	tr.shiftedEntityNum = tr.currentEntityNum << QSORT_REFENTITYNUM_SHIFT;

	for ( i = 0, polybuffer = tr.refdef.polybuffers; i < tr.refdef.numPolyBuffers ; i++, polybuffer++ ) {
		sh = R_GetShaderByHandle( polybuffer->pPolyBuffer->shader );

		R_AddDrawSurf( ( void * )polybuffer, sh, polybuffer->fogIndex, 0 );
	}
}

/*
=====================
RE_AddPolyBufferToScene

=====================
*/
void RE_AddPolyBufferToScene( polyBuffer_t* pPolyBuffer ) {
	srfPolyBuffer_t*    pPolySurf;
	int fogIndex;
	fog_t*              fog;
	vec3_t bounds[2];
	int i;

	if ( r_numpolybuffers >= MAX_POLYS ) {
		return;
	}

	pPolySurf = &backEndData->polybuffers[r_numpolybuffers];
	r_numpolybuffers++;

	pPolySurf->surfaceType = SF_POLYBUFFER;
	pPolySurf->pPolyBuffer = pPolyBuffer;

	VectorCopy( pPolyBuffer->xyz[0], bounds[0] );
	VectorCopy( pPolyBuffer->xyz[0], bounds[1] );
	for ( i = 1 ; i < pPolyBuffer->numVerts ; i++ ) {
		AddPointToBounds( pPolyBuffer->xyz[i], bounds[0], bounds[1] );
	}
	for ( fogIndex = 1 ; fogIndex < tr.world->numfogs ; fogIndex++ ) {
		fog = &tr.world->fogs[fogIndex];
		if ( bounds[1][0] >= fog->bounds[0][0]
			 && bounds[1][1] >= fog->bounds[0][1]
			 && bounds[1][2] >= fog->bounds[0][2]
			 && bounds[0][0] <= fog->bounds[1][0]
			 && bounds[0][1] <= fog->bounds[1][1]
			 && bounds[0][2] <= fog->bounds[1][2] ) {
			break;
		}
	}
	if ( fogIndex == tr.world->numfogs ) {
		fogIndex = 0;
	}

	pPolySurf->fogIndex = fogIndex;
}


//=================================================================================

static int isnan_fp( const float *f )
{
	uint32_t u = *( (uint32_t*) f );
	u = 0x7F800000 - ( u & 0x7FFFFFFF );
	return (int)( u >> 31 );
}


/*
=====================
RE_AddRefEntityToScene
=====================
*/
void RE_AddRefEntityToScene( const refEntity_t *ent, qboolean intShaderTime ) {
	if ( !tr.registered ) {
		return;
	}
	if ( r_numentities >= MAX_REFENTITIES ) {
		ri.Printf( PRINT_DEVELOPER, "RE_AddRefEntityToScene: Dropping refEntity, reached MAX_REFENTITIES\n" );
		return;
	}
	if ( isnan_fp( &ent->origin[0] ) || isnan_fp( &ent->origin[1] ) || isnan_fp( &ent->origin[2] ) ) {
		static qboolean first_time = qtrue;
		if ( first_time ) {
			first_time = qfalse;
			ri.Printf( PRINT_WARNING, "RE_AddRefEntityToScene passed a refEntity which has an origin with a NaN component\n" );
		}
		return;
	}
	if ( (unsigned)ent->reType >= RT_MAX_REF_ENTITY_TYPE ) {
		ri.Error( ERR_DROP, "RE_AddRefEntityToScene: bad reType %i", ent->reType );
	}

	backEndData->entities[r_numentities].e = *ent;
	backEndData->entities[r_numentities].lightingCalculated = qfalse;
	backEndData->entities[r_numentities].intShaderTime = intShaderTime;

	r_numentities++;

	// ydnar: add projected shadows for this model
	// Arnout: casting const away
	R_AddModelShadow( (refEntity_t*) ent );
}

extern qboolean modIsETF;

/*
RE_AddLightToScene()
ydnar: modified dlight system to support seperate radius and intensity
*/

void RE_AddLightToScene( const vec3_t org, float radius, float intensity, float r, float g, float b, qhandle_t hShader, int flags ) {
	dlight_t    *dl;


	// early out
	if ( !tr.registered || r_numdlights >= ARRAY_LEN( backEndData->dlights ) || radius <= 0 || intensity <= 0 ) {
		return;
	}

	// these cards don't have the correct blend mode
	if ( glConfig.hardwareType == GLHW_RIVA128 || glConfig.hardwareType == GLHW_PERMEDIA2 ) {
		return;
	}

	// RF, allow us to force some dlights under all circumstances
	if ( !( flags & REF_FORCE_DLIGHT ) ) {
		if ( r_dynamiclight->integer == 0 ) {
			return;
		}
	}
#ifdef USE_PMLIGHT
	if ( r_dlightMode->integer ) {
		// ETF powerup hack
		if ( modIsETF && ( radius == 200 || radius == 150 ) && intensity >= 1.25f )
			intensity = 1.25f;

		// ENSI NOTE FIXME this is quick and dirty hack to support intensity dlights (dyna)
		r *= intensity;
		g *= intensity;
		b *= intensity;
		// END ENSI

		r *= r_dlightIntensity->value;
		g *= r_dlightIntensity->value;
		b *= r_dlightIntensity->value;
	}
#endif

	// set up a new dlight
	dl = &backEndData->dlights[ r_numdlights++ ];
	VectorCopy( org, dl->origin );
	VectorCopy( org, dl->transformed );
	dl->radius = radius;
	dl->radiusInverseCubed = ( 1.0 / dl->radius );
	dl->radiusInverseCubed = dl->radiusInverseCubed * dl->radiusInverseCubed * dl->radiusInverseCubed;
	dl->intensity = intensity;
	dl->color[ 0 ] = r;
	dl->color[ 1 ] = g;
	dl->color[ 2 ] = b;
	dl->shader = R_GetShaderByHandle( hShader );
	if ( dl->shader == tr.defaultShader ) {
		dl->shader = NULL;
	}
	dl->flags = flags;
	dl->linear = qfalse;
}


/*
=====================
RE_AddLinearLightToScene
=====================
*/
void RE_AddLinearLightToScene( const vec3_t start, const vec3_t end, float intensity, float r, float g, float b  ) {
	dlight_t	*dl;
	if ( VectorCompare( start, end ) ) {
		RE_AddLightToScene( start, intensity, intensity, r, g, b, 0, 0 );
		return;
	}
	if ( !tr.registered ) {
		return;
	}
	if ( r_numdlights >= ARRAY_LEN( backEndData->dlights ) ) {
		return;
	}
	if ( intensity <= 0 ) {
		return;
	}
#ifdef USE_PMLIGHT
	if ( r_dlightMode->integer ) {
		r *= r_dlightIntensity->value;
		g *= r_dlightIntensity->value;
		b *= r_dlightIntensity->value;
	}
#endif
	dl = &backEndData->dlights[ r_numdlights++ ];
	VectorCopy( start, dl->origin );
	VectorCopy( end, dl->origin2 );
	dl->radius = intensity;
	dl->radiusInverseCubed = ( 1.0 / dl->radius );
	dl->radiusInverseCubed = dl->radiusInverseCubed * dl->radiusInverseCubed * dl->radiusInverseCubed;
	dl->intensity = intensity;
	dl->color[0] = r;
	dl->color[1] = g;
	dl->color[2] = b;
	dl->shader = NULL;
	dl->flags = 0;
	dl->linear = qtrue;
}



/*
==============
RE_AddCoronaToScene
==============
*/
void RE_AddCoronaToScene( const vec3_t org, float r, float g, float b, float scale, int id, qboolean visible ) {
	corona_t    *cor;

	if ( !tr.registered ) {
		return;
	}
	if ( r_numcoronas >= MAX_CORONAS ) {
		return;
	}

	cor = &backEndData->coronas[r_numcoronas++];
	VectorCopy( org, cor->origin );
	cor->color[0] = r;
	cor->color[1] = g;
	cor->color[2] = b;
	cor->scale = scale;
	cor->id = id;
	cor->visible = visible;
}

/*
@@@@@@@@@@@@@@@@@@@@@
RE_RenderScene

Draw a 3D view into a part of the window, then return
to 2D drawing.

Rendering a scene may require multiple views to be rendered
to handle mirrors,
@@@@@@@@@@@@@@@@@@@@@
*/
void RE_RenderScene( const refdef_t *fd ) {
	viewParms_t parms;
	int startTime;

	if ( !tr.registered ) {
		return;
	}

	if ( r_norefresh->integer ) {
		return;
	}

	startTime = ri.Milliseconds();

	if ( !tr.world && !( fd->rdflags & RDF_NOWORLDMODEL ) ) {
		ri.Error( ERR_DROP, "R_RenderScene: NULL worldmodel" );
	}

	memcpy( tr.refdef.text, fd->text, sizeof( tr.refdef.text ) );

	tr.refdef.x = fd->x;
	tr.refdef.y = fd->y;
	tr.refdef.width = fd->width;
	tr.refdef.height = fd->height;
	tr.refdef.fov_x = fd->fov_x;
	tr.refdef.fov_y = fd->fov_y;

	VectorCopy( fd->vieworg, tr.refdef.vieworg );
	VectorCopy( fd->viewaxis[0], tr.refdef.viewaxis[0] );
	VectorCopy( fd->viewaxis[1], tr.refdef.viewaxis[1] );
	VectorCopy( fd->viewaxis[2], tr.refdef.viewaxis[2] );

	tr.refdef.time = fd->time;
	tr.refdef.rdflags = fd->rdflags;

	if ( fd->rdflags & RDF_SKYBOXPORTAL ) {
		skyboxportal = 1;
	}

	// copy the areamask data over and note if it has changed, which
	// will force a reset of the visible leafs even if the view hasn't moved
	tr.refdef.areamaskModified = qfalse;
	if ( !( tr.refdef.rdflags & RDF_NOWORLDMODEL ) ) {
		int areaDiff;
		int i;

		// compare the area bits
		areaDiff = 0;
		for ( i = 0 ; i < MAX_MAP_AREA_BYTES / 4 ; i++ ) {
			areaDiff |= ( (int *)tr.refdef.areamask )[i] ^ ( (int *)fd->areamask )[i];
			( (int *)tr.refdef.areamask )[i] = ( (int *)fd->areamask )[i];
		}

		if ( areaDiff ) {
			// a door just opened or something
			tr.refdef.areamaskModified = qtrue;
		}
	}


	// derived info

	tr.refdef.floatTime = (double)tr.refdef.time * 0.001; // -EC-: cast to double

	tr.refdef.numDrawSurfs = r_firstSceneDrawSurf;
	tr.refdef.drawSurfs = backEndData->drawSurfs;

#ifdef USE_PMLIGHT
	tr.refdef.numLitSurfs = r_firstSceneLitSurf;
	tr.refdef.litSurfs = backEndData->litSurfs;
#endif

	tr.refdef.num_entities = r_numentities - r_firstSceneEntity;
	tr.refdef.entities = &backEndData->entities[r_firstSceneEntity];

	tr.refdef.num_dlights = r_numdlights - r_firstSceneDlight;
	tr.refdef.dlights = &backEndData->dlights[r_firstSceneDlight];
	tr.refdef.dlightBits = 0;


	tr.refdef.num_coronas = r_numcoronas - r_firstSceneCorona;
	tr.refdef.coronas = &backEndData->coronas[r_firstSceneCorona];

	tr.refdef.numPolys = r_numpolys - r_firstScenePoly;
	tr.refdef.polys = &backEndData->polys[r_firstScenePoly];

	tr.refdef.numPolyBuffers = r_numpolybuffers - r_firstScenePolybuffer;
	tr.refdef.polybuffers = &backEndData->polybuffers[r_firstScenePolybuffer];

	tr.refdef.numDecalProjectors = r_numDecalProjectors - r_firstSceneDecalProjector;
	tr.refdef.decalProjectors = &backEndData->decalProjectors[ r_firstSceneDecalProjector ];

	tr.refdef.numDecals = 0;
	tr.refdef.decals = &backEndData->decals[ r_firstSceneDecal ];

	// turn off dynamic lighting globally by clearing all the
	// dlights if using permedia hw
	if ( glConfig.hardwareType == GLHW_PERMEDIA2 ) {
		tr.refdef.num_dlights = 0;
	}

	// a single frame may have multiple scenes draw inside it --
	// a 3D game view, 3D status bar renderings, 3D menus, etc.
	// They need to be distinguished by the light flare code, because
	// the visibility state for a given surface may be different in
	// each scene / view.
	tr.frameSceneNum++;
	tr.sceneCount++;

	// setup view parms for the initial view
	//
	// set up viewport
	// The refdef takes 0-at-the-top y coordinates, so
	// convert to GL's 0-at-the-bottom space
	//
	memset( &parms, 0, sizeof( parms ) );
	parms.viewportX = tr.refdef.x;
	parms.viewportY = glConfig.vidHeight - ( tr.refdef.y + tr.refdef.height );
	parms.viewportWidth = tr.refdef.width;
	parms.viewportHeight = tr.refdef.height;

	parms.scissorX = parms.viewportX;
	parms.scissorY = parms.viewportY;
	parms.scissorWidth = parms.viewportWidth;
	parms.scissorHeight = parms.viewportHeight;

	parms.isPortal = qfalse;

#ifdef USE_PMLIGHT
	parms.dlights = tr.refdef.dlights;
	parms.num_dlights = tr.refdef.num_dlights;
#endif

	parms.fovX = tr.refdef.fov_x;
	parms.fovY = tr.refdef.fov_y;
	
	parms.stereoFrame = tr.refdef.stereoFrame;

	VectorCopy( fd->vieworg, parms.orientation.origin );
	VectorCopy( fd->viewaxis[0], parms.orientation.axis[0] );
	VectorCopy( fd->viewaxis[1], parms.orientation.axis[1] );
	VectorCopy( fd->viewaxis[2], parms.orientation.axis[2] );

	VectorCopy( fd->vieworg, parms.pvsOrigin );

	R_RenderView( &parms );

	if ( fd->rdflags & RDF_RENDEROMNIBOT )
		R_RenderOmnibot();

	// the next scene rendered in this frame will tack on after this one
	r_firstSceneDrawSurf = tr.refdef.numDrawSurfs;
#ifdef USE_PMLIGHT
	r_firstSceneLitSurf = tr.refdef.numLitSurfs;
#endif
	r_firstSceneDecal += tr.refdef.numDecals;
	r_firstSceneEntity = r_numentities;
	r_firstSceneDlight = r_numdlights;
	r_firstScenePoly = r_numpolys;
	r_firstScenePolybuffer = r_numpolybuffers;

	tr.frontEndMsec += ri.Milliseconds() - startTime;
}
