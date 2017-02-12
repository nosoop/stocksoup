/**
 * Utility stocks for files.
 */
#if defined __stocksoup_file_included
	#endinput
#endif

#define __stocksoup_file_included

#include <stocksoup/string>

stock bool CreateDirectories(const char[] path, int mode, bool use_valve_fs = false,
		const char[] valve_path_id = "DEFAULT_WRITE_PATH") {
	ArrayList subdirs = ExplodeStringToList(path, "/", PLATFORM_MAX_PATH, true);
	
	char pathBuffer[PLATFORM_MAX_PATH], subdir[PLATFORM_MAX_PATH];
	for (int i = 0; i < subdirs.Length; i++) {
		subdirs.GetString(i, subdir, sizeof(subdir));
		
		StrCat(pathBuffer, sizeof(pathBuffer), subdir);
		StrCat(pathBuffer, sizeof(pathBuffer), "/");
		
		if (!DirExists(pathBuffer, use_valve_fs, valve_path_id)
				&& !CreateDirectory(pathBuffer, mode, use_valve_fs, valve_path_id)) {
			return false;
		}
	}
	return true;
}
