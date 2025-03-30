// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class GameMonsterFactory : public ICoreLoop
{
	DECLARE_SINGLETON(GameMonsterFactory)

public:
	GameMonsterPtr CreateRandomMonster(uint32 entityId);
};