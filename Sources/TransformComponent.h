#pragma once

#include "ComponentBase.h"

class TransformComponent : public ComponentBase
{
public:
	TransformComponent();
	virtual ~TransformComponent();

	void AddPosition(const Vec2d& pos);
	void AddPosition(uint32 x, uint32 y);

	Vec2d GetPosition() const { return m_pos; }
	void SetPosition(const Vec2d& pos) { m_pos = pos; }
	void SetPosition(int32 x, int32 y) { m_pos = Vec2d(static_cast<float>(x), static_cast<float>(y)); }

	const Size& GetSize() const { return m_size; }
	void SetSize(const Size& size) { m_size = size; }
	void SetSize(uint32 width, uint32 height) { m_size = Size(width, height); }

private:
	Vec2d m_pos;
	Size m_size;
};