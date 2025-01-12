// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

template <typename T>
class DelegatorBase;

template <typename TReturn, typename... TArgs>
class DelegatorBase<TReturn(TArgs...)>
{
public:
    virtual TReturn CallFunction(TArgs&&... args) const = 0;
    virtual TReturn CallFixedArgumentsFunction() const = 0;

    void ConnentStaticFunction(const std::function<TReturn(TArgs...)>& staticFunc)
    {
        const auto& argsIdxSequence = std::make_index_sequence<sizeof...(TArgs)>();
        ConnentStaticFunctionImpl(staticFunc, argsIdxSequence);
    }

    void ConnentFixedArgumentsStaticFunction(const std::function<TReturn(TArgs...)>& staticFunc, TArgs&&... args)
    {
        const auto& boundFunc = std::bind(staticFunc, std::forward<TArgs>(args)...);
        m_vecBoundFixedArgsFunction.push_back(boundFunc);
    }

    template <typename TClass>
    void ConnentFunction(TClass* pObj, TReturn(TClass::* memFunc)(TArgs...))
    {
        const auto& argsIdxSequence = std::make_index_sequence<sizeof...(TArgs)>();
        ConnentFunctionImpl(pObj, memFunc, argsIdxSequence);
    }

    template <typename TClass>
    void ConnentFixedArgumentsFunction(TClass* pObj, TReturn(TClass::* memFunc)(TArgs...), TArgs&&... args)
    {
        const auto& boundFunc = std::bind(memFunc, pObj, std::forward<TArgs>(args)...);
        m_vecBoundFixedArgsFunction.push_back(boundFunc);
    }

    void AllDisconnectFunction()
    {
        m_vecBoundFunc.clear();
        m_vecBoundFixedArgsFunction.clear();
    }

private:
    template <typename TClass, std::size_t... Indices>
    void ConnentFunctionImpl(TClass* pObj, TReturn(TClass::* memFunc)(TArgs...), const std::index_sequence<Indices...>&)
    {
        const auto& boundFunc = std::bind(memFunc, pObj, MakePlaceholder<Indices>()...);
        m_vecBoundFunc.push_back(boundFunc);
    }

    template <std::size_t... Indices>
    void ConnentStaticFunctionImpl(std::function<TReturn(TArgs...)> staticFunc, const std::index_sequence<Indices...>&)
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
    std::vector<std::function<TReturn()>> m_vecBoundFixedArgsFunction;
    std::vector<std::function<TReturn(TArgs...)>> m_vecBoundFunc;
};

template <typename T>
class Delegator;

template <typename... TArgs>
class Delegator<void(TArgs...)> : public DelegatorBase<void(TArgs...)>
{
public:
    virtual void CallFunction(TArgs&&... args) const override
    {
        for (const auto& boundFunc : this->m_vecBoundFunc)
        {
            boundFunc(std::forward<TArgs>(args)...);
        }
    }

    virtual void CallFixedArgumentsFunction() const override
    {
        for (const auto& boundFixedArgsFunction : this->m_vecBoundFixedArgsFunction)
        {
            boundFixedArgsFunction();
        }
    }
};

template <typename TReturn, typename... TArgs>
class Delegator<TReturn(TArgs...)> : public DelegatorBase<TReturn(TArgs...)>
{
public:
    virtual TReturn CallFunction(TArgs&&... args) const override
    {
        TReturn retValue = TReturn();
        for (const auto& boundFunc : this->m_vecBoundFunc)
        {
            retValue = boundFunc(std::forward<TArgs>(args)...);
        }

        return retValue;
    }

    virtual TReturn CallFixedArgumentsFunction() const override
    {
        TReturn retValue = TReturn();
        for (const auto& boundFixedArgsFunction : this->m_vecBoundFixedArgsFunction)
        {
            retValue = boundFixedArgsFunction();
        }

        return retValue;
    }
};