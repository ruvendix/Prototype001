#pragma once

#include "ComponentBase.h"

class CameraComponent : public ComponentBase
{
public:
	CameraComponent();
	virtual ~CameraComponent();

	virtual void Startup() override;

	CameraPtr GetCamera() const { return m_spCamera; }

private:
	CameraPtr m_spCamera = nullptr;
};