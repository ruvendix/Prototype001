#pragma once

#include "IDefault.h"
#include "Defines.h"

class Actor;

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

	Actor* GetOwner() const { return m_pOwner; }
	void SetOwner(Actor* pOwner) { m_pOwner = pOwner; }

private:
	Actor* m_pOwner = nullptr;
};