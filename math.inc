/**
 * Sourcemod 1.7 Plugin Template
 */
#if defined __stocksoup_math_included
	#endinput
#endif

#define __stocksoup_math_included

stock float LerpFloat(float flPercent, float a, float b) {
	return A + ((B - A) * flPercent);
}

/**
 * Scales the difference between `initial` to `current` by a factor of `scale`.
 */
stock float ScaleDifference(float current, float initial, float scale) {
	return ((current - initial) * scale) + initial;
}
