// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerActor.h"

#include "Engine/Actor/WorldTileMapActor.h"
#include "PlayerState.h"

namespace
{
	std::array<std::string, TO_NUM(ESceneActorLookAtType::Count)> g_arrPlayerIdleStateDynamicSpriteStringTable =
	{
		"PlayerIdleLeft",
		"PlayerIdleRight"
		"PlayerIdleDown",
		"PlayerIdleUp"
	};

	std::array<std::string, TO_NUM(ESceneActorLookAtType::Count)> g_arrPlayerWalkStateDynamicSpriteStringTable =
	{
		"PlayerWalkLeft",
		"PlayerWalkRight",
		"PlayerWalkDown",
		"PlayerWalkUp"
	};
}

struct PlayerSpriteStartupInfo
{
	std::string strSpriteName;
	std::string strUseTexturePath;
	int32 spriteLine = 0;
};

class PlayerActor::Pimpl
{
	DEFINE_PIMPL(PlayerActor)

public:
	void LoadAndStartupPlayerSprite();
	bool DirectionKeyHandlerImpl(const Vec2d& vMoveDir);
};

void PlayerActor::Pimpl::LoadAndStartupPlayerSprite()
{
	std::array<std::string, TO_NUM(ESceneActorLookAtType::Count)> arrPlayerTexturePathTable =
	{
		"Assets/Texture/Player/PlayerLeft.bmp",
		"Assets/Texture/Player/PlayerRight.bmp",
		"Assets/Texture/Player/PlayerDown.bmp",
		"Assets/Texture/Player/PlayerUp.bmp",
	};

	// �÷��̾� �ؽ�ó �ε�
	for (const std::string strPlayerTexturePath : arrPlayerTexturePathTable)
	{
		ResourceMananger::I()->LoadTexture(strPlayerTexturePath);
	}

#pragma region �÷��̾� ��������Ʈ �ʱ�ȭ ���� ���� (�ϴ��� 8��)
	std::array<PlayerSpriteStartupInfo, 8> arrPlayerSpriteStartupInfo;
	arrPlayerSpriteStartupInfo[0].strSpriteName = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(ESceneActorLookAtType::Left)];
	arrPlayerSpriteStartupInfo[0].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorLookAtType::Left)];

	arrPlayerSpriteStartupInfo[1].strSpriteName = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(ESceneActorLookAtType::Right)];
	arrPlayerSpriteStartupInfo[1].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorLookAtType::Right)];

	arrPlayerSpriteStartupInfo[2].strSpriteName = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(ESceneActorLookAtType::Down)];
	arrPlayerSpriteStartupInfo[2].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorLookAtType::Down)];

	arrPlayerSpriteStartupInfo[3].strSpriteName = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(ESceneActorLookAtType::Up)];
	arrPlayerSpriteStartupInfo[3].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorLookAtType::Up)];

	arrPlayerSpriteStartupInfo[4].strSpriteName = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(ESceneActorLookAtType::Left)];
	arrPlayerSpriteStartupInfo[4].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorLookAtType::Left)];
	arrPlayerSpriteStartupInfo[4].spriteLine = 1;

	arrPlayerSpriteStartupInfo[5].strSpriteName = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(ESceneActorLookAtType::Right)];
	arrPlayerSpriteStartupInfo[5].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorLookAtType::Right)];
	arrPlayerSpriteStartupInfo[5].spriteLine = 1;

	arrPlayerSpriteStartupInfo[6].strSpriteName = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(ESceneActorLookAtType::Down)];
	arrPlayerSpriteStartupInfo[6].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorLookAtType::Down)];
	arrPlayerSpriteStartupInfo[6].spriteLine = 1;

	arrPlayerSpriteStartupInfo[7].strSpriteName = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(ESceneActorLookAtType::Up)];
	arrPlayerSpriteStartupInfo[7].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorLookAtType::Up)];
	arrPlayerSpriteStartupInfo[7].spriteLine = 1;
#pragma endregion

	for (const PlayerSpriteStartupInfo& playerSpriteStartupInfo : arrPlayerSpriteStartupInfo)
	{
		// ��������Ʈ �ε��ε� ������ ����
		const DynamicSpritePtr& spPlayerIdleDownDynamicSprite = ResourceMananger::I()->CreateDynamicSprite(playerSpriteStartupInfo.strSpriteName);
		ASSERT_LOG(spPlayerIdleDownDynamicSprite != nullptr);
		spPlayerIdleDownDynamicSprite->FindAndSetTexture(playerSpriteStartupInfo.strUseTexturePath);

		for (int32 i = 0; i < 10; ++i)
		{
			spPlayerIdleDownDynamicSprite->AddKeyFrame(i * 200, playerSpriteStartupInfo.spriteLine * 200, 200, 200, 0.1f, RGB(128, 128, 128));
		}
	}

	// �⺻ ��������Ʈ ���� (PlayerIdleDown)
	const DynamicSpritePtr& spDefaultPlayerSprite = ResourceMananger::I()->FindDynamicSprite(arrPlayerSpriteStartupInfo[2].strSpriteName);
	ASSERT_LOG_RETURN(spDefaultPlayerSprite != nullptr);

	DynamicSpriteComponent* pDynamicSpriteComponent = m_pOwner->AddComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
	pDynamicSpriteComponent->ApplyDynamicSprite(spDefaultPlayerSprite);

//#if 0 // StaticSprite
//	// ��������Ʈ �ε��ε� ������ ����
//	const StaticSpritePtr& spPlayerIdleDownStaticSprite = ResourceMananger::I()->CreateStaticSprite("PlayerIdleDown");
//	ASSERT_LOG(spPlayerIdleDownStaticSprite != nullptr);
//	spPlayerIdleDownStaticSprite->FindAndSetTexture(strPlayerDownTexPath);
//	spPlayerIdleDownStaticSprite->SetBeginDrawPos(0, 0); // �̰� �����ϸ鼭 �׽�Ʈ
//	spPlayerIdleDownStaticSprite->SetDrawSize(200, 200);
//
//	// ��������Ʈ ����
//	StaticSpriteComponent* pStaticSpriteComponent = m_pOwner->AddComponent<StaticSpriteComponent>();
//	ASSERT_LOG(pStaticSpriteComponent != nullptr);
//	pStaticSpriteComponent->SetStaticSprite(spPlayerIdleDownStaticSprite);
//#else
//	// ��������Ʈ �ε��ε� ������ ����
//	const DynamicSpritePtr& spPlayerIdleDownDynamicSprite = ResourceMananger::I()->CreateDynamicSprite("PlayerIdleDown");
//	ASSERT_LOG(spPlayerIdleDownDynamicSprite != nullptr);
//	spPlayerIdleDownDynamicSprite->SetColorKey(RGB(128, 128, 0));
//	spPlayerIdleDownDynamicSprite->FindAndSetTexture(strPlayerDownTexPath);
//
//	for (int32 i = 0; i < 10; ++i)
//	{
//		spPlayerIdleDownDynamicSprite->AddKeyFrame(i * 200, 0, 200, 200, 0.2f);
//	}
//
//	// ��������Ʈ ����
//	DynamicSpriteComponent* pDynamicSpriteComponent = m_pOwner->AddComponent<DynamicSpriteComponent>();
//	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
//	pDynamicSpriteComponent->ApplyDynamicSprite(spPlayerIdleDownDynamicSprite);
//#endif
}

bool PlayerActor::Pimpl::DirectionKeyHandlerImpl(const Vec2d& vMoveDir)
{
	const PlayerStatePtr& spPlayerState = m_pOwner->m_spPlayerState;
	if (std::dynamic_pointer_cast<PlayerWalkState>(spPlayerState) != nullptr)
	{
		DEFAULT_TRACE_LOG("�̵���!");
		return false;
	}

	SceneActorMoveComponent* pMoveComponent = m_pOwner->FindComponent<SceneActorMoveComponent>();
	ASSERT_LOG_RETURN_VALUE(pMoveComponent != nullptr, false);

	// ���� �� ��ǥ ���
	Position2d currentCellPos = pMoveComponent->GetDestinationCellPosition();

	// ��ǥ ���� �ٲٰ�
	pMoveComponent->ApplyMoveDirectionVector(vMoveDir);

	// �̵� ��������?
	const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor = m_pOwner->m_spWorldTileMapActor;
	if (spWorldTileMapActor->CheckMovingAvailableTile(pMoveComponent->GetDestinationCellPosition()) == false)
	{
		// Idle ��������Ʈ�� �ٲٰ�
		const std::string& strPlayerIdleSprite = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(pMoveComponent->GetLookAtType())];
		m_pOwner->ChangePlayerSprite(strPlayerIdleSprite);

		pMoveComponent->SetDestinationCellPosition(currentCellPos);
		pMoveComponent->ResetMoveDirection();
		DEFAULT_TRACE_LOG("�̵� ����!");

		return false;
	}

	// Walk ��������Ʈ �ٲٰ�
	const std::string& strPlayerWalkSprite = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(pMoveComponent->GetLookAtType())];
	m_pOwner->ChangePlayerSprite(strPlayerWalkSprite);

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

#pragma region �Է� ó��
	// �� ���� �̵��̴ϱ� Vector2�� �ذ�

	// �Է°� �����
	InputActionValue inputActionValue;

	// �Է°� �����ϸ鼭 Ʈ���ſ� �ڵ鷯 ����
	InputActionPtr spInputAction = std::make_unique<InputAction>("PlayerWalk");
	spInputAction->ApplyInputActionValue(inputActionValue, EInputActionValueType::Vector2);

	// ������ ���⼭ ���� ���� ����
#pragma region ����Ű�� ���� �� ����
	InputActionMappingInfo leftKeyInputMappingInfo;
	leftKeyInputMappingInfo.inputValue = EInputValue::Left; // ����Ű�� ������ �۵��ϴ� �� �׽�Ʈ
	leftKeyInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed; // �� �ѹ��� ����
	leftKeyInputMappingInfo.inputActionValueModifierBitset.BitOn(EInputActionValueModifierType::Negative);
	spInputAction->AddInputMappingInfo(leftKeyInputMappingInfo);

	InputActionMappingInfo rightKeyInputMappingInfo;
	rightKeyInputMappingInfo.inputValue = EInputValue::Right;
	rightKeyInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed;
	spInputAction->AddInputMappingInfo(rightKeyInputMappingInfo);

	InputActionMappingInfo upKeyInputMappingInfo;
	upKeyInputMappingInfo.inputValue = EInputValue::Up; // ����Ű�� ������ �۵��ϴ� �� �׽�Ʈ
	upKeyInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed; // �� �ѹ��� ����
	upKeyInputMappingInfo.inputActionValueModifierBitset.BitsOn(EInputActionValueModifierType::Swizzle, EInputActionValueModifierType::Negative);
	spInputAction->AddInputMappingInfo(upKeyInputMappingInfo);

	InputActionMappingInfo downKeyInputMappingInfo;
	downKeyInputMappingInfo.inputValue = EInputValue::Down;
	downKeyInputMappingInfo.inputActionValueModifierBitset.BitOn(EInputActionValueModifierType::Swizzle);
	spInputAction->AddInputMappingInfo(downKeyInputMappingInfo);
#pragma endregion
	// ���� ���ؽ�Ʈ �׽�Ʈ
	InputMappingContextPtr spInputMappingContext = std::make_shared<InputMappingContext>();
	spInputMappingContext->AddInputAction(spInputAction);

	// �Է� �ý����� �ٽ�! �Է� ����̽����� ������
	LocalPlayerInputSystem::I()->AddInputMappingContext(spInputMappingContext, 1);

	// ������ ĸó ����� �̿��� �ڵ鷯 ��� ���
	LocalPlayerInputSystem::I()->BindInputActionHandler(spInputAction, this, &PlayerActor::OnDirectionKeyHandler);
#pragma endregion
#pragma endregion

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
	AddComponent<SceneActorMoveComponent>();
	SceneActorMoveComponent* pMoveComponent = FindComponent<SceneActorMoveComponent>();
	pMoveComponent->SetMoveSpeed(90.0f);
	pMoveComponent->SetDestinationCellPosition(GetCellPosition()); // �ʱ�ȭ�ϱ� �Ȱ���

	// ���� ó��
	m_spPlayerState = std::make_shared<PlayerIdleState>(this);

	// ���̾� ó��
	SetRenderingLayer(ERenderingLayerType::Player);
}

bool PlayerActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}
	
	m_playerStateChangeEvent.ExcuteIfBound();
	m_spPlayerState->UpdateState(deltaSeconds);

	return true;
}

void PlayerActor::Cleanup()
{
	return (Super::Cleanup());
}

void PlayerActor::ChangePlayerSprite(const std::string& strNextPlayerSprite)
{
	const DynamicSpritePtr& spNextPlayerSprite = ResourceMananger::I()->FindDynamicSprite(strNextPlayerSprite);
	ASSERT_LOG_RETURN(spNextPlayerSprite != nullptr);

	DynamicSpriteComponent* pDynamicSpriteComponent = FindComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
	pDynamicSpriteComponent->ApplyDynamicSprite(spNextPlayerSprite);
}

const std::string& PlayerActor::FindPlayerIdleSpriteString(ESceneActorLookAtType moveDir) const
{
	return (g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(moveDir)]);
}

const std::string& PlayerActor::FindPlayerWalkSpriteString(ESceneActorLookAtType moveDir) const
{
	return (g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(moveDir)]);
}

void PlayerActor::OnChangePlayerState(const PlayerStatePtr& spNextPlayerState)
{
	DEFAULT_TRACE_LOG("�÷��̾� ���� ����!");
	m_spPlayerState = spNextPlayerState;
}

void PlayerActor::OnDirectionKeyHandler(const InputActionValue* pInputAction)
{
	const Vec2d& vMoveDir = pInputAction->BringValue<Vec2d>();
	if (m_spPimpl->DirectionKeyHandlerImpl(vMoveDir) == false)
	{
		return;
	}

	// ������ ������ ���� ���� ��ȯ (Update���� �����ϱ� ��� ��ȯ�ص� ��)
	PlayerStatePtr spNextPlayerState = m_spPlayerState->ImmediatelyChangePlayerState();
	if (spNextPlayerState != nullptr)
	{
		m_spPlayerState = spNextPlayerState;
	}
}