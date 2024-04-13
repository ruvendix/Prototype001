#include "Pch.h"
#include "PlayerState.h"

#include "PlayerActor.h"
#include "FlipbookComponent.h"
#include "SystemManager.h"
#include "ResourceSystem.h"

PlayerStateBase::PlayerStateBase(PlayerActor* pOwner)
{
	m_pOwner = pOwner;
}

PlayerStateBase::~PlayerStateBase()
{

}

void PlayerStateBase::Startup()
{

}

bool PlayerStateBase::Update()
{
	return true;
}

bool PlayerStateBase::PostUpdate()
{
	return true;
}

void PlayerStateBase::Render()
{

}

void PlayerStateBase::Cleanup()
{

}

void PlayerStateBase::ChangePlayerFlipbookOnDirection(const PlayerDirectionStrings& playerDirStrs)
{
	std::string strCurrentFlipbook = playerDirStrs[ENUM_TO_NUM(m_pOwner->GetCurrentDirection())];
	FlipbookPtr spPlayerFlipbook = GET_SYSTEM(ResourceSystem)->FindFlipbook(strCurrentFlipbook.c_str());

	FlipbookComponentPtr spFlipbookComponent = GET_COMPONENT(m_pOwner, FlipbookComponent);
	spFlipbookComponent->SetFlipbook(spPlayerFlipbook);
}
//////////////////////////////////////////////////////////////////////////////////////////
PlayerIdleState::~PlayerIdleState()
{

}

bool PlayerIdleState::Update()
{
	std::array<std::string, ENUM_TO_NUM(EPlayerDirection::Count)> idleStrs =
	{
		"Texture/Player/PlayerLeftIdle",
		"Texture/Player/PlayerRightIdle",
		"Texture/Player/PlayerUpIdle",
		"Texture/Player/PlayerDownIdle"
	};

	ChangePlayerFlipbookOnDirection(idleStrs);
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
PlayerWalkState::~PlayerWalkState()
{

}

bool PlayerWalkState::Update()
{
	std::array<std::string, ENUM_TO_NUM(EPlayerDirection::Count)> walkStrs =
	{
		"Texture/Player/PlayerLeftWalk",
		"Texture/Player/PlayerRightWalk",
		"Texture/Player/PlayerUpWalk",
		"Texture/Player/PlayerDownWalk"
	};

	ChangePlayerFlipbookOnDirection(walkStrs);
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
PlayerStateEnd::~PlayerStateEnd()
{

}