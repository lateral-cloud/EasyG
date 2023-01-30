#pragma once

#include "EasyHDC_def.h"
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

HPEN CloneHPen(HPEN hPen) noexcept
{
	LOGPEN logpen;
	::GetObject(hPen, sizeof(LOGPEN), &logpen);
	return ::CreatePenIndirect(&logpen);
}

HBRUSH CloneHBrush(HBRUSH hBrush) noexcept
{
	LOGBRUSH logbrush;
	::GetObject(hBrush, sizeof(LOGBRUSH), &logbrush);
	return ::CreateBrushIndirect(&logbrush);
}

HFONT CloneHFont(HFONT hFont) noexcept
{
	LOGFONT logfont;
	::GetObject(hFont, sizeof(LOGFONT), &logfont);
	return ::CreateFontIndirect(&logfont);
}

HBITMAP CloneHBitmap(HBITMAP hBitmap) noexcept
{
	BITMAP bmp;
	::GetObject(hBitmap, sizeof(BITMAP), &bmp);
	return ::CreateBitmap(bmp.bmWidth, bmp.bmHeight, bmp.bmPlanes, bmp.bmBitsPixel, bmp.bmBits);
}

EasyHDC::EasyHDC(LONG cx, LONG cy, COLORREF bkColor) noexcept
{
	this->Create(cx, cy, bkColor);
}

EasyHDC::EasyHDC(const SIZE& size, COLORREF bkColor) noexcept : EasyHDC(size.cx, size.cy, bkColor) {}

EasyHDC::EasyHDC(const EasyHDC& newImageDC) noexcept : EasyHDC(newImageDC.Clone()) {}

EasyHDC::EasyHDC(EasyHDC&& newImageDC) noexcept
{
	*this = _STD move(newImageDC);
	this->SetBkColor(newImageDC.GetBkColor());
}

EasyHDC::EasyHDC(const EasyHDC& newImageDC, COLORREF bkColor) noexcept : EasyHDC(newImageDC.Clone(), bkColor) {}

EasyHDC::EasyHDC(EasyHDC&& newImageDC, COLORREF bkColor) noexcept
{
	*this = _STD move(newImageDC);
	this->SetBkColor(bkColor);
}

EasyHDC::~EasyHDC() noexcept
{
	this->Destroy();
}

EasyHDC& WINAPI EasyHDC::operator=(const EasyHDC& newImageDC) noexcept
{
	return *this = newImageDC.Clone();
}

EasyHDC& WINAPI EasyHDC::operator=(EasyHDC&& newImageDC) noexcept
{
	this->Destroy();
	ImageDC_Src = newImageDC.ImageDC_Src;
	newImageDC.Detach();
	return *this;
}

EasyHDC WINAPI EasyHDC::Clone() const noexcept
{
	SIZE ImageSize = this->GetBitmapSize();
	EasyHDC newImageDC(ImageSize.cx, ImageSize.cy);
	::BitBlt(newImageDC.ImageDC_Src, 0, 0, ImageSize.cx, ImageSize.cy, ImageDC_Src, 0, 0, SRCCOPY);
	newImageDC.SetBkColor(this->GetBkColor());
	newImageDC.SetBkMode(this->GetBkMode());
	newImageDC.SetTextAlign(this->GetTextAlign());
	newImageDC.SetTextCharacterExtra(this->GetTextCharacterExtra());
	newImageDC.SetTextColor(this->GetTextColor());
	return newImageDC;
}

void WINAPI EasyHDC::Detach() noexcept
{
	ImageDC_Src = nullptr;
}

void WINAPI EasyHDC::Destroy() noexcept
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

COLORREF* WINAPI EasyHDC::Create(LONG cx, LONG cy, COLORREF bkColor) noexcept
{
	this->Destroy();
	ImageDC_Src = ::CreateCompatibleDC(_EASYG EasyHandle::screenHDC);
	const BITMAPINFO info{ { sizeof(BITMAPINFO),cx,-cy,1,sizeof(COLORREF) * 8,BI_RGB,0,1,1,0,0 },0 };
	COLORREF* pDIB_Src;
	::SelectObject(ImageDC_Src, ::CreateDIBSection(_EASYG EasyHandle::screenHDC, &info, DIB_PAL_COLORS, (void**)&pDIB_Src, NULL, NULL));
	this->SetBkColor(bkColor);
	this->SetBkMode(TRANSPARENT);
	return pDIB_Src;
}

COLORREF* WINAPI EasyHDC::Create(const SIZE& size, COLORREF bkColor) noexcept
{
	return this->Create(size.cx, size.cy, bkColor);
}

void WINAPI EasyHDC::Resize(LONG cx, LONG cy) noexcept
{
	EasyHDC newImageDC(cx, cy);
	this->BitBlt(newImageDC.ImageDC_Src, 0, 0, 0, 0, 0, 0, SRCCOPY);
	*this = _STD move(newImageDC);
}

void WINAPI EasyHDC::Resize(const SIZE& size) noexcept
{
	this->Resize(size.cx, size.cy);
}

EasyHDC WINAPI EasyHDC::Alpha(BYTE alpha, BYTE AlphaFormat) const noexcept
{
	const auto ImageSize = this->GetBitmapSize();
	EasyHDC newImageDC(ImageSize.cx, ImageSize.cy);
	this->AlphaBlend(newImageDC.ImageDC_Src, 0, 0, 0, 0, 0, 0, 0, 0, alpha, AlphaFormat);
	return newImageDC;
}

EasyHDC WINAPI EasyHDC::Cut(LONG x, LONG y, LONG cx, LONG cy) const noexcept
{
	EasyHDC newImageDC(cx, cy);
	this->BitBlt(newImageDC.ImageDC_Src, 0, 0, 0, 0, x, y, SRCCOPY);
	return newImageDC;
}

EasyHDC WINAPI EasyHDC::Cut(const PTSZ& ptsz) const noexcept
{
	return this->Cut(ptsz.x, ptsz.y, ptsz.cx, ptsz.cy);
}

COLORREF WINAPI EasyHDC::SetPixel(int x, int y, COLORREF color) noexcept
{
	return ::SetPixel(ImageDC_Src, x, y, color);
}

COLORREF WINAPI EasyHDC::SetPixel(const POINT& point, COLORREF color) noexcept
{
	return this->SetPixel(point.x, point.y, color);
}

void WINAPI EasyHDC::SetPixelV(int x, int y, COLORREF color) noexcept
{
	::SetPixelV(ImageDC_Src, x, y, color);
}

void WINAPI EasyHDC::SetPixelV(const POINT& point, COLORREF color) noexcept
{
	this->SetPixelV(point.x, point.y, color);
}

void WINAPI EasyHDC::SetPixel_Fast(int x, int y, COLORREF color) noexcept
{
	const auto bmp = this->GetBitmap();
	const auto pDIB_Src = (COLORREF*)bmp.bmBits;
	const auto ImageSize = SIZE{ bmp.bmWidth,bmp.bmHeight };
	pDIB_Src[x + y * ImageSize.cx] = ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff00) >> 16);
}

void WINAPI EasyHDC::SetPixel_Fast(const POINT& point, COLORREF color) noexcept
{
	this->SetPixel_Fast(point.x, point.y, color);
}

COLORREF WINAPI EasyHDC::GetPixel(int x, int y) const noexcept
{
	return ::GetPixel(ImageDC_Src, x, y);
}

COLORREF WINAPI EasyHDC::GetPixel(const POINT& point) const noexcept
{
	return this->GetPixel(point.x, point.y);
}

COLORREF WINAPI EasyHDC::GetPixel_Fast(int x, int y) const noexcept
{
	const auto bmp = this->GetBitmap();
	const auto pDIB_Src = (COLORREF*)bmp.bmBits;
	const auto ImageSize = SIZE{ bmp.bmWidth,bmp.bmHeight };
	const auto& color = pDIB_Src[x + y * ImageSize.cx];
	return ((color & 0xff) << 16) | (color & 0xff00) | (color >> 16);
}

COLORREF WINAPI EasyHDC::GetPixel_Fast(const POINT& point) const noexcept
{
	return this->GetPixel_Fast(point.x, point.y);
}

void WINAPI EasyHDC::BitBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, DWORD rop) const noexcept
{
	const auto ImageSize = this->GetBitmapSize();
	::BitBlt(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Src, x1, y1, rop);
}

void WINAPI EasyHDC::BitBlt(HDC hdc, PTSZ Dst, POINT Src, DWORD rop) const noexcept
{
	this->BitBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, rop);
}

void WINAPI EasyHDC::StretchBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, DWORD rop) const noexcept
{
	const auto ImageSize = this->GetBitmapSize();
	::StretchBlt(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Src, x1, y1, cx1 ? cx1 : ImageSize.cx, cy1 ? cy1 : ImageSize.cy, rop);
}

void WINAPI EasyHDC::StretchBlt(HDC hdc, PTSZ Dst, PTSZ Src, DWORD rop) const noexcept
{
	this->StretchBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, rop);
}

void WINAPI EasyHDC::TransparentBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, COLORREF crTransparent) const noexcept
{
	const auto ImageSize = this->GetBitmapSize();
	::TransparentBlt(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Src, x1, y1, cx1 ? cx1 : ImageSize.cx, cy1 ? cy1 : ImageSize.cy, crTransparent);
}

void WINAPI EasyHDC::TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src, COLORREF crTransparent) const noexcept
{
	this->TransparentBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, crTransparent);
}

void WINAPI EasyHDC::TransparentBlt(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1) const noexcept
{
	this->TransparentBlt(hdc, x, y, cx, cy, x1, y1, cx1, cy1, this->GetBkColor());
}

void WINAPI EasyHDC::TransparentBlt(HDC hdc, PTSZ Dst, PTSZ Src) const noexcept
{
	this->TransparentBlt(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, this->GetBkColor());
}

void WINAPI EasyHDC::AlphaBlend(HDC hdc, int x, int y, int cx, int cy, int x1, int y1, int cx1, int cy1, BYTE alpha, BYTE AlphaFormat) const noexcept
{
	const auto ImageSize = this->GetBitmapSize();
	::AlphaBlend(hdc, x, y, cx ? cx : ImageSize.cx, cy ? cy : ImageSize.cy, ImageDC_Src, x1, y1, cx1 ? cx1 : ImageSize.cx, cy1 ? cy1 : ImageSize.cy, BLENDFUNCTION{ AC_SRC_OVER,0,alpha,AlphaFormat });
}

void WINAPI EasyHDC::AlphaBlend(HDC hdc, PTSZ Dst, PTSZ Src, BYTE alpha, BYTE AlphaFormat) const noexcept
{
	this->AlphaBlend(hdc, Dst.x, Dst.y, Dst.cx, Dst.cy, Src.x, Src.y, Src.cx, Src.cy, alpha, AlphaFormat);
}

void WINAPI EasyHDC::SetPenIndirect(const LOGPEN* lppen) noexcept
{
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreatePenIndirect(lppen)));
}

void WINAPI EasyHDC::SetPen(HPEN hpen) noexcept
{
	::DeleteObject(::SelectObject(ImageDC_Src, hpen));
}

void WINAPI EasyHDC::SetPenStyle(UINT style) noexcept
{
	LOGPEN Pen;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_PEN), sizeof(LOGPEN), &Pen);
	Pen.lopnStyle = style;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreatePenIndirect(&Pen)));
}

void WINAPI EasyHDC::SetPenWidth(LONG width) noexcept
{
	LOGPEN Pen;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_PEN), sizeof(LOGPEN), &Pen);
	Pen.lopnWidth.x = width;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreatePenIndirect(&Pen)));
}

void WINAPI EasyHDC::SetPenColor(COLORREF color) noexcept
{
	LOGPEN Pen;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_PEN), sizeof(LOGPEN), &Pen);
	Pen.lopnColor = color;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreatePenIndirect(&Pen)));
}

void WINAPI EasyHDC::SetPen(UINT style, LONG width, COLORREF color) noexcept
{
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreatePen(style, width, color)));
}

void WINAPI EasyHDC::SetBrushIndirect(const LOGBRUSH* lpbrush) noexcept
{
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateBrushIndirect(lpbrush)));
}

void WINAPI EasyHDC::SetPatternBrush(HBITMAP hbm) noexcept
{
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreatePatternBrush(hbm)));
}

void WINAPI EasyHDC::SetBrush(HBRUSH hbrush) noexcept
{
	::DeleteObject(::SelectObject(ImageDC_Src, hbrush));
}

void WINAPI EasyHDC::SetBrushStyle(UINT style) noexcept
{
	LOGBRUSH Brush;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_BRUSH), sizeof(LOGBRUSH), &Brush);
	Brush.lbStyle = style;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateBrushIndirect(&Brush)));
}

void WINAPI EasyHDC::SetBrushColor(COLORREF color) noexcept
{
	LOGBRUSH Brush;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_BRUSH), sizeof(LOGBRUSH), &Brush);
	Brush.lbColor = color;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateBrushIndirect(&Brush)));
}

void WINAPI EasyHDC::SetBrushHatch(ULONG_PTR hatch) noexcept
{
	LOGBRUSH Brush;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_BRUSH), sizeof(LOGBRUSH), &Brush);
	Brush.lbHatch = hatch;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateBrushIndirect(&Brush)));
}

void WINAPI EasyHDC::SetBrush(UINT style, COLORREF color, ULONG_PTR hatch) noexcept
{
	LOGBRUSH Brush{ style ,color ,hatch };
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateBrushIndirect(&Brush)));
}

void WINAPI EasyHDC::SetFontIndirect(const LOGFONT* lplogfont) noexcept
{
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(lplogfont)));
}

void WINAPI EasyHDC::SetFont(HFONT hfont) noexcept
{
	::DeleteObject(::SelectObject(ImageDC_Src, hfont));
}

void WINAPI EasyHDC::SetFontHeight(LONG lfHeight) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfHeight = lfHeight;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontWidth(LONG lfWidth) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfWidth = lfWidth;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontEscapement(LONG lfEscapement) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfEscapement = lfEscapement;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontOrientation(LONG lfOrientation) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfOrientation = lfOrientation;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontWeight(LONG lfWeight) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfWeight = lfWeight;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontItalic(BYTE lfItalic) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfItalic = lfItalic;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontUnderline(BYTE lfUnderline) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfUnderline = lfUnderline;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontStrikeOut(BYTE lfStrikeOut) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfStrikeOut = lfStrikeOut;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontCharSet(BYTE lfCharSet) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfCharSet = lfCharSet;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontOutPrecision(BYTE lfOutPrecision) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfOutPrecision = lfOutPrecision;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontClipPrecision(BYTE lfClipPrecision) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfClipPrecision = lfClipPrecision;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontQuality(BYTE lfQuality) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfQuality = lfQuality;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontPitchAndFamily(BYTE lfPitchAndFamily) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	font.lfPitchAndFamily = lfPitchAndFamily;
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFontFaceName(LPCTSTR lfFaceName) noexcept
{
	LOGFONT font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &font);
	lstrcpy(font.lfFaceName, lfFaceName);
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFontIndirect(&font)));
}

void WINAPI EasyHDC::SetFont(LONG lfHeight, LONG lfWidth, LONG lfEscapement, LONG lfOrientation, LONG lfWeight, BYTE lfItalic, BYTE lfUnderline, BYTE lfStrikeOut,
	BYTE lfCharSet, BYTE lfOutPrecision, BYTE lfClipPrecision, BYTE lfQuality, BYTE lfPitchAndFamily, LPCTSTR lfFaceName) noexcept
{
	::DeleteObject(::SelectObject(ImageDC_Src, ::CreateFont(lfHeight, lfWidth, lfEscapement, lfOrientation, lfWeight, lfItalic, lfUnderline, lfStrikeOut, lfCharSet,
		lfOutPrecision, lfClipPrecision, lfQuality, lfPitchAndFamily, lfFaceName)));
}

void WINAPI EasyHDC::SetTextAlign(UINT align) noexcept
{
	::SetTextAlign(ImageDC_Src, align);
}

void WINAPI EasyHDC::SetTextCharacterExtra(int extra) noexcept
{
	::SetTextCharacterExtra(ImageDC_Src, extra);
}

void WINAPI EasyHDC::SetTextColor(COLORREF color) noexcept
{
	::SetTextColor(ImageDC_Src, color);
}

void WINAPI EasyHDC::SetTextJustification(int extra, int count) noexcept
{
	::SetTextJustification(ImageDC_Src, extra, count);
}

void WINAPI EasyHDC::SetBkColor(COLORREF color) noexcept
{
	::SetBkColor(ImageDC_Src, color);
}

void WINAPI EasyHDC::SetBkMode(int mode) noexcept
{
	::SetBkMode(ImageDC_Src, mode);
}

void WINAPI EasyHDC::GetLogPen(LPLOGPEN lppen) const noexcept
{
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_PEN), sizeof(LOGPEN), lppen);
}

LOGPEN WINAPI EasyHDC::GetLogPen() const noexcept
{
	LOGPEN Pen;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_PEN), sizeof(LOGPEN), &Pen);
	return Pen;
}

HPEN WINAPI EasyHDC::GetHPen() const noexcept
{
	return (HPEN)::GetCurrentObject(ImageDC_Src, OBJ_PEN);
}

void WINAPI EasyHDC::GetLogBrush(LPLOGBRUSH plogbrush) const noexcept
{
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_BRUSH), sizeof(LOGBRUSH), &plogbrush);
}

LOGBRUSH WINAPI EasyHDC::GetLogBrush() const noexcept
{
	LOGBRUSH Brush;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_BRUSH), sizeof(LOGBRUSH), &Brush);
	return Brush;
}

HBRUSH WINAPI EasyHDC::GetHBrush() const noexcept
{
	return (HBRUSH)::GetCurrentObject(ImageDC_Src, OBJ_BRUSH);
}

void WINAPI EasyHDC::GetLogFont(LPLOGFONT lplogfont) const noexcept
{
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), lplogfont);
}

LOGFONT WINAPI EasyHDC::GetLogFont() const noexcept
{
	LOGFONT Font;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_FONT), sizeof(LOGFONT), &Font);
	return Font;
}

HFONT WINAPI EasyHDC::GetHFont() const noexcept
{
	return (HFONT)::GetCurrentObject(ImageDC_Src, OBJ_FONT);
}

int WINAPI EasyHDC::GetTextFace(int count, LPTSTR lpName) const noexcept
{
	return ::GetTextFace(ImageDC_Src, count, lpName);
}

UINT WINAPI EasyHDC::GetTextAlign() const noexcept
{
	return ::GetTextAlign(ImageDC_Src);
}

int WINAPI EasyHDC::GetTextCharacterExtra() const noexcept
{
	return ::GetTextCharacterExtra(ImageDC_Src);
}

COLORREF WINAPI EasyHDC::GetTextColor() const noexcept
{
	return ::GetTextColor(ImageDC_Src);
}

void WINAPI EasyHDC::GetTextSize(TCHAR ch, LPSIZE lpsz) const noexcept
{
	::GetTextExtentPoint32(ImageDC_Src, &ch, 1, lpsz);
}

void WINAPI EasyHDC::GetTextSize(LPCTSTR str, LPSIZE lpsz, int count) const noexcept
{
	if (count != -1)
		::GetTextExtentPoint32(ImageDC_Src, str, count, lpsz);
	else
		::GetTextExtentPoint32(ImageDC_Src, str, lstrlen(str), lpsz);
}

SIZE WINAPI EasyHDC::GetTextSize(TCHAR ch) const noexcept
{
	SIZE size;
	GetTextSize(ch, &size);
	return size;
}

SIZE WINAPI EasyHDC::GetTextSize(LPCTSTR str, int count) const noexcept
{
	SIZE size;
	GetTextSize(str, &size, count);
	return size;
}

int WINAPI EasyHDC::GetTextWidth(TCHAR ch) const noexcept
{
	return GetTextSize(ch).cx;
}

int WINAPI EasyHDC::GetTextWidth(LPCTSTR str, int count) const noexcept
{
	return GetTextSize(str, count).cx;
}

int WINAPI EasyHDC::GetTextHeight(TCHAR ch) const noexcept
{
	return GetTextSize(ch).cy;
}

int WINAPI EasyHDC::GetTextHeight(LPCTSTR str, int count) const noexcept
{
	return GetTextSize(str, count).cy;
}

HBITMAP WINAPI EasyHDC::GetHBitmap() const noexcept
{
	return (HBITMAP)::GetCurrentObject(ImageDC_Src, OBJ_BITMAP);
}

BITMAP WINAPI EasyHDC::GetBitmap() const noexcept
{
	BITMAP bmp;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_BITMAP), sizeof(bmp), &bmp);
	return bmp;
}

COLORREF* WINAPI EasyHDC::GetBitmapBits() noexcept
{
	BITMAP bmp;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_BITMAP), sizeof(bmp), &bmp);
	return (COLORREF*)bmp.bmBits;
}

const COLORREF* WINAPI EasyHDC::GetBitmapBits() const noexcept
{
	BITMAP bmp;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_BITMAP), sizeof(bmp), &bmp);
	return (COLORREF*)bmp.bmBits;
}

SIZE WINAPI EasyHDC::GetBitmapSize() const noexcept
{
	BITMAP bmp;
	::GetObject(::GetCurrentObject(ImageDC_Src, OBJ_BITMAP), sizeof(bmp), &bmp);
	return SIZE{ bmp.bmWidth,bmp.bmHeight };
}

COLORREF WINAPI EasyHDC::GetBkColor() const noexcept
{
	return ::GetBkColor(ImageDC_Src);
}

int WINAPI EasyHDC::GetBkMode() const noexcept
{
	return ::GetBkMode(ImageDC_Src);
}

HPEN WINAPI EasyHDC::CloneHPen() const noexcept
{
	return _EASYG CloneHPen((HPEN)::GetCurrentObject(ImageDC_Src, OBJ_PEN));
}

HBRUSH WINAPI EasyHDC::CloneHBrush() const noexcept
{
	return _EASYG CloneHBrush((HBRUSH)::GetCurrentObject(ImageDC_Src, OBJ_BRUSH));
}

HFONT WINAPI EasyHDC::CloneHFont() const noexcept
{
	return _EASYG CloneHFont((HFONT)::GetCurrentObject(ImageDC_Src, OBJ_FONT));
}

void WINAPI EasyHDC::MoveToEx(int x, int y, LPPOINT rtlppoint) noexcept
{
	::MoveToEx(ImageDC_Src, x, y, rtlppoint);
}

void WINAPI EasyHDC::MoveToEx(const POINT* lppoint, LPPOINT rtlppoint) noexcept
{
	this->MoveToEx(lppoint->x, lppoint->y, rtlppoint);
}

void WINAPI EasyHDC::MoveTo(const POINT* lppoint, LPPOINT rtlppoint) noexcept
{
	this->MoveToEx(lppoint, rtlppoint);
}

void WINAPI EasyHDC::MoveTo(int x, int y, LPPOINT rtlppoint) noexcept
{
	this->MoveToEx(x, y, rtlppoint);
}

void WINAPI EasyHDC::LineTo(int x, int y) noexcept
{
	::LineTo(ImageDC_Src, x, y);
}

void WINAPI EasyHDC::LineTo(const POINT* lppoint) noexcept
{
	this->LineTo(lppoint->x, lppoint->y);
}

void WINAPI EasyHDC::Line(int x1, int y1, int x2, int y2) noexcept
{
	this->MoveToEx(x1, y1);
	this->LineTo(x2, y2);
}

void WINAPI EasyHDC::Line(const POINT* lpf, const POINT* lpt) noexcept
{
	this->Line(lpf->x, lpf->y, lpt->x, lpt->y);
}

void WINAPI EasyHDC::LineRect(int left, int top, int right, int bottom) noexcept
{
	BEGINLINE;
	::Rectangle(ImageDC_Src, left, top, right, bottom);
	ENDLINE;
}

void WINAPI EasyHDC::LineRect(LPCRECT lprect) noexcept
{
	this->LineRect(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::ClearLineRect(int left, int top, int right, int bottom) noexcept
{
	BEGINCLEARLINE;
	::Rectangle(ImageDC_Src, left, top, right, bottom);
	ENDCLEARLINE;
}

void WINAPI EasyHDC::ClearLineRect(LPCRECT lprect) noexcept
{
	this->ClearRect(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::SolidRect(int left, int top, int right, int bottom) noexcept
{
	BEGINSOLID;
	::Rectangle(ImageDC_Src, left, top, right, bottom);
	ENDSOLID;
}

void WINAPI EasyHDC::SolidRect(LPCRECT lprect) noexcept
{
	this->SolidRect(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::ClearSolidRect(int left, int top, int right, int bottom) noexcept
{
	BEGINCLEARSOLID;
	::Rectangle(ImageDC_Src, left, top, right, bottom);
	ENDCLEARSOLID;
}

void WINAPI EasyHDC::ClearSolidRect(LPCRECT lprect) noexcept
{
	this->ClearSolidRect(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::FillRect(int left, int top, int right, int bottom) noexcept
{
	BEGINFILL;
	::Rectangle(ImageDC_Src, left, top, right, bottom);
	ENDFILL;
}

void WINAPI EasyHDC::FillRect(LPCRECT lprect) noexcept
{
	this->FillRect(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::ClearFillRect(int left, int top, int right, int bottom) noexcept
{
	BEGINCLEARFILL;
	::Rectangle(ImageDC_Src, left, top, right, bottom);
	ENDCLEARFILL;
}

void WINAPI EasyHDC::ClearFillRect(LPCRECT lprect) noexcept
{
	this->ClearFillRect(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::Rectangle(int left, int top, int right, int bottom) noexcept
{
	this->LineRect(left, top, right, bottom);
}

void WINAPI EasyHDC::Rectangle(LPCRECT lprect) noexcept
{
	this->LineRect(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::ClearRect(int left, int top, int right, int bottom) noexcept
{
	this->ClearFillRect(left, top, right, bottom);
}

void WINAPI EasyHDC::ClearRect(LPCRECT lprect) noexcept
{
	this->ClearFillRect(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::SolidRgn(HRGN hrgn) noexcept
{
	BEGINFILL;
	::PaintRgn(ImageDC_Src, hrgn);
	ENDFILL;
}

void WINAPI EasyHDC::ClearSolidRgn(HRGN hrgn) noexcept
{
	BEGINCLEARSOLID;
	::PaintRgn(ImageDC_Src, hrgn);
	ENDCLEARSOLID;
}

void WINAPI EasyHDC::FillRgn(HRGN hrgn) noexcept
{
	this->SolidRgn(hrgn);
}

void WINAPI EasyHDC::ClearRgn(HRGN hrgn) noexcept
{
	this->ClearSolidRgn(hrgn);
}

void WINAPI EasyHDC::LineEllipse(int left, int top, int right, int bottom) noexcept
{
	BEGINLINE;
	::Ellipse(ImageDC_Src, left, top, right, bottom);
	ENDLINE;
}

void WINAPI EasyHDC::LineEllipse(LPCRECT lprect) noexcept
{
	this->LineEllipse(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::ClearLineEllipse(int left, int top, int right, int bottom) noexcept
{
	BEGINCLEARLINE;
	::Ellipse(ImageDC_Src, left, top, right, bottom);
	ENDCLEARLINE;
}

void WINAPI EasyHDC::ClearLineEllipse(LPCRECT lprect) noexcept
{
	this->ClearEllipse(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::SolidEllipse(int left, int top, int right, int bottom) noexcept
{
	BEGINSOLID;
	::Ellipse(ImageDC_Src, left, top, right, bottom);
	ENDSOLID;
}

void WINAPI EasyHDC::SolidEllipse(LPCRECT lprect) noexcept
{
	this->SolidEllipse(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::ClearSolidEllipse(int left, int top, int right, int bottom) noexcept
{
	BEGINCLEARSOLID;
	::Ellipse(ImageDC_Src, left, top, right, bottom);
	ENDCLEARSOLID;
}

void WINAPI EasyHDC::ClearSolidEllipse(LPCRECT lprect) noexcept
{
	this->ClearEllipse(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::FillEllipse(int left, int top, int right, int bottom) noexcept
{
	BEGINFILL;
	::Ellipse(ImageDC_Src, left, top, right, bottom);
	ENDFILL;
}

void WINAPI EasyHDC::FillEllipse(LPCRECT lprect) noexcept
{
	this->FillEllipse(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::ClearFillEllipse(int left, int top, int right, int bottom) noexcept
{
	BEGINCLEARFILL;
	::Ellipse(ImageDC_Src, left, top, right, bottom);
	ENDCLEARFILL;
}

void WINAPI EasyHDC::ClearFillEllipse(LPCRECT lprect) noexcept
{
	this->ClearFillEllipse(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::Ellipse(int left, int top, int right, int bottom) noexcept
{
	this->LineEllipse(left, top, right, bottom);
}

void WINAPI EasyHDC::Ellipse(LPCRECT lprect) noexcept
{
	this->LineEllipse(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::ClearEllipse(int left, int top, int right, int bottom) noexcept
{
	this->ClearFillEllipse(left, top, right, bottom);
}

void WINAPI EasyHDC::ClearEllipse(LPCRECT lprect) noexcept
{
	this->ClearFillEllipse(lprect->left, lprect->top, lprect->right, lprect->bottom);
}

void WINAPI EasyHDC::LineCircle(int x, int y, int r) noexcept
{
	this->LineEllipse(x - r, y + r, x + r, y - r);
}

void WINAPI EasyHDC::LineCircle(const POINT* lppoint, int r) noexcept
{
	this->LineCircle(lppoint->x, lppoint->y, r);
}

void WINAPI EasyHDC::ClearLineCircle(int x, int y, int r) noexcept
{
	this->ClearLineEllipse(x - r, y + r, x + r, y - r);
}

void WINAPI EasyHDC::ClearLineCircle(const POINT* lppoint, int r) noexcept
{
	this->ClearLineCircle(lppoint->x, lppoint->y, r);
}

void WINAPI EasyHDC::SolidCircle(int x, int y, int r) noexcept
{
	this->SolidEllipse(x - r, y + r, x + r, y - r);
}

void WINAPI EasyHDC::SolidCircle(const POINT* lppoint, int r) noexcept
{
	this->SolidCircle(lppoint->x, lppoint->y, r);
}

void WINAPI EasyHDC::ClearSolidCircle(int x, int y, int r) noexcept
{
	this->ClearSolidEllipse(x - r, y + r, x + r, y - r);
}

void WINAPI EasyHDC::ClearSolidCircle(const POINT* lppoint, int r) noexcept
{
	this->ClearSolidCircle(lppoint->x, lppoint->y, r);
}

void WINAPI EasyHDC::FillCircle(int x, int y, int r) noexcept
{
	this->FillEllipse(x - r, y + r, x + r, y - r);
}

void WINAPI EasyHDC::FillCircle(const POINT* lppoint, int r) noexcept
{
	this->FillCircle(lppoint->x, lppoint->y, r);
}

void WINAPI EasyHDC::ClearFillCircle(int x, int y, int r) noexcept
{
	this->ClearFillEllipse(x - r, y + r, x + r, y - r);
}

void WINAPI EasyHDC::ClearFillCircle(const POINT* lppoint, int r) noexcept
{
	this->ClearFillCircle(lppoint->x, lppoint->y, r);
}

void WINAPI EasyHDC::Circle(int x, int y, int r) noexcept
{
	this->Ellipse(x - r, y + r, x + r, y - r);
}

void WINAPI EasyHDC::Circle(const POINT* lppoint, int r) noexcept
{
	this->Circle(lppoint->x, lppoint->y, r);
}

void WINAPI EasyHDC::ClearCircle(int x, int y, int r) noexcept
{
	this->ClearEllipse(x - r, y + r, x + r, y - r);
}

void WINAPI EasyHDC::ClearCircle(const POINT* lppoint, int r) noexcept
{
	this->ClearCircle(lppoint->x, lppoint->y, r);
}

void WINAPI EasyHDC::LineRoundRect(int left, int top, int right, int bottom, int width, int height) noexcept
{
	BEGINLINE;
	::RoundRect(ImageDC_Src, left, top, right, bottom, width, height);
	ENDLINE;
}

void WINAPI EasyHDC::LineRoundRect(LPCRECT lprect, int width, int height) noexcept
{
	this->LineRoundRect(lprect->left, lprect->top, lprect->right, lprect->bottom, width, height);
}

void WINAPI EasyHDC::ClearLineRoundRect(int left, int top, int right, int bottom, int width, int height) noexcept
{
	BEGINCLEARLINE;
	::RoundRect(ImageDC_Src, left, top, right, bottom, width, height);
	ENDCLEARLINE;
}

void WINAPI EasyHDC::ClearLineRoundRect(LPCRECT lprect, int width, int height) noexcept
{
	this->ClearLineRoundRect(lprect->left, lprect->top, lprect->right, lprect->bottom, width, height);
}

void WINAPI EasyHDC::SolidRoundRect(int left, int top, int right, int bottom, int width, int height) noexcept
{
	BEGINSOLID;
	::RoundRect(ImageDC_Src, left, top, right, bottom, width, height);
	ENDSOLID;
}

void WINAPI EasyHDC::SolidRoundRect(LPCRECT lprect, int width, int height) noexcept
{
	this->SolidRoundRect(lprect->left, lprect->top, lprect->right, lprect->bottom, width, height);
}

void WINAPI EasyHDC::ClearSolidRoundRect(int left, int top, int right, int bottom, int width, int height) noexcept
{
	BEGINCLEARSOLID;
	::RoundRect(ImageDC_Src, left, top, right, bottom, width, height);
	ENDCLEARSOLID;
}

void WINAPI EasyHDC::ClearSolidRoundRect(LPCRECT lprect, int width, int height) noexcept
{
	this->ClearSolidRoundRect(lprect->left, lprect->top, lprect->right, lprect->bottom, width, height);
}

void WINAPI EasyHDC::FillRoundRect(int left, int top, int right, int bottom, int width, int height) noexcept
{
	BEGINFILL;
	::RoundRect(ImageDC_Src, left, top, right, bottom, width, height);
	ENDFILL;
}

void WINAPI EasyHDC::FillRoundRect(LPCRECT lprect, int width, int height) noexcept
{
	this->FillRoundRect(lprect->left, lprect->top, lprect->right, lprect->bottom, width, height);
}

void WINAPI EasyHDC::ClearFillRoundRect(int left, int top, int right, int bottom, int width, int height) noexcept
{
	BEGINCLEARFILL;
	::RoundRect(ImageDC_Src, left, top, right, bottom, width, height);
	ENDCLEARFILL;
}

void WINAPI EasyHDC::ClearFillRoundRect(LPCRECT lprect, int width, int height) noexcept
{
	this->ClearFillRoundRect(lprect->left, lprect->top, lprect->right, lprect->bottom, width, height);
}

void WINAPI EasyHDC::RoundRect(int left, int top, int right, int bottom, int width, int height) noexcept
{
	this->LineRoundRect(left, top, right, bottom, width, height);
}

void WINAPI EasyHDC::RoundRect(LPCRECT lprect, int width, int height) noexcept
{
	this->RoundRect(lprect->left, lprect->top, lprect->right, lprect->bottom, width, height);
}

void WINAPI EasyHDC::ClearRoundRect(int left, int top, int right, int bottom, int width, int height) noexcept
{
	this->ClearFillRoundRect(left, top, right, bottom, width, height);
}

void WINAPI EasyHDC::ClearRoundRect(LPCRECT lprect, int width, int height) noexcept
{
	this->ClearRoundRect(lprect->left, lprect->top, lprect->right, lprect->bottom, width, height);
}

void WINAPI EasyHDC::Arc(int left, int top, int right, int bottom, int x1, int y1, int x2, int y2) noexcept
{
	::Arc(ImageDC_Src, left, top, right, bottom, x1, y1, x2, y2);
}

void WINAPI EasyHDC::Arc(LPCRECT lprect, const POINT* lpp1, const POINT* lpp2) noexcept
{
	this->Arc(lprect->left, lprect->top, lprect->right, lprect->bottom, lpp1->x, lpp1->y, lpp2->x, lpp2->y);
}

void WINAPI EasyHDC::ArcTo(int left, int top, int right, int bottom, int x1, int y1, int x2, int y2) noexcept
{
	::ArcTo(ImageDC_Src, left, top, right, bottom, x1, y1, x2, y2);
}

void WINAPI EasyHDC::ArcTo(LPCRECT lprect, const POINT* lpp1, const POINT* lpp2) noexcept
{
	this->ArcTo(lprect->left, lprect->top, lprect->right, lprect->bottom, lpp1->x, lpp1->y, lpp2->x, lpp2->y);
}

void WINAPI EasyHDC::LinePie(int left, int top, int right, int bottom, int x1, int y1, int x2, int y2) noexcept
{
	BEGINLINE;
	::Pie(ImageDC_Src, left, top, right, bottom, x1, y1, x2, y2);
	ENDLINE;
}

void WINAPI EasyHDC::LinePie(LPCRECT lprect, const POINT* lpp1, const POINT* lpp2) noexcept
{
	this->LinePie(lprect->left, lprect->top, lprect->right, lprect->bottom, lpp1->x, lpp1->y, lpp2->x, lpp2->y);
}

void WINAPI EasyHDC::ClearLinePie(int left, int top, int right, int bottom, int x1, int y1, int x2, int y2) noexcept
{
	BEGINCLEARLINE;
	::Pie(ImageDC_Src, left, top, right, bottom, x1, y1, x2, y2);
	ENDCLEARLINE;
}

void WINAPI EasyHDC::ClearLinePie(LPCRECT lprect, const POINT* lpp1, const POINT* lpp2) noexcept
{
	this->ClearLinePie(lprect->left, lprect->top, lprect->right, lprect->bottom, lpp1->x, lpp1->y, lpp2->x, lpp2->y);
}

void WINAPI EasyHDC::SolidPie(int left, int top, int right, int bottom, int x1, int y1, int x2, int y2) noexcept
{
	BEGINSOLID;
	::Pie(ImageDC_Src, left, top, right, bottom, x1, y1, x2, y2);
	ENDSOLID;
}

void WINAPI EasyHDC::SolidPie(LPCRECT lprect, const POINT* lpp1, const POINT* lpp2) noexcept
{
	this->SolidPie(lprect->left, lprect->top, lprect->right, lprect->bottom, lpp1->x, lpp1->y, lpp2->x, lpp2->y);
}

void WINAPI EasyHDC::ClearSolidPie(int left, int top, int right, int bottom, int x1, int y1, int x2, int y2) noexcept
{
	BEGINCLEARSOLID;
	::Pie(ImageDC_Src, left, top, right, bottom, x1, y1, x2, y2);
	ENDCLEARSOLID;
}

void WINAPI EasyHDC::ClearSolidPie(LPCRECT lprect, const POINT* lpp1, const POINT* lpp2) noexcept
{
	this->ClearSolidPie(lprect->left, lprect->top, lprect->right, lprect->bottom, lpp1->x, lpp1->y, lpp2->x, lpp2->y);
}

void WINAPI EasyHDC::FillPie(int left, int top, int right, int bottom, int x1, int y1, int x2, int y2) noexcept
{
	BEGINFILL;
	::Pie(ImageDC_Src, left, top, right, bottom, x1, y1, x2, y2);
	ENDFILL;
}

void WINAPI EasyHDC::FillPie(LPCRECT lprect, const POINT* lpp1, const POINT* lpp2) noexcept
{
	this->FillPie(lprect->left, lprect->top, lprect->right, lprect->bottom, lpp1->x, lpp1->y, lpp2->x, lpp2->y);
}

void WINAPI EasyHDC::ClearFillPie(int left, int top, int right, int bottom, int x1, int y1, int x2, int y2) noexcept
{
	BEGINCLEARFILL;
	::Pie(ImageDC_Src, left, top, right, bottom, x1, y1, x2, y2);
	ENDCLEARFILL;
}

void WINAPI EasyHDC::ClearFillPie(LPCRECT lprect, const POINT* lpp1, const POINT* lpp2) noexcept
{
	this->ClearFillPie(lprect->left, lprect->top, lprect->right, lprect->bottom, lpp1->x, lpp1->y, lpp2->x, lpp2->y);
}

void WINAPI EasyHDC::Pie(int left, int top, int right, int bottom, int x1, int y1, int x2, int y2) noexcept
{
	this->LinePie(left, top, right, bottom, x1, y1, x2, y2);
}

void WINAPI EasyHDC::Pie(LPCRECT lprect, const POINT* lpp1, const POINT* lpp2) noexcept
{
	this->Pie(lprect->left, lprect->top, lprect->right, lprect->bottom, lpp1->x, lpp1->y, lpp2->x, lpp2->y);
}

void WINAPI EasyHDC::ClearPie(int left, int top, int right, int bottom, int x1, int y1, int x2, int y2) noexcept
{
	this->ClearFillPie(left, top, right, bottom, x1, y1, x2, y2);
}

void WINAPI EasyHDC::ClearPie(LPCRECT lprect, const POINT* lpp1, const POINT* lpp2) noexcept
{
	this->ClearPie(lprect->left, lprect->top, lprect->right, lprect->bottom, lpp1->x, lpp1->y, lpp2->x, lpp2->y);
}

void WINAPI EasyHDC::LinePolygon(const POINT* apt, int cpt) noexcept
{
	BEGINLINE;
	::Polygon(ImageDC_Src, apt, cpt);
	ENDLINE;
}

void WINAPI EasyHDC::ClearLinePolygon(const POINT* apt, int cpt) noexcept
{
	BEGINCLEARLINE;
	::Polygon(ImageDC_Src, apt, cpt);
	ENDCLEARLINE;
}

void WINAPI EasyHDC::SolidPolygon(const POINT* apt, int cpt) noexcept
{
	BEGINSOLID;
	::Polygon(ImageDC_Src, apt, cpt);
	ENDSOLID;
}

void WINAPI EasyHDC::ClearSolidPolygon(const POINT* apt, int cpt) noexcept
{
	BEGINCLEARSOLID;
	::Polygon(ImageDC_Src, apt, cpt);
	ENDCLEARSOLID;
}

void WINAPI EasyHDC::FillPolygon(const POINT* apt, int cpt) noexcept
{
	BEGINFILL;
	::Polygon(ImageDC_Src, apt, cpt);
	ENDFILL;
}

void WINAPI EasyHDC::ClearFillPolygon(const POINT* apt, int cpt) noexcept
{
	BEGINCLEARFILL;
	::Polygon(ImageDC_Src, apt, cpt);
	ENDCLEARFILL;
}

void WINAPI EasyHDC::Polygon(const POINT* apt, int cpt) noexcept
{
	this->LinePolygon(apt, cpt);
}

void WINAPI EasyHDC::ClearPolygon(const POINT* apt, int cpt) noexcept
{
	this->ClearFillPolygon(apt, cpt);
}

void WINAPI EasyHDC::PolyLine(const POINT* apt, int cpt) noexcept
{
	::Polyline(ImageDC_Src, apt, cpt);
}

void WINAPI EasyHDC::PolyLineTo(const POINT* apt, int cpt) noexcept
{
	::PolylineTo(ImageDC_Src, apt, cpt);
}

void WINAPI EasyHDC::PolyBezier(const POINT* apt, int cpt) noexcept
{
	::PolyBezier(ImageDC_Src, apt, cpt);
}

void WINAPI EasyHDC::PolyBezierTo(const POINT* apt, int cpt) noexcept
{
	::PolyBezierTo(ImageDC_Src, apt, cpt);
}

void WINAPI EasyHDC::FloodFill(int x, int y, COLORREF color, UINT type) noexcept
{
	::ExtFloodFill(ImageDC_Src, x, y, color, type);
}

void WINAPI EasyHDC::FloodFill(const POINT* lppoint, COLORREF color, UINT type) noexcept
{
	this->FloodFill(lppoint->x, lppoint->y, color, type);
}

void WINAPI EasyHDC::Fill() noexcept
{
	const auto ImageSize = this->GetBitmapSize();
	this->FillRect(0, 0, ImageSize.cx, ImageSize.cy);
}

void WINAPI EasyHDC::Solid() noexcept
{
	const auto ImageSize = this->GetBitmapSize();
	this->SolidRect(0, 0, ImageSize.cx, ImageSize.cy);
}

void WINAPI EasyHDC::Clear() noexcept
{
	const auto ImageSize = this->GetBitmapSize();
	this->ClearRect(0, 0, ImageSize.cx, ImageSize.cy);
}

void WINAPI EasyHDC::OutTextExXY(int x, int y, LPCTSTR str, UINT options, LPCRECT lprect, const int* lpDx, int count) noexcept
{
	if (count == -1)
		count = lstrlen(str);
	::ExtTextOut(ImageDC_Src, x, y, options, lprect, str, count, lpDx);
}

void WINAPI EasyHDC::OutTextExXY(int x, int y, TCHAR ch, UINT options, LPCRECT lprect, const int* lpDx) noexcept
{
	this->OutTextExXY(x, y, &ch, options, lprect, lpDx, 1);
}

void WINAPI EasyHDC::OutTextXY(int x, int y, LPCTSTR str, int count) noexcept
{
	this->OutTextExXY(x, y, str, ETO_CLIPPED, NULL, NULL, count);
}

void WINAPI EasyHDC::OutTextXY(int x, int y, TCHAR ch) noexcept
{
	this->OutTextXY(x, y, &ch, 1);
}

void WINAPI EasyHDC::DrawCTextEx(LPCTSTR str, PRECT prect, UINT format, LPDRAWTEXTPARAMS lpdtp, int count) noexcept
{
	if (count == -1)
		count = lstrlen(str);
	::DrawTextEx(ImageDC_Src, (LPTSTR)str, count, prect, format, lpdtp);
}

void WINAPI EasyHDC::DrawCTextEx(TCHAR ch, PRECT prect, UINT format, LPDRAWTEXTPARAMS lpdtp) noexcept
{
	this->DrawCTextEx(&ch, prect, format, lpdtp, 1);
}

void WINAPI EasyHDC::DrawCText(LPCTSTR str, PRECT prect, UINT format, int count) noexcept
{
	this->DrawCTextEx(str, prect, format, NULL, count);
}

void WINAPI EasyHDC::DrawCText(TCHAR ch, PRECT prect, UINT format) noexcept
{
	this->DrawCText(&ch, prect, format, 1);
}

void WINAPI EasyHDC::DrawTextEx(LPCTSTR str, PRECT prect, UINT format, LPDRAWTEXTPARAMS lpdtp, int count) noexcept
{
	if (count == -1)
		count = lstrlen(str);
	LPTSTR tstr = new TCHAR[count + 4];
	lstrcpy(tstr, str);
	::DrawTextEx(ImageDC_Src, tstr, count, prect, format, lpdtp);
	delete tstr;
}

void WINAPI EasyHDC::DrawTextEx(TCHAR ch, PRECT prect, UINT format, LPDRAWTEXTPARAMS lpdtp) noexcept
{
	this->DrawTextEx((LPCTSTR)&ch, prect, format, lpdtp, 1);
}

void WINAPI EasyHDC::DrawText(LPCTSTR str, PRECT prect, UINT format, int count) noexcept
{
	this->DrawTextEx(str, prect, format, NULL, count);
}

void WINAPI EasyHDC::DrawText(TCHAR ch, PRECT prect, UINT format) noexcept
{
	this->DrawText((LPCTSTR)&ch, prect, format, 1);
}

void WINAPI EasyHDC::DrawTextEx(LPTSTR str, PRECT prect, UINT format, LPDRAWTEXTPARAMS lpdtp, int count) noexcept
{
	if (count == -1)
		count = lstrlen(str);
	::DrawTextEx(ImageDC_Src, str, count, prect, format, lpdtp);
}

void WINAPI EasyHDC::DrawText(LPTSTR str, PRECT prect, UINT format, int count) noexcept
{
	this->DrawTextEx(str, prect, format, NULL, count);
}

EasyHDC& WINAPI EasyHDC::Src() noexcept
{
	return *this;
}

const EasyHDC& WINAPI EasyHDC::Src() const noexcept
{
	return *this;
}

_EASYG_END