// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "LocalPlayerActor.h"

#include "PlayerState.h"

class LocalPlayerActor::Pimpl
{
	DEFINE_PIMPL(LocalPlayerActor)

public:
	void InitializePlayerInput();

	bool AKeyHandlerImpl(const InputActionValue* pInputAction);
};

void LocalPlayerActor::Pimpl::InitializePlayerInput()
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

	LocalPlayerInputSystem::I()->BindInputActionHandler(spPlayerWalkInputAction, m_pOwner, &LocalPlayerActor::OnDirectionKeyHandler);
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

	LocalPlayerInputSystem::I()->BindInputActionHandler(spPlayerAttackInputAction, m_pOwner, &LocalPlayerActor::OnSpaceBarKeyHandler);
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

	LocalPlayerInputSystem::I()->BindInputActionHandler(spPlayerDefenceInputAction, m_pOwner, &LocalPlayerActor::OnAKeyHandler);
	spPlayerInputMappingContext->AddInputAction(spPlayerDefenceInputAction);
#pragma endregion
#pragma endregion
}

bool LocalPlayerActor::Pimpl::AKeyHandlerImpl(const InputActionValue* pInputAction)
{
	bool bIdleState = m_pOwner->IsSamePawnActorState<PawnActorIdleState>();

	bool bKeyPressed = pInputAction->BringValue<bool>();
	if (bKeyPressed == false)
	{
		if (m_pOwner->IsSamePawnActorState<PlayerDefenceState>())
		{
			const std::shared_ptr<PlayerDefenceState>& spPlayerDefenceState = m_pOwner->GetCurrentExactPawnActorState<PlayerDefenceState>();
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
		//DEFAULT_TRACE_LOG("Idle�� ���� �ൿ ����!");
		return false;
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LocalPlayerActor::~LocalPlayerActor()
{

}

void LocalPlayerActor::Startup()
{
	Super::Startup();
	ALLOC_PIMPL;

	// ���� �÷��̾� �Է� ���� �ʱ�ȭ
	m_spPimpl->InitializePlayerInput();
}

bool LocalPlayerActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	SyncToServer_EntityInfoIfNeed();
	return true;
}

void LocalPlayerActor::Cleanup()
{
	Super::Cleanup();
}

void LocalPlayerActor::OnDirectionKeyHandler(const InputActionValue* pInputAction)
{
	Vector2d vMoveDir = pInputAction->BringValue<Vector2d>();
	ProcessMoveDirection(vMoveDir);
}

void LocalPlayerActor::OnSpaceBarKeyHandler(const InputActionValue* pInputAction)
{
	if (IsSamePawnActorState<PawnActorIdleState>() == false)
	{
		DEFAULT_TRACE_LOG("Idle�� ���� �ൿ ����!");
		return;
	}

	ImmediatelyChangeState<PlayerAttackState>();
	DEFAULT_TRACE_LOG("(�⺻ -> ����) ���·� ��ȯ!");
}

void LocalPlayerActor::OnAKeyHandler(const InputActionValue* pInputAction)
{
	if (m_spPimpl->AKeyHandlerImpl(pInputAction) == false)
	{
		return;
	}

	ProcessDefense();
}