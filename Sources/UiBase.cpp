#include "Pch.h"
#include "UiBase.h"

#include "GameApplication.h"

UiBase::UiBase()
{

}

UiBase::~UiBase()
{

}

void UiBase::Startup()
{

}

bool UiBase::Update()
{
	return true;
}

void UiBase::Render()
{
	const SpriteInfo* pCurrentUiSpriteInfo = m_spUiSprite->GetSpriteInfo(m_uiSpriteIdx);
	assert(pCurrentUiSpriteInfo != nullptr);

	::TransparentBlt
	(
		GameApplication::I()->GetBackBufferDc(),
		m_pos.x - (m_size.width / 2),
		m_pos.y - (m_size.height / 2),
		m_size.width,
		m_size.height,
		pCurrentUiSpriteInfo->spTex->GetDc(),
		pCurrentUiSpriteInfo->pos.x,
		pCurrentUiSpriteInfo->pos.y,
		pCurrentUiSpriteInfo->size.width,
		pCurrentUiSpriteInfo->size.height,
		pCurrentUiSpriteInfo->excludeColor
	);
}

void UiBase::Cleanup()
{

}