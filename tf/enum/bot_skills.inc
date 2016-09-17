/** 
 * A list of bot difficulties in Team Fortress 2, as well as getters and setters for them.
 */

#if defined __stocksoup_tf_enum_bot_skills_included
	#endinput
#endif

#define __stocksoup_tf_enum_bot_skills_included

enum TFBotSkill {
	Skill_Easy = 0,
	Skill_Normal,
	Skill_Hard,
	Skill_Expert
};

stock TFBotSkill TF2_GetBotSkill(int client) {
	return view_as<TFBotSkill>(GetEntProp(client, Prop_Send, "m_nBotSkill"));
}

stock void TF2_SetBotSkill(int client, TFBotSkill skill) {
	SetEntProp(client, Prop_Send, "m_nBotSkill", view_as<int>(skill));
}