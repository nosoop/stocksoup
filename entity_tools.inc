#if defined __stocksoup_entity_utils_included
	#endinput
#endif

#define __stocksoup_entity_utils_included

stock bool ParentEntity(int parent, int attachment, const char[] attachPoint = "",
		bool maintainOffset = false) {
	SetVariantString("!activator");
	AcceptEntityInput(attachment, "SetParent", parent, attachment, 0);
	
	if (strlen(attachPoint) > 0) {
		SetVariantString(attachPoint);
		AcceptEntityInput(attachment,
				maintainOffset? "SetParentAttachmentMaintainOffset" : "SetParentAttachment",
				parent, parent);
	}
}

/**
 * Copies the entity's model into the specified buffer.
 */
stock void GetEntityModelPath(int entity, char[] buffer, int maxlen) {
	GetEntPropString(entity, Prop_Data, "m_ModelName", buffer, maxlen);
}

/**
 * Attempts to retrieve a target name for an entity.
 */
stock bool GetEntityTargetName(int entity, char[] target, int maxlen) {
	return !!GetEntPropString(entity, Prop_Data, "m_iName", buffer, maxlen);
}

/**
 * Searches for an entity by target name. 
 */
stock int FindEntityByTargetName(int startEnt = -1, const char[] target,
		const char[] className = "*") {
	int targetmaxlen = strlen(target) + 1;
	char[] targetbuf = new char[targetmaxlen];
	while ((startEnt = FindEntityByClassname(startEnt, className)) != -1) {
		GetEntityTargetName(startEnt, targetbuf, targetmaxlen);
		
		if (StrEqual(target, targetbuf)) {
			return startEnt;
		}
	}
	return startEnt;
}
