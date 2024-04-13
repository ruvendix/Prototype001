#include "Pch.h"
#include "TileMapActor.h"

#include "GameApplication.h"
#include "SystemManager.h"
#include "ResourceSystem.h"
#include "InputSystem.h"
#include "TransformComponent.h"
#include "TextureComponent.h"
#include "CollisionComponent.h"
#include "Tile.h"
#include "Sprite.h"
#include "Camera.h"
#include "Texture.h"
#include "TileMap.h"
#include "BoxCollider.h"

void TileMapActor::Startup()
{
	Super::Startup();

#pragma region ��¥�� (�׽�Ʈ������ �־�� ���� �ʿ������� �Ǵ���)
	TextureComponentPtr spTexComponent = ADD_COMPONENT(this, TextureComponent);
	TexturePtr spWorldMapTex = spTexComponent->LoadTexture("Texture/Map/Stage01.bmp");
	GameApplication::I()->SetWorldSize(spWorldMapTex->GetSize());

	// �������� ũ��
	const Size& screenSize = GameApplication::I()->GetResolution();
	const Size& screenHalfSize = GameApplication::I()->GetHalfResolution();

	TransformComponentPtr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spTransformComponent->SetPosition(screenHalfSize.width, screenHalfSize.height);
	spTransformComponent->SetSize(screenSize);
#pragma endregion

	m_spTileMap = GET_SYSTEM(ResourceSystem)->LoadTileMap("Texture/Map/TileMap");
	m_spTileMap->Startup();

	// X ǥ�ð� �� �͵鸸 ���ͷ� ����
	const TileMap::Tiles& tiles = m_spTileMap->GetTiles();
	for (uint32 y = 0; y < tiles.size(); ++y)
	{
		for (uint32 x = 0; x < tiles[0].size(); ++x)
		{
			TilePtr spTile = tiles[y][x];
			if (spTile->GetTileSpriteIndex() != 1)
			{
				continue;
			}

			// �⺻ ���Ϳ� ������Ʈ�� ����
			Actor* pTileActor = new Actor;
			pTileActor->Startup();
			pTileActor->SetActorState(EActorState::Deactivated);

			// Ʈ�������� ������ ������� ���е�
			TransformComponentPtr spTileTransformComponent = GET_COMPONENT(pTileActor, TransformComponent);
			const Size& tileHalfSize = m_spTileMap->GetTileSize() / 2;
			Point2d tileCenterPos = spTile->GetPosition() + tileHalfSize;
			spTileTransformComponent->SetPosition(tileCenterPos);

			// �ݶ��̴� ����
			BoxColliderPtr spBoxCollider = std::make_shared<BoxCollider>();
			spBoxCollider->SetExtents(tileHalfSize);

			CollisionComponentPtr spCollisionComponent = ADD_COMPONENT(pTileActor, CollisionComponent);
			spCollisionComponent->ApplyCollider(spBoxCollider);
			spCollisionComponent->SetCollisionObjectType(ECollisionObjectType::WorldStatic);

			AddChild(pTileActor);
		}
	}
}

bool TileMapActor::Update()
{
	if (Super::Update() == false)
	{
		return false;
	}

	if (m_spTileMap->Update() == false)
	{
		return false;
	}

	// ���� ���� ����
	const std::vector<Actor*> tileActors = GetChilds();
	uint32 tileActorCount = tileActors.size();
	for (uint32 i = 0; i < tileActorCount; ++i)
	{
		tileActors[i]->SetActorState(EActorState::Deactivated);
	}

#pragma region ���� ���̴� Ÿ�ϸ� Ȱ��ȭ
	const Size& screenSize = GameApplication::I()->GetResolution();
	CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();

	for (uint32 i = 0; i < tileActorCount; ++i)
	{
		TransformComponentPtr spTileTransformComponent = GET_COMPONENT(tileActors[i], TransformComponent);

		Point2d tileOffsetCenterPos = spTileTransformComponent->GetPosition() - spCamera->GetOffsetPosition();
		if (((0 <= tileOffsetCenterPos.x) && (tileOffsetCenterPos.x <= static_cast<int32>(screenSize.width))) &&
			((0 <= tileOffsetCenterPos.y) && (tileOffsetCenterPos.y <= static_cast<int32>(screenSize.height))))
		{
			tileActors[i]->SetActorState(EActorState::Activated);
		}
	}
#pragma endregion

	return true;
}

void TileMapActor::Render()
{
	Super::Render();
	m_spTileMap->Render();
}