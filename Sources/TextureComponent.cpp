#include "Pch.h"
#include "TextureComponent.h"

#include "SystemManager.h"
#include "ResourceSystem.h"
#include "GameApplication.h"
#include "Actor.h"
#include "TransformComponent.h"

TextureComponent::TextureComponent()
{

}

TextureComponent::~TextureComponent()
{

}

void TextureComponent::Render()
{
	TransformComponent::Ptr spTransformComponent = GET_COMPONENT(GetOwner(), TransformComponent);
	const Size& size = spTransformComponent->GetSize();

	Point2d destPos;
	Camera::Ptr spCamera = GameApplication::I()->GetCurrentCamera();
	destPos.x = spTransformComponent->GetPosition().x - (size.width / 2);
	destPos.x -= spCamera->GetOffsetPosition().x;

	destPos.y = spTransformComponent->GetPosition().y - (size.height / 2);
	destPos.y -= spCamera->GetOffsetPosition().y;

	const Size& texSize = m_spTex->GetSize();

	::BitBlt
	(
		GameApplication::I()->GetBackBufferDc(),
		destPos.x,
		destPos.y,
		texSize.width,
		texSize.height,
		m_spTex->GetDc(),
		0,
		0,
		SRCCOPY
	);
}

Texture::Ptr TextureComponent::LoadTexture(const std::string& strTexPath)
{
	assert(m_spTex == nullptr);
	m_spTex = GET_SYSTEM(ResourceSystem)->LoadTexture(strTexPath);
	return m_spTex;
}

void TextureComponent::ChangeTexture(const std::string& strTexPath)
{
	m_spTex = GET_SYSTEM(ResourceSystem)->FindTexture(strTexPath);
	if (m_spTex == nullptr)
	{
		// 없으면 새로 로드
		m_spTex = GET_SYSTEM(ResourceSystem)->LoadTexture(strTexPath);
	}
}