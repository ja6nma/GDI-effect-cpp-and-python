//by DeepSeek
#include <windows.h>
#include <cmath>
#include <iostream>

const double PI = 3.14159265358979323846;


COLORREF GetRainbowColor(double angle) {

    double normalized = angle;
    while (normalized < 0) normalized += 2 * PI;
    while (normalized > 2 * PI) normalized -= 2 * PI;


    double t = normalized / (2 * PI);


    int r = (int)(255 * (0.5 + 0.5 * cos(2 * PI * t)));
    int g = (int)(255 * (0.5 + 0.5 * cos(2 * PI * t - 2 * PI / 3)));
    int b = (int)(255 * (0.5 + 0.5 * cos(2 * PI * t - 4 * PI / 3)));


    if (r < 0) r = 0;
    if (r > 255) r = 255;
    if (g < 0) g = 0;
    if (g > 255) g = 255;
    if (b < 0) b = 0;
    if (b > 255) b = 255;

    return RGB(r, g, b);
}

int main() {

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);


    HDC hdc = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdc);

    if (hdc == NULL || hdcMem == NULL) {
        std::cerr << "Не удалось получить контекст устройства" << std::endl;
        return 1;
    }


    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, sw, sh);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    int dx = 1;
    int dy = 1;
    double angle = 0.0;
    double colorAngle = 0.0;
    double size = 1.0;
    double speed = 5.0;
    double colorSpeed = 2.0;


    HBRUSH hBrush = NULL;


    while (true) {

        BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);


        BitBlt(hdcMem, 0, 0, sw, sh, hdcMem, dx, dy, SRCCOPY);


        COLORREF rainbowColor = GetRainbowColor(colorAngle);


        if (hBrush) DeleteObject(hBrush);
        hBrush = CreateSolidBrush(rainbowColor);


        SetBkMode(hdcMem, TRANSPARENT);


        RECT rect = { 0, 0, sw, sh };
        FillRect(hdcMem, &rect, hBrush);


        BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);


        dx = (int)(ceil(sin(angle) * size * 10));
        dy = (int)(ceil(cos(angle) * size * 10));


        angle += speed / 10;
        colorAngle += colorSpeed / 30;


        if (angle > PI) {
            angle = -PI;
        }


        size = 0.8 + 0.2 * sin(angle * 2);


        Sleep(30);
    }


    if (hBrush) DeleteObject(hBrush);
    SelectObject(hdcMem, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdc);

    return 0;
}