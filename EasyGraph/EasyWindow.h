#pragma once

#include "EasyGraphDef.h"

namespace EasyGraph {
    class Window
    {
    public:
#ifdef UNICODE
        //注册窗口
        ATOM WINAPI RegisterClassExW();

        //创建窗口
        HWND WINAPI CreateWindowExW(
            const DWORD& dwExStyle,
            const LPCWSTR& lpWindowName,                //窗口名
            const DWORD& dwStyle = WS_OVERLAPPEDWINDOW, //窗口风格
            const int& X = CW_USEDEFAULT,               //窗口左上角X坐标
            const int& Y = CW_USEDEFAULT,               //窗口左上角Y坐标
            const int& nWidth = CW_USEDEFAULT,          //窗口宽度
            const int& nHeight = CW_USEDEFAULT,         //窗口高度
            const HWND& hWndParent = NULL,              //父窗口句柄
            const HMENU& hMenu = NULL,                  //菜单句柄
            const HINSTANCE& hInstance = NULL,          //实例句柄
            const LPVOID& lpParam = NULL);              //其他参数

        //打开窗口
        HWND WINAPI OpenWindow(const LPCWSTR& lpWindowName);

        //弹出窗口
        int WINAPI MessageBoxExW(LPCWSTR lpText,        //内容
            LPCWSTR lpCaption,                          //标题
            UINT uType = MB_OK,                         //参数
            WORD wLanguageId = 0);                      //语言标识符
#else
        //注册窗口
        ATOM WINAPI RegisterClassExA();

        //创建窗口
        HWND WINAPI CreateWindowExA(
            const DWORD& dwExStyle,
            const LPCSTR& lpWindowName,                 //窗口名
            const DWORD& dwStyle = WS_OVERLAPPEDWINDOW, //窗口风格
            const int& X = CW_USEDEFAULT,               //窗口左上角X坐标
            const int& Y = CW_USEDEFAULT,               //窗口左上角Y坐标
            const int& nWidth = CW_USEDEFAULT,          //窗口宽度
            const int& nHeight = CW_USEDEFAULT,         //窗口高度
            const HWND& hWndParent = NULL,              //父窗口句柄
            const HMENU& hMenu = NULL,                  //菜单句柄
            const HINSTANCE& hInstance = NULL,          //实例句柄
            const LPVOID& lpParam = NULL);              //其他参数

        //打开窗口
        HWND WINAPI OpenWindow(const LPCSTR& lpWindowName);

        //弹出窗口
        int WINAPI MessageBoxExA(LPCSTR lpText,         //内容
            LPCSTR lpCaption,                            //标题
            UINT uType = MB_OK,                         //参数
            WORD wLanguageId = 0);                      //语言标识符
#endif // UNICODE

        //关闭窗口
        BOOL WINAPI DestroyWindow();

        //显示窗口
        BOOL WINAPI ShowWindow(int nCmdShow = SW_SHOWNORMAL);

        //最小化窗口
        BOOL WINAPI CloseWindow();

        //更新窗口
        BOOL WINAPI UpdateWindow();

        //获得窗口句柄
        const HWND& WINAPI GetHWND() const;

        //默认设置
        static void defaults(WNDCLASSEX& WC);

        void defaults();

        Window();

        ~Window();

        //窗口
        WNDCLASSEX wc;

        //窗口句柄
        HWND hwnd;
    };
}