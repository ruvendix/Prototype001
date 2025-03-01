// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "InputMappingContext.h"

#include "InputAction.h"

namespace
{
	static int32 g_inputMappingContextIdCounter = 0;
}

InputMappingContext::InputMappingContext()
{
	m_id = g_inputMappingContextIdCounter;
	++g_inputMappingContextIdCounter;
}

InputMappingContext::~InputMappingContext()
{

}

void InputMappingContext::AddInputAction(const InputActionPtr& spInputAction)
{
	m_vecInputAction.push_back(spInputAction);
}

bool InputMappingContext::ProcessInputMappingContext()
{
	bool bSucced = false;

	for (const InputActionPtr& spInputAction : m_vecInputAction)
	{
		bSucced = spInputAction->ProcessInputAction();
	}

	return bSucced;
}