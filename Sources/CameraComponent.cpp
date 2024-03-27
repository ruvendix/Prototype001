#include "Pch.h"
#include "CameraComponent.h"

#include "Actor.h"
#include "TransformComponent.h"

CameraComponent::CameraComponent()
{

}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::Startup()
{
	m_spCamera = std::make_shared<Camera>();
	m_spCamera->SetTarget(GetOwner());
}