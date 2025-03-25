// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "ErrorEnums.h"

class ServerErrorHandler : public ErrorHandlerBase<EServerErrorCode>
{
	DECLARE_SINGLETON(ServerErrorHandler)

public:
	virtual void Startup() override;
};