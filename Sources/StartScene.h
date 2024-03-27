/*
<내용 요약>
게임이 실행되면 가장 먼저 작동되는 씬
*/

#pragma once
#include "SceneBase.h"

class Texture;
class Flipbook;
class UiButton;

class StartScene : public SceneBase
{
public:
	virtual void Startup() override;

	void TestFunc();
};