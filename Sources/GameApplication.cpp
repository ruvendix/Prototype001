#include "Pch.h"
#include "GameApplication.h"

#include "WindowProcedure.h"
#include "SystemManager.h"
#include "FrameSystem.h"
#include "InputSystem.h"
#include "Camera.h"

namespace
{
    const char* g_szAppTitle = "Prototype001";
    const char* g_szClassName = "Prototype001 window class";
}

GameApplication::GameApplication()
{

}

GameApplication::~GameApplication()
{

}

int32 GameApplication::Drive()
{
	Startup();

    // 게임 루프
    MSG msg;
    ::ZeroMemory(&msg, sizeof(MSG));

    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if ((Update() == false) ||
                (PostUpdate() == false))
            {
                break;
            }

            Render();
        }
    }

    Cleanup();
	return static_cast<int32>(msg.wParam);
}

void GameApplication::Startup()
{
#pragma region 윈도우 클래스 설정
    WNDCLASSEX wcex;
    ::ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProcedure;
    wcex.hInstance = m_hInst;
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(::GetStockObject(DKGRAY_BRUSH));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = g_szClassName;

    if (RegisterClassEx(&wcex) == 0)
    {
        return;
    }
#pragma endregion

#pragma region 윈도우 생성
    m_resolution.width = 800;
    m_resolution.height = 600;

    m_halfResolution.width = m_resolution.width / 2;
    m_halfResolution.height = m_resolution.height / 2;

    RECT windowRect{ 0, 0, static_cast<int32>(m_resolution.width), static_cast<int32>(m_resolution.height) };
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    m_hAppWnd = CreateWindow(g_szClassName, g_szAppTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, (windowRect.right - windowRect.left), (windowRect.bottom - windowRect.top), nullptr, nullptr, m_hInst, nullptr);

    if (m_hAppWnd == nullptr)
    {
        return;
    }

    ShowWindow(m_hAppWnd, SW_SHOW);
    UpdateWindow(m_hAppWnd);
#pragma endregion

#pragma region 더블 버퍼링 설정
    m_hDc = ::GetDC(m_hAppWnd);
    m_hBackBufferDc = ::CreateCompatibleDC(m_hDc);
    m_hBackBuffer = ::CreateCompatibleBitmap(m_hDc, m_resolution.width, m_resolution.height);
    HBITMAP hPrevBitmap = static_cast<HBITMAP>(::SelectObject(m_hBackBufferDc, m_hBackBuffer));
    ::DeleteObject(hPrevBitmap);
#pragma endregion

    SystemManager::I()->Startup();
}

bool GameApplication::Update()
{
    bool bRet = SystemManager::I()->Update();

    if (GET_SYSTEM(InputSystem)->IsKeyDown(EInputValue::Esc))
    {
        bRet = false;
    }

    return bRet;
}

bool GameApplication::PostUpdate()
{
    bool bRet = SystemManager::I()->PostUpdate();

    // 모든 업데이트가 반영된 후 카메라 위치 조정
    if (m_spCurrentCamera != nullptr)
    {
        m_spCurrentCamera->CalcScreenCenterOffsetPosition();
    }

    return bRet;
}

void GameApplication::Render()
{
    SystemManager::I()->Render();

    RenderUi2d();

    ::BitBlt(m_hDc, 0, 0, m_resolution.width, m_resolution.height, m_hBackBufferDc, 0, 0, SRCCOPY);
    ::PatBlt(m_hBackBufferDc, 0, 0, m_resolution.width, m_resolution.height, WHITENESS);
}

void GameApplication::Cleanup()
{
    SystemManager::I()->Cleanup();

    ::DeleteObject(m_hBackBuffer);
    ::ReleaseDC(m_hAppWnd, m_hBackBufferDc);
    ::ReleaseDC(m_hAppWnd, m_hDc);
}

void GameApplication::RenderUi2d()
{
    std::string strFrame = GET_SYSTEM(FrameSystem)->GetFrameGuide();
    ::TextOut(m_hBackBufferDc, 0, 0, strFrame.c_str(), strFrame.length());

    const Int2d& mousePos = GET_SYSTEM(InputSystem)->GetMousePosition();
    std::string strMousePos = std::format("[{0}, {1}]", mousePos.x, mousePos.y);
    ::TextOut(m_hBackBufferDc, 0, 20, strMousePos.c_str(), strMousePos.length());
}

HDC GameApplication::CreateCompatibleDc()
{
    return ::CreateCompatibleDC(m_hDc);
}