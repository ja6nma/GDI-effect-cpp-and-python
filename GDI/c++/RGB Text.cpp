#include <windows.h>
#include <math.h>
#include <string>
#include <vector>
#include <ctime>

typedef union COLOR {
    COLORREF rgb;
    struct {
        BYTE blue;
        BYTE green;
        BYTE red;
    };
} COLOR;

double intensity = 0.0;
bool state = false;

struct MovingText {
    std::wstring text;
    int x, y;
    int dx, dy;
    COLORREF color;
    int fontSize;
};

std::vector<MovingText> texts;
int currentTextIndex = 0;
int frameCounter = 0;

double fade(double maxIntensity) {
    if (state == false) {
        intensity += 10.0;
        if (intensity >= maxIntensity) {
            state = true;
        }
    }
    else {
        intensity -= 1.0;
        if (intensity <= 0) {
            state = false;
        }
    }
    return intensity;
}

void InitializeTexts(int screenWidth, int screenHeight) {
    texts = {
        {L"YouAreInIdiot", screenWidth / 4, screenHeight / 4, 3, 2, RGB(255, 0, 0), 36},
        {L"System32 Deleted", screenWidth / 3, screenHeight / 3, -2, 3, RGB(0, 255, 0), 32},
        {L"R.I.P PC", screenWidth / 2, screenHeight / 2, 4, -2, RGB(0, 0, 255), 48},
        {L"Your PC is going to die right now :)", screenWidth / 5, screenHeight * 3 / 4, -3, -4, RGB(255, 255, 0), 28}
    };
}

void UpdateTextPositions(int screenWidth, int screenHeight) {
    frameCounter++;


    if (frameCounter % 2000 == 0) {
        currentTextIndex = (currentTextIndex + 1) % texts.size();
    }

    for (auto& text : texts) {

        text.x += text.dx;
        text.y += text.dy;


        if (text.x <= 0 || text.x >= screenWidth - 200) {
            text.dx = -text.dx;
            text.x = max(0, min(screenWidth - 200, text.x));
        }
        if (text.y <= 0 || text.y >= screenHeight - 50) {
            text.dy = -text.dy;
            text.y = max(0, min(screenHeight - 50, text.y));
        }


        if (rand() % 100 == 0) {
            text.dx = (rand() % 7) - 3;
            text.dy = (rand() % 7) - 3;
        }


        if (frameCounter % 30 == 0) {
            text.color = RGB(rand() % 256, rand() % 256, rand() % 256);
        }
    }
}

void DrawMovingText(HDC hdc, const MovingText& text) {
    HFONT hFont = CreateFont(text.fontSize, 0, 0, 0, FW_BOLD,
        //HFONT hFont = CreateFont(text.fontSize, 50, 0, 0, FW_BOLD
        //HFONT hFont = CreateFont(text.fontSize, 0, 50, 0, FW_BOLD
        FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    SetTextColor(hdc, text.color);
    SetBkMode(hdc, TRANSPARENT);


    SetTextColor(hdc, RGB(0, 0, 0));
    //SetTextColor(hdc, RGB(255, 0, 0))
    //SetTextColor(hdc, RGB(0, 255, 0))
    //SetTextColor(hdc, RGB(0, 0, 255))
    TextOut(hdc, text.x + 2, text.y + 2, text.text.c_str(), text.text.length());


    SetTextColor(hdc, text.color);
    TextOut(hdc, text.x, text.y, text.text.c_str(), text.text.length());

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

DWORD WINAPI shader3b(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int r = 0;

    srand(static_cast<unsigned int>(time(0)));
    InitializeTexts(x, y);

    COLOR* data = (COLOR*)VirtualAlloc(0, (x * y + x) * sizeof(COLOR), MEM_COMMIT, PAGE_READWRITE);
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    while (true) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);
        //BitBlt(mdc, 1, 0, x, y, hdc, 0, 0, SRCCOPY)
        //BitBlt(mdc, 0, 1, x, y, hdc, 0, 0, SRCCOPY)
        //BitBlt(mdc, 0, 0, x, y, hdc, 1, 0, SRCCOPY)
        //BitBlt(mdc, 0, 0, x, y, hdc, 0, 1, SRCCOPY)
        GetBitmapBits(bmp, x * y * sizeof(COLOR), data);

        for (int i = 0; i < x * y; i++) {
            ((BYTE*)(data + i))[1 % 2] = ((BYTE*)(data + i))[r & 2];
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);
        //BitBlt(hdc, 1, 0, x, y, mdc, 0, 0, SRCCOPY)
        //BitBlt(hdc, 0, 1, x, y, mdc, 0, 0, SRCCOPY)
        //BitBlt(hdc, 0, 0, x, y, mdc, 1, 0, SRCCOPY)
        //BitBlt(hdc, 0, 0, x, y, mdc, 0, 1, SRCCOPY)


        UpdateTextPositions(x, y);


        for (const auto& text : texts) {
            DrawMovingText(hdc, text);
        }


        if (frameCounter % 10 < 5) {
            MovingText highlighted = texts[currentTextIndex];
            highlighted.fontSize += 8;
            DrawMovingText(hdc, highlighted);
        }

        Sleep(30);
        r = fade(3);
    }

    DeleteObject(bmp);
    DeleteDC(mdc);
    ReleaseDC(0, hdc);
    VirtualFree(data, 0, MEM_RELEASE);

    return 0;
}

int main() {
    DWORD threadId;
    HANDLE hThread = CreateThread(0, 0, shader3b, 0, 0, &threadId);
    if (hThread) {
        Sleep(-1);
        CloseHandle(hThread);
    }
    return 0;
}