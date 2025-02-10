// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerActor.h"

#include "Engine/Actor/WorldTileMapActor.h"
#include "Engine/Actor/AnimationActor/AnimationActorState.h"
#include "Client/Monster/SnakeActor.h"

class PlayerActor::Pimpl
{
	DEFINE_PIMPL(PlayerActor)

public:
	void LoadAndStartupPlayerSprite();
	void InitializePlayerInput();

	bool DirectionKeyHandlerImpl(const Vec2d& vMoveDir);
	bool SpaceBarKeyHandlerImpl();
};

void PlayerActor::Pimpl::LoadAndStartupPlayerSprite()
{
	// �̰� Ȯ���ϰ� ���߷��� �ε������� �־����
	m_pOwner->LoadActorLookAtTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtType::Left);
	m_pOwner->LoadActorLookAtTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtType::Right);
	m_pOwner->LoadActorLookAtTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtType::Down);
	m_pOwner->LoadActorLookAtTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtType::Up);

	// ���¸��� ��������Ʈ�� �̸� �����
	m_pOwner->CreateActorStateLookAtDynamicSprites<AnimationActorIdleState>("PlayerIdle", true);
	m_pOwner->CreateActorStateLookAtDynamicSprites<AnimationActorWalkState>("PlayerWalk", true);
	m_pOwner->CreateActorStateLookAtDynamicSprites<AnimationActorAttackState>("PlayerAttack", false);

	// ��������Ʈ���� ����
	m_pOwner->AddActorStateKeyFrames<AnimationActorIdleState>(0, 9, 0, Size{ 200, 200 }, RGB(128, 128, 128), 0.1f);
	m_pOwner->AddActorStateKeyFrames<AnimationActorWalkState>(0, 9, 1, Size{ 200, 200 }, RGB(128, 128, 128), 0.05f);
	m_pOwner->AddActorStateKeyFrames<AnimationActorAttackState>(0, 7, 3, Size{ 200, 200 }, RGB(128, 128, 128), 0.06f);

	// �⺻ ��������Ʈ ���� (PlayerIdleDown)
	DynamicSpriteComponent* pDynamicSpriteComponent = m_pOwner->AddComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	const DynamicSpritePtr& spDefaultPlayerDynamicSprite = m_pOwner->FindActorStateLookAtDynamicSprite<AnimationActorIdleState>(EActorLookAtType::Down);
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
	leftKeyInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed; // �� �ѹ��� ����
	leftKeyInputMappingInfo.inputActionValueModifierBitset.BitOn(EInputActionValueModifierType::Negative);
	spPlayerWalkInputAction->AddInputMappingInfo(leftKeyInputMappingInfo);

	InputActionMappingInfo rightKeyInputMappingInfo;
	rightKeyInputMappingInfo.inputValue = EInputValue::Right;
	rightKeyInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed;
	spPlayerWalkInputAction->AddInputMappingInfo(rightKeyInputMappingInfo);

	InputActionMappingInfo upKeyInputMappingInfo;
	upKeyInputMappingInfo.inputValue = EInputValue::Up; // ����Ű�� ������ �۵��ϴ� �� �׽�Ʈ
	upKeyInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed; // �� �ѹ��� ����
	upKeyInputMappingInfo.inputActionValueModifierBitset.BitsOn(EInputActionValueModifierType::Swizzle, EInputActionValueModifierType::Negative);
	spPlayerWalkInputAction->AddInputMappingInfo(upKeyInputMappingInfo);

	InputActionMappingInfo downKeyInputMappingInfo;
	downKeyInputMappingInfo.inputValue = EInputValue::Down;
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
	spaceBarKeyInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed;
	spPlayerAttackInputAction->AddInputMappingInfo(spaceBarKeyInputMappingInfo);

	LocalPlayerInputSystem::I()->BindInputActionHandler(spPlayerAttackInputAction, m_pOwner, &PlayerActor::OnSpaceBarKeyHandler);
	spPlayerInputMappingContext->AddInputAction(spPlayerAttackInputAction);
#pragma endregion
#pragma endregion
}

bool PlayerActor::Pimpl::DirectionKeyHandlerImpl(const Vec2d& vMoveDir)
{
	if (m_pOwner->IsSameAnimationActorState<AnimationActorIdleState>() == false)
	{
		DEFAULT_TRACE_LOG("Idle�� ���� �ൿ ����!");
		return false;
	}

	CellActorMoveComponent* pMoveComponent = m_pOwner->FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN_VALUE(pMoveComponent != nullptr, false);

	// ���� �� ��ǥ ���
	Position2d currentCellPos = pMoveComponent->GetDestinationCellPosition();

	// ��ǥ ���� �� ���� �ٲٰ�
	m_pOwner->ApplyMoveDirection(vMoveDir);
	pMoveComponent->ApplyMoveDirection(vMoveDir);

	// �̵� ��������?
	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);
	if (pCurrentScene->CheckCanMoveToCellPosition(pMoveComponent->GetDestinationCellPosition()) == false)
	{
		// Idle ��������Ʈ�� �ٲ�
		m_pOwner->ChangeActorStateDynamicSprite<AnimationActorIdleState>();

		pMoveComponent->SetDestinationCellPosition(currentCellPos);
		pMoveComponent->ResetMoveDirection();
		DEFAULT_TRACE_LOG("�̵� ����!");

		return false;
	}

	// Walk ��������Ʈ�� �ٲ�
	m_pOwner->ChangeActorStateDynamicSprite<AnimationActorWalkState>();
	return true;
}

bool PlayerActor::Pimpl::SpaceBarKeyHandlerImpl()
{
	if (m_pOwner->IsSameAnimationActorState<AnimationActorIdleState>() == false)
	{
		DEFAULT_TRACE_LOG("Idle�� ���� �ൿ ����!");
		return false;
	}

	// Attack ��������Ʈ�� �ٲ�
	m_pOwner->ChangeActorStateDynamicSprite<AnimationActorAttackState>();
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
	pMoveComponent->SetMoveSpeed(90.0f);
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

void PlayerActor::OnDirectionKeyHandler(const InputActionValue* pInputAction)
{
	const Vec2d& vMoveDir = pInputAction->BringValue<Vec2d>();
	if (m_spPimpl->DirectionKeyHandlerImpl(vMoveDir) == false)
	{
		return;
	}

	ImmediatelyChangePlayerState<AnimationActorWalkState>();
	DEFAULT_TRACE_LOG("(�⺻ -> �ȱ�) ���·� ��ȯ!");
}

void PlayerActor::OnSpaceBarKeyHandler(const InputActionValue* pInputAction)
{
	if (m_spPimpl->SpaceBarKeyHandlerImpl() == false)
	{
		return;
	}
	
	ImmediatelyChangePlayerState<AnimationActorAttackState>();
	DEFAULT_TRACE_LOG("(�⺻ -> ����) ���·� ��ȯ!");
}