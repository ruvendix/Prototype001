#pragma once

#include "IGameDefault.h"
#include "ComponentBase.h"

class TransformComponent : public ComponentBase
{
public:
	DEFINE_SMART_PTR(TransformComponent);

public:
	TransformComponent();
	virtual ~TransformComponent();

	void AddPosition(const Point2d& pos);
	void AddPosition(uint32 x, uint32 y);

	const Point2d& GetPosition() const { return m_pos; }
	void SetPosition(const Point2d& pos) { m_pos = pos; }
	void SetPosition(int32 x, int32 y) { m_pos = Point2d(x, y); }

	const Size& GetSize() const { return m_size; }
	void SetSize(const Size& size) { m_size = size; }
	void SetSize(uint32 width, uint32 height) { m_size = Size(width, height); }

private:
	Point2d m_pos;
	Size    m_size;
};