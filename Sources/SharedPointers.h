#pragma once

DEFINE_SMART_PTR(Texture);
DEFINE_SMART_PTR(Sprite);
DEFINE_SMART_PTR(Flipbook);

DEFINE_SMART_PTR(Camera);
DEFINE_SMART_PTR(Tile);

DEFINE_SMART_PTR(SceneBase);

DEFINE_SMART_PTR(Actor);
DEFINE_SMART_PTR(PawnActor);

DEFINE_SMART_PTR(TileMapActor);
DEFINE_SMART_PTR(PlayerActor);
DEFINE_SMART_PTR(MonsterActor);

DEFINE_SMART_PTR(PlayerStateBase);

DEFINE_SMART_PTR(ColliderBase);
DEFINE_SMART_PTR(BoxCollider);

DEFINE_SMART_PTR(ComponentBase);
DEFINE_SMART_PTR(TransformComponent);
DEFINE_SMART_PTR(CameraComponent);
DEFINE_SMART_PTR(CollisionComponent);
DEFINE_SMART_PTR(FlipbookComponent);
DEFINE_SMART_PTR(TextureComponent);

DEFINE_SMART_PTR(UiBase);
DEFINE_SMART_PTR(UiButton);