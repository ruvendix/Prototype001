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
	void RespawnMonsters();

	void EnterGameRoom(const RxGameSessionPtr& spGameSession);
	void LeaveGameRoom(const RxGameSessionPtr& spGameSession);

	void SyncGameEntities(const RxGameSessionPtr& spGameSession);

	void AddGameEntity(const GameEntityPtr& spEntity, bool bBroadcast);
	void RemoveGameEntity(const GameEntityPtr& spEntity);

	void ParsingPacket_MoveEntityPacket(const Protocol::C_MoveEntityPacket& moveEntityPacket) const;
	void ParsingPacket_ModifyEntityLookAtDirectionPacket(const Protocol::C_ModifyEntityLookAtDirectionPacket& modifyEntityLookAtDirectionPacket) const;
	void ParsingPacket_ModifyEntityStatePacket(const Protocol::C_ModifyEntityStatePacket& modifyEntityStatePacket) const;
	void ParsingPacket_AttackToEntityPacket(const Protocol::C_AttckToEntityPacket& attckToEntityPacket) const;
	void ParsingPacket_CreateProjectilePacket(const Protocol::C_CreateProjectilePacket& createProjectilePacket);
	void ParsingPacket_DieEntityPacket(const Protocol::C_DieEntityPacket& dieEntityPacket);

	Position2d GenerateRandomCellPosition() const;
	bool CheckCanMoveToCellPosition(const Position2d& destCellPos, const GameEntityPtr& spExcludeEntity) const;

	GameEntityPtr FindGameEntity(const Protocol::EntityInfo& entityInfo) const;
	GameEntityPtr FindGameEntity(const Position2d& targetCellPos) const;

	GamePlayerPtr FindGamePlayer(uint64 entityId) const;
	GameMonsterPtr FindGameMonster(uint64 entityId) const;
	GameProjectilePtr FindGameProjectile(uint64 entityId) const;

	int32 CalculateVictimDamage(const Protocol::EntityInfo& attackerInfo, const Protocol::EntityInfo& victimInfo) const;

	const std::unordered_map<uint64, GamePlayerPtr>& GetGamePlayers() const { return m_mapPlayer; }

private:
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;
	std::unordered_map<uint64, GamePlayerPtr> m_mapPlayer;
	std::unordered_map<uint64, GameMonsterPtr> m_mapMonster;
	std::unordered_map<uint64, GameProjectilePtr> m_mapProjectile;

	Timer m_respawnTimer;
	uint32 m_currentMonsterCount = 0;
};