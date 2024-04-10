#pragma once

#include "IDefault.h"

class Actor;

class ComponentBase : IDefault
{
public:
	DEFINE_SMART_PTR(ComponentBase);

public:
	ComponentBase();
	virtual ~ComponentBase();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;
	virtual void Cleanup() override;

	virtual void PostUpdate();

	Actor* GetOwner() const { return m_pOwner; }
	void SetOwner(Actor* pOwner) { m_pOwner = pOwner; }

private:
	Actor* m_pOwner;
};