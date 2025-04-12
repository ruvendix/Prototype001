// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "PlayerActor.h"

class InputActionValue;

class NetworkPlayerActor : public PlayerActor
{
	using Super = PlayerActor;

public:
	using Super::Super;
	virtual ~NetworkPlayerActor();

	virtual bool IsLocalPlayer() const override { return false; }
};