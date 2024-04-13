#include "Pch.h"
#include "StartScene.h"

#include "GameApplication.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "TextureComponent.h"
#include "TileMapActor.h"
#include "PlayerActor.h"
#include "MonsterActor.h"
#include "UiSystem.h"

// Ui�� ��������Ʈ�� �����ϴ� �� �����Ǿ����
#include "SystemManager.h"
#include "ResourceSystem.h"
#include "UiButton.h"

void StartScene::Startup()
{
	// ����� ���� ����
	CreateActor<TileMapActor>("TileMapActor", EActorLayer::World);

	// �ø����� ���� ����
	CreateActor<PlayerActor>("Player", EActorLayer::Character);

	// �׽�Ʈ ����
	//CreateActor<MonsterActor>("Monster", EActorLayer::Character);

#pragma region ��ư �׽�Ʈ
	//UiButtonPtr spUiTestButton = GET_SYSTEM(UiSystem)->CreateUi<UiButton>("TestButton");
	//spUiTestButton->SetPosition(Point2d(300, 300));
	//spUiTestButton->SetSize(Size(200, 200));

	//// ��ư ��������Ʈ
	//TexturePtr spUiButtonTex = GET_SYSTEM(ResourceSystem)->LoadTexture("Texture/UI/Start.bmp");
	//SpritePtr spUiButtonSprite = GET_SYSTEM(ResourceSystem)->CreateSprite("UiButton");

	//SpriteInfo uiButtonSpriteInfo[2];
	//uiButtonSpriteInfo[0].pos = Point2d(0, 0);
	//uiButtonSpriteInfo[0].size = Size(150, 150);
	//uiButtonSpriteInfo[0].spTex = spUiButtonTex;
	//uiButtonSpriteInfo[0].excludeColor = RGB(255, 255, 255);
	//spUiButtonSprite->AddSpriteInfo(uiButtonSpriteInfo[0]);

	//uiButtonSpriteInfo[1].pos = Point2d(150, 0);
	//uiButtonSpriteInfo[1].size = Size(150, 150);
	//uiButtonSpriteInfo[1].spTex = spUiButtonTex;
	//uiButtonSpriteInfo[1].excludeColor = RGB(255, 255, 255);
	//spUiButtonSprite->AddSpriteInfo(uiButtonSpriteInfo[1]);

	//spUiTestButton->SetSprite(spUiButtonSprite);

	//// ��ŸƮ ���� ����� ȣ���Ѵٴ� ����
	//std::function<void()> funcOnCliked = std::bind(&StartScene::TestFunc, this);
	//spUiTestButton->SetEventListener_OnClicked(funcOnCliked);
#pragma endregion
}

void StartScene::TestFunc()
{
	::OutputDebugString("��ư Ŭ���̴�!\n");
}