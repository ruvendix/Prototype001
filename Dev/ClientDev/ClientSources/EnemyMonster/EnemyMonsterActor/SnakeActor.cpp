// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SnakeActor.h"

DEFINE_COMPILETIME_ID(SnakeActor, EnemeyActorIdCounter)

class SnakeActor::Pimpl
{
	DEFINE_PIMPL(SnakeActor)

public:
	void LoadAndStartupSnakeSprite();
};

void SnakeActor::Pimpl::LoadAndStartupSnakeSprite()
{
	// �̰� Ȯ���ϰ� ���߷��� �ε������� �־����
	m_pOwner->LoadActorLookAtDirectionTexture("Assets/Texture/Monster/Snake.bmp");

	// ���¸��� ��������Ʈ�� �̸� �����
	m_pOwner->CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("SnakeIdle", true);

	// ��������Ʈ���� ����
	m_pOwner->AddActorStateKeyFrame<PawnActorIdleState>(0, 3, 0, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtDirection::Down);
	m_pOwner->AddActorStateKeyFrame<PawnActorIdleState>(0, 3, 1, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtDirection::Right);
	m_pOwner->AddActorStateKeyFrame<PawnActorIdleState>(0, 3, 2, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtDirection::Left);
	m_pOwner->AddActorStateKeyFrame<PawnActorIdleState>(0, 3, 3, Size{ 100, 100 }, RGB(128, 128, 128), 0.15f, EActorLookAtDirection::Up);

	// �⺻ ��������Ʈ ���� (SnakeIdleDown)
	DynamicSpriteComponent* pDynamicSpriteComponent = m_pOwner->AddComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	const DynamicSpritePtr& spDefaultPlayerDynamicSprite = m_pOwner->FindActorStateLookAtDynamicSprite<PawnActorIdleState>(EActorLookAtDirection::Down);
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
	TransformComponent* pTransformComponent = GetComponent<TransformComponent>();
	ASSERT_LOG(pTransformComponent != nullptr);

	// ��ǥ�� ũ�� ����
	//pTransformComponent->SetPosition(0.0f, 0.0f);
	pTransformComponent->SetSize(100, 100);

	CellActorMoveComponent* pCellActorMoveComponent = GetComponent<CellActorMoveComponent>();
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

void SnakeActor::RegisterStateOnBidirectional()
{
	Super::RegisterStateOnBidirectional();

	RegisterActorStateMappingTable<PawnActorIdleState>(Protocol::ENetworkEntityState::Chase);
	RegisterActorStateMappingTable<PawnActorIdleState>(Protocol::ENetworkEntityState::Attack);

	RegisterNetworkEntityStateMappingTable<PawnActorIdleState>(Protocol::ENetworkEntityState::Chase);
	RegisterNetworkEntityStateMappingTable<PawnActorIdleState>(Protocol::ENetworkEntityState::Attack);
}

void SnakeActor::ProcessDamaged(const std::shared_ptr<PawnActor>& spAttacker)
{
	// ����Ʈ ����
	Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);

	EffectSpawnInfo effectSpawnInfo;
	effectSpawnInfo.strEffectName = "OneTime_HitEffect";
	effectSpawnInfo.spawnCellPos = GetCellPosition();
	effectSpawnInfo.effectSize = Size(80, 80);
	pCurrentScene->ReserveCreateEffectActor(effectSpawnInfo);

	// ��� ��ƾ
	DecreaseEnemyCountToEnemyRespawner();
	pCurrentScene->ReserveEraseActor(SharedFromThisExactType<Actor>());
	DEFAULT_TRACE_LOG("�� ���!");
}