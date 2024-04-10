#pragma once

#include "SystemBase.h"
#include "CollisionResponseInfo.h"

class CollisionSystem : public SystemBase
{
public:
	CollisionSystem();
	virtual ~CollisionSystem();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;

	CollisionResponseInfo CreateCollisionResponseInfo(const std::string& strObjectTypeTag);

	int32 AddIntersectedRect(const RECT& intersectedRect);

	const RECT& GetIntersectedRect(int32 intersectedRectIdx) { return m_intersectedRects[intersectedRectIdx]; }

private:
	std::vector<CollisionResponseInfo> m_globalCollisionResponseInfos;
	std::vector<RECT> m_intersectedRects;
};