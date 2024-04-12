#pragma once

#include "SceneBase.h"

class StartScene : public SceneBase
{
public:
	virtual void Startup() override;

	void TestFunc();
};