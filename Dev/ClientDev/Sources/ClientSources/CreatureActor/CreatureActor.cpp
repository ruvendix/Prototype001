// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "CreatureActor.h"

CreatureActor::~CreatureActor()
{

}

void CreatureActor::Startup()
{
	Super::Startup();
	m_spGameCreatureInfo = std::make_shared<Protocol::S_GameCreatureInfo>();
}

bool CreatureActor::Update(float deltaSeconds)
{
	return Super::Update(deltaSeconds);
}

void CreatureActor::Cleanup()
{
	Super::Cleanup();
}