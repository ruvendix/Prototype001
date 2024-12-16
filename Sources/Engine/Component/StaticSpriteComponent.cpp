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
	Vec2d renderingPos = SceneRenderer::I()->ConvertWorldPositionToRenderingPosition(pTransformComponent->GetPosition());

	Position2d renderingStartPos;
	renderingStartPos.x = static_cast<int32>(renderingPos.x) - (pTransformComponent->GetWidth() / 2);
	renderingStartPos.y = static_cast<int32>(renderingPos.y) - (pTransformComponent->GetHeight() / 2);

	//DEFAULT_TRACE_LOG("%d", renderingStartPos.x);

	// ���� ����ó�� �ʿ�

	//::TransparentBlt(hBackBufferDc,
	//	renderingStartPos.x, renderingStartPos.y,
	//	pTransformComponent->GetWidth(), pTransformComponent->GetHeight(),
	//	pTex->GetTextureDc(),
	//	m_spStaticSprite->GetBeginDrawPosX(), m_spStaticSprite->GetBeginDrawPosY(),
	//	m_spStaticSprite->GetDrawWidth(), m_spStaticSprite->GetDrawHeight(),
	//	0); // m_spDynamicSprite->GetColorKey());

	::BitBlt(hBackBufferDc,
		renderingStartPos.x, renderingStartPos.y,
		pTransformComponent->GetWidth(), pTransformComponent->GetHeight(),
		pTex->GetTextureDc(),
		m_spStaticSprite->GetBeginDrawPosX(), m_spStaticSprite->GetBeginDrawPosY(),
		SRCCOPY);

	//::StretchBlt(hBackBufferDc,
	//	renderingStartPos.x, renderingStartPos.y,
	//	pTransformComponent->GetWidth(), pTransformComponent->GetHeight(),
	//	pTex->GetTextureDc(),
	//	m_spStaticSprite->GetBeginDrawPosX(), m_spStaticSprite->GetBeginDrawPosY(),
	//	m_spStaticSprite->GetDrawWidth(), m_spStaticSprite->GetDrawHeight(),
	//	SRCCOPY);
}