// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameApplication.h"

#include "WindowProcedure.h"
#include "Client/Scene/GameScene.h"

namespace
{
    const char* g_szGameAppTitle = "Prototype001";
    const char* g_szWndClassName = "Prototype001 window class";
}

// Pimpl �����ؾ���
DEFINE_SINGLETON(GameApplication);

int32 GameApplication::Drive()
{
	Startup();

    // ���� ����
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
            float deltaSeconds = GameFrameHandler::I()->GetDeltaSeconds();
            if (Update(deltaSeconds) == false)
            {
                break;
            }

            // ������
            SceneRenderer::I()->Render();

            GameFrameHandler::I()->SyncFrameRate();
            GameFrameHandler::I()->ProcessFramePerSeconds();
        }
    }

    Cleanup();
	return static_cast<int32>(msg.wParam);
}

void KeyHoldEnd()
{
    TRACE_LOG(LogDefault, "Ű Ȧ��!");
}

void GameApplication::Startup()
{
    // �� �޸� üũ��
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(10); // �޸� �Ҵ��� �� �ߴ���

    // Ÿ�̸ӿ� ���Ǵ� ���ļ� �ʱ�ȭ
    InitializePerformanceFrequency();

#pragma region ������ Ŭ���� ����
    WNDCLASSEX wcex;
    ::ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProcedure;
    wcex.hInstance = m_hGameAppInstance;
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(::GetStockObject(DKGRAY_BRUSH));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = g_szWndClassName;

    if (RegisterClassEx(&wcex) == 0)
    {
        return;
    }
#pragma endregion

#pragma region ������ ����
    ViewerInfo viewerInfo;
    viewerInfo.resolution = Size(1024, 768);

    RECT windowRect{ 0, 0, static_cast<int32>(viewerInfo.resolution.width), static_cast<int32>(viewerInfo.resolution.height) };
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    viewerInfo.hWndViewer = CreateWindow(g_szWndClassName, g_szGameAppTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, (windowRect.right - windowRect.left), (windowRect.bottom - windowRect.top),
        nullptr, nullptr, m_hGameAppInstance, nullptr);

    if (viewerInfo.hWndViewer == nullptr)
    {
        return;
    }

    ShowWindow(viewerInfo.hWndViewer, SW_SHOW);
    UpdateWindow(viewerInfo.hWndViewer);

    // �������� �ٷ� ����
    SceneRenderer::I()->Initialize(viewerInfo);
#pragma endregion

#pragma region ���� ��ü �ʱ�ȭ
    LogSystem::I()->Startup();
    ErrorHandler::I()->Startup();
    GameFrameHandler::I()->Startup();
    KeyboardInputHandler::I()->Startup();
    MouseInputHandler::I()->Startup();
    ResourceMananger::I()->Startup();    
    SceneManager::I()->Startup();
#pragma endregion

    // �� �����ؼ� �׽�Ʈ
    SceneManager::I()->CreateCurrentScene<GameScene>();
}

bool GameApplication::Update(float deltaSeconds)
{
    // �׳� �̰͵� ���� ƽ �׷����� �������
    // �׷��� ���� ����

    // �Է� ó��
    KeyboardInputHandler::I()->Update(deltaSeconds);
    MouseInputHandler::I()->Update(deltaSeconds);

    // �̰͵� ƽ �׷�?
    TimerManager::I()->UpdateTimers(deltaSeconds);

    // Ÿ�̸� ó�� ����
    SceneManager::I()->Update(deltaSeconds); // �⺻ ����(�Է� ó��, ��ǥ ���� ��)
    SceneManager::I()->PostUpdate(deltaSeconds); // �ļ� ����(�浹 ó�� ��)

    /*
    �ļ� ���� �� ��ȿ�� �������� ������
    ��ȿ�� ���͵��� Ʈ�������� �����ϴµ� 2D������ �ǹ� ����
    �׳� �������ؾ� �ϴ� �������� �߰��� Ȯ�� */
    SceneTracer::I()->Tracer();

    return true;
}

void GameApplication::Cleanup()
{
    SceneRenderer::I()->ReleaseBuffers();
    SceneManager::I()->Cleanup(); 
}