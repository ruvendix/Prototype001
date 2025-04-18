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
	TOwner* m_pOwner = nullptr;

#define ALLOC_PIMPL m_spPimpl = std::make_shared<Pimpl>(this)

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

#define TO_NUM(enumValue) static_cast<uint32>(enumValue)
#define TO_ENUM(value, EnumType) static_cast<EnumType>(value)

#define DECLARE_COMPILETIMER_COUNTER(TCounter) struct TCounter { }
#define DEFINE_COMPILETIMER_COUNTER(TCounter) uint32 CompiletimeCounter<TCounter>::s_num = 0
#define GET_NEXT_COMPILEITME_ID(TCounter) CompiletimeCounter<TCounter>::NextValue()
#define GET_COMPILEITME_ID_COUNT(TCounter) CompiletimeCounter<TCounter>::s_num

#define DEFINE_ROOT_COMPILETIME_ID_CLASS public: virtual uint32 CompiletimeId() const { return -1; }
#define DEFINE_COMPILETIME_ID(Type, TCounter) uint32 Type::s_id = GET_NEXT_COMPILEITME_ID(TCounter);

#define DECLARE_COMPILETIME_ID \
public:\
	static uint32 s_id; \
	virtual uint32 CompiletimeId() const override { return s_id; }