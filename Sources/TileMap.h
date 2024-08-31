#pragma once

#include "ResourceBase.h"

class TileMap : public ResourceBase
{
public:
	using TileLine = std::vector<TilePtr>;
	using Tiles = std::vector<TileLine>;

public:
	TileMap(const std::string& strResPath);
	virtual ~TileMap();

	virtual bool Update()  override;
	virtual void Render()  override;

	void SaveResource() override;
	void LoadResource() override;

	TilePtr FindTile(const Int2d& cellPos) const;
	Vec2d ConvertCellPositionToWorldPosition(const Int2d& cellPos) const;

	const Size& GetTileSize() const { return m_tileSize; }
	const Tiles& GetTiles() const { return m_tiles; };

private:
	Tiles m_tiles;
	Size m_tileSize;

	// 타일 개수는 (가로 끝 인덱스 + 1) * (세로 끝 인덱스 + 1)
	Int2d m_tileEnd2dIdx;	
};