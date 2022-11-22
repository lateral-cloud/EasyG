#pragma once

#include "EasyDrawWindow.h"

namespace EasyGraph {
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam) {
        switch (uMsg)
        {
        case WM_CLOSE: {
            DestroyWindow(hwnd);
            break;
        }
        case WM_DESTROY: {
            break;
        }
        case WM_LBUTTONDOWN:
        {
            break;
        }
        case WM_KEYDOWN: {
            break;
        }
        case WM_PAINT:
        {
            break;
        }
        case WM_ERASEBKGND:
        {
            //break;
        }
        default: {
            return DefWindowProc(hwnd, uMsg, wparam, lparam);
        }
        }
        return 0;
    }
}