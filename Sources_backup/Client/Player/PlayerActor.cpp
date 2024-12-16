// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerActor.h"

#include "PlayerImplementation.h"

namespace
{
	std::array<std::string, TO_NUM(ESceneActorMoveDirection::Count)> g_arrPlayerIdleStateDynamicSpriteStringTable =
	{
		"PlayerIdleLeft",
		"PlayerIdleRight"
		"PlayerIdleDown",
		"PlayerIdleUp"
	};

	std::array<std::string, TO_NUM(ESceneActorMoveDirection::Count)> g_arrPlayerWalkStateDynamicSpriteStringTable =
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

// �̰� �̺�Ʈ �����ʿ� ����?
CLASS_PIMPL(PlayerActor)
{
	DEFINE_PIMPL(PlayerActor);

public:
	void LoadAndStartupPlayerSprite();
	bool ProcessKeyboardDownImpl(ESceneActorMoveDirection moveDir);
};

void PlayerActor::Pimpl::LoadAndStartupPlayerSprite()
{
	std::array<std::string, TO_NUM(ESceneActorMoveDirection::Count)> arrPlayerTexturePathTable =
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
	arrPlayerSpriteStartupInfo[0].strSpriteName = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Left)];
	arrPlayerSpriteStartupInfo[0].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Left)];

	arrPlayerSpriteStartupInfo[1].strSpriteName = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Right)];
	arrPlayerSpriteStartupInfo[1].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Right)];

	arrPlayerSpriteStartupInfo[2].strSpriteName = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Down)];
	arrPlayerSpriteStartupInfo[2].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Down)];

	arrPlayerSpriteStartupInfo[3].strSpriteName = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Up)];
	arrPlayerSpriteStartupInfo[3].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Up)];

	arrPlayerSpriteStartupInfo[4].strSpriteName = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Left)];
	arrPlayerSpriteStartupInfo[4].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Left)];
	arrPlayerSpriteStartupInfo[4].spriteLine = 1;

	arrPlayerSpriteStartupInfo[5].strSpriteName = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Right)];
	arrPlayerSpriteStartupInfo[5].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Right)];
	arrPlayerSpriteStartupInfo[5].spriteLine = 1;

	arrPlayerSpriteStartupInfo[6].strSpriteName = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Down)];
	arrPlayerSpriteStartupInfo[6].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Down)];
	arrPlayerSpriteStartupInfo[6].spriteLine = 1;

	arrPlayerSpriteStartupInfo[7].strSpriteName = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Up)];
	arrPlayerSpriteStartupInfo[7].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Up)];
	arrPlayerSpriteStartupInfo[7].spriteLine = 1;
#pragma endregion

	for (const PlayerSpriteStartupInfo& playerSpriteStartupInfo : arrPlayerSpriteStartupInfo)
	{
		// ��������Ʈ �ε��ε� ������ ����
		const DynamicSpritePtr& spPlayerIdleDownDynamicSprite = ResourceMananger::I()->CreateDynamicSprite(playerSpriteStartupInfo.strSpriteName);
		ASSERT_LOG(spPlayerIdleDownDynamicSprite != nullptr);
		spPlayerIdleDownDynamicSprite->SetColorKey(RGB(128, 128, 128));
		spPlayerIdleDownDynamicSprite->FindAndSetTexture(playerSpriteStartupInfo.strUseTexturePath);

		for (int32 i = 0; i < 10; ++i)
		{
			spPlayerIdleDownDynamicSprite->AddKeyFrame(i * 200, playerSpriteStartupInfo.spriteLine * 200, 200, 200, 0.1f);
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

bool PlayerActor::Pimpl::ProcessKeyboardDownImpl(ESceneActorMoveDirection moveDir)
{
	SceneActorMoveComponent* pMoveComponent = m_pOwner->FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->IsMoving())
	{
		return false;
	}

	// ��ǥ ���� �ٲٰ�
	pMoveComponent->ApplyMoveDirection(moveDir);

	// ��������Ʈ �ٲٰ�
	const std::string& strPlayerSprite = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(pMoveComponent->GetMoveDirection())];
	m_pOwner->ChangePlayerSprite(strPlayerSprite);

	// ������ ���� ���� �ݿ�
	//m_spPlayerImpl->ProcessPlayerInput();

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

	CREATE_PIMPL;

	// �÷��̾� ��������Ʈ �ε� �� �ʱ�ȭ
	m_spPimpl->LoadAndStartupPlayerSprite();

#pragma region �Է� ó��
	UserInputCallback leftKeyDownCallback = std::bind(&PlayerActor::OnKeyboardDown_Left, this);
	KeyboardInputHandler::I()->BindKeyboardInput(EKeyboardValue::Left, leftKeyDownCallback, nullptr, nullptr);

	UserInputCallback rightKeyDownCallback = std::bind(&PlayerActor::OnKeyboardDown_Right, this);
	KeyboardInputHandler::I()->BindKeyboardInput(EKeyboardValue::Right, rightKeyDownCallback, nullptr, nullptr);

	UserInputCallback downKeyDownCallback = std::bind(&PlayerActor::OnKeyboardDown_Down, this);
	KeyboardInputHandler::I()->BindKeyboardInput(EKeyboardValue::Down, downKeyDownCallback, nullptr, nullptr);

	UserInputCallback upKeyDownCallback = std::bind(&PlayerActor::OnKeyboardDown_Up, this);
	KeyboardInputHandler::I()->BindKeyboardInput(EKeyboardValue::Up, upKeyDownCallback, nullptr, nullptr);
#pragma endregion

#pragma region �÷��̾� �⺻ ���� �ʱ�ȭ
	TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG(pTransformComponent != nullptr);

	// ��ǥ�� ũ�� ����
	//pTransformComponent->SetPosition(0.0f, 0.0f);
	pTransformComponent->SetSize(200, 200);

	// �� ũ�⸦ 200���� �����ϸ�?
	ApplyCellPosition(0, 0);
#pragma endregion

	// �̵� ó��
	SceneActorMoveComponent* pMoveComponent = FindComponent<SceneActorMoveComponent>();
	pMoveComponent->SetMoveSpeed(200.0f);
	pMoveComponent->SetMoveCellPosition(GetCellPosition()); // �ʱ�ȭ�ϱ� �Ȱ���

	// ���� ó��
	m_spPlayerImpl = std::make_unique<PlayerImplementation>(this);
	m_spPlayerImpl->Startup();
}

bool PlayerActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	m_spPlayerImpl->Update(deltaSeconds);
	return true;
}

bool PlayerActor::PostUpdate(float deltaSeconds)
{
	return (Super::PostUpdate(deltaSeconds));
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

void PlayerActor::OnKeyboardDown_Left()
{
	DEFAULT_TRACE_LOG("���� Ű");

	// ���������� ��ġ�� �Լ����� ����
	m_spPimpl->ProcessKeyboardDownImpl(ESceneActorMoveDirection::Left);

	SceneActorMoveComponent* pMoveComponent = FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->IsMoving())
	{
		return;
	}

	// ��ǥ ���� �ٲٰ�
	pMoveComponent->ApplyMoveDirection(ESceneActorMoveDirection::Left);

	// ��������Ʈ �ٲٰ�
	const std::string& strPlayerSprite = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(pMoveComponent->GetMoveDirection())];
	ChangePlayerSprite(strPlayerSprite);

	// ������ ���� ���� �ݿ�
	m_spPlayerImpl->ProcessPlayerInput();
}

void PlayerActor::OnKeyboardDown_Right()
{
	DEFAULT_TRACE_LOG("������ Ű");

	// ��������Ʈ �ٲٰ�

	// ��ǥ ���� �����ϰ�
	SceneActorMoveComponent* pMoveComponent = FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->IsMoving())
	{
		return;
	}

	// ������ ���� ���� �ݿ�
	pMoveComponent->ApplyMoveDirection(ESceneActorMoveDirection::Right);
	m_spPlayerImpl->ProcessPlayerInput();
}

void PlayerActor::OnKeyboardDown_Down()
{
	DEFAULT_TRACE_LOG("�Ʒ��� Ű");

	// ��������Ʈ �ٲٰ�

	// ��ǥ ���� �����ϰ�
	SceneActorMoveComponent* pMoveComponent = FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->IsMoving())
	{
		return;
	}

	// ������ ���� ���� �ݿ�
	pMoveComponent->ApplyMoveDirection(ESceneActorMoveDirection::Down);
	m_spPlayerImpl->ProcessPlayerInput();
}

void PlayerActor::OnKeyboardDown_Up()
{
	DEFAULT_TRACE_LOG("���� Ű");

	// ��������Ʈ �ٲٰ�

	// ��ǥ ���� �����ϰ�
	SceneActorMoveComponent* pMoveComponent = FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->IsMoving())
	{
		return;
	}

	// ������ ���� ���� �ݿ�
	pMoveComponent->ApplyMoveDirection(ESceneActorMoveDirection::Up);
	m_spPlayerImpl->ProcessPlayerInput();
}