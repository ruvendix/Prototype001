#pragma once

using int8  = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8  = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

struct Size
{
	uint32 width  = 0;
	uint32 height = 0;

	Size operator/(uint32 value) const
	{
		return Size(width / value, height / value);
	}
};

struct int2d
{
	int32 x = 0;
	int32 y = 0;

	int2d operator+(const int2d& pos) const
	{
		return int2d(x + pos.x, y + pos.y);
	}

	void operator+=(const int2d& pos)
	{
		*this = (*this + pos);
	}

	int2d operator+(const Size& size) const
	{
		return int2d(x + size.width, y + size.height);
	}

	int2d operator-(const int2d& pos) const
	{
		return int2d(x - pos.x, y - pos.y);
	}

	void operator-=(const int2d& pos)
	{
		*this = (*this - pos);
	}
};

struct uint2d
{
	uint32 x = 0;
	uint32 y = 0;
};

// 정수면 점, 실수면 벡터
using Point2d = int2d;