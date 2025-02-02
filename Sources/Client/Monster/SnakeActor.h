// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class WorldTileMapActor;

// 일단은 클라이언트에 있는 걸 사용
class SnakeActor : public AnimationActor
{
	DECLARE_PIMPL;
	using Super = AnimationActor;

public:
	using Super::Super;
	virtual ~SnakeActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;
};