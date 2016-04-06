#if !defined __tf_glow_model_included
	#define __tf_glow_model_included
#else
	#endinput
#endif

#pragma newdecls optional
#include <tf2>
#pragma newdecls required

stock int AddGlowModel(int entity, TFTeam team = TFTeam_Unassigned) {
	char model[PLATFORM_MAX_PATH];
	GetEntPropString(entity, Prop_Data, "m_ModelName", model, PLATFORM_MAX_PATH);
	
	if (strlen(model) != 0) {
		int prop = CreateEntityByName("tf_taunt_prop");
		
		if (IsValidEntity(prop)) {
			if (team != TFTeam_Unassigned) {
				SetEntProp(prop, Prop_Data, "m_iInitialTeamNum", view_as<int>(team));
				SetEntProp(prop, Prop_Send, "m_iTeamNum", view_as<int>(team));
			}
			DispatchSpawn(prop);

			SetEntityModel(prop, model);

			SetEntPropEnt(prop, Prop_Data, "m_hEffectEntity", entity);
			SetEntProp(prop, Prop_Send, "m_bGlowEnabled", 1);
			
			int iFlags = GetEntProp(prop, Prop_Send, "m_fEffects");
			
			int efBoneMerge = (1 << 0), efNoShadow = 16, efNoInterp = 8;
			
			SetEntProp(prop, Prop_Send, "m_fEffects",
					iFlags | efBoneMerge | efNoShadow | efNoInterp);

			SetVariantString("!activator");
			AcceptEntityInput(prop, "SetParent", entity);
			
			SetEntityRenderMode(prop, RENDER_TRANSCOLOR);
			SetEntityRenderColor(prop, 255, 255, 255, 0);
		}
		return prop;
	}
	return -1;
}
