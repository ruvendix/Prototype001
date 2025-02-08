// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class SceneTracer
{
	DECLARE_SINGLETON(SceneTracer)

public:
	void Tracer();

	RenderableActorsArray& GetRenderableActors() { return m_arrRenderableActorPtrs; }
	const RenderableActorsArray& GetRenderableActors() const { return m_arrRenderableActorPtrs; }

	void CleanupRenderableActors();

private:
	RenderableActorsArray m_arrRenderableActorPtrs;
};