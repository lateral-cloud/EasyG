#pragma once

#include "EasyFlag_def.h"
#include "EasyPattern.h"

_EASYG_BEGIN

void EasyFlag::ChineseFlag(HDC dc, const POINT* lcpt, int width) noexcept
{
	static const int nunit = (int)(100 * (width / 3000.0));
	static const int unit = nunit;

	HDC dcDst = ::CreateCompatibleDC(NULL);
	HBITMAP bmpDst = ::CreateCompatibleBitmap(::GetDC(NULL), 30 * nunit, 20 * nunit);
	::SelectObject(dcDst, bmpDst);

	LOGBRUSH Bs{ BS_SOLID,REDCOLOR,NULL };
	LOGPEN Ps{ PS_SOLID,POINT{1,0}, REDCOLOR };
	HBRUSH hBs = ::CreateBrushIndirect(&Bs);
	HPEN hPs = ::CreatePenIndirect(&Ps);
	::SelectObject(dcDst, hBs);
	::SelectObject(dcDst, hPs);

	RECT rect{ 0,0,3000,2000 };
	::FillRect(dcDst, &rect, hBs);

	Bs.lbColor = YELLOWCOLOR;
	Ps.lopnColor = YELLOWCOLOR;
	hBs = ::CreateBrushIndirect(&Bs);
	hPs = ::CreatePenIndirect(&Ps);
	::DeleteObject(::SelectObject(dcDst, hBs));
	::DeleteObject(::SelectObject(dcDst, hPs));

	POINT point = lcpt == NULL ? POINT{ 0,0 } : *lcpt;
	::SetPolyFillMode(dcDst, WINDING);

	FivePointedStar(dcDst, 5 * unit, 5 * unit, 3 * unit, -PI / 2);
	FivePointedStar(dcDst, 10 * unit, 2 * unit, 1 * unit, PI / 2 + atan(5.0 / 3.0));
	FivePointedStar(dcDst, 12 * unit, 4 * unit, 1 * unit, PI / 2 + atan(7.0 / 1.0));
	FivePointedStar(dcDst, 12 * unit, 7 * unit, 1 * unit, -PI / 2 - atan(7.0 / 2.0));
	FivePointedStar(dcDst, 10 * unit, 9 * unit, 1 * unit, -PI / 2 - atan(5.0 / 4.0));

	::BitBlt(dc, point.x, point.y, 30 * nunit, 20 * nunit, dcDst, 0, 0, SRCCOPY);
}

_EASYG_END