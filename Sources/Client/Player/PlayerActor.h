// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class WorldTileMapActor;

// 일단은 클라이언트에 있는 걸 사용
class PlayerActor : public CellActor
{
	DECLARE_PIMPL;

	using Super = CellActor;

public:
	using Super::Super;
	virtual ~PlayerActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	template <typename TPlayerState>
	void ReserveNextPlayerState()
	{
		PlayerStatePtr spNextPlayerState = std::make_shared<TPlayerState>(this);
		m_playerStateChangeEvent.RegisterEventHandler(this, &PlayerActor::OnChangePlayerState, spNextPlayerState);
	}

public:
	void ChangePlayerSprite(const std::string& strNextPlayerSprite);

	const std::string& FindPlayerIdleSpriteString(ESceneActorMoveDirection moveDir) const;
	const std::string& FindPlayerWalkSpriteString(ESceneActorMoveDirection moveDir) const;

	void SetWorldTileMapActor(const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor) { m_spWorldTileMapActor = spWorldTileMapActor; }

private:
	void OnChangePlayerState(const PlayerStatePtr& spNextPlayerState);
	void OnDirectionKeyHandler(const InputActionValue* pInputAction);

private:
	PlayerStatePtr m_spPlayerState = nullptr;
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;

	Event<const PlayerStatePtr& /* spNextPlayerState */> m_playerStateChangeEvent;
};