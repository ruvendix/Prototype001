// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "ErrorEnums.h"

class EngineErrorHandler : public ErrorHandlerBase<EEngineErrorCode>
{
	DECLARE_SINGLETON(EngineErrorHandler)

public:
	virtual void Startup() override;
};