/**
 * Provides stocks that deal with player entities.
 */

#if defined __stocksoup_tf_player_included
	#endinput
#endif

#define __stocksoup_tf_player_included

#include <stocksoup/traceutils>

/**
 * Attempts to heal player by the specified amount.  Also provides functionality to visually
 * indicate to players that they have been healed.
 * 
 * @return true if the player was successfully (over) healed, false if no heals were applied
 */
stock bool TF2_HealPlayer(int client, int nHealAmount, bool overheal = false,
		bool notify = false) {
	if (IsPlayerAlive(client)) {
		int nHealth = GetClientHealth(client);
		int nMaxHealth = TF2_GetPlayerMaxHealth(client);
		
		// cap heals to max health
		if (!overheal && nHealAmount > nMaxHealth - nHealth) {
			nHealAmount = nMaxHealth - nHealth;
		}
		
		if (nHealAmount > 0) {
			SetEntityHealth(client, nHealth + nHealAmount);
			
			// player health HUD notification
			if (notify) {
				Event event = CreateEvent("player_healonhit");
				if (event) {
					event.SetInt("amount", nHealAmount);
					event.SetInt("entindex", client);
					
					event.FireToClient(client);
					delete event;
				}
			}
			
			return true;
		}
	}
	return false;
}

/**
 * Returns the current maximum amount of health that a player can have.
 */
stock int TF2_GetPlayerMaxHealth(int client) {
	return GetEntProp(client, Prop_Data, "m_iMaxHealth");
}

/**
 * Attempts to teleport the player to the given position without collisions.
 * 
 * @param client	Client to unstuck.
 * @param scale		Player scale modifier.  This determines the hull size.
 * @param position	Desired position.  If NULL_VECTOR, uses current player position.
 */
stock bool TF2_UnstuckPlayer(int client, float scale = 1.0, float searchScale = 1.0,
		const float position[3] = NULL_VECTOR) {
	float vecMins[3], vecMaxs[3], vecPosition[3], vecDestination[3];
	GetEntPropVector(client, Prop_Data, "m_vecMinsPreScaled", vecMins);
	GetEntPropVector(client, Prop_Data, "m_vecMaxsPreScaled", vecMaxs);
	
	ScaleVector(vecMins, scale);
	ScaleVector(vecMaxs, scale);
	
	if (IsNullVector(position)) {
		GetClientAbsOrigin(client, vecPosition);
	} else {
		vecPosition = position;
	}
	
	if (FindNonCollideHullPosition(vecPosition, vecMins, vecMaxs, vecDestination,
			MASK_PLAYERSOLID, __stocksoup_UnstuckTraceFilter, client)) {
		TeleportEntity(client, vecDestination, NULL_VECTOR, NULL_VECTOR);
		return true;
	}
	
	// check a little higher for slopes
	vecPosition[2] += 10.0;
	if (FindNonCollideHullPositionExtent(vecPosition, vecMins, vecMaxs, vecDestination,
			MASK_PLAYERSOLID, __stocksoup_UnstuckTraceFilter, client, searchScale)) {
		TeleportEntity(client, vecDestination, NULL_VECTOR, NULL_VECTOR);
		return true;
	}
	
	return false;
}

/**
 * Return true if teleport should be stopped.
 */
static stock bool __stocksoup_UnstuckTraceFilter(int entity, int contents, int self) {
	return entity > 0 && (self == INVALID_ENT_REFERENCE || self != entity);
}
