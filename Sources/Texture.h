#pragma once

#include "ResourceBase.h"

class Texture : public ResourceBase
{
public:
	Texture();
	virtual ~Texture();

	virtual void LoadResource(const std::string& strPath) override;

	const Size& GetSize() const { return m_size; }
	const HDC   GetDc() const { return m_hDc; }

private:
	HDC     m_hDc = nullptr;
	HBITMAP m_hBitMap = nullptr;
	Size    m_size;
};