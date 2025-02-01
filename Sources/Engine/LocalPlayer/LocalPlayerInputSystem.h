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

		//InputActionDelegator boundInputActionHandler =
		//	[=]() // BindInputActionHandler�� ����� �Ŀ��� ���ڵ��� �����ؾ� �ϹǷ� ����� ĸó
		//	{
		//		(pObj->*memFunc)(pInputActionValue, (args)...);
		//	};

		BoundFunction boundFunc = std::bind(memFunc, pObj, pInputActionValue, std::forward<TArgs>(args)...);

		DefaultDelegator inputActionDelegator;
		inputActionDelegator.ConnectFixedArgumentsStaticFunction(boundFunc);
		spInputAction->SetInputActionDelegator(inputActionDelegator);
	}

public:
	void AddInputMappingContext(const InputMappingContextPtr& spInputMappingContext, int32 priority);
	void ModifyInputMappingContextPriority(int32 id, int32 priority);
	void RemoveInputMappingContext(int32 id);
	void CleanupInputMappingContext();

private:
	int32 FindInputMappingContextPriorityById(int32 id) const;

private:
	SortVector<InputMappingContextPtr> m_sortVecInputMappingContext;
};