#if defined __stocksoup_entity_prop_stocks_included
	#endinput
#endif

#define __stocksoup_entity_prop_stocks_included

/**
 * Copies the entity's model into the specified buffer.
 */
stock int GetEntityModelPath(int entity, char[] buffer, int maxlen) {
	return GetEntPropString(entity, Prop_Data, "m_ModelName", buffer, maxlen);
}

/**
 * Stores the target name of an entity into a buffer.
 * 
 * @param entity        Edict index.
 * @param buffer        Buffer to store the target name.
 * @param maxlen        Maximum size of the buffer.
 * @return              Number of non-null bytes written.
 */
stock int GetEntityTargetName(int entity, char[] buffer, int maxlen) {
	return GetEntPropString(entity, Prop_Data, "m_iName", buffer, maxlen);
}

/**
 * Sets the target name of an entity.
 * 
 * @param entity        Edict index.
 * @param targetName    New target name.
 */
stock void SetEntityTargetName(int entity, const char[] targetName) {
	SetEntPropString(entity, Prop_Data, "m_iName", targetName);
}

/**
 * Returns the entity that this entity is attached to, if any.
 * 
 * @param entity    Entity index or reference.
 * @return          Movement parent entity index, or -1 if entity is parentless.
 */
stock int GetEntityMoveParent(int entity) {
	if (!IsValidEntity(entity)) {
		return INVALID_ENT_REFERENCE;
	}
	return GetEntPropEnt(entity, Prop_Data, "m_hMoveParent");
}

/**
 * Returns the first child in the movement relationship.
 * 
 * To enumerate the other movement children, use `GetEntityMovePeer()`; you will get an
 * `INVALID_ENT_REFERENCE` once all child entities have been enumerated.
 *
 * @param entity    Entity index or reference.
 * @return          Movement child entity index, or -1 if no entity is a parent to this one.
 */
stock int GetEntityMoveFirstChild(int entity) {
	if (!IsValidEntity(entity)) {
		return INVALID_ENT_REFERENCE;
	}
	return GetEntPropEnt(entity, Prop_Data, "m_hMoveChild");
}

/**
 * Returns the next entity in the movement relationship, sharing the same movement parent.
 * 
 * @param entity    Entity index or reference.
 * @return          Next movement peer entity index, or -1 if entity is not in a movement
 *                  relationship or if entity is the last one in the list.
 */
stock int GetEntityMovePeer(int entity) {
	if (!IsValidEntity(entity)) {
		return INVALID_ENT_REFERENCE;
	}
	return GetEntPropEnt(entity, Prop_Data, "m_hMovePeer");
}
