// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	Cleanup();
}

void Scene::Startup()
{

}

bool Scene::Update(float deltaSeconds)
{
	for (ActorPtr& spActor : m_vecActors)
	{
		spActor->Update(deltaSeconds);
	}

	return true;
}

bool Scene::PostUpdate(float deltaSeconds)
{
	for (ActorPtr& spActor : m_vecActors)
	{
		spActor->PostUpdate(deltaSeconds);
	}

	return true;
}

void Scene::Cleanup()
{
	for (ActorPtr& spActor : m_vecActors)
	{
		spActor->Cleanup();
	}

	m_vecActors.clear();
}