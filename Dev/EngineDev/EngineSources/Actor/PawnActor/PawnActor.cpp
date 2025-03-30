// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PawnActor.h"

#include "PawnActorState.h"

PawnActor::ActorLookAtStringTable PawnActor::g_actorLookAtStringTable =
{ 
	"Left",
	"Up",
	"Right",
	"Down"
};

const std::array<Position2d, TO_NUM(EActorLookAtDirection::Count)> PawnActor::g_lookAtForwardCellPosTable =
{
	Position2d(-1, +0),
	Position2d(+0, -1),
	Position2d(+1, +0),
	Position2d(+0, +1),
};

const std::array<Position2d, TO_NUM(EActorLookAtDirection::Count)> PawnActor::g_lookAtBackwardCellPosTable =
{
	Position2d(+1, +0),
	Position2d(+0, +1),
	Position2d(-1, +0),
	Position2d(+0, -1),
};

const std::array<EActorLookAtDirection, TO_NUM(EActorLookAtDirection::Count)> PawnActor::g_oppositeLookAtDirTable =
{
	EActorLookAtDirection::Right,
	EActorLookAtDirection::Down,
	EActorLookAtDirection::Left,
	EActorLookAtDirection::Up,
};

PawnActor::PawnActor(const PawnActor& src) : Super(src)
{
	m_lookAtDir = src.m_lookAtDir;
	m_spWorldTileMapActor = src.m_spWorldTileMapActor;	
	m_actorLookAtDirTexturePathTable = src.m_actorLookAtDirTexturePathTable;
	m_vecActorStateDynamicSpriteTable = src.m_vecActorStateDynamicSpriteTable;
	DEFAULT_TRACE_LOG("테스트!");
}

PawnActor::~PawnActor()
{

}

void PawnActor::Startup()
{
	Super::Startup();
	InitializeActorStateTable();
}

bool PawnActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	m_pawnActorStateChangeEvent.ExcuteIfBound();

	const PawnActorStatePtr& spCurrentActorState = GetCurrentPawnActorState();
	if (spCurrentActorState != nullptr)
	{
		spCurrentActorState->Update(deltaSeconds);
	}

	return true;
}

void PawnActor::ProcessDamaged(const std::shared_ptr<PawnActor>& spAttacker)
{

}

void PawnActor::InitializeActorStateTable()
{
	m_vecActorStateTable.resize(GET_COMPILEITME_ID_COUNT(PawnActorStateIdCounter));

	RegisterActorState<PawnActorIdleState>();
	RegisterActorState<PawnActorWalkState>();
	RegisterActorState<PawnActorAttackState>();
}

bool PawnActor::ApplyLookAtDirectionSprite()
{
	DynamicSpriteComponent* pDynamicSpriteComponent = GetComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	DynamicSpritePtr spChangeActorStateDynamicSprite = FindCurrentActorStateLookAtDynamicSprite(m_lookAtDir);
	if (spChangeActorStateDynamicSprite != nullptr)
	{
		pDynamicSpriteComponent->ApplyDynamicSprite(spChangeActorStateDynamicSprite);
		return true;
	}

	return false;
}

bool PawnActor::ApplyLookAtDirectionSpriteOnDefaultState()
{
	if (m_vecActorStateDynamicSpriteTable.empty() == true)
	{
		DETAIL_ERROR_LOG(EngineErrorHandler, EEngineErrorCode::EmptyLookAtDirectionSprite);
		return false;
	}

	DynamicSpriteComponent* pDynamicSpriteComponent = GetComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	const ActorLookAtDynamicSpriteTable& actorLookAtDynmaicSpriteTable = m_vecActorStateDynamicSpriteTable[0];
	DynamicSpritePtr spChangeActorStateDynamicSprite = actorLookAtDynmaicSpriteTable[TO_NUM(m_lookAtDir)];
	if (spChangeActorStateDynamicSprite != nullptr)
	{
		pDynamicSpriteComponent->ApplyDynamicSprite(spChangeActorStateDynamicSprite);
		return true;
	}

	return false;
}

DynamicSpritePtr PawnActor::FindCurrentActorStateLookAtDynamicSprite(EActorLookAtDirection actorLookAtDir) const
{
	const PawnActorStatePtr& spCurrentActorState = GetCurrentPawnActorState();
	if (spCurrentActorState == nullptr)
	{
		return nullptr;
	}

	const ActorLookAtDynamicSpriteTable& actorLookAtDynmaicSpriteTable = m_vecActorStateDynamicSpriteTable[spCurrentActorState->CompiletimeId()];
	return (actorLookAtDynmaicSpriteTable[TO_NUM(actorLookAtDir)]);
}

// 실수니까 정확히 체크
void PawnActor::ApplyMoveDirectionToLookAtDirection(const Vector2d& vMoveDir)
{
	if (math::CheckAlikeValue(vMoveDir.x, 1.0f) == true)
	{
		m_lookAtDir = EActorLookAtDirection::Right;
	}
	else if (math::CheckAlikeValue(vMoveDir.x, -1.0f) == true)
	{
		m_lookAtDir = EActorLookAtDirection::Left;
	}

	if (math::CheckAlikeValue(vMoveDir.y, 1.0f) == true)
	{
		m_lookAtDir = EActorLookAtDirection::Down;
	}
	else if (math::CheckAlikeValue(vMoveDir.y, -1.0f) == true)
	{
		m_lookAtDir = EActorLookAtDirection::Up;
	}
}

// 방향마다 텍스처 분리되지 않은 경우
void PawnActor::LoadActorLookAtDirectionTexture(const std::string& strActorLookAtDirTexturePath)
{
	for (int32 i = 0; i < TO_NUM(EActorLookAtDirection::Count); ++i)
	{
		m_actorLookAtDirTexturePathTable[i] = strActorLookAtDirTexturePath;
	}

	ResourceMananger::I()->LoadTexture(strActorLookAtDirTexturePath);
}

void PawnActor::LoadActorLookAtDirectionTexture(const std::string& strActorLookAtDirTexturePath, EActorLookAtDirection actorLookAtDir)
{
	m_actorLookAtDirTexturePathTable[TO_NUM(actorLookAtDir)] = strActorLookAtDirTexturePath;
	ResourceMananger::I()->LoadTexture(strActorLookAtDirTexturePath);
}

void PawnActor::ImmediatelyChangeState(uint32 actorStateId)
{
	ASEERT_VALIDATE_INDEX(actorStateId, m_vecActorStateTable.size());
	m_currentActorStateIdx = actorStateId;

	const PawnActorStatePtr& spActorState = GetPawnActorState(actorStateId);
	ASSERT_LOG(spActorState != nullptr);
	spActorState->Startup();
}

void PawnActor::ChangeActorDynamicSpriteByExternal(const DynamicSpritePtr& spChangedDynamicSprite)
{
	DynamicSpriteComponent* pDynamicSpriteComponent = GetComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
	pDynamicSpriteComponent->ApplyDynamicSprite(spChangedDynamicSprite);
}

Position2d PawnActor::CalculateForwardCellPosition() const
{
	Position2d forwardCellPos = (GetCellPosition() + g_lookAtForwardCellPosTable[TO_NUM(m_lookAtDir)]);
	return forwardCellPos;
}

Position2d PawnActor::CalculateBackwardCellPosition() const
{
	Position2d backwardCellPos = (GetCellPosition() + g_lookAtBackwardCellPosTable[TO_NUM(m_lookAtDir)]);
	return backwardCellPos;
}

Vector2d PawnActor::CalculateBackwardDirection() const
{
	const Position2d& backwardCellPos = g_lookAtBackwardCellPosTable[TO_NUM(m_lookAtDir)];
	Vector2d vBackwardDir{ static_cast<float>(backwardCellPos.x), static_cast<float>(backwardCellPos.y) };
	return vBackwardDir;
}

bool PawnActor::CheckPossibleKnockback(EActorLookAtDirection srcLookAtDir) const
{
	return (m_lookAtDir == g_oppositeLookAtDirTable[TO_NUM(srcLookAtDir)]);
}

void PawnActor::OnChangePawnActorState(uint32 nextActorStateIdx)
{
	ASEERT_VALIDATE_INDEX(nextActorStateIdx, m_vecActorStateTable.size());
	m_currentActorStateIdx = nextActorStateIdx;

	const PawnActorStatePtr& spCurrentActorState = GetCurrentPawnActorState();
	if (spCurrentActorState != nullptr)
	{
		spCurrentActorState->Startup();
	}

	DEFAULT_TRACE_LOG("애니메이션 액터 상태 변경! (지연)");
}