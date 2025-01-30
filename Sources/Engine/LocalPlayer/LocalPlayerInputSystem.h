// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class LocalPlayerInputSystem : public ICoreLoop
{
	DECLARE_SINGLETON(LocalPlayerInputSystem)

public:
	virtual bool Update(float deltaSeconds);

public:
	// ȣ���� �Լ��� ���ڸ� �̸� �ְ� ���߿� ȣ���ϴ� ����� ���ٸ� ���� => ĸó ��� ����
	template <typename TObject, typename... TArgs>
	void BindInputActionHandler(const InputActionPtr& spInputAction, TObject* pObj, void(TObject::* memFunc)(const InputActionValue*, TArgs...), TArgs&&... args)
	{
		const InputActionValue* pInputActionValue = spInputAction->GetInputActionValue();

		//InputActionHandler boundInputActionHandler =
		//	[=]() // BindInputActionHandler�� ����� �Ŀ��� ���ڵ��� �����ؾ� �ϹǷ� ����� ĸó
		//	{
		//		(pObj->*memFunc)(pInputActionValue, (args)...);
		//	};

		InputActionHandler boundInputActionHandler = std::bind(memFunc, pObj, pInputActionValue, std::forward<TArgs>(args)...);
		spInputAction->SetInputActionHandler(boundInputActionHandler);
	}

public:
	void AddInputMappingContext(const InputMappingContextPtr& spInputMappingContext, int32 priority);
	void ModifyInputMappingContextPriority(int32 id, int32 priority);
	void RemoveInputMappingContext(int32 id);

private:
	std::set<InputMappingContextInfo> m_setInputMappingContext;
};