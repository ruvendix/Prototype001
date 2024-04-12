#pragma once

#include "IGameDefault.h"
#include "SystemDefines.h"

class SystemBase : public IGameDefault
{
public:
	SystemBase();
	virtual ~SystemBase();

	virtual void Startup() override;
	virtual bool Update() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;
	virtual void Cleanup() override;
};