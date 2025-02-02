// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

enum class EActorFlag
{
	Activation = 0,
	RenderingTarget,
	
	Count,
};

enum class EActorLookAtType
{
	Left = 0,
	Up,
	Right,
	Down,

	Count
};