// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class SceneTracer
{
	DECLARE_SINGLETON(SceneTracer)

public:
	void Tracer();

	LayerActors& GetRenderableActors() { return m_arrRenderableActors; }
	const LayerActors& GetRenderableActors() const { return m_arrRenderableActors; }

	void CleanupRenderableActors();

private:
	LayerActors m_arrRenderableActors;
};