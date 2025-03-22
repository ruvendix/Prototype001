// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class GameApplication : public ICoreLoop
{
public:
	DECLARE_SINGLETON(GameApplication)

public:
	int32 Drive();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	void SetInstanceHandle(HINSTANCE hGameAppInstance) { m_hGameAppInstance = hGameAppInstance; }
	void SetForceShutdown(bool bForceShutdown) { m_bForceShutdown = bForceShutdown; }

private:
	HINSTANCE m_hGameAppInstance = nullptr;
	bool m_bForceShutdown = false;
};