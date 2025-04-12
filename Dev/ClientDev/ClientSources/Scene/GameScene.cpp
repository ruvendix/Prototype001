// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameScene.h"

#include "EngineSources/Actor/WorldTileMapActor.h"
#include "ClientSources/World/WorldBackgroundActor.h"
#include "ClientSources/Player/LocalPlayerActor.h"
#include "ClientSources/EnemyMonster/EnemyMonsterActor/SnakeActor.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Startup()
{
	Super::Startup();

	// ��Ʈ��ũ �۵�
	NetworkManager::I()->Startup();

#pragma region ����Ʈ �ε� �׽�Ʈ
	// ����Ʈ�� ���� ���̳��� ��������Ʈ
	const DynamicSpritePtr& spOneTimeHitEffectDynamicSprite = ResourceMananger::I()->CreateDynamicSprite("OneTime_HitEffect");
	spOneTimeHitEffectDynamicSprite->LoadAndSetTexture("Assets/Texture/Effect/Hit.bmp");
	spOneTimeHitEffectDynamicSprite->AddKeyFrames(0, 5, 0, Size(50, 47), RGB(0, 0, 0), 0.1f);

	const EffectPtr& spHitEffect = ResourceMananger::I()->CreateEffect("OneTime_HitEffect");
	spHitEffect->SetDynamicSprite(spOneTimeHitEffectDynamicSprite);
#pragma endregion

	const std::shared_ptr<WorldBackgroundActor>& spWorldBackgroundActor = GetWorldBackgroundActor();
	spWorldBackgroundActor->SetActorFlagBitOff(EActorFlag::RenderingTarget);

	const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor = GetWorldTileMapActor();
	spWorldTileMapActor->ShowWorldTileGuideShape(false);

	// ���� ���� �۵�
	WeaponFactory::I()->Startup();

	// �÷��̾� ���� �߰�
	//m_spLocalPlayer = CreateActorToScene<LocalPlayerActor>(EActorLayerType::Creature);
	//m_spLocalPlayer->SetWorldTileMapActor(spWorldTileMapActor);

	// ������ �ʿ��� ������?
	// ���� Id �̰� Ŀ��, �׸��� �⺻ ����, ������ ����

	// �⺻ �� ���� �߰�
	m_spSnakeActor = CreateActor<SnakeActor>(EActorLayerType::Creature);
	m_spSnakeActor->SetActorName("��");
	m_spSnakeActor->SetWorldTileMapActor(spWorldTileMapActor);

	// ������ ������ (�����κ��� ��û ���� ���� �۵�)
	m_spEnemyRespawner = std::make_shared<EnemyRespawner>();
	m_spEnemyRespawner->AddPrototypeEnemyActor(m_spSnakeActor);
}

bool GameScene::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	// ��Ʈ��ũ ó��
	NetworkManager::I()->Update(deltaSeconds);

	// �������� ������ ���� �����Ӻ��� Ȱ���� (�������� �ٷ� ����)
	m_spEnemyRespawner->Update(deltaSeconds);

	// ����� ����ü�� �̹� �����ӿ��� ����
	WeaponFactory::I()->Update(deltaSeconds);

	return true;
}

void GameScene::Cleanup()
{
	Super::Cleanup();
	NetworkManager::I()->Cleanup();
}

CommunicationActorPtr GameScene::FindEntityActor(uint64 entityId, EActorLayerType actorLayer) const
{
	std::vector<CommunicationActorPtr> vecCommunicationActor;
	FindExactTypeActors<CommunicationActor>(actorLayer, vecCommunicationActor);
	if (vecCommunicationActor.empty() == true)
	{
		return nullptr;
	}

	for (const CommunicationActorPtr& spCommunicationActor : vecCommunicationActor)
	{
		if ((spCommunicationActor != nullptr) &&
			(spCommunicationActor->IsCreatedEntityInfo() == false))
		{
			continue;
		}

		if (spCommunicationActor->GetEntityId() == entityId)
		{
			return spCommunicationActor;
		}
	}
	
	return nullptr;
}

void GameScene::ParsingPacket_LeaveGamePacket(const Protocol::S_LeaveGamePacket& leaveGamePacket)
{
	DEFAULT_TRACE_LOG("%lld ���� ����", leaveGamePacket.user_id());
	const Protocol::EntityInfo& playerInfo = leaveGamePacket.player_info();
	const CommunicationActorPtr& spPlayer = FindEntityActor(playerInfo.entity_id(), EActorLayerType::Creature);
	if (spPlayer == nullptr)
	{
		return;
	}

	ReserveEraseActor(spPlayer);
}

void GameScene::ParsingPacket_CreateMainPlayerPacket(const Protocol::S_CreateMainPlayerPacket& createMainPlayerPacket)
{
	const Protocol::EntityInfo& mainPlayerInfo = createMainPlayerPacket.main_player_info();
	m_spLocalPlayer = CreateActorToScene<LocalPlayerActor>(EActorLayerType::Creature);
	m_spLocalPlayer->SyncFromServer_EntityInfo(mainPlayerInfo);

	// ī�޶� ����ϰ� �� �������� ���� ī�޶� Ÿ������ ����
	RegisterMainCameraActorToScene(m_spLocalPlayer);

	// ���� ��� ������ ����
	const std::shared_ptr<WorldBackgroundActor>& spWorldBackgroundActor = GetWorldBackgroundActor();
	spWorldBackgroundActor->SetActorFlagBitOn(EActorFlag::RenderingTarget);
}

void GameScene::ParsingPacket_SyncEntitiesPacket(const Protocol::S_SyncEntitiesPacket& syncEntitiesPacket)
{
	int32 playerCount = syncEntitiesPacket.players_info_size();
	for (int32 i = 0; i < playerCount; ++i)
	{
		const Protocol::EntityInfo& playerInfo = syncEntitiesPacket.players_info(i);

		// ���� �÷��̾�� ����
		if (m_spLocalPlayer->GetEntityId() == playerInfo.entity_id())
		{
			continue;
		}

		const PlayerActorPtr& spPlayer = CreateActorToScene<NetworkPlayerActor>(EActorLayerType::Creature);
		spPlayer->SyncFromServer_EntityInfo(playerInfo);
	}

	int32 monsterCount = syncEntitiesPacket.monsters_info_size();
	for (int32 i = 0; i < monsterCount; ++i)
	{
		const Protocol::MonsterInfo& monsterInfo = syncEntitiesPacket.monsters_info(i);
		m_spEnemyRespawner->RespawnEnemy(monsterInfo, this);
	}
}

void GameScene::ParsingPacket_ModifyPlayerInformationPacket(const Protocol::S_ModifyPlayerInformationPacket& modifyPlayerInformationPacket) const
{
	const Protocol::EntityInfo& playerInfo = modifyPlayerInformationPacket.player_info();
	const CommunicationActorPtr& spPlayer = FindEntityActor(playerInfo.entity_id(), EActorLayerType::Creature);
	if (spPlayer == nullptr)
	{
		return;
	}

	spPlayer->SyncFromServer_EntityInfo(playerInfo);
}

void GameScene::ParsingPacket_ModifyEntityLookAtDirectionPacket(const Protocol::S_ModifyEntityLookAtDirectionPacket& modifyEntityLookAtDirectionPacket) const
{
	const Protocol::EntityInfo& EntityInfo = modifyEntityLookAtDirectionPacket.entity_info();
	const CommunicationActorPtr& spEntityActor = FindEntityActor(EntityInfo.entity_id(), EActorLayerType::Creature);
	if (spEntityActor == nullptr)
	{
		return;
	}

	spEntityActor->SyncFromServer_EntityLookAtDirection(EntityInfo);
}

void GameScene::ParsingPacket_MoveEntityPacket(const Protocol::S_MoveEntityPacket& moveEntityPacket) const
{
	const Protocol::EntityInfo& EntityInfo = moveEntityPacket.entity_info();
	const CommunicationActorPtr& spEntityActor = FindEntityActor(EntityInfo.entity_id(), EActorLayerType::Creature);
	if (spEntityActor == nullptr)
	{
		return;
	}

	spEntityActor->SyncFromServer_EntityMove(EntityInfo);
}

void GameScene::ParsingPacket_ModifyEntityStatePacket(const Protocol::S_ModifyEntityStatePacket& modifyEntityStatePacket) const
{
	const Protocol::EntityInfo& EntityInfo = modifyEntityStatePacket.entity_info();
	const CommunicationActorPtr& spEntityActor = FindEntityActor(EntityInfo.entity_id(), EActorLayerType::Creature);
	if (spEntityActor == nullptr)
	{
		return;
	}

	// ���� �÷��̾�� ����
	if (m_spLocalPlayer->GetEntityId() == EntityInfo.entity_id())
	{
		return;
	}

	spEntityActor->SyncFromServer_EntityState(EntityInfo);
}

void GameScene::ParsingPacket_HitDamageToEntityPacket(const Protocol::S_HitDamageToEntityPacket& hitDamageToEntityPacket) const
{
	// �����ں��� ����ȭ
	const Protocol::EntityInfo& attackerInfo = hitDamageToEntityPacket.attacker_info();
	const CommunicationActorPtr& spAttacker = FindEntityActor(attackerInfo.entity_id(), EActorLayerType::Creature);
	if (spAttacker == nullptr)
	{
		return;
	}

	// ĳ���Ͱ� �ƴ� ����
	if (attackerInfo.entity_type() != Protocol::EEntityType::Player)
	{
		spAttacker->SyncFromServer_EntityLookAtDirection(attackerInfo);
	}

	const Protocol::EntityInfo& victimInfo = hitDamageToEntityPacket.victim_info();
	const CommunicationActorPtr& spVictim = FindEntityActor(victimInfo.entity_id(), EActorLayerType::Creature);
	if (spVictim == nullptr)
	{
		return;
	}

	spVictim->SyncFromServer_EntityHitDamage(spAttacker, victimInfo);
}

void GameScene::ParsingPacket_DieEntityPacket(const Protocol::S_DieEntityPacket& dieEntityPacket)
{
	const Protocol::EntityInfo& victimInfo = dieEntityPacket.victim_info();

	EActorLayerType actorLayer = EActorLayerType::Creature;
	if (victimInfo.entity_type() == Protocol::EEntityType::Projectile)
	{
		actorLayer = EActorLayerType::WorldForeground;
	}

	const CommunicationActorPtr& spVictim = FindEntityActor(victimInfo.entity_id(), actorLayer);
	if (spVictim == nullptr)
	{
		return;
	}

	spVictim->ProcessDamaged(nullptr);
	ReserveEraseActor(spVictim);
}

void GameScene::ParsingPacket_CreateProjectilePacket(const Protocol::S_CreateProjectilePacket& createProjectilePacket)
{
	const Protocol::ProjectileInfo& projectileInfo = createProjectilePacket.projectile_info();
	const ProjectileActorPtr& spProjectile = WeaponFactory::I()->CreateProjectileActor(projectileInfo.projectile_id());
	spProjectile->InitializeActorStateTable();
	spProjectile->RegisterStateOnBidirectional();

	const Protocol::EntityInfo& entityInfo = projectileInfo.entity_info();
	spProjectile->SyncFromServer_EntityInfo(entityInfo);

	// �̵��� ���� ���ϱ�
	const Position2d& moveDir = (spProjectile->CalculateForwardCellPosition() - spProjectile->GetCellPosition());
	const Vector2d& vMoveDir = Vector2d(static_cast<float>(moveDir.x), static_cast<float>(moveDir.y));

	ProjectileSpawnInfo projectileSpawnInfo;
	projectileSpawnInfo.projectileSize = Size{ 100, 100 };
	projectileSpawnInfo.vMoveDir = vMoveDir;
	projectileSpawnInfo.flyingSpeed = 1000.0f;

	spProjectile->SpawnProjectile(projectileSpawnInfo);
	DEFAULT_TRACE_LOG("����ü ����!");
}