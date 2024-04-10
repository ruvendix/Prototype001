#pragma once

#include "functional"
#include "UiBase.h"
#include "BoxCollider.h"

enum class EButtonState
{
	None = 0,
	Clicked,
};

class UiButton : public UiBase
{
public:
	DEFINE_SMART_PTR(UiButton);

public:
	UiButton();
	virtual ~UiButton();

	virtual bool Update() override;
	virtual bool TestMouseHit() override;

	void SetEventListener_OnClicked(std::function<void()> eventListener_onClicked) { m_eventListener_onClicked = eventListener_onClicked; }

private:
	RECT m_buttonRect;
	std::function<void()> m_eventListener_onClicked;
	BoxCollider::Ptr m_spBoxCollider = nullptr;
	EButtonState m_buttonState = EButtonState::None;
};