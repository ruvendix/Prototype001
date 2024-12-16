// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "TimeUtility.h"

namespace
{
	LARGE_INTEGER g_frequency;
}

void InitializePerformanceFrequency()
{
	::QueryPerformanceFrequency(&g_frequency);
}

void RenewLocalTimeString(std::string& outStrLocalTime)
{
	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);  // 시스템의 현지 시간을 SYSTEMTIME으로 가져옴

	// 연월일
	outStrLocalTime += std::format("{:04}_{:02}_{:02}_", systemTime.wYear, systemTime.wMonth, systemTime.wDay);

	// 요일을 문자열로 변환 (0 = Sunday, ..., 6 = Saturday)
	const char* szDaysOfWeekTable[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
	outStrLocalTime += std::format("{}_", szDaysOfWeekTable[systemTime.wDayOfWeek]);

	// 시분초 (12시간제)
	int hour = ((systemTime.wHour % 12) == 0);
	hour = ((hour == 0) ? (12) : (hour)); // 12시간제에서 0시는 12로 표시

	outStrLocalTime += std::format("{:02}_{:02}_", hour, systemTime.wMinute);
	outStrLocalTime += std::format("{:02}_", systemTime.wSecond);

	// AM/PM 추가
	outStrLocalTime += (systemTime.wHour < 12 ? "AM" : "PM");
}

LARGE_INTEGER MeasureTime()
{
	LARGE_INTEGER time;
	::QueryPerformanceCounter(&time);
	return time;
}

float CalculateTimeInterval(LARGE_INTEGER lhsTime, LARGE_INTEGER rhsTime)
{
	float timeInterval = static_cast<float>(lhsTime.QuadPart - rhsTime.QuadPart);
	timeInterval = std::abs(timeInterval); // 음수 방지
	timeInterval /= (g_frequency.QuadPart); // 주파수로 나누면 초단위

	//float diff = (() / static_cast<float>(Timer::s_frequency.QuadPart));
	//if (diff < 0.016f)
	//{
	//	::timeBeginPeriod(1);
	//	std::this_thread::sleep_for(16ms);
	//	::timeEndPeriod(1);
	//}

	return timeInterval;
}