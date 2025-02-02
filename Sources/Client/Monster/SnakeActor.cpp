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
	// �̰� Ȯ���ϰ� ���߷��� �ε������� �־����
	m_pOwner->LoadActorLookAtTexture("Assets/Texture/Monster/Snake.bmp");

	// ���¸��� ��������Ʈ�� �̸� �����
	m_pOwner->CreateActorStateLookAtDynamicSprites<AnimationActorIdleState>("SnakeIdle", true);

	// ��������Ʈ���� ����
	m_pOwner->AddActorStateKeyFrame<AnimationActorIdleState>(0, 3, 0, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtType::Down);
	m_pOwner->AddActorStateKeyFrame<AnimationActorIdleState>(0, 3, 1, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtType::Right);
	m_pOwner->AddActorStateKeyFrame<AnimationActorIdleState>(0, 3, 2, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtType::Left);
	m_pOwner->AddActorStateKeyFrame<AnimationActorIdleState>(0, 3, 3, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtType::Up);

	// �⺻ ��������Ʈ ���� (SnakeIdleDown)
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

	// ������ũ ��������Ʈ �ε� �� �ʱ�ȭ
	m_spPimpl->LoadAndStartupSnakeSprite();

#pragma region ������ũ �⺻ ���� �ʱ�ȭ
	TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG(pTransformComponent != nullptr);

	// ��ǥ�� ũ�� ����
	//pTransformComponent->SetPosition(0.0f, 0.0f);
	pTransformComponent->SetSize(100, 100);

	// �� ũ��� 48�� ����!
	ApplyCellPosition(6, 8);
#pragma endregion

	// ���̾� ó��
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