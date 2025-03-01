// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

template <typename... Args>
void MakeFormatString(std::string& outStr, const char* szFormat, Args... args)
{
    // snprintf()�� ��ȯ���� ���ڿ� ������ ������ ������ ũ�⸦ �˷��ִµ�
    // �̸� �̿��ؼ� nullptr�� �����ϸ� ��ŭ ������ Ȯ���ؾ� ������ �� �� �ֽ��ϴ�.
    // �̷��� �̸� ���� ������ Ȯ���� �� �ְ� �˷��ִ� �Լ��� �� �����ϴ�.

    int32 length = snprintf(nullptr, 0, szFormat, std::forward<Args>(args)...) + 1; // ���Ṯ�� ����
    if (length <= 0)
    {
        return;
    }

    outStr.resize(length - 1); // ���Ṯ�� ����
    snprintf(outStr.data(), length, szFormat, std::forward<Args>(args)...); // �ٽ� ���Ṯ�� ����
}