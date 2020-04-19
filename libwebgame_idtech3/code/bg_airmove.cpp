#include "bg_pmove.h"
#include "bg_airmove.h"
#include "bg_slidemove.h"
#include "client.h"
#include "cg_vars.h"

void PM_Debug() {
		
	if (pm->imgui && imgui_airmove->integer) {

		//ImGui::Begin("AirMove");
		//ImGui::DragVector3(pm->ps->velocity);
		//
		//ImGui::Text("pml.forward");
		//ImGui::SameLine();
		//ImGui::DragVector3(pml.forward);
		//
		//
		//ImGui::Text("forwardmove=%d rightmove=%d upmove=%d", pm->cmd.forwardmove, pm->cmd.rightmove, pm->cmd.upmove);
		//
		//
		//
		//ImGui::End();

	}
}

cvar_t *g_air_acceleration_div = NULL;
cvar_t *g_air_acceleration_mul = NULL;
cvar_t *g_air_acceleration_max = NULL;



void PM_ClipVelocityAir( vec3_t playerVelocityIn, vec3_t planeNormal, vec3_t playerVelocityOut, float overbounce/*, float playerVelocityNormalized_dot_planeNormal */) {
	float	backoff;
	int		i;
	
	backoff = DotProduct (playerVelocityIn, planeNormal);
	
	if ( backoff < 0 ) {
		backoff *= overbounce;
	} else {
		backoff /= overbounce;
	}

	float change[3];
	
	change[0] = planeNormal[0] * backoff;
	change[1] = planeNormal[1] * backoff;
	change[2] = planeNormal[2] * backoff;

	//float changeLength = VectorLength(change);
	//if (changeLength != 0.0f)
	//{
	//	DebugBreak();
	//}

	//float playerVelocityNormal[3];
	
	
	//float speedBefore = VectorLength(playerVelocityIn);
	for ( i	=0 ; i<3 ; i++ ) {
		playerVelocityOut[i] = playerVelocityIn[i] - change[i];
		//playerVelocityOut[i] += playerVelocityIn[i] * changeLength; // keep velocity, just in other direction
	}
	
	
	//float speedAfter = VectorLength(playerVelocityIn);
	//float needSpeed = speedBefore - speedAfter;
	//
	//VectorNormalize2(playerVelocityIn, playerVelocityNormal);
	//
	//playerVelocityIn[0] += playerVelocityNormal[0] * needSpeed; // * playerVelocityNormalized_dot_planeNormal;
	//playerVelocityIn[1] += playerVelocityNormal[1] * needSpeed; // * playerVelocityNormalized_dot_planeNormal;
	//playerVelocityIn[2] += playerVelocityNormal[2] * needSpeed; // * playerVelocityNormalized_dot_planeNormal;

	//playerVelocityIn[2] += changeLength * 2; // * backoff;

	//return changeLength;
}


#define AIRCONTROL

#ifdef AIRCONTROL
// kungfooman: ripped from https://github.com/ETrun/ETrun/blob/master/src/game/bg_pmove.c
// Nico, from Racesow
// when using +strafe convert the inertia to forward speed.

// Nico, from Racesow
const float pm_aircontrol        = 150.0f; // aircontrol multiplier (intertia velocity to forward velocity conversion), default: 150
const float pm_strafeaccelerate  = 100; // forward acceleration when strafe bunny hopping, default: 70
const float pm_wishspeed         = 100; // Nico, default value 30
const float pm_airstopaccelerate = 2.0f; // Nico, CPM value: 2.5f, Racesow value: 2.0f
const float pm_slickaccelerate   = 10.0f; // Nico, slick control accelerate
const float pm_accelerate_AP     = 15; // Nico, only used for AP

static void PM_Aircontrol(pmove_t *pm, vec3_t wishdir, float wishspeed) {
	float zspeed, speed, dot, k;
	int   i;
	float smove;

	//if (!pm_aircontrol) {
	//	return;
	//}

	// accelerate
	smove = pm->cmd.rightmove;

	if ((smove > 0 || smove < 0) || (wishspeed == 0.0)) {
		return; // can't control movement if not moving forward or backward
	}

	zspeed              = pm->ps->velocity[2];
	pm->ps->velocity[2] = 0;
	speed               = VectorNormalize(pm->ps->velocity);

	dot = DotProduct(pm->ps->velocity, wishdir);
	k   = 32.0f * pm_aircontrol * dot * dot * pml.frametime;

	if (dot > 0) {
		// we can't change direction while slowing down
		for (i = 0; i < 2; ++i) {
			pm->ps->velocity[i] = pm->ps->velocity[i] * speed + wishdir[i] * k;
		}
		VectorNormalize(pm->ps->velocity);
	}

	for (i = 0; i < 2; ++i) {
		pm->ps->velocity[i] *= speed;
	}
	pm->ps->velocity[2] = zspeed;
}
#endif


void PM_AirMove( void ) {
	int			i;
	vec3_t		wishvel;
	float		fmove, smove;
	vec3_t		wishdir;
	float		wishspeed;
	float		scale;
	usercmd_t	cmd;

	PM_Friction();

	fmove = pm->cmd.forwardmove;
	smove = pm->cmd.rightmove;

	cmd = pm->cmd;
	scale = PM_CmdScale( &cmd );

	// set the movementDir so clients can rotate the legs for strafing
	PM_SetMovementDir();

	// project moves down to flat plane
	pml.forward[2] = 0;
	pml.right[2] = 0;
	VectorNormalize (pml.forward);
	VectorNormalize (pml.right);

	for ( i = 0 ; i < 2 ; i++ ) {
		wishvel[i] = pml.forward[i]*fmove + pml.right[i]*smove;
	}
	wishvel[2] = 0;

	VectorCopy (wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);
	wishspeed *= scale;

#ifdef AIRCONTROL
		float wishspeed2 = wishspeed, accel;

		if (DotProduct(pm->ps->velocity, wishdir) < 0) {
			accel = pm_airstopaccelerate;
		} else {
			accel = pm_airaccelerate;
		}

		if ((smove > 0 || smove < 0) && !fmove) {
			if (wishspeed > pm_wishspeed) {
				wishspeed = pm_wishspeed;
			}
			accel = pm_strafeaccelerate;
		}

		// Air control
		PM_Accelerate(wishdir, wishspeed, accel);
		PM_Aircontrol(pm, wishdir, wishspeed2);
#else
	// not on ground, so little effect on velocity
	PM_Accelerate (wishdir, wishspeed, pm_airaccelerate);
#endif

	// we may have a ground plane that is very steep, even
	// though we don't have a groundentity
	// slide along the steep plane
	if ( pml.groundPlane ) {
		PM_ClipVelocity (pm->ps->velocity, pml.groundTrace.plane.normal, 
			pm->ps->velocity, OVERCLIP );
	}

#if 0
	//ZOID:  If we are on the grapple, try stair-stepping
	//this allows a player to use the grapple to pull himself
	//over a ledge
	if (pm->ps->pm_flags & PMF_GRAPPLE_PULL)
		PM_StepSlideMove ( qtrue );
	else
		PM_SlideMove ( qtrue );
#endif

	PM_StepSlideMove ( qtrue );

	//if (pm->cmd.rightmove) {
	if (pm->cmd.buttons & BUTTON_WALKING) {
		vec3_t velocityNormal;
		VectorNormalize2(pm->ps->velocity, velocityNormal);
		vec3_t angles = {0,0,0};
		vec3_t forward2d;
		angles[1] = pm->ps->viewangles[1];
		AngleVectors(angles, forward2d, NULL, NULL);
 		float *vel = pm->ps->velocity;
		float oldZ = pm->ps->velocity[2];
		pm->ps->velocity[2] = 0.0f;
		float xyVelocityLength = sqrtf(vel[0] * vel[0] + vel[1] * vel[1]);
		pm->ps->velocity[0] = forward2d[0] * xyVelocityLength;
		pm->ps->velocity[1] = forward2d[1] * xyVelocityLength;
		pm->ps->velocity[2] = oldZ;
	}
}

void PM_AirMoveSurf() {
	int			i;
	vec3_t		wishvel;
	float		fmove, smove;
	vec3_t		wishdir;
	float		wishspeed;
	float		scale;
	usercmd_t	cmd;

	PM_Friction();

	if (g_air_acceleration_div == NULL) {
		g_air_acceleration_div = Cvar_Get( "g_air_acceleration_div", "5000", 0);
		g_air_acceleration_mul = Cvar_Get( "g_air_acceleration_mul", "2", 0);
		g_air_acceleration_max = Cvar_Get( "g_air_acceleration_max", "500", 0);
	}


	fmove = pm->cmd.forwardmove;
	smove = pm->cmd.rightmove;

	cmd = pm->cmd;
	scale = PM_CmdScale( &cmd );

	// set the movementDir so clients can rotate the legs for strafing
	PM_SetMovementDir();

	// project moves down to flat plane
	//pml.forward[2] = 0;
	//pml.right[2] = 0;
	//VectorNormalize (pml.forward);
	//VectorNormalize (pml.right);

	for ( i = 0 ; i < 2 ; i++ ) {
		wishvel[i] = pml.forward[i]*fmove + pml.right[i]*smove;
	}
	wishvel[2] = 0;
	

	


	VectorCopy (wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);
	wishspeed *= scale;

	// not on ground, so little effect on velocity
	PM_Accelerate (wishdir, wishspeed, pm_airaccelerate);

	// we may have a ground plane that is very steep, even
	// though we don't have a groundentity
	// slide along the steep plane
	if ( pml.groundPlane ) {
		
		float playerVelocityNormalized[3];
		VectorNormalize2(pm->ps->velocity, playerVelocityNormalized);
		float playerVelocityNormalized_dot_planeNormal = DotProduct(playerVelocityNormalized, pml.groundTrace.plane.normal);

		
#ifdef ENABLE_IMGUI
		if (pm->imgui && imgui_airmove->integer){


			float *in = pm->ps->velocity;
			float *normal = pml.groundTrace.plane.normal;
			float backoff = DotProduct (in, normal);
			

			float change[3];
			change[0] = normal[0] * backoff;
			change[1] = normal[1] * backoff;
			change[2] = normal[2] * backoff;

			ImGui::Begin("AirMove");

		static float backoffPlayerVelocity[3];
			static float biggestBackoff;
			static float smallestBackoff;
			static float biggestDot = 0.0f, smallestDot = 0.0f;
			if (playerVelocityNormalized_dot_planeNormal > biggestDot)
				biggestDot = playerVelocityNormalized_dot_planeNormal;
			if (playerVelocityNormalized_dot_planeNormal < smallestDot)
				smallestDot = playerVelocityNormalized_dot_planeNormal;
			if (backoff > biggestBackoff) {
				biggestBackoff = backoff;
				VectorCopy(pm->ps->velocity, backoffPlayerVelocity);
			}
			if (backoff < smallestBackoff) {
				smallestBackoff = backoff;
			}

			if (pm->cmd.buttons & BUTTON_ATTACK) {
				biggestBackoff = 0.0f;
				smallestBackoff = 0.0f;
				biggestDot = 0.0f;
				smallestDot = 0.0f;
			}
			

			
			ImGui::Text("playerVelocityNormalized_dot_planeNormal: %f", playerVelocityNormalized_dot_planeNormal);
			ImGui::Text("biggestDot: %f smallestDot: %f", biggestDot, smallestDot);
			ImGui::Text("backoff: %f", backoff);
			ImGui::Text("biggestBackoff: %f smallestBackoff: %f", biggestBackoff, smallestBackoff);

			//ImGui::Text("changeLength: %f", changeLength);
			
			ImGui::Text("pml.groundTrace.plane.normal");
			ImGui::SameLine();
			ImGui::DragVector3(pml.groundTrace.plane.normal);


			ImGui::Text("backoffPlayerVelocity");
			ImGui::SameLine();
			ImGui::DragVector3(backoffPlayerVelocity);

			ImGui::Text("change");
			ImGui::SameLine();
			ImGui::DragVector3(change);

			
			float changeSecond[3];
			float invert = 1.0f / pml.frametime;
			changeSecond[0] = change[0] * invert;
			changeSecond[1] = change[1] * invert;
			changeSecond[2] = change[2] * invert;


			ImGui::Text("changeSecond");
			ImGui::SameLine();
			ImGui::DragVector3(changeSecond);
			
			float velocityLength = VectorLength(pm->ps->velocity);
			//float changeLength = VectorLength(change);

			
			ImGui::Text("velocityLength = %f", velocityLength);
			//ImGui::Text("changeLength = %f", changeLength);

			ImGui::End();
		}
#endif

		PM_ClipVelocityAir(pm->ps->velocity, pml.groundTrace.plane.normal, pm->ps->velocity, OVERCLIP );

		//float overbounce = OVERCLIP;
		//float planeNormal[3];
		//VectorCopy(pml.groundTrace.plane.normal, planeNormal);
		//auto &playerVelocityIn = pm->ps->velocity;
		//auto &playerVelocityOut = pm->ps->velocity;
		////float PM_ClipVelocityAir( vec3_t playerVelocityIn, vec3_t planeNormal, vec3_t playerVelocityOut, float overbounce, float playerVelocityNormalized_dot_planeNormal ) {
		//float	backoff;
		//int		i;
		//backoff = DotProduct (pm->ps->velocity, pml.groundTrace.plane.normal);
		//if ( backoff < 0 ) {
		//	backoff *= overbounce;
		//} else {
		//	backoff /= overbounce;
		//}
		//float change[3];
		//change[0] = planeNormal[0] * backoff;
		//change[1] = planeNormal[1] * backoff;
		//change[2] = planeNormal[2] * backoff;
		//float changeLength = VectorLength(change);
		//float playerVelocityNormal[3];
		//float speedBefore = VectorLength(playerVelocityIn);
		//for ( i	=0 ; i<3 ; i++ ) {
		//	playerVelocityOut[i] = playerVelocityIn[i] - change[i];
		//	//playerVelocityOut[i] += playerVelocityIn[i] * changeLength; // keep velocity, just in other direction
		//}
		//float speedAfter = VectorLength(playerVelocityIn);
		//float needSpeed = speedBefore - speedAfter;
		//VectorNormalize2(playerVelocityIn, playerVelocityNormal);
		//playerVelocityIn[0] += playerVelocityNormal[0] * needSpeed; // * playerVelocityNormalized_dot_planeNormal;
		//playerVelocityIn[1] += playerVelocityNormal[1] * needSpeed; // * playerVelocityNormalized_dot_planeNormal;
		//playerVelocityIn[2] += playerVelocityNormal[2] * needSpeed; // * playerVelocityNormalized_dot_planeNormal;
		////playerVelocityIn[2] += changeLength * 2; // * backoff;

	}


	

#if 0
	//ZOID:  If we are on the grapple, try stair-stepping
	//this allows a player to use the grapple to pull himself
	//over a ledge
	if (pm->ps->pm_flags & PMF_GRAPPLE_PULL)
		PM_StepSlideMove ( qtrue );
	else
		PM_SlideMove ( qtrue );
#endif

	//float speedBefore = VectorLength(pm->ps->velocity);

	PM_StepSlideMove ( qtrue );

	//float speedAfter = VectorLength(pm->ps->velocity);
	//float needSpeed = speedBefore - speedAfter;
	//// only add missing speed, dont substract if player became slower
	//if (needSpeed > 0.0 && pml.groundPlane) {
	//	float playerVelocityNormal[3];
	//	VectorNormalize2(pm->ps->velocity, playerVelocityNormal);
	//	pm->ps->velocity[0] += playerVelocityNormal[0] * needSpeed;
	//	pm->ps->velocity[1] += playerVelocityNormal[1] * needSpeed;
	//	pm->ps->velocity[2] += playerVelocityNormal[2] * needSpeed;
	//}

	// needs to be below PM_StepSlideMove, otherwise the player "sticks" to the sliding surface
	// when left or right button is pressed...
#if 0
	if (pm->cmd.rightmove) {
		vec3_t velocityNormal;
		VectorNormalize2(pm->ps->velocity, velocityNormal);

		vec3_t angles = {0,0,0};
		vec3_t forward2d;
		angles[1] = pm->ps->viewangles[1];
		AngleVectors(angles, forward2d, NULL, NULL);

 		float *vel = pm->ps->velocity;

		float oldZ = pm->ps->velocity[2];
		pm->ps->velocity[2] = 0.0f;
		float xyVelocityLength = sqrtf(vel[0] * vel[0] + vel[1] * vel[1]);

		if (xyVelocityLength < g_air_acceleration_max->value) {
			if (xyVelocityLength < 1.0f)
				xyVelocityLength = 1.0f;
			float add = g_air_acceleration_div->value / (xyVelocityLength * g_air_acceleration_mul->value);
			xyVelocityLength += xyVelocityLength * add * pml.frametime;
		}
		
		pm->ps->velocity[0] = forward2d[0] * xyVelocityLength;
		pm->ps->velocity[1] = forward2d[1] * xyVelocityLength;
		pm->ps->velocity[2] = oldZ;

		if (pm->imgui) {

			ImGui::Begin("AirMove");

		
			ImGui::Text("wishvel");
			ImGui::SameLine();
			ImGui::DragVector3(wishvel);
		
			ImGui::Text("velocityNormal");
			ImGui::SameLine();
			ImGui::DragVector3(velocityNormal);
		
			ImGui::Text("forward2d");
			ImGui::SameLine();
			ImGui::DragVector3(forward2d);

			ImGui::Text("pm->ps->velocity");
			ImGui::SameLine();
			ImGui::DragVector3(pm->ps->velocity);

			// [0] is up/down, [1] is left/right -180 to 180
			ImGui::Text("pm->ps->viewangles");
			ImGui::SameLine();
			ImGui::DragVector3(pm->ps->viewangles);
		
			ImGui::Text("xyVelocityLength = %f pml.frametime=%f", xyVelocityLength, pml.frametime);
			ImGui::Text("oldZ = %f", oldZ);

			ImGui::End();

		}
	}
#else
#endif
}