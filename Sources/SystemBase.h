/*
<내용 요약>
시스템의 기본형
*/

#pragma once
#include "Types.h"
#include "Defines.h"
#include "IDefault.h"
#include "SystemDefines.h"

class SystemBase : public IDefault
{
public:
	SystemBase();
	virtual ~SystemBase();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;
	virtual void Cleanup() override;
};