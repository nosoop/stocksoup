#if defined __stocksoup_entity_utils_included
	#endinput
#endif

#define __stocksoup_entity_utils_included

#include <stocksoup/entity_prop_stocks>

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
