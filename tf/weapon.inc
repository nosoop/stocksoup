/**
 * Provides stocks that deal with weapon entities.
 */

#if defined __stocksoup_tf_weapon_included
	#endinput
#endif

#define __stocksoup_tf_weapon_included

#include <sdktools_functions>

/**
 * Gives a weapon ammo.  Rather, it gives ammo to the player holding the weapon.
 * This stock uses GivePlayerAmmo, so it obeys the maximum ammo count that the player can carry.
 * 
 * @return Amount of ammo actually given.
 */
stock void TF2_GiveWeaponAmmo(int weapon, int amount, bool supressSound = true) {
	int ammoType = GetEntProp(weapon, Prop_Send, "m_iPrimaryAmmoType");
	int client = GetEntPropEnt(weapon, Prop_Send, "m_hOwner");
	
	if (client > 0 && client <= MaxClients) {
		return GivePlayerAmmo(client, amount, ammoType, supressSound);
	}
	return 0;
}