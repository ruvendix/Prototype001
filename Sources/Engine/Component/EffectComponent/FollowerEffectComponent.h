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
	void SetTargetActor(const ActorPtr& spActor) { m_spTargetActor = spActor; }

private:
	ActorPtr m_spTargetActor = nullptr;
};