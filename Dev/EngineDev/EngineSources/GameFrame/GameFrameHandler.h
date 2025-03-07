// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class GameFrameHandler : public ICoreLoop
{
	DECLARE_SINGLETON(GameFrameHandler)

public:
	virtual void Startup() override;

	void SyncFrameRate();
	void ProcessFramePerSeconds();
	void ApplyFrameRate(int32 frameRate);

	float GetDeltaSeconds() const { return m_deltaSeconds; }

	void OnTimerFps();

private:
	LARGE_INTEGER m_prevFrameTime;
	int32 m_frameRate = 0; /// -1이면 무제한 프레임

	int32 m_frameCount = 0;
	int32 m_frameCountBeforeSecond = 0; // 이전 프레임 때의 프레임 카운트

	float m_targetFrameRateSeconds = 0.0f;
	float m_deltaSeconds = 0.0f;

	Timer m_frameTimer;
};