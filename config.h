#ifndef CONFIG_H
#define CONFIG_H

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define BUZZER_PIN 8

// Varsayılan alarm süreleri (milisaniye cinsinden)
const unsigned long DEFAULT_TIMES[] = {
  180000,  // 3 dakika
  450000,  // 7.5 dakika
  600000,  // 10 dakika
  1200000  // 20 dakika
};

// Tüm melodiler
const uint16_t MELODIES[][8] = {
    {440, 554, 659, 880, 659, 554, 440, 0},    // Do-Re-Mi
    {523, 587, 659, 783, 659, 587, 523, 0},    // Klasik Alarm
    {880, 988, 880, 784, 880, 988, 880, 0},    // Kuş Sesi
    {660, 660, 660, 510, 660, 770, 660, 0},    // Mario
    {440, 440, 440, 349, 523, 440, 349, 0},    // Beethoven
    {329, 493, 698, 659, 493, 329, 493, 0},    // Trumpet
    {587, 784, 987, 587, 784, 987, 587, 0},    // Dijital
    {415, 466, 523, 622, 523, 466, 415, 0},    // Soft
    {830, 830, 830, 622, 830, 932, 830, 0},    // Yüksek
    {246, 293, 349, 415, 493, 587, 698, 0}     // Artan
};

const uint16_t MELODY_DURATIONS[][8] = {
  {200, 200, 200, 400, 200, 200, 400, 100},  // Do-Re-Mi
  {200, 200, 200, 400, 200, 200, 400, 100},  // Klasik Alarm
  {150, 150, 150, 300, 150, 150, 300, 100},  // Kuş Sesi
  {200, 200, 200, 300, 200, 200, 400, 100},  // Mario
  {300, 300, 300, 400, 300, 300, 400, 100},  // Beethoven
  {200, 200, 200, 300, 200, 200, 300, 100},  // Trumpet
  {150, 150, 150, 150, 150, 150, 300, 100},  // Dijital
  {250, 250, 250, 350, 250, 250, 350, 100},  // Soft
  {200, 200, 200, 300, 200, 200, 400, 100},  // Yüksek
  {150, 150, 150, 150, 150, 150, 150, 100}   // Artan
};

// Arayüz sabitleri
#define BUTTON_WIDTH 140
#define BUTTON_HEIGHT 80
#define MELODY_BUTTON_WIDTH 60
#define MELODY_BUTTON_HEIGHT 40

// Buton konumları (merkezlenmiş)
#define MARGIN 20
#define TIMER1_X MARGIN
#define TIMER1_Y MARGIN
#define TIMER2_X (SCREEN_WIDTH - BUTTON_WIDTH - MARGIN)
#define TIMER2_Y MARGIN
#define TIMER3_X MARGIN
#define TIMER3_Y (SCREEN_HEIGHT - BUTTON_HEIGHT - MARGIN)
#define TIMER4_X (SCREEN_WIDTH - BUTTON_WIDTH - MARGIN)
#define TIMER4_Y (SCREEN_HEIGHT - BUTTON_HEIGHT - MARGIN)

// Melodi butonları için offset (timer butonunun altında merkezlenmiş)
#define MELODY_OFFSET_Y (BUTTON_HEIGHT + 5)

// Arayüz renkleri
#define COLOR_BACKGROUND 0x0000  // Siyah (0,0,0)
#define COLOR_TEXT 0xFFFF        // Beyaz (255,255,255)
#define COLOR_BUTTON 0x07E0      // Yeşil (0,255,0)
#define COLOR_ACTIVE 0xF800      // Kırmızı (255,0,0)
#define COLOR_BUTTON_BORDER 0x8410  // Gri (128,128,128)
#define COLOR_TIMER_TEXT 0xFFE0     // Sarı (255,255,0)
#define COLOR_MELODY_TEXT 0x07FF    // Açık Mavi (0,255,255)
#define COLOR_RESET 0x001F      // Mavi (0,0,255)

// Reset süresi (ms)
#define RESET_DURATION 2000

#endif
