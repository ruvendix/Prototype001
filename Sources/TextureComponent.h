#pragma once

#include "IGameDefault.h"
#include "ComponentBase.h"
#include "Texture.h"

class TextureComponent : public ComponentBase
{
public:
	DEFINE_SMART_PTR(TextureComponent);

public:
	TextureComponent();
	virtual ~TextureComponent();

	virtual void Render() override;

	Texture::Ptr LoadTexture(const std::string& strTexPath);
	void ChangeTexture(const std::string& strTexPath);

	const Size& GetTextureSize() const { return m_spTex->GetSize(); }

private:
	Texture::Ptr m_spTex = nullptr;
};