// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

// 컴파일 타임 카운터는 타입으로 취급
template <typename TCounter>
struct CompiletimeCounter
{
	// 각 TCounter마다 고유한 카운트 시작값
	static uint32 NextValue() { return s_num++; }
	static uint32 s_num;
};

struct Array2dInfo
{
	int32 xIdxCount = 0;
	int32 yIdxCount = 0;
};

struct FloatRect
{
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
};

using ActorPtr = std::shared_ptr<class Actor>;
using PawnActorPtr = std::shared_ptr<class PawnActor>;

using ComponentPtr = std::shared_ptr<class Component>;
using ScenePtr = std::shared_ptr<class Scene>;
using PersistantResourcePtr = std::shared_ptr<class PersistantResource>;
using TexturePtr = std::shared_ptr<class Texture>;
using StaticSpritePtr = std::shared_ptr<class StaticSprite>;
using DynamicSpritePtr = std::shared_ptr<class DynamicSprite>;
using EffectPtr = std::shared_ptr<class Effect>;
using PawnActorStatePtr = std::shared_ptr<class PawnActorState>;

// Bound된 함수는 항상 void() 형식
using BoundFunction = std::function<void()>;