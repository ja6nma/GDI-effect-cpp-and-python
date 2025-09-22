//by DeepSeek
#include <Windows.h>
#include <cmath>

const double PI = 3.14159265358979323846;
double angle = 0;
int colorCycle = 0;

double clamp(double value, double min_val, double max_val)
{
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

double my_abs(double x)
{
    return x < 0 ? -x : x;
}

COLORREF GetRainbowColor(double position)
{

    double normalized = position - floor(position);


    double r = clamp(1.5 - my_abs(normalized - 0.25) * 4.0, 0.0, 1.0);
    double g = clamp(1.5 - my_abs(normalized - 0.5) * 4.0, 0.0, 1.0);
    double b = clamp(1.5 - my_abs(normalized - 0.75) * 4.0, 0.0, 1.0);

    return RGB((int)(r * 255), (int)(g * 255), (int)(b * 255));
}

void CreateRainbowEffect(HDC hdc, int width, int height)
{

    for (int y = 0; y < height; y++)
    {
        double position = (double)y / height + (double)colorCycle / 360.0;
        COLORREF color = GetRainbowColor(position);

        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        MoveToEx(hdc, 0, y, NULL);
        LineTo(hdc, width, y);

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
}

void RotateDesktop()
{
    HDC hdc = GetDC(NULL);
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hBmp = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);


    CreateRainbowEffect(hMemDC, width, height);

    XFORM xform;
    double rad = angle * PI / 180.0;
    int centerX = width / 2;
    int centerY = height / 2;

    xform.eM11 = (FLOAT)cos(rad);
    xform.eM12 = (FLOAT)sin(rad);
    xform.eM21 = (FLOAT)-sin(rad);
    xform.eM22 = (FLOAT)cos(rad);
    xform.eDx = (FLOAT)(centerX - centerX * cos(rad) + centerY * sin(rad));
    xform.eDy = (FLOAT)(centerY - centerX * sin(rad) - centerY * cos(rad));

    SetGraphicsMode(hdc, GM_ADVANCED);
    SetWorldTransform(hdc, &xform);


    BitBlt(hdc, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);


    HBRUSH hBrush = CreateSolidBrush(GetRainbowColor((double)colorCycle / 360.0));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    int circleSize = (width < height ? width : height) / 4;
    Ellipse(hdc, centerX - circleSize, centerY - circleSize,
        centerX + circleSize, centerY + circleSize);

    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);

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
        angle += 2.0;
        if (angle >= 360) angle -= 360;

        colorCycle += 1;
        if (colorCycle >= 360) colorCycle = 0;

        RotateDesktop();
        Sleep(30);
    }

    return 0;
}