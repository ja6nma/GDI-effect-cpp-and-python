//by DeepSeek
#include <windows.h>
#include <math.h>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>

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
    int lifeTime;
    bool active;
};

struct Particle {
    int x, y;
    int dx, dy;
    COLORREF color;
    int life;
    int size;
};

std::vector<MovingText> texts;
std::vector<Particle> particles;
int currentTextIndex = 0;
int frameCounter = 0;
int effectMode = 0;

double fade(double maxIntensity) {
    if (state == false) {
        intensity += 1.0;
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
        {L"YouAreInIdiot", screenWidth / 4, screenHeight / 4, 3, 2, RGB(255, 0, 0), 36, 0, true},
        {L"System32 Deleted", screenWidth / 3, screenHeight / 3, -2, 3, RGB(0, 255, 0), 32, 0, true},
        {L"R.I.P PC", screenWidth / 2, screenHeight / 2, 4, -2, RGB(0, 0, 255), 48, 0, true},
        {L"Your PC is going to die right now :)", screenWidth / 5, screenHeight * 3 / 4, -3, -4, RGB(255, 255, 0), 28, 0, true},
        {L"VIRUS DETECTED!", screenWidth / 6, screenHeight / 6, 2, -3, RGB(255, 0, 255), 40, 0, true},
        {L"CRITICAL ERROR", screenWidth * 2 / 3, screenHeight / 5, -4, 2, RGB(255, 100, 0), 38, 0, true},
        {L"FORMATTING C: DRIVE", screenWidth / 8, screenHeight * 4 / 5, 5, 1, RGB(0, 255, 255), 34, 0, true}
    };
}

void CreateParticle(int x, int y, COLORREF color) {
    Particle p;
    p.x = x;
    p.y = y;
    p.dx = (rand() % 11) - 5;
    p.dy = (rand() % 11) - 5;
    p.color = color;
    p.life = 50 + rand() % 100;
    p.size = 2 + rand() % 8;
    particles.push_back(p);
}

void UpdateParticles(int screenWidth, int screenHeight) {
    for (size_t i = 0; i < particles.size(); ) {
        particles[i].x += particles[i].dx;
        particles[i].y += particles[i].dy;
        particles[i].life = particles[i].life - 1;

        if (particles[i].life <= 0 || particles[i].x < 0 || particles[i].x > screenWidth || particles[i].y < 0 || particles[i].y > screenHeight) {
            particles.erase(particles.begin() + i);
        }
        else {
            i = i + 1;
        }
    }
}

void UpdateTextPositions(int screenWidth, int screenHeight) {
    frameCounter = frameCounter + 1;


    if (frameCounter % 500 == 0) {
        effectMode = (effectMode + 1) % 4;
    }

    for (size_t i = 0; i < texts.size(); ) {
        if (texts[i].lifeTime > 0) {
            texts[i].lifeTime = texts[i].lifeTime - 1;
            if (texts[i].lifeTime <= 0) {
                texts.erase(texts.begin() + i);
                continue;
            }
        }


        texts[i].x += texts[i].dx;
        texts[i].y += texts[i].dy;

 
        if (texts[i].x <= 0 || texts[i].x >= screenWidth - 300) {
            texts[i].dx = -texts[i].dx;
            texts[i].x = max(0, min(screenWidth - 300, texts[i].x));
            CreateParticle(texts[i].x, texts[i].y, texts[i].color);
        }
        if (texts[i].y <= 0 || texts[i].y >= screenHeight - 50) {
            texts[i].dy = -texts[i].dy;
            texts[i].y = max(0, min(screenHeight - 50, texts[i].y));
            CreateParticle(texts[i].x, texts[i].y, texts[i].color);
        }


        if (rand() % 80 == 0) {
            texts[i].dx = (rand() % 9) - 4;
            texts[i].dy = (rand() % 9) - 4;
            CreateParticle(texts[i].x, texts[i].y, texts[i].color);
        }


        if (frameCounter % 20 == 0) {
            texts[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
        }


        if (frameCounter % 3 == 0) {
            CreateParticle(texts[i].x, texts[i].y, texts[i].color);
        }

        i = i + 1;
    }

    UpdateParticles(screenWidth, screenHeight);
}

void DrawParticles(HDC hdc) {
    for (const auto& particle : particles) {
        HBRUSH brush = CreateSolidBrush(particle.color);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

        int alpha = min(255, particle.life * 2);
        SetDCBrushColor(hdc, particle.color);

        Ellipse(hdc, particle.x - particle.size, particle.y - particle.size,
            particle.x + particle.size, particle.y + particle.size);

        SelectObject(hdc, oldBrush);
        DeleteObject(brush);
    }
}

void DrawMovingText(HDC hdc, const MovingText& text) {
    HFONT hFont = CreateFont(text.fontSize, 0, 0, 0, FW_BOLD,
        FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Consolas");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);


    SetTextColor(hdc, RGB(0, 0, 0));
    TextOut(hdc, text.x + 3, text.y + 3, text.text.c_str(), text.text.length());


    SetTextColor(hdc, text.color);
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, text.x, text.y, text.text.c_str(), text.text.length());

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

void DrawScanLines(HDC hdc, int screenWidth, int screenHeight) {
    for (int y = 0; y < screenHeight; y += 4) {
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        MoveToEx(hdc, 0, y, NULL);
        LineTo(hdc, screenWidth, y);
        SelectObject(hdc, oldPen);
        DeleteObject(pen);
    }
}

void DrawGlitchEffect(HDC hdc, int screenWidth, int screenHeight, int offset) {
    BitBlt(hdc, offset, 0, screenWidth - offset, screenHeight, hdc, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, offset, screenWidth, screenHeight - offset, hdc, 0, 0, SRCCOPY);
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
        GetBitmapBits(bmp, x * y * sizeof(COLOR), data);


        for (int i = 0; i < x * y; i++) {
            BYTE* pixel = (BYTE*)(data + i);

            switch (effectMode) {
            case 0: 
                pixel[1 % 2] = pixel[r & 2];
                break;

            case 1: 
                pixel[0] = 255 - pixel[0];
                pixel[1] = 255 - pixel[1];
                pixel[2] = 255 - pixel[2];
                break;

            case 2: 
            {
                BYTE blue = pixel[0];
                BYTE green = pixel[1];
                BYTE red = pixel[2];
                BYTE gray = (red + green + blue) / 3;
                pixel[2] = min(255, gray + 40);  
                pixel[1] = min(255, gray + 20);  
                pixel[0] = gray;                 
            }
            break;

            case 3: 
                if (rand() % 10 == 0) {
                    pixel[rand() % 3] = rand() % 256;
                }
                break;
            }
        }

        SetBitmapBits(bmp, x * y * sizeof(COLOR), data);
        BitBlt(hdc, 0, 0, x, y, mdc, 0, 0, SRCCOPY);


        if (effectMode == 1) {
            DrawScanLines(hdc, x, y);
        }

        if (frameCounter % 60 < 30) {
            DrawGlitchEffect(hdc, x, y, 10 + rand() % 30);
        }


        UpdateTextPositions(x, y);


        DrawParticles(hdc);


        for (const auto& text : texts) {
            DrawMovingText(hdc, text);
        }

        Sleep(40);
        r = fade(5);
    }

    DeleteObject(bmp);
    DeleteDC(mdc);
    ReleaseDC(0, hdc);
    VirtualFree(data, 0, MEM_RELEASE);

    return 0;
}

int main() {
    HANDLE hThread = CreateThread(0, 0, shader3b, 0, 0, 0);
    if (hThread) {
        Sleep(-1);
        CloseHandle(hThread);
    }
    return 0;
}