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

	const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor = GetWorldTileMapActor();
	spWorldTileMapActor->ShowWorldTileGuideShape(false);

	// 무기 공장 작동
	WeaponFactory::I()->Startup();

	// 플레이어 액터 추가
	//m_spLocalPlayerActor = CreateActorToScene<LocalPlayerActor>(EActorLayerType::Creature);
	//m_spLocalPlayerActor->SetWorldTileMapActor(spWorldTileMapActor);

	// 기본 뱀 액터 추가
	m_spSnakeActor = CreateActor<SnakeActor>(EActorLayerType::Creature);
	m_spSnakeActor->SetActorName("뱀");
	m_spSnakeActor->SetWorldTileMapActor(spWorldTileMapActor);

	// 적군들 자동 생성기
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

void GameScene::ParsingPacket_CreateLocalGamePlayer(const Protocol::S_CreateLocalGamePlayer& createLocalGamePlayerPacket)
{
	const Protocol::GameEntityInfo& localGamePlayerInfo = createLocalGamePlayerPacket.local_game_player_info();
	m_spLocalPlayerActor = CreateActorToScene<LocalPlayerActor>(EActorLayerType::Creature);
	m_spLocalPlayerActor->ApplyGamePlayerInfoFromServer(localGamePlayerInfo);

	// 카메라 등록하고 씬 렌더러의 메인 카메라 타겟으로 설정
	RegisterMainCameraActorToScene(m_spLocalPlayerActor);
}