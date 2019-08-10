#if defined __stocksoup_traceutils_included
	#endinput
#endif

#define __stocksoup_traceutils_included

#include <sdktools>

/**
 * Attempts to find a nearby position where the trace hull doesn't intersect anything.
 * At the worst case, this function performs 9 hull traces.
 * 
 * @param vecPosition		Desired position
 * @param vecMins			A vector containing the smaller coordinates for a bounding box
 * @param vecMaxs			A vector containing the larger coordinates for a bouding box
 * @param vecDestination	A valid destination, if any
 * @param flags				Trace flags.
 * @param filter			Function to use as a filter.
 * @param data				Arbitrary data value to pass through to the filter function.
 * @param searchScale		The scale that is used on the bounding box when searching.
 * 
 * @return true if a valid position is found
 */
stock bool FindNonCollideHullPosition(const float vecPosition[3],
		const float vecMins[3], const float vecMaxs[3], float vecDestination[3],
		int flags, TraceEntityFilter filter, any data, float searchScale = 1.0) {
	
	Handle trace = TR_TraceHullFilterEx(vecPosition, vecPosition, vecMins, vecMaxs,
			flags, filter, data);
	
	bool valid = !TR_DidHit(trace);
	delete trace;
	
	if (valid) {
		vecDestination = vecPosition;
		return true;
	}
	
	return FindNonCollideHullPositionExtent(vecPosition, vecMins, vecMaxs, vecDestination,
			flags, filter, data, searchScale);
}

enum EHullExtent {
	HullExtent_Center,
	HullExtent_Minimum,
	HullExtent_Maximum
};

/**
 * Checks the perimeter bounded by vecMins and vecMaxs for a position that doesn't hit anything.
 * At the worst case, this function performs 8 hull traces.
 */
stock bool FindNonCollideHullPositionExtent(const float vecPosition[3],
		const float vecMins[3], const float vecMaxs[3], float vecDestination[3],
		int flags, TraceEntityFilter filter, any data, float searchScale = 1.0) {
	float vecCenter[3];
	for (int i; i < sizeof(vecCenter); i++) {
		vecCenter[i] = (vecMaxs[i] + vecMins[i]) / 2;
	}
	
	float vecScaledMins[3], vecScaledMaxs[3];
	vecScaledMins = vecMins;
	vecScaledMaxs = vecMaxs;
	
	ScaleVector(vecScaledMins, searchScale);
	ScaleVector(vecScaledMaxs, searchScale);
	
	/** 
	 * Basically we treat the corners and center edges of the bounding box as potential
	 * unstuck position candidates.
	 */
	
	for (EHullExtent x = HullExtent_Center; x <= HullExtent_Maximum; x++) {
		for (EHullExtent y = HullExtent_Center; y <= HullExtent_Maximum; y++) {
			// we assume the center has already been checked
			if (x == HullExtent_Center && y == HullExtent_Center) {
				continue;
			}
			
			float vecOffset[] = { 0.0, 0.0, 10.0 };
			
			switch (x) {
				case HullExtent_Minimum: { vecOffset[0] = vecScaledMins[0]; }
				case HullExtent_Maximum: { vecOffset[0] = vecScaledMaxs[0]; }
				case HullExtent_Center:  { vecOffset[0] = vecCenter[0]; }
			}
			
			switch (y) {
				case HullExtent_Minimum: { vecOffset[1] = vecScaledMins[1]; }
				case HullExtent_Maximum: { vecOffset[1] = vecScaledMaxs[1]; }
				case HullExtent_Center:  { vecOffset[0] = vecCenter[0]; }
			}
			
			float vecTestPosition[3];
			AddVectors(vecPosition, vecOffset, vecTestPosition);
			
			Handle trace = TR_TraceHullFilterEx(vecTestPosition, vecTestPosition,
					vecMins, vecMaxs, flags, filter, data);
			
			bool valid = !TR_DidHit(trace);
			
			delete trace;
			
			if (valid) {
				vecDestination = vecTestPosition;
				return true;
			}
		}
	}
	return false;
}
