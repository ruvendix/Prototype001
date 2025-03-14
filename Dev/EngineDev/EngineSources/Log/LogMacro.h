// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

// LogCategory를 상속받는 새로운 로그 카테고리 클래스를 선언합니다. (매크로가 문자열을 인식할 수 없으므로)
#define DECLARE_LOG_CATEGORY(LogCategoryClass) \
	class LogCategoryClass : public LogCategory \
	{ \
	public: \
		using LogCategory::LogCategory; \
	}; \
	\
	extern LogCategoryClass g_##LogCategoryClass

// 로그 카테고리를 객체를 만들 때는 매크로에 전달한 이름과 동일하게 만듭니다.
#define DEFINE_LOG_CATEGORY(LogCategoryClass) LogCategoryClass g_##LogCategoryClass(#LogCategoryClass)

#define ACTIVATE_LOG_CATEGORY(LogCategoryClass) g_##LogCategoryClass.SetOn(true)
#define DEACTIVATE_LOG_CATEGORY(LogCategoryClass) g_##LogCategoryClass.SetOn(false)

// 로그는 이렇게 사용
#define LOG_IMPL(LogCategory, logVerbosity, szLog, ...) { std::string strFormattedLog; MakeFormatString(strFormattedLog, szLog, __VA_ARGS__); LogSystem::I()->Log(&g_##LogCategory, strFormattedLog, logVerbosity, __FILE__, __LINE__, __FUNCTION__); }

#define TRACE_LOG(LogCategory, szLog, ...) LOG_IMPL(LogCategory, ELogVerbosity::OnlyTrace, szLog, __VA_ARGS__)
#define POPUP_LOG(LogCategory, szLog, ...) LOG_IMPL(LogCategory, ELogVerbosity::OnlyPopup, szLog, __VA_ARGS__)
#define FILE_LOG(LogCategory, szLog, ...) LOG_IMPL(LogCategory, ELogVerbosity::OnlyFile, szLog, __VA_ARGS__)
#define SCREEN_LOG(LogCategory, szLog, ...) LOG_IMPL(LogCategory, ELogVerbosity::OnlyScreen, szLog, __VA_ARGS__)

#define NOTICE_LOG(LogCategory, szLog, ...) LOG_IMPL(LogCategory, ELogVerbosity::Notice, szLog, __VA_ARGS__)
#define ERROR_LOG(LogCategory, szLog, ...) LOG_IMPL(LogCategory, ELogVerbosity::Error, szLog, __VA_ARGS__)
#define DISPLAY_LOG(LogCategory, szLog, ...) LOG_IMPL(LogCategory, ELogVerbosity::Display, szLog, __VA_ARGS__)

#define DEFAULT_TRACE_LOG(szLog, ...) TRACE_LOG(LogDefault, szLog, __VA_ARGS__)

#ifdef _DEBUG
#define ASSERT_LOG(expression) \
	if (!(expression)) \
	{ \
        LogSystem::I()->Log(&g_LogAssert, #expression, ELogVerbosity::Assert, __FILE__, __LINE__, __FUNCTION__); \
	}

#define ASSERT_LOG_RETURN(expression) \
	ASSERT_LOG(expression) \
	if (!(expression)) \
	{ \
		return; \
	}

#define ASSERT_LOG_RETURN_VALUE(expression, value) \
	ASSERT_LOG(expression) \
	if (!(expression)) \
	{ \
		return value; \
	}
#else
#define ASSERT_LOG(expression) __noop
#define ASSERT_LOG_RETURN(expression) __noop
#define ASSERT_LOG_RETURN_VALUE(expression, value) __noop
#endif

#define DETAIL_ERROR_LOG(errorCode, ...) ErrorHandler::I()->ProcessErrorContents(errorCode, __VA_ARGS__)