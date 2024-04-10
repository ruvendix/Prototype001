#pragma once

#include "ColliderBase.h"

class BoxCollider : public ColliderBase
{
public:
	DEFINE_SMART_PTR(BoxCollider);

public:
	BoxCollider();
	virtual ~BoxCollider();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;

	virtual bool TestIntersect(ColliderBase::Ptr spTargetCollider) override;

	void SetExtents(const Size& extents) { m_extents = extents; }

	const RECT& GetBoxRect() const { return m_boxRect; }
	void SetUseCamera(bool bUseCamera) { m_bUseCamera = bUseCamera; }

	int32 GetInstersectedRectIndex() const { return m_intersectedRectIdx; }

private:
	void UpdateBoxRect();
	bool TestIntersectBox(BoxCollider::Ptr spTargetBoxCollider);

private:
	Size  m_extents;
	RECT  m_boxRect;
	int32 m_intersectedRectIdx = -1;
	bool  m_bUseCamera = true;
};