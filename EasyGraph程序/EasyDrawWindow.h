#pragma once

#include "EasyWindow.h"
#include "EasyPaint.h"

namespace EasyGraph {
	class DrawWindow
	{
	public:
		//����
		Window window;

		//����
		Paint paint;

		HWND WINAPI OpenWindow(LPCWSTR lpWindowName);

		DrawWindow();

		~DrawWindow();
	};
}