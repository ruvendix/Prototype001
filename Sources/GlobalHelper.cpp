#include "Pch.h"
#include "GlobalHelper.h"

namespace global
{
    void SaveStringToFile(const std::string& str, FILE* pFile)
    {
        uint32 strLength = str.length();
        fwrite(&strLength, sizeof(uint32), 1, pFile);
        fwrite(str.c_str(), sizeof(char), strLength, pFile);
    }

    std::string LoadStringFromFile(FILE* pFile)
    {
        // ��Ʈ���� �� ���ڸ� ������� �˾ƾ���
        uint32 strLength = 0;
        fread(&strLength, sizeof(uint32), 1, pFile);

        std::string str;

        char* stringBuffer = static_cast<char*>(malloc(sizeof(char) * (strLength + 1))); // ���� ���� ����
        if (stringBuffer != nullptr)
        {
            fread(stringBuffer, sizeof(char), strLength, pFile);
            stringBuffer[strLength] = '\0';
            str = stringBuffer;
            free(stringBuffer);
        }

        return str;
    }
}