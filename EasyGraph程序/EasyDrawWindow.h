#pragma once

#include "EasyWindow.h"
#include "EasyPaint.h"

namespace EasyGraph {
	class DrawWindow
	{
	public:
		//´°¿Ú
		Window window;

		//»­±Ê
		Paint paint;

		HWND WINAPI OpenWindow(LPCWSTR lpWindowName);

		DrawWindow();

		~DrawWindow();
	};
}