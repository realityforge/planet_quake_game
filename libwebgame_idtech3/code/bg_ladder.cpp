#include "bg_pmove.h"
#include "bg_ladder.h"
#include "bg_slidemove.h"
#include <cg_vars.h>

qboolean ladderforward;
vec3_t   laddervec;

#define TRACE_LADDER_DIST   48.0


/**
 * @brief Checks to see if we are on a ladder
 */

EXTERNC int c_pmove;

void PM_CheckLadderMove(void)
{
	vec3_t   spot;
	vec3_t   flatforward;
	trace_t  trace;
	float    tracedist;
	qboolean wasOnLadder;

	
	if (pm->ps->pm_time)
	{
		return;
	}

	if (pml.walking)
	{
		tracedist = 1.0;
	}
	else
	{
		tracedist = TRACE_LADDER_DIST;
	}

	wasOnLadder = ((pm->ps->pm_flags & PMF_LADDER) != 0);

	pml.ladder        = qfalse;
	pm->ps->pm_flags &= ~PMF_LADDER;    // clear ladder bit
	ladderforward     = qfalse;

	if (pm->ps->stats[STAT_HEALTH] <= 0)
	{
		pm->ps->groundEntityNum = ENTITYNUM_NONE;
		pml.groundPlane         = qfalse;
		pml.walking             = qfalse;
		return;
	}

	//FIX//// Can't climb ladders while prone
	//FIX//if (pm->ps->eFlags & EF_PRONE)
	//FIX//{
	//FIX//	return;
	//FIX//}

	// check for ladder
	flatforward[0] = pml.forward[0];
	flatforward[1] = pml.forward[1];
	flatforward[2] = 0;
	VectorNormalize(flatforward);

	int debugimgui = pm->imgui && imgui_ladder->integer;

	#ifdef ENABLE_IMGUI
	if (debugimgui) {
		ImGui::Begin("ladder");
		ImGui::Text("chopcount=%d c_pmove=%d flatforward = (%.2f, %.2f, %.2f)", pm->chopcount, c_pmove, flatforward[0], flatforward[1], flatforward[2]);
		
	}
	#endif
	
	// out = vec + b * scalar
	// VectorMA(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))

	// spot = origin + forward * tracedist
	VectorMA(pm->ps->origin, tracedist, flatforward, spot);



	pm->trace(&trace, pm->ps->origin, pm->mins, pm->maxs, spot, pm->ps->clientNum, pm->tracemask);
	if ((trace.fraction < 1) && (trace.surfaceFlags & SURF_LADDER))
	{
		pml.ladder = qtrue;
	}

	if (pml.ladder)
	{
		VectorCopy(trace.plane.normal, laddervec);
		#ifdef ENABLE_IMGUI
		if (debugimgui)
			ImGui::Text("laddervec = (%.2f, %.2f, %.2f)", laddervec[0], laddervec[1], laddervec[2]);
		#endif
	}

	if (pml.ladder && !pml.walking && (trace.fraction * tracedist > 1.0f))
	{
		vec3_t mins;
		// if we are only just on the ladder, don't do this yet, or it may throw us back off the ladder
		pml.ladder = qfalse;
		VectorCopy(pm->mins, mins);

		#ifdef ENABLE_IMGUI
		if (debugimgui)
			ImGui::Text("mins = (%.2f, %.2f, %.2f)", mins[0], mins[1], mins[2]);
		#endif

		mins[2] = -1;
		VectorMA(pm->ps->origin, -tracedist, laddervec, spot);
		pm->trace(&trace, pm->ps->origin, mins, pm->maxs, spot, pm->ps->clientNum, pm->tracemask);
		if ((trace.fraction < 1) && (trace.surfaceFlags & SURF_LADDER))
		{
			ladderforward     = qtrue;
			pml.ladder        = qtrue;
			pm->ps->pm_flags |= PMF_LADDER; // set ladder bit
		}
		else
		{
			pml.ladder = qfalse;
		}
	}
	else if (pml.ladder)
	{
		pm->ps->pm_flags |= PMF_LADDER; // set ladder bit
	}

	// create some up/down velocity if touching ladder
	if (pml.ladder)
	{
		if (pml.walking)
		{
			// we are currently on the ground, only go up and prevent X/Y if we are pushing forwards
			if (pm->cmd.forwardmove <= 0)
			{
				pml.ladder = qfalse;
			}
		}
	}

	#ifdef ENABLE_IMGUI
	if (debugimgui)
		ImGui::End();
	#endif

	//FIX//// if we have just dismounted the ladder at the top, play dismount
	//FIX//if (!pml.ladder && wasOnLadder && pm->ps->velocity[2] > 0)
	//FIX//{
	//FIX//	BG_AnimScriptEvent(pm->ps, pm->character->animModelInfo, ANIM_ET_CLIMB_DISMOUNT, qfalse, qfalse);
	//FIX//}
	//FIX//// if we have just mounted the ladder
	//FIX//if (pml.ladder && !wasOnLadder && pm->ps->velocity[2] < 0)        // only play anim if going down ladder
	//FIX//{
	//FIX//	BG_AnimScriptEvent(pm->ps, pm->character->animModelInfo, ANIM_ET_CLIMB_MOUNT, qfalse, qfalse);
	//FIX//}
}

void PM_LadderMove(void)
{
	float  wishspeed, scale;
	vec3_t wishdir, wishvel;
	float  upscale;

	if (ladderforward)
	{
		// move towards the ladder
		VectorScale(laddervec, -200.0, wishvel);
		pm->ps->velocity[0] = wishvel[0];
		pm->ps->velocity[1] = wishvel[1];
	}

	upscale = (pml.forward[2] + 0.5f) * 2.5f;
	if (upscale > 1.0f)
	{
		upscale = 1.0f;
	}
	else if (upscale < -1.0f)
	{
		upscale = -1.0f;
	}

	// forward/right should be horizontal only
	pml.forward[2] = 0;
	pml.right[2]   = 0;
	VectorNormalize(pml.forward);
	VectorNormalize(pml.right);

	// move depending on the view, if view is straight forward, then go up
	// if view is down more then X degrees, start going down
	// if they are back pedalling, then go in reverse of above
	scale = PM_CmdScale(&pm->cmd);
	VectorClear(wishvel);

	if (pm->cmd.forwardmove)
	{
		wishvel[2] = 0.9f * upscale * scale * (float)pm->cmd.forwardmove;
	}
	//Com_Printf("wishvel[2] = %i, fwdmove = %i\n", (int)wishvel[2], (int)pm->cmd.forwardmove );

	if (pm->cmd.rightmove)
	{
		// strafe, so we can jump off ladder
		vec3_t ladder_right, ang;
		vectoangles(laddervec, ang);
		AngleVectors(ang, NULL, ladder_right, NULL);

		// if we are looking away from the ladder, reverse the right vector
		if (DotProduct(laddervec, pml.forward) < 0)
		{
			VectorInverse(ladder_right);
		}

		//VectorMA( wishvel, 0.5 * scale * (float)pm->cmd.rightmove, pml.right, wishvel );
		VectorMA(wishvel, 0.5f * scale * (float)pm->cmd.rightmove, ladder_right, wishvel);
	}

	// do strafe friction
	PM_Friction();

	if (pm->ps->velocity[0] < 1 && pm->ps->velocity[0] > -1)
	{
		pm->ps->velocity[0] = 0;
	}
	if (pm->ps->velocity[1] < 1 && pm->ps->velocity[1] > -1)
	{
		pm->ps->velocity[1] = 0;
	}

	wishspeed = VectorNormalize2(wishvel, wishdir);

	PM_Accelerate(wishdir, wishspeed, pm_accelerate);
	if (wishvel[2] == 0.f)
	{
		if (pm->ps->velocity[2] > 0)
		{
			pm->ps->velocity[2] -= pm->ps->gravity * pml.frametime;
			if (pm->ps->velocity[2] < 0)
			{
				pm->ps->velocity[2] = 0;
			}
		}
		else
		{
			pm->ps->velocity[2] += pm->ps->gravity * pml.frametime;
			if (pm->ps->velocity[2] > 0)
			{
				pm->ps->velocity[2] = 0;
			}
		}
	}

	//Com_Printf("vel[2] = %i\n", (int)pm->ps->velocity[2] );

	PM_StepSlideMove(qfalse);    // no gravity while going up ladder

	// always point legs forward
	pm->ps->movementDir = 0;
}