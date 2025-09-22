//Recode by DeepSeek
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


        int effectType = frame % 100 / 100; 

        switch (effectType) {
        case 0:

            for (int i = 0; i < 3; i++) {
                int offsetX = rand() % 41 - 20;
                int offsetY = rand() % 41 - 20;
                BitBlt(hdc, offsetX, offsetY, sw, sh, hdc, 0, 0, SRCCOPY);
            }
            break;

        case 1:

        {
            float angle = frame * 0.1f;
            int offsetX = static_cast<int>(cos(angle) * 30);
            int offsetY = static_cast<int>(sin(angle) * 30);
            BitBlt(hdc, offsetX, offsetY, sw, sh, hdc, 0, 0, SRCPAINT);
        }
        break;

        case 2:

        {
            float scale = 0.5f + 0.3f * sin(frame * 0.05f);
            int newWidth = static_cast<int>(sw * scale);
            int newHeight = static_cast<int>(sh * scale);
            int offsetX = (sw - newWidth) / 2;
            int offsetY = (sh - newHeight) / 2;

            StretchBlt(hdc, offsetX, offsetY, newWidth, newHeight,
                hdc, 0, 0, sw, sh, SRCINVERT);
        }
        break;

        case 3:

        {
            int blockSize = 20 + rand() % 50;
            for (int y = 0; y < sh; y += blockSize) {
                for (int x = 0; x < sw; x += blockSize) {
                    int srcX = rand() % sw;
                    int srcY = rand() % sh;
                    BitBlt(hdc, x, y, blockSize, blockSize,
                        hdc, srcX, srcY, SRCCOPY);
                }
            }
        }
        break;
        }


        if (frame % 10 == 0) {
            BitBlt(hdc, rand() % 21 - 10, rand() % 21 - 10,
                sw, sh, hdc, 0, 0, PATINVERT);
        }

        SelectObject(hdc, oldBrush);
        DeleteObject(brush);
        ReleaseDC(nullptr, hdc);

        color += 0.005f; 
        if (color > 1.0f) color -= 1.0f;

        frame++;
        Sleep(30); 
    }

    return 0;
}