// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#ifndef new
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define SAFE_DELETE(ptr) \
	if (ptr != nullptr) \
	{ \
		delete ptr; \
		ptr = nullptr; \
	}

#define FORBID_COPY(T) \
private: \
    T(const T&); \
    T& operator=(const T&) = delete;

#define DECLARE_PIMPL \
	class Pimpl; \
	friend class Pimpl; \
	std::shared_ptr<Pimpl> m_spPimpl

#define CLASS_PIMPL(TOwner) class TOwner::Pimpl

#define DEFINE_PIMPL(TOwner) \
public: \
	Pimpl(TOwner* pOwner) { m_pOwner = pOwner; } \
	~Pimpl() = default; \
private: \
	TOwner* m_pOwner = nullptr

#define CREATE_PIMPL m_spPimpl = std::make_shared<Pimpl>(this)

#define DECLARE_SINGLETON(T) \
FORBID_COPY(T) \
\
public: \
    static T* I() \
    { \
        static std::once_flag onceFlag; \
        std::call_once(onceFlag, []() { s_spInstance.reset(new T); }); \
        return (s_spInstance.get()); \
    } \
private: \
    static std::unique_ptr<T> s_spInstance; \
    T() = default;

#define DEFINE_SINGLETON(T) std::unique_ptr<T> T::s_spInstance = nullptr

#define DEFINE_ID(CompilerCounterType, IdType)\
	class IdType##Id\
	{\
	public:\
		constexpr static int s_id = CompilerCounterType##Instance.next<__COUNTER__>();\
	}

#define VALIDATE_INDEX_RANGE(idx, size) global::ValidateIndexRange(static_cast<int32>(idx), static_cast<int32>(size));

#define TO_NUM(enumValue) static_cast<int32>(enumValue)
#define TO_ENUM(value, EnumType) static_cast<EnumType>(value)

#define DEFINE_COMPILETIMER_COUNTER(TCounter) struct TCounter { };
#define DECLARE_COMPILETIME_ID(Type) public: static const int32 s_id;
#define DEFINE_COMPILETIME_ID(Type, TCounter) const int32 Type::s_id = CompiletimeCounter<TCounter>::NextValue();