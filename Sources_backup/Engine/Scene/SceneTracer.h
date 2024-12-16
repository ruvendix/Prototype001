// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class SceneTracer
{
	DECLARE_SINGLETON(SceneTracer)

public:
	void Tracer();

	std::vector<ActorPtr>& GetRenderingTargetActors() { return m_vecRenderingTargetActor; }
	const std::vector<ActorPtr>& GetRenderingTargetActors() const { return m_vecRenderingTargetActor; }

private:
	std::vector<ActorPtr> m_vecRenderingTargetActor;
};