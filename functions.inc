/**
 * Utility stocks for functions.
 */
#if defined __stocksoup_functions_included
	#endinput
#endif

#define __stocksoup_functions_included

/**
 * Gets a client index from a native parameter.  If the client index is out of bounds or the
 * client is not in-game, the function throws a native error.
 * 
 * @param param        Parameter number, starting from 1.
 * @return             Client index at the parameter number.  This index is guaranteed to be an
 *                     in-game client.
 */
stock int GetNativeInGameClient(int param) {
	int client = GetNativeCell(param);
	if (client < 1 || client > MaxClients) {
		ThrowNativeError(SP_ERROR_NATIVE, "Client index %d is not valid (param %d)", client,
				param);
	} else if (!IsClientInGame(client)) {
		ThrowNativeError(SP_ERROR_NATIVE, "Client index %d is not in game (param %d)", client,
				param);
	}
	return client;
}

/**
 * Gets an entity index or reference from a native parameter.  If the entity is not valid, the
 * function throws a native error unless NULL entities are specifically allowed.
 * 
 * @param param        Parameter number, starting from 1.
 * @param allowNull    If true, the native will accept and return INVALID_ENT_REFERENCE if it
 *                     was explicitly passed in.  Other values will still throw errors if the
 *                     entity is not valid.
 * @return             Entity index or reference at the parameter number.  This index is
 *                     guaranteed to refer to a valid entity, or if allowNull is set,
 *                     may also refer to INVALID_ENT_REFERENCE.
 */
stock int GetNativeEntity(int param, bool allowNull = false) {
	int entity = GetNativeCell(param);
	if (entity == INVALID_ENT_REFERENCE) {
		if (allowNull) {
			return INVALID_ENT_REFERENCE;
		}
		ThrowNativeError(SP_ERROR_NATIVE, "Entity is NULL (param %d)", param);
	} else if (!IsValidEntity(entity)) {
		ThrowNativeError(SP_ERROR_NATIVE, "Entity %d is invalid (param %d)", entity, param);
	}
	return entity;
}
