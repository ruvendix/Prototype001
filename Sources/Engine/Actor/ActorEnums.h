// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

enum class EActorFlag
{
	Activation = 0,
	RenderingTarget,
	
	Count,
};

// �������� �� ������� ����
enum class EActorLayerType
{
	Unknown = -1,

	WorldBackground,
	WorldForeground,
	Creature,
	Effect,

	Count
};

enum class EActorLookAtType
{
	Left = 0,
	Up,
	Right,
	Down,

	Count
};