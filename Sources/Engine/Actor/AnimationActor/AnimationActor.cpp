// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "AnimationActor.h"

#include "AnimationActorState.h"

AnimationActor::ActorLookAtStringTable AnimationActor::g_actorLookAtStringTable =
{ 
	"Left",
	"Up",
	"Right",
	"Down"
};

const std::array<Position2d, TO_NUM(EActorLookAtDirection::Count)> AnimationActor::g_lookAtForwardCellPosTable =
{
	Position2d(-1, +0),
	Position2d(+0, -1),
	Position2d(+1, +0),
	Position2d(+0, +1),
};

AnimationActor::~AnimationActor()
{

}

void AnimationActor::Startup()
{
	Super::Startup();
	m_spAnimationActorState = std::make_shared<AnimationActorIdleState>(this);
}

bool AnimationActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	m_animationActorStateChangeEvent.ExcuteIfBound();
	m_spAnimationActorState->Update(deltaSeconds);

	return true;
}

bool AnimationActor::CheckMovingState() const
{
	return (IsSameAnimationActorState<AnimationActorWalkState>());
}

// 실수니까 정확히 체크
void AnimationActor::ApplyMoveDirectionToLookAtDirection(const Vector2d& vMoveDir)
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
void AnimationActor::LoadActorLookAtDirectionTexture(const std::string& strActorLookAtDirTexturePath)
{
	for (int32 i = 0; i < TO_NUM(EActorLookAtDirection::Count); ++i)
	{
		m_actorLookAtDirTexturePathTable[i] = strActorLookAtDirTexturePath;
	}

	ResourceMananger::I()->LoadTexture(strActorLookAtDirTexturePath);
}

void AnimationActor::LoadActorLookAtDirectionTexture(const std::string& strActorLookAtDirTexturePath, EActorLookAtDirection actorLookAtDir)
{
	m_actorLookAtDirTexturePathTable[TO_NUM(actorLookAtDir)] = strActorLookAtDirTexturePath;
	ResourceMananger::I()->LoadTexture(strActorLookAtDirTexturePath);
}

Position2d AnimationActor::CalculateForwardCellPosition() const
{
	Position2d forwardCellPos = (GetCellPosition() + g_lookAtForwardCellPosTable[TO_NUM(m_lookAtDir)]);
	return forwardCellPos;
}

void AnimationActor::OnChangeAnimationActorState(const AnimationActorStatePtr& spAnimationActorState)
{
	m_spAnimationActorState = spAnimationActorState;
	m_spAnimationActorState->Startup();
	DEFAULT_TRACE_LOG("애니메이션 액터 상태 변경! (지연)");
}