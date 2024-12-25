#pragma once

#include <Engine/Global/DataTypes.h>

// std::bitset 래핑하는 클래스
template<typename TEnum, int32 count>
class EnumBitset
{
    static_assert(std::is_enum_v<TEnum>, "EnumType must be an enum.");

public:
    void BitOn(TEnum enumValue)
    {
        m_bitset.set(TO_NUM(enumValue));
    }

    void BitsOn(const std::initializer_list<TEnum>& enumValues)
    {
        for (TEnum enumValue : enumValues)
        {
            m_bitset.set(TO_NUM(enumValue));
        }
    }

    void AllBitsOn()
    {
        m_bitset.set();
    }

    void BitOff(TEnum enumValue)
    {
        m_bitset.reset(TO_NUM(enumValue));
    }

    void BitsOff(const std::initializer_list<TEnum>& enumValues)
    {
        for (TEnum enumValue : enumValues)
        {
            m_bitset.reset(TO_NUM(enumValue));
        }
    }

    void AllBitsOff()
    {
        m_bitset.reset();
    }

    bool IsBitOn(TEnum enumValue) const
    {
        return (m_bitset.test(TO_NUM(enumValue)));
    }

    bool IsBitsOn(const std::initializer_list<TEnum>& enumValues) const
    {
        for (TEnum enumValue : enumValues)
        {
            if (m_bitset.test(TO_NUM(enumValue)) == false)
            {
                return false;
            }
        }

        return true;
    }

private:
    std::bitset<count> m_bitset;
};