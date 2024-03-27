/*
<내용 요약>
자주 사용하는 매크로 모음
*/

#pragma once

#define SAFE_DELETE(ptr)\
	if (ptr != nullptr)\
	{\
		delete ptr;\
		ptr = nullptr;\
	}

#define FOR_EACH_SAFE_DELETE(Container)\
	for (auto iter : Container)\
	{\
		SAFE_DELETE(iter);\
	}

#define FOR_EACH_PAIR_SAFE_DELETE(Container)\
	for (auto iter : Container)\
	{\
		SAFE_DELETE(iter.second);\
	}

#define DEFINE_SINGLETON(ClassType)\
	static ClassType* I()\
	{\
		static ClassType s_singleton;\
		return &s_singleton;\
	}

#define DEFINE_COMPILER_COUNTER(Type)\
    struct Type { };\
    fameta::counter<Type, 0> Type##Instance

#define DEFINE_ID(CompilerCounterType, IdType)\
	class IdType##Id\
	{\
	public:\
		constexpr static int s_id = CompilerCounterType##Instance.next<__COUNTER__>();\
	}

#ifndef new
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define DEFINE_SMART_PTR(Type) using Ptr = std::shared_ptr<Type>
#define ENUM_TO_NUM(EnumValue) static_cast<uint32>(EnumValue)