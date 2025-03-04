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

// 엔진이 서버에 관여하면 안되므로 이건 서버의 값을 변환해서 사용하는 걸로 고정
enum class EActorLookAtDirection
{
	Left = 0,
	Up,
	Right,
	Down,

	Count
};