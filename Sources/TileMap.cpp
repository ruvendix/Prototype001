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
#pragma region 타일 클릭한 경우
	if (GET_SYSTEM(InputSystem)->IsKeyDown(EInputValue::LButton))
	{
		// 클릭한 좌표에서의 인덱스 구하기
		const Int2d& mousePos = GET_SYSTEM(InputSystem)->GetMousePosition();

		// 마우스 좌표는 클라이언트 영역 기준이므로 카메라 보정 필요
		CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();

		Int2d selectedTileIdxes;
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
#pragma region 타일 렌더링
	Int2d totalTileCount;
	totalTileCount.y = m_tiles.size();
	totalTileCount.x = m_tiles[0].size(); // 첫번째 줄로 확인

	const Size& screenSize = GameApplication::I()->GetResolution();
	CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();

#pragma region 렌더링할 시작 인덱스와 종료 인덱스 및 개수 구하기
	Int2d renderTileStartIdxes;
	renderTileStartIdxes.x = spCamera->GetOffsetPosition().x / m_tileSize.width;
	renderTileStartIdxes.y = spCamera->GetOffsetPosition().y / m_tileSize.height;

	Int2d renderTileEndIdxes;
	renderTileEndIdxes.x = screenSize.width / m_tileSize.width;
	renderTileEndIdxes.y = screenSize.height / m_tileSize.height;

	Int2d loopCount;
	loopCount.x = renderTileStartIdxes.x + renderTileEndIdxes.x + 1;
	loopCount.y = renderTileStartIdxes.y + renderTileEndIdxes.y + 1;
#pragma endregion

	for (int32 y = renderTileStartIdxes.y; y < loopCount.y; ++y)
	{
		for (int32 x = renderTileStartIdxes.x; x < loopCount.x; ++x)
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

	// 타일맵 파일 스트림 생성
	FILE* pTileMapFile = nullptr;
	fopen_s(&pTileMapFile, strTileMapFullPath.c_str(), "wb");
	if ((pTileMapFile == nullptr) ||
		(ferror(pTileMapFile) != 0))
	{
		return;
	}

	// 타일 정보들로 텍스처 스트링 테이블 구성
	std::vector<std::string> spritePathTable;

	for (int32 y = 0; y <= m_tileEnd2dIdx.y; ++y)
	{
		for (int32 x = 0; x <= m_tileEnd2dIdx.x; ++x)
		{
			TilePtr spTile = m_tiles[y][x];
			SpritePtr spTileSprite = spTile->GetTileSprite();
			spritePathTable.emplace_back(spTileSprite->GetResourcePath());
		}
	}
	spritePathTable.erase(std::unique(spritePathTable.begin(), spritePathTable.end()), spritePathTable.end());

	uint32 spritePathCount = spritePathTable.size();
	fwrite(&spritePathCount, sizeof(uint32), 1, pTileMapFile);

	// 타일 스프라이트 경로부터 저장
	for (uint32 i = 0; i < spritePathCount; ++i)
	{
		global::SaveStringToFile(spritePathTable[i], pTileMapFile);
	}

	// 타일맵 정보 쓰기
	Int2d tile2dCount = m_tileEnd2dIdx + 1;
	fwrite(&tile2dCount, sizeof(Int2d), 1, pTileMapFile);
	fwrite(&m_tileSize, sizeof(Size), 1, pTileMapFile);

	// 타일마다 정보 쓰기
	for (int32 y = 0; y < tile2dCount.y; ++y)
	{
		for (int32 x = 0; x < tile2dCount.x; ++x)
		{
			TilePtr spTile = m_tiles[y][x];

			// 스프라이트 경로 인덱스를 저장
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

	// 타일맵 파일 스트림 생성
	FILE* pTileMapFile = nullptr;
	fopen_s(&pTileMapFile, strTileMapFullPath.c_str(), "rb");
	if ((pTileMapFile == nullptr) ||
		(ferror(pTileMapFile) != 0))
	{
		return;
	}

	// 스프라이트 스트링 테이블 읽기
	uint32 spritePathCount = 0;
	fread(&spritePathCount, sizeof(uint32), 1, pTileMapFile);

	std::vector<std::string> spritePathTable;
	for (uint32 i = 0; i < spritePathCount; ++i)
	{
		spritePathTable.emplace_back(global::LoadStringFromFile(pTileMapFile));
	}

	// 타일맵 개수 정보 읽기
	Int2d tile2dCount; // 1x1이라면 x와 y는 각각 1인데 이거 이름 잘못지음
	fread(&tile2dCount, sizeof(Int2d), 1, pTileMapFile);
	m_tileEnd2dIdx = tile2dCount - 1;

	// 타일의 각 사이즈 읽기
	fread(&m_tileSize, sizeof(Size), 1, pTileMapFile);

	// 타일마다 정보 읽기
	m_tiles.resize(tile2dCount.y);
	for (int32 y = 0; y < tile2dCount.y; ++y)
	{
		m_tiles[y].resize(tile2dCount.x);
		for (int32 x = 0; x < tile2dCount.x; ++x)
		{
			TilePtr spTile = std::make_shared<Tile>(this);

			// 스프라이트 경로 인덱스를 저장
			uint32 spritePathIdx = 0;
			fread(&spritePathIdx, sizeof(uint32), 1, pTileMapFile);
			SpritePtr spTileSprite = GET_SYSTEM(ResourceSystem)->LoadSprite(spritePathTable[spritePathIdx]);
			spTile->SetTileSprite(spTileSprite);

			uint32 tileSpriteIdx = spTile->GetTileSpriteIndex();
			fread(&tileSpriteIdx, sizeof(uint32), 1, pTileMapFile);
			spTile->SetTileSpriteIndex(tileSpriteIdx);

			// 로딩할 때 타일 좌표 설정
			Int2d tilePos;
			tilePos.x = x * m_tileSize.width;
			tilePos.y = y * m_tileSize.height;
			spTile->SetPosition(tilePos);

			m_tiles[y][x] = spTile;
		}
	}

	fclose(pTileMapFile);
}

TilePtr TileMap::FindTile(const Int2d& cellPos) const
{
	if ((cellPos.x < 0) ||
		(cellPos.x > m_tileEnd2dIdx.x) ||
		(cellPos.y < 0) ||
		(cellPos.y > m_tileEnd2dIdx.y))
	{
		return nullptr;
	}

	return m_tiles[cellPos.y][cellPos.x];
}

Vec2d TileMap::ConvertCellPositionToWorldPosition(const Int2d& cellPos) const
{
	Vec2d worldPos;
	worldPos.x = (m_tileSize.width * cellPos.x) + (m_tileSize.width / 2);
	worldPos.y = (m_tileSize.height * cellPos.y) + (m_tileSize.height / 2);
	return worldPos;
}