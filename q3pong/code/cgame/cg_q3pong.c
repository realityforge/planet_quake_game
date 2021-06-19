#include "cg_local.h"
#include "cg_q3pong.h"

void Ball_AddShadow(refEntity_t *ent);

#define VectorAverage(v1, v2, vo)   (vo)[0] = ((v1)[0] + (v2)[0]) / 2; (vo)[1] = ((v1)[1] + (v2)[1]) / 2; (vo)[2] = ((v1)[2] + (v2)[2]) / 2;

void CG_Ball( centity_t *cent ) {
    refEntity_t			ent;
    entityState_t		*s1;
    float				scale;

    s1 = &cent->currentState;

    // create the render entity
    memset (&ent, 0, sizeof(ent));
    VectorCopy( cent->lerpOrigin, ent.origin);
    VectorCopy( cent->lerpOrigin, ent.oldorigin);

    ent.customSkin = cgs.gameSkins[s1->otherEntityNum];
    ent.hModel = cgs.gameModels[s1->modelindex];
    ent.renderfx = 0; //RF_NOSHADOW;

    VectorCopy(s1->angles, ent.axis[0]);
    VectorCopy(s1->angles2, ent.axis[2]);
    VectorNormalize(ent.axis[0]);
    VectorNormalize(ent.axis[2]);

    CrossProduct(ent.axis[0], ent.axis[2], ent.axis[1]);
    VectorNormalize(ent.axis[1]);

    //CG_Printf("(%.4f %.4f %.4f)(%.4f %.4f %.4f)(%.4f %.4f %.4f)\n", ent.axis[0][0], ent.axis[0][1], ent.axis[0][2], ent.axis[1][0], ent.axis[1][1], ent.axis[1][2], ent.axis[2][0], ent.axis[2][1], ent.axis[2][2]);

    scale = (float)(s1->generic1) / 100.0f;

    ent.nonNormalizedAxes = qtrue;
    VectorScale(ent.axis[0], scale, ent.axis[0]);
    VectorScale(ent.axis[1], scale, ent.axis[1]);
    VectorScale(ent.axis[2], scale, ent.axis[2]);

    ent.radius = scale * 40;
    Ball_AddShadow(&ent);

    // add to refresh list
    trap_R_AddRefEntityToScene( &ent );
}

void Ball_AddShadow(refEntity_t *ent){
    vec3_t		end, mins = {-15, -15, 0}, maxs = {15, 15, 2};
    trace_t		trace;
    float		alpha;

    ent->shadowPlane = 0;

    if ( cg_shadows.integer == 0 ) {
        return;
    }

    // send a trace down from the ball to the ground
    VectorCopy( ent->origin, end );
    end[2] -= 1024;

    trap_CM_BoxTrace( &trace, ent->origin, end, mins, maxs, 0, MASK_PLAYERSOLID );

    // no shadow if too high
    if ( trace.fraction == 1.0 ) {
        return;
    }

    ent->shadowPlane = trace.endpos[2] + 1;

    // fade the shadow out with height
    alpha = 1.0 - trace.fraction;

    // add the mark as a temporary, so it goes directly to the renderer
    // without taking a spot in the cg_marks array
    CG_ImpactMark( cgs.media.shadowMarkShader, trace.endpos, trace.plane.normal, 
        0, alpha,alpha,alpha,1, qfalse, ent->radius, qtrue );

}

