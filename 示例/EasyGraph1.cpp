#include <iostream>
#include "EasyG.h"

using namespace std;
using namespace EasyG;

//�������ཻ��͸�����Σ�����Ӧ��Ļ����
int main()
{
    EasyWindow win;
    win.Create(EasyWindowSets(L"", SIZE{ 512,512 }, nullptr, nullptr, EWF_HIDECONSOLE, 0));
    //����һ�����ڣ�Ĭ�ϱ��⣬��СΪ 512*512���޸��Ӵ��ڹ��̺������޸����ڣ������ؿ���̨��ǩ���޶�����ʽ

    constexpr auto CANVASSIZE = 256;
    EasyCanvas canvas2(CANVASSIZE, CANVASSIZE);
    //������������СΪ CANVASSIZE*CANVASSIZE

    win.SetCanvas(&canvas2);
    //�����ڵĻ�����Ϊ canvas2

    win.GetInfo().bStretch = 1;
    //��ʹ�ô�����������Ϊ����

    EasyMessage msg;
    while (1)
    {
        if (!win.isAlive())
            break;
        //���������پ��˳����ڣ�ʹ�ú��� win.Destroy() �� ������ڵĹرհ�ť ʱ�����Ϊ false

        win.GetMessage(&msg);
        //����Ϣ���� msg����û������Ϣʱ�ȴ�

        EasyMessage Emsg(msg);
        switch (msg.message) {
        case WM_LBUTTONUP: {
            EasyCanvas canvas3(1024, 1024);
            EasyImage image1(1024, 1024), image2(1024, 1024);
            //����������ͼƬ

            canvas3.SetPenColor(REDCOLOR);
            canvas3.SetBrushColor(BLUECOLOR);
            canvas3.FillRect(100, 50, 150, 250);
            //���ñ߿�������ɫ����������

            canvas3.BitBlt(image1.ImageDC_Src);
            //�������� image1 �� HDC �У�Ҳ���Ի������� HDC ��

            canvas3.Clear();
            //��� canvas3 �Ļ�

            canvas3.SetPenColor(YELLOWCOLOR);
            canvas3.SetBrushColor(GREENCOLOR);
            canvas3.FillRect(50, 100, 250, 150);
            canvas3.BitBlt(image2.ImageDC_Src);
            canvas3.Clear();

            canvas2.Clear();
            image1.AlphaBlend(canvas3.ImageDC_Src);
            //�� image1 ��ͼ�λ��� canvas3 �� HDC �ϣ�����͸���� 255�����ڸ��ǣ�

            image2.AlphaBlend(canvas3.ImageDC_Src, NULLPTSZ, NULLPTSZ, 127);
            //�� image2 ��ͼ�λ��� canvas3 �� HDC �ϣ�����͸���� 177��ʹ�� image2 �ĳ�����������

            canvas3.Flush();
            //��canvas3��ͼ�δ�Դ HDC ���� ��� HDC ��
            //����ֱ�Ӳٿ� canvas3 �� HDC ��Ӧ��ʹ�� Update() ����״̬����Ϊ����һ��Ҫ��ͼ������ʹ�� Flush()

            canvas3.BitBlt(canvas2.ImageDC_Src);
            //��canvas3��ͼ�λ��� canvas2 �� HDC ��

            canvas2.Flush();

            break;

            /*
            //Ҳ����ֱ�ӻ��� canvas2 ��
            //canvas3.Clear();                                                    //û��Ҫ
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