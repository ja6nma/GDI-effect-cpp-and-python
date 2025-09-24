#include <windows.h>
#include <ctime>
#include <cmath>
#include <vector>

#pragma comment(lib, "Msimg32.lib")

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

struct Particle {
    float x, y;
    float vx, vy;
    float life;
    float maxLife;
};

int main() {
    HDC hdc = GetDC(0);
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hdcMem, hBitmap);

    srand(static_cast<unsigned>(time(nullptr)));
    float hue = 0.0f;
    float time = 0.0f;

    std::vector<Particle> particles;

    while (true) {
        time += 0.01f;
        hue = fmod(hue + 0.7f, 360.0f);


        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 200, 0 }; 
        AlphaBlend(hdcMem, 0, 0, width, height, hdcMem, 0, 0, width, height, blend);


        for (int i = 0; i < 2; i++) {
            float angle = time + i * 3.14159f;
            int x = width / 2 + (int)(cos(angle) * 300 * sin(time * 0.5f));
            int y = height / 2 + (int)(sin(angle) * 300 * cos(time * 0.3f));

            for (int r = 150; r > 0; r -= 15) {
                float progress = (float)r / 150;
                COLORREF color = HSLtoRGB(
                    fmod(hue + r * 2.0f + i * 120, 360),
                    progress,
                    0.3f + progress * 0.4f
                );
                HBRUSH brush = CreateSolidBrush(color);
                SelectObject(hdcMem, brush);
                int ripple = (int)(sin(time * 2 + r * 0.1f) * 10);
                Ellipse(hdcMem, x - r - ripple, y - r - ripple, x + r + ripple, y + r + ripple);
                DeleteObject(brush);
            }
        }


        if (rand() % 3 == 0) {
            Particle p;
            p.x = rand() % width;
            p.y = rand() % height;
            p.vx = (rand() % 100 - 50) * 0.1f;
            p.vy = (rand() % 100 - 50) * 0.1f;
            p.life = 0;
            p.maxLife = 50 + rand() % 100;
            particles.push_back(p);
        }


        for (auto it = particles.begin(); it != particles.end(); ) {
            it->x += it->vx;
            it->y += it->vy;
            it->vx *= 0.98f;
            it->vy *= 0.98f;
            it->life += 1.0f;

            float lifeProgress = it->life / it->maxLife;
            int size = (int)((1.0f - lifeProgress) * 20);

            if (size > 0) {
                COLORREF color = HSLtoRGB(
                    fmod(hue + it->life * 2, 360),
                    1.0f - lifeProgress,
                    0.5f + 0.5f * sin(lifeProgress * 3.14159f)
                );
                HBRUSH brush = CreateSolidBrush(color);
                SelectObject(hdcMem, brush);
                Ellipse(hdcMem,
                    (int)it->x - size, (int)it->y - size,
                    (int)it->x + size, (int)it->y + size);
                DeleteObject(brush);
            }

            if (it->life > it->maxLife) {
                it = particles.erase(it);
            }
            else {
                ++it;
            }
        }


        for (int layer = 0; layer < 4; layer++) {
            float layerPhase = time + layer * 1.57f;
            int offsetX = (int)(sin(layerPhase * 1.3f) * 40 * (1.0f + 0.5f * layer));
            int offsetY = (int)(cos(layerPhase * 0.8f) * 40 * (1.0f + 0.5f * layer));


            if (layer % 2 == 0) {
                BitBlt(hdcMem, offsetX, offsetY, width, height, hdcMem, 0, 0, SRCINVERT);
            }
            else {
                BitBlt(hdcMem, -offsetX / 2, -offsetY / 2, width, height, hdcMem, 0, 0, NOTSRCERASE);
            }
        }


        for (int i = 0; i < 8; i++) {
            float lineHue = fmod(hue + i * 45, 360);
            int x1 = width / 2 + (int)(sin(time * 0.5f + i) * 400);
            int y1 = height / 2 + (int)(cos(time * 0.3f + i) * 300);
            int x2 = width / 2 + (int)(cos(time * 0.7f + i) * 400);
            int y2 = height / 2 + (int)(sin(time * 0.4f + i) * 300);

            HPEN pen = CreatePen(PS_SOLID, 2, HSLtoRGB(lineHue, 1.0f, 0.6f));
            SelectObject(hdcMem, pen);
            MoveToEx(hdcMem, x1, y1, NULL);
            LineTo(hdcMem, x2, y2);


            HBRUSH dotBrush = CreateSolidBrush(HSLtoRGB(lineHue + 30, 1.0f, 0.8f));
            SelectObject(hdcMem, dotBrush);
            Ellipse(hdcMem, x1 - 3, y1 - 3, x1 + 3, y1 + 3);
            Ellipse(hdcMem, x2 - 3, y2 - 3, x2 + 3, y2 + 3);
            DeleteObject(dotBrush);
            DeleteObject(pen);
        }


        BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);


        if (rand() % 100 == 0) {

            for (int i = 0; i < 3; i++) {
                int flashX = rand() % width;
                int flashY = rand() % height;
                int flashSize = 50 + rand() % 100;
                HBRUSH flashBrush = CreateSolidBrush(HSLtoRGB(hue + i * 120, 1.0f, 0.9f));
                SelectObject(hdc, flashBrush);
                Ellipse(hdc, flashX - flashSize, flashY - flashSize,
                    flashX + flashSize, flashY + flashSize);
                DeleteObject(flashBrush);
            }
        }

        Sleep(30);
    }

    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(0, hdc);
    return 0;
}