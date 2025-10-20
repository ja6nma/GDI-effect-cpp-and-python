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
        timeCounter += 0.02f;
        hue = fmod(hue + 1.2f, 360.0f);

        for (int i = 0; i < 3; i++) {
            int x = width / 2 + (int)(sin(timeCounter * 0.8f + i * 2.1f) * 400);
            int y = height / 2 + (int)(cos(timeCounter * 0.7f + i * 1.8f) * 300);
            int radius = rand() % 100 + 80;

            for (int r = radius; r > 0; r -= 8) {
                float progress = (float)r / radius;
                float wave = sin(timeCounter * 1.5f + r * 0.08f) * 45.0f;
                COLORREF color = HSLtoRGB(hue + wave + i * 40, 1.0f, 0.3f + 0.4f * progress);
                HBRUSH brush = CreateSolidBrush(color);
                SelectObject(hdc, brush);
                Ellipse(hdc, x - r, y - r, x + r, y + r);
                DeleteObject(brush);
            }
        }

        for (int i = 0; i < 4; i++) {
            float amplitude = 25.0f + sin(timeCounter * 0.3f + i) * 35.0f;
            int offsetX = (int)(sin(timeCounter * 1.2f + i * 0.9f) * amplitude);
            int offsetY = (int)(cos(timeCounter * 1.1f + i * 1.4f) * amplitude);
            BitBlt(hdc, offsetX, offsetY, width, height, hdc, 0, 0, NOTSRCERASE);
        }

        for (int i = 0; i < 12; i++) {
            float angle = timeCounter * 1.8f + i * 0.7f;
            int x1 = width / 2 + (int)(cos(angle) * 250);
            int y1 = height / 2 + (int)(sin(angle) * 250);
            int x2 = width / 2 + (int)(cos(angle + 2.8f) * 400);
            int y2 = height / 2 + (int)(sin(angle + 2.8f) * 400);

            HPEN pen = CreatePen(PS_SOLID, 3, HSLtoRGB(hue + i * 30, 0.9f, 0.5f));
            SelectObject(hdc, pen);
            MoveToEx(hdc, x1, y1, NULL);
            LineTo(hdc, x2, y2);
            DeleteObject(pen);
        }

        for (int i = 0; i < 25; i++) {
            int x = rand() % width;
            int y = rand() % height;
            COLORREF dotColor = HSLtoRGB(hue + rand() % 90, 1.0f, 0.6f + sin(timeCounter + i) * 0.3f);
            SetPixel(hdc, x, y, dotColor);
        }

        for (int i = 0; i < 5; i++) {
            int rectX = rand() % width;
            int rectY = rand() % height;
            int rectWidth = rand() % 50 + 20;
            int rectHeight = rand() % 50 + 20;
            COLORREF rectColor = HSLtoRGB(hue + i * 72, 0.7f, 0.4f);
            HBRUSH rectBrush = CreateSolidBrush(rectColor);
            SelectObject(hdc, rectBrush);
            Rectangle(hdc, rectX, rectY, rectX + rectWidth, rectY + rectHeight);
            DeleteObject(rectBrush);
        }

        Sleep(40);
    }

    ReleaseDC(0, hdc);
    return 0;
}