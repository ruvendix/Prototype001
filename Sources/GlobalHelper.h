#pragma once

namespace global
{
	void SaveStringToFile(const std::string& str, FILE* pFile);
	std::string LoadStringFromFile(FILE* pFile);
}

namespace global
{
	template <typename TInt>
	TInt ClampCycle(TInt value, TInt min, TInt max)
	{
		if (value < min)
		{
			value = max;
		}
		else if (value > max)
		{
			value = min;
		}

		return value;
	}

	template <typename TInt>
	TInt ClampCycleForIndex(TInt value, TInt max)
	{
		value %= max;
		return value;
	}
}