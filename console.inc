/**
 * A couple of convenience functions for commands.
 */
#if defined __stocksoup_console_included
	#endinput
#endif

#define __stocksoup_console_included

stock bool GetCmdArgInt(int argnum, int &value, int nBase = 10) {
	char buffer[32];
	if (!GetCmdArg(argnum, buffer, sizeof(buffer))) {
		return false;
	}
	return !!StringToIntEx(buffer, value, nBase);
}

stock bool GetCmdArgFloat(int argnum, float &value) {
	char buffer[32];
	if (!GetCmdArg(argnum, buffer, sizeof(buffer))) {
		return false;
	}
	return !!StringToFloatEx(buffer, value);
}
