/**
 * Some math-related functions.
 */
#if defined __stocksoup_math_included
	#endinput
#endif

#define __stocksoup_math_included

/**
 * Linearly interpolates float across (a, b).
 */
stock float LerpFloat(float flPercent, float a, float b) {
	return a + ((b - a) * flPercent);
}

/**
 * Scales the difference between `initial` to `current` by a factor of `scale`.
 * Completely missed the fact that LerpFloat does the same thing; this is now just present for
 * backwards compatibility.
 */
#pragma deprecated This is literally just LerpFloat with a different name and argument order
stock float ScaleDifference(float current, float initial, float scale) {
	return LerpFloat(scale, initial, current);
}

/**
 * Clamps the given value to between min / max.
 */
stock float ClampFloat(float value, float min, float max) {
	if (value > max) {
		return max;
	} else if (value < min) {
		return min;
	}
	return value;
}

/**
 * Returns a normalized angle between [-180.0, 180).
 */
stock float NormalizeAngle(float value) {
	// https://stackoverflow.com/a/43780476
	return value - 360.0 * RoundToFloor((value + 180.0) / 360.0);
}

/**
 * Converts a signed 8-bit value into a signed 32-bit value.
 * Any higher bits set will be ignored and replaced accordingly.
 */
stock int SignExtendInt8(int byte) {
	return (byte >> 7 & 1)? 0xFFFFFF00 | byte : 0x000000FF & byte;
}

/**
 * Converts a signed 16-bit value into a signed 32-bit value.
 * Any higher bits set will be ignored and replaced accordingly.
 */
stock int SignExtendInt16(int word) {
	return (word >> 15 & 1)? 0xFFFF0000 | word : 0x0000FFFF & word;
}
