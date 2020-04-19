#ifndef G_WEAPON_H
#define G_WEAPON_H

#include "g_local.h"

typedef struct serverweapon_s {
	int (*fire)(gentity_t *ent);
	int (*pm_weapon)(pmove_t *pm);
} serverweapon_t;

EXTERNC serverweapon_t serverweapons[32];

int bazooka_fire(gentity_t *player);
int bazooka_pm_weapon(pmove_t *pm);

#endif