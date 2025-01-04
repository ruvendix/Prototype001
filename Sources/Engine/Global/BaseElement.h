// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class FileStream;

class BaseElement
{
public:
	BaseElement() = default;
	virtual ~BaseElement() = default;

	virtual void Startup();
	virtual bool Update(float deltaSeconds);
	virtual bool PostUpdate(float deltaSeconds);
	virtual void Cleanup();

	virtual void SaveToFileStream(const FileStream& fileStream) const;
	virtual void LoadFromFileStream(const FileStream& fileStream);
};