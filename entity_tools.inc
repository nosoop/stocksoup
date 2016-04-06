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