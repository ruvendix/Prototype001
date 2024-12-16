// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class IGameLoop
{
public:
	IGameLoop() = default;
	virtual ~IGameLoop() = default;

	virtual void Startup() { }
	virtual bool Update(float deltaSeconds) { return true; }
	virtual bool PostUpdate(float deltaSeconds) { return true; }
	virtual void Cleanup() { }
};