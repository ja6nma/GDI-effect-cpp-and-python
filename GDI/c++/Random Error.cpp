#include <Windows.h>
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    HDC hdc = GetDC(NULL);
    SetProcessDPIAware();
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    while (true) {
        DrawIcon(
            hdc,
            std::rand() % w,
            std::rand() % h,
            LoadIconW(NULL, IDI_HAND)
            //IDI_APPLICATION,    
            // IDI_HAND,           
            //IDI_QUESTION,       
            // IDI_EXCLAMATION,                
            // IDI_WARNING, ...       
        );
        Sleep(10);
    }

    ReleaseDC(NULL, hdc);
    return 0;
}