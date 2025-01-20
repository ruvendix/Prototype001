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
	void Initialize(const ViewerInfo& viewerInfo);
	void Render();
	void ReleaseBuffers();

	HDC CreateCompatibleDc();
	Vec2d ConvertWorldPositionToRenderingPosition(const Vec2d& worldPos) const;

	const Size& GetViewerResolution() const { return m_viewerInfo.resolution; }
	
	int32 GetViewerWidth() const { return m_viewerInfo.resolution.width; }
	int32 GetViewerHeight() const { return m_viewerInfo.resolution.height; }

	Size GetViewerHalfResoultion() const { return (m_viewerInfo.resolution / 2); }

	int32 GetViewerHalfWidth() const { return (m_viewerInfo.resolution.width / 2); }
	int32 GetViewerHalfHeight() const { return (m_viewerInfo.resolution.height / 2); }

	HWND GetViewerHandle() const { return (m_viewerInfo.hWndViewer); }

	void SetMainCameraActor(const std::shared_ptr<CameraActor>& spMainCameraActor) { m_spMainCameraActor = spMainCameraActor; }
	const std::shared_ptr<CameraActor>& GetMainCameraActor() const { return m_spMainCameraActor; } // ¾À¿¡ ¼Ò¼ÓµÈ ¾×ÅÍ°¡ ¾Æ´Ô!

private:
	ViewerInfo m_viewerInfo;

	HDC m_hFrontBufferDc = nullptr;
	HDC m_hBackBufferDc = nullptr;
	HBITMAP m_hBackBuffer = nullptr;

	std::shared_ptr<CameraActor> m_spMainCameraActor = nullptr;
};