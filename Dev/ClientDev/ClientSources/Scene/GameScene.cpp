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
	//WeaponFactory::I()->Startup();

	// �÷��̾� ���� �߰�
	//m_spLocalPlayerActor = CreateActorToScene<LocalPlayerActor>(EActorLayerType::Creature);
	//m_spLocalPlayerActor->SetWorldTileMapActor(spWorldTileMapActor);

	// �⺻ �� ���� �߰�
	m_spSnakeActor = CreateActor<SnakeActor>(EActorLayerType::Creature);
	m_spSnakeActor->SetActorName("��");
	m_spSnakeActor->SetWorldTileMapActor(spWorldTileMapActor);

	// ������ �ڵ� ������
	m_spEnemyRespawner = std::make_shared<EnemyRespawner>();
	m_spEnemyRespawner->Startup();
	m_spEnemyRespawner->SetMaxEnemyCount(0);
	m_spEnemyRespawner->AddPrototypeEnemyActor(m_spSnakeActor);
	m_spEnemyRespawner->RespawnEnemies(this);
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

GameEntityActorPtr GameScene::FindGameEntityActor(uint64 gameEntityId) const
{
	std::vector<GameEntityActorPtr> vecGameEntityActor;
	FindExactTypeActors<GameEntityActor>(EActorLayerType::Creature, vecGameEntityActor);
	if (vecGameEntityActor.empty() == true)
	{
		return nullptr;
	}

	for (const GameEntityActorPtr& spGameEntityActor : vecGameEntityActor)
	{
		if (spGameEntityActor->GetGameEntityId() == gameEntityId)
		{
			return spGameEntityActor;
		}
	}
	
	return nullptr;
}

void GameScene::ParsingPacket_LeaveGamePlayer(const Protocol::S_LeaveGame& leaveGamePacket)
{
	DEFAULT_TRACE_LOG("%lld ���� ����", leaveGamePacket.user_id());
	const Protocol::GameEntityInfo& leaveGamePlayerInfo = leaveGamePacket.game_player_info();
	const GameEntityActorPtr& spGamePlayerActor = FindGameEntityActor(leaveGamePlayerInfo.entity_id());
	if (spGamePlayerActor == nullptr)
	{
		return;
	}

	ReserveEraseActor(spGamePlayerActor);
}

void GameScene::ParsingPacket_CreateLocalGamePlayer(const Protocol::S_CreateLocalGamePlayer& createLocalGamePlayerPacket)
{
	const Protocol::GameEntityInfo& localGamePlayerInfo = createLocalGamePlayerPacket.local_game_player_info();
	m_spLocalPlayerActor = CreateActorToScene<LocalPlayerActor>(EActorLayerType::Creature);
	m_spLocalPlayerActor->SyncFromServer_GameEntityInfo(localGamePlayerInfo);

	// ī�޶� ����ϰ� �� �������� ���� ī�޶� Ÿ������ ����
	RegisterMainCameraActorToScene(m_spLocalPlayerActor);

	// ���� ��� ������ ����
	const std::shared_ptr<WorldBackgroundActor>& spWorldBackgroundActor = GetWorldBackgroundActor();
	spWorldBackgroundActor->SetActorFlagBitOn(EActorFlag::RenderingTarget);
}

void GameScene::ParsingPacket_CreateGameEntities(const Protocol::S_SyncGameEntities& syncGameEntities)
{
	int32 gameEntityCount = syncGameEntities.game_entities_size();
	for (int32 i = 0; i < gameEntityCount; ++i)
	{
		const Protocol::GameEntityInfo& gameEntityInfo = syncGameEntities.game_entities(i);
		switch (gameEntityInfo.entity_type())
		{
		case Protocol::EGameEntityType::Player:
		{
			// ���� �÷��̾�� ����
			if (m_spLocalPlayerActor->GetGameEntityId() == gameEntityInfo.entity_id())
			{
				continue;
			}

			const PlayerActorPtr& spPlayerActor = CreateActorToScene<PlayerActor>(EActorLayerType::Creature);
			spPlayerActor->SyncFromServer_GameEntityInfo(gameEntityInfo);
		}
		break;

		case Protocol::EGameEntityType::Monster:
		{

		}
		break;

		default:
			break;
		}
	}
}

void GameScene::ParsingPacket_SyncGamePlayer(const Protocol::S_SyncGamePlayer& syncGamePlayer)
{
	const Protocol::GameEntityInfo& gamePlayerInfo = syncGamePlayer.game_player_info();
	const GameEntityActorPtr& spGamePlayerActor = FindGameEntityActor(gamePlayerInfo.entity_id());
	if (spGamePlayerActor == nullptr)
	{
		return;
	}

	spGamePlayerActor->SyncFromServer_GameEntityInfo(gamePlayerInfo);
}

void GameScene::ParsingPacket_SyncGameEntityLookAtDirection(const Protocol::S_SyncGameEntityLookAtDir& syncGameEntityLookAtDir)
{
	const Protocol::GameEntityInfo& gameEntityInfo = syncGameEntityLookAtDir.game_player_info();
	const GameEntityActorPtr& spGameEntityActor = FindGameEntityActor(gameEntityInfo.entity_id());
	if (spGameEntityActor == nullptr)
	{
		return;
	}

	spGameEntityActor->SyncFromServer_GameEntityLookAtDirection(gameEntityInfo);
}

void GameScene::ParsingPacket_SyncGamePlayerMove(const Protocol::S_SyncGamePlayerMove& syncGamePlayerMove)
{
	const Protocol::GameEntityInfo& gamePlayerInfo = syncGamePlayerMove.game_player_info();
	const GameEntityActorPtr& spGamePlayerActor = FindGameEntityActor(gamePlayerInfo.entity_id());
	if (spGamePlayerActor == nullptr)
	{
		return;
	}

	spGamePlayerActor->SyncFromServer_GameEntityMove(gamePlayerInfo);
}