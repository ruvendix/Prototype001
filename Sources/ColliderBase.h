#pragma once

#include "IGameDefault.h"

class Actor;

class ColliderBase : public IGameDefault
{
public:
	ColliderBase();
	virtual ~ColliderBase();

	virtual void Startup() override;
	virtual bool Update() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	virtual bool TestIntersect(ColliderBasePtr spTargetCollider) = 0;

	Actor* GetActor() const { return m_pActor; }
	void SetActor(Actor* pActor) { m_pActor = pActor; }

	void SetDebugRender(bool bDebugRedner) { m_bDebugRender = bDebugRedner; }
	bool IsDebugRender() const { return (m_bDebugRender == true); }

private:
	Actor* m_pActor = nullptr;
	bool m_bDebugRender = false;
};