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

	// 플레이어 스프라이트 로딩 및 초기화
	LoadAndStartupPlayerSprite();

	//// 무기 설정
	//m_arrWeaponActor[TO_NUM(EWeaponSlotType::Primary)] = WeaponFactory::I()->CreateWeaponActor(2);
	//m_arrWeaponActor[TO_NUM(EWeaponSlotType::Secondary)] = WeaponFactory::I()->CreateWeaponActor(1);

	//for (const std::shared_ptr<WeaponActor>& spWeaponActor : m_arrWeaponActor)
	//{
	//	spWeaponActor->SetWeaponOwner(std::dynamic_pointer_cast<PlayerActor>(shared_from_this()));
	//}

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
	pCurrentScene->ReserveCreateEffectActor(effectSpawnInfo);

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

void PlayerActor::ProcessMoveDirection(const Vector2d& vMoveDir)
{
	// 축값이 전부 존재한다면 무효
	if ((vMoveDir.x != 0.0f) &&
		(vMoveDir.y != 0.0f))
	{
		return;
	}

	if (IsSamePawnActorState<PawnActorIdleState>() == false)
	{
		DEFAULT_TRACE_LOG("Idle일 때만 행동 가능!");
		return;
	}

	CellActorMoveComponent* pMoveComponent = FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN(pMoveComponent != nullptr);
	if (pMoveComponent->ProcessMoveDirection(vMoveDir, true) == false)
	{
		return;
	}

	ImmediatelyChangeState<PawnActorWalkState>();
	DEFAULT_TRACE_LOG("(기본 -> 걷기) 상태로 전환!");
}

void PlayerActor::LoadAndStartupPlayerSprite()
{
	// 이걸 확실하게 맞추려면 인덱스마다 넣어야함
	LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtDirection::Left);
	LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtDirection::Right);
	LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtDirection::Down);
	LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtDirection::Up);

	// 상태마다 스프라이트를 미리 만들고 (필수는 아님)
	CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("PlayerIdle", true);
	CreateActorStateLookAtDynamicSprites<PawnActorWalkState>("PlayerWalk", true);

	// 스프라이트마다 설정
	AddActorStateKeyFrames<PawnActorIdleState>(0, 9, 0, Size{ 200, 200 }, RGB(128, 128, 128), 0.1f);
	AddActorStateKeyFrames<PawnActorWalkState>(0, 9, 1, Size{ 200, 200 }, RGB(128, 128, 128), 0.05f);

	// 다이나믹 스프라이트 추가
	DynamicSpriteComponent* pDynamicSpriteComponent = AddComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	// 기본 스프라이트 설정 (PlayerIdleDown)
	//DynamicSpriteComponent* pDynamicSpriteComponent = AddComponent<DynamicSpriteComponent>();
	//ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	//const DynamicSpritePtr& spDefaultPlayerDynamicSprite = FindActorStateLookAtDynamicSprite<PawnActorIdleState>(EActorLookAtDirection::Down);
	//pDynamicSpriteComponent->ApplyDynamicSprite(spDefaultPlayerDynamicSprite);
}