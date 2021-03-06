/**
 * Utility stocks for KeyValues handles.
 */

#if defined __stocksoup_keyvalues_included
	#endinput
#endif

#define __stocksoup_keyvalues_included

/**
 * Copies the subkeys in the origin KeyValues to the destination KeyValues.
 * Any subkeys in the destination that are not overwritten are preserved.
 * 
 * @param recursive      If true, subsections are merged.  Otherwise, only the top-level subkeys
 *                       from the origin are copied to the destination.
 */
stock void KvMergeSubKeys(KeyValues origin, KeyValues dest, bool recursive = false,
		int keysize = 1024, int valuesize = 1024) {
	char[] key = new char[keysize];
	char[] value = new char[valuesize];
	
	origin.GotoFirstSubKey(false);
	do {
		origin.GetSectionName(key, keysize);
		if (origin.GetDataType(NULL_STRING) == KvData_None) {
			// subsection
			if (!recursive) {
				continue;
			}
			if (dest.JumpToKey(key)) {
				// merge with existing child
				KvMergeSubKeys(origin, dest, recursive, keysize, valuesize);
			} else {
				// import new child and don't traverse
				dest.JumpToKey(key, true);
				dest.Import(origin);
			}
			dest.GoBack();
		} else {
			// plain key / value pair
			origin.GetString(NULL_STRING, value, valuesize);
			dest.SetString(key, value);
		}
	} while (origin.GotoNextKey(false));
	origin.GoBack();
}
