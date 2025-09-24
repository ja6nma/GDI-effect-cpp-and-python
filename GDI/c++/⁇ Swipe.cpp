#include <Windows.h>
#include <cmath>
#include <ctime>

int main() {
    SetProcessDPIAware();

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    HDC hdc = NULL;
    float time = 0.0f;
    srand(static_cast<unsigned>(time(nullptr)));

    while (true) {
        hdc = GetDC(0);

        if (hdc) {
            time += 0.05f;
            
            int effectType = (int)(time * 0.5f) % 6;
            
            int destX = 0, destY = 0;
            int destWidth = screenWidth, destHeight = screenHeight;
            DWORD rop = SRCCOPY;
            
            switch (effectType) {
            case 0: {
                destY = (int)(sin(time * 3) * 30 - 20);
                destHeight = screenHeight + 40;
                rop = SRCCOPY;
                break;
            }
                
            case 1: {
                destX = (int)(cos(time * 2) * 25);
                destWidth = screenWidth + 50;
                rop = NOTSRCCOPY;
                break;
            }
                
            case 2: {
                StretchBlt(hdc, 
                    (int)(sin(time) * 20), -15, 
                    screenWidth, screenHeight + 30,
                    hdc, 0, 0, screenWidth, screenHeight, SRCINVERT);
                    
                StretchBlt(hdc, 
                    -(int)(cos(time) * 20), -15, 
                    screenWidth, screenHeight + 30,
                    hdc, 0, 0, screenWidth, screenHeight, SRCINVERT);
                break;
            }
                
            case 3: {
                float scale = 1.0f + sin(time * 2) * 0.1f;
                int offsetX = (int)((screenWidth - screenWidth * scale) / 2);
                int offsetY = (int)((screenHeight - screenHeight * scale) / 2);
                
                StretchBlt(hdc, 
                    offsetX, offsetY - 10,
                    (int)(screenWidth * scale), (int)(screenHeight * scale) + 20,
                    hdc, 0, 0, screenWidth, screenHeight, SRCPAINT);
                break;
            }
                
            case 4: {
                for (int i = 0; i < 3; i++) {
                    int randX = rand() % 41 - 20;
                    int randY = rand() % 41 - 20;
                    
                    StretchBlt(hdc, 
                        randX, randY - 10,
                        screenWidth, screenHeight + 20,
                        hdc, 0, 0, screenWidth, screenHeight, 
                        (i % 2 == 0) ? SRCAND : SRCPAINT);
                }
                break;
            }
                
            case 5: {
                float angle = time * 2;
                int spiralX = (int)(cos(angle) * 40);
                int spiralY = (int)(sin(angle) * 40 - 20);
                
                for (int i = 0; i < 2; i++) {
                    StretchBlt(hdc, 
                        spiralX * (i+1), spiralY * (i+1),
                        screenWidth, screenHeight + 40,
                        hdc, 0, 0, screenWidth, screenHeight, 
                        (i == 0) ? SRCINVERT : NOTSRCERASE);
                }
                break;
            }
            }

            if (effectType != 2) {
                StretchBlt(hdc, 
                    destX, destY, 
                    destWidth, destHeight,
                    hdc, 0, 0, screenWidth, screenHeight, rop);
            }

            if (rand() % 30 == 0) {
                int scanY = rand() % screenHeight;
                PatBlt(hdc, 0, scanY, screenWidth, 3, DSTINVERT);
            }

            if (rand() % 100 == 0) {
                PatBlt(hdc, 0, 0, screenWidth, screenHeight, WHITENESS);
                Sleep(5);
                PatBlt(hdc, 0, 0, screenWidth, screenHeight, BLACKNESS);
            }

            ReleaseDC(0, hdc);
        }

        Sleep(15);
    }

    return 0;
}