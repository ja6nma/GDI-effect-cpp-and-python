#include <Windows.h>
#include <iostream>

int main()
{

    SetProcessDPIAware();

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);


    RECT screenRect;
    GetWindowRect(GetDesktopWindow(), &screenRect);

    LONG left = screenRect.left;
    LONG top = screenRect.top;
    LONG right = screenRect.right;
    LONG bottom = screenRect.bottom;


    POINT points[3] = {
        {left + 50, top - 50},
        {right + 50, top + 50},
        {left - 50, bottom - 50}
    };

    while (true)
    {

        HDC hdc = GetDC(NULL);
        HDC mhdc = CreateCompatibleDC(hdc);


        HBITMAP hbit = CreateCompatibleBitmap(hdc, sh, sw);
        HBITMAP holdbit = (HBITMAP)SelectObject(mhdc, hbit);


        PlgBlt(
            hdc,
            points,
            hdc,
            left - 20,
            top - 20,
            (right - left) + 40,
            (bottom - top) + 40,
            NULL,
            0,
            0
        );


        SelectObject(mhdc, holdbit);
        DeleteObject(hbit);
        DeleteDC(mhdc);
        ReleaseDC(NULL, hdc);


        Sleep(1); //10, 100
    }

    return 0;
}