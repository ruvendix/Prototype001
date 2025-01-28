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

// ��� ���� ���п����θ� ��� (�迭������ �� ���� ��)
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
	OnlyPressed = 0, // �Է� ���°� ���ʷ� Pressed�� �Ǿ��� ����
	PressedAndRelease, // �Է� ���°� Pressed�� Release�� ����
	OnlyReleased, // �Է� ���°� ���ʷ� Released�� �Ǿ��� ����
	Triggered, // Pressed, Holding, Release ����

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