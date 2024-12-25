// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class IGameLoop
{
public:
	IGameLoop() = default;
	virtual ~IGameLoop() = default;

	virtual void Startup() { }
	virtual bool Update(float deltaSeconds) { return true; }
	virtual bool PostUpdate(float deltaSeconds) { return true; } // Update의 결과를 사용할 때 필요함
	virtual void Cleanup() { }
};