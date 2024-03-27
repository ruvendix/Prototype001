/*
<내용 요약>
프로그램이 시작되는 부분
*/

#include "Pch.h"
#include "GameApplication.h"

int APIENTRY WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR szCmdLine, _In_ int32 cmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInst);
    UNREFERENCED_PARAMETER(szCmdLine);
    UNREFERENCED_PARAMETER(cmdShow);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    GameApplication::I()->SetInstanceHandle(hInst);
    return (GameApplication::I()->Drive());
}