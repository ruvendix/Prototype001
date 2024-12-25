// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class WorldTileMapActor : public Actor
{
	using Super = Actor;

public:
	using Super::Super;
	virtual ~WorldTileMapActor() override;

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;
};