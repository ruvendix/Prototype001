// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class WorldBackgroundActor : public Actor
{
	using Super = Actor;

public:
	using Super::Super;
	virtual ~WorldBackgroundActor();

	virtual void Startup() override;
	virtual void Cleanup() override;
};