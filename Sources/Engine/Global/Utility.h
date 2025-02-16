#pragma once

namespace global
{
	bool ValidateIntegerRange(int32 value, int32 min, int32 max);
	bool ValidateIndexRange(int32 value, int32 size);
	
	int32 CirculateIndex(int32 idx, int32 idxCount);

	template <typename TValue>
	TValue Clamp(TValue value, TValue minValue, TValue maxValue)
	{
		TValue resultValue = value;

		if (value < minValue)
		{
			resultValue = minValue;
		}

		if (value > maxValue)
		{
			resultValue = maxValue;
		}

		return resultValue;
	}

	template <typename TEnum>
	bool ValidateIndexRangeByEnum(TEnum value, TEnum size)
	{
		return ValidateIndexRange(TO_NUM(value), TO_NUM(size));
	}
}