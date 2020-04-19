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

/*****************************************************************************
 * name:		be_aas_debug.c
 *
 * desc:		AAS debug code
 *
 * $Archive: /MissionPack/code/botlib/be_aas_debug.c $
 *
 *****************************************************************************/

#include <q_shared.h>

#define MAX_DEBUGLINES				1024
#define MAX_DEBUGPOLYGONS			8192

int debuglines[MAX_DEBUGLINES];
int debuglinevisible[MAX_DEBUGLINES];
int numdebuglines;

static int debugpolygons[MAX_DEBUGPOLYGONS];

#if 0
void AAS_ClearShownPolygons(void)
{
	int i;
//*
	for (i = 0; i < MAX_DEBUGPOLYGONS; i++)
	{
		if (debugpolygons[i]) botimport.DebugPolygonDelete(debugpolygons[i]);
		debugpolygons[i] = 0;
	} //end for
//*/
/*
	for (i = 0; i < MAX_DEBUGPOLYGONS; i++)
	{
		botimport.DebugPolygonDelete(i);
		debugpolygons[i] = 0;
	} //end for
*/
} //end of the function AAS_ClearShownPolygons

void AAS_ShowPolygon(int color, int numpoints, vec3_t *points)
{
	int i;

	for (i = 0; i < MAX_DEBUGPOLYGONS; i++)
	{
		if (!debugpolygons[i])
		{
			debugpolygons[i] = botimport.DebugPolygonCreate(color, numpoints, points);
			break;
		} //end if
	} //end for
} //end of the function AAS_ShowPolygon

void AAS_ClearShownDebugLines(void)
{
	int i;

	//make all lines invisible
	for (i = 0; i < MAX_DEBUGLINES; i++)
	{
		if (debuglines[i])
		{
			//botimport.DebugLineShow(debuglines[i], NULL, NULL, LINECOLOR_NONE);
			botimport.DebugLineDelete(debuglines[i]);
			debuglines[i] = 0;
			debuglinevisible[i] = qfalse;
		} //end if
	} //end for
} //end of the function AAS_ClearShownDebugLines

void AAS_DebugLine(vec3_t start, vec3_t end, int color)
{
	int line;

	for (line = 0; line < MAX_DEBUGLINES; line++)
	{
		if (!debuglines[line])
		{
			debuglines[line] = botimport.DebugLineCreate();
			debuglinevisible[line] = qfalse;
			numdebuglines++;
		} //end if
		if (!debuglinevisible[line])
		{
			botimport.DebugLineShow(debuglines[line], start, end, color);
			debuglinevisible[line] = qtrue;
			return;
		} //end else
	} //end for
} //end of the function AAS_DebugLine

void AAS_PermanentLine(vec3_t start, vec3_t end, int color)
{
	int line;

	line = botimport.DebugLineCreate();
	botimport.DebugLineShow(line, start, end, color);
} //end of the function AAS_PermenentLine

void AAS_DrawPermanentCross(vec3_t origin, float size, int color)
{
	int i, debugline;
	vec3_t start, end;

	for (i = 0; i < 3; i++)
	{
		VectorCopy(origin, start);
		start[i] += size;
		VectorCopy(origin, end);
		end[i] -= size;
		AAS_DebugLine(start, end, color);
		debugline = botimport.DebugLineCreate();
		botimport.DebugLineShow(debugline, start, end, color);
	} //end for
} //end of the function AAS_DrawPermanentCross

void AAS_DrawPlaneCross(vec3_t point, vec3_t normal, float dist, int type, int color)
{
	int n0, n1, n2, j, line, lines[2];
	vec3_t start1, end1, start2, end2;

	//make a cross in the hit plane at the hit point
	VectorCopy(point, start1);
	VectorCopy(point, end1);
	VectorCopy(point, start2);
	VectorCopy(point, end2);

	n0 = type % 3;
	n1 = (type + 1) % 3;
	n2 = (type + 2) % 3;
	start1[n1] -= 6;
	start1[n2] -= 6;
	end1[n1] += 6;
	end1[n2] += 6;
	start2[n1] += 6;
	start2[n2] -= 6;
	end2[n1] -= 6;
	end2[n2] += 6;

	start1[n0] = (dist - (start1[n1] * normal[n1] +
				start1[n2] * normal[n2])) / normal[n0];
	end1[n0] = (dist - (end1[n1] * normal[n1] +
				end1[n2] * normal[n2])) / normal[n0];
	start2[n0] = (dist - (start2[n1] * normal[n1] +
				start2[n2] * normal[n2])) / normal[n0];
	end2[n0] = (dist - (end2[n1] * normal[n1] +
				end2[n2] * normal[n2])) / normal[n0];

	for (j = 0, line = 0; j < 2 && line < MAX_DEBUGLINES; line++)
	{
		if (!debuglines[line])
		{
			debuglines[line] = botimport.DebugLineCreate();
			lines[j++] = debuglines[line];
			debuglinevisible[line] = qtrue;
			numdebuglines++;
		} //end if
		else if (!debuglinevisible[line])
		{
			lines[j++] = debuglines[line];
			debuglinevisible[line] = qtrue;
		} //end else
	} //end for
	botimport.DebugLineShow(lines[0], start1, end1, color);
	botimport.DebugLineShow(lines[1], start2, end2, color);
} //end of the function AAS_DrawPlaneCross

void AAS_ShowBoundingBox(vec3_t origin, vec3_t mins, vec3_t maxs)
{
	vec3_t bboxcorners[8];
	int lines[3];
	int i, j, line;

	//upper corners
	bboxcorners[0][0] = origin[0] + maxs[0];
	bboxcorners[0][1] = origin[1] + maxs[1];
	bboxcorners[0][2] = origin[2] + maxs[2];
	//
	bboxcorners[1][0] = origin[0] + mins[0];
	bboxcorners[1][1] = origin[1] + maxs[1];
	bboxcorners[1][2] = origin[2] + maxs[2];
	//
	bboxcorners[2][0] = origin[0] + mins[0];
	bboxcorners[2][1] = origin[1] + mins[1];
	bboxcorners[2][2] = origin[2] + maxs[2];
	//
	bboxcorners[3][0] = origin[0] + maxs[0];
	bboxcorners[3][1] = origin[1] + mins[1];
	bboxcorners[3][2] = origin[2] + maxs[2];
	//lower corners
	Com_Memcpy(bboxcorners[4], bboxcorners[0], sizeof(vec3_t) * 4);
	for (i = 0; i < 4; i++) bboxcorners[4 + i][2] = origin[2] + mins[2];
	//draw bounding box
	for (i = 0; i < 4; i++)
	{
		for (j = 0, line = 0; j < 3 && line < MAX_DEBUGLINES; line++)
		{
			if (!debuglines[line])
			{
				debuglines[line] = botimport.DebugLineCreate();
				lines[j++] = debuglines[line];
				debuglinevisible[line] = qtrue;
				numdebuglines++;
			} //end if
			else if (!debuglinevisible[line])
			{
				lines[j++] = debuglines[line];
				debuglinevisible[line] = qtrue;
			} //end else
		} //end for
		//top plane
		botimport.DebugLineShow(lines[0], bboxcorners[i],
									bboxcorners[(i+1)&3], LINECOLOR_RED);
		//bottom plane
		botimport.DebugLineShow(lines[1], bboxcorners[4+i],
									bboxcorners[4+((i+1)&3)], LINECOLOR_RED);
		//vertical lines
		botimport.DebugLineShow(lines[2], bboxcorners[i],
									bboxcorners[4+i], LINECOLOR_RED);
	} //end for
} //end of the function AAS_ShowBoundingBox

void AAS_ShowFace(int facenum)
{
	int i, color, edgenum;
	aas_edge_t *edge;
	aas_face_t *face;
	aas_plane_t *plane;
	vec3_t start, end;

	color = LINECOLOR_YELLOW;
	//check if face number is in range
	if (facenum >= aasworld.numfaces)
	{
		botimport.Print(PRT_ERROR, "facenum %d out of range\n", facenum);
	} //end if
	face = &aasworld.faces[facenum];
	//walk through the edges of the face
	for (i = 0; i < face->numedges; i++)
	{
		//edge number
		edgenum = abs(aasworld.edgeindex[face->firstedge + i]);
		//check if edge number is in range
		if (edgenum >= aasworld.numedges)
		{
			botimport.Print(PRT_ERROR, "edgenum %d out of range\n", edgenum);
		} //end if
		edge = &aasworld.edges[edgenum];
		if (color == LINECOLOR_RED) color = LINECOLOR_GREEN;
		else if (color == LINECOLOR_GREEN) color = LINECOLOR_BLUE;
		else if (color == LINECOLOR_BLUE) color = LINECOLOR_YELLOW;
		else color = LINECOLOR_RED;
		AAS_DebugLine(aasworld.vertexes[edge->v[0]],
										aasworld.vertexes[edge->v[1]],
										color);
	} //end for
	plane = &aasworld.planes[face->planenum];
	edgenum = abs(aasworld.edgeindex[face->firstedge]);
	edge = &aasworld.edges[edgenum];
	VectorCopy(aasworld.vertexes[edge->v[0]], start);
	VectorMA(start, 20, plane->normal, end);
	AAS_DebugLine(start, end, LINECOLOR_RED);
} //end of the function AAS_ShowFace

void AAS_ShowFacePolygon(int facenum, int color, int flip)
{
	int i, edgenum, numpoints;
	vec3_t points[128];
	aas_edge_t *edge;
	aas_face_t *face;

	//check if face number is in range
	if (facenum >= aasworld.numfaces)
	{
		botimport.Print(PRT_ERROR, "facenum %d out of range\n", facenum);
	} //end if
	face = &aasworld.faces[facenum];
	//walk through the edges of the face
	numpoints = 0;
	if (flip)
	{
		for (i = face->numedges-1; i >= 0; i--)
		{
			//edge number
			edgenum = aasworld.edgeindex[face->firstedge + i];
			edge = &aasworld.edges[abs(edgenum)];
			VectorCopy(aasworld.vertexes[edge->v[edgenum < 0]], points[numpoints]);
			numpoints++;
		} //end for
	} //end if
	else
	{
		for (i = 0; i < face->numedges; i++)
		{
			//edge number
			edgenum = aasworld.edgeindex[face->firstedge + i];
			edge = &aasworld.edges[abs(edgenum)];
			VectorCopy(aasworld.vertexes[edge->v[edgenum < 0]], points[numpoints]);
			numpoints++;
		} //end for
	} //end else
	AAS_ShowPolygon(color, numpoints, points);
} //end of the function AAS_ShowFacePolygon

void AAS_ShowArea(int areanum, int groundfacesonly)
{
	int areaedges[MAX_DEBUGLINES];
	int numareaedges, i, j, n, color = 0, line;
	int facenum, edgenum;
	aas_area_t *area;
	aas_face_t *face;
	aas_edge_t *edge;

	//
	numareaedges = 0;
	//
	if (areanum < 0 || areanum >= aasworld.numareas)
	{
		botimport.Print(PRT_ERROR, "area %d out of range [0, %d]\n",
								areanum, aasworld.numareas);
		return;
	} //end if
	//pointer to the convex area
	area = &aasworld.areas[areanum];
	//walk through the faces of the area
	for (i = 0; i < area->numfaces; i++)
	{
		facenum = abs(aasworld.faceindex[area->firstface + i]);
		//check if face number is in range
		if (facenum >= aasworld.numfaces)
		{
			botimport.Print(PRT_ERROR, "facenum %d out of range\n", facenum);
		} //end if
		face = &aasworld.faces[facenum];
		//ground faces only
		if (groundfacesonly)
		{
			if (!(face->faceflags & (FACE_GROUND | FACE_LADDER))) continue;
		} //end if
		//walk through the edges of the face
		for (j = 0; j < face->numedges; j++)
		{
			//edge number
			edgenum = abs(aasworld.edgeindex[face->firstedge + j]);
			//check if edge number is in range
			if (edgenum >= aasworld.numedges)
			{
				botimport.Print(PRT_ERROR, "edgenum %d out of range\n", edgenum);
			} //end if
			//check if the edge is stored already
			for (n = 0; n < numareaedges; n++)
			{
				if (areaedges[n] == edgenum) break;
			} //end for
			if (n == numareaedges && numareaedges < MAX_DEBUGLINES)
			{
				areaedges[numareaedges++] = edgenum;
			} //end if
		} //end for
		//AAS_ShowFace(facenum);
	} //end for
	//draw all the edges
	for (n = 0; n < numareaedges; n++)
	{
		for (line = 0; line < MAX_DEBUGLINES; line++)
		{
			if (!debuglines[line])
			{
				debuglines[line] = botimport.DebugLineCreate();
				debuglinevisible[line] = qfalse;
				numdebuglines++;
			} //end if
			if (!debuglinevisible[line])
			{
				break;
			} //end else
		} //end for
		if (line >= MAX_DEBUGLINES) return;
		edge = &aasworld.edges[areaedges[n]];
		if (color == LINECOLOR_RED) color = LINECOLOR_BLUE;
		else if (color == LINECOLOR_BLUE) color = LINECOLOR_GREEN;
		else if (color == LINECOLOR_GREEN) color = LINECOLOR_YELLOW;
		else color = LINECOLOR_RED;
		botimport.DebugLineShow(debuglines[line],
									aasworld.vertexes[edge->v[0]],
									aasworld.vertexes[edge->v[1]],
									color);
		debuglinevisible[line] = qtrue;
	} //end for*/
} //end of the function AAS_ShowArea

void AAS_ShowAreaPolygons(int areanum, int color, int groundfacesonly)
{
	int i, facenum;
	aas_area_t *area;
	aas_face_t *face;

	//
	if (areanum < 0 || areanum >= aasworld.numareas)
	{
		botimport.Print(PRT_ERROR, "area %d out of range [0, %d]\n",
								areanum, aasworld.numareas);
		return;
	} //end if
	//pointer to the convex area
	area = &aasworld.areas[areanum];
	//walk through the faces of the area
	for (i = 0; i < area->numfaces; i++)
	{
		facenum = abs(aasworld.faceindex[area->firstface + i]);
		//check if face number is in range
		if (facenum >= aasworld.numfaces)
		{
			botimport.Print(PRT_ERROR, "facenum %d out of range\n", facenum);
		} //end if
		face = &aasworld.faces[facenum];
		//ground faces only
		if (groundfacesonly)
		{
			if (!(face->faceflags & (FACE_GROUND | FACE_LADDER))) continue;
		} //end if
		AAS_ShowFacePolygon(facenum, color, face->frontarea != areanum);
	} //end for
} //end of the function AAS_ShowAreaPolygons

void AAS_DrawCross(vec3_t origin, float size, int color)
{
	int i;
	vec3_t start, end;

	for (i = 0; i < 3; i++)
	{
		VectorCopy(origin, start);
		start[i] += size;
		VectorCopy(origin, end);
		end[i] -= size;
		AAS_DebugLine(start, end, color);
	} //end for
} //end of the function AAS_DrawCross

void AAS_DrawArrow(vec3_t start, vec3_t end, int linecolor, int arrowcolor)
{
	vec3_t dir, cross, p1, p2, up = {0, 0, 1};
	float dot;

	VectorSubtract(end, start, dir);
	VectorNormalize(dir);
	dot = DotProduct(dir, up);
	if (dot > 0.99 || dot < -0.99) VectorSet(cross, 1, 0, 0);
	else CrossProduct(dir, up, cross);

	VectorMA(end, -6, dir, p1);
	VectorCopy(p1, p2);
	VectorMA(p1, 6, cross, p1);
	VectorMA(p2, -6, cross, p2);

	AAS_DebugLine(start, end, linecolor);
	AAS_DebugLine(p1, end, arrowcolor);
	AAS_DebugLine(p2, end, arrowcolor);
} //end of the function AAS_DrawArrow

void AAS_ShowReachability(aas_reachability_t *reach)
{
	vec3_t dir, cmdmove, velocity;
	float speed, zvel;
	aas_clientmove_t move;

	AAS_ShowAreaPolygons(reach->areanum, 5, qtrue);
	//AAS_ShowArea(reach->areanum, qtrue);
	AAS_DrawArrow(reach->start, reach->end, LINECOLOR_BLUE, LINECOLOR_YELLOW);
	//
	if ((reach->traveltype & TRAVELTYPE_MASK) == TRAVEL_JUMP ||
		(reach->traveltype & TRAVELTYPE_MASK) == TRAVEL_WALKOFFLEDGE)
	{
		AAS_HorizontalVelocityForJump(aassettings.phys_jumpvel, reach->start, reach->end, &speed);
		//
		VectorSubtract(reach->end, reach->start, dir);
		dir[2] = 0;
		VectorNormalize(dir);
		//set the velocity
		VectorScale(dir, speed, velocity);
		//set the command movement
		VectorClear(cmdmove);
		cmdmove[2] = aassettings.phys_jumpvel;
		//
		AAS_PredictClientMovement(&move, -1, reach->start, PRESENCE_NORMAL, qtrue,
									velocity, cmdmove, 3, 30, 0.1f,
									SE_HITGROUND|SE_ENTERWATER|SE_ENTERSLIME|
									SE_ENTERLAVA|SE_HITGROUNDDAMAGE, 0, qtrue);
		//
		if ((reach->traveltype & TRAVELTYPE_MASK) == TRAVEL_JUMP)
		{
			AAS_JumpReachRunStart(reach, dir);
			AAS_DrawCross(dir, 4, LINECOLOR_BLUE);
		} //end if
	} //end if
	else if ((reach->traveltype & TRAVELTYPE_MASK) == TRAVEL_ROCKETJUMP)
	{
		zvel = AAS_RocketJumpZVelocity(reach->start);
		AAS_HorizontalVelocityForJump(zvel, reach->start, reach->end, &speed);
		//
		VectorSubtract(reach->end, reach->start, dir);
		dir[2] = 0;
		VectorNormalize(dir);
		//get command movement
		VectorScale(dir, speed, cmdmove);
		VectorSet(velocity, 0, 0, zvel);
		//
		AAS_PredictClientMovement(&move, -1, reach->start, PRESENCE_NORMAL, qtrue,
									velocity, cmdmove, 30, 30, 0.1f,
									SE_ENTERWATER|SE_ENTERSLIME|
									SE_ENTERLAVA|SE_HITGROUNDDAMAGE|
									SE_TOUCHJUMPPAD|SE_HITGROUNDAREA, reach->areanum, qtrue);
	} //end else if
	else if ((reach->traveltype & TRAVELTYPE_MASK) == TRAVEL_JUMPPAD)
	{
		VectorSet(cmdmove, 0, 0, 0);
		//
		VectorSubtract(reach->end, reach->start, dir);
		dir[2] = 0;
		VectorNormalize(dir);
		//set the velocity
		//NOTE: the edgenum is the horizontal velocity
		VectorScale(dir, reach->edgenum, velocity);
		//NOTE: the facenum is the Z velocity
		velocity[2] = reach->facenum;
		//
		AAS_PredictClientMovement(&move, -1, reach->start, PRESENCE_NORMAL, qtrue,
									velocity, cmdmove, 30, 30, 0.1f,
									SE_ENTERWATER|SE_ENTERSLIME|
									SE_ENTERLAVA|SE_HITGROUNDDAMAGE|
									SE_TOUCHJUMPPAD|SE_HITGROUNDAREA, reach->areanum, qtrue);
	} //end else if
} //end of the function AAS_ShowReachability

void AAS_ShowReachableAreas(int areanum)
{
	aas_areasettings_t *settings;
	static aas_reachability_t reach;
	static int index, lastareanum;
	static float lasttime;

	if (areanum != lastareanum)
	{
		index = 0;
		lastareanum = areanum;
	} //end if
	settings = &aasworld.areasettings[areanum];
	//
	if (!settings->numreachableareas) return;
	//
	if (index >= settings->numreachableareas) index = 0;
	//
	if (AAS_Time() - lasttime > 1.5)
	{
		Com_Memcpy(&reach, &aasworld.reachability[settings->firstreachablearea + index], sizeof(aas_reachability_t));
		index++;
		lasttime = AAS_Time();
		AAS_PrintTravelType(reach.traveltype & TRAVELTYPE_MASK);
		botimport.Print(PRT_MESSAGE, "\n");
	} //end if
	AAS_ShowReachability(&reach);
} //end of the function ShowReachableAreas

void AAS_FloodAreas_r(int areanum, int cluster, int *done)
{
	int nextareanum, i, facenum;
	aas_area_t *area;
	aas_face_t *face;
	aas_areasettings_t *settings;
	aas_reachability_t *reach;

	AAS_ShowAreaPolygons(areanum, 1, qtrue);
	//pointer to the convex area
	area = &aasworld.areas[areanum];
	settings = &aasworld.areasettings[areanum];
	//walk through the faces of the area
	for (i = 0; i < area->numfaces; i++)
	{
		facenum = abs(aasworld.faceindex[area->firstface + i]);
		face = &aasworld.faces[facenum];
		if (face->frontarea == areanum)
			nextareanum = face->backarea;
		else
			nextareanum = face->frontarea;
		if (!nextareanum)
			continue;
		if (done[nextareanum])
			continue;
		done[nextareanum] = qtrue;
		if (aasworld.areasettings[nextareanum].contents & AREACONTENTS_VIEWPORTAL)
			continue;
		if (AAS_AreaCluster(nextareanum) != cluster)
			continue;
		AAS_FloodAreas_r(nextareanum, cluster, done);
	} //end for
	//
	for (i = 0; i < settings->numreachableareas; i++)
	{
		reach = &aasworld.reachability[settings->firstreachablearea + i];
		nextareanum = reach->areanum;
		if (!nextareanum)
			continue;
		if (done[nextareanum])
			continue;
		done[nextareanum] = qtrue;
		if (aasworld.areasettings[nextareanum].contents & AREACONTENTS_VIEWPORTAL)
			continue;
		if (AAS_AreaCluster(nextareanum) != cluster)
			continue;
		/*
		if ((reach->traveltype & TRAVELTYPE_MASK) == TRAVEL_WALKOFFLEDGE)
		{
			AAS_DebugLine(reach->start, reach->end, 1);
		}
		*/
		AAS_FloodAreas_r(nextareanum, cluster, done);
	}
}

void AAS_FloodAreas(vec3_t origin)
{
	int areanum, cluster, *done;

	done = (int *) GetClearedMemory(aasworld.numareas * sizeof(int));
	areanum = AAS_PointAreaNum(origin);
	cluster = AAS_AreaCluster(areanum);
	AAS_FloodAreas_r(areanum, cluster, done);
}

#endif

void AAS_ProjectPointOntoVector( vec3_t point, vec3_t vStart, vec3_t vEnd, vec3_t vProj )
{
	vec3_t pVec, vec;

	VectorSubtract( point, vStart, pVec );
	VectorSubtract( vEnd, vStart, vec );
	VectorNormalize( vec );
	// project onto the directional vector for this segment
	VectorMA( vStart, DotProduct( pVec, vec ), vec, vProj );
}



// returns the Z velocity when rocket jumping at the origin
//
// Parameter:			-
// Returns:				-
// Changes Globals:		-
//===========================================================================
float AAS_WeaponJumpZVelocity(vec3_t origin, float radiusdamage)
{
	vec3_t kvel, v, start, end, forward, right, viewangles, dir;
	float	mass, knockback, points;
	vec3_t rocketoffset = {8, 8, -8};
	vec3_t botmins = {-16, -16, -24};
	vec3_t botmaxs = {16, 16, 32};
	//bsp_trace_t bsptrace;

	//look down (90 degrees)
	viewangles[PITCH] = 90;
	viewangles[YAW] = 0;
	viewangles[ROLL] = 0;
	//get the start point shooting from
	VectorCopy(origin, start);
	start[2] += 8; //view offset Z
	AngleVectors(viewangles, forward, right, NULL);
	start[0] += forward[0] * rocketoffset[0] + right[0] * rocketoffset[1];
	start[1] += forward[1] * rocketoffset[0] + right[1] * rocketoffset[1];
	start[2] += forward[2] * rocketoffset[0] + right[2] * rocketoffset[1] + rocketoffset[2];
	//end point of the trace
	VectorMA(start, 500, forward, end);
	//trace a line to get the impact point
	//bsptrace = AAS_Trace(start, NULL, NULL, end, 1, CONTENTS_SOLID);
	//calculate the damage the bot will get from the rocket impact
	VectorAdd(botmins, botmaxs, v);
	VectorMA(origin, 0.5, v, v);
	//VectorSubtract(bsptrace.endpos, v, v);
	//
	points = radiusdamage - 0.5 * VectorLength(v);
	if (points < 0) points = 0;
	//the owner of the rocket gets half the damage
	points *= 0.5;
	//mass of the bot (p_client.c: PutClientInServer)
	mass = 200;
	//knockback is the same as the damage points
	knockback = points;
	//direction of the damage (from trace.endpos to bot origin)
	//VectorSubtract(origin, bsptrace.endpos, dir);
	VectorNormalize(dir);
	//damage velocity
	VectorScale(dir, 1600.0 * (float)knockback / mass, kvel);	//the rocket jump hack...
	//rocket impact velocity + jump velocity
	return kvel[2]; // + aassettings.phys_jumpvel;
} //end of the function AAS_WeaponJumpZVelocity
//===========================================================================
//
// Parameter:			-
// Returns:				-
// Changes Globals:		-
//===========================================================================
float AAS_RocketJumpZVelocity(vec3_t origin)
{
	//rocket radius damage is 120 (p_weapon.c: Weapon_RocketLauncher_Fire)
	return AAS_WeaponJumpZVelocity(origin, 120);
} //end of the function AAS_RocketJumpZVelocity
//===========================================================================
//
// Parameter:			-
// Returns:				-
// Changes Globals:		-
//===========================================================================
float AAS_BFGJumpZVelocity(vec3_t origin)
{
	//bfg radius damage is 1000 (p_weapon.c: weapon_bfg_fire)
	return AAS_WeaponJumpZVelocity(origin, 120);
} //end of the function AAS_BFGJumpZVelocity
//===========================================================================
// applies ground friction to the given velocity
//
// Parameter:			-
// Returns:				-
// Changes Globals:		-
//===========================================================================
void AAS_Accelerate(vec3_t velocity, float frametime, vec3_t wishdir, float wishspeed, float accel)
{
	// q2 style
	int			i;
	float		addspeed, accelspeed, currentspeed;

	currentspeed = DotProduct(velocity, wishdir);
	addspeed = wishspeed - currentspeed;
	if (addspeed <= 0) {
		return;
	}
	accelspeed = accel*frametime*wishspeed;
	if (accelspeed > addspeed) {
		accelspeed = addspeed;
	}
	
	for (i=0 ; i<3 ; i++) {
		velocity[i] += accelspeed*wishdir[i];	
	}
} //end of the function AAS_Accelerate
//===========================================================================
// applies ground friction to the given velocity
//
// Parameter:			-
// Returns:				-
// Changes Globals:		-
//===========================================================================
void AAS_ApplyFriction(vec3_t vel, float friction, float stopspeed,
													float frametime)
{
	float speed, control, newspeed;

	//horizontal speed
	speed = sqrt(vel[0] * vel[0] + vel[1] * vel[1]);
	if (speed)
	{
		control = speed < stopspeed ? stopspeed : speed;
		newspeed = speed - frametime * control * friction;
		if (newspeed < 0) newspeed = 0;
		newspeed /= speed;
		vel[0] *= newspeed;
		vel[1] *= newspeed;
	} //end if
} //end of the function AAS_ApplyFriction
//===========================================================================
//
// Parameter:			-
// Returns:				-
// calculates the horizontal velocity needed to perform a jump from start
// to end
//
// Parameter:			zvel	: z velocity for jump
//						start	: start position of jump
//						end		: end position of jump
//						*speed	: returned speed for jump
// Returns:				qfalse if too high or too far from start to end
// Changes Globals:		-
//===========================================================================
int AAS_HorizontalVelocityForJump(float zvel, vec3_t start, vec3_t end, float *velocity)
{
	float phys_gravity, phys_maxvelocity;
	float maxjump, height2fall, t, top;
	vec3_t dir;

	phys_gravity = 123; // aassettings.phys_gravity;
	phys_maxvelocity = 123; // aassettings.phys_maxvelocity;

	//maximum height a player can jump with the given initial z velocity
	maxjump = 0.5 * phys_gravity * (zvel / phys_gravity) * (zvel / phys_gravity);
	//top of the parabolic jump
	top = start[2] + maxjump;
	//height the bot will fall from the top
	height2fall = top - end[2];
	//if the goal is to high to jump to
	if (height2fall < 0)
	{
		*velocity = phys_maxvelocity;
		return 0;
	} //end if
	//time a player takes to fall the height
	t = sqrt(height2fall / (0.5 * phys_gravity));
  	//direction from start to end
	VectorSubtract(end, start, dir);
	//
	if ( (t + zvel / phys_gravity) == 0.0f ) {
		*velocity = phys_maxvelocity;
		return 0;
	}
	//calculate horizontal speed
	*velocity = sqrt(dir[0]*dir[0] + dir[1]*dir[1]) / (t + zvel / phys_gravity);
	//the horizontal speed must be lower than the max speed
	if (*velocity > phys_maxvelocity)
	{
		*velocity = phys_maxvelocity;
		return 0;
	} //end if
	return 1;
} //end of the function AAS_HorizontalVelocityForJump




// damage = deltavelocity*deltavelocity  * 0.0001
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
int AAS_FallDamageDistance(void)
{
	float maxzvelocity, gravity, t;

	maxzvelocity = sqrt(30 * 10000);
	gravity = 123; // aassettings.phys_gravity;
	t = maxzvelocity / gravity;
	return 0.5 * gravity * t * t;
} //end of the function AAS_FallDamageDistance
//===========================================================================
// distance = 0.5 * gravity * t * t
// vel = t * gravity
// damage = vel * vel * 0.0001
//
// Parameter:			-
// Returns:				-
// Changes Globals:		-
//===========================================================================
float AAS_FallDelta(float distance)
{
	float t, delta, gravity;

	gravity = 123; // aassettings.phys_gravity;
	t = sqrt(fabs(distance) * 2 / gravity);
	delta = t * gravity;
	return delta * delta * 0.0001;
} //end of the function AAS_FallDelta
//===========================================================================
//
// Parameter:			-
// Returns:				-
// Changes Globals:		-
//===========================================================================
float AAS_MaxJumpHeight(float phys_jumpvel)
{
	float phys_gravity;

	phys_gravity = 123; // aassettings.phys_gravity;
	//maximum height a player can jump with the given initial z velocity
	return 0.5 * phys_gravity * (phys_jumpvel / phys_gravity) * (phys_jumpvel / phys_gravity);
} //end of the function MaxJumpHeight
//===========================================================================
// returns true if a player can only crouch in the area
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
float AAS_MaxJumpDistance(float phys_jumpvel)
{
	float phys_gravity, phys_maxvelocity, t;

	phys_gravity = 123; // aassettings.phys_gravity;
	phys_maxvelocity = 123; // aassettings.phys_maxvelocity;
	//time a player takes to fall the height
	//t = sqrt(aassettings.rs_maxjumpfallheight / (0.5 * phys_gravity));
   //maximum distance
	return phys_maxvelocity * (t + phys_jumpvel / phys_gravity);
} //end of the function AAS_MaxJumpDistance

// Changes Globals:		-
//===========================================================================
float VectorDistance(vec3_t v1, vec3_t v2)
{
	vec3_t dir;

	VectorSubtract(v2, v1, dir);
	return VectorLength(dir);
} //end of the function VectorDistance
//===========================================================================
// returns true if the first vector is between the last two vectors
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
int VectorBetweenVectors(vec3_t v, vec3_t v1, vec3_t v2)
{
	vec3_t dir1, dir2;

	VectorSubtract(v, v1, dir1);
	VectorSubtract(v, v2, dir2);
	return (DotProduct(dir1, dir2) <= 0);
} //end of the function VectorBetweenVectors
//===========================================================================
// returns the mid point between the two vectors
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
void VectorMiddle(vec3_t v1, vec3_t v2, vec3_t middle)
{
	VectorAdd(v1, v2, middle);
	VectorScale(middle, 0.5, middle);
} //end of the function VectorMiddle
//===========================================================================
// calculate a range of points closest to each other on both edges
//
// Parameter:			beststart1		start of the range of points on edge v1-v2
//						beststart2		end of the range of points  on edge v1-v2
//						bestend1		start of the range of points on edge v3-v4
//						bestend2		end of the range of points  on edge v3-v4
//						bestdist		best distance so far
// Returns:				-
// Changes Globals:		-
//===========================================================================
/*
float AAS_ClosestEdgePoints(vec3_t v1, vec3_t v2, vec3_t v3, vec3_t v4,
							aas_plane_t *plane1, aas_plane_t *plane2,
							vec3_t beststart, vec3_t bestend, float bestdist)
{
	vec3_t dir1, dir2, p1, p2, p3, p4;
	float a1, a2, b1, b2, dist;
	int founddist;

	//edge vectors
	VectorSubtract(v2, v1, dir1);
	VectorSubtract(v4, v3, dir2);
	//get the horizontal directions
	dir1[2] = 0;
	dir2[2] = 0;
	//
	// p1 = point on an edge vector of area2 closest to v1
	// p2 = point on an edge vector of area2 closest to v2
	// p3 = point on an edge vector of area1 closest to v3
	// p4 = point on an edge vector of area1 closest to v4
	//
	if (dir2[0])
	{
		a2 = dir2[1] / dir2[0];
		b2 = v3[1] - a2 * v3[0];
		//point on the edge vector of area2 closest to v1
		p1[0] = (DotProduct(v1, dir2) - (a2 * dir2[0] + b2 * dir2[1])) / dir2[0];
		p1[1] = a2 * p1[0] + b2;
		//point on the edge vector of area2 closest to v2
		p2[0] = (DotProduct(v2, dir2) - (a2 * dir2[0] + b2 * dir2[1])) / dir2[0];
		p2[1] = a2 * p2[0] + b2;
	} //end if
	else
	{
		//point on the edge vector of area2 closest to v1
		p1[0] = v3[0];
		p1[1] = v1[1];
		//point on the edge vector of area2 closest to v2
		p2[0] = v3[0];
		p2[1] = v2[1];
	} //end else
	//
	if (dir1[0])
	{
		//
		a1 = dir1[1] / dir1[0];
		b1 = v1[1] - a1 * v1[0];
		//point on the edge vector of area1 closest to v3
		p3[0] = (DotProduct(v3, dir1) - (a1 * dir1[0] + b1 * dir1[1])) / dir1[0];
		p3[1] = a1 * p3[0] + b1;
		//point on the edge vector of area1 closest to v4
		p4[0] = (DotProduct(v4, dir1) - (a1 * dir1[0] + b1 * dir1[1])) / dir1[0];
		p4[1] = a1 * p4[0] + b1;
	} //end if
	else
	{
		//point on the edge vector of area1 closest to v3
		p3[0] = v1[0];
		p3[1] = v3[1];
		//point on the edge vector of area1 closest to v4
		p4[0] = v1[0];
		p4[1] = v4[1];
	} //end else
	//start with zero z-coordinates
	p1[2] = 0;
	p2[2] = 0;
	p3[2] = 0;
	p4[2] = 0;
	//calculate the z-coordinates from the ground planes
	p1[2] = (plane2->dist - DotProduct(plane2->normal, p1)) / plane2->normal[2];
	p2[2] = (plane2->dist - DotProduct(plane2->normal, p2)) / plane2->normal[2];
	p3[2] = (plane1->dist - DotProduct(plane1->normal, p3)) / plane1->normal[2];
	p4[2] = (plane1->dist - DotProduct(plane1->normal, p4)) / plane1->normal[2];
	//
	founddist = qfalse;
	//
	if (VectorBetweenVectors(p1, v3, v4))
	{
		dist = VectorDistance(v1, p1);
		if (dist > bestdist - 0.5 && dist < bestdist + 0.5)
		{
			VectorMiddle(beststart, v1, beststart);
			VectorMiddle(bestend, p1, bestend);
		} //end if
		else if (dist < bestdist)
		{
			bestdist = dist;
			VectorCopy(v1, beststart);
			VectorCopy(p1, bestend);
		} //end if
		founddist = qtrue;
	} //end if
	if (VectorBetweenVectors(p2, v3, v4))
	{
		dist = VectorDistance(v2, p2);
		if (dist > bestdist - 0.5 && dist < bestdist + 0.5)
		{
			VectorMiddle(beststart, v2, beststart);
			VectorMiddle(bestend, p2, bestend);
		} //end if
		else if (dist < bestdist)
		{
			bestdist = dist;
			VectorCopy(v2, beststart);
			VectorCopy(p2, bestend);
		} //end if
		founddist = qtrue;
	} //end else if
	if (VectorBetweenVectors(p3, v1, v2))
	{
		dist = VectorDistance(v3, p3);
		if (dist > bestdist - 0.5 && dist < bestdist + 0.5)
		{
			VectorMiddle(beststart, p3, beststart);
			VectorMiddle(bestend, v3, bestend);
		} //end if
		else if (dist < bestdist)
		{
			bestdist = dist;
			VectorCopy(p3, beststart);
			VectorCopy(v3, bestend);
		} //end if
		founddist = qtrue;
	} //end else if
	if (VectorBetweenVectors(p4, v1, v2))
	{
		dist = VectorDistance(v4, p4);
		if (dist > bestdist - 0.5 && dist < bestdist + 0.5)
		{
			VectorMiddle(beststart, p4, beststart);
			VectorMiddle(bestend, v4, bestend);
		} //end if
		else if (dist < bestdist)
		{
			bestdist = dist;
			VectorCopy(p4, beststart);
			VectorCopy(v4, bestend);
		} //end if
		founddist = qtrue;
	} //end else if
	//if no shortest distance was found the shortest distance
	//is between one of the vertexes of edge1 and one of edge2
	if (!founddist)
	{
		dist = VectorDistance(v1, v3);
		if (dist < bestdist)
		{
			bestdist = dist;
			VectorCopy(v1, beststart);
			VectorCopy(v3, bestend);
		} //end if
		dist = VectorDistance(v1, v4);
		if (dist < bestdist)
		{
			bestdist = dist;
			VectorCopy(v1, beststart);
			VectorCopy(v4, bestend);
		} //end if
		dist = VectorDistance(v2, v3);
		if (dist < bestdist)
		{
			bestdist = dist;
			VectorCopy(v2, beststart);
			VectorCopy(v3, bestend);
		} //end if
		dist = VectorDistance(v2, v4);
		if (dist < bestdist)
		{
			bestdist = dist;
			VectorCopy(v2, beststart);
			VectorCopy(v4, bestend);
		} //end if
	} //end if
	return bestdist;
} //end of the function AAS_ClosestEdgePoints*/







float AngleDiff(float ang1, float ang2)
{
	float diff;

	diff = ang1 - ang2;
	if (ang1 > ang2)
	{
		if (diff > 180.0) diff -= 360.0;
	} //end if
	else
	{
		if (diff < -180.0) diff += 360.0;
	} //end else
	return diff;
}

float DistanceFromLineSquared(vec3_t p, vec3_t lp1, vec3_t lp2)
{
	vec3_t proj, dir;
	int j;

	AAS_ProjectPointOntoVector(p, lp1, lp2, proj);
	for (j = 0; j < 3; j++)
		if ((proj[j] > lp1[j] && proj[j] > lp2[j]) ||
			(proj[j] < lp1[j] && proj[j] < lp2[j]))
			break;
	if (j < 3) {
		if (fabs(proj[j] - lp1[j]) < fabs(proj[j] - lp2[j]))
			VectorSubtract(p, lp1, dir);
		else
			VectorSubtract(p, lp2, dir);
		return VectorLengthSquared(dir);
	}
	VectorSubtract(p, proj, dir);
	return VectorLengthSquared(dir);
}
float VectorDistanceSquared(vec3_t p1, vec3_t p2)
{
	vec3_t dir;
	VectorSubtract(p2, p1, dir);
	return VectorLengthSquared(dir);
} //end of the function VectorDistanceSquared

int Intersection(vec2_t p1, vec2_t p2, vec2_t p3, vec2_t p4, vec2_t out)
{
   float x1, dx1, dy1, x2, dx2, dy2, d;

   dx1 = p2[0] - p1[0];
   dy1 = p2[1] - p1[1];
   dx2 = p4[0] - p3[0];
   dy2 = p4[1] - p3[1];

   d = dy1 * dx2 - dx1 * dy2;
   if (d != 0)
   {
      x1 = p1[1] * dx1 - p1[0] * dy1;
      x2 = p3[1] * dx2 - p3[0] * dy2;
      out[0] = (int) ((dx1 * x2 - dx2 * x1) / d);
      out[1] = (int) ((dy1 * x2 - dy2 * x1) / d);
		return qtrue;
   } //end if
   else
   {
      return qfalse;
   } //end else
}



#define BBOX_NORMAL_EPSILON		0.001
#define ON_EPSILON					0 //0.0005
#define TRACEPLANE_EPSILON			0.125



//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================

// LETS KEEP THIIS
void AAS_PresenceTypeBoundingBox(int presencetype, vec3_t mins, vec3_t maxs)
{
	int index;
	//bounding box size for each presence type
	vec3_t boxmins[3] = {{0, 0, 0}, {-15, -15, -24}, {-15, -15, -24}};
	vec3_t boxmaxs[3] = {{0, 0, 0}, { 15,  15,  32}, { 15,  15,   8}};

	//if (presencetype == PRESENCE_NORMAL) index = 1;
	//else if (presencetype == PRESENCE_CROUCH) index = 2;
	//else
	//{
	//	//botimport.Print(PRT_FATAL, "AAS_PresenceTypeBoundingBox: unknown presence type\n");
	//	index = 2;
	//} //end if
	VectorCopy(boxmins[index], mins);
	VectorCopy(boxmaxs[index], maxs);
} //end of the function AAS_PresenceTypeBoundingBox
//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================

//===========================================================================
// calculates the minimum distance between the origin of the box and the
// given plane when both will collide on the given side of the plane
//
// normal	=	normal vector of plane to calculate distance from
// mins		=	minimums of box relative to origin
// maxs		=	maximums of box relative to origin
// side		=	side of the plane we want to calculate the distance from
//					0 normal vector side
//					1 not normal vector side
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
vec_t AAS_BoxOriginDistanceFromPlane(vec3_t normal, vec3_t mins, vec3_t maxs, int side)
{
	vec3_t v1, v2;
	int i;

	//swap maxs and mins when on the other side of the plane
	if (side)
	{
		//get a point of the box that would be one of the first
		//to collide with the plane
		for (i = 0; i < 3; i++)
		{
			if (normal[i] > BBOX_NORMAL_EPSILON) v1[i] = maxs[i];
			else if (normal[i] < -BBOX_NORMAL_EPSILON) v1[i] = mins[i];
			else v1[i] = 0;
		} //end for
	} //end if
	else
	{
		//get a point of the box that would be one of the first
		//to collide with the plane
		for (i = 0; i < 3; i++)
		{
			if (normal[i] > BBOX_NORMAL_EPSILON) v1[i] = mins[i];
			else if (normal[i] < -BBOX_NORMAL_EPSILON) v1[i] = maxs[i];
			else v1[i] = 0;
		} //end for
	} //end else
	//
	VectorCopy(normal, v2);
	VectorInverse(v2);
//	VectorNegate(normal, v2);
	return DotProduct(v1, v2);
} //end of the function AAS_BoxOriginDistanceFromPlane

//===========================================================================
// a simple cross product
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
// void AAS_OrthogonalToVectors(vec3_t v1, vec3_t v2, vec3_t res)
#define AAS_OrthogonalToVectors(v1, v2, res) \
	(res)[0] = ((v1)[1] * (v2)[2]) - ((v1)[2] * (v2)[1]);\
	(res)[1] = ((v1)[2] * (v2)[0]) - ((v1)[0] * (v2)[2]);\
	(res)[2] = ((v1)[0] * (v2)[1]) - ((v1)[1] * (v2)[0]);

//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
//int AAS_BoxOnPlaneSide(vec3_t absmins, vec3_t absmaxs, aas_plane_t *p)
#define AAS_BoxOnPlaneSide(absmins, absmaxs, p) (\
	( (p)->type < 3) ?\
	(\
		( (p)->dist <= (absmins)[(p)->type]) ?\
		(\
			1\
		)\
		:\
		(\
			( (p)->dist >= (absmaxs)[(p)->type]) ?\
			(\
				2\
			)\
			:\
			(\
				3\
			)\
		)\
	)\
	:\
	(\
		AAS_BoxOnPlaneSide2((absmins), (absmaxs), (p))\
	)\
) 



float DistancePointToLine(vec3_t v1, vec3_t v2, vec3_t point)
{
	vec3_t vec, p2;

	AAS_ProjectPointOntoVector(point, v1, v2, p2);
	VectorSubtract(point, p2, vec);
	return VectorLength(vec);
}




void PC_ConvertPath(char *path)
{
	char *ptr;

	//remove double path seperators
	for (ptr = path; *ptr;)
	{
		if ((*ptr == '\\' || *ptr == '/') &&
				(*(ptr+1) == '\\' || *(ptr+1) == '/'))
		{
			memmove(ptr, ptr+1, strlen(ptr));
		} //end if
		else
		{
			ptr++;
		} //end else
	} //end while
	//set OS dependent path seperators
	for (ptr = path; *ptr;)
	{
		//if (*ptr == '/' || *ptr == '\\') *ptr = PATHSEPERATOR_CHAR;
		ptr++;
	} //end while
}


void StripDoubleQuotes(char *string)
{
	if (*string == '\"')
	{
		memmove(string, string+1, strlen(string));
	} //end if
	if (string[strlen(string)-1] == '\"')
	{
		string[strlen(string)-1] = '\0';
	} //end if
}
void StripSingleQuotes(char *string)
{
	if (*string == '\'')
	{
		memmove(string, string+1, strlen(string));
	} //end if
	if (string[strlen(string)-1] == '\'')
	{
		string[strlen(string)-1] = '\0';
	} //end if
}