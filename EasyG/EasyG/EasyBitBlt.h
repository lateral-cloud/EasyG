#pragma once

#include "EasyGHead.h"
#include <unordered_set>

_EASYG_BEGIN

BOOL WINAPI TransparentBlt(HDC hdcDest, int xoriginDest, int yoriginDest, int wDest, int hDest, HDC hdcSrc, int xoriginSrc, int yoriginSrc,
    int wSrc, int hSrc, const COLORREF* crTransparent, UINT Count = 1) noexcept
{
    std::unordered_set<COLORREF> colors(crTransparent, crTransparent + Count);
    HBITMAP DstHbp = (HBITMAP)::GetCurrentObject(hdcDest, OBJ_BITMAP);
    HBITMAP SrcHbp = (HBITMAP)::GetCurrentObject(hdcSrc, OBJ_BITMAP);
    BITMAP SrcBp;
    ::GetObject(SrcHbp, sizeof(BITMAP), &SrcBp);
    HDC basedc = ::GetDC(nullptr);
    HDC DstDcNew = ::CreateCompatibleDC(basedc);
    HBITMAP DstHbpNew = ::CreateCompatibleBitmap(basedc, SrcBp.bmWidth, SrcBp.bmHeight);
    ::ReleaseDC(nullptr, basedc);
    BITMAP DstBp;
    ::GetObject(DstHbp, sizeof(BITMAP), &DstBp);
    const RECT rctSrc{ xoriginSrc,yoriginSrc,xoriginSrc + (wSrc < SrcBp.bmWidth ? wSrc : SrcBp.bmWidth),yoriginSrc + (hSrc < SrcBp.bmHeight ? hSrc : SrcBp.bmHeight) };
    for (LONG i = rctSrc.top; i < rctSrc.bottom; ++i)
        for (LONG j = rctSrc.left; j < rctSrc.right; ++j)
        {
            const LONG pos = i * DstBp.bmWidth + j;
            if (colors.count(((COLORREF*)DstBp.bmBits)[pos]) == 0)
                ((COLORREF*)DstBp.bmBits)[pos] = ((COLORREF*)SrcBp.bmBits)[pos];
        }
    return TRUE;
}

_EASYG_END