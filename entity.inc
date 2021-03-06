/**
 * Entity property helpers.
 */

#if defined __stocksoup_entity_included
	#endinput
#endif

#define __stocksoup_entity_included

#define COPYPROP_MAX_STRING_LENGTH 256

/**
 * Assigns an integer value to the `dest` entity property from the `src` entity property.
 */
stock void CopyEntProp(int src, int dest, PropType type, const char[] prop, int size = 4,
		int element = 0) {
	SetEntProp(dest, type, prop, GetEntProp(src, type, prop, size, element), size, element);
}

/**
 * Assigns a float value to the `dest` entity property from the `src` entity property.
 */
stock void CopyEntPropFloat(int src, int dest, PropType type, const char[] prop,
		int element = 0) {
	SetEntPropFloat(dest, type, prop, (GetEntPropFloat(src, type, prop, element)), element);
}

/**
 * Assigns an entity index to the `dest` entity property from the `src` entity property.
 */
stock void CopyEntPropEnt(int src, int dest, PropType type, const char[] prop,
		int element = 0) {
	SetEntPropEnt(dest, type, prop, (GetEntPropEnt(src, type, prop, element)), element);
}

/**
 * Assigns a string value to the `dest` entity property from the `src` entity property.
 */
stock void CopyEntPropString(int src, int dest, PropType type, const char[] prop,
		int element = 0, int maxlen = COPYPROP_MAX_STRING_LENGTH) {
	char[] buffer = new char[maxlen];
	GetEntPropString(src, type, prop, buffer, maxlen, element);
	SetEntPropString(dest, type, prop, buffer, element);
}

/**
 * Assigns a vector of floats to the `dest` entity property from the `src` entity property.
 */
stock void CopyEntPropVector(int src, int dest, PropType type, const char[] prop,
		int element = 0) {
	float vec[3];
	GetEntPropVector(src, type, prop, vec, element);
	SetEntPropVector(dest, type, prop, vec, element);
}

/**
 * Marks an entity for deletion, deferred until the next frame.
 */
stock void RemoveEntityNextFrame(int entity) {
	RequestFrame(OnRemoveEntityFrame, EntIndexToEntRef(entity));
}

/**
 * Internal `RequestFrameCallback` function that marks a given entity for deletion.
 */
static stock void OnRemoveEntityFrame(int entref) {
	int entity = EntRefToEntIndex(entref);
	if (IsValidEntity(entity)) {
		RemoveEntity(entity);
	}
}

/**
 * Marks an entity for deletion after a given delay.
 */
stock void RemoveEntityDelayed(int entity, float delay) {
	CreateTimer(delay, OnRemoveEntityTimer, EntIndexToEntRef(entity), TIMER_FLAG_NO_MAPCHANGE);
}

/**
 * Internal `Timer` callback function that marks a given entity for deletion.
 */
static stock Action OnRemoveEntityTimer(Handle timer, int entref) {
	int entity = EntRefToEntIndex(entref);
	if (IsValidEntity(entity)) {
		RemoveEntity(entity);
	}
	return Plugin_Handled;
}
