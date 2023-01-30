#pragma once

#include "EasyGHead.h"

_EASYG_BEGIN

typedef union tagPTSZ
{
	struct
	{
		POINT point;
		SIZE size;
	};
	struct
	{
		LONG x, y, cx, cy;
	};
	tagPTSZ() noexcept = default;
	constexpr tagPTSZ(RECT rect) noexcept : x(rect.left), y(rect.top), cx(rect.right - rect.left), cy(rect.bottom - rect.top) {}
	constexpr tagPTSZ(POINT pt, SIZE sz) noexcept : point(pt), size(sz) {}
	constexpr tagPTSZ(LONG x, LONG y, LONG cx, LONG cy) noexcept : x(x), y(y), cx(cx), cy(cy) {}
	constexpr operator RECT() const noexcept { return RECT{ x,y,x + cx,y + cy }; }
	tagPTSZ& operator=(const tagPTSZ& rect) noexcept = default;
	tagPTSZ& operator=(const RECT& rect) noexcept { x = rect.left; y = rect.top; cx = rect.right - rect.left; cy = rect.bottom - rect.top; }
}PTSZ, * PPTSZ, NEAR* NPPTSZ, FAR* LPTSZ;
typedef const PTSZ FAR* LPCPTSZ;

constexpr PTSZ NULLPTSZ{ 0,0,0,0 };
constexpr POINT NULLPOINT{ 0,0 };
constexpr SIZE NULLSIZE{ 0,0 };

#define SPREAD_PTSZ_TO2(ptsz)		((ptsz).point),((ptsz).size)
#define SPREAD_PTSZ_TO4(ptsz)		((ptsz).x),((ptsz).y),((ptsz).cx),((ptsz).cy)
#define SPREAD_RECT(rect)			((rect).left),((rect).top),((rect).right),((rect).bottom)
#define SPREAD_POINT(pt)			((pt).x),((pt).y)
#define SPREAD_SIZE(sz)			((sz).cx),((sz).cy)
#define PLACEHOLDER_NULL4		NULL,NULL,NULL,NULL
#define PLACEHOLDER_NULL2		NULL,NULL
#define PLACEHOLDER_NULL		NULL

_EASYG_END