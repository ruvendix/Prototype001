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
	static int32 NextValue() { return s_num++; }
	static int32 GetCount() { return s_num; }

	static int32 s_num;
};

struct Position2d
{
	int32 x = 0;
	int32 y = 0;

	Position2d operator + (const Position2d& otherPos) const
	{
		return Position2d(x + otherPos.x, y + otherPos.y);
	}
	
	bool operator == (const Position2d& otherPos) const
	{
		if ((x == otherPos.x) &&
			(y == otherPos.y))
		{
			return true;
		}

		return false;
	}
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

// 이건 분량이 커지면 분리
struct Vec2d
{
	float x = 0.0f;
	float y = 0.0f;

	Vec2d operator + (const Vec2d& other) const
	{
		Vec2d ret;
		ret.x = (x + other.x);
		ret.y = (y + other.y);

		return ret;
	}

	Vec2d operator - (const Vec2d& other) const
	{
		Vec2d ret;
		ret.x = (x - other.x);
		ret.y = (y - other.y);

		return ret;
	}

	Vec2d operator * (const Vec2d& other) const
	{
		Vec2d ret;
		ret.x = (x * other.x);
		ret.y = (y * other.y);

		return ret;
	}

	Vec2d operator * (float scalar) const
	{
		Vec2d vResult;
		vResult.x = (x * scalar);
		vResult.y = (y * scalar);

		return vResult;
	}

	const Vec2d& operator += (const Vec2d& other)
	{
		x += other.x;
		y += other.y;

		return (*this);
	}

	const Vec2d& operator *= (float scalar)
	{
		x *= scalar;
		y *= scalar;

		return (*this);
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
using AnimationActorStatePtr = std::shared_ptr<class AnimationActorState>;

using Actors = std::vector<ActorPtr>;

// Bound된 함수는 항상 void() 형식
using BoundFunction = std::function<void()>;