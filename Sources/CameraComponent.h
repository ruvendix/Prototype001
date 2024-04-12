#pragma once

#include "IGameDefault.h"
#include "ComponentBase.h"
#include "Camera.h"

class CameraComponent : public ComponentBase
{
public:
	DEFINE_SMART_PTR(CameraComponent);

public:
	CameraComponent();
	virtual ~CameraComponent();

	virtual void Startup() override;

	Camera::Ptr GetCamera() const { return m_spCamera; }

private:
	Camera::Ptr m_spCamera = nullptr;
};