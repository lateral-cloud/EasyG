#pragma once

#include "EasyGraphDef.h"

namespace EasyGraph {
	class Paint {
	public:
		//���ô���
		void WINAPI SetHWND(HWND hWnd);

		//����ͼƬ��С
		void WINAPI Resize(int w, int h);

		//���ñ���ʽ
		void WINAPI SetPenStyle(UINT style);

		//���ñ���ʽ
		void WINAPI SetPenWidth(LONG width);

		//���ñ���ɫ
		void WINAPI SetPenColor(COLORREF color);

		//���ñ�
		void WINAPI SetPen(UINT style, LONG width, COLORREF color);

		//���ñ�
		void WINAPI SetPen(LOGPEN logpen);

		//���ñ�
		void WINAPI SetPen(HPEN hpen);

		//����ˢ����ʽ
		void WINAPI SetBrushStyle(UINT style);

		//����ˢ����ɫ
		void WINAPI SetBrushColor(COLORREF color);

		//����ˢ����Ӱ
		void WINAPI SetBrushHatch(ULONG_PTR hatch);

		//����ˢ��
		void WINAPI SetBrush(UINT style, COLORREF color, ULONG_PTR hatch);

		//����ˢ��
		void WINAPI SetBrush(LOGBRUSH logbrush);

		//����ˢ��
		void WINAPI SetBrush(HBRUSH hbrush);

		//��ñ�
		LOGPEN WINAPI GetLogPen() const;

		//���ˢ��
		LOGBRUSH WINAPI GetLogBrush() const;

		//��ñ�
		HPEN WINAPI GetHPen() const;

		//���ˢ��
		HBRUSH WINAPI GetHBrush() const;

		//���ͼ��
		void WINAPI GetImage(HDC dcDst) const;

		//���ͼ��
		void WINAPI GetImage(HBITMAP* bitmapDst) const;

		//���ͼ��
		HBITMAP WINAPI GetImage() const;

		//���´���
		void WINAPI Update();

		//��ˢ����ɫ������
		void WINAPI Fill(RECT rect);

		//��ˢ����ɫ�������
		void WINAPI Fill(HRGN rgn);

		//��ˢ����ɫ��䱳��
		void WINAPI Fill();

		//�������
		void WINAPI OutText(LPCWSTR str, int x, int y);

		//���߶�
		void WINAPI Line(int x1, int y1, int x2, int y2);

		//����Բ
		void WINAPI Ellipse(int left, int top, int right, int bottom);

		//��Բ
		void WINAPI Circle(int x, int y, int r);

		//��ͼ
		void WINAPI Image(HBITMAP image, int x, int y, DWORD dwROP = SRCCOPY);

		Paint(HWND hWnd = NULL);

		~Paint();

		void WINAPI defaults();

		//����
		PAINTSTRUCT ps;
		
		//����
		HDC hdc;

		//���ھ��
		HWND hwnd;

		HPEN hPen;

		HBRUSH hBrush;
	};
}