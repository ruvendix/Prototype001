// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class ICoreLoop
{
public:
	ICoreLoop() = default;
	virtual ~ICoreLoop() = default;

	virtual void Startup() { }
	virtual bool Update(float deltaSeconds) { return true; }
	virtual void Cleanup() { }
};