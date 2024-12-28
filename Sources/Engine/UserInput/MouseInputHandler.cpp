// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "MouseInputHandler.h"

#include "UserInputMacro.h"

DEFINE_SINGLETON(MouseInputHandler);

void MouseInputHandler::Startup()
{
	// 초기화
	UserInputInfo mouseInputInfo;
	m_arrCurrentMouseUserInputInfo.fill(mouseInputInfo);

	mouseInputInfo.userInputVirtualCode = VK_LBUTTON;
	m_arrCurrentMouseUserInputInfo[TO_NUM(EMouseValue::LeftButton)] = mouseInputInfo;
}

bool MouseInputHandler::Update(float deltaSeconds)
{
	UNREFERENCED_PARAMETER(deltaSeconds);

	// 마우스 좌표는 최초 1번만 갱신
	POINT currentMousePos;
	::GetCursorPos(&currentMousePos);
	::ScreenToClient(SceneRenderer::I()->GetViewerHandle(), &currentMousePos);

	// 변환한 마우스 좌표 저장
	m_currentMousePos.x = currentMousePos.x;
	m_currentMousePos.y = currentMousePos.y;

	//DEFAULT_TRACE_LOG("마우스 좌표(%d, %d)", m_currentMousePos.x, m_currentMousePos.y);

	// 유저의 현재 마우스 입력 상태를 확인
	int32 mouseValueCount = TO_NUM(EMouseValue::Count);
	for (int32 i = 0; i < mouseValueCount; ++i)
	{
		UserInputInfo& refMouseInputInfo = m_arrCurrentMouseUserInputInfo[i];
		bool bExistMouseInput = IS_EXIST_USER_INPUT(refMouseInputInfo.userInputVirtualCode);
		
		switch (refMouseInputInfo.userInputState)
		{
		case EUserInputState::Down:
		{
			if (bExistMouseInput == true)
			{
				refMouseInputInfo.userInputState = EUserInputState::Pressing;
			}
			else
			{
				refMouseInputInfo.userInputState = EUserInputState::Up;
			}

			break;
		}

		case EUserInputState::Pressing:
		{
			if (bExistMouseInput == false)
			{
				refMouseInputInfo.userInputState = EUserInputState::Up;
			}

			break;
		}

		case EUserInputState::Up:
		{
			if (bExistMouseInput == true)
			{
				refMouseInputInfo.userInputState = EUserInputState::Down;
			}
			else
			{
				refMouseInputInfo.userInputState = EUserInputState::Count;
			}

			break;
		}

		// Down으로 전환용
		case EUserInputState::Count:
		{
			if (bExistMouseInput == true)
			{
				refMouseInputInfo.userInputState = EUserInputState::Down;
			}

			break;
		}
		}
	}

	for (const auto& iter : m_mapMouseInputBoundInfo)
	{
		EUserInputState mouseInputState = m_arrCurrentMouseUserInputInfo[TO_NUM(iter.first)].userInputState;
		if (mouseInputState == EUserInputState::Count)
		{
			continue;
		}

		const Callback& mouseInputCallback = iter.second.arrCallback[TO_NUM(mouseInputState)];
		if (mouseInputCallback != nullptr)
		{			
			mouseInputCallback(iter.second.callbackArgs);
		}
	}

	for (auto& refIter : m_mapMouseHoldingInputBoundInfo)
	{
		EUserInputState mouseInputState = m_arrCurrentMouseUserInputInfo[TO_NUM(refIter.first)].userInputState;
		if (mouseInputState == EUserInputState::Count)
		{
			continue;
		}

		// 홀딩일 때만 타이머 증가
		Timer& refMouseInputHoldingTimer = refIter.second.timer;
		if (mouseInputState == EUserInputState::Pressing)
		{
			refMouseInputHoldingTimer.Update(deltaSeconds);
		}
		else if (mouseInputState == EUserInputState::Up)
		{
			refMouseInputHoldingTimer.SetSwitch(true);
		}
		else
		{
			refMouseInputHoldingTimer.Reset();
		}
	}

	return true;
}

void MouseInputHandler::BindMouseInput(EMouseValue mouseValue,
	const Callback& downCallback, const Callback& pressingCallback, const Callback& upCallback)
{
	if (m_mapMouseInputBoundInfo.find(mouseValue) != m_mapMouseInputBoundInfo.cend())
	{
		TRACE_LOG(LogDefault, "이미 존재하는 바인드 정보!");
		return;
	}

	MouseInputBoundInfo mouseInputBoundInfo;
	mouseInputBoundInfo.mouseValue = mouseValue;
	mouseInputBoundInfo.arrCallback[TO_NUM(EUserInputState::Down)] = downCallback;
	mouseInputBoundInfo.arrCallback[TO_NUM(EUserInputState::Pressing)] = pressingCallback;
	mouseInputBoundInfo.arrCallback[TO_NUM(EUserInputState::Up)] = upCallback;

	const auto& retIter = m_mapMouseInputBoundInfo.insert(std::make_pair(mouseValue, mouseInputBoundInfo));
	ASSERT_LOG(retIter.second == true);
}

void MouseInputHandler::BindMouseHoldingInput(EMouseValue mouseValue, float targetTime, const Callback& holdingEndCallback)
{
	if (m_mapMouseHoldingInputBoundInfo.find(mouseValue) != m_mapMouseHoldingInputBoundInfo.cend())
	{
		TRACE_LOG(LogDefault, "이미 존재하는 홀딩 바인드 정보!");
		return;
	}

	MouseHoldingInputBoundInfo mouseHoldingInputBoundInfo;
	mouseHoldingInputBoundInfo.mouseValue = mouseValue;
	mouseHoldingInputBoundInfo.timer = TimerManager::I()->CreateTimer(targetTime, false, holdingEndCallback);

	const auto& retIter = m_mapMouseHoldingInputBoundInfo.insert(std::make_pair(mouseValue, mouseHoldingInputBoundInfo));
	ASSERT_LOG(retIter.second == true);
}

bool MouseInputHandler::CheckMouseValueDown(EMouseValue mouseValue) const
{
	return (m_arrCurrentMouseUserInputInfo[TO_NUM(mouseValue)].userInputState == EUserInputState::Down);
}

bool MouseInputHandler::CheckMouseValuePressing(EMouseValue mouseValue) const
{
	return (m_arrCurrentMouseUserInputInfo[TO_NUM(mouseValue)].userInputState == EUserInputState::Pressing);
}

bool MouseInputHandler::CheckMouseValueUp(EMouseValue mouseValue) const
{
	return (m_arrCurrentMouseUserInputInfo[TO_NUM(mouseValue)].userInputState == EUserInputState::Up);
}