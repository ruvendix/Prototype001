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

	// 네트워크 작동
	NetworkManager::I()->Startup();

#pragma region 이펙트 로딩 테스트
	// 이펙트를 위한 다이나믹 스프라이트
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

	// 무기 공장 작동
	WeaponFactory::I()->Startup();

	// 플레이어 액터 추가
	//m_spLocalPlayer = CreateActorToScene<LocalPlayerActor>(EActorLayerType::Creature);
	//m_spLocalPlayer->SetWorldTileMapActor(spWorldTileMapActor);

	// 서버가 필요한 정보는?
	// 몬스터 Id 이건 커먼, 그리고 기본 상태, 개수는 랜덤

	// 기본 뱀 액터 추가
	m_spSnakeActor = CreateActor<SnakeActor>(EActorLayerType::Creature);
	m_spSnakeActor->SetActorName("뱀");
	m_spSnakeActor->SetWorldTileMapActor(spWorldTileMapActor);

	// 적군들 생성기 (서버로부터 요청 받을 때만 작동)
	m_spEnemyRespawner = std::make_shared<EnemyRespawner>();
	m_spEnemyRespawner->AddPrototypeEnemyActor(m_spSnakeActor);
}

bool GameScene::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	// 네트워크 처리
	NetworkManager::I()->Update(deltaSeconds);

	// 리스폰된 적군은 다음 프레임부터 활동함 (렌더링은 바로 개시)
	m_spEnemyRespawner->Update(deltaSeconds);

	// 예약된 투사체는 이번 프레임에서 생성
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
	DEFAULT_TRACE_LOG("%lld 유저 나감", leaveGamePacket.user_id());
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

	// 카메라 등록하고 씬 렌더러의 메인 카메라 타겟으로 설정
	RegisterMainCameraActorToScene(m_spLocalPlayer);

	// 월드 배경 렌더링 시작
	const std::shared_ptr<WorldBackgroundActor>& spWorldBackgroundActor = GetWorldBackgroundActor();
	spWorldBackgroundActor->SetActorFlagBitOn(EActorFlag::RenderingTarget);
}

void GameScene::ParsingPacket_SyncEntitiesPacket(const Protocol::S_SyncEntitiesPacket& syncEntitiesPacket)
{
	int32 playerCount = syncEntitiesPacket.players_info_size();
	for (int32 i = 0; i < playerCount; ++i)
	{
		const Protocol::EntityInfo& playerInfo = syncEntitiesPacket.players_info(i);

		// 로컬 플레이어는 제외
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

	// 로컬 플레이어는 제외
	if (m_spLocalPlayer->GetEntityId() == EntityInfo.entity_id())
	{
		return;
	}

	spEntityActor->SyncFromServer_EntityState(EntityInfo);
}

void GameScene::ParsingPacket_HitDamageToEntityPacket(const Protocol::S_HitDamageToEntityPacket& hitDamageToEntityPacket) const
{
	// 공격자부터 동기화
	const Protocol::EntityInfo& attackerInfo = hitDamageToEntityPacket.attacker_info();
	const CommunicationActorPtr& spAttacker = FindEntityActor(attackerInfo.entity_id(), EActorLayerType::Creature);
	if (spAttacker == nullptr)
	{
		return;
	}

	// 캐릭터가 아닐 때만
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

	// 이동할 방향 구하기
	const Position2d& moveDir = (spProjectile->CalculateForwardCellPosition() - spProjectile->GetCellPosition());
	const Vector2d& vMoveDir = Vector2d(static_cast<float>(moveDir.x), static_cast<float>(moveDir.y));

	ProjectileSpawnInfo projectileSpawnInfo;
	projectileSpawnInfo.projectileSize = Size{ 100, 100 };
	projectileSpawnInfo.vMoveDir = vMoveDir;
	projectileSpawnInfo.flyingSpeed = 1000.0f;

	spProjectile->SpawnProjectile(projectileSpawnInfo);
	DEFAULT_TRACE_LOG("투사체 생성!");
}