#pragma once

#include "EasyImage_def.h"
#include "EasyBitBlt.h"
#include "EasyHandle.h"
#include <xutility>

_EASYG_BEGIN

#define BEGINLINE			LOGBRUSH OldBrushObj = this->GetLogBrush();							\
							LOGBRUSH NewOldBrushObj{BS_NULL,0x000000,0};						\
							this->SetBrushIndirect(&NewOldBrushObj)
#define ENDLINE				this->SetBrushIndirect(&OldBrushObj)

#define BEGINSOLID			LOGPEN OldPenObj = this->GetLogPen();								\
							LOGPEN NewOldPenObj{PS_NULL,OldPenObj.lopnWidth,0x000000};			\
							this->SetPenIndirect(&NewOldPenObj)
#define ENDSOLID			this->SetPenIndirect(&OldPenObj)

#define BEGINFILL
#define ENDFILL

#define BEGINCLEARLINE		LOGPEN OldPenObj = this->GetLogPen();								\
							LOGBRUSH OldBrushObj = this->GetLogBrush();							\
							LOGPEN NewOldPenObj{PS_SOLID,OldPenObj.lopnWidth,this->GetBkColor()};\
							LOGBRUSH NewOldBrushObj{BS_NULL,NewOldPenObj.lopnColor,0};			\
							this->SetPenIndirect(&NewOldPenObj);								\
							this->SetBrushIndirect(&NewOldBrushObj)
#define ENDCLEARLINE		this->SetBrushIndirect(&OldBrushObj);								\
							this->SetPenIndirect(&OldPenObj)

#define BEGINCLEARSOLID		LOGPEN OldPenObj = this->GetLogPen();								\
							LOGBRUSH OldBrushObj = this->GetLogBrush();							\
							LOGPEN NewOldPenObj{PS_NULL,OldPenObj.lopnWidth,this->GetBkColor()};\
							LOGBRUSH NewOldBrushObj{BS_SOLID,NewOldPenObj.lopnColor,0};			\
							this->SetPenIndirect(&NewOldPenObj);								\
							this->SetBrushIndirect(&NewOldBrushObj)
#define ENDCLEARSOLID		this->SetBrushIndirect(&OldBrushObj);								\
							this->SetPenIndirect(&OldPenObj)

#define BEGINCLEARFILL		LOGPEN OldPenObj = this->GetLogPen();								\
							LOGBRUSH OldBrushObj = this->GetLogBrush();							\
							LOGPEN NewOldPenObj{PS_SOLID,OldPenObj.lopnWidth,this->GetBkColor()};\
							LOGBRUSH NewOldBrushObj{BS_SOLID,NewOldPenObj.lopnColor,0};			\
							this->SetPenIndirect(&NewOldPenObj);								\
							this->SetBrushIndirect(&NewOldBrushObj)
#define ENDCLEARFILL		this->SetBrushIndirect(&OldBrushObj);								\
							this->SetPenIndirect(&OldPenObj)


EasyImage::EasyImage(LONG cx, LONG cy, COLORREF bkColor) noexcept
{
	this->Create(cx, cy, bkColor);
}

EasyImage::EasyImage(const SIZE& size, COLORREF bkColor) noexcept : EasyImage(size.cx, size.cy, bkColor) {}

EasyImage::EasyImage(const EasyImage& newImageDC) noexcept : EasyImage(newImageDC.Clone()) {}

EasyImage::EasyImage(EasyImage&& newImageDC) noexcept
{
	*this = _STD move(newImageDC);
	this->SetBkColor(newImageDC.GetBkColor());
}

EasyImage::~EasyImage() noexcept
{
	this->Destroy();
}

EasyImage& WINAPI EasyImage::operator=(const EasyHDC& newImageDC) noexcept
{
	return *this = newImageDC.Clone();
}

EasyImage& WINAPI EasyImage::operator=(EasyHDC&& newImageDC) noexcept
{
	this->Destroy();
	const auto bmp = newImageDC.GetBitmap();
	ImageDC_Src = newImageDC.ImageDC_Src;
	ImageSize = SIZE{ bmp.bmWidth,bmp.bmHeight };
	pDIB_Src = (COLORREF*)bmp.bmBits;
	newImageDC.Detach();
	return *this;
}

EasyImage& WINAPI EasyImage::operator=(const EasyImage& newImageDC) noexcept
{
	return *this = newImageDC.Clone();
}

EasyImage& WINAPI EasyImage::operator=(EasyImage&& newImageDC) noexcept
{
	this->Destroy();
	ImageDC_Src = newImageDC.ImageDC_Src;
	ImageSize = newImageDC.ImageSize;
	pDIB_Src = newImageDC.pDIB_Src;
	newImageDC.Detach();
	return *this;
}

EasyImage WINAPI EasyImage::Clone() const noexcept
{
	EasyImage newImageDC(ImageSize.cx, ImageSize.cy);
	::BitBlt(newImageDC.ImageDC_Src, 0, 0, ImageSize.cx, ImageSize.cy, ImageDC_Src, 0, 0, SRCCOPY);
	newImageDC.SetBkColor(this->GetBkColor());
	newImageDC.SetBkMode(this->GetBkMode());
	newImageDC.SetTextAlign(this->GetTextAlign());
	newImageDC.SetTextCharacterExtra(this->GetTextCharacterExtra());
	newImageDC.SetTextColor(this->GetTextColor());
	return newImageDC;
}

void WINAPI EasyImage::Detach() noexcept
{
	EasyHDC::Detach();
	pDIB_Src = nullptr;
	ImageSize.cx = ImageSize.cy = 0;
}

void WINAPI EasyImage::Destroy() noexcept
{
	if (ImageDC_Src)
	{
		HPEN hpen = (HPEN)::GetCurrentObject(ImageDC_Src, OBJ_PEN);
		HBRUSH hbrush = (HBRUSH)::GetCurrentObject(ImageDC_Src, OBJ_BRUSH);
		HFONT hfont = (HFONT)::GetCurrentObject(ImageDC_Src, OBJ_FONT);
		HBITMAP hbmp = (HBITMAP)::GetCurrentObject(ImageDC_Src, OBJ_BITMAP);
		::DeleteDC(ImageDC_Src);
		::DeleteObject(hbmp);
		this->Detach();
	}
}

COLORREF* WINAPI EasyImage::Create(LONG cx, LONG cy, COLORREF bkColor) noexcept
{
	this->Destroy();
	ImageDC_Src = ::CreateCompatibleDC(EasyHandle::screenHDC);
	const BITMAPINFO info{ { sizeof(BITMAPINFO),cx,-cy,1,sizeof(COLORREF) * 8,BI_RGB,0,1,1,0,0 },0 };
	::SelectObject(ImageDC_Src, ::CreateDIBSection(EasyHandle::screenHDC, &info, DIB_PAL_COLORS, (void**)&pDIB_Src, NULL, NULL));
	ImageSize.cx = cx;
	ImageSize.cy = cy;
	this->SetBkColor(bkColor);
	this->SetBkMode(TRANSPARENT);
	return pDIB_Src;
}

COLORREF* WINAPI EasyImage::Create(const SIZE& size, COLORREF bkColor) noexcept
{
	return this->Create(size.cx, size.cy, bkColor);
}

void WINAPI EasyImage::Resize(LONG cx, LONG cy) noexcept
{
	EasyImage newImageDC(cx, cy);
	this->BitBlt(newImageDC.ImageDC_Src, 0, 0, 0, 0, 0, 0, SRCCOPY);
	*this = _STD move(newImageDC);
}

void WINAPI EasyImage::Resize(const SIZE& size) noexcept
{
	this->Resize(size.cx, size.cy);
}

EasyImage WINAPI EasyImage::Alpha(BYTE alpha, BYTE AlphaFormat) const noexcept
{
	EasyImage newImageDC(ImageSize.cx, ImageSize.cy);
	this->AlphaBlend(newImageDC.ImageDC_Src, 0, 0, 0, 0, 0, 0, 0, 0, alpha, AlphaFormat);
	return newImageDC;
}

EasyImage WINAPI EasyImage::Cut(LONG x, LONG y, LONG cx, LONG cy) const noexcept
{
	EasyImage newImageDC(cx, cy);
	this->BitBlt(newImageDC.ImageDC_Src, 0, 0, 0, 0, x, y, SRCCOPY);
	return newImageDC;
}

EasyImage WINAPI EasyImage::Cut(const PTSZ& ptsz) const noexcept
{
	return this->Cut(ptsz.x, ptsz.y, ptsz.cx, ptsz.cy);
}

void WINAPI EasyImage::SetPixel_Fast(int x, int y, COLORREF color) noexcept
{
	pDIB_Src[x + y * ImageSize.cx] = ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff00) >> 16);
}

void WINAPI EasyImage::SetPixel_Fast(const POINT& point, COLORREF color) noexcept
{
	this->SetPixel_Fast(point.x, point.y, color);
}

constexpr COLORREF WINAPI EasyImage::GetPixel_Fast(int x, int y) const noexcept
{
	const auto& color = pDIB_Src[x + y * ImageSize.cx];
	return ((color & 0xff) << 16) | (color & 0xff00) | (color >> 16);
}

constexpr COLORREF WINAPI EasyImage::GetPixel_Fast(const POINT& point) const noexcept
{
	return this->GetPixel_Fast(point.x, point.y);
}

void WINAPI EasyImage::BitBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, DWORD rop) const noexcept
{
	::BitBlt(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Src, x1, y1, rop);
}

void WINAPI EasyImage::BitBlt(HDC hdc, PTSZ Dst, POINT Src, DWORD rop) const noexcept
{
	this->BitBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, rop);
}

void WINAPI EasyImage::StretchBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, DWORD rop) const noexcept
{
	::StretchBlt(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Src, x1, y1, cx1 ? cx1 : ImageSize.cx, cy1 ? cy1 : ImageSize.cy, rop);
}

void WINAPI EasyImage::StretchBlt(HDC hdc, PTSZ Dst, PTSZ Src, DWORD rop) const noexcept
{
	this->StretchBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, rop);
}

void WINAPI EasyImage::TransparentBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, COLORREF crTransparent) const noexcept
{
	::TransparentBlt(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Src, x1, y1, cx1 ? cx1 : ImageSize.cx, cy1 ? cy1 : ImageSize.cy, crTransparent);
}

void WINAPI EasyImage::TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src, COLORREF crTransparent) const noexcept
{
	this->TransparentBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, crTransparent);
}

void WINAPI EasyImage::TransparentBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1) const noexcept
{
	this->TransparentBlt(hdc, x, y, cx, cy, x1, y1, cx1, cy1, this->GetBkColor());
}

void WINAPI EasyImage::TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src) const noexcept
{
	this->TransparentBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, this->GetBkColor());
}

void WINAPI EasyImage::AlphaBlend(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, BYTE alpha, BYTE AlphaFormat) const noexcept
{
	::AlphaBlend(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Src, x1, y1, cx1 ? cx1 : ImageSize.cx, cy1 ? cy1 : ImageSize.cy, BLENDFUNCTION{ AC_SRC_OVER,0,alpha,AlphaFormat });
}

void WINAPI EasyImage::AlphaBlend(HDC hdc, PTSZ Dst, PTSZ Src, BYTE alpha, BYTE AlphaFormat) const noexcept
{
	this->AlphaBlend(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, alpha, AlphaFormat);
}

EasyImage2::EasyImage2(LONG cx, LONG cy, COLORREF bkColor) noexcept 
{
	this->Create(cx, cy, bkColor);
}

EasyImage2::EasyImage2(const SIZE& size, COLORREF bkColor) noexcept : EasyImage2(size.cx, size.cy, bkColor) {}

EasyImage2::EasyImage2(const EasyHDC& newImageDC) noexcept : EasyImage2(newImageDC.Clone()) {}

EasyImage2::EasyImage2(EasyHDC&& newImageDC) noexcept
{
	*this = _STD move(newImageDC);
}

EasyImage2::EasyImage2(const EasyImage& newImageDC) noexcept : EasyImage2(newImageDC.Clone()) {}

EasyImage2::EasyImage2(EasyImage&& newImageDC) noexcept
{
	*this = _STD move(newImageDC);
}

EasyImage2::EasyImage2(const EasyImage2& newImageDC) noexcept : EasyImage2(newImageDC.Clone()) {}

EasyImage2::EasyImage2(EasyImage2&& newImageDC) noexcept
{
	*this = _STD move(newImageDC);
}

EasyImage2::~EasyImage2() noexcept
{
	this->Destroy();
}

EasyImage2& WINAPI EasyImage2::operator=(const EasyHDC& newImageDC) noexcept
{
	return *this = newImageDC.Clone();
}

EasyImage2& WINAPI EasyImage2::operator=(EasyHDC&& newImageDC) noexcept
{
	this->Destroy();
	const auto bmp = newImageDC.GetBitmap();
	ImageDC_Src = newImageDC.ImageDC_Src;
	ImageSize = SIZE{ bmp.bmWidth,bmp.bmHeight };
	pDIB_Src = (COLORREF*)bmp.bmBits;
	ImageDC_Dst = ::CreateCompatibleDC(EasyHandle::screenHDC);
	const BITMAPINFO info{ { sizeof(BITMAPINFO),ImageSize.cx,-ImageSize.cy,1,sizeof(COLORREF) * 8,BI_RGB,0,1,1,0,0 },0 };
	::SelectObject(ImageDC_Dst, ::CreateDIBSection(EasyHandle::screenHDC, &info, DIB_PAL_COLORS, (void**)&pDIB_Dst, NULL, NULL));
	newImageDC.Detach();
	return *this;
}

EasyImage2& WINAPI EasyImage2::operator=(const EasyImage& newImageDC) noexcept
{
	return *this = newImageDC.Clone();
}

EasyImage2& WINAPI EasyImage2::operator=(EasyImage&& newImageDC) noexcept
{
	this->Destroy();
	ImageDC_Src = newImageDC.ImageDC_Src;
	pDIB_Src = newImageDC.pDIB_Src;
	ImageSize = newImageDC.ImageSize;
	ImageDC_Dst = ::CreateCompatibleDC(EasyHandle::screenHDC);
	const BITMAPINFO info{ { sizeof(BITMAPINFO),ImageSize.cx,-ImageSize.cy,1,sizeof(COLORREF) * 8,BI_RGB,0,1,1,0,0 },0 };
	::SelectObject(ImageDC_Dst, ::CreateDIBSection(EasyHandle::screenHDC, &info, DIB_PAL_COLORS, (void**)&pDIB_Dst, NULL, NULL));
	newImageDC.Detach();
	return *this;
}

EasyImage2& WINAPI EasyImage2::operator=(const EasyImage2& newImageDC) noexcept
{
	return *this = newImageDC.Clone();
}

EasyImage2& WINAPI EasyImage2::operator=(EasyImage2&& newImageDC) noexcept
{
	this->Destroy();
	ImageDC_Src = newImageDC.ImageDC_Src;
	pDIB_Src = newImageDC.pDIB_Src;
	ImageDC_Dst = newImageDC.ImageDC_Dst;
	pDIB_Dst = newImageDC.pDIB_Dst;
	ImageSize = newImageDC.ImageSize;
	newImageDC.Detach();
	return *this;
}

EasyImage2 WINAPI EasyImage2::Clone() const noexcept
{
	EasyImage2 newImageDC(ImageSize.cx, ImageSize.cy);
	::BitBlt(newImageDC.ImageDC_Src, 0, 0, ImageSize.cx, ImageSize.cy, ImageDC_Src, 0, 0, SRCCOPY);
	::BitBlt(newImageDC.ImageDC_Dst, 0, 0, ImageSize.cx, ImageSize.cy, ImageDC_Dst, 0, 0, SRCCOPY);
	newImageDC.SetBkColor(this->GetBkColor());
	newImageDC.SetBkMode(this->GetBkMode());
	newImageDC.SetTextAlign(this->GetTextAlign());
	newImageDC.SetTextCharacterExtra(this->GetTextCharacterExtra());
	newImageDC.SetTextColor(this->GetTextColor());
	return newImageDC;
}

void WINAPI EasyImage2::Detach() noexcept
{
	ImageDC_Src = nullptr;
	pDIB_Src = nullptr;
	ImageDC_Dst = nullptr;
	pDIB_Dst = nullptr;
	ImageSize.cx = ImageSize.cy = 0;
}

void WINAPI EasyImage2::Destroy() noexcept
{
	if (ImageDC_Src)
	{
		HPEN hpen = (HPEN)::GetCurrentObject(ImageDC_Src, OBJ_PEN);
		HBRUSH hbrush = (HBRUSH)::GetCurrentObject(ImageDC_Src, OBJ_BRUSH);
		HFONT hfont = (HFONT)::GetCurrentObject(ImageDC_Src, OBJ_FONT);
		HBITMAP hbmp = (HBITMAP)::GetCurrentObject(ImageDC_Src, OBJ_BITMAP);
		::DeleteDC(ImageDC_Src);
		::DeleteObject(hbmp);
		HBITMAP hbmp2 = (HBITMAP)::GetCurrentObject(ImageDC_Dst, OBJ_BITMAP);
		::DeleteDC(ImageDC_Dst);
		::DeleteObject(hbmp2);
		this->Detach();
	}
}

COLORREF* WINAPI EasyImage2::Create(LONG cx, LONG cy, COLORREF bkColor) noexcept
{
	const BITMAPINFO info{ { sizeof(BITMAPINFO),cx,-cy,1,sizeof(COLORREF) * 8,BI_RGB,0,1,1,0,0 },0 };
	ImageDC_Src = ::CreateCompatibleDC(EasyHandle::screenHDC);
	::SelectObject(ImageDC_Src, ::CreateDIBSection(EasyHandle::screenHDC, &info, DIB_PAL_COLORS, (void**)&pDIB_Src, NULL, NULL));
	ImageDC_Dst = ::CreateCompatibleDC(EasyHandle::screenHDC);
	::SelectObject(ImageDC_Dst, ::CreateDIBSection(EasyHandle::screenHDC, &info, DIB_PAL_COLORS, (void**)&pDIB_Dst, NULL, NULL));
	ImageSize.cx = cx;
	ImageSize.cy = cy;
	this->SetBkColor(bkColor);
	this->SetBkMode(TRANSPARENT);
	return pDIB_Src;
}

COLORREF* WINAPI EasyImage2::Create(const SIZE& size, COLORREF bkColor) noexcept
{
	return this->Create(size.cx, size.cy, bkColor);
}

void WINAPI EasyImage2::Resize(LONG cx, LONG cy) noexcept
{
	EasyImage2 newImageDC2(cx, cy);
	this->BitBlt(newImageDC2.ImageDC_Src, 0, 0, 0, 0, 0, 0, SRCCOPY);
	this->BitBlt(newImageDC2.ImageDC_Dst, 0, 0, 0, 0, 0, 0, SRCCOPY);
	*this = _STD move(newImageDC2);
}

void WINAPI EasyImage2::Resize(const SIZE& size) noexcept
{
	this->Resize(size.cx, size.cy);
}

EasyImage2 WINAPI EasyImage2::Alpha(BYTE alpha, BYTE AlphaFormat) const noexcept
{
	EasyImage2 newImageDC2(ImageSize.cx, ImageSize.cy);
	this->AlphaBlend(newImageDC2.ImageDC_Src, 0, 0, 0, 0, 0, 0, 0, 0, alpha, AlphaFormat);
	this->AlphaBlend(newImageDC2.ImageDC_Dst, 0, 0, 0, 0, 0, 0, 0, 0, alpha, AlphaFormat);
	return newImageDC2;
}

EasyImage2 WINAPI EasyImage2::Cut(LONG x, LONG y, LONG cx, LONG cy) const noexcept
{
	EasyImage2 newImageDC2(cx, cy);
	this->BitBlt(newImageDC2.ImageDC_Src, 0, 0, 0, 0, x, y, SRCCOPY);
	this->BitBlt(newImageDC2.ImageDC_Dst, 0, 0, 0, 0, x, y, SRCCOPY);
	return newImageDC2;
}

EasyImage2 WINAPI EasyImage2::Cut(const PTSZ& ptsz) const noexcept
{
	return this->Cut(ptsz.x, ptsz.y, ptsz.cx, ptsz.cy);
}

constexpr COLORREF WINAPI EasyImage2::GetPixel_Fast(int x, int y) const noexcept
{
	const auto& color = pDIB_Dst[x + y * ImageSize.cx];
	return ((color & 0xff) << 16) | (color & 0xff00) | (color >> 16);
}

constexpr COLORREF WINAPI EasyImage2::GetPixel_Fast(const POINT& point) const noexcept
{
	return this->GetPixel_Fast(point.x, point.y);
}

void WINAPI EasyImage2::BitBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, DWORD rop) const noexcept
{
	::BitBlt(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Dst, x1, y1, rop);
}

void WINAPI EasyImage2::BitBlt(HDC hdc, PTSZ Dst, POINT Src, DWORD rop) const noexcept
{
	this->BitBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, rop);
}

void WINAPI EasyImage2::StretchBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, DWORD rop) const noexcept
{
	::StretchBlt(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Dst, x1, y1, cx1 ? cx1 : ImageSize.cx, cy1 ? cy1 : ImageSize.cy, rop);
}

void WINAPI EasyImage2::StretchBlt(HDC hdc, PTSZ Dst, PTSZ Src, DWORD rop) const noexcept
{
	this->StretchBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, rop);
}

void WINAPI EasyImage2::TransparentBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, COLORREF crTransparent) const noexcept
{
	::TransparentBlt(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Dst, x1, y1, cx1 ? cx1 : ImageSize.cx, cy1 ? cy1 : ImageSize.cy, crTransparent);
}

void WINAPI EasyImage2::TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src, COLORREF crTransparent) const noexcept
{
	this->TransparentBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, crTransparent);
}

void WINAPI EasyImage2::TransparentBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1) const noexcept
{
	this->TransparentBlt(hdc, x, y, cx, cy, x1, y1, cx1, cy1, this->GetBkColor());
}

void WINAPI EasyImage2::TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src) const noexcept
{
	this->TransparentBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, this->GetBkColor());
}

void WINAPI EasyImage2::TransparentBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, const COLORREF* crTransparent, UINT Count) const noexcept
{
	_EASYG TransparentBlt(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Dst, x1, y1, cx1 ? cx1 : ImageSize.cx, cy1 ? cy1 : ImageSize.cy, crTransparent);
}

void WINAPI EasyImage2::TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src, const COLORREF* crTransparent, UINT Count) const noexcept
{
	this->TransparentBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, crTransparent, Count);
}

void WINAPI EasyImage2::AlphaBlend(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, BYTE alpha, BYTE AlphaFormat) const noexcept
{
	::AlphaBlend(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Dst, x1, y1, cx1 ? cx1 : ImageSize.cx, cy1 ? cy1 : ImageSize.cy, BLENDFUNCTION{ AC_SRC_OVER,0,alpha,AlphaFormat });
}

void WINAPI EasyImage2::AlphaBlend(HDC hdc, PTSZ Dst, PTSZ Src, BYTE alpha, BYTE AlphaFormat) const noexcept
{
	this->AlphaBlend(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, alpha, AlphaFormat);
}

void WINAPI EasyImage2::Flush() noexcept
{
	::BitBlt(ImageDC_Dst, 0, 0, ImageSize.cx, ImageSize.cy, ImageDC_Src, 0, 0, SRCCOPY);
}

void WINAPI EasyImage2::Flush(const PTSZ& ptsz) noexcept
{
	::BitBlt(ImageDC_Dst, ptsz.x, ptsz.y, ptsz.cx, ptsz.cy, ImageDC_Src, ptsz.x, ptsz.y, SRCCOPY);
}

void WINAPI EasyImage2::Flush(LONG x, LONG y, LONG cx, LONG cy) noexcept
{
	::BitBlt(ImageDC_Dst, x, y, cx, cy, ImageDC_Src, x, y, SRCCOPY);
}

EasyHDC& WINAPI EasyImage2::Dst() noexcept
{
	return *EasyHDC::FromHandle(ImageDC_Dst);
}

const EasyHDC& WINAPI EasyImage2::Dst() const noexcept
{
	return *EasyHDC::FromHandle(ImageDC_Dst);
}

_EASYG_END