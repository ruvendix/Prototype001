#pragma once

namespace math
{
	// �⺻�� �ε��Ҽ����� (���ø� ���ڿ� 0�� ������ �ش� �Լ��� ȣ���� �� �� ���ڴ� ���� ����)
	template <typename TValue, typename std::enable_if_t<std::is_floating_point_v<TValue>, int> = 0>
	bool CheckAlikeValue(TValue lhs, TValue rhs)
	{
		return (std::abs(lhs - rhs) < std::numeric_limits<TValue>::epsilon());
	}

	// Ư��ȭ�� ������
	template <typename TValue, typename std::enable_if_t<std::is_integral_v<TValue>, int> = 0>
	bool CheckAlikeValue(TValue lhs, TValue rhs)
	{
		return (lhs == rhs);
	}
}