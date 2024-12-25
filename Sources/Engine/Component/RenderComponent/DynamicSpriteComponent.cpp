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
		if (m_localTime > (endKeyFrameTime * 1.2f)) // 120% �Ѿ�� ������ ������� ����
		{
			while (m_localTime > endKeyFrameTime)
			{
				m_localTime -= endKeyFrameTime;
			}

			// ���� �˻����� ã�� (������ Ÿ�� ���̺� �ȿ� ����)
			const auto& foundIter = std::lower_bound(vecTimeTable.cbegin(), vecTimeTable.cend(), m_localTime);
			m_currentSpriteDrawInfoIdx = std::distance(vecTimeTable.cbegin(), foundIter);
		}
		else
		{
			++m_currentSpriteDrawInfoIdx; // ���� �ε���
		}

		// �ε����� ������ �ڿ� ó��
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

	// ��ǥ ��������
	const TransformComponent* pTransformComponent = GetOwner()->BringTransformComponent();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);

	// ���� ��ǥ�� �Ǻ�
	Position2d renderingStartPos = CalculateRenderingStartPosition();

	// ��������Ʈ ���� ��������
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