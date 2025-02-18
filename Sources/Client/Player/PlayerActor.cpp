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
	// 이걸 확실하게 맞추려면 인덱스마다 넣어야함
	m_pOwner->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtDirection::Left);
	m_pOwner->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtDirection::Right);
	m_pOwner->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtDirection::Down);
	m_pOwner->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtDirection::Up);

	// 상태마다 스프라이트를 미리 만들고 (필수는 아님)
	m_pOwner->CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("PlayerIdle", true);
	m_pOwner->CreateActorStateLookAtDynamicSprites<PawnActorWalkState>("PlayerWalk", true);

	// 스프라이트마다 설정
	m_pOwner->AddActorStateKeyFrames<PawnActorIdleState>(0, 9, 0, Size{ 200, 200 }, RGB(128, 128, 128), 0.1f);
	m_pOwner->AddActorStateKeyFrames<PawnActorWalkState>(0, 9, 1, Size{ 200, 200 }, RGB(128, 128, 128), 0.05f);

	// 기본 스프라이트 설정 (PlayerIdleDown)
	DynamicSpriteComponent* pDynamicSpriteComponent = m_pOwner->AddComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	const DynamicSpritePtr& spDefaultPlayerDynamicSprite = m_pOwner->FindActorStateLookAtDynamicSprite<PawnActorIdleState>(EActorLookAtDirection::Down);
	pDynamicSpriteComponent->ApplyDynamicSprite(spDefaultPlayerDynamicSprite);
}

void PlayerActor::Pimpl::InitializePlayerInput()
{
#pragma region 입력 처리
	InputMappingContextPtr spPlayerInputMappingContext = std::make_shared<InputMappingContext>();
	LocalPlayerInputSystem::I()->AddInputMappingContext(spPlayerInputMappingContext, 0);

#pragma region 플레이어 이동 (Vector2로 해결)
	InputActionValue playerWalkInputActionValue;

	// 입력값 포함하면서 트리거와 핸들러 저장
	InputActionPtr spPlayerWalkInputAction = std::make_unique<InputAction>("PlayerWalk");
	spPlayerWalkInputAction->ApplyInputActionValue(playerWalkInputActionValue, EInputActionValueType::Vector2);

	// 순서상 여기서 매핑 정보 넣음
#pragma region 방향키에 따른 값 설정
	InputActionMappingInfo leftKeyInputMappingInfo;
	leftKeyInputMappingInfo.inputValue = EInputValue::Left; // 왼쪽키를 누르면 작동하는 것 테스트
	leftKeyInputMappingInfo.inputTrigger = EInputTrigger::PressedAndHolding; // 딱 한번만 누름
	leftKeyInputMappingInfo.inputActionValueModifierBitset.BitOn(EInputActionValueModifierType::Negative);
	spPlayerWalkInputAction->AddInputMappingInfo(leftKeyInputMappingInfo);

	InputActionMappingInfo rightKeyInputMappingInfo;
	rightKeyInputMappingInfo.inputValue = EInputValue::Right;
	rightKeyInputMappingInfo.inputTrigger = EInputTrigger::PressedAndHolding;
	spPlayerWalkInputAction->AddInputMappingInfo(rightKeyInputMappingInfo);

	InputActionMappingInfo upKeyInputMappingInfo;
	upKeyInputMappingInfo.inputValue = EInputValue::Up; // 왼쪽키를 누르면 작동하는 것 테스트
	upKeyInputMappingInfo.inputTrigger = EInputTrigger::PressedAndHolding; // 딱 한번만 누름
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

#pragma region 플레이어 공격
	InputActionValue playerAttackInputActionValue;

	// 입력값 포함하면서 트리거와 핸들러 저장
	InputActionPtr spPlayerAttackInputAction = std::make_unique<InputAction>("PlayerAttack");
	spPlayerAttackInputAction->ApplyInputActionValue(playerAttackInputActionValue, EInputActionValueType::Boolean);

	InputActionMappingInfo spaceBarKeyInputMappingInfo;
	spaceBarKeyInputMappingInfo.inputValue = EInputValue::SpaceBar;
	spaceBarKeyInputMappingInfo.inputTrigger = EInputTrigger::PressedAndHolding;
	spPlayerAttackInputAction->AddInputMappingInfo(spaceBarKeyInputMappingInfo);

	LocalPlayerInputSystem::I()->BindInputActionHandler(spPlayerAttackInputAction, m_pOwner, &PlayerActor::OnSpaceBarKeyHandler);
	spPlayerInputMappingContext->AddInputAction(spPlayerAttackInputAction);
#pragma endregion

#pragma region 플레이어 방어
	InputActionValue playerDefenceInputActionValue;

	// 입력값 포함하면서 트리거와 핸들러 저장
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
	// 축값이 전부 존재한다면 무효
	if ((vMoveDir.x != 0.0f) &&
		(vMoveDir.y != 0.0f))
	{
		return false;
	}

	if (m_pOwner->IsSamePawnActorState<PawnActorIdleState>() == false)
	{
		DEFAULT_TRACE_LOG("Idle일 때만 행동 가능!");
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
		DEFAULT_TRACE_LOG("Idle일 때만 행동 가능!");
		return false;
	}

	// 현재 무기에 따라 작동
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
		DEFAULT_TRACE_LOG("Idle일 때만 행동 가능!");
		return false;
	}

	// 현재 무기에 따라 작동
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

	// 플레이어 스프라이트 로딩 및 초기화
	m_spPimpl->LoadAndStartupPlayerSprite();

	// 플레이어 입력 관련 초기화
	m_spPimpl->InitializePlayerInput();

	// 무기 설정
	m_arrWeaponActor[TO_NUM(EWeaponSlotType::Primary)] = WeaponFactory::I()->CreateWeaponActor(0);
	m_arrWeaponActor[TO_NUM(EWeaponSlotType::Secondary)] = WeaponFactory::I()->CreateWeaponActor(1);

	for (const std::shared_ptr<WeaponActor>& spWeaponActor : m_arrWeaponActor)
	{
		spWeaponActor->SetWeaponOwner(std::dynamic_pointer_cast<PlayerActor>(shared_from_this()));
	}

#pragma region 플레이어 기본 정보 초기화
	TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG(pTransformComponent != nullptr);

	// 좌표와 크기 설정
	//pTransformComponent->SetPosition(0.0f, 0.0f);
	pTransformComponent->SetSize(200, 200);

	// 셀 크기는 48로 고정!
	ApplyCellPosition(3, 6);
#pragma endregion

	// 이동 처리
	AddComponent<CellActorMoveComponent>();
	CellActorMoveComponent* pMoveComponent = FindComponent<CellActorMoveComponent>();
	pMoveComponent->SetMoveSpeed(110.0f);
	pMoveComponent->SetDestinationCellPosition(GetCellPosition()); // 초기화니까 똑같음
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
	// 이펙트 생성
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
		// spSecondaryWeapon 이거의 스탯을 가져와서 이것 저것

		// 넉백이 가능한 상황이라면 넉백 실행
		if (CheckPossibleKnockback(spAttacker->GetActorLookAtDirection()) == true)
		{
			CellActorMoveComponent* pCellActorMoveComponent = FindComponent<CellActorMoveComponent>();
			ASSERT_LOG_RETURN(pCellActorMoveComponent != nullptr);
			pCellActorMoveComponent->ProcessMoveDirection(CalculateBackwardDirection(), false);

			DEFAULT_TRACE_LOG("플레이어 넉백!");
		}

		DEFAULT_TRACE_LOG("플레이어 방어!");
		return;
	}

	// 사망 루틴
	//pCurrentScene->ReserveEraseActor(shared_from_this());
	//SetActorFlagBitOff(EActorFlag::Activation);

	DEFAULT_TRACE_LOG("플레이어 사망!");
}

void PlayerActor::OnDirectionKeyHandler(const InputActionValue* pInputAction)
{
	Vector2d vMoveDir = pInputAction->BringValue<Vector2d>();
	if (m_spPimpl->DirectionKeyHandlerImpl(vMoveDir) == false)
	{
		return;
	}

	ImmediatelyChangeState<PawnActorWalkState>();
	DEFAULT_TRACE_LOG("(기본 -> 걷기) 상태로 전환!");
}

void PlayerActor::OnSpaceBarKeyHandler(const InputActionValue* pInputAction)
{
	if (m_spPimpl->SpaceBarKeyHandlerImpl() == false)
	{
		return;
	}
	
	ImmediatelyChangeState<PlayerAttackState>();
	DEFAULT_TRACE_LOG("(기본 -> 공격) 상태로 전환!");
}

void PlayerActor::OnAKeyHandler(const InputActionValue* pInputAction)
{
	if (m_spPimpl->AKeyHandlerImpl(pInputAction) == false)
	{
		return;
	}

	ImmediatelyChangeState<PlayerDefenceState>();
	DEFAULT_TRACE_LOG("(기본 -> 방어) 상태로 전환!");
}