#pragma once

#include "ResourceBase.h"

class Texture : public ResourceBase
{
public:
	using ResourceBase::ResourceBase;
	virtual ~Texture();

	virtual void SaveResource() override;
	virtual void LoadResource() override;

	const Size& GetSize() const { return m_size; }
	const HDC   GetDc() const { return m_hDc; }

private:
	HDC m_hDc = nullptr;
	HBITMAP m_hBitMap = nullptr;
	Size m_size;
};