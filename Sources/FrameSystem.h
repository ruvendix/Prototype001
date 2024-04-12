#pragma once

#include "SystemBase.h"

class FrameSystem : public SystemBase
{
public:
	FrameSystem();
	virtual ~FrameSystem();

	virtual void Startup() override;
	virtual bool Update() override;

	float GetDeltaSeconds() const { return m_deltaSeconds; }
	const std::string& GetFrameGuide() const { return m_strFrameGuide; }

private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_prevCounter;
	
	float m_deltaSeconds = 0.0f;
	uint32 m_frameCount = 0;
	uint32 m_prevFpsFrameCount = 0;
	uint32 m_fps = 0;

	std::string m_strFrameGuide;
};