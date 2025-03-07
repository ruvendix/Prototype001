// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ClientSources/GameApplication/GameApplication.h"

int32 APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR szCmdLine, _In_ int32 cmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(szCmdLine);
    UNREFERENCED_PARAMETER(cmdShow);
    
    GameApplication::I()->SetInstanceHandle(hInstance);
    return (GameApplication::I()->Drive());
}