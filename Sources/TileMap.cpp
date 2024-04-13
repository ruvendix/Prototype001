#include "Pch.h"
#include "TileMap.h"

#include "GameApplication.h"
#include "SystemManager.h"
#include "ResourceSystem.h"
#include "InputSystem.h"
#include "PathSystem.h"
#include "Texture.h"
#include "Sprite.h"
#include "Tile.h"
#include "Camera.h"

TileMap::TileMap(const std::string& strResPath) :
	ResourceBase(strResPath)
{
	AddFileExtension(".tilemap");
}

TileMap::~TileMap()
{

}

bool TileMap::Update()
{
#pragma region Ÿ�� Ŭ���� ���
	if (GET_SYSTEM(InputSystem)->IsKeyDown(EInputValue::LButton))
	{
		// Ŭ���� ��ǥ������ �ε��� ���ϱ�
		const Point2d& mousePos = GET_SYSTEM(InputSystem)->GetMousePosition();

		// ���콺 ��ǥ�� Ŭ���̾�Ʈ ���� �����̹Ƿ� ī�޶� ���� �ʿ�
		CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();

		uint2d selectedTileIdxes;
		selectedTileIdxes.x = (mousePos.x + spCamera->GetOffsetPosition().x) / m_tileSize.width;
		selectedTileIdxes.y = (mousePos.y + spCamera->GetOffsetPosition().y) / m_tileSize.height;

		TilePtr spTile = m_tiles[selectedTileIdxes.y][selectedTileIdxes.x];
		SpritePtr spTileSprite = spTile->GetTileSprite();
		uint32 nextTileSpriteIdx = global::ClampCycleForIndex(spTile->GetTileSpriteIndex() + 1, spTileSprite->GetSpriteInfoCount());
		spTile->SetTileSpriteIndex(nextTileSpriteIdx);
	}
#pragma endregion

	return true;
}

void TileMap::Render()
{
#pragma region Ÿ�� ������
	uint2d totalTileCount;
	totalTileCount.y = m_tiles.size();
	totalTileCount.x = m_tiles[0].size(); // ù��° �ٷ� Ȯ��

	const Size& screenSize = GameApplication::I()->GetResolution();
	CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();

#pragma region �������� ���� �ε����� ���� �ε��� �� ���� ���ϱ�
	uint2d renderTileStartIdxes;
	renderTileStartIdxes.x = spCamera->GetOffsetPosition().x / m_tileSize.width;
	renderTileStartIdxes.y = spCamera->GetOffsetPosition().y / m_tileSize.height;

	uint2d renderTileEndIdxes;
	renderTileEndIdxes.x = screenSize.width / m_tileSize.width;
	renderTileEndIdxes.y = screenSize.height / m_tileSize.height;

	uint2d loopCount;
	loopCount.x = renderTileStartIdxes.x + renderTileEndIdxes.x + 1;
	loopCount.y = renderTileStartIdxes.y + renderTileEndIdxes.y + 1;
#pragma endregion

	for (uint32 y = renderTileStartIdxes.y; y < loopCount.y; ++y)
	{
		for (uint32 x = renderTileStartIdxes.x; x < loopCount.x; ++x)
		{
			m_tiles[y][x]->Render();
		}
	}
#pragma endregion
}

void TileMap::SaveResource()
{
	std::string strTileMapFullPath = GET_SYSTEM(PathSystem)->GetAssetsDirectory();
	strTileMapFullPath += GetResourcePath();

	// Ÿ�ϸ� ���� ��Ʈ�� ����
	FILE* pTileMapFile = nullptr;
	fopen_s(&pTileMapFile, strTileMapFullPath.c_str(), "wb");
	if ((pTileMapFile == nullptr) ||
		(ferror(pTileMapFile) != 0))
	{
		return;
	}

	// Ÿ�� ������� �ؽ�ó ��Ʈ�� ���̺� ����
	std::vector<std::string> spritePathTable;

	for (uint32 y = 0; y < m_totalTileCount.y; ++y)
	{
		for (uint32 x = 0; x < m_totalTileCount.x; ++x)
		{
			TilePtr spTile = m_tiles[y][x];
			SpritePtr spTileSprite = spTile->GetTileSprite();
			spritePathTable.emplace_back(spTileSprite->GetResourcePath());
		}
	}
	spritePathTable.erase(std::unique(spritePathTable.begin(), spritePathTable.end()), spritePathTable.end());

	uint32 spritePathCount = spritePathTable.size();
	fwrite(&spritePathCount, sizeof(uint32), 1, pTileMapFile);

	// Ÿ�� ��������Ʈ ��κ��� ����
	for (uint32 i = 0; i < spritePathCount; ++i)
	{
		global::SaveStringToFile(spritePathTable[i], pTileMapFile);
	}

	// Ÿ�ϸ� ���� ����
	fwrite(&m_totalTileCount, sizeof(uint2d), 1, pTileMapFile);
	fwrite(&m_tileSize, sizeof(Size), 1, pTileMapFile);

	// Ÿ�ϸ��� ���� ����
	for (uint32 y = 0; y < m_totalTileCount.y; ++y)
	{
		for (uint32 x = 0; x < m_totalTileCount.x; ++x)
		{
			TilePtr spTile = m_tiles[y][x];

			// ��������Ʈ ��� �ε����� ����
			SpritePtr spTileSprite = spTile->GetTileSprite();
			const auto& foundIter = std::find(spritePathTable.begin(), spritePathTable.end(), spTileSprite->GetResourcePath());
			uint32 spritePathIdx = std::distance(spritePathTable.begin(), foundIter);
			fwrite(&spritePathIdx, sizeof(uint32), 1, pTileMapFile);

			uint32 tileSpriteIdx = spTile->GetTileSpriteIndex();
			fwrite(&tileSpriteIdx, sizeof(uint32), 1, pTileMapFile);
		}
	}

	fclose(pTileMapFile);
}

void TileMap::LoadResource()
{
	std::string strTileMapFullPath = GET_SYSTEM(PathSystem)->GetAssetsDirectory();
	strTileMapFullPath += GetResourcePath();

	// Ÿ�ϸ� ���� ��Ʈ�� ����
	FILE* pTileMapFile = nullptr;
	fopen_s(&pTileMapFile, strTileMapFullPath.c_str(), "rb");
	if ((pTileMapFile == nullptr) ||
		(ferror(pTileMapFile) != 0))
	{
		return;
	}

	// ��������Ʈ ��Ʈ�� ���̺� �б�
	uint32 spritePathCount = 0;
	fread(&spritePathCount, sizeof(uint32), 1, pTileMapFile);

	std::vector<std::string> spritePathTable;
	for (uint32 i = 0; i < spritePathCount; ++i)
	{
		spritePathTable.emplace_back(global::LoadStringFromFile(pTileMapFile));
	}

	// Ÿ�ϸ� ���� �б�
	fread(&m_totalTileCount, sizeof(uint2d), 1, pTileMapFile);
	fread(&m_tileSize, sizeof(Size), 1, pTileMapFile);

	// Ÿ�ϸ��� ���� �б�
	m_tiles.resize(m_totalTileCount.y);
	for (uint32 y = 0; y < m_totalTileCount.y; ++y)
	{
		m_tiles[y].resize(m_totalTileCount.x);
		for (uint32 x = 0; x < m_totalTileCount.x; ++x)
		{
			TilePtr spTile = std::make_shared<Tile>(this);

			// ��������Ʈ ��� �ε����� ����
			uint32 spritePathIdx = 0;
			fread(&spritePathIdx, sizeof(uint32), 1, pTileMapFile);
			SpritePtr spTileSprite = GET_SYSTEM(ResourceSystem)->LoadSprite(spritePathTable[spritePathIdx]);
			spTile->SetTileSprite(spTileSprite);

			uint32 tileSpriteIdx = spTile->GetTileSpriteIndex();
			fread(&tileSpriteIdx, sizeof(uint32), 1, pTileMapFile);
			spTile->SetTileSpriteIndex(tileSpriteIdx);

			// �ε��� �� Ÿ�� ��ǥ ����
			Point2d tilePos;
			tilePos.x = x * m_tileSize.width;
			tilePos.y = y * m_tileSize.height;
			spTile->SetPosition(tilePos);

			m_tiles[y][x] = spTile;
		}
	}

	fclose(pTileMapFile);
}