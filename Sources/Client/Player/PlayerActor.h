// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class PlayerActor : public AnimationActor
{
	DECLARE_PIMPL;
	using Super = AnimationActor;

public:
	using Super::Super;
	virtual ~PlayerActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

private:
	void OnDirectionKeyHandler(const InputActionValue* pInputAction);
	void OnSpaceBarKeyHandler(const InputActionValue* pInputAction);
};