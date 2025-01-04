// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class BaseElement
{
public:
	BaseElement() = default;
	virtual ~BaseElement() = default;

	virtual void Startup();
	virtual bool Update(float deltaSeconds);
	virtual bool PostUpdate(float deltaSeconds);
	virtual void Cleanup();

	virtual void SaveToFileStream();
	virtual void LoadFromFileStream();
};