#pragma once

#include "EasyGraphDef.h"

namespace EasyGraph {
    class Window
    {
    public:
#ifdef UNICODE
        //ע�ᴰ��
        ATOM WINAPI RegisterClassExW();

        //��������
        HWND WINAPI CreateWindowExW(
            const DWORD& dwExStyle,
            const LPCWSTR& lpWindowName,                //������
            const DWORD& dwStyle = WS_OVERLAPPEDWINDOW, //���ڷ��
            const int& X = CW_USEDEFAULT,               //�������Ͻ�X����
            const int& Y = CW_USEDEFAULT,               //�������Ͻ�Y����
            const int& nWidth = CW_USEDEFAULT,          //���ڿ��
            const int& nHeight = CW_USEDEFAULT,         //���ڸ߶�
            const HWND& hWndParent = NULL,              //�����ھ��
            const HMENU& hMenu = NULL,                  //�˵����
            const HINSTANCE& hInstance = NULL,          //ʵ�����
            const LPVOID& lpParam = NULL);              //��������

        //�򿪴���
        HWND WINAPI OpenWindow(const LPCWSTR& lpWindowName);

        //��������
        int WINAPI MessageBoxExW(LPCWSTR lpText,        //����
            LPCWSTR lpCaption,                          //����
            UINT uType = MB_OK,                         //����
            WORD wLanguageId = 0);                      //���Ա�ʶ��
#else
        //ע�ᴰ��
        ATOM WINAPI RegisterClassExA();

        //��������
        HWND WINAPI CreateWindowExA(
            const DWORD& dwExStyle,
            const LPCSTR& lpWindowName,                 //������
            const DWORD& dwStyle = WS_OVERLAPPEDWINDOW, //���ڷ��
            const int& X = CW_USEDEFAULT,               //�������Ͻ�X����
            const int& Y = CW_USEDEFAULT,               //�������Ͻ�Y����
            const int& nWidth = CW_USEDEFAULT,          //���ڿ��
            const int& nHeight = CW_USEDEFAULT,         //���ڸ߶�
            const HWND& hWndParent = NULL,              //�����ھ��
            const HMENU& hMenu = NULL,                  //�˵����
            const HINSTANCE& hInstance = NULL,          //ʵ�����
            const LPVOID& lpParam = NULL);              //��������

        //�򿪴���
        HWND WINAPI OpenWindow(const LPCSTR& lpWindowName);

        //��������
        int WINAPI MessageBoxExA(LPCSTR lpText,         //����
            LPCSTR lpCaption,                            //����
            UINT uType = MB_OK,                         //����
            WORD wLanguageId = 0);                      //���Ա�ʶ��
#endif // UNICODE

        //�رմ���
        BOOL WINAPI DestroyWindow();

        //��ʾ����
        BOOL WINAPI ShowWindow(int nCmdShow = SW_SHOWNORMAL);

        //��С������
        BOOL WINAPI CloseWindow();

        //���´���
        BOOL WINAPI UpdateWindow();

        //��ô��ھ��
        const HWND& WINAPI GetHWND() const;

        //Ĭ������
        static void defaults(WNDCLASSEX& WC);

        void defaults();

        Window();

        ~Window();

        //����
        WNDCLASSEX wc;

        //���ھ��
        HWND hwnd;
    };
}