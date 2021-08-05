/**
 * Utility stocks for files.
 */
#if defined __stocksoup_file_included
	#endinput
#endif

#define __stocksoup_file_included

#include <stocksoup/string>

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
	
	int p = -1;
	while ( (p = FindNextCharInString(p, path, '/')) != -1 ) {
		// exclude trailing forward slash
		strcopy(partialPathBuffer, p, path);
		
		if (!DirExists(partialPathBuffer, use_valve_fs, valve_path_id)
				&& !CreateDirectory(partialPathBuffer, mode, use_valve_fs, valve_path_id)) {
			return false;
		}
	}
	
	// try to create the last directory in the path
	return DirExists(path, use_valve_fs, valve_path_id) ||
			CreateDirectory(path, mode, use_valve_fs, valve_path_id);
}

/**
 * Returns an ArrayList containing the paths of all files in the given directory and its
 * subdirectories.  Each entry contains the input path as the prefix.
 * 
 * This performs a breadth-first iteration.
 * 
 * @param path           Path to open.
 * @param use_valve_fs   If true, the Valve file stem will be used instead.  This can be used to
 *                       find files in the game's Valve search paths, rather than solely files
 *                       existing in the gamedir.
 * @param valve_path_id  If use_valve_fs, a search path from gameinfo or NULL_STRING for all
 *                       search paths.
 */
stock ArrayList GetFilesInDirectoryRecursive(const char[] path, bool use_valve_fs = false,
		const char[] valve_path_id = "GAME") {
	ArrayList files = new ArrayList(ByteCountToCells(PLATFORM_MAX_PATH));
	
	ArrayList paths = new ArrayList(ByteCountToCells(PLATFORM_MAX_PATH));
	paths.PushString(path);
	
	/**
	 * Check paths.Length on every iteration since we store tracked paths, just in case we 
	 * somehow get caught in a loop.
	 */
	for (int i; i < paths.Length; i++) {
		char currentPath[PLATFORM_MAX_PATH], currentEntry[PLATFORM_MAX_PATH];
		FileType type;
		
		paths.GetString(i, currentPath, sizeof(currentPath));
		
		DirectoryListing listing = OpenDirectory(currentPath, use_valve_fs, valve_path_id);
		if (!listing) {
			continue;
		}
		
		while (listing.GetNext(currentEntry, sizeof(currentEntry), type)) {
			char fullpath[PLATFORM_MAX_PATH];
			
			// SourceMod doesn't do native path separators without BuildPath...
			// ... forward slashes it is.
			FormatEx(fullpath, sizeof(fullpath), "%s/%s", currentPath, currentEntry);
			
			switch (type) {
				case FileType_File: {
					files.PushString(fullpath);
				}
				case FileType_Directory: {
					if (StrEqual(currentEntry, ".") || StrEqual(currentEntry, "..")) {
						continue;
					}
					if (paths.FindString(fullpath) == -1) {
						paths.PushString(fullpath);
					}
				}
			}
		}
		delete listing;
	}
	delete paths;
	return files;
}
