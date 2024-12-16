// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

DEFINE_COMPILETIMER_COUNTER(ComponentCounter)

class Component : public IGameLoop
{
public:
	Component();
	virtual ~Component();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	template <typename TActor>
	TActor* GetOwner() const
	{
		TActor* pActor = dynamic_cast<TActor*>(m_pOwner);
		ASSERT_LOG(pActor != nullptr);
		return pActor;
	}

	Actor* GetOwner() const { return m_pOwner; }
	void SetOwner(Actor* pOwner) { m_pOwner = pOwner; }

	void SetRenderComponent(bool bRenderComponent) { m_bRenderComponent = bRenderComponent; }
	bool IsRenderComponent() const { return (m_bRenderComponent == true); }

private:
	Actor* m_pOwner = nullptr;
	bool m_bRenderComponent = false;
};