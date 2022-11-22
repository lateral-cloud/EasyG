#pragma once

#include "EasyWindow.h"
#include "EasyWindowProc.h"

namespace EasyGraph {
#ifdef UNICODE
    ATOM WINAPI Window::RegisterClassExW() {
        return ::RegisterClassExW(&wc);
    }

    HWND WINAPI Window::CreateWindowExW(
        const DWORD& dwExStyle,
        const LPCWSTR& lpWindowName,                            //窗口名
        const DWORD& dwStyle,                                   //窗口风格
        const int& X,                                           //窗口左上角X坐标
        const int& Y,                                           //窗口左上角Y坐标
        const int& nWidth,                                      //窗口宽度
        const int& nHeight,                                     //窗口高度
        const HWND& hWndParent,                                 //父窗口句柄
        const HMENU& hMenu,                                     //菜单句柄
        const HINSTANCE& hInstance,                             //实例句柄
        const LPVOID& lpParam)                                  //其他参数
    {
        return hwnd = ::CreateWindowExW(dwExStyle, wc.lpszClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    }

    HWND WINAPI Window::OpenWindow(const LPCWSTR& lpWindowName) {
        RegisterClassExW();
        CreateWindowExW(0L, lpWindowName);
        ShowWindow();
        UpdateWindow();
        return hwnd;
    }

    int WINAPI Window::MessageBoxExW(LPCWSTR lpText,    //内容
        LPCWSTR lpCaption,                              //标题
        UINT uType,                                     //参数
        WORD wLanguageId)                               //语言标识符
    {
        return ::MessageBoxExW(hwnd, lpText, lpCaption, uType, wLanguageId);
    }
#else
    ATOM WINAPI Window::RegisterClassExA() {
        return ::RegisterClassExA(&wc);
    }

    HWND WINAPI Window::CreateWindowExA(
        const DWORD& dwExStyle,
        const LPCSTR& lpWindowName,                             //窗口名
        const DWORD& dwStyle,                                   //窗口风格
        const int& X,                                           //窗口左上角X坐标
        const int& Y,                                           //窗口左上角Y坐标
        const int& nWidth,                                      //窗口宽度
        const int& nHeight,                                     //窗口高度
        const HWND& hWndParent,                                 //父窗口句柄
        const HMENU& hMenu,                                     //菜单句柄
        const HINSTANCE& hInstance,                             //实例句柄
        const LPVOID& lpParam)                                  //其他参数
    {
        return hwnd = ::CreateWindowExA(dwExStyle, wc.lpszClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    }

    HWND WINAPI Window::OpenWindow(const LPCSTR& lpWindowName) {
        RegisterClassExA();
        CreateWindowExA(0L, lpWindowName);
        ShowWindow();
        UpdateWindow();
        return hwnd;
    }

    int WINAPI Window::MessageBoxExA(LPCSTR lpText,     //内容
        LPCSTR lpCaption,                               //标题
        UINT uType,                                     //参数
        WORD wLanguageId)                               //语言标识符
    {
        return ::MessageBoxExA(hwnd, lpText, lpCaption, uType, wLanguageId);
    }
#endif // UNICODE

    BOOL WINAPI Window::DestroyWindow() {
        return ::DestroyWindow(hwnd);
    }

    BOOL WINAPI Window::ShowWindow(int nCmdShow) {
        return ::ShowWindow(hwnd, nCmdShow);
    }

    BOOL WINAPI Window::CloseWindow() {
        return ::CloseWindow(hwnd);
    }

    BOOL WINAPI Window::UpdateWindow() {
        return ::UpdateWindow(hwnd);
    }

    const HWND& WINAPI Window::GetHWND() const {
        return hwnd;
    }

    void Window::defaults(WNDCLASSEX& WC) {
        WC.cbSize = sizeof(WNDCLASSEX);
        WC.style = 0;
        WC.lpfnWndProc = WindowProc;
        WC.cbClsExtra = 0;
        //WC.cbWndExtra = sizeof(CBitmap);
        WC.cbWndExtra = 0;
        WC.hInstance = NULL;
        WC.hIcon = NULL;
        WC.hCursor = LoadCursor(NULL, IDC_HAND);
        WC.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        WC.lpszMenuName = NULL;
        WC.lpszClassName = TEXT("WINDOW");
        WC.hIconSm = NULL;
    }

    void Window::defaults() {
        defaults(wc);
    }

    Window::Window() {
        defaults();
    }

    Window::~Window() {
        DestroyWindow();
    }
}