#include "Pch.h"
#include "FlipbookComponent.h"

#include "SystemManager.h"
#include "FrameSystem.h"
#include "ResourceSystem.h"
#include "GameApplication.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "Texture.h"
#include "Camera.h"
#include "Flipbook.h"

FlipbookComponent::FlipbookComponent()
{

}

FlipbookComponent::~FlipbookComponent()
{

}

bool FlipbookComponent::Update()
{
	m_localTime += GET_SYSTEM(FrameSystem)->GetDeltaSeconds();
	float flipbookEndTime = m_spFlipbook->GetFlipbookEndTime();

	if (m_localTime > flipbookEndTime)
	{
		if (m_bLoop == false)
		{
			return true;
		}

		m_localTime -= flipbookEndTime;
	}

	m_pCurrentKeyframe = m_spFlipbook->CalcKeyframe(m_localTime);
	return true;
}

void FlipbookComponent::Render()
{
	if (m_pCurrentKeyframe == nullptr)
	{
		return;
	}

	const TexturePtr spKeyframeTex = m_pCurrentKeyframe->spTex;
	if (spKeyframeTex == nullptr)
	{
		return;
	}

	TransformComponentPtr spTransformComponent = GET_COMPONENT(GetOwner(), TransformComponent);
	const Size& size = spTransformComponent->GetSize();

	Point2d destPos;
	CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();
	destPos.x = spTransformComponent->GetPosition().x - (size.width / 2);
	destPos.x -= spCamera->GetOffsetPosition().x;

	destPos.y = spTransformComponent->GetPosition().y - (size.height / 2);
	destPos.y -= spCamera->GetOffsetPosition().y;

	::TransparentBlt
	(
		GameApplication::I()->GetBackBufferDc(),
		destPos.x,
		destPos.y,
		size.width,
		size.height,
		spKeyframeTex->GetDc(),
		m_pCurrentKeyframe->startPos.x,
		m_pCurrentKeyframe->startPos.y,
		m_pCurrentKeyframe->size.width,
		m_pCurrentKeyframe->size.height,
		m_pCurrentKeyframe->excludeColor
	);
}

float FlipbookComponent::BringFlipbookEndTime() const
{
	return m_spFlipbook->GetFlipbookEndTime();
}