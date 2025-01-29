#pragma once

namespace math
{
	// 기본은 부동소수점용 (템플릿 인자에 0을 넣으면 해당 함수를 호출할 때 그 인자는 생략 가능)
	template <typename TValue, typename std::enable_if_t<std::is_floating_point_v<TValue>, int> = 0>
	bool CheckAlikeValue(TValue lhs, TValue rhs)
	{
		return (std::abs(lhs - rhs) < std::numeric_limits<TValue>::epsilon());
	}

	// 특수화는 정수용
	template <typename TValue, typename std::enable_if_t<std::is_integral_v<TValue>, int> = 0>
	bool CheckAlikeValue(TValue lhs, TValue rhs)
	{
		return (lhs == rhs);
	}
}