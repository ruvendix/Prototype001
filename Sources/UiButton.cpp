#include "Pch.h"
#include "UiButton.h"

#include "SystemManager.h"
#include "InputSystem.h"

UiButton::UiButton()
{

}

UiButton::~UiButton()
{

}

bool UiButton::Update()
{
	bool bRet = UiBase::Update();
	
	// 한번만 처리하면됨
	if (m_spBoxCollider == nullptr)
	{
		m_spBoxCollider = std::make_shared<BoxCollider>();
		m_spBoxCollider->SetUseCamera(false);

		Size halfSize(GetSize().width / 2, GetSize().height / 2);
		m_spBoxCollider->SetExtents(halfSize);
	}

	m_spBoxCollider->Update();

	if (TestMouseHit() == true)
	{
		//if (m_buttonState != EButtonState::OnMouse)
		//{
		//	// 범위 안에 들어온거면 이미지 변경
		//	m_buttonState = EButtonState::OnMouse;
		//	SetUiSpriteIndex(ENUM_TO_NUM(m_buttonState));
		//}

		if (GET_SYSTEM(InputSystem)->IsKeyDown(EInputValue::LButton))
		{
			// 범위 안에 들어온거면 이미지 변경
			m_buttonState = EButtonState::Clicked;
			SetUiSpriteIndex(ENUM_TO_NUM(m_buttonState));
		}

		if ((m_buttonState == EButtonState::Clicked) &&
			(GET_SYSTEM(InputSystem)->IsKeyUp(EInputValue::LButton)))
		{
			// 콜백 작동
			if (m_eventListener_onClicked != nullptr)
			{
				m_eventListener_onClicked();
			}

			m_buttonState = EButtonState::None;
			SetUiSpriteIndex(ENUM_TO_NUM(m_buttonState));
		}
	}
	else
	{
		if ((m_buttonState == EButtonState::Clicked) &&
			(GET_SYSTEM(InputSystem)->IsKeyUp(EInputValue::LButton)))
		{
			m_buttonState = EButtonState::None;
			SetUiSpriteIndex(ENUM_TO_NUM(m_buttonState));
		}
	}

	return bRet;
}

bool UiButton::TestMouseHit()
{
	const RECT& boxRect = m_spBoxCollider->GetBoxRect();
	POINT mousePos(GET_SYSTEM(InputSystem)->GetMousePosition().x, GET_SYSTEM(InputSystem)->GetMousePosition().y);

	// 박스 안에 점이 있는지?
	if (::PtInRect(&boxRect, mousePos) == FALSE)
	{
		return false;
	}

	return true;
}