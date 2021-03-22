#if defined __stocksoup_convars_included
	#endinput
#endif

#define __stocksoup_convars_included

/**
 * Creates a standard "version" ConVar for plugins to advertise their current version.
 */
stock ConVar CreateVersionConVar(const char[] name, const char[] description = "") {
	ConVar convar;
	char version[16];
	if (GetPluginInfo(INVALID_HANDLE, PlInfo_Version, version, sizeof(version))) {
		// FCVAR_NOTIFY posts the ConVar to A2S_RULES
		// FCVAR_DONTRECORD prevents the ConVar from being added to AutoExecConfig
		convar = CreateConVar(name, "", description, FCVAR_NOTIFY | FCVAR_DONTRECORD);
		convar.AddChangeHook(__stocksoup_LockVersionConVar);
		convar.SetString(version);
	}
	return convar;
}

stock void LockConVar(ConVar convar) {
	convar.RestoreDefault();
	convar.AddChangeHook(__stocksoup_LockConVar);
}

static stock void __stocksoup_LockConVar(ConVar convar, const char[] oldValue,
		const char[] newValue) {
	char defaultValue[64];
	convar.GetDefault(defaultValue, sizeof(defaultValue));
	
	if (!StrEqual(newValue, defaultValue)) {
		convar.RestoreDefault();
	}
}

/**
 * Default value may be stale from a previous plugin load; always pull the data from plugin
 * info.
 */
static stock void __stocksoup_LockVersionConVar(ConVar convar, const char[] oldValue,
		const char[] newValue) {
	char version[16];
	GetPluginInfo(INVALID_HANDLE, PlInfo_Version, version, sizeof(version));
	
	if (!StrEqual(newValue, version)) {
		convar.SetString(version);
	}
}
