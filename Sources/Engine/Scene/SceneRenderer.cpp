// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SceneRenderer.h"

DEFINE_SINGLETON(SceneRenderer);

void SceneRenderer::InitBuffers(const ViewerInfo& viewerInfo)
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

	for (ActorPtr& spActor : SceneTracer::I()->GetRenderingTargetActors())
	{
		// 액터마다 렌더 컴포넌트 가져오기
		RenderComponentVector vecRenderComponent;
		spActor->FindRenderComponents(vecRenderComponent);

		// 렌더 컴포넌트마다 렌더 처리
		for (RenderComponent* pRenderComponent : vecRenderComponent)
		{
			ASSERT_LOG(pRenderComponent != nullptr);
			pRenderComponent->Render(m_hBackBufferDc); // 트랜스폼 넘기는 건 렌더 컴포넌트마다 다를 수 있음
		}
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

Vec2d SceneRenderer::ConvertWorldPositionToRenderingPosition(const Vec2d& worldPos) const
{
	ASSERT_LOG(m_pCameraTargetActor != nullptr);
	CameraComponent* pCameraComponent = m_pCameraTargetActor->FindComponent<CameraComponent>();
	ASSERT_LOG(pCameraComponent != nullptr);

	Vec2d renderingPos = (worldPos - pCameraComponent->GetCameraOffsetPosition());
	return renderingPos;
}