#include <windows.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <algorithm>

#pragma comment(lib, "Msimg32.lib")

struct Particle {
    float x, y;
    float vx, vy;
    float life;
    float maxLife;
    float size;
    COLORREF color;
};

struct Vortex {
    float x, y;
    float power;
    float radius;
};

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

    HDC bufferDC = CreateCompatibleDC(hdc);
    HBITMAP bufferBitmap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(bufferDC, bufferBitmap);

    srand(static_cast<unsigned>(time(nullptr)));
    float timeCounter = 0.0f;

    std::vector<Particle> particles;
    std::vector<Vortex> vortices;


    for (int i = 0; i < 5; i++) {
        Vortex v;
        v.x = rand() % width;
        v.y = rand() % height;
        v.power = (rand() % 100) / 50.0f + 0.5f;
        v.radius = rand() % 200 + 100;
        vortices.push_back(v);
    }

    while (true) {
        timeCounter += 0.016f;


        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 20, 0 };
        AlphaBlend(bufferDC, 0, 0, width, height, bufferDC, 0, 0, width, height, blend);


        for (int i = 0; i < 15; i++) {
            Particle p;
            p.x = width / 2 + (sin(timeCounter * 2 + i) * 100);
            p.y = height / 2 + (cos(timeCounter * 2 + i) * 100);
            p.vx = (rand() % 100 - 50) / 20.0f;
            p.vy = (rand() % 100 - 50) / 20.0f;
            p.life = 1.0f;
            p.maxLife = (rand() % 100) / 50.0f + 1.0f;
            p.size = (rand() % 20) + 5;
            p.color = HSLtoRGB(fmod(timeCounter * 50 + i * 30, 360), 1.0f, 0.7f);
            particles.push_back(p);
        }


        for (auto it = particles.begin(); it != particles.end();) {
            Particle& p = *it;


            for (auto& vortex : vortices) {
                float dx = p.x - vortex.x;
                float dy = p.y - vortex.y;
                float dist = sqrt(dx * dx + dy * dy);

                if (dist < vortex.radius) {
                    float force = vortex.power * (1.0f - dist / vortex.radius);
                    p.vx += (-dy / dist) * force * 0.1f;
                    p.vy += (dx / dist) * force * 0.1f;
                }
            }

            p.x += p.vx;
            p.y += p.vy;
            p.life -= 0.01f;
            p.vx *= 0.98f;
            p.vy *= 0.98f;


            float alpha = p.life / p.maxLife;
            int currentSize = int(p.size * alpha);

            for (int r = currentSize; r > 0; r -= 2) {
                float progress = (float)r / currentSize;
                COLORREF color = HSLtoRGB(
                    fmod(timeCounter * 30 + r * 10, 360),
                    1.0f,
                    0.5f * progress * alpha
                );
                HBRUSH brush = CreateSolidBrush(color);
                SelectObject(bufferDC, brush);
                Ellipse(bufferDC, p.x - r, p.y - r, p.x + r, p.y + r);
                DeleteObject(brush);
            }

            if (p.life <= 0 || p.x < -100 || p.x > width + 100 || p.y < -100 || p.y > height + 100) {
                it = particles.erase(it);
            }
            else {
                ++it;
            }
        }


        for (auto& vortex : vortices) {
            vortex.x += sin(timeCounter + vortex.x * 0.01f) * 2;
            vortex.y += cos(timeCounter + vortex.y * 0.01f) * 2;


            if (vortex.x < 0) vortex.x = width;
            if (vortex.x > width) vortex.x = 0;
            if (vortex.y < 0) vortex.y = height;
            if (vortex.y > height) vortex.y = 0;


            HPEN pen = CreatePen(PS_SOLID, 2, HSLtoRGB(fmod(timeCounter * 40, 360), 0.7f, 0.6f));
            SelectObject(bufferDC, pen);
            HBRUSH nullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
            SelectObject(bufferDC, nullBrush);
            Ellipse(bufferDC, vortex.x - vortex.radius, vortex.y - vortex.radius,
                vortex.x + vortex.radius, vortex.y + vortex.radius);
            DeleteObject(pen);
        }


        for (int wave = 0; wave < 3; wave++) {
            float waveRadius = fmod(timeCounter * 50 + wave * 120, 800);
            HPEN pen = CreatePen(PS_SOLID, 3, HSLtoRGB(fmod(timeCounter * 60 + wave * 80, 360), 0.9f, 0.5f));
            SelectObject(bufferDC, pen);
            HBRUSH nullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
            SelectObject(bufferDC, nullBrush);

            for (int i = 0; i < 5; i++) {
                float offset = i * 20;
                Ellipse(bufferDC,
                    width / 2 - waveRadius - offset, height / 2 - waveRadius - offset,
                    width / 2 + waveRadius + offset, height / 2 + waveRadius + offset);
            }
            DeleteObject(pen);
        }


        if (rand() % 100 < 5) {
            int flashX = rand() % width;
            int flashY = rand() % height;
            int flashSize = rand() % 100 + 50;

            for (int r = flashSize; r > 0; r -= 5) {
                COLORREF color = HSLtoRGB(rand() % 360, 1.0f, 0.7f);
                HBRUSH brush = CreateSolidBrush(color);
                SelectObject(bufferDC, brush);
                Ellipse(bufferDC, flashX - r, flashY - r, flashX + r, flashY + r);
                DeleteObject(brush);
            }
        }


        BitBlt(hdc, 0, 0, width, height, bufferDC, 0, 0, SRCINVERT);


        for (int i = 0; i < 2; i++) {
            int offsetX = (int)(sin(timeCounter * 1.3f + i) * 15);
            int offsetY = (int)(cos(timeCounter * 1.7f + i) * 15);
            BitBlt(hdc, offsetX, offsetY, width, height, hdc, 0, 0, NOTSRCCOPY);
        }

        Sleep(16);
    }

    DeleteObject(bufferBitmap);
    DeleteDC(bufferDC);
    ReleaseDC(0, hdc);
    return 0;
}