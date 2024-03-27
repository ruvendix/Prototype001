#pragma once

#include "SystemBase.h"

class CollisionSystem : public SystemBase
{
public:
	CollisionSystem();
	virtual ~CollisionSystem();

	virtual bool Update() override;
	virtual void Render() override;
};