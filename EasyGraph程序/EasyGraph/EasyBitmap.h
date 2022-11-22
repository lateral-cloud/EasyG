#pragma once

#include "EasyGraphDef.h"

namespace EasyGraph {
	void CopyBitmap(HBITMAP bitmapSrc, HDC dcDst) {
		BITMAP bitmapinfo;
		::GetObject(bitmapSrc, sizeof(BITMAP), &bitmapinfo);
		HDC dcSrc;
		dcSrc = ::CreateCompatibleDC(dcDst);
		::SelectObject(dcSrc, bitmapSrc);
		::BitBlt(dcDst, 0, 0, bitmapinfo.bmWidth, bitmapinfo.bmHeight, dcSrc, 0, 0, SRCCOPY);
		::DeleteDC(dcSrc);
	}

	void CopyBitmap(HBITMAP bitmapSrc, HBITMAP bitmapDst ,HDC dc = NULL) {
		HDC dcDst;
		dcDst = ::CreateCompatibleDC(dc);
		::SelectObject(dcDst, bitmapDst);
		CopyBitmap(bitmapSrc, dcDst);
		::DeleteDC(dcDst);
	}
}