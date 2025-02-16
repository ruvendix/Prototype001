// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameScene.h"

#include "Engine/Actor/WorldTileMapActor.h"
#include "Client/World/WorldBackgroundActor.h"
#include "Client/Player/PlayerActor.h"
#include "Client/EnemyMonster/EnemyMonsterActor/SnakeActor.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Startup()
{
	Super::Startup();

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

	// �÷��̾� ���� �߰�
	m_spPlayerActor = CreateActorToScene<PlayerActor>(EActorLayerType::Creature);
	m_spPlayerActor->SetWorldTileMapActor(spWorldTileMapActor);

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

	// ī�޶� ����ϰ� �� �������� ���� ī�޶� Ÿ������ ����
 	RegisterMainCameraActorToScene(m_spPlayerActor);
}

bool GameScene::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	// �������� ������ ���� �����Ӻ��� Ȱ���� (�������� �ٷ� ����)
	m_spEnemyRespawner->Update(deltaSeconds);
	return true;
}

void GameScene::Cleanup()
{
	Super::Cleanup();
}