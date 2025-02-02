// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SnakeActor.h"

#include "Engine/Actor/WorldTileMapActor.h"
#include "Engine/Actor/AnimationActor/AnimationActorState.h"

class SnakeActor::Pimpl
{
	DEFINE_PIMPL(SnakeActor)

public:
	void LoadAndStartupSnakeSprite();
};

void SnakeActor::Pimpl::LoadAndStartupSnakeSprite()
{
	// 이걸 확실하게 맞추려면 인덱스마다 넣어야함
	m_pOwner->LoadActorLookAtTexture("Assets/Texture/Monster/Snake.bmp");

	// 상태마다 스프라이트를 미리 만들고
	m_pOwner->CreateActorStateLookAtDynamicSprites<AnimationActorIdleState>("SnakeIdle", true);

	// 스프라이트마다 세팅
	m_pOwner->AddActorStateKeyFrame<AnimationActorIdleState>(0, 3, 0, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtType::Down);
	m_pOwner->AddActorStateKeyFrame<AnimationActorIdleState>(0, 3, 1, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtType::Right);
	m_pOwner->AddActorStateKeyFrame<AnimationActorIdleState>(0, 3, 2, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtType::Left);
	m_pOwner->AddActorStateKeyFrame<AnimationActorIdleState>(0, 3, 3, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtType::Up);

	// 기본 스프라이트 설정 (SnakeIdleDown)
	DynamicSpriteComponent* pDynamicSpriteComponent = m_pOwner->AddComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	const DynamicSpritePtr& spDefaultPlayerDynamicSprite = m_pOwner->FindActorStateLookAtDynamicSprite<AnimationActorIdleState>(EActorLookAtType::Down);
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

	// 셀 크기는 48로 고정!
	ApplyCellPosition(6, 8);
#pragma endregion

	// 레이어 처리
	SetRenderingLayer(ERenderingLayerType::Creature);
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