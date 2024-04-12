#include "Pch.h"
#include "TileMapActor.h"

#include "GameApplication.h"
#include "SystemManager.h"
#include "ResourceSystem.h"
#include "InputSystem.h"
#include "TransformComponent.h"
#include "TextureComponent.h"
#include "Tile.h"
#include "Sprite.h"
#include "Camera.h"
#include "Texture.h"

void TileMapActor::Startup()
{
	Actor::Startup();

#pragma region 통짜맵 (테스트용으로 넣어보고 굳이 필요한지는 판단함)
	TextureComponentPtr spTexComponent = ADD_COMPONENT(this, TextureComponent);
	TexturePtr spWorldMapTex = spTexComponent->LoadTexture("Sprite/Map/Stage01.bmp");
	GameApplication::I()->SetWorldSize(spWorldMapTex->GetSize());

	// 렌더링할 크기
	const Size& screenSize = GameApplication::I()->GetResolution();
	const Size& screenHalfSize = GameApplication::I()->GetHalfResolution();

	TransformComponentPtr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spTransformComponent->SetPosition(screenHalfSize.width, screenHalfSize.height);
	spTransformComponent->SetSize(screenSize);
#pragma endregion

#pragma region 타일
	// 타일로 사용할 텍스처 로딩
	TexturePtr spTileTex = GET_SYSTEM(ResourceSystem)->LoadTexture("Sprite/Map/Tile.bmp");

	// 타일 하나의 크기
	Size tileSize;
	tileSize.width  = spTileTex->GetSize().width / 4;
	tileSize.height = spTileTex->GetSize().height;
	m_tileMapInfo.tileSize = tileSize;

	// 타일은 스프라이트로 구성
	SpritePtr spTileSprite = GET_SYSTEM(ResourceSystem)->CreateSprite("TileSprite");

	// 스프라이트 정보
	for (uint32 i = 0; i < 4; ++i)
	{
		SpriteInfo tileSpriteInfo;
		tileSpriteInfo.spTex = spTileTex;
		tileSpriteInfo.excludeColor = RGB(128, 128, 128);
		tileSpriteInfo.pos  = Point2d(tileSize.width * i, 0);
		tileSpriteInfo.size = tileSize;
		spTileSprite->AddSpriteInfo(tileSpriteInfo);
	}
#pragma endregion

#pragma region 타일맵 구성
	// 타일 개수 구하기
	uint2d totalTileCount;
	totalTileCount.x = (spWorldMapTex->GetSize().width / tileSize.width);
	totalTileCount.y = (spWorldMapTex->GetSize().height / tileSize.height);
	m_tileMapInfo.totalTileCount = totalTileCount;
	
	m_tileMap.resize(totalTileCount.y);
	for (uint32 y = 0; y < totalTileCount.y; ++y)
	{
		m_tileMap[y].resize(totalTileCount.x);
		for (uint32 x = 0; x < totalTileCount.x; ++x)
		{
			TilePtr spTile = std::make_shared<Tile>();
			spTile->SetTileMapInfo(&m_tileMapInfo);
			spTile->SetTileSprite(spTileSprite);

			Point2d tilePos;
			tilePos.x = x * tileSize.width;
			tilePos.y = y * tileSize.height;

			spTile->SetPosition(tilePos);
			m_tileMap[y][x] = spTile;
		}
	}
#pragma endregion
}

bool TileMapActor::Update()
{
	bool bRet = Actor::Update();

#pragma region 타일 클릭한 경우
	if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::LButton))
	{
		// 클릭한 좌표에서의 인덱스 구하기
		const Point2d& mousePos = GET_SYSTEM(InputSystem)->GetMousePosition();
		
		// 마우스 좌표는 클라이언트 영역 기준이므로 카메라 보정 필요
		CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();

		uint2d selectedTileIdxes;
		selectedTileIdxes.x = (mousePos.x + spCamera->GetOffsetPosition().x) / m_tileMapInfo.tileSize.width;
		selectedTileIdxes.y = (mousePos.y + spCamera->GetOffsetPosition().y) / m_tileMapInfo.tileSize.height;

		m_tileMap[selectedTileIdxes.y][selectedTileIdxes.x]->SetSpriteIndex(2);
	}
#pragma endregion

	return bRet;
}

void TileMapActor::Render()
{
	Actor::Render();

#pragma region 타일 렌더링
	uint2d totalTileCount;
	totalTileCount.y = m_tileMap.size();
	totalTileCount.x = m_tileMap[0].size(); // 첫번째 줄로 확인

	const Size& screenSize = GameApplication::I()->GetResolution();
	CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();

#pragma region 렌더링할 시작 인덱스와 종료 인덱스 및 개수 구하기
	uint2d renderTileStartIdxes;
	renderTileStartIdxes.x = spCamera->GetOffsetPosition().x / m_tileMapInfo.tileSize.width;
	renderTileStartIdxes.y = spCamera->GetOffsetPosition().y / m_tileMapInfo.tileSize.height;

	uint2d renderTileEndIdxes;
	renderTileEndIdxes.x = screenSize.width / m_tileMapInfo.tileSize.width;
	renderTileEndIdxes.y = screenSize.height / m_tileMapInfo.tileSize.height;

	uint2d loopCount;
	loopCount.x = renderTileStartIdxes.x + renderTileEndIdxes.x + 1;
	loopCount.y = renderTileStartIdxes.y + renderTileEndIdxes.y + 1;
#pragma endregion

	//for (uint32 y = renderTileStartIdxes.y; y < loopCount.y; ++y)
	//{
	//	for (uint32 x = renderTileStartIdxes.x; x < loopCount.x; ++x)
	//	{
	//		m_tileMap[y][x]->Render();
	//	}
	//}
#pragma endregion
}