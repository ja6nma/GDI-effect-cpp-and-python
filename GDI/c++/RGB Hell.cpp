#include <windows.h>
#include <cmath>
#include <cstdlib>
#include <ctime>


void hsvToRgb(float h, float s, float v, float& r, float& g, float& b) {
    int i = static_cast<int>(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (i % 6) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    float color = 0.0f;

    while (true) {
        HDC hdc = GetDC(nullptr);

        float r, g, b;
        hsvToRgb(color, 1.0f, 1.0f, r, g, b);

        HBRUSH brush = CreateSolidBrush(RGB(
            static_cast<int>(r * 255),
            static_cast<int>(g * 255),
            static_cast<int>(b * 255)
        ));

        HGDIOBJ oldBrush = SelectObject(hdc, brush);


        int offsetX1 = rand() % 31 - 15;
        int offsetY1 = rand() % 31 - 15;
        int offsetX2 = rand() % 31 - 15;
        int offsetY2 = rand() % 31 - 15;


        BitBlt(hdc, offsetX1, offsetY1, sw, sh, hdc, 0, 0, SRCPAINT);
        //BitBlt(hdc, offsetX1, offsetY1, sw, sh, hdc, 100, 0, SRCPAINT)
        //BitBlt(hdc, offsetX1, offsetY1, sw, sh, hdc, 0, 100, SRCPAINT)



        BitBlt(hdc, offsetX2, offsetY2, sw, sh, hdc, 0, 0, PATINVERT);


        BitBlt(hdc, -offsetX1, -offsetY1, sw, sh, hdc, 0, 0, SRCAND);

        SelectObject(hdc, oldBrush);
        DeleteObject(brush);
        ReleaseDC(nullptr, hdc);

        color += 0.03f;
        if (color > 1.0f) {
            color -= 1.0f;
        }

        Sleep(20);
    }

    return 0;
}