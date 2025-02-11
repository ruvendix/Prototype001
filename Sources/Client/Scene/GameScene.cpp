// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameScene.h"

#include "Engine/Actor/WorldTileMapActor.h"
#include "Client/World/WorldBackgroundActor.h"
#include "Client/Player/PlayerActor.h"
#include "Client/EnemyMonster/SnakeActor.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Startup()
{
	Super::Startup();

#pragma region 이펙트 로딩 테스트
	// 이펙트를 위한 다이나믹 스프라이트
	const DynamicSpritePtr& spOneTimeHitEffectDynamicSprite = ResourceMananger::I()->CreateDynamicSprite("OneTime_HitEffect");
	spOneTimeHitEffectDynamicSprite->LoadAndSetTexture("Assets/Texture/Effect/Hit.bmp");
	spOneTimeHitEffectDynamicSprite->AddKeyFrames(0, 5, 0, Size(50, 47), RGB(0, 0, 0), 0.1f);

	const EffectPtr& spHitEffect = ResourceMananger::I()->CreateEffect("OneTime_HitEffect");
	spHitEffect->SetDynamicSprite(spOneTimeHitEffectDynamicSprite);
#pragma endregion

#pragma region 월드맵 처리
	// 월드맵 액터 추가
	m_spWorldBackgroundActor = CreateActorToScene<WorldBackgroundActor>(EActorLayerType::WorldBackground);

	// 셀 정보 넣기
	WorldContext::I()->SetCellSize(48);

	// 셀 정보에 따른 타일맵 구성
	m_spWorldTileMapActor = CreateActorToScene<WorldTileMapActor>(EActorLayerType::WorldForeground);
	m_spWorldTileMapActor->ShowWorldTileGuideShape(false);
#pragma endregion

	// 플레이어 액터 추가
	m_spPlayerActor = CreateActorToScene<PlayerActor>(EActorLayerType::Creature);
	m_spPlayerActor->SetWorldTileMapActor(m_spWorldTileMapActor);

	// 기본 뱀 액터 추가
	m_spSnakeActor = CreateActor<SnakeActor>(EActorLayerType::Creature);
	m_spSnakeActor->SetActorName("뱀");
	m_spSnakeActor->SetWorldTileMapActor(m_spWorldTileMapActor);

	// 적군들 자동 생성기
	m_spEnemyRespawner = std::make_shared<EnemyRespawner>();
	m_spEnemyRespawner->Startup();
	m_spEnemyRespawner->SetMaxEnemyCount(10);
	m_spEnemyRespawner->AddPrototypeEnemyActor(m_spSnakeActor);
	m_spEnemyRespawner->RespawnEnemies(this);

	// 카메라 등록하고 씬 렌더러의 메인 카메라 타겟으로 설정
 	RegisterMainCameraActorToScene(m_spPlayerActor);
}

bool GameScene::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	// 리스폰된 적군은 다음 프레임부터 활동함 (렌더링은 바로 개시)
	m_spEnemyRespawner->Update(deltaSeconds);
	return true;
}

void GameScene::Cleanup()
{
	Super::Cleanup();
}

bool GameScene::CheckCanMoveToCellPosition(const Position2d& destCellPos) const
{
	if (m_spWorldTileMapActor->CheckMovingAvailableTile(destCellPos) == false)
	{
		return false;
	}

	const ActorPtr& spFoundActor = FindAnyCellActor(EActorLayerType::Creature, destCellPos);
	if (spFoundActor != nullptr)
	{
		return false;
	}

	return true;
}