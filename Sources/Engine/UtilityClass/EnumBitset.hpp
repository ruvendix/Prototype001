#pragma once

#include <Engine/Global/DataTypes.h>

// std::bitset �����ϴ� Ŭ����
template<typename TEnum, int32 count>
class EnumBitset
{
    static_assert(std::is_enum_v<TEnum>, "EnumType must be an enum.");

public:
    template <typename TEnum>
    void BitsOnImpl(const std::initializer_list<TEnum>& enumValues)
    {
        for (TEnum enumValue : enumValues)
        {
            m_bitset.set(TO_NUM(enumValue));
        }
    }

    template <typename... TArgs>
    void BitsOn(TArgs&&... args)
    {
        BitsOnImpl({ std::forward<TArgs>(args)... });
    }

    template <typename TEnum>
    void BitsOffImpl(const std::initializer_list<TEnum>& enumValues)
    {
        for (TEnum enumValue : enumValues)
        {
            m_bitset.reset(TO_NUM(enumValue));
        }
    }

    template <typename... TArgs>
    void BitsOff(TArgs&&... args)
    {
        BitsOffImpl({ std::forward<TArgs>(args)... });
    }

    template <typename TEnum>
    bool IsBitsOnImpl(const std::initializer_list<TEnum>& enumValues) const
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

    template <typename... TArgs>
    bool IsBitsOn(TArgs&&... args) const
    {
        return IsBitsOnImpl({ std::forward<TArgs>(args)... });
    }

public:
    void BitOn(TEnum enumValue)
    {
        m_bitset.set(TO_NUM(enumValue));
    }

    void AllBitsOn()
    {
        m_bitset.set();
    }

    void BitOff(TEnum enumValue)
    {
        m_bitset.reset(TO_NUM(enumValue));
    }

    void AllBitsOff()
    {
        m_bitset.reset();
    }

    bool IsBitOn(TEnum enumValue) const
    {
        return (m_bitset.test(TO_NUM(enumValue)));
    }

private:
    std::bitset<count> m_bitset;
};