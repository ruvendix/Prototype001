#pragma once

#include "IDefault.h"
#include "Sprite.h"

class UiBase : public IDefault
{
public:
	DEFINE_SMART_PTR(UiBase);

public:
	UiBase();
	virtual ~UiBase();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;
	virtual void Cleanup() override;

	virtual bool TestMouseHit() = 0;

	void SetSprite(Sprite::Ptr spUiSprite) { m_spUiSprite = spUiSprite; }

	const Point2d& GetPosition() const { return m_pos; }
	void SetPosition(const Point2d& pos) { m_pos = pos; }

	const Size& GetSize() const { return m_size; }
	void SetSize(const Size& size) { m_size = size; }

	void SetUiSpriteIndex(uint32 uiSpriteIdx) { m_uiSpriteIdx = uiSpriteIdx; }

private:
	Point2d     m_pos;
	Size        m_size;
	Sprite::Ptr m_spUiSprite = nullptr;
	uint32      m_uiSpriteIdx = 0;
};