#pragma once

#include "EasyGraphDef.h"

namespace EasyGraph {
	class Paint {
	public:
		//设置窗口
		void WINAPI SetHWND(HWND hWnd);

		//设置图片大小
		void WINAPI Resize(int w, int h);

		//设置笔样式
		void WINAPI SetPenStyle(UINT style);

		//设置笔样式
		void WINAPI SetPenWidth(LONG width);

		//设置笔颜色
		void WINAPI SetPenColor(COLORREF color);

		//设置笔
		void WINAPI SetPen(UINT style, LONG width, COLORREF color);

		//设置笔
		void WINAPI SetPen(LOGPEN logpen);

		//设置笔
		void WINAPI SetPen(HPEN hpen);

		//设置刷子样式
		void WINAPI SetBrushStyle(UINT style);

		//设置刷子颜色
		void WINAPI SetBrushColor(COLORREF color);

		//设置刷子阴影
		void WINAPI SetBrushHatch(ULONG_PTR hatch);

		//设置刷子
		void WINAPI SetBrush(UINT style, COLORREF color, ULONG_PTR hatch);

		//设置刷子
		void WINAPI SetBrush(LOGBRUSH logbrush);

		//设置刷子
		void WINAPI SetBrush(HBRUSH hbrush);

		//获得笔
		LOGPEN WINAPI GetLogPen() const;

		//获得刷子
		LOGBRUSH WINAPI GetLogBrush() const;

		//获得笔
		HPEN WINAPI GetHPen() const;

		//获得刷子
		HBRUSH WINAPI GetHBrush() const;

		//获得图像
		void WINAPI GetImage(HDC dcDst) const;

		//获得图像
		void WINAPI GetImage(HBITMAP* bitmapDst) const;

		//获得图像
		HBITMAP WINAPI GetImage() const;

		//更新窗口
		void WINAPI Update();

		//用刷子颜色填充矩形
		void WINAPI Fill(RECT rect);

		//用刷子颜色填充区域
		void WINAPI Fill(HRGN rgn);

		//用刷子颜色填充背景
		void WINAPI Fill();

		//输出文字
		void WINAPI OutText(LPCWSTR str, int x, int y);

		//画线段
		void WINAPI Line(int x1, int y1, int x2, int y2);

		//画椭圆
		void WINAPI Ellipse(int left, int top, int right, int bottom);

		//画圆
		void WINAPI Circle(int x, int y, int r);

		//画图
		void WINAPI Image(HBITMAP image, int x, int y, DWORD dwROP = SRCCOPY);

		Paint(HWND hWnd = NULL);

		~Paint();

		void WINAPI defaults();

		//画笔
		PAINTSTRUCT ps;
		
		//画布
		HDC hdc;

		//窗口句柄
		HWND hwnd;

		HPEN hPen;

		HBRUSH hBrush;
	};
}