#include <Windows.h>
#include <iostream>

int main() {

    SetProcessDPIAware();


    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    HDC hdc = NULL;

    while (true) {

        hdc = GetDC(0);

        if (hdc) {

            StretchBlt(
                hdc,                    // destination DC
                0,                      // destination X
                -20,                    // destination Y (creates vertical offset)
                screenWidth,            // destination width
                screenHeight + 40,      // destination height (stretched)
                hdc,                    // source DC
                0,                      // source X
                0,                      // source Y
                screenWidth,            // source width
                screenHeight,           // source height
                SRCCOPY                 // raster operation code
            );


            ReleaseDC(0, hdc);
        }


        Sleep(10);
    }

    return 0;
}