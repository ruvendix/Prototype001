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

// ������ Ÿ�� ī���ʹ� Ÿ������ ���
template <typename CounterType>
struct CompiletimeCounter
{
	static int32 NextValue()
	{
		static int32 s_num = 0; // �� CounterType���� ������ ī��Ʈ ���۰�
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

// �̰� �з��� Ŀ���� �и�
struct Vec2d
{
	float x = 0.0f;
	float y = 0.0f;

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

// Bound�� �Լ��� �׻� void() ����
using BoundFunction = std::function<void()>;