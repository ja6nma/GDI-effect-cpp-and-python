#By ITTM, Dodo
import ctypes
import random
import time
import math
import win32gui
import win32api
import win32con

def hide_console():
    kernel32 = ctypes.WinDLL('kernel32')
    user32 = ctypes.WinDLL('user32')
    hwnd = kernel32.GetConsoleWindow()
    if hwnd: user32.ShowWindow(hwnd, 0)

def main():
    hide_console()
    hdc = win32gui.GetDC(0)
    w = win32api.GetSystemMetrics(win32con.SM_CXSCREEN)
    h = win32api.GetSystemMetrics(win32con.SM_CYSCREEN)
    
    angle = 0
    effect_switch = 0
    start_time = time.time()
    
    while True:

        if time.time() - start_time > 10:
            effect_switch = (effect_switch + 1) % 6
            start_time = time.time()
        
        if effect_switch == 0:  
            ctypes.windll.gdi32.BitBlt(
                hdc, random.randint(-50, 50), random.randint(-50, 50), w, h,
                hdc, random.randint(-50, 50), random.randint(-50, 50), 0x001100A6)
        
        elif effect_switch == 1:  
            angle += 0.05
            offset_x = int(math.sin(angle) * 100)
            offset_y = int(math.cos(angle * 0.7) * 100)
            ctypes.windll.gdi32.BitBlt(
                hdc, offset_x, offset_y, w, h, hdc, 0, 0, 0x00CC0020)  # SRCPAINT
        
        elif effect_switch == 2:  
            ctypes.windll.gdi32.StretchBlt(
                hdc, 10, 10, w-20, h-20, hdc, 0, 0, w, h, 0x00330008)  # NOTSRCCOPY
        
        elif effect_switch == 3:  
            for i in range(5):
                x, y = random.randint(0, w), random.randint(0, h)
                ctypes.windll.gdi32.BitBlt(
                    hdc, x, y, 200, 200, hdc, x+random.randint(-50,50), 
                    y+random.randint(-50,50), 0x00BB0226)  # MERGECOPY
        
        elif effect_switch == 4:  
            for i, op in enumerate([0x00CC0020, 0x00660046, 0x008800C6]):
                ctypes.windll.gdi32.BitBlt(
                    hdc, i*3-3, i*3-3, w, h, hdc, 0, 0, op)
        
        elif effect_switch == 5:  
            ctypes.windll.gdi32.PatBlt(
                hdc, random.randint(0,w), random.randint(0,h),
                random.randint(50,300), random.randint(50,300),
                0x00A50055)  # PATINVERT
        
        time.sleep(0.02)
        win32gui.ReleaseDC(0, hdc)
        hdc = win32gui.GetDC(0) 
if name == "main":
    main()