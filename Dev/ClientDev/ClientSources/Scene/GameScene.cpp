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
	//m_spLocalPlayerActor = CreateActorToScene<LocalPlayerActor>(EActorLayerType::Creature);
	//m_spLocalPlayerActor->SetWorldTileMapActor(spWorldTileMapActor);

	// ������ �ʿ��� ������?
	// ���� Id �̰� Ŀ��, �׸��� �⺻ ����, ������ ����

	// �⺻ �� ���� �߰�
	m_spSnakeActor = CreateActor<SnakeActor>(EActorLayerType::Creature);
	m_spSnakeActor->SetActorName("��");
	m_spSnakeActor->SetWorldTileMapActor(spWorldTileMapActor);

	// ������ �ڵ� ������
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
	DEFAULT_TRACE_LOG("%lld ���� ����", leaveGamePacket.user_id());
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

	// ī�޶� ����ϰ� �� �������� ���� ī�޶� Ÿ������ ����
	RegisterMainCameraActorToScene(m_spLocalPlayerActor);

	// ���� ��� ������ ����
	const std::shared_ptr<WorldBackgroundActor>& spWorldBackgroundActor = GetWorldBackgroundActor();
	spWorldBackgroundActor->SetActorFlagBitOn(EActorFlag::RenderingTarget);
}

void GameScene::ParsingPacket_SyncEntitiesPacket(const Protocol::S_SyncEntitiesPacket& syncEntitiesPacket)
{
	int32 networkPlayerCount = syncEntitiesPacket.players_info_size();
	for (int32 i = 0; i < networkPlayerCount; ++i)
	{
		const Protocol::NetworkEntityInfo& networkPlayerInfo = syncEntitiesPacket.players_info(i);

		// ���� �÷��̾�� ����
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

	// ���� �÷��̾�� ����
	if (m_spLocalPlayerActor->GetNetworkEntityId() == networkEntityInfo.entity_id())
	{
		return;
	}

	spNetworkEntityActor->SyncFromServer_NetworkEntityState(networkEntityInfo);
}

void GameScene::ParsingPacket_HitDamageToEntityPacket(const Protocol::S_HitDamageToEntityPacket& hitDamageToEntityPacket) const
{
	// �����ں��� ����ȭ
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