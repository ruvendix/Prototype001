// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class LocalPlayerInputSystem : public ICoreLoop
{
	DECLARE_SINGLETON(LocalPlayerInputSystem)

public:
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	// 호출할 함수에 인자를 미리 넣고 나중에 호출하는 방식은 람다만 가능 => 캡처 기능 때문
	template <typename TObject, typename... TArgs>
	void BindInputActionHandler(const InputActionPtr& spInputAction, TObject* pObj, void(TObject::* memFunc)(const InputActionValue*, TArgs...), TArgs&&... args)
	{
		const InputActionValue* pInputActionValue = spInputAction->GetInputActionValue();

		//InputActionDelegator boundInputActionHandler =
		//	[=]() // BindInputActionHandler가 종료된 후에도 인자들을 유지해야 하므로 복사로 캡처
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

private:
	int32 FindInputMappingContextPriorityById(int32 id) const;

private:
	SortVector<InputMappingContextPtr> m_sortVecInputMappingContext;
};