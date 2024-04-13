#include "Pch.h"
#include "DebugSystem.h"

#include "SystemManager.h"
#include "InputSystem.h"

DebugSystem::DebugSystem()
{

}

DebugSystem::~DebugSystem()
{

}

bool DebugSystem::Update()
{
	InputSystem* pInputSys = GET_SYSTEM(InputSystem);
	if (pInputSys->IsKeyPress(EInputValue::LCtrl) == false)
	{
		return false;
	}

	if (pInputSys->IsKeyDown(EInputValue::Q))
	{
		m_debugOption.ToggleBit(EDebugOption::ShowCollider);
	}

	if (pInputSys->IsKeyDown(EInputValue::E))
	{
		m_debugOption.ToggleBit(EDebugOption::ShowDebugTile);
	}
	
	return true;
}