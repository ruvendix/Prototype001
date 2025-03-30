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
	//m_spLocalPlayerActor = CreateActorToScene<LocalPlayerActor>(EActorLayerType::Creature);
	//m_spLocalPlayerActor->SetWorldTileMapActor(spWorldTileMapActor);

	// 서버가 필요한 정보는?
	// 몬스터 Id 이건 커먼, 그리고 기본 상태, 개수는 랜덤

	// 기본 뱀 액터 추가
	m_spSnakeActor = CreateActor<SnakeActor>(EActorLayerType::Creature);
	m_spSnakeActor->SetActorName("뱀");
	m_spSnakeActor->SetWorldTileMapActor(spWorldTileMapActor);

	// 적군들 자동 생성기
	m_spEnemyRespawner = std::make_shared<EnemyRespawner>();
	m_spEnemyRespawner->Startup();
	m_spEnemyRespawner->SetMaxEnemyCount(1);
	m_spEnemyRespawner->AddPrototypeEnemyActor(m_spSnakeActor);
	//m_spEnemyRespawner->RespawnEnemies(this);
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

NetworkEntityActorPtr GameScene::FindNetworkEntityActor(uint64 NetworkEntityId) const
{
	std::vector<NetworkEntityActorPtr> vecNetworkEntityActor;
	FindExactTypeActors<NetworkEntityActor>(EActorLayerType::Creature, vecNetworkEntityActor);
	if (vecNetworkEntityActor.empty() == true)
	{
		return nullptr;
	}

	for (const NetworkEntityActorPtr& spNetworkEntityActor : vecNetworkEntityActor)
	{
		if ((spNetworkEntityActor != nullptr) &&
			(spNetworkEntityActor->IsCreatedNetworkEntityInfo() == false))
		{
			continue;
		}

		if (spNetworkEntityActor->GetNetworkEntityId() == NetworkEntityId)
		{
			return spNetworkEntityActor;
		}
	}
	
	return nullptr;
}

void GameScene::ParsingPacket_LeaveGamePacket(const Protocol::S_LeaveGamePacket& leaveGamePacket)
{
	DEFAULT_TRACE_LOG("%lld 유저 나감", leaveGamePacket.user_id());
	const Protocol::NetworkEntityInfo& networkPlayerInfo = leaveGamePacket.player_info();
	const NetworkEntityActorPtr& spNetworkPlayerActor = FindNetworkEntityActor(networkPlayerInfo.entity_id());
	if (spNetworkPlayerActor == nullptr)
	{
		return;
	}

	ReserveEraseActor(spNetworkPlayerActor);
}

void GameScene::ParsingPacket_CreateMainPlayerPacket(const Protocol::S_CreateMainPlayerPacket& createMainPlayerPacket)
{
	const Protocol::NetworkEntityInfo& networkMainPlayerInfo = createMainPlayerPacket.main_player_info();
	m_spLocalPlayerActor = CreateActorToScene<LocalPlayerActor>(EActorLayerType::Creature);
	m_spLocalPlayerActor->SyncFromServer_NetworkEntityInfo(networkMainPlayerInfo);

	// 카메라 등록하고 씬 렌더러의 메인 카메라 타겟으로 설정
	RegisterMainCameraActorToScene(m_spLocalPlayerActor);

	// 월드 배경 렌더링 시작
	const std::shared_ptr<WorldBackgroundActor>& spWorldBackgroundActor = GetWorldBackgroundActor();
	spWorldBackgroundActor->SetActorFlagBitOn(EActorFlag::RenderingTarget);
}

void GameScene::ParsingPacket_SyncEntitiesPacket(const Protocol::S_SyncEntitiesPacket& syncEntitiesPacket)
{
	int32 networkPlayerCount = syncEntitiesPacket.players_info_size();
	for (int32 i = 0; i < networkPlayerCount; ++i)
	{
		const Protocol::NetworkEntityInfo& networkPlayerInfo = syncEntitiesPacket.players_info(i);

		// 로컬 플레이어는 제외
		if (m_spLocalPlayerActor->GetNetworkEntityId() == networkPlayerInfo.entity_id())
		{
			continue;
		}

		const PlayerActorPtr& spNetworkPlayerActor = CreateActorToScene<NetworkPlayerActor>(EActorLayerType::Creature);
		spNetworkPlayerActor->SyncFromServer_NetworkEntityInfo(networkPlayerInfo);
	}

	int32 networkMonsterCount = syncEntitiesPacket.monsters_info_size();
	for (int32 i = 0; i < networkMonsterCount; ++i)
	{
		const Protocol::NetworkMonsterInfo& networkMonsterInfo = syncEntitiesPacket.monsters_info(i);
		m_spEnemyRespawner->RespawnEnemy(networkMonsterInfo, this);
	}
}

void GameScene::ParsingPacket_ModifyPlayerInformationPacket(const Protocol::S_ModifyPlayerInformationPacket& modifyPlayerInformationPacket) const
{
	const Protocol::NetworkEntityInfo& networkPlayerInfo = modifyPlayerInformationPacket.player_info();
	const NetworkEntityActorPtr& networkPlayerActor = FindNetworkEntityActor(networkPlayerInfo.entity_id());
	if (networkPlayerActor == nullptr)
	{
		return;
	}

	networkPlayerActor->SyncFromServer_NetworkEntityInfo(networkPlayerInfo);
}

void GameScene::ParsingPacket_ModifyEntityLookAtDirectionPacket(const Protocol::S_ModifyEntityLookAtDirectionPacket& modifyEntityLookAtDirectionPacket) const
{
	const Protocol::NetworkEntityInfo& networkEntityInfo = modifyEntityLookAtDirectionPacket.entity_info();
	const NetworkEntityActorPtr& spNetworkEntityActor = FindNetworkEntityActor(networkEntityInfo.entity_id());
	if (spNetworkEntityActor == nullptr)
	{
		return;
	}

	spNetworkEntityActor->SyncFromServer_NetworkEntityLookAtDirection(networkEntityInfo);
}

void GameScene::ParsingPacket_MoveEntityPacket(const Protocol::S_MoveEntityPacket& moveEntityPacket) const
{
	const Protocol::NetworkEntityInfo& networkEntityInfo = moveEntityPacket.entity_info();
	const NetworkEntityActorPtr& spNetworkEntityActor = FindNetworkEntityActor(networkEntityInfo.entity_id());
	if (spNetworkEntityActor == nullptr)
	{
		return;
	}

	spNetworkEntityActor->SyncFromServer_NetworkEntityMove(networkEntityInfo);
}

void GameScene::ParsingPacket_ModifyEntityStatePacket(const Protocol::S_ModifyEntityStatePacket& modifyEntityStatePacket) const
{
	const Protocol::NetworkEntityInfo& networkEntityInfo = modifyEntityStatePacket.entity_info();
	const NetworkEntityActorPtr& spNetworkEntityActor = FindNetworkEntityActor(networkEntityInfo.entity_id());
	if (spNetworkEntityActor == nullptr)
	{
		return;
	}

	// 로컬 플레이어는 제외
	if (m_spLocalPlayerActor->GetNetworkEntityId() == networkEntityInfo.entity_id())
	{
		return;
	}

	spNetworkEntityActor->SyncFromServer_NetworkEntityState(networkEntityInfo);
}

void GameScene::ParsingPacket_HitDamageToEntityPacket(const Protocol::S_HitDamageToEntityPacket& hitDamageToEntityPacket) const
{
	// 공격자부터 동기화
	const Protocol::NetworkEntityInfo& networkAttackerInfo = hitDamageToEntityPacket.attacker_info();
	const NetworkEntityActorPtr& networkAttackerActor = FindNetworkEntityActor(networkAttackerInfo.entity_id());
	if (networkAttackerActor == nullptr)
	{
		return;
	}
	networkAttackerActor->SyncFromServer_NetworkEntityLookAtDirection(networkAttackerInfo);

	const Protocol::NetworkEntityInfo& networkVictimInfo = hitDamageToEntityPacket.victim_info();
	const NetworkEntityActorPtr& spNetworkVictim = FindNetworkEntityActor(networkVictimInfo.entity_id());
	if (spNetworkVictim == nullptr)
	{
		return;
	}

	spNetworkVictim->SyncFromServer_NetworkEntityHitDamage(networkAttackerActor, networkVictimInfo);
}

void GameScene::ParsingPacket_DiePlayerPacket(const Protocol::S_DiePlayerPacket& diePlayerPacket)
{
	const Protocol::NetworkEntityInfo& networkVictimInfo = diePlayerPacket.victim_info();
	const NetworkEntityActorPtr& networkVictimActor = FindNetworkEntityActor(networkVictimInfo.entity_id());
	if (networkVictimActor == nullptr)
	{
		return;
	}

	ReserveEraseActor(networkVictimActor);
}