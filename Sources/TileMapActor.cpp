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

#pragma region ��¥�� (�׽�Ʈ������ �־�� ���� �ʿ������� �Ǵ���)
	TextureComponentPtr spTexComponent = ADD_COMPONENT(this, TextureComponent);
	TexturePtr spWorldMapTex = spTexComponent->LoadTexture("Sprite/Map/Stage01.bmp");
	GameApplication::I()->SetWorldSize(spWorldMapTex->GetSize());

	// �������� ũ��
	const Size& screenSize = GameApplication::I()->GetResolution();
	const Size& screenHalfSize = GameApplication::I()->GetHalfResolution();

	TransformComponentPtr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spTransformComponent->SetPosition(screenHalfSize.width, screenHalfSize.height);
	spTransformComponent->SetSize(screenSize);
#pragma endregion

#pragma region Ÿ��
	// Ÿ�Ϸ� ����� �ؽ�ó �ε�
	TexturePtr spTileTex = GET_SYSTEM(ResourceSystem)->LoadTexture("Sprite/Map/Tile.bmp");

	// Ÿ�� �ϳ��� ũ��
	Size tileSize;
	tileSize.width  = spTileTex->GetSize().width / 4;
	tileSize.height = spTileTex->GetSize().height;
	m_tileMapInfo.tileSize = tileSize;

	// Ÿ���� ��������Ʈ�� ����
	SpritePtr spTileSprite = GET_SYSTEM(ResourceSystem)->CreateSprite("TileSprite");

	// ��������Ʈ ����
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

#pragma region Ÿ�ϸ� ����
	// Ÿ�� ���� ���ϱ�
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

#pragma region Ÿ�� Ŭ���� ���
	if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::LButton))
	{
		// Ŭ���� ��ǥ������ �ε��� ���ϱ�
		const Point2d& mousePos = GET_SYSTEM(InputSystem)->GetMousePosition();
		
		// ���콺 ��ǥ�� Ŭ���̾�Ʈ ���� �����̹Ƿ� ī�޶� ���� �ʿ�
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

#pragma region Ÿ�� ������
	uint2d totalTileCount;
	totalTileCount.y = m_tileMap.size();
	totalTileCount.x = m_tileMap[0].size(); // ù��° �ٷ� Ȯ��

	const Size& screenSize = GameApplication::I()->GetResolution();
	CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();

#pragma region �������� ���� �ε����� ���� �ε��� �� ���� ���ϱ�
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