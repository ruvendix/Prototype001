// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "MouseInputDevice.h"

#include "InputMacros.h"

DEFINE_SINGLETON(MouseInputDevice);

void MouseInputDevice::Startup()
{
	// 초기화
	InputRawInfo mouseInputInfo;
	m_arrCurrentMouseUserInputInfo.fill(mouseInputInfo);

	mouseInputInfo.virtualCode = VK_LBUTTON;
	m_arrCurrentMouseUserInputInfo[TO_NUM(EMouseValue::LeftButton)] = mouseInputInfo;
}

bool MouseInputDevice::Update(float deltaSeconds)
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
		InputRawInfo& refMouseInputInfo = m_arrCurrentMouseUserInputInfo[i];
		bool bExistMouseInput = IS_EXIST_USER_INPUT(refMouseInputInfo.virtualCode);
		
		switch (refMouseInputInfo.inputTrigger)
		{
		case EInputTrigger::Pressed:
		{
			if (bExistMouseInput == true)
			{
				refMouseInputInfo.inputTrigger = EInputTrigger::Holding;
			}
			else
			{
				refMouseInputInfo.inputTrigger = EInputTrigger::Released;
			}

			break;
		}

		case EInputTrigger::Holding:
		{
			if (bExistMouseInput == false)
			{
				refMouseInputInfo.inputTrigger = EInputTrigger::Released;
			}

			break;
		}

		case EInputTrigger::Released:
		{
			if (bExistMouseInput == true)
			{
				refMouseInputInfo.inputTrigger = EInputTrigger::Pressed;
			}
			else
			{
				refMouseInputInfo.inputTrigger = EInputTrigger::Count;
			}

			break;
		}

		// Down으로 전환용
		case EInputTrigger::Count:
		{
			if (bExistMouseInput == true)
			{
				refMouseInputInfo.inputTrigger = EInputTrigger::Pressed;
			}

			break;
		}
		}
	}

	for (const auto& iter : m_mapMouseInputBoundInfo)
	{
		EInputTrigger mouseInputState = m_arrCurrentMouseUserInputInfo[TO_NUM(iter.first)].inputTrigger;
		if (mouseInputState == EInputTrigger::Count)
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
		EInputTrigger mouseInputState = m_arrCurrentMouseUserInputInfo[TO_NUM(refIter.first)].inputTrigger;
		if (mouseInputState == EInputTrigger::Count)
		{
			continue;
		}

		// 홀딩일 때만 타이머 증가
		Timer& refMouseInputHoldingTimer = refIter.second.timer;
		if (mouseInputState == EInputTrigger::Holding)
		{
			refMouseInputHoldingTimer.Update(deltaSeconds);
		}
		else if (mouseInputState == EInputTrigger::Released)
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

void MouseInputDevice::BindMouseInput(EMouseValue mouseValue,
	const Callback& downCallback, const Callback& pressingCallback, const Callback& upCallback)
{
	if (m_mapMouseInputBoundInfo.find(mouseValue) != m_mapMouseInputBoundInfo.cend())
	{
		TRACE_LOG(LogDefault, "이미 존재하는 바인드 정보!");
		return;
	}

	MouseInputBoundInfo mouseInputBoundInfo;
	mouseInputBoundInfo.mouseValue = mouseValue;
	mouseInputBoundInfo.arrCallback[TO_NUM(EInputTrigger::Pressed)] = downCallback;
	mouseInputBoundInfo.arrCallback[TO_NUM(EInputTrigger::Holding)] = pressingCallback;
	mouseInputBoundInfo.arrCallback[TO_NUM(EInputTrigger::Released)] = upCallback;

	const auto& retInsert = m_mapMouseInputBoundInfo.insert(std::make_pair(mouseValue, mouseInputBoundInfo));
	ASSERT_LOG(retInsert.second == true);
}

void MouseInputDevice::UnbindMouseInput(EMouseValue mouseValue)
{
	m_mapMouseInputBoundInfo.erase(mouseValue);
}

void MouseInputDevice::BindMouseHoldingInput(EMouseValue mouseValue, float targetTime, const Callback& holdingEndCallback)
{
	if (m_mapMouseHoldingInputBoundInfo.find(mouseValue) != m_mapMouseHoldingInputBoundInfo.cend())
	{
		TRACE_LOG(LogDefault, "이미 존재하는 홀딩 바인드 정보!");
		return;
	}

	MouseHoldingInputBoundInfo mouseHoldingInputBoundInfo;
	mouseHoldingInputBoundInfo.mouseValue = mouseValue;
	mouseHoldingInputBoundInfo.timer = TimerManager::I()->CreateTimer(targetTime, false, holdingEndCallback);

	const auto& retInsert = m_mapMouseHoldingInputBoundInfo.insert(std::make_pair(mouseValue, mouseHoldingInputBoundInfo));
	ASSERT_LOG(retInsert.second == true);
}

void MouseInputDevice::UnbindMouseHoldingInput(EMouseValue mouseValue)
{
	m_mapMouseInputBoundInfo.erase(mouseValue);
}

bool MouseInputDevice::CheckMouseValueDown(EMouseValue mouseValue) const
{
	return (m_arrCurrentMouseUserInputInfo[TO_NUM(mouseValue)].inputTrigger == EInputTrigger::Pressed);
}

bool MouseInputDevice::CheckMouseValuePressing(EMouseValue mouseValue) const
{
	return (m_arrCurrentMouseUserInputInfo[TO_NUM(mouseValue)].inputTrigger == EInputTrigger::Holding);
}

bool MouseInputDevice::CheckMouseValueUp(EMouseValue mouseValue) const
{
	return (m_arrCurrentMouseUserInputInfo[TO_NUM(mouseValue)].inputTrigger == EInputTrigger::Released);
}