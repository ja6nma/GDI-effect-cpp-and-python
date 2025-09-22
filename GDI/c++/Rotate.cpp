#include <Windows.h>
#include <cmath>

const double PI = 3.14159265358979323846; //404
double angle = 0; //100

void RotateDesktop()
{
    HDC hdc = GetDC(NULL);
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hBmp = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);


    BitBlt(hMemDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
    //BitBlt(hMemDC, 500, 0, width, height, hMemDC, 0, 0, SRCCOPY)
    //BitBlt(hMemDC, 0, 500, width, height, hMemDC, 0, 0, SRCCOPY)
    //BitBlt(hMemDC, 0, 0, width, height, hdc, 0, 500, SRCCOPY)
	//BitBlt(hMemDC, 0, 0, width, height, hdc, 500, 0, SRCCOPY)
	//NOTSRCERASE

    XFORM xform;
    double rad = angle * PI / 180.0;
    int centerX = width / 2; //10
    int centerY = height / 2; //10

    xform.eM11 = (FLOAT)cos(rad);
    xform.eM12 = (FLOAT)sin(rad);
    xform.eM21 = (FLOAT)-sin(rad);
    xform.eM22 = (FLOAT)cos(rad);
    xform.eDx = (FLOAT)(centerX - centerX * cos(rad) + centerY * sin(rad));
    xform.eDy = (FLOAT)(centerY - centerX * sin(rad) - centerY * cos(rad));


    SetGraphicsMode(hdc, GM_ADVANCED);
    SetWorldTransform(hdc, &xform);


    BitBlt(hdc, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);
    //BitBlt(hdc, 500, 0, width, height, hMemDC, 0, 0, SRCCOPY)
    //BitBlt(hdc, 0, 500, width, height, hMemDC, 0, 0, SRCCOPY)
    //BitBlt(hdc, 0, 0, width, height, hdc, 0, 500, SRCCOPY)
	//BitBlt(hdc, 0, 0, width, height, hdc, 500, 0, SRCCOPY)
    //NOTSRCERASE

    ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
    SetGraphicsMode(hdc, GM_COMPATIBLE);


    SelectObject(hMemDC, hOldBmp);
    DeleteObject(hBmp);
    DeleteDC(hMemDC);
    ReleaseDC(NULL, hdc);
}

int main()
{

    while (true)
    {
        angle += 1.0; //0.0; 5.0
        if (angle >= 360) angle -= 360;

        RotateDesktop();
        Sleep(10); //1
    }

    return 0;
}