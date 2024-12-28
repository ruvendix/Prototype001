#pragma once

namespace global
{
	bool ValidateIntegerRange(int32 value, int32 min, int32 max);
	bool ValidateIndexRange(int32 value, int32 size);
	
	int32 CirculateIndex(int32 idx, int32 idxCount);
}