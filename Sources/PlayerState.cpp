#include "Pch.h"
#include "PlayerState.h"

#include "PlayerActor.h"
#include "FlipbookComponent.h"
#include "TransformComponent.h"
#include "SystemManager.h"
#include "ResourceSystem.h"
#include "InputSystem.h"
#include "FrameSystem.h"
#include "TileMap.h"

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
	std::string strCurrentFlipbook = playerDirStrs[TO_NUM(m_pOwner->GetCurrentDirection())];
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
	// 입력 확인해서 이동 상태로 변경 여부를 따짐
	if (ProcessIdleInput() == false)
	{
		std::array<std::string, TO_NUM(EPlayerDirection::Count)> arrIdleStrings =
		{
			"Texture/Player/PlayerLeftIdle",
			"Texture/Player/PlayerRightIdle",
			"Texture/Player/PlayerUpIdle",
			"Texture/Player/PlayerDownIdle"
		};

		ChangePlayerFlipbookOnDirection(arrIdleStrings);
	}
	else
	{
		// 상태가 변경되면 업데이트 돌림
		GetOwner()->GetCurrentState()->Update();
	}
	
	return true;
}

// Idle 상태에서는 입력만 받고 이동 상태로 변경이 가능한지만 확인
bool PlayerIdleState::ProcessIdleInput()
{
	// 플레이어 가져옴
	PlayerActor* pPlayerActor = GetOwner();
	assert(pPlayerActor != nullptr);

	// 입력 처리
	EPlayerDirection playerNextDir = EPlayerDirection::Count;
	if (GET_SYSTEM(InputSystem)->IsKeyDown(EInputValue::Left))
	{
		playerNextDir = EPlayerDirection::Left;
	}
	else if (GET_SYSTEM(InputSystem)->IsKeyDown(EInputValue::Right))
	{
		playerNextDir = EPlayerDirection::Right;
		
	}
	else if (GET_SYSTEM(InputSystem)->IsKeyDown(EInputValue::Up))
	{
		playerNextDir = EPlayerDirection::Up;
	}
	else if (GET_SYSTEM(InputSystem)->IsKeyDown(EInputValue::Down))
	{
		playerNextDir = EPlayerDirection::Down;
	}
	else
	{
		return false;
	}

	// 이동이 가능한지 검사
	std::array<Int2d, TO_NUM(EPlayerDirection::Count)> arrDeltaCellMove =
	{
		Int2d{-1, 0},
		Int2d{1, 0},
		Int2d{0, -1},
		Int2d{0, 1},
	};

	// 속도 가져와 델타 이동값 계산
	Int2d nextCellPos = pPlayerActor->GetCurrentCellPosition() + arrDeltaCellMove[TO_NUM(playerNextDir)];

	// 타일맵을 가져와서 확인
	TileMapPtr spTileMap = GET_SYSTEM(ResourceSystem)->GetTileMap();
	TilePtr spTile = spTileMap->FindTile(nextCellPos);
	if (spTile != nullptr) // nullptr이면 갈 수 없는 곳
	{
		pPlayerActor->SetDirection(playerNextDir);
		pPlayerActor->SetPlayerStateIndex(PlayerWalkStateId::s_id);
		pPlayerActor->ApplyNextCellPosition(nextCellPos, true);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
PlayerWalkState::~PlayerWalkState()
{

}

bool PlayerWalkState::Update()
{
	if (ProcessWalkState() == true)
	{
		std::array<std::string, TO_NUM(EPlayerDirection::Count)> walkStrs =
		{
			"Texture/Player/PlayerLeftWalk",
			"Texture/Player/PlayerRightWalk",
			"Texture/Player/PlayerUpWalk",
			"Texture/Player/PlayerDownWalk"
		};

		ChangePlayerFlipbookOnDirection(walkStrs);
	}
	else
	{
		// 상태가 변경되면 업데이트 돌림
		GetOwner()->GetCurrentState()->Update();
	}

	return true;
}

bool PlayerWalkState::ProcessWalkState()
{
	// 플레이어 가져옴
	PlayerActor* pPlayerActor = GetOwner();
	assert(pPlayerActor != nullptr);

	// 프레임당 이동량으로 계산
	float deltaSeconds = GET_SYSTEM(FrameSystem)->GetDeltaSeconds();
	float deltaMove = pPlayerActor->GetSpeed() * deltaSeconds;

	// 플레이어 컴포넌트 가져옴
	TransformComponentPtr spTransformComponent = GET_COMPONENT(pPlayerActor, TransformComponent);
	Vec2d currentPos = spTransformComponent->GetPosition();
	Vec2d destPos = pPlayerActor->CalcDestinationWorldPosition();

	bool bRet = true;

	Vec2d moveDir = destPos - currentPos;
 	if (moveDir.LengthSquared() < 100.0f)
	{
		pPlayerActor->ApplyNextCellPositionToCurrentCellPosition();
		pPlayerActor->SetPlayerStateIndex(PlayerIdleStateId::s_id);
		bRet = false;
	}
	else
	{
		Vec2d nextPos = currentPos;

		switch (pPlayerActor->GetCurrentDirection())
		{
		case EPlayerDirection::Left:
			nextPos.x -= deltaMove;
			break;

		case EPlayerDirection::Right:
			nextPos.x += deltaMove;
			break;

		case EPlayerDirection::Up:
			nextPos.y -= deltaMove;
			break;

		case EPlayerDirection::Down:
			nextPos.y += deltaMove;
			break;
		}

		spTransformComponent->SetPosition(nextPos);
	}

	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////////////
PlayerStateEnd::~PlayerStateEnd()
{

}