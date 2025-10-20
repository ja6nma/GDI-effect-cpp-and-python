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
    if hwnd:
        user32.ShowWindow(hwnd, 0)

class GDIEffects:
    def __init__(self):
        self.hdc = win32gui.GetDC(0)
        self.w = win32api.GetSystemMetrics(win32con.SM_CXSCREEN)
        self.h = win32api.GetSystemMetrics(win32con.SM_CYSCREEN)
        self.angle = 0
        self.time_counter = 0
        self.effect_duration = 8
        
    def effect_glitch_shake(self):
        for _ in range(3):
            offset = random.randint(-20, 20)
            ctypes.windll.gdi32.BitBlt(
                self.hdc, offset, offset, self.w, self.h,
                self.hdc, -offset, -offset, 0x00CC0020)
            time.sleep(0.01)
    
    def effect_spiral_distortion(self):
        self.angle += 0.1
        for i in range(5):
            radius = 50 + i * 30
            offset_x = int(math.sin(self.angle + i) * radius)
            offset_y = int(math.cos(self.angle + i) * radius)
            ctypes.windll.gdi32.BitBlt(
                self.hdc, offset_x, offset_y, self.w, self.h,
                self.hdc, -offset_x, -offset_y, 0x008800C6)
    
    def effect_pixel_wave(self):
        for y in range(0, self.h, 10):
            wave_offset = int(math.sin(self.angle + y * 0.01) * 15)
            ctypes.windll.gdi32.BitBlt(
                self.hdc, wave_offset, y, self.w, 8,
                self.hdc, 0, y, 0x00330008)
    
    def effect_zoom_rotation(self):
        scale = 0.8 + 0.2 * math.sin(self.angle)
        new_w = int(self.w * scale)
        new_h = int(self.h * scale)
        offset_x = (self.w - new_w) // 2
        offset_y = (self.h - new_h) // 2
        
        ctypes.windll.gdi32.StretchBlt(
            self.hdc, offset_x, offset_y, new_w, new_h,
            self.hdc, 0, 0, self.w, self.h, 0x00CC0020)
    
    def effect_color_invert(self):
        ctypes.windll.gdi32.BitBlt(
            self.hdc, 0, 0, self.w, self.h, self.hdc, 0, 0, 0x00990066)
        
        for _ in range(10):
            x, y = random.randint(0, self.w), random.randint(0, self.h)
            size = random.randint(50, 200)
            ctypes.windll.gdi32.PatBlt(
                self.hdc, x, y, size, size, 0x00A000C9)
    
    def effect_tunnel_vision(self):
        center_x, center_y = self.w // 2, self.h // 2
        max_radius = min(self.w, self.h) // 2
        
        for radius in range(max_radius, 0, -50):
            scale = 1.0 - (radius / max_radius) * 0.3
            new_size = int(radius * 2 * scale)
            offset_x = center_x - new_size // 2
            offset_y = center_y - new_size // 2
            
            ctypes.windll.gdi32.StretchBlt(
                self.hdc, offset_x, offset_y, new_size, new_size,
                self.hdc, center_x - radius, center_y - radius,
                radius * 2, radius * 2, 0x00BB0226)
    
    def effect_data_bending(self):
        strips = 20
        strip_height = self.h // strips
        
        for i in range(strips):
            bend_offset = int(math.sin(self.angle + i * 0.5) * 100)
            strip_y = i * strip_height
            
            ctypes.windll.gdi32.BitBlt(
                self.hdc, bend_offset, strip_y, self.w, strip_height,
                self.hdc, -bend_offset, strip_y, 0x007700E6)
    
    def effect_kaleidoscope(self):
        segments = 8
        segment_angle = 2 * math.pi / segments
        
        for i in range(segments):
            angle = i * segment_angle + self.angle
            offset_x = int(math.cos(angle) * 100)
            offset_y = int(math.sin(angle) * 100)
            
            ctypes.windll.gdi32.BitBlt(
                self.hdc, offset_x, offset_y, self.w // 2, self.h // 2,
                self.hdc, self.w // 2, self.h // 2, self.w // 2, self.h // 2, 
                0x00CC0020)
    
    def effect_plasma_wave(self):
        for x in range(0, self.w, 30):
            for y in range(0, self.h, 30):
                wave = math.sin(x * 0.01 + self.angle) * math.cos(y * 0.01 + self.angle)
                offset = int(wave * 20)
                
                ctypes.windll.gdi32.BitBlt(
                    self.hdc, x + offset, y + offset, 25, 25,
                    self.hdc, x - offset, y - offset, 0x00660046)

def main():
    hide_console()
    effects = GDIEffects()
    
    effect_functions = [
        effects.effect_glitch_shake,
        effects.effect_spiral_distortion,
        effects.effect_pixel_wave,
        effects.effect_zoom_rotation,
        effects.effect_color_invert,
        effects.effect_tunnel_vision,
        effects.effect_data_bending,
        effects.effect_kaleidoscope,
        effects.effect_plasma_wave
    ]
    
    current_effect = 0
    start_time = time.time()
    
    try:
        while True:
            if time.time() - start_time > effects.effect_duration:
                current_effect = (current_effect + 1) % len(effect_functions)
                start_time = time.time()
                effects.angle = 0
            
            effect_functions[current_effect]()
            
            effects.angle += 0.05
            effects.time_counter += 0.02
            
            win32gui.ReleaseDC(0, effects.hdc)
            effects.hdc = win32gui.GetDC(0)
            
            time.sleep(0.03)
            
    except KeyboardInterrupt:
        pass
    finally:
        win32gui.ReleaseDC(0, effects.hdc)

if __name__ == "__main__":
    main()