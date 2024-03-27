#pragma once

#include "IDefault.h"
#include "Defines.h"

class ColliderBase : public IDefault
{
public:
	DEFINE_SMART_PTR(ColliderBase);

public:
	ColliderBase();
	virtual ~ColliderBase();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;
	virtual void Cleanup() override;

	virtual bool TestIntersect(ColliderBase::Ptr spTargetCollider) = 0;
};