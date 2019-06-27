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
 * @param entity		Edict index.
 * @param buffer		Buffer to store the target name.
 * @param maxlen		Maximum size of the buffer.
 * @return				Number of non-null bytes written.
 */
stock int GetEntityTargetName(int entity, char[] buffer, int maxlen) {
	return GetEntPropString(entity, Prop_Data, "m_iName", buffer, maxlen);
}
