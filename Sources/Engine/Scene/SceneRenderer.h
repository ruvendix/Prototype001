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
	Vec2d ConvertWorldPositionToRenderingPosition(const Vec2d& worldPos) const;
	
	int32 GetViewerWidth() const { return m_viewerInfo.resolution.width; }
	int32 GetViewerHeight() const { return m_viewerInfo.resolution.height; }

	Size GetViewerHalfResoultion() const { return (m_viewerInfo.resolution / 2); }

	int32 GetViewerHalfWidth() const { return (m_viewerInfo.resolution.width / 2); }
	int32 GetViewerHalfHeight() const { return (m_viewerInfo.resolution.height / 2); }

	void SetCameraTargetActor(Actor* pCameraTargetActor) { m_pCameraTargetActor = pCameraTargetActor; }

private:
	ViewerInfo m_viewerInfo;

	HDC m_hFrontBufferDc = nullptr;
	HDC m_hBackBufferDc = nullptr;
	HBITMAP m_hBackBuffer = nullptr;

	Actor* m_pCameraTargetActor = nullptr;
};