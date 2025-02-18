// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerActor.h"

#include "PlayerState.h"

class PlayerActor::Pimpl
{
	DEFINE_PIMPL(PlayerActor)

public:
	void LoadAndStartupPlayerSprite();
	void InitializePlayerInput();

	bool DirectionKeyHandlerImpl(const Vector2d& vMoveDir);
	bool SpaceBarKeyHandlerImpl();
	bool AKeyHandlerImpl(const InputActionValue* pInputAction);
};

void PlayerActor::Pimpl::LoadAndStartupPlayerSprite()
{
	// �̰� Ȯ���ϰ� ���߷��� �ε������� �־����
	m_pOwner->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtDirection::Left);
	m_pOwner->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtDirection::Right);
	m_pOwner->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtDirection::Down);
	m_pOwner->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtDirection::Up);

	// ���¸��� ��������Ʈ�� �̸� ����� (�ʼ��� �ƴ�)
	m_pOwner->CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("PlayerIdle", true);
	m_pOwner->CreateActorStateLookAtDynamicSprites<PawnActorWalkState>("PlayerWalk", true);

	// ��������Ʈ���� ����
	m_pOwner->AddActorStateKeyFrames<PawnActorIdleState>(0, 9, 0, Size{ 200, 200 }, RGB(128, 128, 128), 0.1f);
	m_pOwner->AddActorStateKeyFrames<PawnActorWalkState>(0, 9, 1, Size{ 200, 200 }, RGB(128, 128, 128), 0.05f);

	// �⺻ ��������Ʈ ���� (PlayerIdleDown)
	DynamicSpriteComponent* pDynamicSpriteComponent = m_pOwner->AddComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	const DynamicSpritePtr& spDefaultPlayerDynamicSprite = m_pOwner->FindActorStateLookAtDynamicSprite<PawnActorIdleState>(EActorLookAtDirection::Down);
	pDynamicSpriteComponent->ApplyDynamicSprite(spDefaultPlayerDynamicSprite);
}

void PlayerActor::Pimpl::InitializePlayerInput()
{
#pragma region �Է� ó��
	InputMappingContextPtr spPlayerInputMappingContext = std::make_shared<InputMappingContext>();
	LocalPlayerInputSystem::I()->AddInputMappingContext(spPlayerInputMappingContext, 0);

#pragma region �÷��̾� �̵� (Vector2�� �ذ�)
	InputActionValue playerWalkInputActionValue;

	// �Է°� �����ϸ鼭 Ʈ���ſ� �ڵ鷯 ����
	InputActionPtr spPlayerWalkInputAction = std::make_unique<InputAction>("PlayerWalk");
	spPlayerWalkInputAction->ApplyInputActionValue(playerWalkInputActionValue, EInputActionValueType::Vector2);

	// ������ ���⼭ ���� ���� ����
#pragma region ����Ű�� ���� �� ����
	InputActionMappingInfo leftKeyInputMappingInfo;
	leftKeyInputMappingInfo.inputValue = EInputValue::Left; // ����Ű�� ������ �۵��ϴ� �� �׽�Ʈ
	leftKeyInputMappingInfo.inputTrigger = EInputTrigger::PressedAndHolding; // �� �ѹ��� ����
	leftKeyInputMappingInfo.inputActionValueModifierBitset.BitOn(EInputActionValueModifierType::Negative);
	spPlayerWalkInputAction->AddInputMappingInfo(leftKeyInputMappingInfo);

	InputActionMappingInfo rightKeyInputMappingInfo;
	rightKeyInputMappingInfo.inputValue = EInputValue::Right;
	rightKeyInputMappingInfo.inputTrigger = EInputTrigger::PressedAndHolding;
	spPlayerWalkInputAction->AddInputMappingInfo(rightKeyInputMappingInfo);

	InputActionMappingInfo upKeyInputMappingInfo;
	upKeyInputMappingInfo.inputValue = EInputValue::Up; // ����Ű�� ������ �۵��ϴ� �� �׽�Ʈ
	upKeyInputMappingInfo.inputTrigger = EInputTrigger::PressedAndHolding; // �� �ѹ��� ����
	upKeyInputMappingInfo.inputActionValueModifierBitset.BitsOn(EInputActionValueModifierType::Swizzle, EInputActionValueModifierType::Negative);
	spPlayerWalkInputAction->AddInputMappingInfo(upKeyInputMappingInfo);

	InputActionMappingInfo downKeyInputMappingInfo;
	downKeyInputMappingInfo.inputValue = EInputValue::Down;
	downKeyInputMappingInfo.inputTrigger = EInputTrigger::PressedAndHolding;
	downKeyInputMappingInfo.inputActionValueModifierBitset.BitOn(EInputActionValueModifierType::Swizzle);
	spPlayerWalkInputAction->AddInputMappingInfo(downKeyInputMappingInfo);
#pragma endregion

	LocalPlayerInputSystem::I()->BindInputActionHandler(spPlayerWalkInputAction, m_pOwner, &PlayerActor::OnDirectionKeyHandler);
	spPlayerInputMappingContext->AddInputAction(spPlayerWalkInputAction);
#pragma endregion

#pragma region �÷��̾� ����
	InputActionValue playerAttackInputActionValue;

	// �Է°� �����ϸ鼭 Ʈ���ſ� �ڵ鷯 ����
	InputActionPtr spPlayerAttackInputAction = std::make_unique<InputAction>("PlayerAttack");
	spPlayerAttackInputAction->ApplyInputActionValue(playerAttackInputActionValue, EInputActionValueType::Boolean);

	InputActionMappingInfo spaceBarKeyInputMappingInfo;
	spaceBarKeyInputMappingInfo.inputValue = EInputValue::SpaceBar;
	spaceBarKeyInputMappingInfo.inputTrigger = EInputTrigger::PressedAndHolding;
	spPlayerAttackInputAction->AddInputMappingInfo(spaceBarKeyInputMappingInfo);

	LocalPlayerInputSystem::I()->BindInputActionHandler(spPlayerAttackInputAction, m_pOwner, &PlayerActor::OnSpaceBarKeyHandler);
	spPlayerInputMappingContext->AddInputAction(spPlayerAttackInputAction);
#pragma endregion

#pragma region �÷��̾� ���
	InputActionValue playerDefenceInputActionValue;

	// �Է°� �����ϸ鼭 Ʈ���ſ� �ڵ鷯 ����
	InputActionPtr spPlayerDefenceInputAction = std::make_unique<InputAction>("PlayerDefence");
	spPlayerDefenceInputAction->ApplyInputActionValue(playerDefenceInputActionValue, EInputActionValueType::Boolean);

	InputActionMappingInfo aKeyInputMappingInfo;
	aKeyInputMappingInfo.inputValue = EInputValue::A;
	aKeyInputMappingInfo.inputTrigger = EInputTrigger::Triggered;
	spPlayerDefenceInputAction->AddInputMappingInfo(aKeyInputMappingInfo);

	LocalPlayerInputSystem::I()->BindInputActionHandler(spPlayerDefenceInputAction, m_pOwner, &PlayerActor::OnAKeyHandler);
	spPlayerInputMappingContext->AddInputAction(spPlayerDefenceInputAction);
#pragma endregion
#pragma endregion
}

bool PlayerActor::Pimpl::DirectionKeyHandlerImpl(const Vector2d& vMoveDir)
{
	// �ప�� ���� �����Ѵٸ� ��ȿ
	if ((vMoveDir.x != 0.0f) &&
		(vMoveDir.y != 0.0f))
	{
		return false;
	}

	if (m_pOwner->IsSamePawnActorState<PawnActorIdleState>() == false)
	{
		DEFAULT_TRACE_LOG("Idle�� ���� �ൿ ����!");
		return false;
	}

	CellActorMoveComponent* pCellActorMoveComponent = m_pOwner->FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN_VALUE(pCellActorMoveComponent != nullptr, false);
	if (pCellActorMoveComponent->ProcessMoveDirection(vMoveDir, true) == false)
	{
		return false;
	}

	return true;
}

bool PlayerActor::Pimpl::SpaceBarKeyHandlerImpl()
{
	if (m_pOwner->IsSamePawnActorState<PawnActorIdleState>() == false)
	{
		DEFAULT_TRACE_LOG("Idle�� ���� �ൿ ����!");
		return false;
	}

	// ���� ���⿡ ���� �۵�
	const std::shared_ptr<WeaponActor>& spPrimaryWeapon = m_pOwner->GetWeaponActor(EWeaponSlotType::Primary);
	if (spPrimaryWeapon == nullptr)
	{
		return false;
	}

	spPrimaryWeapon->ApplyDynamicSpriteToOwner();
	return true;
}

bool PlayerActor::Pimpl::AKeyHandlerImpl(const InputActionValue* pInputAction)
{
	bool bIdleState = m_pOwner->IsSamePawnActorState<PawnActorIdleState>();

	bool bKeyPressed = pInputAction->BringValue<bool>();
	if (bKeyPressed == false)
	{
		if (m_pOwner->IsSamePawnActorState<PlayerDefenceState>())
		{
			const std::shared_ptr<PlayerDefenceState>& spPlayerDefenceState = m_pOwner->GetCurrentPawnActorState<PlayerDefenceState>();
			if (spPlayerDefenceState->CheckKnockbackExisted() == false)
			{
				m_pOwner->ImmediatelyChangeState<PawnActorIdleState>();
			}

			return false;
		}
		else if (bIdleState == true)
		{
			return false;
		}
	}

	if (bIdleState == false)
	{
		DEFAULT_TRACE_LOG("Idle�� ���� �ൿ ����!");
		return false;
	}

	// ���� ���⿡ ���� �۵�
	const std::shared_ptr<WeaponActor>& spSecondaryWeapon = m_pOwner->GetWeaponActor(EWeaponSlotType::Secondary);
	if (spSecondaryWeapon == nullptr)
	{
		return false;
	}

	spSecondaryWeapon->ApplyDynamicSpriteToOwner();
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
PlayerActor::~PlayerActor()
{
	Cleanup();
}

void PlayerActor::Startup()
{
	Super::Startup();
	ALLOC_PIMPL;

	// �÷��̾� ��������Ʈ �ε� �� �ʱ�ȭ
	m_spPimpl->LoadAndStartupPlayerSprite();

	// �÷��̾� �Է� ���� �ʱ�ȭ
	m_spPimpl->InitializePlayerInput();

	// ���� ����
	m_arrWeaponActor[TO_NUM(EWeaponSlotType::Primary)] = WeaponFactory::I()->CreateWeaponActor(0);
	m_arrWeaponActor[TO_NUM(EWeaponSlotType::Secondary)] = WeaponFactory::I()->CreateWeaponActor(1);

	for (const std::shared_ptr<WeaponActor>& spWeaponActor : m_arrWeaponActor)
	{
		spWeaponActor->SetWeaponOwner(std::dynamic_pointer_cast<PlayerActor>(shared_from_this()));
	}

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
	pCurrentScene->ReserveCreateEffect(effectSpawnInfo);

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

void PlayerActor::OnDirectionKeyHandler(const InputActionValue* pInputAction)
{
	Vector2d vMoveDir = pInputAction->BringValue<Vector2d>();
	if (m_spPimpl->DirectionKeyHandlerImpl(vMoveDir) == false)
	{
		return;
	}

	ImmediatelyChangeState<PawnActorWalkState>();
	DEFAULT_TRACE_LOG("(�⺻ -> �ȱ�) ���·� ��ȯ!");
}

void PlayerActor::OnSpaceBarKeyHandler(const InputActionValue* pInputAction)
{
	if (m_spPimpl->SpaceBarKeyHandlerImpl() == false)
	{
		return;
	}
	
	ImmediatelyChangeState<PlayerAttackState>();
	DEFAULT_TRACE_LOG("(�⺻ -> ����) ���·� ��ȯ!");
}

void PlayerActor::OnAKeyHandler(const InputActionValue* pInputAction)
{
	if (m_spPimpl->AKeyHandlerImpl(pInputAction) == false)
	{
		return;
	}

	ImmediatelyChangeState<PlayerDefenceState>();
	DEFAULT_TRACE_LOG("(�⺻ -> ���) ���·� ��ȯ!");
}