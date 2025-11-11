#include <windows.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <chrono>
#include <thread>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int sw, sh;
HDC hdc, desk, desc;
int xx, yy;
int dx = 1, dy = 1;
double angle = 0.0;
int size = 1;
int speed = 40;

int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    sw = GetSystemMetrics(SM_CXSCREEN);
    sh = GetSystemMetrics(SM_CYSCREEN);
    xx = sw;
    yy = sh;

    HWND desktop = GetDesktopWindow();  
    hdc = GetDC(desktop);              
    desk = GetDC(0);
    desc = GetDC(0);

    SetProcessDPIAware();

    std::wstring text = L"#DeadWave";

    std::this_thread::sleep_for(std::chrono::seconds(1));

    while (true) {
        hdc = GetDC(0);

        COLORREF color = RGB(randomInt(0, 122), randomInt(0, 430), randomInt(0, 310));
        HBRUSH brush = CreateSolidBrush(color);
        HGDIOBJ oldBrush = SelectObject(hdc, brush);

        BitBlt(hdc, randomInt(-10, 10), randomInt(-10, 10), sw, sh, hdc, 0, 0, SRCCOPY);
        BitBlt(hdc, randomInt(-10, 10), randomInt(-10, 10), sw, sh, hdc, 0, 0, PATINVERT);

        static int x = 0, y = 0;
        DrawIcon(hdc, x, y, LoadIcon(NULL, IDI_ERROR));
        x += 30;
        if (x >= sw) {
            y += 30;
            x = 0;
        }
        if (y >= sh) {
            x = y = 0;
        }

        HBRUSH brush2 = CreateSolidBrush(RGB(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
        HGDIOBJ oldBrush2 = SelectObject(desc, brush2);

        PatBlt(desc, randomInt(0, xx), randomInt(0, yy), randomInt(0, xx), randomInt(0, yy), PATINVERT);

        SelectObject(desc, oldBrush2);
        DeleteObject(brush2);
        SelectObject(hdc, oldBrush);
        DeleteObject(brush);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        size += 1;
        BitBlt(hdc, 0, 0, sw, sh, hdc, dx, dy, SRCCOPY);

        dx = static_cast<int>(ceil(sin(angle) * size * 10));
        dy = static_cast<int>(ceil(cos(angle) * size * 10));
        angle += speed / 10.0;

        RECT rect1 = { randomInt(0, xx), randomInt(0, yy), randomInt(0, xx), randomInt(0, yy) };
        RECT rect2 = { randomInt(0, xx), randomInt(0, yy), randomInt(0, xx), randomInt(0, yy) };

        DrawTextW(desk, text.c_str(), -1, &rect1, DT_LEFT);
        DrawTextW(desk, text.c_str(), -1, &rect2, DT_LEFT);

        if (angle > M_PI) {
            angle = -M_PI;
        }

        ReleaseDC(0, hdc);
    }

    ReleaseDC(desktop, hdc);
    ReleaseDC(0, desk);
    ReleaseDC(0, desc);

    return 0;
}

