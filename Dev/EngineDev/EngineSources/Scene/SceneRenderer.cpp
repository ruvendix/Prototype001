// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SceneRenderer.h"

DEFINE_SINGLETON(SceneRenderer);

void SceneRenderer::Initialize(const ViewerInfo& viewerInfo)
{
	m_viewerInfo = viewerInfo;

	m_hFrontBufferDc = ::GetDC(m_viewerInfo.hWndViewer);
	m_hBackBufferDc = ::CreateCompatibleDC(m_hFrontBufferDc);
	m_hBackBuffer = ::CreateCompatibleBitmap(m_hFrontBufferDc, m_viewerInfo.resolution.width, m_viewerInfo.resolution.height);

	HBITMAP hPrevBitMap = static_cast<HBITMAP>(::SelectObject(m_hBackBufferDc, m_hBackBuffer));
	::DeleteObject(hPrevBitMap);
}

void SceneRenderer::Render()
{
	const Size& viewerResolution = m_viewerInfo.resolution;

	// 백버퍼를 흰색으로 덮음
	::PatBlt(m_hBackBufferDc, 0, 0, viewerResolution.width, viewerResolution.height, WHITENESS);

	// 렌더 컴포넌트마다 렌더 처리
	for (RenderComponent* pRenderComponent : SceneTracer::I()->GetOutputRenderComponents())
	{
		ASSERT_LOG(pRenderComponent != nullptr);
		pRenderComponent->Render(m_hBackBufferDc);
	}

	// 백버퍼에 그린 걸 프론트버퍼로 복사
	::BitBlt(m_hFrontBufferDc, 0, 0, viewerResolution.width, viewerResolution.height, m_hBackBufferDc, 0, 0, SRCCOPY);
}

void SceneRenderer::ReleaseBuffers()
{
	HWND hViewerWnd = m_viewerInfo.hWndViewer;

	::DeleteObject(m_hBackBuffer);
	::ReleaseDC(hViewerWnd, m_hBackBufferDc);
	::ReleaseDC(hViewerWnd, m_hFrontBufferDc);
}

HDC SceneRenderer::CreateCompatibleDc()
{
	return (::CreateCompatibleDC(m_hFrontBufferDc));
}

Vector2d SceneRenderer::ConvertWorldPositionToRenderingPosition(const Vector2d& worldPos) const
{
	if (m_spMainCameraActor == nullptr)
	{
		return Vector2d();
	}

	CameraComponent* pCameraComponent = m_spMainCameraActor->GetComponent<CameraComponent>();
	ASSERT_LOG(pCameraComponent != nullptr);

	Vector2d renderingPos = (worldPos - pCameraComponent->GetCameraOffsetPosition());
	return renderingPos;
}