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
	::GetLocalTime(&systemTime);  // �ý����� ���� �ð��� SYSTEMTIME���� ������

	// ������
	outStrLocalTime += std::format("{:04}_{:02}_{:02}_", systemTime.wYear, systemTime.wMonth, systemTime.wDay);

	// ������ ���ڿ��� ��ȯ (0 = Sunday, ..., 6 = Saturday)
	const char* szDaysOfWeekTable[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
	outStrLocalTime += std::format("{}_", szDaysOfWeekTable[systemTime.wDayOfWeek]);

	// �ú��� (12�ð���)
	int hour = ((systemTime.wHour % 12) == 0);
	hour = ((hour == 0) ? (12) : (hour)); // 12�ð������� 0�ô� 12�� ǥ��

	outStrLocalTime += std::format("{:02}_{:02}_", hour, systemTime.wMinute);
	outStrLocalTime += std::format("{:02}_", systemTime.wSecond);

	// AM/PM �߰�
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
	timeInterval = std::abs(timeInterval); // ���� ����
	timeInterval /= (g_frequency.QuadPart); // ���ļ��� ������ �ʴ���

	//float diff = (() / static_cast<float>(Timer::s_frequency.QuadPart));
	//if (diff < 0.016f)
	//{
	//	::timeBeginPeriod(1);
	//	std::this_thread::sleep_for(16ms);
	//	::timeEndPeriod(1);
	//}

	return timeInterval;
}