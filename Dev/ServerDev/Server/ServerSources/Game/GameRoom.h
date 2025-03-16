// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

DECLARE_COMPILETIMER_COUNTER(GameEntityIdCounter);

class WorldTileMapActor;

class GameRoom : public EnableSharedClass
{
	DECLARE_SINGLETON(GameRoom)

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	void InitializeMonsters();

	void EnterGameRoom(const RxGameSessionPtr& spGameSession);
	void LeaveGameRoom(const RxGameSessionPtr& spGameSession);

	void SyncGameEntities(const RxGameSessionPtr& spGameSession);

	void AddGameEntity(const GameEntityPtr& spGameEntity, bool bBroadcast);
	void RemoveGameEntity(const GameEntityPtr& spGameEntity);

	void ParsingPacket_SyncGameEntityLookAtDirection(const Protocol::C_SyncGameEntityLookAtDir& syncGameEntityLookAtDir);
	void ParsingPacket_SyncGameEntityMove(const Protocol::C_SyncGameEntityMove& syncGameEntityMove);
	void ParsingPacket_SyncGameEntityState(const Protocol::C_SyncGameEntityState& syncGameEntityState);

	Position2d GenerateRandomCellPosition();
	bool CheckCanMoveToCellPosition(const Position2d& destCellPos, const GameEntityPtr& spExcludeGameEntity) const;

private:
	GameEntityPtr FindGameEntity(const Protocol::GameEntityInfo& gameEntityInfo) const;
	GameEntityPtr FindGameEntity(const Position2d& targetCellPos) const;
	GamePlayerPtr FindGamePlayer(uint64 entityId) const;
	GameMonsterPtr FindGameMonster(uint64 entityId) const;

private:
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;
	std::unordered_map<uint64, GamePlayerPtr> m_mapGamePlayer;
	std::unordered_map<uint64, GameMonsterPtr> m_mapGameMonster;
};