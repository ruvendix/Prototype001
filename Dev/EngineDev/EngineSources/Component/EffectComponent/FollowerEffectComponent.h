// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class FollowerEffectComponent : public Component
{
	DECLARE_COMPILETIME_ID;

public:
	FollowerEffectComponent();
	virtual ~FollowerEffectComponent();

	virtual bool Update(float deltaSeconds) override;

public:
	void SetFollowTargetActor(const ActorPtr& spFollowTargetActor) { m_spFollowTargetActor = spFollowTargetActor; }

private:
	ActorPtr m_spFollowTargetActor = nullptr;
};