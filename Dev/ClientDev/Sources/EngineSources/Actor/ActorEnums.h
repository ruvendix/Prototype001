// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

enum class EActorFlag
{
	Activation = 0,
	RenderingTarget,
	
	Count,
};

// 렌더링도 이 순서대로 적용
enum class EActorLayerType
{
	Default = 0,
	WorldBackground,
	WorldForeground,
	Creature,
	Effect,

	Count
};

enum class EActorUpdateOrder
{
	Default,
	Post,
};

enum class EActorLookAtDirection
{
	Left = 0,
	Up,
	Right,
	Down,

	Count
};