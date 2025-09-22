//recode by DeepSeek
#include <Windows.h>
#include <random>
#include <cmath>


COLORREF HSVtoRGB(float H, float S, float V) {
    float R, G, B;

    int i = static_cast<int>(H * 6);
    float f = H * 6 - i;
    float p = V * (1 - S);
    float q = V * (1 - f * S);
    float t = V * (1 - (1 - f) * S);

    switch (i % 6) {
    case 0: R = V; G = t; B = p; break;
    case 1: R = q; G = V; B = p; break;
    case 2: R = p; G = V; B = t; break;
    case 3: R = p; G = q; B = V; break;
    case 4: R = t; G = p; B = V; break;
    case 5: R = V; G = p; B = q; break;
    }

    return RGB(static_cast<int>(R * 255), static_cast<int>(G * 255), static_cast<int>(B * 255));
    //  rand()
}

int main() {

    SetProcessDPIAware();


    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    float color = 0.0f;
    HDC hdc = GetDC(NULL);


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(0, w);
    std::uniform_int_distribution<> distY(0, h);


    static int frameCount = 0;
    const int maxFrames = 500;

    while (true) {
        frameCount++;
        if (frameCount > maxFrames) {
            frameCount = 0;

            RECT rect = { 0, 0, w, h };
            FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
        }


        float speed = 0.02f * (1.0f + 0.5f * sin(frameCount * 0.01f));
        color = fmod(color + speed, 1.0f);


        COLORREF rgbColor = HSVtoRGB(
            fmod(color + (frameCount % 100) * 0.001f, 1.0f),
            0.8f + 0.2f * sin(frameCount * 0.02f),
            0.9f + 0.1f * cos(frameCount * 0.015f)
        );


        POINT points[4];

        if (frameCount % 100 < 50) {

            for (int i = 0; i < 4; i++) {
                points[i].x = distX(gen);
                points[i].y = distY(gen);
            }
        }
        else {

            int centerX = w / 2 + (distX(gen) % 200 - 100);
            int centerY = h / 2 + (distY(gen) % 200 - 100);
            int radius = 100 + distX(gen) % 300;

            points[0].x = centerX - radius;
            points[0].y = centerY;
            points[1].x = centerX - radius / 2;
            points[1].y = centerY - radius;
            points[2].x = centerX + radius / 2;
            points[2].y = centerY + radius;
            points[3].x = centerX + radius;
            points[3].y = centerY;
        }


        int penWidth = 3 + (frameCount % 6);
        HPEN hPen = CreatePen(PS_SOLID, penWidth, rgbColor);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);


        if (frameCount % 150 < 100) {
            PolyBezier(hdc, points, 4);
        }
        else {

            MoveToEx(hdc, points[0].x, points[0].y, NULL);
            for (int i = 1; i < 4; i++) {
                LineTo(hdc, points[i].x, points[i].y);
            }
        }


        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);


        int delay = 25 + (int)(15 * sin(frameCount * 0.005f));
        Sleep(delay);
    }

    ReleaseDC(NULL, hdc);
    return 0;
}