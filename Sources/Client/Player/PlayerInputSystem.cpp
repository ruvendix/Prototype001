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
	// 찾고자 하는 작업을 검색
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
		// 기존 요소 삭제
		InputMappingContextInfo tempSpInputMappingContext = (*foundIter);
		m_setInputMappingContext.erase(foundIter);

		// 우선순위를 변경한 다음 다시 삽입
		tempSpInputMappingContext.priority = priority;
		m_setInputMappingContext.insert(tempSpInputMappingContext);
	}
}

void PlayerInputSystem::ProcessPlayerInput()
{
	for (const InputMappingContextInfo& inputMappingContextInfo : m_setInputMappingContext)
	{
		// 키가 입력되었는지를 알아내야함
		if (inputMappingContextInfo.spInputMappingContext == nullptr)
		{
			continue;
		}

		// 컨텍스트가 처리되었다면 패스
		if (inputMappingContextInfo.spInputMappingContext->ProcessInputMappingContext() == true)
		{
			break;
		}
	}
}