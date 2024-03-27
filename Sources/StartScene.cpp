/*
<내용 요약>
게임이 실행되면 가장 먼저 작동되는 씬
*/

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

// Ui에 스프라이트를 세팅하는 건 제공되어야함
#include "SystemManager.h"
#include "ResourceSystem.h"
#include "UiButton.h"

void StartScene::Startup()
{
	// 배경을 가진 액터
	CreateActor<TileMapActor>("TileMapActor");

	// 플립북을 가진 액터
	CreateActor<PlayerActor>("Player");

	// 테스트 액터
	CreateActor<MonsterActor>("Monster");

#pragma region 버튼 테스트
	//UiButton::Ptr spUiTestButton = GET_SYSTEM(UiSystem)->CreateUi<UiButton>("TestButton");
	//spUiTestButton->SetPosition(Point2d(300, 300));
	//spUiTestButton->SetSize(Size(200, 200));

	//// 버튼 스프라이트
	//Texture::Ptr spUiButtonTex = GET_SYSTEM(ResourceSystem)->LoadTexture("Sprite/UI/Start.bmp");
	//Sprite::Ptr spUiButtonSprite = GET_SYSTEM(ResourceSystem)->CreateSprite("UiButton");

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

	//// 스타트 씬의 멤버를 호출한다는 가정
	//std::function<void()> funcOnCliked = std::bind(&StartScene::TestFunc, this);
	//spUiTestButton->SetEventListener_OnClicked(funcOnCliked);
#pragma endregion
}

void StartScene::TestFunc()
{
	::OutputDebugString("버튼 클릭이다!\n");
}