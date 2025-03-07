// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class LifeTimeEffectComponent : public Component
{
	DECLARE_COMPILETIME_ID;

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

public:
	void SetLifeTime(float lifeTime) { m_lifeTimer.SetTriggerTime(lifeTime); }

private:
	void OnLifeTimeEnd();

private:
	Timer m_lifeTimer;
};