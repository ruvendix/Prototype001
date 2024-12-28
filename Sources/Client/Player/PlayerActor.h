// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "PlayerEvent.h"

class WorldTileMapActor;

class PlayerActor : public CellActor
{
	DECLARE_PIMPL;
	DEFINE_EVENT_HANDLER;

	using Super = CellActor;

public:
	using Super::Super;
	virtual ~PlayerActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

	template <typename TPlayerState>
	void ReserveChangePlayerState()
	{
		PlayerStatePtr spNextPlayerState = std::make_shared<TPlayerState>(this);
		ReserveEvent<PlayerStateChangeEvent>(spNextPlayerState);
	}

	void ProcessPlayerInput();
	void ChangePlayerSprite(const std::string& strNextPlayerSprite);

	const std::string& FindPlayerIdleSpriteString(ESceneActorMoveDirection moveDir) const;
	const std::string& FindPlayerWalkSpriteString(ESceneActorMoveDirection moveDir) const;

	void SetWorldTileMapActor(const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor) { m_spWorldTileMapActor = spWorldTileMapActor; }

private:
	void OnLeftKeyDown();
	void OnRightKeyDown();
	void OnDownKeyDown();
	void OnUpKeyDown();

	void OnChangeState(const CallbackArgs& eventArgs);

private:
	PlayerStatePtr m_spPlayerState = nullptr;
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;
};