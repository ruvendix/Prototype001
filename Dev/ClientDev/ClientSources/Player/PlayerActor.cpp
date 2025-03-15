// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerActor.h"

#include "PlayerState.h"

PlayerActor::~PlayerActor()
{
	Cleanup();
}

void PlayerActor::Startup()
{
	Super::Startup();

	// �÷��̾� ��������Ʈ �ε� �� �ʱ�ȭ
	LoadAndStartupPlayerSprite();

	//// ���� ����
	//m_arrWeaponActor[TO_NUM(EWeaponSlotType::Primary)] = WeaponFactory::I()->CreateWeaponActor(2);
	//m_arrWeaponActor[TO_NUM(EWeaponSlotType::Secondary)] = WeaponFactory::I()->CreateWeaponActor(1);

	//for (const std::shared_ptr<WeaponActor>& spWeaponActor : m_arrWeaponActor)
	//{
	//	spWeaponActor->SetWeaponOwner(std::dynamic_pointer_cast<PlayerActor>(shared_from_this()));
	//}

#pragma region �÷��̾� �⺻ ���� �ʱ�ȭ
	TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG(pTransformComponent != nullptr);

	// ��ǥ�� ũ�� ����
	//pTransformComponent->SetPosition(0.0f, 0.0f);
	pTransformComponent->SetSize(200, 200);

	// �� ũ��� 48�� ����!
	ApplyCellPosition(3, 6);
#pragma endregion

	// �̵� ó��
	AddComponent<CellActorMoveComponent>();
	CellActorMoveComponent* pMoveComponent = FindComponent<CellActorMoveComponent>();
	pMoveComponent->SetMoveSpeed(110.0f);
	pMoveComponent->SetDestinationCellPosition(GetCellPosition()); // �ʱ�ȭ�ϱ� �Ȱ���
}

bool PlayerActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}
	
	return true;
}

void PlayerActor::Cleanup()
{
	return (Super::Cleanup());
}

void PlayerActor::ProcessDamaged(const std::shared_ptr<PawnActor>& spAttacker)
{
	// ����Ʈ ����
	Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);

	EffectSpawnInfo effectSpawnInfo;
	effectSpawnInfo.strEffectName = "OneTime_HitEffect";
	effectSpawnInfo.spawnCellPos = GetCellPosition();
	effectSpawnInfo.effectSize = Size(80, 80);
	pCurrentScene->ReserveCreateEffectActor(effectSpawnInfo);

	if (IsSamePawnActorState<PlayerDefenceState>())
	{
		const std::shared_ptr<WeaponActor>& spSecondaryWeapon = GetWeaponActor(EWeaponSlotType::Secondary);
		// spSecondaryWeapon �̰��� ������ �����ͼ� �̰� ����

		// �˹��� ������ ��Ȳ�̶�� �˹� ����
		if (CheckPossibleKnockback(spAttacker->GetActorLookAtDirection()) == true)
		{
			CellActorMoveComponent* pCellActorMoveComponent = FindComponent<CellActorMoveComponent>();
			ASSERT_LOG_RETURN(pCellActorMoveComponent != nullptr);
			pCellActorMoveComponent->ProcessMoveDirection(CalculateBackwardDirection(), false);

			DEFAULT_TRACE_LOG("�÷��̾� �˹�!");
		}

		DEFAULT_TRACE_LOG("�÷��̾� ���!");
		return;
	}

	// ��� ��ƾ
	//pCurrentScene->ReserveEraseActor(shared_from_this());
	//SetActorFlagBitOff(EActorFlag::Activation);

	DEFAULT_TRACE_LOG("�÷��̾� ���!");
}

void PlayerActor::ProcessMoveDirection(const Vector2d& vMoveDir)
{
	// �ప�� ���� �����Ѵٸ� ��ȿ
	if ((vMoveDir.x != 0.0f) &&
		(vMoveDir.y != 0.0f))
	{
		return;
	}

	if (IsSamePawnActorState<PawnActorIdleState>() == false)
	{
		DEFAULT_TRACE_LOG("Idle�� ���� �ൿ ����!");
		return;
	}

	CellActorMoveComponent* pMoveComponent = FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN(pMoveComponent != nullptr);
	if (pMoveComponent->ProcessMoveDirection(vMoveDir, true) == false)
	{
		return;
	}

	ImmediatelyChangeState<PawnActorWalkState>();
	DEFAULT_TRACE_LOG("(�⺻ -> �ȱ�) ���·� ��ȯ!");
}

void PlayerActor::LoadAndStartupPlayerSprite()
{
	// �̰� Ȯ���ϰ� ���߷��� �ε������� �־����
	LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtDirection::Left);
	LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtDirection::Right);
	LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtDirection::Down);
	LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtDirection::Up);

	// ���¸��� ��������Ʈ�� �̸� ����� (�ʼ��� �ƴ�)
	CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("PlayerIdle", true);
	CreateActorStateLookAtDynamicSprites<PawnActorWalkState>("PlayerWalk", true);

	// ��������Ʈ���� ����
	AddActorStateKeyFrames<PawnActorIdleState>(0, 9, 0, Size{ 200, 200 }, RGB(128, 128, 128), 0.1f);
	AddActorStateKeyFrames<PawnActorWalkState>(0, 9, 1, Size{ 200, 200 }, RGB(128, 128, 128), 0.05f);

	// ���̳��� ��������Ʈ �߰�
	DynamicSpriteComponent* pDynamicSpriteComponent = AddComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	// �⺻ ��������Ʈ ���� (PlayerIdleDown)
	//DynamicSpriteComponent* pDynamicSpriteComponent = AddComponent<DynamicSpriteComponent>();
	//ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	//const DynamicSpritePtr& spDefaultPlayerDynamicSprite = FindActorStateLookAtDynamicSprite<PawnActorIdleState>(EActorLookAtDirection::Down);
	//pDynamicSpriteComponent->ApplyDynamicSprite(spDefaultPlayerDynamicSprite);
}