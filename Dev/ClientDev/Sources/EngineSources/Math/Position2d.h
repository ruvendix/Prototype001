// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

struct Position2d
{
	int32 x = 0;
	int32 y = 0;

	Position2d operator + (const Position2d& other) const
	{
		return Position2d(x + other.x, y + other.y);
	}

	Position2d operator - (const Position2d& other) const
	{
		return Position2d(x - other.x, y - other.y);
	}

	bool operator == (const Position2d& other) const
	{
		if ((x == other.x) &&
			(y == other.y))
		{
			return true;
		}

		return false;
	}

	bool operator < (const Position2d& other) const
	{
		if (x != other.x)
		{
			return x < other.x;
		}

		return (y < other.y);
	}
};