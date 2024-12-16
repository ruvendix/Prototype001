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

	// ����۸� ������� ����
	::PatBlt(m_hBackBufferDc, 0, 0, viewerResolution.width, viewerResolution.height, WHITENESS);

	for (ActorPtr& spActor : SceneTracer::I()->GetRenderingTargetActors())
	{
		// ���͸��� ���� ������Ʈ ��������
		RenderComponentVector vecRenderComponent;
		spActor->FindRenderComponents(vecRenderComponent);

		// ���� ������Ʈ���� ���� ó��
		for (RenderComponent* pRenderComponent : vecRenderComponent)
		{
			ASSERT_LOG(pRenderComponent != nullptr);
			pRenderComponent->Render(m_hBackBufferDc); // Ʈ������ �ѱ�� �� ���� ������Ʈ���� �ٸ� �� ����
		}
	}

	// ����ۿ� �׸� �� ����Ʈ���۷� ����
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