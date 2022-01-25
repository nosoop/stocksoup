/**
 * Utility stocks for files.
 */
#if defined __stocksoup_file_included
	#endinput
#endif

#define __stocksoup_file_included

#include <stocksoup/string>

/**
 * Copies the contents of the file located at `source` to the path specified by `dest`.  No
 * other information (e.g. metadata) is copied.
 * 
 * @param source       Path to source file.
 * @param dest         Path to destination.  Any file present is overwritten.
 * @error              Source / destination file(s) could not be opened for reading / writing.
 */
stock void CopyFile(const char[] source, const char[] dest) {
	if (!FileExists(source)) {
		ThrowError("Source file \"%s\" does not exist", source);
	}
	
	File srcFile = OpenFile(source, "rb");
	File dstFile = OpenFile(dest, "wb");
	
	if (!srcFile) {
		ThrowError("Failed to open source file \"%s\"", source);
	} else if (!dstFile) {
		ThrowError("Failed to open destination file \"%s\"", dest);
	}
	
	int buffer[4096];
	int numRead;
	do {
		numRead = srcFile.Read(buffer, sizeof(buffer), 1);
		dstFile.Write(buffer, numRead, 1);
	} while (numRead > 0);
	
	delete srcFile;
	delete dstFile;
}

/**
 * Creates a directory, creating parent directories if they don't exist.
 * 
 * @param path			Path to create.  Parent directories that do not exist are also created.
 * 						On POSIX systems, a backslash will be treated as part of the folder name
 * 						(this may not work -- for simplicity's sake stick to '/' no matter the
 * 						platform).
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
	char normalizedPath[PLATFORM_MAX_PATH];
	strcopy(normalizedPath, sizeof(normalizedPath), path);
	NormalizePathToPOSIX(normalizedPath);
	
	char partialPathBuffer[PLATFORM_MAX_PATH];
	
	int p = -1;
	while ( (p = FindNextCharInString(p, normalizedPath, '/')) != -1 ) {
		// exclude trailing forward slash
		strcopy(partialPathBuffer, p, normalizedPath);
		
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
 * subdirectories.  Each entry contains the input path as the prefix, and directory separators
 * will use the system's native form ('\' on Windows, '/' on POSIX).
 * 
 * This performs a breadth-first iteration.
 * 
 * @param path           Starting path to search files.
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
			FormatEx(fullpath, sizeof(fullpath), "%s%c%s",
					currentPath, DumbHackTo_GetPathComponentSeparator(), currentEntry);
			
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

/**
 * Normalizes the input path to use POSIX path component separators only on non-POSIX systems.
 */
stock void NormalizePathToPOSIX(char[] path) {
	char sep = DumbHackTo_GetPathComponentSeparator();
	if (sep == '/') {
		return;
	}
	
	for (int i; path[i]; i++) {
		if (path[i] == sep) {
			path[i] = '/';
		}
	}
}

/**
 * Returns the path component separator.  This is '/' for POSIX and '\\' for Windows.
 */
static stock char DumbHackTo_GetPathComponentSeparator() {
	static char result;
	
	if (!result) {
		char dir[PLATFORM_MAX_PATH];
		BuildPath(Path_SM, dir, sizeof(dir), "%s", "/");
		
		result = dir[strlen(dir) - 1];
	}
	return result;
}
