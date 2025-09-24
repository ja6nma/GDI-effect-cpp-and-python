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
    float angle = 0.0f;
    int frame = 0;

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

        float spiralRadius = 50.0f + sin(frame * 0.1f) * 30.0f;
        int offsetX1 = static_cast<int>(cos(angle) * spiralRadius);
        int offsetY1 = static_cast<int>(sin(angle) * spiralRadius);
        
        int offsetX2 = static_cast<int>(cos(angle + 1.57f) * spiralRadius * 0.7f);
        int offsetY2 = static_cast<int>(sin(angle + 1.57f) * spiralRadius * 0.7f);


        for (int i = 0; i < 3; i++) {
            int mult = i + 1;
            BitBlt(hdc, offsetX1 * mult, offsetY1 * mult, sw, sh, hdc, 0, 0, SRCPAINT);
            BitBlt(hdc, offsetX2 * mult, offsetY2 * mult, sw, sh, hdc, 0, 0, PATINVERT);
        }


        if (frame % 10 == 0) {
            BitBlt(hdc, -offsetX1, -offsetY1, sw, sh, hdc, 0, 0, SRCAND);
            BitBlt(hdc, rand() % 21 - 10, rand() % 21 - 10, sw, sh, hdc, 0, 0, SRCCOPY);
        }

        SelectObject(hdc, oldBrush);
        DeleteObject(brush);
        ReleaseDC(nullptr, hdc);

        color += 0.03f;
        angle += 0.05f;
        frame++;
        
        if (color > 1.0f) color -= 1.0f;
        if (angle > 6.28f) angle -= 6.28f;

        Sleep(30);
    }

    return 0;
}