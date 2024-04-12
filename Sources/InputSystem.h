#pragma once

#include "SystemBase.h"
#include "InputEnums.h"

class InputSystem : public SystemBase
{
public:
	InputSystem();
	virtual ~InputSystem();

	virtual void Startup() override;
	virtual bool Update() override;

	bool IsKeyUp(EInputValue inputValue) const { return (m_inputStates[ENUM_TO_NUM(inputValue)] == EInputState::Up); }
	bool IsKeyPress(EInputValue inputValue) const { return (m_inputStates[ENUM_TO_NUM(inputValue)] == EInputState::Press); }
	bool IsKeyDown(EInputValue inputValue) const { return (m_inputStates[ENUM_TO_NUM(inputValue)] == EInputState::Down); }

	const Point2d& GetMousePosition() const { return m_mousePos; }

private:
	std::array<EInputState, ENUM_TO_NUM(EInputValue::Count)> m_inputStates;
	Point2d m_mousePos;
};