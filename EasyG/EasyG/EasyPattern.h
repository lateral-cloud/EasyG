#pragma once

#include "EasyGHead.h"

_EASYG_BEGIN

constexpr auto  PI = 3.141562653589793;

void FivePointedStar(HDC dcDst, int x, int y, int radius, double startAngle)
{
	static const double delta = 2 * PI / 5;
	POINT points[5];
	for (int i = 0; i < 5; i++)
	{
		points[i].x = x + (LONG)(cos(startAngle + i * delta * 2) * radius);
		points[i].y = y + (LONG)(sin(startAngle + i * delta * 2) * radius);
	}
	::Polygon(dcDst, points, 5);
}

_EASYG_END