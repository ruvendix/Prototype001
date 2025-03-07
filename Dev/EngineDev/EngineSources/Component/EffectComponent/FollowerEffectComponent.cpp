// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "FollowerEffectComponent.h"

DEFINE_COMPILETIME_ID(FollowerEffectComponent, ComponentIdCounter)

FollowerEffectComponent::FollowerEffectComponent()
{

}

FollowerEffectComponent::~FollowerEffectComponent()
{

}

bool FollowerEffectComponent::Update(float deltaSeconds)
{
	if (m_spFollowTargetActor == nullptr)
	{
		return false;
	}

	const Vector2d& vTargetPos = m_spFollowTargetActor->BringPosition();
	GetOwner()->ApplyPosition(vTargetPos);

	return true;
}