#pragma once

#include "ComponentBase.h"

class TextureComponent : public ComponentBase
{
public:
	TextureComponent();
	virtual ~TextureComponent();

	virtual void Render() override;

	TexturePtr LoadTexture(const std::string& strTexPath);
	void ChangeTexture(const std::string& strTexPath);

	const Size& BringTextureSize() const;

private:
	TexturePtr m_spTex = nullptr;
};