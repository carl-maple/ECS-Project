#pragma once
#include "Types.h"
struct CVector2i
{
	constexpr CVector2i()
		:x(0), y(0)
	{
	}

	constexpr CVector2i(const int32 in_x, const int32 in_y)
		: x(in_x), y(in_y)
	{
	}

	void operator=(CVector2i in)
	{
		x = in.x;
		y = in.y;
	}

	void operator+=(CVector2i in)
	{
		x += in.x;
		y += in.y;
	}

	void operator-=(CVector2i in)
	{
		x -= in.x;
		y -= in.y;
	}

	int32 x;
	int32 y;
};


