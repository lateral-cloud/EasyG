#include <iostream>
#include "EasyG.h"

using namespace std;
using namespace EasyG;

//画两个相交的透明矩形，且适应屏幕拉伸
int test1()
{
    EasyWindow win;
    win.Create(EasyWindowSets(L"", SIZE{ 512,512 }, nullptr, nullptr, EWF_HIDECONSOLE, 0));
    //创建一个窗口，默认标题，大小为 512*512，无附加窗口过程函数，无父窗口，带隐藏控制台标签，无额外样式

    constexpr auto CANVASSIZE = 256;
    EasyCanvas canvas2(CANVASSIZE, CANVASSIZE);
    //创建画布，大小为 CANVASSIZE*CANVASSIZE

    win.SetCanvas(&canvas2);
    //将窗口的画布设为 canvas2

    win.GetInfo().bStretch = 1;
    //将使用窗口拉伸设置为开启

    EasyMessage msg;
    while (1)
    {
        if (!win.isAlive())
            break;
        //若窗口销毁就退出，在：使用函数 win.Destroy() 或 点击窗口的关闭按钮 时，结果为 false

        win.GetMessage(&msg);
        //将消息存入 msg，在没有新消息时等待

        EasyMessage Emsg(msg);
        switch (msg.message) {
        case WM_LBUTTONUP: {
            EasyCanvas canvas3(1024, 1024);
            EasyImage image1(1024, 1024), image2(1024, 1024);
            //创建画布与图片

            canvas3.SetPenColor(REDCOLOR);
            canvas3.SetBrushColor(BLUECOLOR);
            canvas3.FillRect(100, 50, 150, 250);
            //设置边框和填充颜色，并画矩形

            canvas3.BitBlt(image1.ImageDC_Src);
            //将画画在 image1 的 HDC 中，也可以画在其他 HDC 中

            canvas3.Clear();
            //清除 canvas3 的画

            canvas3.SetPenColor(YELLOWCOLOR);
            canvas3.SetBrushColor(GREENCOLOR);
            canvas3.FillRect(50, 100, 250, 150);
            canvas3.BitBlt(image2.ImageDC_Src);
            canvas3.Clear();

            canvas2.Clear();
            image1.AlphaBlend(canvas3.ImageDC_Src);
            //将 image1 的图形画在 canvas3 的 HDC 上，附加透明度 255（等于覆盖）

            image2.AlphaBlend(canvas3.ImageDC_Src, NULLPTSZ, NULLPTSZ, 127);
            //将 image2 的图形画在 canvas3 的 HDC 上，附加透明度 177，使用 image2 的长宽，即不拉伸

            canvas3.Flush();1
            //将canvas3的图形从源 HDC 画到 输出 HDC 中
            //由于直接操控 canvas3 的 HDC ，应该使用 Update() 更新状态，因为这里一定要画图，所以使用 Flush()

            canvas3.BitBlt(canvas2.ImageDC_Src);
            //将canvas3的图形画到 canvas2 的 HDC 中

            canvas2.Flush();

            break;

            /*
            //也可以直接画在 canvas2 上
            //canvas3.Clear();                                                    //没必要
            canvas3.SetPenColor(REDCOLOR);
            canvas3.SetBrushColor(BLUECOLOR);
            canvas3.FillRect(100, 50, 150, 250);
            canvas3.Flush();
            canvas3.AlphaBlend(canvas2.ImageDC_Src);
            canvas3.Clear();
            canvas3.SetPenColor(YELLOWCOLOR);
            canvas3.SetBrushColor(GREENCOLOR);
            canvas3.FillRect(50, 100, 250, 150);
            canvas3.Flush();
            canvas3.AlphaBlend(canvas2.ImageDC_Src, NULLPTSZ, NULLPTSZ, 127);
            canvas3.Clear();
            canvas2.Flush();
            */
        }
        }
        win.Redraw();
    }
    cout << "tests end\n" << endl;
    getchar();
    return 0;
}