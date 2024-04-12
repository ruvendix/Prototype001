#pragma once

#include "IGameDefault.h"

class Actor;

class ComponentBase : IGameDefault
{
public:
	DEFINE_SMART_PTR(ComponentBase);

public:
	ComponentBase();
	virtual ~ComponentBase();

	virtual void Startup() override;
	virtual bool Update() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	Actor* GetOwner() const { return m_pOwner; }
	void SetOwner(Actor* pOwner) { m_pOwner = pOwner; }

private:
	Actor* m_pOwner;
};