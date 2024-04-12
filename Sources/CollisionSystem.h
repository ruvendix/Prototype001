#pragma once

#include "SystemBase.h"
#include "CollisionResponseInfo.h"

class CollisionSystem : public SystemBase
{
public:
	CollisionSystem();
	virtual ~CollisionSystem();

	virtual bool PostUpdate() override;
	virtual void Render() override;

	int32 AddIntersectedRect(const RECT& intersectedRect);

	const RECT& GetIntersectedRect(int32 intersectedRectIdx) { return m_intersectedRects[intersectedRectIdx]; }

private:
	std::vector<RECT> m_intersectedRects;
};