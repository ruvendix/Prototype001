// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class PlayerImplementation;

class PlayerActor : public SceneActor
{
	DECLARE_PIMPL;
	using Super = SceneActor;

public:
	using Super::Super;
	virtual ~PlayerActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

	void ChangePlayerSprite(const std::string& strNextPlayerSprite);

private:
	void OnKeyboardDown_Left();
	void OnKeyboardDown_Right();
	void OnKeyboardDown_Down();
	void OnKeyboardDown_Up();

private:
	std::unique_ptr<PlayerImplementation> m_spPlayerImpl = nullptr;
};