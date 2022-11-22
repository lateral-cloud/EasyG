#pragma once

#include "EasyPaint.h"

#define EW_DEFAULTSIZE 500
#define EW_DEFAULTCOLOR 0xffffffff

namespace EasyGraph {
	Paint::Paint(HWND hWnd) {
		hPen = NULL;
		hBrush = NULL;
		hdc = NULL;
		hwnd = NULL;
		if (hWnd != NULL) {
			::BeginPaint(hwnd = hWnd, &ps);
			hdc = ::CreateCompatibleDC(ps.hdc);
			HBITMAP bitmap;
			bitmap = ::CreateCompatibleBitmap(ps.hdc, EW_DEFAULTSIZE, EW_DEFAULTSIZE);
			::SelectObject(hdc, bitmap);
		}
		defaults();
	}

	void WINAPI Paint::Resize(int w, int h) {
		HBITMAP bitmap, bitmapOld;
		bitmap = ::CreateCompatibleBitmap(ps.hdc, w, h);
		bitmapOld = (HBITMAP)::SelectObject(hdc, bitmap);
		DeleteObject(bitmapOld);
	}

	void WINAPI Paint::SetHWND(HWND hWnd) {
		if (hwnd != NULL) {
			::EndPaint(hwnd, &ps);
		}
		if (hWnd != NULL) {
			::BeginPaint(hWnd, &ps);
		}
		hwnd = hWnd;
	}

	Paint::~Paint() {
		if (hwnd != NULL) {
			::EndPaint(hwnd, &ps);
		}
	}

	void WINAPI Paint::defaults() {
		if (hdc != NULL) {
			SetPen(PS_SOLID, 1, EW_DEFAULTCOLOR);
			SetBrush(BS_SOLID, EW_DEFAULTCOLOR, NULL);
		}
	}

	void WINAPI Paint::SetPenStyle(UINT style) {
		LOGPEN LogPen;
		::GetObject(hPen, sizeof(LOGPEN), &LogPen);
		LogPen.lopnStyle = style;
		hPen = ::CreatePenIndirect(&LogPen);
		::DeleteObject(::SelectObject(hdc, hPen));
	}

	void WINAPI Paint::SetPenWidth(LONG width) {
		LOGPEN LogPen;
		::GetObject(hPen, sizeof(LOGPEN), &LogPen);
		LogPen.lopnWidth.x = width;
		hPen = ::CreatePenIndirect(&LogPen);
		::DeleteObject(::SelectObject(hdc, hPen));
	}

	void WINAPI Paint::SetPenColor(COLORREF color) {
		LOGPEN LogPen;
		::GetObject(hPen, sizeof(LOGPEN), &LogPen);
		LogPen.lopnColor = color;
		hPen = ::CreatePenIndirect(&LogPen);
		::DeleteObject(::SelectObject(hdc, hPen));
	}

	void WINAPI Paint::SetPen(UINT style, LONG width, COLORREF color) {
		LOGPEN LogPen;
		LogPen.lopnStyle = style;
		LogPen.lopnWidth.x = width;
		LogPen.lopnColor = color;
		hPen = ::CreatePenIndirect(&LogPen);
		::DeleteObject(::SelectObject(hdc, hPen));
	}

	void WINAPI Paint::SetPen(LOGPEN logpen) {
		hPen = ::CreatePenIndirect(&logpen);
		::DeleteObject(::SelectObject(hdc, hPen));
	}

	void WINAPI Paint::SetPen(HPEN hpen) {
		hPen = hpen;
		::DeleteObject(::SelectObject(hdc, hPen));
	}
	
	void WINAPI Paint::SetBrushStyle(UINT style) {
		LOGBRUSH LogBrush;
		::GetObject(hBrush, sizeof(LOGBRUSH), &LogBrush);
		LogBrush.lbStyle = style;
		hBrush = ::CreateBrushIndirect(&LogBrush);
		::DeleteObject(::SelectObject(hdc, hBrush));
	}

	void WINAPI Paint::SetBrushColor(COLORREF color) {
		LOGBRUSH LogBrush;
		::GetObject(hBrush, sizeof(LOGBRUSH), &LogBrush);
		LogBrush.lbColor = color;
		hBrush = ::CreateBrushIndirect(&LogBrush);
		::DeleteObject(::SelectObject(hdc, hBrush));
	}

	void WINAPI Paint::SetBrushHatch(ULONG_PTR hatch) {
		LOGBRUSH LogBrush;
		::GetObject(hBrush, sizeof(LOGBRUSH), &LogBrush);
		LogBrush.lbHatch = hatch;
		hBrush = ::CreateBrushIndirect(&LogBrush);
		::DeleteObject(::SelectObject(hdc, hBrush));
	}

	void WINAPI Paint::SetBrush(UINT style, COLORREF color, ULONG_PTR hatch) {
		LOGBRUSH LogBrush;
		LogBrush.lbStyle = style;
		LogBrush.lbColor = color;
		LogBrush.lbHatch = hatch;
		hBrush = ::CreateBrushIndirect(&LogBrush);
		::DeleteObject(::SelectObject(hdc, hBrush));
	}

	void WINAPI Paint::SetBrush(LOGBRUSH logbrush) {
		hBrush = ::CreateBrushIndirect(&logbrush);
		::DeleteObject(::SelectObject(hdc, hBrush));
	}

	void WINAPI Paint::SetBrush(HBRUSH hbrush) {
		hBrush = hbrush;
		::DeleteObject(::SelectObject(hdc, hBrush));
	}

	LOGPEN WINAPI Paint::GetLogPen() const {
		LOGPEN LogPen;
		::GetObject(hPen, sizeof(LOGPEN), &LogPen);
		return LogPen;
	}

	LOGBRUSH WINAPI Paint::GetLogBrush() const {
		LOGBRUSH LogBrush;
		::GetObject(hBrush, sizeof(LOGBRUSH), &LogBrush);
		return LogBrush;
	}

	HPEN WINAPI Paint::GetHPen() const {
		return hPen;
	}

	HBRUSH WINAPI Paint::GetHBrush() const {
		return hBrush;
	}

	void WINAPI Paint::GetImage(HDC dcDst) const {
		RECT rect;
		::GetClientRect(hwnd, &rect);
		::BitBlt(dcDst, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);
	}

	void WINAPI Paint::GetImage(HBITMAP* bitmapDst) const {
		RECT rect;
		::GetClientRect(hwnd, &rect);
		HDC dc;
		dc = ::CreateCompatibleDC(hdc);
		::DeleteObject(*bitmapDst);
		*bitmapDst = ::CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		::SelectObject(dc, *bitmapDst);
		::BitBlt(dc, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);
		::DeleteDC(dc);
	}

	HBITMAP WINAPI Paint::GetImage() const {
		HBITMAP bitmapDst = NULL;
		GetImage(&bitmapDst);
		return bitmapDst;
	}

	void WINAPI Paint::Update() {
		if (hwnd != NULL) {
			RECT rect;
			::GetClientRect(hwnd, &rect);
			::BitBlt(ps.hdc, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);
		}
	}

	void WINAPI Paint::Fill(RECT rect) {
		::FillRect(hdc, &rect, hBrush);
		Update();
	}

	void WINAPI Paint::Fill(HRGN rgn) {
		::FillRgn(hdc, rgn, hBrush);
		Update();
	}

	void WINAPI Paint::Fill() {
		RECT rect;
		::GetWindowRect(hwnd, &rect);
		::FillRect(hdc, &rect, hBrush);
		Update();
	}

	void WINAPI Paint::OutText(LPCWSTR str, int x, int y) {
		::TextOut(hdc, x, y, str, lstrlen(str));
		Update();
	}

	void WINAPI Paint::Line(int x1, int y1, int x2, int y2) {
		::MoveToEx(hdc, x1, x2, NULL);
		::LineTo(hdc, x2, y2);
		Update();
	}

	void WINAPI Paint::Ellipse(int left, int top, int right, int bottom) {
		::Ellipse(hdc, left, top, right, bottom);
		Update();
	}

	void WINAPI Paint::Circle(int x, int y, int r) {
		::Ellipse(hdc, x - r, y + r, x + r, y - r);
		Update();
	}
	
	void WINAPI Paint::Image(HBITMAP Img, int x, int y, DWORD dwROP) {
		BITMAP bitmapinfo;
		::GetObject(Img, sizeof(BITMAP), &bitmapinfo);
		HDC dcSrc;
		dcSrc = ::CreateCompatibleDC(ps.hdc);
		::SelectObject(dcSrc, Img);
		::BitBlt(hdc, x, y, bitmapinfo.bmWidth, bitmapinfo.bmHeight, dcSrc, 0, 0, dwROP);
		::DeleteDC(dcSrc);
		Update();
	}
}