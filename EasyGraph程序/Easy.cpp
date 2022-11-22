#include <iostream>
#include "EasyGraph.h"

#define discard(a)  ((void)(a))

using namespace std;
using namespace EasyGraph;

int main()
{
    srand((int)time(0));
    discard(rand());

    cout << sizeof(Window) << " + " << sizeof(Paint) << " = " << sizeof(Window) + sizeof(Paint) << endl;
    Window win;

    win.OpenWindow(L"TEXT WINDOW 1");

    Paint pt(win.hwnd);

    pt.SetPenColor(RGB(rand() % 256, rand() % 256, rand() % 256));
    pt.SetBrushColor(RGB(rand() % 256, rand() % 256, rand() % 256));

    HBITMAP bp1 = NULL;

    MSG msg;
    while (1)
    {
        if (GetMessageW(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        switch (msg.message) {
        case WM_RBUTTONUP: {
            pt.GetImage(&bp1);
            break;
        }
        case WM_LBUTTONUP: {
            pt.Line(rand() % 500, rand() % 500, rand() % 500, rand() % 500);
            pt.Circle(rand() % 500, rand() % 500, rand() % 50);
            break;
        }
        case WM_MBUTTONUP: {
            pt.Image(bp1, 100, 100);
            break;
        }
        }
        /*
        EasyMessage emsg;
        emsg.lParam = msg.lParam;
        emsg.wParam = msg.wParam;
        emsg.message = msg.message;
        if (emsg.message == WM_MOUSEWHEEL)
            cout << GET_WHEEL_DELTA_WPARAM(msg.wParam) << " " << emsg.wheel << endl;
        //*/

    }
    return 0;
}