// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

template <typename... Args>
void MakeFormatString(std::string& outStr, const char* szFormat, Args... args)
{
    // snprintf()의 반환값은 문자열 저장이 가능한 공간의 크기를 알려주는데
    // 이를 이용해서 nullptr을 전달하면 얼만큼 공간을 확보해야 할지를 알 수 있습니다.
    // 이렇게 미리 저장 공간을 확보할 수 있게 알려주는 함수가 꽤 많습니다.

    int32 length = snprintf(nullptr, 0, szFormat, std::forward<Args>(args)...) + 1; // 종료문자 포함
    if (length <= 0)
    {
        return;
    }

    outStr.resize(length - 1); // 종료문자 제외
    snprintf(outStr.data(), length, szFormat, std::forward<Args>(args)...); // 다시 종료문자 포함
}