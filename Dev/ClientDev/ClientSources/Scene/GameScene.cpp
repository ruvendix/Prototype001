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

	const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor = GetWorldTileMapActor();
	spWorldTileMapActor->ShowWorldTileGuideShape(false);

	// ���� ���� �۵�
	WeaponFactory::I()->Startup();

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
	m_spEnemyRespawner->SetMaxEnemyCount(20);
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

void GameScene::ParsingPacket_CreateLocalGamePlayer(const Protocol::S_CreateLocalGamePlayer& createLocalGamePlayerPacket)
{
	const Protocol::GameEntityInfo& localGamePlayerInfo = createLocalGamePlayerPacket.local_game_player_info();
	m_spLocalPlayerActor = CreateActorToScene<LocalPlayerActor>(EActorLayerType::Creature);
	m_spLocalPlayerActor->ApplyGamePlayerInfoFromServer(localGamePlayerInfo);

	// ī�޶� ����ϰ� �� �������� ���� ī�޶� Ÿ������ ����
	RegisterMainCameraActorToScene(m_spLocalPlayerActor);
}