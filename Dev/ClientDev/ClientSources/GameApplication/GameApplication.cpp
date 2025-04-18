// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameApplication.h"

#include "WindowProcedure.h"
#include "ClientSources/Scene/EditScene.h"

namespace
{
    const char* g_szGameAppTitle = "Prototype001";
    const char* g_szWndClassName = "Prototype001 window class";
}

// Pimpl 도입해야함
DEFINE_SINGLETON(GameApplication);

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
            float deltaSeconds = GameFrameHandler::I()->GetDeltaSeconds();
            if ((m_bForceShutdown == true) ||
                (Update(deltaSeconds) == false))
            {
                break;
            }

            // 렌더링
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
    TRACE_LOG(LogDefault, "키 홀딩!");
}

void GameApplication::Startup()
{
    // 힙 메모리 체크용
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // 타이머에 사용되는 주파수 초기화
    InitializePerformanceFrequency();

#pragma region 윈도우 클래스 설정
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

#pragma region 윈도우 생성
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

    // 렌더러에 바로 적용
    SceneRenderer::I()->Initialize(viewerInfo);
#pragma endregion

#pragma region 전역 객체 초기화
    LogSystem::I()->Startup();
    EngineErrorHandler::I()->Startup();
    ClientErrorHandler::I()->Startup();
    // 서버 예약
    GameFrameHandler::I()->Startup();
    InputDeviceMananger::I()->Startup();
    ResourceMananger::I()->Startup();    
    SceneManager::I()->Startup();
    ClientPacketHandler::I()->Startup();
#pragma endregion

    // 로그 폴더 생성
    LogSystem::I()->RegisterFileLoggerPathKey("ClientLog");
    FILE_LOG(LogDefault, "Test");

    // 씬 생성해서 테스트
    SceneManager::I()->CreateCurrentScene<EditScene>();
}

bool GameApplication::Update(float deltaSeconds)
{
    // 입력 처리
    InputDeviceMananger::I()->Update(deltaSeconds);

    // 로컬 플레이어 입력 처리
    LocalPlayerInputSystem::I()->Update(deltaSeconds);

    // 타이머 처리
    TimerManager::I()->UpdateTimers(deltaSeconds);

    // 씬 처리
    SceneManager::I()->Update(deltaSeconds); // 기본 갱신(입력 처리, 좌표 갱신 등)

    /*
    후속 갱신 때 유효한 액터인지 판정됨
    유효한 액터들은 트랜스폼을 갱신하는데 2D에서는 의미 없고
    그냥 렌더링해야 하는 액터인지 추가로 확인 */
    SceneTracer::I()->Tracer();

    return true;
}

void GameApplication::Cleanup()
{
    SceneRenderer::I()->ReleaseBuffers();
    SceneManager::I()->Cleanup();
    ClientPacketHandler::I()->Cleanup();

    // Protobuf 정리
    google::protobuf::ShutdownProtobufLibrary();
}