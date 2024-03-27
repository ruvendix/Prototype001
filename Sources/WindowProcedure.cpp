/*
<내용 요약>
윈도우 메시지를 받으면 호출되는 콜백 함수
*/

#include "Pch.h"
#include "WindowProcedure.h"

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hDc = BeginPaint(hWnd, &ps);
        UNREFERENCED_PARAMETER(hDc);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(EXIT_SUCCESS);
        break;

    default:
        return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}