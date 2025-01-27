// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Input/InputStructs.h"
#include "Engine/Input/InputAction.h"

class PlayerInputSystem
{
public:
	PlayerInputSystem();
	~PlayerInputSystem();

public:
	// 호출할 함수에 인자를 미리 넣고 나중에 호출하는 방식은 람다만 가능 => 캡처 기능 때문
	template <typename TObj, typename... TArgs>
	void BindInputActionHandler(const InputActionPtr& spInputAction, TObj* pObj, void(TObj::* memFunc)(const InputActionValue*, TArgs...), TArgs&&... args)
	{
		const InputActionValue* pInputActionValue = spInputAction->GetInputActionValue();

		InputActionHandler boundInputActionHandler =
			[=]() // BindInputActionHandler가 종료된 후에도 가변인자들을 유지해야 하므로 복사로 캡처
			{
				(pObj->*memFunc)(pInputActionValue, (args)...);
			};

		spInputAction->SetInputActionHandler(boundInputActionHandler);
	}

public:
	void AddInputMappingContext(const InputMappingContextPtr& spInputMappingContext, int32 priority);
	void ModifyInputMappingContextPriority(int32 handle, int32 priority);

	void ProcessPlayerInput();

private:
	std::set<InputMappingContextInfo> m_setInputMappingContext;
};