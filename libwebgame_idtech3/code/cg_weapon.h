#ifndef CG_WEAPON_H
#define CG_WEAPON_H

#include "cg_local.h"

void          crowbar_precache(weaponInfo_t *weapon);
void          crowbar_activate(weaponInfo_t *weapon, centity_t *cent);
void            sword_precache(weaponInfo_t *weapon);
void            sword_activate(weaponInfo_t *weapon, centity_t *cent);
void            kar98_precache(weaponInfo_t *weapon);
void            kar98_activate(weaponInfo_t *weapon, centity_t *cent);
void             mp44_precache(weaponInfo_t *weapon);
void             mp44_activate(weaponInfo_t *weapon, centity_t *cent);
void    panzerschreck_precache(weaponInfo_t *weapon);
void    panzerschreck_activate(weaponInfo_t *weapon, centity_t *cent);
void          bazooka_precache(weaponInfo_t *weapon);
void          bazooka_activate(weaponInfo_t *weapon, centity_t *cent);

#endif