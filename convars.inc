#if defined __stocksoup_convars_included
	#endinput
#endif

#define __stocksoup_convars_included

stock ConVar CreateVersionConVar(const char[] name, const char[] description = "") {
	ConVar convar;
	char version[16];
	if (GetPluginInfo(INVALID_HANDLE, PlInfo_Version, version, sizeof(version))) {
		convar = CreateConVar(name, version, description, FCVAR_NOTIFY | FCVAR_DONTRECORD);
		LockConVar(convar);
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
