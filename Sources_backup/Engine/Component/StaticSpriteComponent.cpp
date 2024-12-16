// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "StaticSpriteComponent.h"

#include "Engine/Resource/Sprite/StaticSprite.h"

DEFINE_COMPILETIME_ID(StaticSpriteComponent, ComponentCounter)

StaticSpriteComponent::StaticSpriteComponent()
{

}

StaticSpriteComponent::~StaticSpriteComponent()
{

}

void StaticSpriteComponent::Render(HDC hBackBufferDc)
{
	ASSERT_LOG_RETURN(m_spStaticSprite != nullptr);
	const Texture* pTex = m_spStaticSprite->GetTexture();
	ASSERT_LOG_RETURN(pTex != nullptr);
	//TRACE_LOG(LogDefault, "��������Ʈ ������Ʈ!");

	// ��ǥ ��������
	const TransformComponent* pTransformComponent = GetOwner()->BringTransformComponent();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);

	// ���� ��ǥ�� �Ǻ�
	Position2d drawStartPos;
	drawStartPos.x = static_cast<int32>(pTransformComponent->GetPositionX()) - (pTransformComponent->GetWidth() / 2);
	drawStartPos.y = static_cast<int32>(pTransformComponent->GetPositionY()) - (pTransformComponent->GetHeight() / 2);

	::StretchBlt(hBackBufferDc,
		drawStartPos.x, drawStartPos.y,
		pTransformComponent->GetWidth(), pTransformComponent->GetHeight(),
		pTex->GetTextureDc(),
		m_spStaticSprite->GetBegingDrawPosX(), m_spStaticSprite->GetBegingDrawPosY(),
		m_spStaticSprite->GetDrawWidth(), m_spStaticSprite->GetDrawHeight(),
		SRCCOPY);

	//::BitBlt(hBackBufferDc,
	//	static_cast<int32>(pTransformComponent->GetPosX()), static_cast<int32>(pTransformComponent->GetPosY()),
	//	pTransformComponent->GetWidth(), pTransformComponent->GetHeight(),
	//	pTex->GetTextureDc(),
	//	m_spStaticSprite->GetBegingDrawPosX(), m_spStaticSprite->GetBegingDrawPosY(),
	//	SRCCOPY);
}