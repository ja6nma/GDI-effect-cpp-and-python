#include <windows.h>
#include <cmath>
#include <iostream>

const double PI = 3.14159265358979323846;

int main() {

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);


    HDC hdc = GetDC(NULL);

    int dx = 1;
    int dy = 1;
    double angle = 0.0;
    double size = 5.0;
    double speed = 5.0; //10


    while (true) {

        BitBlt(hdc, 0, 0, sw, sh, hdc, dx, dy, SRCCOPY);


        dx = static_cast<int>(std::ceil(std::sin(angle) * size * 10));
        dy = static_cast<int>(std::ceil(std::cos(angle) * size * 10));


        angle += speed / 10; //100


        if (angle > PI) {
            angle = -PI;
        }


        Sleep(10);
    }


    ReleaseDC(NULL, hdc);

    return 0;
}