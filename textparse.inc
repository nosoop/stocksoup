#if defined __stocksoup_textparse_included
	#endinput
#endif

#define __stocksoup_textparse_included

/**
 * Translates a 16-bit UCS2 code unit to its UTF8 variable-width equivalent.
 * Lifted from DoctorMcKay/enhanced_items.sp
 * 
 * @param data      A 16-bit value representing the character.
 * @param buffer    Buffer to store the result.  The result is up to four bytes comprising a
 *                  single character, then a null terminator (for convenience in performing
 *                  string operations).
 * @return          Number of bytes in the character (excluding null terminator).  If reading
 *                  failed, 0 is returned.
 * @error           Received a value in the surrogate pair range.
 */
stock int DecodeUTF16LEChar(int data, char buffer[5]) {
	int n;
	if (data < 0x80) {
		buffer[n++] = data;
	} else if (data < 0x800) {
		buffer[n++] = ((data >> 6) & 0x1F) | 0xC0;
		buffer[n++] = (data & 0x3F) | 0x80;
	} else if (0xD800 <= data <= 0xDFFF) {
		// refuse to decode a surrogate pair here
		ThrowError("UTF-16 character %04x is in the surrogate pair range; "
				... "get the next 16-bit value and use DecodeUTF16LESurrogatePair", data);
	} else if (data <= 0xFFFF) {
		buffer[n++] = ((data >> 12) & 0x0F) | 0xE0;
		buffer[n++] = ((data >> 6) & 0x3F) | 0x80;
		buffer[n++] = (data & 0x3F) | 0x80;
	}
	buffer[n] = '\0';
	return n;
}

/**
 * Translates a 2x16-bit UCS2 surrogate pair to its UTF8 variable-width equivalent.
 * Lifted from https://github.com/DoctorMcKay/sourcemod-plugins/pull/1
 * 
 * @param highdata    A 16-bit value representing the high surrogate pair.
 * @param lowdata     A 16-bit value representing the low surrogate pair.
 * @param buffer      Buffer to store the result.  The result is up to four bytes comprising a
 *                    single character, then a null terminator (for convenience in performing
 *                    string operations).
 * @return            Number of bytes in the character (excluding null terminator).  If reading
 *                    failed, 0 is returned.
 */
stock int DecodeUTF16LESurrogatePair(int highdata, int lowdata, char buffer[5]) {
	int n;
	
	int data = ((highdata - 0xD800) << 10) + (lowdata - 0xDC00) + 0x10000;
	buffer[n++] = ((data >> 18) & 0x07) | 0xF0;
	buffer[n++] = ((data >> 12) & 0x3F) | 0x80;
	buffer[n++] = ((data >> 6) & 0x3F) | 0x80;
	buffer[n++] = (data & 0x3F) | 0x80;
	
	return n;
}

/**
 * Reads a UTF8 string from a UTF16LE-encoded file.
 * 
 * @param file        Handle to the file.
 * @param buffer      Buffer to store the string.
 * @param max_size    Maximum size of the string buffer.
 * @param cu_count    Maximum number of code units to read.  Each two byte code unit may produce
 *                    up to four bytes in UTF8, so if you want the maximum, the buffer should be
 *                    no less than (4 * cu_count + 1).
 * @return            Number of bytes written.
 */
stock int ReadFileUTF16LEString(File file, char[] buffer, int max_size, int cu_count) {
	int n;
	for (int i; i < cu_count; i++) {
		char charbuf[5];
		int cs = ReadFileUTF16LEChar(file, charbuf);
		
		if (cs + n >= max_size) {
			// do not copy over the codepoint if it would overflow past the buffer
			// we do not allow partial codepoints
			break;
		}
		n += strcopy(buffer[n], max_size - n, charbuf);
	}
	return n;
}

/**
 * Decodes a UTF16LE entry at the current file position.  The file is assumed to be well-formed.
 * 
 * @param file      Handle to the file.
 * @param buffer    Buffer to store the result.  The result is up to four bytes comprising a
 *                  single character, then a null terminator.
 * @return          Number of bytes in the character (excluding null terminator).  If reading
 *                  failed, 0 is returned.
 */
stock int ReadFileUTF16LEChar(File file, char buffer[5]) {
	int data;
	if (!file.ReadUint16(data)) {
		return 0;
	}
	
	// process high surrogate range here
	if (0xD800 <= data <= 0xDFFF) {
		int lowdata;
		if (!file.ReadUint16(lowdata)) {
			// previous char was a high surrogate marker but we EOF'd
			return 0;
		}
		return DecodeUTF16LESurrogatePair(data, lowdata, buffer);
	}
	
	return DecodeUTF16LEChar(data, buffer);
}

/**
 * Possible result codes for TranslationFileParser.ParseOpenUTF16File().
 */
enum TranslationFileParseResult {
	TranslationFileParse_OK,
	TranslationFileParse_BadBOM,
	TranslationFileParse_StartSectionWithoutKey,
	TranslationFileParse_EndSectionWithUnpairedKey,
	TranslationFileParse_UnknownToken,
	TranslationFileParse_BadEscapeSequence,
	TranslationFileParse_UnexpectedEOF,
}

/**
 * Called when parsing is started.
 */
typedef TranslationFile_ParseStart = function void();

/**
 * Called when the parser is entering a new section or sub-section.
 * 
 * @param name    String containing section name.
 */
typedef TranslationFile_NewSection = function void(const char[] name);

/**
 * Called when the parser finds a new key/value pair.
 * Note: Enclosing quotes are always stripped.
 * 
 * @param key      String containing key name.
 * @param value    String containing value name.
 */
typedef TranslationFile_KeyValue = function void(const char[] key, const char[] value);

/**
 * Called when the parser finds the end of the current section.
 */
typedef TranslationFile_EndSection = function void();

/**
 * Called when parsing a translation file has ended.
 */
typedef TranslationFile_ParseEnd = function void();

/**
 * A callback-driven parser for Valve translation files.
 */
enum struct TranslationFileParser {
	TranslationFile_ParseStart OnStart;
	TranslationFile_ParseEnd OnEnd;
	TranslationFile_NewSection OnEnterSection;
	TranslationFile_EndSection OnLeaveSection;
	TranslationFile_KeyValue OnKeyValue;
	
	/**
	 * Initializes the TranslationFileParser.  Callbacks do not get zero init'd to
	 * INVALID_FUNCTION, so this must be called to ensure that users do not attempt to call
	 * invalid function pointers.
	 * 
	 * After calling this, users are expected to set the callbacks as needed for their use case.
	 */
	void Init() {
		this.OnStart = INVALID_FUNCTION;
		this.OnEnd = INVALID_FUNCTION;
		this.OnEnterSection = INVALID_FUNCTION;
		this.OnLeaveSection = INVALID_FUNCTION;
		this.OnKeyValue = INVALID_FUNCTION;
	}
	
	/**
	 * Parses a translation file, specified in UTF16 (UCS2?) format.
	 * 
	 * @param file    An open file handle.  The file should be in binary read mode.
	 * @return        A TranslationFileParseResult value.
	 */
	TranslationFileParseResult ParseOpenUTF16File(File f) {
		// extract the byte order mark
		int bom;
		f.ReadUint16(bom);
		if (bom != 0xFEFF) {
			LogStackTrace("Attempting to read a non UTF16LE file (position %d)", f.Position);
			return TranslationFileParse_BadBOM;
		}
		
		/**
		 * This is the maximum length we allow for each string.  Keys / values in the file will
		 * be truncated if they are larger than this, but parsing should still be correct.
		 */
		char keybuf[512], valbuf[512];
		
		// key buffer may be a zero-length string so we store its state here
		bool bReadKey;
		
		if (this.OnStart != INVALID_FUNCTION) {
			Call_StartFunction(null, this.OnStart);
			Call_Finish();
		}
		
		char charbuf[5];
		for (int n; (n = ReadFileUTF16LEChar(f, charbuf));) {
			if (n > 1) {
				LogStackTrace("Unexpected character '%s' (position %d)", charbuf, f.Position);
				return TranslationFileParse_UnknownToken;
			}
			
			switch (charbuf[0]) {
				case '\n', '\r', ' ', '\t': {
					// ignore whitespace
					continue;
				}
				case '{': {
					// emit section start
					if (!bReadKey) {
						LogStackTrace("Unexpected start of section without section name "
								... "(position %d)", f.Position);
						return TranslationFileParse_StartSectionWithoutKey;
					}
					
					if (this.OnEnterSection != INVALID_FUNCTION) {
						Call_StartFunction(null, this.OnEnterSection);
						Call_PushString(keybuf);
						Call_Finish();
					}
					
					bReadKey = false;
				}
				case '}': {
					// emit section end
					if (bReadKey) {
						LogStackTrace("Unexpected end of section; expected value for "
							... "key / value pair (position %d)", f.Position);
						return TranslationFileParse_EndSectionWithUnpairedKey;
					} else if (this.OnLeaveSection != INVALID_FUNCTION) {
						Call_StartFunction(null, this.OnLeaveSection);
						Call_Finish();
					}
				}
				case '"': {
					// start of quoted string
					if (!bReadKey) {
						// key buffer isn't populated, so consume the quoted string
						TranslationFileParseResult result = ReadLanguageFileQuotedString(
								f, keybuf, sizeof(keybuf));
						if (result != TranslationFileParse_OK) {
							return result;
						}
						bReadKey = true;
					} else {
						// consuming the value portion of the pair; emit pair to callback
						TranslationFileParseResult result = ReadLanguageFileQuotedString(
								f, valbuf, sizeof(valbuf));
						if (result != TranslationFileParse_OK) {
							return result;
						}
						
						if (this.OnKeyValue != INVALID_FUNCTION) {
							Call_StartFunction(null, this.OnKeyValue);
							Call_PushString(keybuf);
							Call_PushString(valbuf);
							Call_Finish();
						}
						
						bReadKey = false;
					}
				}
				case '/': {
					// consume next character and check if it's a comment
					n = ReadFileUTF16LEChar(f, charbuf);
					
					if (n == 1 && charbuf[0] == '/') {
						// comment - read until newline
						while (ReadFileUTF16LEChar(f, charbuf)) {
							if (charbuf[0] == '\n') {
								break;
							}
						}
					} else if (!n) {
						LogStackTrace("Reached end of file while scanning for comment start "
								... "(position %d)", f.Position);
						return TranslationFileParse_UnexpectedEOF;
					} else {
						LogStackTrace(
								"Unexpected character '%s' while scanning for comment start "
								... "(position %d)", charbuf, f.Position);
						return TranslationFileParse_UnknownToken;
					}
				}
				default: {
					LogStackTrace("Unexpected character '%s' (position %d)", charbuf,
							f.Position);
					return TranslationFileParse_UnknownToken;
				}
			}
		}
		
		if (this.OnEnd != INVALID_FUNCTION) {
			Call_StartFunction(null, this.OnEnd);
			Call_Finish();
		}
		return TranslationFileParse_OK;
	}
	
	/**
	 * Gets an error string for a TranslationFileParseResult value.
	 * 
	 * @param result     The TranslationFileParseResult value.
	 * @param buffer     A string buffer for the error.
	 * @param buf_max    The maximum size of the buffer.
	 * @return           True if the result was an error value.
	 */
	bool GetErrorString(TranslationFileParseResult result, char[] buffer, int buf_max) {
		switch (result) {
			case TranslationFileParse_OK: {
				return false;
			}
			case TranslationFileParse_BadBOM: {
				strcopy(buffer, buf_max, "Failed to assert BOM at start of file");
			}
			case TranslationFileParse_StartSectionWithoutKey: {
				strcopy(buffer, buf_max,
						"Reached start of section without a header present");
			}
			case TranslationFileParse_EndSectionWithUnpairedKey: {
				strcopy(buffer, buf_max,
						"Unexpected end of section; expected value for key / value pair");
			}
			case TranslationFileParse_UnexpectedEOF: {
				strcopy(buffer, buf_max, "Unexpected end of file");
			}
			case TranslationFileParse_UnknownToken: {
				strcopy(buffer, buf_max, "Reached unknown token");
			}
			case TranslationFileParse_BadEscapeSequence: {
				strcopy(buffer, buf_max, "Bad escape sequence in quoted string");
			}
			default: {
				strcopy(buffer, buf_max, "Unspecified error");
			}
		}
		return true;
	}
}

/**
 * Reads a language file quoted string.  The first character is expected to be part of the
 * output string, provided it isn't the closing quote to end the string.
 */
static TranslationFileParseResult ReadLanguageFileQuotedString(File file, char[] buffer,
		int maxlen) {
	char charbuf[5];
	for (int c, n; (n = ReadFileUTF16LEChar(file, charbuf)); ) {
		if (n > 1) {
			// copy multi-byte character to the output buffer
			c += strcopy(buffer[c], maxlen - c, charbuf);
			continue;
		}
		
		// this is a single character; act on it
		switch (charbuf[0]) {
			case '\\': {
				// consume next character; escape newlines and double quotes
				n = ReadFileUTF16LEChar(file, charbuf);
				
				if (!n) {
					LogStackTrace("Reached end of file while reading escape sequence");
					return TranslationFileParse_UnexpectedEOF;
				} else if (n > 1) {
					LogStackTrace("Bad escape sequence character '%s' (position %d)", charbuf,
							file.Position);
					return TranslationFileParse_BadEscapeSequence;
				}
				
				switch (charbuf[0]) {
					case 'n': {
						c += strcopy(buffer[c], maxlen - c, "\n");
					}
					case '"': {
						c += strcopy(buffer[c], maxlen - c, "\"");
					}
					case '\\': {
						c += strcopy(buffer[c], maxlen - c, "\\");
					}
					case 'x': {
						// ZI (TF2 Halloween, 2023) added hex escaped strings
						// decode the two-char byte value immediately following the x
						char hexsequence[3];
						ReadFileUTF16LEString(file, hexsequence, sizeof(hexsequence), 2);
						
						char seq[4];
						Format(seq, sizeof(seq), "%c", StringToInt(hexsequence, 0xF) & 0xFF);
						c += strcopy(buffer[c], maxlen - c, seq);
					}
					default: {
						LogStackTrace("Bad escape sequence character '%s' (position %d)",
								charbuf, file.Position);
						return TranslationFileParse_BadEscapeSequence;
					}
				}
			}
			case '"': {
				// reached end of the string; return
				return TranslationFileParse_OK;
			}
			default: {
				c += strcopy(buffer[c], maxlen - c, charbuf);
			}
		}
	}
	
	// we broke out of the loop due to a zero-read; that is bad
	LogStackTrace("Reached end of file while reading quoted string");
	return TranslationFileParse_UnexpectedEOF;
}
