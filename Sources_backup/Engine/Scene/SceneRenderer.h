// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

struct ViewerInfo
{
	HWND hWndViewer = nullptr;
	Size resolution;
};

class SceneRenderer
{
	DECLARE_SINGLETON(SceneRenderer)

public:
	void InitBuffers(const ViewerInfo& viewerInfo);
	void Render();
	void ReleaseBuffers();

	HDC CreateCompatibleDc();
	
	int32 GetViewerWidth() const { return m_viewerInfo.resolution.width; }
	int32 GetViewerHeight() const { return m_viewerInfo.resolution.height; }

private:
	ViewerInfo m_viewerInfo;

	HDC m_hFrontBufferDc = nullptr;
	HDC m_hBackBufferDc = nullptr;
	HBITMAP m_hBackBuffer = nullptr;
};