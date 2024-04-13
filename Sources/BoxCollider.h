#pragma once

#include "ColliderBase.h"

class BoxCollider : public ColliderBase
{
public:
	BoxCollider();
	virtual ~BoxCollider();

	virtual void Startup() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;

	virtual bool TestIntersect(ColliderBasePtr spTargetCollider) override;

	void UpdateBoxRect();

	void SetExtents(const Size& extents) { m_extents = extents; }

	const RECT& GetBoxRect() const { return m_boxRect; }
	void SetUseCamera(bool bUseCamera) { m_bUseCamera = bUseCamera; }

	int32 GetInstersectedRectIndex() const { return m_intersectedRectIdx; }
	void SetInstersectedRectIndex(int32 intersectedRectIdx) { m_intersectedRectIdx = intersectedRectIdx; }

private:
	bool TestIntersectBox(BoxColliderPtr spTargetBoxCollider);

private:
	Size  m_extents;
	RECT  m_boxRect;
	int32 m_intersectedRectIdx = -1;
	bool  m_bUseCamera = true;
};