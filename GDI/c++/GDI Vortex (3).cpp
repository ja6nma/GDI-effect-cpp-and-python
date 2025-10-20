#include <windows.h>
#include <ctime>
#include <cmath>

COLORREF HSLtoRGB(float H, float S, float L) {
    float C = (1 - fabs(2 * L - 1)) * S;
    float X = C * (1 - fabs(fmod(H / 60.0f, 2) - 1));
    float m = L - C / 2.0f;
    float r, g, b;

    if (H < 60) { r = C; g = X; b = 0; }
    else if (H < 120) { r = X; g = C; b = 0; }
    else if (H < 180) { r = 0; g = C; b = X; }
    else if (H < 240) { r = 0; g = X; b = C; }
    else if (H < 300) { r = X; g = 0; b = C; }
    else { r = C; g = 0; b = X; }

    return RGB((r + m) * 255, (g + m) * 255, (b + m) * 255);
}

int main() {
    HDC hdc = GetDC(0);
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    srand(static_cast<unsigned>(time(nullptr)));
    float hue = 0.0f;
    float timeCounter = 0.0f;

    while (true) {
        timeCounter += 0.01f;
        hue = fmod(hue + 0.7f, 360.0f);

        for (int i = 0; i < 2; i++) {
            int x = rand() % width;
            int y = rand() % height;
            int radius = rand() % 150 + 50;

            for (int r = radius; r > 0; r -= 5) {
                float progress = (float)r / radius;
                float wave = sin(timeCounter + r * 0.1f) * 30.0f;
                COLORREF color = HSLtoRGB(hue + wave, 1.0f, 0.4f + 0.3f * progress);
                HBRUSH brush = CreateSolidBrush(color);
                SelectObject(hdc, brush);
                Ellipse(hdc, x - r, y - r, x + r, y + r);
                DeleteObject(brush);
            }
        }

        for (int i = 0; i < 3; i++) {
            float amplitude = 30.0f + sin(timeCounter * 0.5f + i) * 20.0f;
            int offsetX = (int)(sin(timeCounter + i * 1.3f) * amplitude);
            int offsetY = (int)(cos(timeCounter + i * 1.7f) * amplitude);
            BitBlt(hdc, offsetX, offsetY, width, height, hdc, 0, 0, SRCINVERT);
        }

        for (int i = 0; i < 8; i++) {
            float angle = timeCounter * 2.0f + i * 0.5f;
            int x1 = width / 2 + (int)(cos(angle) * 200);
            int y1 = height / 2 + (int)(sin(angle) * 200);
            int x2 = width / 2 + (int)(cos(angle + 3.14f) * 300);
            int y2 = height / 2 + (int)(sin(angle + 3.14f) * 300);
            
            HPEN pen = CreatePen(PS_SOLID, 2, HSLtoRGB(hue + i * 45, 0.8f, 0.6f));
            SelectObject(hdc, pen);
            MoveToEx(hdc, x1, y1, NULL);
            LineTo(hdc, x2, y2);
            DeleteObject(pen);
        }

        for (int i = 0; i < 20; i++) {
            int x = rand() % width;
            int y = rand() % height;
            COLORREF dotColor = HSLtoRGB(hue + rand() % 60, 0.9f, 0.7f);
            SetPixel(hdc, x, y, dotColor);
        }

        Sleep(30);
    }

    ReleaseDC(0, hdc);
    return 0;
}