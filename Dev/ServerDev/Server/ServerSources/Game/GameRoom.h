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

	void AddGameEntity(const GameEntityPtr& spEntity, bool bBroadcast);
	void RemoveGameEntity(const GameEntityPtr& spEntity);

	void ParsingPacket_MoveEntityPacket(const Protocol::C_MoveEntityPacket& moveEntityPacket) const;
	void ParsingPacket_ModifyEntityLookAtDirectionPacket(const Protocol::C_ModifyEntityLookAtDirectionPacket& modifyEntityLookAtDirectionPacket) const;
	void ParsingPacket_ModifyEntityStatePacket(const Protocol::C_ModifyEntityStatePacket& modifyEntityStatePacket) const;

	Position2d GenerateRandomCellPosition() const;
	bool CheckCanMoveToCellPosition(const Position2d& destCellPos, const GameEntityPtr& spExcludeEntity) const;

	GameEntityPtr FindGameEntity(const Protocol::NetworkEntityInfo& entityInfo) const;
	GameEntityPtr FindGameEntity(const Position2d& targetCellPos) const;

	GamePlayerPtr FindGamePlayer(uint64 entityId) const;
	GameMonsterPtr FindGameMonster(uint64 entityId) const;

	const std::unordered_map<uint64, GamePlayerPtr>& GetGamePlayers() const { return m_mapPlayer; }

private:
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;
	std::unordered_map<uint64, GamePlayerPtr> m_mapPlayer;
	std::unordered_map<uint64, GameMonsterPtr> m_mapMonster;
};