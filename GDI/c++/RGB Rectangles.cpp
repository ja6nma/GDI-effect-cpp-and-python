#include <Windows.h>
#include <random>

#pragma comment(lib, "Msimg32.lib")

void DrawGradientRectangles(HDC hdc)
{
    RECT rect;
    GetClientRect(GetDesktopWindow(), &rect);
    int width = rect.right;
    int height = rect.bottom;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> xDist(0, width);
    std::uniform_int_distribution<int> yDist(0, height);
    std::uniform_int_distribution<int> colorDist(0, 0xFF00);

    for (int i = 0; i < 25; ++i)
    {
        int x1 = xDist(gen);
        int y1 = yDist(gen);
        int x2 = xDist(gen);
        int y2 = yDist(gen);

        TRIVERTEX vertices[2] = {
            {
                min(x1, x2),
                min(y1, y2),
                static_cast<COLOR16>(colorDist(gen)),
                static_cast<COLOR16>(colorDist(gen)),
                static_cast<COLOR16>(colorDist(gen)),
                0
            },
            {
                max(x1, x2),
                max(y1, y2),
                static_cast<COLOR16>(colorDist(gen)),
                static_cast<COLOR16>(colorDist(gen)),
                static_cast<COLOR16>(colorDist(gen)),
                0
            }
        };

        GRADIENT_RECT mesh = { 0, 1 };
        GradientFill(hdc, vertices, 2, &mesh, 1, GRADIENT_FILL_RECT_H);
    }
}

int main()
{
    SetProcessDPIAware();
    HDC hdcScreen = GetDC(nullptr);

    while (true)
    {
        DrawGradientRectangles(hdcScreen);
    }

    ReleaseDC(nullptr, hdcScreen);
    return 0;
}