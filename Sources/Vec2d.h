#pragma once

// 구조체는 POD가 원칙이지만 수학 관련은 편의성을 위해 지키지 않음

// float으로 고정
struct Vec2d
{
	Vec2d()
	{
		x = 0;
		y = 0;
	}

	Vec2d(float otherX, float otherY)
	{
		x = otherX;
		y = otherY;
	}

	Vec2d operator+(const Vec2d& other)
	{
		Vec2d ret;
		ret.x = x + other.x;
		ret.y = y + other.y;
		return ret;
	}

	Vec2d operator-(const Vec2d& other)
	{
		Vec2d ret;
		ret.x = x - other.x;
		ret.y = y - other.y;
		return ret;
	}

	Vec2d operator*(float value)
	{
		Vec2d ret;
		ret.x = x * value;
		ret.y = y * value;
		return ret;
	}

	void operator+=(const Vec2d& other)
	{
		x += other.x;
		y += other.y;
	}

	void operator-=(const Vec2d& other)
	{
		x -= other.x;
		y -= other.y;
	}

	void operator*=(float ratio)
	{
		x *= ratio;
		y *= ratio;
	}

	float LengthSquared()
	{
		return (std::pow(x, 2.0f) + std::pow(y, 2.0f));
	}

	float Length()
	{
		return (std::sqrt(LengthSquared()));
	}

	float x = 0;
	float y = 0;
};

// int로 고정
struct Int2d
{
	Int2d operator+(const Int2d& pos) const
	{
		return Int2d(x + pos.x, y + pos.y);
	}

	Int2d operator+(int32 value) const
	{
		return Int2d(x + value, y + value);
	}

	void operator+=(const Int2d& pos)
	{
		*this = (*this + pos);
	}

	Int2d operator+(const Size& size) const
	{
		return Int2d(x + size.width, y + size.height);
	}

	Int2d operator-(const Int2d& pos) const
	{
		return Int2d(x - pos.x, y - pos.y);
	}

	Int2d operator-(int32 value) const
	{
		return Int2d(x - value, y - value);
	}

	void operator-=(const Int2d& pos)
	{
		*this = (*this - pos);
	}

	int32 x = 0;
	int32 y = 0;
};