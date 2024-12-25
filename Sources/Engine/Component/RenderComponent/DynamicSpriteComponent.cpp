// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "DynamicSpriteComponent.h"

#include "Engine/Resource/Sprite/DynamicSprite.h"

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

	if ((m_bLoop == false) &&
		(m_currentSpriteDrawInfoIdx == m_spriteKeyFrameCount))
	{
		return false;
	}

	m_localTime += deltaSeconds;

	float spriteKeyFrameTime = m_spDynamicSprite->GetSpriteKeyFrameTime(m_currentSpriteDrawInfoIdx);
	if (m_localTime > spriteKeyFrameTime)
	{
		const std::vector<float>& vecTimeTable = m_spDynamicSprite->GetTimeTable();

		float endKeyFrameTime = vecTimeTable.back();
		if (m_localTime > (endKeyFrameTime * 1.2f)) // 120% 넘어가면 프레임 드랍으로 가정
		{
			while (m_localTime > endKeyFrameTime)
			{
				m_localTime -= endKeyFrameTime;
			}

			// 이진 검색으로 찾기 (무조건 타임 테이블 안에 있음)
			const auto& foundIter = std::lower_bound(vecTimeTable.cbegin(), vecTimeTable.cend(), m_localTime);
			m_currentSpriteDrawInfoIdx = std::distance(vecTimeTable.cbegin(), foundIter);
		}
		else
		{
			++m_currentSpriteDrawInfoIdx; // 다음 인덱스
		}

		// 인덱스가 증가된 뒤에 처리
		if (m_currentSpriteDrawInfoIdx == m_spriteKeyFrameCount)
		{
			if (m_bLoop == true)
			{
				m_localTime -= spriteKeyFrameTime;
				m_currentSpriteDrawInfoIdx = 0;
			}
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
	const TransformComponent* pTransformComponent = GetOwner()->BringTransformComponent();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);

	// 중점 좌표가 피봇
	Position2d renderingStartPos = CalculateRenderingStartPosition();

	// 스프라이트 정보 가져오기
	const SpriteDrawInfo& spriteDrawInfo = m_spDynamicSprite->GetSpriteDrawInfo(m_currentSpriteDrawInfoIdx);

	::TransparentBlt(hBackBufferDc,
		renderingStartPos.x, renderingStartPos.y,
		pTransformComponent->GetWidth(), pTransformComponent->GetHeight(),
		pTex->GetTextureDc(),
		spriteDrawInfo.beginDrawPos.x, spriteDrawInfo.beginDrawPos.y,
		spriteDrawInfo.drawSize.width, spriteDrawInfo.drawSize.height,
		spriteDrawInfo.colorKey);
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