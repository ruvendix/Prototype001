#pragma once

#include "ColliderBase.h"

class BoxCollider : public ColliderBase
{
public:
	DEFINE_SMART_PTR(BoxCollider);

public:
	BoxCollider();
	virtual ~BoxCollider();

	virtual bool Update() override;
	virtual void Render() override;

	virtual bool TestIntersect(ColliderBase::Ptr spTargetCollider) override;

	void SetPosition(const Point2d& pos) { m_pos = pos; }
	void SetExtents(const Size& extents) { m_extents = extents; }

	const RECT& GetBoxRect() const { return m_boxRect; }
	void SetUseCamera(bool bUseCamera) { m_bUseCamera = bUseCamera; }

private:
	Point2d m_pos;
	Size    m_extents;
	RECT    m_boxRect;
	bool    m_bUseCamera = true;
};