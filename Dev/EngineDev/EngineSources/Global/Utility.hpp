#pragma once

namespace global
{
	__forceinline bool ValidateIntegerRange(int32 value, int32 min, int32 max)
	{
		if ((value < min) ||
			(value > max))
		{

			return false;
		}

		return true;
	}

	__forceinline bool ValidateIndexRange(int32 value, int32 size)
	{
		return ValidateIntegerRange(value, 0, size - 1);
	}

	__forceinline int32 CirculateIndex(int32 idx, int32 idxCount)
	{
		++idx;
		return (idx % idxCount);
	}

	template <typename TValue>
	__forceinline TValue Clamp(TValue value, TValue minValue, TValue maxValue)
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
	__forceinline bool ValidateIndexRangeByEnum(TEnum value, TEnum size)
	{
		return ValidateIndexRange(TO_NUM(value), TO_NUM(size));
	}
}