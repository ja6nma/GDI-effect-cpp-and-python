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

    while (true) {
        hue = fmod(hue + 0.01f, 360.0f);


        for (int i = 0; i < 3; i++) {
            int x = rand() % width;
            int y = rand() % height;
            int radius = rand() % 200 + 30;
	     // int radius = rand() % 350 + 5;

            for (int r = radius; r > 0; r -= 10) {
                float progress = (float)r / radius;
                COLORREF color = HSLtoRGB(hue + r * 0.5f, 1.0f, 0.5f * progress);
                HBRUSH brush = CreateSolidBrush(color);
                SelectObject(hdc, brush);
                Ellipse(hdc, x - r, y - r, x + r, y + r);
                DeleteObject(brush);
            }
        }


        for (int i = 0; i < 5; i++) {
            int offsetX = (int)(sin(GetTickCount() * 0.001f + i) * 50);
            int offsetY = (int)(cos(GetTickCount() * 0.001f + i) * 50);
            BitBlt(hdc, offsetX, offsetY, width, height, hdc, 0, 0, NOTSRCERASE); //SRCINVERT
        }


        for (int i = 0; i < 10; i++) {
            int x1 = rand() % width;
            int y1 = rand() % height;
            int x2 = rand() % width;
            int y2 = rand() % height;
            HPEN pen = CreatePen(PS_SOLID, 1, HSLtoRGB(hue + i * 30, 1.0f, 0.7f));
            SelectObject(hdc, pen);
            MoveToEx(hdc, x1, y1, NULL);
            LineTo(hdc, x2, y2);
            DeleteObject(pen);
        }

        Sleep(25);
    }

    ReleaseDC(0, hdc);
    return 0;
}