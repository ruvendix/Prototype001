// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Component.h"

DEFINE_COMPILETIMER_COUNTER(ComponentIdCounter);

Component::Component()
{

}

Component::~Component()
{

}

void Component::Startup()
{

}

bool Component::Update(float deltaSeconds)
{
	return true;
}

void Component::Cleanup()
{

}

ComponentPtr Component::CreateClone()
{
	return std::make_shared<Component>(*this);
}