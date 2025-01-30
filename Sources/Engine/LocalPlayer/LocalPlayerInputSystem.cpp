// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "LocalPlayerInputSystem.h"

DEFINE_SINGLETON(LocalPlayerInputSystem);

bool LocalPlayerInputSystem::Update(float deltaSeconds)
{
	for (const InputMappingContextInfo& inputMappingContextInfo : m_setInputMappingContext)
	{
		// Ű�� �ԷµǾ������� �˾Ƴ�����
		if (inputMappingContextInfo.spInputMappingContext == nullptr)
		{
			continue;
		}

		// ���ؽ�Ʈ�� ó���Ǿ��ٸ� �н�
		if (inputMappingContextInfo.spInputMappingContext->ProcessInputMappingContext() == true)
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

	InputMappingContextInfo inputMappingContextInfo(priority, spInputMappingContext);
	auto resultIter = m_setInputMappingContext.insert(inputMappingContextInfo);
	if (resultIter.second == false)
	{
		DETAIL_ERROR_LOG(EErrorCode::OverlapInputMappingContextPriority, priority);
	}
}

void LocalPlayerInputSystem::ModifyInputMappingContextPriority(int32 id, int32 priority)
{
	// ã���� �ϴ� �۾��� �˻�
	auto foundIter = m_setInputMappingContext.begin();
	for (/* */; foundIter != m_setInputMappingContext.cend(); ++foundIter)
	{
		if ((*foundIter).spInputMappingContext->GetId() == id)
		{
			break;
		}
	}

	if (foundIter != m_setInputMappingContext.end())
	{
		// ���� ��� ����
		InputMappingContextInfo tempSpInputMappingContext = (*foundIter);
		m_setInputMappingContext.erase(foundIter);

		// �켱������ ������ ���� �ٽ� ����
		tempSpInputMappingContext.priority = priority;
		m_setInputMappingContext.insert(tempSpInputMappingContext);
	}
}

void LocalPlayerInputSystem::RemoveInputMappingContext(int32 id)
{
	auto foundIter = std::find_if(m_setInputMappingContext.begin(), m_setInputMappingContext.end(),
		[=] (const InputMappingContextInfo& other)
		{
			return (id == other.spInputMappingContext->GetId());
		});

	if (foundIter == m_setInputMappingContext.cend())
	{
		return;
	}

	m_setInputMappingContext.erase(foundIter);
}