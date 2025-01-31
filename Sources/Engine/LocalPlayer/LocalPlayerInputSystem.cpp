// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "LocalPlayerInputSystem.h"

DEFINE_SINGLETON(LocalPlayerInputSystem);

bool LocalPlayerInputSystem::Update(float deltaSeconds)
{
	for (auto iter = m_sortVecInputMappingContext.Begin(); iter != m_sortVecInputMappingContext.End(); ++iter)
	{
		const InputMappingContextPtr& spInputMappingContext = iter->obj;

		// 키가 입력되었는지를 알아내야함
		if (spInputMappingContext == nullptr)
		{
			continue;
		}

		// 컨텍스트가 처리되었다면 패스
		if (spInputMappingContext->ProcessInputMappingContext() == true)
		{
			break;
		}
	}

	return true;
}

void LocalPlayerInputSystem::AddInputMappingContext(const InputMappingContextPtr& spInputMappingContext, int32 priority)
{
	if (priority < 0)
	{
		return;
	}

	m_sortVecInputMappingContext.AddObject(spInputMappingContext, priority);
}

void LocalPlayerInputSystem::ModifyInputMappingContextPriority(int32 id, int32 priority)
{
	int32 foundPriority = FindInputMappingContextPriorityById(id);
	if (foundPriority == -1)
	{
		return;
	}

	m_sortVecInputMappingContext.SwapPriority(foundPriority, priority);
}

void LocalPlayerInputSystem::RemoveInputMappingContext(int32 id)
{
	int32 foundPriority = FindInputMappingContextPriorityById(id);
	if (foundPriority == -1)
	{
		return;
	}

	m_sortVecInputMappingContext.RemovePriority(foundPriority);
}

int32 LocalPlayerInputSystem::FindInputMappingContextPriorityById(int32 id) const
{
	int32 foundPriority = -1;

	// id로 어떤 priority인지 찾아야함
	for (auto iter = m_sortVecInputMappingContext.Begin(); iter != m_sortVecInputMappingContext.End(); ++iter)
	{
		const InputMappingContextPtr& spInputMappingContext = iter->obj;
		if (spInputMappingContext->GetId() == id)
		{
			foundPriority = iter->priority;
			break;
		}
	}

	return foundPriority;
}