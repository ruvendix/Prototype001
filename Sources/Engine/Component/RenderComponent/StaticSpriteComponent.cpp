// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "StaticSpriteComponent.h"

#include "Engine/Resource/Sprite/StaticSprite.h"

DEFINE_COMPILETIME_ID(StaticSpriteComponent, ComponentIdCounter)

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
	//TRACE_LOG(LogDefault, "스프라이트 컴포넌트!");

	// 좌표 가져오기
	const TransformComponent* pTransformComponent = GetOwner()->BringTransformComponent();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);

	Position2d renderingStartPos = CalculateRenderingStartPosition();
	const SpriteDrawInfo& spriteDrawInfo = m_spStaticSprite->GetSpriteDrawInfo();

	if (spriteDrawInfo.colorKey != 0)
	{
		::TransparentBlt(hBackBufferDc,
			renderingStartPos.x, renderingStartPos.y,
			pTransformComponent->GetWidth(), pTransformComponent->GetHeight(),
			pTex->GetTextureDc(),
			spriteDrawInfo.beginDrawPos.x, spriteDrawInfo.beginDrawPos.y,
			spriteDrawInfo.drawSize.width, spriteDrawInfo.drawSize.height,
			spriteDrawInfo.colorKey);
	}
	else
	{
		::StretchBlt(hBackBufferDc,
			renderingStartPos.x, renderingStartPos.y,
			pTransformComponent->GetWidth(), pTransformComponent->GetHeight(),
			pTex->GetTextureDc(),
			spriteDrawInfo.beginDrawPos.x, spriteDrawInfo.beginDrawPos.y,
			spriteDrawInfo.drawSize.width, spriteDrawInfo.drawSize.height,
			SRCCOPY);
	}

	//::BitBlt(hBackBufferDc,
	//	renderingStartPos.x, renderingStartPos.y,
	//	pTransformComponent->GetWidth(), pTransformComponent->GetHeight(),
	//	pTex->GetTextureDc(),
	//	m_spStaticSprite->GetBeginDrawPosX(), m_spStaticSprite->GetBeginDrawPosY(),
	//	SRCCOPY);
}