// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "ErrorEnums.h"

class ClientErrorHandler : public ErrorHandlerBase<EClientErrorCode>
{
	DECLARE_SINGLETON(ClientErrorHandler)

public:
	virtual void Startup() override;
};