// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

void InitializePerformanceFrequency();
void RenewLocalTimeString(std::string& outStrLocalTime);
LARGE_INTEGER MeasureTime();
float CalculateTimeInterval(LARGE_INTEGER lhsTime, LARGE_INTEGER rhsTime);