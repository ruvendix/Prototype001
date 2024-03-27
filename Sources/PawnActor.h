#pragma once

#include "Actor.h"

class PawnActor : public Actor
{
public:
	DEFINE_SMART_PTR(PawnActor);
	using Actor::Actor;

public:
	virtual ~PawnActor();
};