// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "TransformComponent.h"

DEFINE_COMPILETIME_ID(TransformComponent, ComponentCounter)

TransformComponent::TransformComponent()
{

}

TransformComponent::~TransformComponent()
{

}

void TransformComponent::Startup()
{
}

bool TransformComponent::Update(float deltaSeconds)
{
	return false;
}

void TransformComponent::Cleanup()
{

}

void TransformComponent::BuildWolrdMatrix()
{

}