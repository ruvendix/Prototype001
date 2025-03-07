// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

struct Vector2d
{
	float x = 0.0f;
	float y = 0.0f;

	Vector2d operator + (const Vector2d& other) const
	{
		Vector2d ret;
		ret.x = (x + other.x);
		ret.y = (y + other.y);

		return ret;
	}

	Vector2d operator - (const Vector2d& other) const
	{
		Vector2d ret;
		ret.x = (x - other.x);
		ret.y = (y - other.y);

		return ret;
	}

	Vector2d operator * (const Vector2d& other) const
	{
		Vector2d ret;
		ret.x = (x * other.x);
		ret.y = (y * other.y);

		return ret;
	}

	Vector2d operator * (float scalar) const
	{
		Vector2d vResult;
		vResult.x = (x * scalar);
		vResult.y = (y * scalar);

		return vResult;
	}

	const Vector2d& operator += (const Vector2d& other)
	{
		x += other.x;
		y += other.y;

		return (*this);
	}

	const Vector2d& operator *= (float scalar)
	{
		x *= scalar;
		y *= scalar;

		return (*this);
	}

	bool operator == (const Vector2d& other) const
	{
		return ((math::CheckAlikeValue(x, other.x) == true) && (math::CheckAlikeValue(y, other.y) == true));
	}

	bool operator != (const Vector2d& other) const
	{
		return !(*this == other);
	}

	float CalculateLengthSquare() const
	{
		return (std::pow(x, 2.0f) + std::pow(y, 2.0f));
	}
};