#if defined __stocksoup_tf_teams_included
	#endinput
#endif

#define __stocksoup_tf_teams_included

enum TFTeamRoles {
	TFTeamRole_Undefined = 0,
	TFTeamRole_Defenders,
	TFTeamRole_Attackers
};

stock TFTeamRoles TF2_GetTeamRole(TFTeam team) {
	int teamEntity = TF2_GetTeamEntity(team);
	if (IsValidEntity(teamEntity)) {
		return view_as<TFTeamRoles>(GetEntProp(team, Prop_Send, "m_iRole"));
	}
	return TFTeamRole_Undefined;
}

stock int TF2_GetTeamEntity(TFTeam team) {
	while ((team = FindEntityByClassname(team, "tf_team")) != -1) {
		if (team == view_as<TFTeam>(GetEntProp(team, Prop_Send, "m_iTeamNum"))) {
			return team;
		}
	}
	return INVALID_ENT_REFERENCE;
}