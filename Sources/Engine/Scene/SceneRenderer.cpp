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

	m_spMainCameraActor = Scene::CreateActor<CameraActor>();
}

void SceneRenderer::Render()
{
	const Size& viewerResolution = m_viewerInfo.resolution;

	// 백버퍼를 흰색으로 덮음
	::PatBlt(m_hBackBufferDc, 0, 0, viewerResolution.width, viewerResolution.height, WHITENESS);

	RenderComponentVectorArray arrVecRenderComponent;
	for (ActorPtr& spActor : SceneTracer::I()->GetRenderingTargetActors())
	{
		// 액터마다 렌더 컴포넌트 가져오기
		spActor->FindRenderComponents(arrVecRenderComponent);

		// 렌더 컴포넌트마다 렌더 처리
		for (const RenderComponentVector& vecRenderComponent : arrVecRenderComponent)
		{
			for (RenderComponent* pRenderComponent : vecRenderComponent)
			{
				ASSERT_LOG(pRenderComponent != nullptr);
				pRenderComponent->Render(m_hBackBufferDc); // 트랜스폼 넘기는 건 렌더 컴포넌트마다 다를 수 있음
			}
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
	ASSERT_LOG(m_spMainCameraActor != nullptr);
	CameraComponent* pCameraComponent = m_spMainCameraActor->FindComponent<CameraComponent>();
	ASSERT_LOG(pCameraComponent != nullptr);

	Vec2d renderingPos = (worldPos - pCameraComponent->GetCameraOffsetPosition());
	return renderingPos;
}

void SceneRenderer::ApplyTargetActorToMainCamera(const ActorPtr& spTargetActor)
{
	CameraComponent* pCameraComponent = m_spMainCameraActor->FindComponent<CameraComponent>();
	ASSERT_LOG_RETURN(pCameraComponent != nullptr);

	// 만약에 타겟 액터가 오너라면 무시
	if (spTargetActor.get() == pCameraComponent->GetOwner())
	{
		TRACE_LOG(LogDefault, "타겟 액터가 메인 카메라의 오너라서 설정 불가능!");
		return;
	}

	pCameraComponent->SetTargetActor(spTargetActor);
}