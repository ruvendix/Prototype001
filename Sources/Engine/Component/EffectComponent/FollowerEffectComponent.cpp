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
	if (m_spTargetActor == nullptr)
	{
		return false;
	}

	const Vec2d& vTargetPos = m_spTargetActor->BringPosition();
	GetOwner()->ApplyPosition(vTargetPos);

	return true;
}