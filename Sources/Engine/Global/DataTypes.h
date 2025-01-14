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
template <typename CounterType>
struct CompiletimeCounter
{
	static int32 NextValue()
	{
		static int32 s_num = 0; // 각 CounterType마다 고유한 카운트 시작값
		return s_num++;
	}
};

struct Position2d
{
	int32 x = 0;
	int32 y = 0;
};

struct Array2dInfo
{
	int32 xIdxCount = 0;
	int32 yIdxCount = 0;
};

// 이건 분량이 커지면 분리
struct Vec2d
{
	float x = 0.0f;
	float y = 0.0f;

	Vec2d operator- (const Vec2d& target) const
	{
		Vec2d ret;
		ret.x = (x - target.x);
		ret.y = (y - target.y);

		return ret;
	}

	float CalculateLengthSquare() const
	{
		return (std::pow(x, 2.0f) + std::pow(y, 2.0f));
	}
};

using ActorPtr = std::shared_ptr<class Actor>;
using ComponentPtr = std::shared_ptr<class Component>;
using ScenePtr = std::shared_ptr<class Scene>;
using PersistantResourcePtr = std::shared_ptr<class PersistantResource>;
using TexturePtr = std::shared_ptr<class Texture>;
using StaticSpritePtr = std::shared_ptr<class StaticSprite>;
using DynamicSpritePtr = std::shared_ptr<class DynamicSprite>;
using EventPtr = std::shared_ptr<class Event>;

using CallbackArgs = std::vector<std::any>;
using Callback = std::function<void(const CallbackArgs&)>;