/*
<내용 요약>
자주 사용하는 기본형을 편하게 래핑함
*/

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
};

struct int2d
{
	int32 x = 0;
	int32 y = 0;

	int2d operator+(const int2d& pos)
	{
		return int2d(x + pos.x, y + pos.y);
	}

	void operator+=(const int2d& pos)
	{
		*this = (*this + pos);
	}
};

struct uint2d
{
	uint32 x = 0;
	uint32 y = 0;
};

// 정수면 점, 실수면 벡터
using Point2d = int2d;