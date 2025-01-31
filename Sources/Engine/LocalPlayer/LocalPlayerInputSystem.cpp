// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "LocalPlayerInputSystem.h"

DEFINE_SINGLETON(LocalPlayerInputSystem);

bool LocalPlayerInputSystem::Update(float deltaSeconds)
{
	for (auto iter = m_sortVecInputMappingContext.Begin(); iter != m_sortVecInputMappingContext.End(); ++iter)
	{
		const InputMappingContextPtr& spInputMappingContext = iter->obj;

		// Ű�� �ԷµǾ������� �˾Ƴ�����
		if (spInputMappingContext == nullptr)
		{
			continue;
		}

		// ���ؽ�Ʈ�� ó���Ǿ��ٸ� �н�
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

	// id�� � priority���� ã�ƾ���
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