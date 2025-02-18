// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SnakeActor.h"

#include "Client/EnemyMonster/EnemyMonsterState/EnemyMonsterState.h"

DEFINE_COMPILETIME_ID(SnakeActor, EnemeyActorIdCounter)

class SnakeActor::Pimpl
{
	DEFINE_PIMPL(SnakeActor)

public:
	void LoadAndStartupSnakeSprite();
};

void SnakeActor::Pimpl::LoadAndStartupSnakeSprite()
{
	// 이걸 확실하게 맞추려면 인덱스마다 넣어야함
	m_pOwner->LoadActorLookAtDirectionTexture("Assets/Texture/Monster/Snake.bmp");

	// 상태마다 스프라이트를 미리 만들고
	m_pOwner->CreateActorStateLookAtDynamicSprites<EnmeyMonsterIdleState>("SnakeIdle", true);

	// 스프라이트마다 설정
	m_pOwner->AddActorStateKeyFrame<EnmeyMonsterIdleState>(0, 3, 0, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtDirection::Down);
	m_pOwner->AddActorStateKeyFrame<EnmeyMonsterIdleState>(0, 3, 1, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtDirection::Right);
	m_pOwner->AddActorStateKeyFrame<EnmeyMonsterIdleState>(0, 3, 2, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtDirection::Left);
	m_pOwner->AddActorStateKeyFrame<EnmeyMonsterIdleState>(0, 3, 3, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtDirection::Up);

	// 기본 스프라이트 설정 (SnakeIdleDown)
	DynamicSpriteComponent* pDynamicSpriteComponent = m_pOwner->AddComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	const DynamicSpritePtr& spDefaultPlayerDynamicSprite = m_pOwner->FindActorStateLookAtDynamicSprite<EnmeyMonsterIdleState>(EActorLookAtDirection::Down);
	pDynamicSpriteComponent->ApplyDynamicSprite(spDefaultPlayerDynamicSprite);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
SnakeActor::~SnakeActor()
{
	Cleanup();
}

void SnakeActor::Startup()
{
	Super::Startup();
	ALLOC_PIMPL;

	// 스네이크 스프라이트 로딩 및 초기화
	m_spPimpl->LoadAndStartupSnakeSprite();

#pragma region 스네이크 기본 정보 초기화
	TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG(pTransformComponent != nullptr);

	// 좌표와 크기 설정
	//pTransformComponent->SetPosition(0.0f, 0.0f);
	pTransformComponent->SetSize(100, 100);

	CellActorMoveComponent* pCellActorMoveComponent = FindComponent<CellActorMoveComponent>();
	pCellActorMoveComponent->SetMoveSpeed(40.0f);
#pragma endregion
}

bool SnakeActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

void SnakeActor::Cleanup()
{
	return (Super::Cleanup());
}

ActorPtr SnakeActor::CreateClone()
{
	return std::make_shared<SnakeActor>(*this);
}

void SnakeActor::ProcessDamaged(const std::shared_ptr<PawnActor>& spAttacker)
{
	// 이펙트 생성
	Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);

	EffectSpawnInfo effectSpawnInfo;
	effectSpawnInfo.strEffectName = "OneTime_HitEffect";
	effectSpawnInfo.spawnCellPos = GetCellPosition();
	effectSpawnInfo.effectSize = Size(80, 80);
	pCurrentScene->ReserveCreateEffect(effectSpawnInfo);

	// 사망 루틴
	DecreaseEnemyCountToEnemyRespawner();
	pCurrentScene->ReserveEraseActor(shared_from_this());
	DEFAULT_TRACE_LOG("뱀 사망!");
}