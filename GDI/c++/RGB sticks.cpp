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
    std::uniform_int_distribution<> distWidth(1, 8);
    std::uniform_int_distribution<> distPoints(3, 6);

    while (true) {

        color = fmod(color + 0.015f + (sin(GetTickCount() * 0.001f) * 0.005f), 1.0f);
        COLORREF rgbColor = HSVtoRGB(color, 0.9f, 0.95f);


        int numPoints = distPoints(gen);
        POINT* points = new POINT[numPoints];


        static POINT lastPoints[6] = { 0 };
        for (int i = 0; i < numPoints; i++) {
            if (i < 4 && lastPoints[i].x != 0 && lastPoints[i].y != 0) {

                points[i].x = lastPoints[i].x + distX(gen) % 100 - 50;
                points[i].y = lastPoints[i].y + distY(gen) % 100 - 50;
            }
            else {
                points[i].x = distX(gen);
                points[i].y = distY(gen);
            }
            lastPoints[i] = points[i];
        }


        int penWidth = distWidth(gen);
        HPEN hPen = CreatePen(PS_SOLID, penWidth, rgbColor);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);


        PolyBezier(hdc, points, numPoints);


        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
        delete[] points;


        Sleep(20 + (rand() % 30));
		//Sleep(20);
    }

    ReleaseDC(NULL, hdc);
    return 0;
}