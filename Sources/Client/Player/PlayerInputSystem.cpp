// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerInputSystem.h"

PlayerInputSystem::PlayerInputSystem()
{

}

PlayerInputSystem::~PlayerInputSystem()
{

}

void PlayerInputSystem::AddInputMappingContext(const InputMappingContextPtr& spInputMappingContext, int32 priority)
{
	if (priority < 0)
	{
		return;
	}

	InputMappingContextInfo inputMappingContextInfo(priority, spInputMappingContext);
	m_setInputMappingContext.insert(inputMappingContextInfo);
}

void PlayerInputSystem::ModifyInputMappingContextPriority(int32 id, int32 priority)
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

void PlayerInputSystem::ProcessPlayerInput()
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
}