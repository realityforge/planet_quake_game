#include "g_weapon.h"

void Weapon_RocketLauncher_Fire( gentity_t *ent );

int bazooka_fire(gentity_t *player) {
	Weapon_RocketLauncher_Fire( player );
	return 1;
}

int bazooka_pm_weapon(pmove_t *pm) {
	pm->ps->weaponTime += 1000; // one shoot per second
	return 1;
}