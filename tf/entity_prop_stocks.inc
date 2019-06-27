/**
 * Provides stocks for TF2 entity properties.
 */
#if defined __stocksoup_tf_entity_prop_stocks_included
	#endinput
#endif

#define __stocksoup_tf_entity_prop_stocks_included

#include <tf2_stocks>
#include <stocksoup/tf/teams>

/**
 * Returns a player's class disguise, or TFClass_Unknown if not disguised.
 */
stock TFClassType TF2_GetPlayerDisguiseClass(int client) {
	if (!TF2_IsPlayerInCondition(client, TFCond_Disguised)) {
		return TFClass_Unknown;
	}
	return view_as<TFClassType>(GetEntProp(client, Prop_Send, "m_nDisguiseClass"));
}

/**
 * Returns a player's team disguise, or TFTeam_Unassigned if not disguised.
 */
stock TFTeam TF2_GetClientDisguiseTeam(int client) {
	if (!TF2_IsPlayerInCondition(client, TFCond_Disguised)) {
		return TFTeam_Unassigned;
	}
	return view_as<TFTeam>(GetEntProp(client, Prop_Send, "m_nDisguiseTeam"));
}

/**
 * Returns a player's disguise health.
 */
stock int TF2_GetClientDisguiseHealth(int client) {
	return GetEntProp(client, Prop_Send, "m_iDisguiseHealth");
}

/**
 * Checks if a player is in a spawn room.
 * 
 * @param client			Client index.
 * @return					True if the spawn room touch count is non-zero.
 */
stock bool TF2_IsPlayerInSpawnRoom(int client) {
	return !!GetEntProp(client, Prop_Send, "m_iSpawnRoomTouchCount");
}

/**
 * Checks if a player is a coach.
 * 
 * @param client			Client index.
 * @return					True if the player is coaching.
 */
stock bool TF2_IsPlayerCoaching(int client) {
	return !!GetEntProp(client, Prop_Send, "m_bIsCoaching");
}

/**
 * Returns the class that `other` views `client` as.  This is intended to show disguises to
 * enemy players.
 * 
 * @param client			Client index.
 * @param other				Client index that would be viewing `client`.
 * @return					True if the other entity sees `client` as their disguise class.
 */
stock TFClassType TF2_GetPlayerClassFromClient(int client, int other) {
	return TF2_IsPlayerDisguisedFromClient(client, other)?
			TF2_GetPlayerDisguiseClass(client) : TF2_GetPlayerClass(client);
}

/**
 * Returns the team that `other` views `client` as.  This is intended for friendly team checks
 * that are fooled by disguises.
 * 
 * @param client			Client index.
 * @param other				Client index that would be viewing `client`.
 * @return					True if the other entity sees `client` as their disguise team.
 */
stock TFTeam TF2_GetClientTeamFromClient(int client, int other) {
	return TF2_IsPlayerDisguisedFromClient(client, other)?
			TF2_GetClientDisguiseTeam(client) : TF2_GetClientTeam(client);
}

/**
 * Returns true if `other` sees `client` as their disguise.
 * 
 * @param client			Client index.
 * @param other				Client index that would be viewing `client`.
 * @return					True if the other entity sees `client` as their disguise.
 */
stock bool TF2_IsPlayerDisguisedFromClient(int client, int other) {
	return TF2_IsPlayerDisguisedFromTeam(client, TF2_GetClientTeam(other));
}

/**
 * Checks if a player shows their disguise towards a given team.
 * 
 * @param client			Client index.
 * @param team				Team that would be viewing `client`.
 * @return					True if the given team sees `client` as their disguise.
 */
stock bool TF2_IsPlayerDisguisedFromTeam(int client, TFTeam team) {
	return TF2_IsPlayerInCondition(client, TFCond_Disguised)
			&& TF2_IsEnemyTeam(TF2_GetClientTeam(client), team);
}

/**
 * Returns a player's active weapon.
 * 
 * @param client			Client index.
 * @return					Player's active weapon.
 */
stock int TF2_GetClientActiveWeapon(int client) {
	return GetEntPropEnt(client, Prop_Send, "m_hActiveWeapon");
}

/**
 * Returns an entity's owner.
 */
stock int TF2_GetEntityOwner(int entity) {
	return GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity");
}

/**
 * Returns the builder of a object (i.e., a TF2 building / sapper).
 */
stock int TF2_GetObjectBuilder(int entity) {
	if (!HasEntProp(entity, Prop_Send, "m_hBuilder")) {
		ThrowError("Entity index %d is not an object", entity);
	}
	return GetEntPropEnt(entity, Prop_Send, "m_hBuilder");
}
