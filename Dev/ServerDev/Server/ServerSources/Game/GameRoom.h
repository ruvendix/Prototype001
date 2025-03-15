// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

DECLARE_COMPILETIMER_COUNTER(GameEntityIdCounter);

class WorldTileMapActor;

class GameRoom : public EnableSharedClass
{
	DECLARE_SINGLETON(GameRoom)

public:
	virtual void Startup() override;
	virtual void Cleanup() override;

public:
	void EnterGameRoom(const RxGameSessionPtr& spGameSession);
	void LeaveGameRoom(const RxGameSessionPtr& spGameSession);

	void SyncGameEntities(const RxGameSessionPtr& spGameSession);

	void AddGameEntity(const GameEntityPtr& spGameEntity);
	void RemoveGameEntity(const GameEntityPtr& spGameEntity);

	void ParsingPacket_SyncGameEntityLookAtDirection(const Protocol::C_SyncGameEntityLookAtDir& syncGameEntityLookAtDir);
	void ParsingPacket_SyncGamePlayerMove(const Protocol::C_SyncGamePlayerMove& syncGamePlayerMove);

private:
	GamePlayerPtr FindGamePlayer(uint64 entityId) const;

private:
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;
	std::unordered_map<uint64, GamePlayerPtr> m_mapGamePlayer;
	std::unordered_map<uint64, GameEntityPtr> m_mapGameMonster;
};