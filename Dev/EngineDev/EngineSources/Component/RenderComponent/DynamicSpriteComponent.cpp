// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "DynamicSpriteComponent.h"

#include "EngineSources/Resource/Sprite/DynamicSprite.h"

DEFINE_COMPILETIME_ID(DynamicSpriteComponent, ComponentIdCounter)

DynamicSpriteComponent::DynamicSpriteComponent()
{

}

DynamicSpriteComponent::~DynamicSpriteComponent()
{

}

bool DynamicSpriteComponent::Update(float deltaSeconds)
{
	if (m_spDynamicSprite == nullptr)
	{
		return false;
	}

	const std::vector<float>& vecTimeTable = m_spDynamicSprite->GetTimeTable();
	if (vecTimeTable.empty() == true)
	{
		return false;
	}

	// 이전 프레임에서 마지막 키프레임을 달성했다면 현재 프레임에서 추가 처리
	if (CheckDynamicSpriteEnd() == true)
	{
		if (m_spDynamicSprite->IsLoopDynamicSprite())
		{
			m_localTime -= (m_spDynamicSprite->GetDurationTime());
			m_currentSpriteDrawInfoIdx = 0;
		}
		else
		{
			return true;
		}
	}

	m_localTime += deltaSeconds;

	float spriteKeyFrameTime = m_spDynamicSprite->GetSpriteKeyFrameTime(m_currentSpriteDrawInfoIdx);
	if (m_localTime > spriteKeyFrameTime)
	{
		float endKeyFrameTime = vecTimeTable.back();
		if (m_localTime > (endKeyFrameTime * 1.2f)) // 120% 넘어가면 프레임 드랍으로 가정
		{
			while (m_localTime > endKeyFrameTime)
			{
				m_localTime -= endKeyFrameTime;
			}

			// 이진 검색으로 찾기 (무조건 타임 테이블 안에 있음)
			auto foundIter = std::lower_bound(vecTimeTable.cbegin(), vecTimeTable.cend(), m_localTime);
			m_currentSpriteDrawInfoIdx = std::distance(vecTimeTable.cbegin(), foundIter);
		}
		else
		{
			m_currentSpriteDrawInfoIdx = global::Clamp(m_currentSpriteDrawInfoIdx + 1, 0, m_spriteKeyFrameCount - 1);
		}
	}

	return true;
}

void DynamicSpriteComponent::Render(HDC hBackBufferDc)
{
	ASSERT_LOG_RETURN(m_spDynamicSprite != nullptr);
	const Texture* pTex = m_spDynamicSprite->GetTexture();
	ASSERT_LOG_RETURN(pTex != nullptr);

	// 좌표 가져오기
	const TransformComponent* pTransformComponent = GetOwner()->GetComponent<TransformComponent>();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);

	// 중점 좌표가 피봇
	Position2d renderingStartPos = CalculateRenderingStartPosition();

	// 렌더링할 스프라이트 인덱스 알아내기
	int32 renderingSpriteDrawInfoIdx = m_currentSpriteDrawInfoIdx;

	// 인덱스가 0이 아니고 무한 반복이 아니라면 마지막 인덱스로 고정
	if ((renderingSpriteDrawInfoIdx != 0) &&
		(m_spDynamicSprite->IsLoopDynamicSprite() == false))
	{
		if (m_currentSpriteDrawInfoIdx == m_spDynamicSprite->GetSpriteKeyFrameCount())
		{
			--renderingSpriteDrawInfoIdx;
		}
	}

	// 스프라이트 정보 가져오기
	const SpriteDrawInfo& spriteDrawInfo = m_spDynamicSprite->GetSpriteDrawInfo(renderingSpriteDrawInfoIdx);

	::TransparentBlt(hBackBufferDc,
		renderingStartPos.x, renderingStartPos.y,
		pTransformComponent->GetWidth(), pTransformComponent->GetHeight(),
		pTex->GetTextureDc(),
		spriteDrawInfo.drawingStartPos.x, spriteDrawInfo.drawingStartPos.y,
		spriteDrawInfo.drawSize.width, spriteDrawInfo.drawSize.height,
		spriteDrawInfo.colorKey);
}

ComponentPtr DynamicSpriteComponent::CreateClone()
{
	return std::make_shared<DynamicSpriteComponent>(*this);
}

void DynamicSpriteComponent::ResetDynamicSprite()
{
	m_localTime = 0.0f;
	m_currentSpriteDrawInfoIdx = 0;
}

void DynamicSpriteComponent::ApplyDynamicSprite(const DynamicSpritePtr& spDynamicSprite)
{
	ResetDynamicSprite();

	m_spriteKeyFrameCount = spDynamicSprite->GetSpriteKeyFrameCount();
	m_spDynamicSprite = spDynamicSprite;
}

float DynamicSpriteComponent::BringDynamicSpriteDurationTime() const
{
	return (m_spDynamicSprite->GetDurationTime());
}

bool DynamicSpriteComponent::CheckDynamicSpriteEnd() const
{
	return (m_localTime > m_spDynamicSprite->GetDurationTime());
}