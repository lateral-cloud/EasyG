#pragma once

#include "EasyHDC.h"

_EASYG_BEGIN

class EasyImage : public EasyHDC
{
public:
	COLORREF* pDIB_Src;
	SIZE ImageSize;

	EasyImage() noexcept = default;
	EasyImage(LONG cx, LONG cy, COLORREF bkColor = BLACKCOLOR) noexcept;
	EasyImage(const SIZE& size, COLORREF bkColor = BLACKCOLOR) noexcept;
	EasyImage(const EasyHDC&) noexcept;
	EasyImage(EasyHDC&&) noexcept;
	EasyImage(const EasyImage&) noexcept;
	EasyImage(EasyImage&&) noexcept;
	~EasyImage() noexcept;

	EasyImage& WINAPI operator=(const EasyHDC&) noexcept;
	EasyImage& WINAPI operator=(EasyHDC&&) noexcept;
	EasyImage& WINAPI operator=(const EasyImage&) noexcept;
	EasyImage& WINAPI operator=(EasyImage&&) noexcept;

	EasyImage WINAPI Clone() const noexcept;
	void WINAPI Detach() noexcept;
	void WINAPI Destroy() noexcept;
	COLORREF* WINAPI Create(LONG cx, LONG cy, COLORREF bkColor = BLACKCOLOR) noexcept;
	COLORREF* WINAPI Create(const SIZE& size, COLORREF bkColor = BLACKCOLOR) noexcept;
	void WINAPI Resize(LONG cx, LONG cy) noexcept;
	void WINAPI Resize(const SIZE& size) noexcept;
	EasyImage WINAPI Alpha(BYTE alpha, BYTE AlphaFormat = 0) const noexcept;
	EasyImage WINAPI Cut(LONG x, LONG y, LONG cx, LONG cy) const noexcept;
	EasyImage WINAPI Cut(const PTSZ& prect) const noexcept;

	void WINAPI SetPixel_Fast(int x, int y, COLORREF color) noexcept;
	void WINAPI SetPixel_Fast(const POINT& point, COLORREF color) noexcept;
	constexpr COLORREF WINAPI GetPixel_Fast(int x, int y) const noexcept;
	constexpr COLORREF WINAPI GetPixel_Fast(const POINT& point) const noexcept;

	void WINAPI BitBlt(HDC hdc, int x = 0, int y = 0, int cx = 0, int cy = 0, int x1 = 0, int y1 = 0, DWORD rop = SRCCOPY) const noexcept;
	void WINAPI BitBlt(HDC hdc, PTSZ Dst, POINT Src = {}, DWORD rop = SRCCOPY) const noexcept;
	void WINAPI StretchBlt(HDC hdc, int x = 0, int y = 0, int cx = 0, int cy = 0, int x1 = 0, int y1 = 0, int cx1 = 0, int cy1 = 0, DWORD rop = SRCCOPY) const noexcept;
	void WINAPI StretchBlt(HDC hdc, PTSZ Dst, PTSZ Src = {}, DWORD rop = SRCCOPY) const noexcept;
	void WINAPI TransparentBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, COLORREF crTransparent) const noexcept;
	void WINAPI TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src, COLORREF crTransparent) const noexcept;
	void WINAPI TransparentBlt(HDC hdc, int x = 0, int y = 0, int cx = 0, int cy = 0, int x1 = 0, int y1 = 0, int cx1 = 0, int cy1 = 0) const noexcept;
	void WINAPI TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src = {}) const noexcept;
	void WINAPI TransparentBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, const COLORREF* crTransparent, UINT Count = 1) const noexcept;
	void WINAPI TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src, const COLORREF* crTransparent, UINT Count = 1) const noexcept;
	void WINAPI AlphaBlend(HDC hdc, int x = 0, int y = 0, int cx = 0, int cy = 0, int x1 = 0, int y1 = 0, int cx1 = 0, int cy1 = 0, BYTE alpha = 255, BYTE AlphaFormat = 0) const noexcept;
	void WINAPI AlphaBlend(HDC hdc, PTSZ Dst, PTSZ Src = {}, BYTE alpha = 255, BYTE AlphaFormat = 0) const noexcept;

	COLORREF* WINAPI GetBitmapBits() noexcept;
	const COLORREF* WINAPI GetBitmapBits() const noexcept;
	SIZE WINAPI GetBitmapSize() const noexcept;
};

class EasyImage2 : public EasyImage
{
public:
	HDC ImageDC_Dst;
	COLORREF* pDIB_Dst;

	EasyImage2() noexcept = default;
	EasyImage2(LONG cx, LONG cy, COLORREF bkColor = BLACKCOLOR) noexcept;
	EasyImage2(const SIZE& size, COLORREF bkColor = BLACKCOLOR) noexcept;
	EasyImage2(const EasyHDC&) noexcept;
	EasyImage2(EasyHDC&&) noexcept;
	EasyImage2(const EasyImage&) noexcept;
	EasyImage2(EasyImage&&) noexcept;
	EasyImage2(const EasyImage2&) noexcept;
	EasyImage2(EasyImage2&&) noexcept;
	~EasyImage2() noexcept;

	EasyImage2& WINAPI operator=(const EasyHDC&) noexcept;
	EasyImage2& WINAPI operator=(EasyHDC&&) noexcept;
	EasyImage2& WINAPI operator=(const EasyImage&) noexcept;
	EasyImage2& WINAPI operator=(EasyImage&&) noexcept;
	EasyImage2& WINAPI operator=(const EasyImage2&) noexcept;
	EasyImage2& WINAPI operator=(EasyImage2&&) noexcept;

	EasyImage2 WINAPI Clone() const noexcept;
	void WINAPI Detach() noexcept;
	void WINAPI Destroy() noexcept;
	COLORREF* WINAPI Create(LONG cx, LONG cy, COLORREF bkColor = BLACKCOLOR) noexcept;
	COLORREF* WINAPI Create(const SIZE& size, COLORREF bkColor = BLACKCOLOR) noexcept;
	void WINAPI Resize(LONG cx, LONG cy) noexcept;
	void WINAPI Resize(const SIZE& size) noexcept;
	EasyImage2 WINAPI Alpha(BYTE alpha, BYTE AlphaFormat = 0) const noexcept;
	EasyImage2 WINAPI Cut(LONG x, LONG y, LONG cx, LONG cy) const noexcept;
	EasyImage2 WINAPI Cut(const PTSZ& prect) const noexcept;

	constexpr COLORREF WINAPI GetPixel_Fast(int x, int y) const noexcept;
	constexpr COLORREF WINAPI GetPixel_Fast(const POINT& point) const noexcept;

	void WINAPI BitBlt(HDC hdc, int x = 0, int y = 0, int cx = 0, int cy = 0, int x1 = 0, int y1 = 0, DWORD rop = SRCCOPY) const noexcept;
	void WINAPI BitBlt(HDC hdc, PTSZ Dst, POINT Src = {}, DWORD rop = SRCCOPY) const noexcept;
	void WINAPI StretchBlt(HDC hdc, int x = 0, int y = 0, int cx = 0, int cy = 0, int x1 = 0, int y1 = 0, int cx1 = 0, int cy1 = 0, DWORD rop = SRCCOPY) const noexcept;
	void WINAPI StretchBlt(HDC hdc, PTSZ Dst, PTSZ Src = {}, DWORD rop = SRCCOPY) const noexcept;
	void WINAPI TransparentBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, COLORREF crTransparent) const noexcept;
	void WINAPI TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src, COLORREF crTransparent) const noexcept;
	void WINAPI TransparentBlt(HDC hdc, int x = 0, int y = 0, int cx = 0, int cy = 0, int x1 = 0, int y1 = 0, int cx1 = 0, int cy1 = 0) const noexcept;
	void WINAPI TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src = {}) const noexcept;
	void WINAPI TransparentBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, const COLORREF* crTransparent, UINT Count = 1) const noexcept;
	void WINAPI TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src, const COLORREF* crTransparent, UINT Count = 1) const noexcept;
	void WINAPI AlphaBlend(HDC hdc, int x = 0, int y = 0, int cx = 0, int cy = 0, int x1 = 0, int y1 = 0, int cx1 = 0, int cy1 = 0, BYTE alpha = 255, BYTE AlphaFormat = 0) const noexcept;
	void WINAPI AlphaBlend(HDC hdc, PTSZ Dst, PTSZ Src = {}, BYTE alpha = 255, BYTE AlphaFormat = 0) const noexcept;

	void WINAPI Flush() noexcept;
	void WINAPI Flush(const PTSZ& ptsz) noexcept;
	void WINAPI Flush(LONG x, LONG y, LONG cx, LONG cy) noexcept;

	EasyHDC& WINAPI Dst() noexcept;
	const EasyHDC& WINAPI Dst() const noexcept;
};

_EASYG_END