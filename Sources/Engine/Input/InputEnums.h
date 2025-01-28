// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

enum class EKeyboardValue
{
	Left = 0,
	Right,
	Down,
	Up,
	A,
	X,

	Count,
};

enum class EMouseValue
{
	LeftButton = 0,

	Count,
};

// 몇몇 값은 구분용으로만 사용 (배열에서는 안 쓰는 값)
enum class EInputValue
{
	Unknown = 0,

	Left,
	Right,
	Down,
	Up,
	A,
	X,

	KeyboardEnd,
	MouseLeftButton,

	Count,
};

enum class EInputValueState
{
	Pressed = 0,
	Holding,
	Released,
	None,

	Count,
};

enum class EInputTrigger
{
	OnlyPressed = 0, // 입력 상태가 최초로 Pressed가 되었을 때만
	PressedAndRelease, // 입력 상태가 Pressed와 Release일 때만
	OnlyReleased, // 입력 상태가 최초로 Released가 되었을 때만
	Triggered, // Pressed, Holding, Release 전부

	Count,
};

enum class EInputActionValueType
{
	Boolean,
	Vector1,
	Vector2,
	Vector3
};

enum class EInputActionValueModifierType
{
	Negative = 0,
	Count,
};