/**
 * Utility stocks for files.
 */
#if defined __stocksoup_file_included
	#endinput
#endif

#define __stocksoup_file_included

/**
 * Creates a directory, creating parent directories if they don't exist.
 * 
 * @param path			Path to create.  Parent directories that do not exist are also created.
 * @param mode			Permissions (default is o=rx,g=rx,u=rwx).  Note that folders must have
 *						the execute bit set on Linux.  On Windows, the mode is ignored.
 * 						Nonexisting directories will be created with these permissions; existing
 * 						directories' permissions will not be modified.
 * @param use_valve_fs	If true, the Valve file stem will be used instead.  This can be used to
 * 						create folders in the game's Valve search paths, rather than directly in
 * 						the gamedir.
 * @param valve_path_id	If use_valve_fs, a search path from gameinfo or NULL_STRING for default.
 * 						In this case, mode is ignored.
 */
stock bool CreateDirectories(const char[] path, int mode, bool use_valve_fs = false,
		const char[] valve_path_id = "DEFAULT_WRITE_PATH") {
	char partialPathBuffer[PLATFORM_MAX_PATH];
	
	int currentSplit = 0;
	while (currentSplit < strlen(path)) {
		int subSplit = FindCharInString(path[currentSplit], '/', false);
		
		if (subSplit == -1) {
			// subsplit the remaining portion of the string
			subSplit = strlen(path[currentSplit]);
		}
		
		currentSplit += subSplit + 1; // subsplit + forward slash
		strcopy(partialPathBuffer, currentSplit + 1, path); // currentsplit + null
		
		if (!DirExists(partialPathBuffer, use_valve_fs, valve_path_id)
				&& !CreateDirectory(partialPathBuffer, mode, use_valve_fs, valve_path_id)) {
			return false;
		}
	}
	return true;
}

