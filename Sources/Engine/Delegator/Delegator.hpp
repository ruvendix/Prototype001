// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

template <typename T>
class DelegatorBase;

template <typename TReturn, typename... TArgs>
class DelegatorBase<TReturn(TArgs...)>
{
public:
    virtual TReturn CallFunctions(TArgs&&... args) const = 0;
    virtual TReturn CallFixedArgumentFunctions() const = 0;

public:
    template <typename TClass>
    void ConnectFunction(TClass* pObj, TReturn(TClass::* memFunc)(TArgs...))
    {
        const auto& argsIdxSequence = std::make_index_sequence<sizeof...(TArgs)>();
        ConnectFunctionImpl(pObj, memFunc, argsIdxSequence);
    }

    template <typename TClass>
    void ConnectFixedArgumentsFunction(TClass* pObj, TReturn(TClass::* memFunc)(TArgs...), TArgs&&... args)
    {
        const auto& boundFunc = std::bind(memFunc, pObj, std::forward<TArgs>(args)...);
        m_vecBoundFixedArgsFunc.push_back(boundFunc);
    }

public:
    bool IsBoundAnyFunction() const { return (m_vecBoundFunc.empty() == false); }
    bool IsBoundAnyFixedArgumentsFunction() const { return (m_vecBoundFixedArgsFunc.empty() == false); }

    void ConnectStaticFunction(const std::function<TReturn(TArgs...)>& staticFunc)
    {
        const auto& argsIdxSequence = std::make_index_sequence<sizeof...(TArgs)>();
        ConnectStaticFunctionImpl(staticFunc, argsIdxSequence);
    }

    void ConnectFixedArgumentsStaticFunction(const std::function<TReturn(TArgs...)>& staticFunc, TArgs&&... args)
    {
        const auto& boundFunc = std::bind(staticFunc, std::forward<TArgs>(args)...);
        m_vecBoundFixedArgsFunc.push_back(boundFunc);
    }

    void AllDisconnectFunction()
    {
        m_vecBoundFunc.clear();
        m_vecBoundFixedArgsFunc.clear();
    }

private:
    template <typename TClass, std::size_t... Indices>
    void ConnectFunctionImpl(TClass* pObj, TReturn(TClass::* memFunc)(TArgs...), const std::index_sequence<Indices...>&)
    {
        const auto& boundFunc = std::bind(memFunc, pObj, MakePlaceholder<Indices>()...);
        m_vecBoundFunc.push_back(boundFunc);
    }

    template <std::size_t... Indices>
    void ConnectStaticFunctionImpl(std::function<TReturn(TArgs...)> staticFunc, const std::index_sequence<Indices...>&)
    {
        const auto& boundFunc = std::bind(staticFunc, MakePlaceholder<Indices>()...);
        m_vecBoundFunc.push_back(boundFunc);
    }

    template <std::size_t idx>
    constexpr auto MakePlaceholder() const
    {
        constexpr auto placeholders = std::make_tuple(
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5,
            std::placeholders::_6, std::placeholders::_7, std::placeholders::_8, std::placeholders::_9, std::placeholders::_10,
            std::placeholders::_11, std::placeholders::_12, std::placeholders::_13, std::placeholders::_14, std::placeholders::_15,
            std::placeholders::_16, std::placeholders::_17, std::placeholders::_18, std::placeholders::_19, std::placeholders::_20);

        return std::get<idx>(placeholders);
    }

protected:
    std::vector<std::function<TReturn()>> m_vecBoundFixedArgsFunc;
    std::vector<std::function<TReturn(TArgs...)>> m_vecBoundFunc;
};

template <typename T>
class Delegator;

template <typename... TArgs>
class Delegator<void(TArgs...)> : public DelegatorBase<void(TArgs...)>
{
public:
    virtual void CallFunctions(TArgs&&... args) const override
    {
        for (const auto& boundFunc : this->m_vecBoundFunc)
        {
            boundFunc(std::forward<TArgs&&>(args)...);
        }
    }

    virtual void CallFixedArgumentFunctions() const override
    {
        for (const auto& boundFixedArgsFunction : this->m_vecBoundFixedArgsFunc)
        {
            boundFixedArgsFunction();
        }
    }
};

template <typename TReturn, typename... TArgs>
class Delegator<TReturn(TArgs...)> : public DelegatorBase<TReturn(TArgs...)>
{
public:
    virtual TReturn CallFunctions(TArgs&&... args) const override
    {
        TReturn retValue = TReturn();
        for (const auto& boundFunc : this->m_vecBoundFunc)
        {
            retValue = boundFunc(std::forward<TArgs&&>(args)...);
        }

        return retValue;
    }

    virtual TReturn CallFixedArgumentFunctions() const override
    {
        TReturn retValue = TReturn();
        for (const auto& boundFixedArgsFunction : this->m_vecBoundFixedArgsFunc)
        {
            retValue = boundFixedArgsFunction();
        }

        return retValue;
    }
};

using DefaultDelegator = Delegator<void()>;