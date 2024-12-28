#include "Pch.h"
#include "Utility.h"

namespace global
{
	bool ValidateIntegerRange(int32 value, int32 min, int32 max)
	{
		if ((value < min) ||
			(value > max))
		{
			DETAIL_ERROR_LOG(EErrorCode::NotFoundResource);
			return false;
		}

		return true;
	}

	bool ValidateIndexRange(int32 value, int32 size)
	{
		return ValidateIntegerRange(value, 0, size - 1);
	}

	int32 CirculateIndex(int32 idx, int32 idxCount)
	{
		++idx;
		return (idx % idxCount);
	}
}