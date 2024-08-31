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