#pragma once

template <typename TEnum, typename TSize = uint32>
class EnumBitFlag
{
public:
	EnumBitFlag() = default;
	~EnumBitFlag() = default;

	std::underlying_type_t<TEnum> ToShiftValue(TEnum enumValue)
	{
		std::underlying_type_t<TEnum> value = static_cast<std::underlying_type_t<TEnum>>(enumValue);
		assert(value < sizeof(TSize) * CHAR_BIT);
		return (1 << value);
	}

	void AddBit(TEnum enumValue)
	{
		m_flag |= ToShiftValue(enumValue);
	}

	void RemoveBit(TEnum enumValue)
	{
		m_flag &= ~(ToShiftValue(enumValue));
	}

	bool TestBit(TEnum enumValue)
	{
		return ((m_flag & ToShiftValue(enumValue)) != 0);
	}

	bool TestFlag(const EnumBitFlag& enumBitFlag) const { return ((m_flag & enumBitFlag.GetFlag()) != 0); }

	TSize GetFlag() const { return m_flag; }
	void SetFlag(const EnumBitFlag& enumBitFlag) { m_flag = enumBitFlag.GetFlag(); }

private:
	TSize m_flag = 0;
};