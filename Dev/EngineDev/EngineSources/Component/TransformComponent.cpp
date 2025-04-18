// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "TransformComponent.h"

DEFINE_COMPILETIME_ID(TransformComponent, ComponentIdCounter)

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

ComponentPtr TransformComponent::CreateClone()
{
	return std::make_shared<TransformComponent>(*this);
}

void TransformComponent::BuildWolrdMatrix()
{

}