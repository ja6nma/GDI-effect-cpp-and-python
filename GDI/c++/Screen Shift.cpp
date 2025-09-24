#include <windows.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

void sines() {
    HWND desktop = GetDesktopWindow();
    HDC hdc = GetWindowDC(desktop);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    double angle = 0;

    while (true) {
        hdc = GetWindowDC(desktop);
        for (int i = 0; i < sw + sh; i++) {
            int a = static_cast<int>(sin(angle) * randomInt(1, 999));
            BitBlt(hdc, randomInt(0, 99), i, sw, randomInt(0, 99), hdc, 0, i, SRCCOPY);
            angle += M_PI / randomInt(10, 999);
        }
        ReleaseDC(desktop, hdc);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    sines();
    return 0;
}