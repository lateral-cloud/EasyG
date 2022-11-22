#pragma once

#include "EasyDrawWindow.h"

namespace EasyGraph {
	HWND WINAPI DrawWindow::OpenWindow(LPCWSTR lpWindowName) {
		HWND hWnd = window.OpenWindow(lpWindowName);
		paint.SetHWND(hWnd);
		return hWnd;
	}

	DrawWindow::DrawWindow() : window(), paint() {
	}

	DrawWindow::~DrawWindow() {}
}