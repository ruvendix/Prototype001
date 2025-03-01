// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class SceneTracer
{
	DECLARE_SINGLETON(SceneTracer)

public:
	void Tracer();

	RenderComponentVector& GetOutputRenderComponents() { return m_vecOutputRenderComponent; }
	const RenderComponentVector& GetOutputRenderComponents() const { return m_vecOutputRenderComponent; }

private:
	RenderComponentVector m_vecOutputRenderComponent;
};