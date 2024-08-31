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
	// �Է� Ȯ���ؼ� �̵� ���·� ���� ���θ� ����
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
		// ���°� ����Ǹ� ������Ʈ ����
		GetOwner()->GetCurrentState()->Update();
	}
	
	return true;
}

// Idle ���¿����� �Է¸� �ް� �̵� ���·� ������ ���������� Ȯ��
bool PlayerIdleState::ProcessIdleInput()
{
	// �÷��̾� ������
	PlayerActor* pPlayerActor = GetOwner();
	assert(pPlayerActor != nullptr);

	// �Է� ó��
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

	// �̵��� �������� �˻�
	std::array<Int2d, TO_NUM(EPlayerDirection::Count)> arrDeltaCellMove =
	{
		Int2d{-1, 0},
		Int2d{1, 0},
		Int2d{0, -1},
		Int2d{0, 1},
	};

	// �ӵ� ������ ��Ÿ �̵��� ���
	Int2d nextCellPos = pPlayerActor->GetCurrentCellPosition() + arrDeltaCellMove[TO_NUM(playerNextDir)];

	// Ÿ�ϸ��� �����ͼ� Ȯ��
	TileMapPtr spTileMap = GET_SYSTEM(ResourceSystem)->GetTileMap();
	TilePtr spTile = spTileMap->FindTile(nextCellPos);
	if (spTile != nullptr) // nullptr�̸� �� �� ���� ��
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
		// ���°� ����Ǹ� ������Ʈ ����
		GetOwner()->GetCurrentState()->Update();
	}

	return true;
}

bool PlayerWalkState::ProcessWalkState()
{
	// �÷��̾� ������
	PlayerActor* pPlayerActor = GetOwner();
	assert(pPlayerActor != nullptr);

	// �����Ӵ� �̵������� ���
	float deltaSeconds = GET_SYSTEM(FrameSystem)->GetDeltaSeconds();
	float deltaMove = pPlayerActor->GetSpeed() * deltaSeconds;

	// �÷��̾� ������Ʈ ������
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