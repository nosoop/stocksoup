/**
 * A bunch of functions to work with strings.  Or character arrays, rather.
 */

#if defined __stocksoup_string_included
	#endinput
#endif

#define __stocksoup_string_included

/**
 * Returns whether or not the string terminates at the first character (e.g., zero length, no
 * text).
 */
stock bool StrEmpty(const char[] str) {
	// Courtesy of Root_ for the creative implementation.
	return !str[0];
}

/**
 * Breaks a string into pieces and pushes each piece into an ArrayList.
 * 
 * @param text				The string to split.
 * @param split				The string to use as a split delimiter.
 * @param maxlen			Maximum length of each string.
 * @param copyRemainder		False (default) discards excess pieces.
 * 
 * @return An ArrayList containing strings
 */
stock ArrayList ExplodeStringToList(const char[] text, const char[] split, int maxlen,
		bool copyRemainder = false) {
	int reloc_idx, idx;
	
	ArrayList list = new ArrayList(maxlen);
	
	if (split[0]) {
		char buffer[64];
		while ((idx = SplitString(text[reloc_idx], split, buffer, sizeof(buffer))) != -1) {
			reloc_idx += idx;
			list.PushString(buffer);
		}
	}
	
	if (copyRemainder) {
		list.PushString(text[reloc_idx]);
	}
	return list;
}

/**
 * A naive implementation where, in a buffer, replacements are performed using StringMap keys as
 * substring matches and replaced with their values.
 * 
 * Replacements are performed in string order (e.g., if "ace" and "eba" are substrings and the
 * string to be processed is "aceba ebace", it will end up processing "{ace}ba {eba}ce").
 * 
 * If multiple matches are found at a specific index, the longest substring has priority (e.g.,
 * if "arse" and "arsenic" are possible matches for string "arsenic arse", "{arsenic} {arse}"
 * are matched).
 * 
 * Performance is probably abysmal.
 */
stock void ReplaceMultiple(char[] buffer, int maxlen, StringMap replacements) {
	int nMaxReplacements = strlen(buffer);
	int[] replacementIndex = new int[nMaxReplacements];
	
	for (int i = 0; i < nMaxReplacements; i++) {
		replacementIndex[i] = -1;
	}
	
	StringMapSnapshot substrings = replacements.Snapshot();
	
	// find replacement strings
	// if replacement string is found, save a reference to its snapshot index
	for (int i = 0; i < substrings.Length; i++) {
		int substringLength = substrings.KeyBufferSize(i);
		char[] substring = new char[substringLength];
		substrings.GetKey(i, substring, substringLength);
		
		int last, next;
		do {
			next = StrContains(buffer[last], substring);
			
			if (next >= 0) {
				int existingMatch = replacementIndex[last + next];
				if (existingMatch == -1
						|| substrings.KeyBufferSize(existingMatch) < substringLength) {
					replacementIndex[last + next] = i;
				}
				last += next + substringLength;
			}
		} while (next != -1);
	}
	
	char[] source = new char[maxlen];
	strcopy(source, maxlen, buffer);
	
	// zero out buffer and iterate over replacementIndex
	// if replacementIndex is -1, copy corresponding character from source
	// else copy value from stringmap
	strcopy(buffer, maxlen, "");
	for (int i = 0; i < strlen(source); i++) {
		// if you want to do any sort of conflict resolution, you could do it here, somewhere.
		// some sort of lookahead would probably work well
		int index = replacementIndex[i];
		if (index == -1) {
			char next[1];
			next[0] = source[i];
			StrCat(buffer, maxlen, next);
		} else {
			int substringLength = substrings.KeyBufferSize(index);
			char[] substring = new char[substringLength];
			substrings.GetKey(index, substring, substringLength);
			
			// TODO allocate required amount
			char replacement[64];
			replacements.GetString(substring, replacement, sizeof(replacement));
			
			StrCat(buffer, maxlen, replacement);
			
			// skip matched substring, off-by-one since for loop will increment
			i += strlen(substring) - 1;
		}
	}
	delete substrings;
}
