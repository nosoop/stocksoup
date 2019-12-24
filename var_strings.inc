/**
 * Basic parser for space-delimited key=value pairs.
 * Returns the value given by the first match.
 */

#if defined __stocksoup_var_strings_included
	#endinput
#endif

#define __stocksoup_var_strings_included

#include <stocksoup/string>

/**
 * Reads a float value associated with a key.
 */
stock float ReadFloatVar(const char[] varstring, const char[] key, float flDefaultValue = 0.0) {
	int iValPos = FindKeyAssignInString(varstring, key);
	if (iValPos == -1) {
		return flDefaultValue;
	}
	
	float retVal;
	if (StringToFloatEx(varstring[iValPos], retVal)) {
		return retVal;
	}
	return flDefaultValue;
}

/**
 * Reads an integer value associated with a key.
 */
stock int ReadIntVar(const char[] varstring, const char[] key, int iDefaultValue = 0) {
	int iValPos = FindKeyAssignInString(varstring, key);
	if (iValPos == -1) {
		return iDefaultValue;
	}
	
	int retVal;
	if (StringToIntEx(varstring[iValPos], retVal)) {
		return retVal;
	}
	return iDefaultValue;
}

/**
 * Reads a string value associated with a key.
 */
stock bool ReadStringVar(const char[] varstring, const char[] key, char[] buffer, int maxlen,
		const char[] defVal = "") {
	int iValPos = FindKeyAssignInString(varstring, key);
	if (iValPos == -1) {
		strcopy(buffer, maxlen, defVal);
		return false;
	}
	
	strcopy(buffer, maxlen, varstring[iValPos]);
	int space;
	if ((space = FindCharInString(buffer, ' ')) != -1) {
		buffer[space] = '\0';
	}
	return true;
}

/**
 * Returns the position of the first character after the first instance of `key=`, or -1 if the
 * key doesn't exist.
 * 
 * The instance of `key=` will either be at the start of the string or preceded by a space.
 */
static stock int FindKeyAssignInString(const char[] varstring, const char[] key) {
	char keyBuf[32];
	strcopy(keyBuf, sizeof(keyBuf), key);
	StrCat(keyBuf, sizeof(keyBuf), "=");
	
	int keylen = strlen(keyBuf);
	int split = -1;
	while ((split = FindNextSplitInString(split, varstring, keyBuf)) != -1) {
		int start = split - keylen;
		if (!start || varstring[start - 1] == ' ') {
			return split;
		}
	}
	return split;
}
