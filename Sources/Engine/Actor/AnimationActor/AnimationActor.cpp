// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "AnimationActor.h"

#include "AnimationActorState.h"

AnimationActor::ActorLookAtStringTable AnimationActor::s_actorLookAtStringTable = { "Left", "Up", "Right", "Down" };

AnimationActor::AnimationActor()
{

}

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
	m_spAnimationActorState->UpdateState(deltaSeconds);

	return true;
}

// 실수니까 정확히 체크
void AnimationActor::ApplyMoveDirection(const Vec2d& vMoveDir)
{
	if (math::CheckAlikeValue(vMoveDir.x, 1.0f) == true)
	{
		m_lookAtType = EActorLookAtType::Right;
	}
	else if (math::CheckAlikeValue(vMoveDir.x, -1.0f) == true)
	{
		m_lookAtType = EActorLookAtType::Left;
	}

	if (math::CheckAlikeValue(vMoveDir.y, 1.0f) == true)
	{
		m_lookAtType = EActorLookAtType::Down;
	}
	else if (math::CheckAlikeValue(vMoveDir.y, -1.0f) == true)
	{
		m_lookAtType = EActorLookAtType::Up;
	}
}

void AnimationActor::LoadActorLookAtTexture(const std::string& strActorLookAtTexturePath, EActorLookAtType actorLookAtType)
{
	m_actorLookAtTexturePathTable[TO_NUM(actorLookAtType)] = strActorLookAtTexturePath;
	ResourceMananger::I()->LoadTexture(strActorLookAtTexturePath);
}

void AnimationActor::OnChangeAnimationActorState(const AnimationActorStatePtr& spAnimationActorState)
{
	DEFAULT_TRACE_LOG("애니메이션 액터 상태 변경!");
	m_spAnimationActorState = spAnimationActorState;
}