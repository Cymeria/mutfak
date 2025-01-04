#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <stdint.h>
#include "config.h"
#include "Timer.h"

MCUFRIEND_kbv tft;
// TouchScreen tanımlamaları buraya gelecek

#define MINPRESSURE 200
#define MAXPRESSURE 1000

// Dokunmatik ekran kalibrasyon değerleri
#define TS_MINX 120
#define TS_MAXX 900
#define TS_MINY 70
#define TS_MAXY 920

// Dokunmatik ekran tanımlamaları
#define YP A2
#define XM A3
#define YM 8
#define XP 9
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Timer timers[4];
bool needUpdate = true;
bool alarmTriggered = false;
unsigned long lastBlinkTime = 0;
bool blinkState = false;
#define BLINK_INTERVAL 500 // Yanıp sönme hızı (ms)

// Global değişkenlere ekle
unsigned long resetStartTime = 0;
int resettingTimer = -1;

void setup() {
  // Seri haberleşmeyi başlat (hata ayıklama için)
  Serial.begin(9600);
  
  // Ekran başlatma
  tft.reset();
  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  
  if (ID == 0xD3D3) ID = 0x9481; // Force ID if write-only display
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(COLOR_BACKGROUND);
  
  // Buzzer pini
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Zamanlayıcıları başlat
  for (int i = 0; i < 4; i++) {
    timers[i].setDuration(DEFAULT_TIMES[i]);
  }
  
  drawInterface();
}

void loop() {
  // Dokunma kontrolü
  bool touch = checkTouch();
  if (touch) {
    Serial.println("Touch detected!");
    needUpdate = true;
    if (alarmTriggered) {
      alarmTriggered = false;
      blinkState = false;
      needUpdate = true;
    }
  }
  
  // Zamanlayıcıları güncelle
  bool timerChanged = false;
  for (int i = 0; i < 4; i++) {
    unsigned long oldRemaining = timers[i].getRemaining();
    timers[i].update();
    unsigned long newRemaining = timers[i].getRemaining();
    
    // Eğer kalan süre değiştiyse ekranı güncelle
    if (oldRemaining != newRemaining) {
      timerChanged = true;
    }
    
    if (timers[i].isDone() && !alarmTriggered) {
      playAlarm(i);
      alarmTriggered = true;
    }
  }
  
  // Süre değiştiğinde ekranı güncelle
  if (timerChanged) {
    needUpdate = true;
  }
  
  // Alarm durumunda yanıp sönme efekti
  if (alarmTriggered) {
    if (millis() - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = millis();
      blinkState = !blinkState;
      needUpdate = true;
    }
  }
  
  // Reset süresini kontrol et
  if (resettingTimer >= 0 && 
      (millis() - resetStartTime >= RESET_DURATION)) {
    resettingTimer = -1;
    needUpdate = true;
  }
  
  // Ekranı güncelle
  if (needUpdate) {
    updateDisplay();
    needUpdate = false;
  }
  
  delay(10); // Daha kısa bekleme süresi
}

void drawInterface() {
  tft.fillScreen(COLOR_BACKGROUND);
  
  int timerX[] = {TIMER1_X, TIMER2_X, TIMER3_X, TIMER4_X};
  int timerY[] = {TIMER1_Y, TIMER2_Y, TIMER3_Y, TIMER4_Y};
  
  for(int i = 0; i < 4; i++) {
    // Timer butonu
    uint16_t buttonColor = COLOR_BUTTON;
    if (i == resettingTimer) {
      buttonColor = COLOR_RESET;
    } else if (timers[i].isActive()) {
      buttonColor = COLOR_ACTIVE;
    }
    
    drawButton(timerX[i], timerY[i], BUTTON_WIDTH, BUTTON_HEIGHT, 
               String(DEFAULT_TIMES[i]/60000) + "dk", buttonColor);
    
    // Melodi butonu (timer butonunun altında merkezlenmiş)
    int melodyX = timerX[i] + (BUTTON_WIDTH - MELODY_BUTTON_WIDTH) / 2;
    drawButton(melodyX, timerY[i] + MELODY_OFFSET_Y, 
               MELODY_BUTTON_WIDTH, MELODY_BUTTON_HEIGHT,
               "M" + String(timers[i].getMelody()+1), COLOR_MELODY_TEXT);
               
    // Aktif timer için süre gösterimi
    if(timers[i].isActive()) {
      unsigned long remaining = timers[i].getRemaining();
      int minutes = remaining / 60000;
      int seconds = (remaining % 60000) / 1000;
      String timeStr = String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds);
      
      tft.setTextColor(COLOR_TIMER_TEXT);
      tft.setTextSize(3);
      
      // Süreyi ortala
      int16_t x1, y1;
      uint16_t tw, th;
      tft.getTextBounds(timeStr, 0, 0, &x1, &y1, &tw, &th);
      tft.setCursor(timerX[i] + (BUTTON_WIDTH - tw) / 2, 
                   timerY[i] + BUTTON_HEIGHT - th - 10);
      tft.print(timeStr);
    }
  }
}

void drawButton(int x, int y, int w, int h, String text, uint16_t color) {
  tft.fillRoundRect(x, y, w, h, 5, color);
  tft.drawRoundRect(x, y, w, h, 5, COLOR_BUTTON_BORDER);
  
  // Text boyutunu buton boyutuna göre ayarla
  int textSize = (w > 100) ? 3 : 2;
  tft.setTextSize(textSize);
  
  // Texti ortala
  int16_t x1, y1;
  uint16_t tw, th;
  tft.getTextBounds(text, 0, 0, &x1, &y1, &tw, &th);
  
  int textX = x + (w - tw) / 2;
  int textY = y + (h - th) / 2;
  
  tft.setTextColor(COLOR_TEXT);
  tft.setCursor(textX, textY);
  tft.print(text);
}

void updateDisplay() {
  int timerX[] = {TIMER1_X, TIMER2_X, TIMER3_X, TIMER4_X};
  int timerY[] = {TIMER1_Y, TIMER2_Y, TIMER3_Y, TIMER4_Y};
  
  for(int i = 0; i < 4; i++) {
    unsigned long remaining = timers[i].getRemaining();
    
    // Timer butonunun rengini güncelle
    uint16_t buttonColor = COLOR_BUTTON;
    if (timers[i].isActive()) {
      buttonColor = COLOR_ACTIVE;
    } else if (alarmTriggered && timers[i].isDone()) {
      buttonColor = blinkState ? COLOR_ACTIVE : COLOR_BUTTON;
    }
    
    drawButton(timerX[i], timerY[i], BUTTON_WIDTH, BUTTON_HEIGHT, 
               String(DEFAULT_TIMES[i]/60000) + "dk", 
               buttonColor);
    
    // Kalan süreyi göster - timer butonunun içinde
    if(timers[i].isActive()) {
      tft.setTextColor(COLOR_TIMER_TEXT);
      tft.setCursor(timerX[i] + 10, timerY[i] + BUTTON_HEIGHT/2);
      
      int minutes = remaining / 60000;
      int seconds = (remaining % 60000) / 1000;
      tft.print(String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds));
    }
  }
}

bool checkTouch() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // ÖNEMLI: Pin modlarını geri ayarla
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    Serial.print("Raw X = "); Serial.print(p.x);
    Serial.print("\tRaw Y = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
    
    // Koordinat dönüşümünü düzelt - her iki koordinatı da ters çevir
    int16_t x = map(p.x, TS_MAXX, TS_MINX, 0, tft.width());
    int16_t y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    
    Serial.print("Mapped X = "); Serial.print(x);
    Serial.print("\tMapped Y = "); Serial.println(y);
    
    int timerX[] = {TIMER1_X, TIMER2_X, TIMER3_X, TIMER4_X};
    int timerY[] = {TIMER1_Y, TIMER2_Y, TIMER3_Y, TIMER4_Y};
    
    for(int i = 0; i < 4; i++) {
      // Timer butonu kontrolü
      if(x >= timerX[i] && x <= timerX[i] + BUTTON_WIDTH &&
         y >= timerY[i] && y <= timerY[i] + BUTTON_HEIGHT) {
        // Reset modundaysa ve bu timer resetleniyorsa, dokunmayı yoksay
        if (resettingTimer == i && (millis() - resetStartTime < RESET_DURATION)) {
          return false;
        }
        
        if(timers[i].isActive()) {
          timers[i].stop();
        } else if (timers[i].isDone() && alarmTriggered) {
          // Alarmı durdur ve reset moduna geç
          alarmTriggered = false;
          blinkState = false;
          resettingTimer = i;
          resetStartTime = millis();
        } else {
          timers[i].start();
        }
        needUpdate = true;
        return true;
      }
      
      // Melodi butonu kontrolü
      int melodyX = timerX[i] + (BUTTON_WIDTH - MELODY_BUTTON_WIDTH) / 2;
      if(x >= melodyX && x <= melodyX + MELODY_BUTTON_WIDTH &&
         y >= timerY[i] + MELODY_OFFSET_Y && 
         y <= timerY[i] + MELODY_OFFSET_Y + MELODY_BUTTON_HEIGHT) {
        timers[i].setMelody((timers[i].getMelody() + 1) % 10);
        needUpdate = true;
        return true;
      }
    }
  }
  return false;
}

void playAlarm(int timerIndex) {
  uint8_t melodyIndex = timers[timerIndex].getMelody();
  
  // Melodiyi bir kez çal
  for(int note = 0; note < 8; note++) {
    if(MELODIES[melodyIndex][note] > 0) {
      tone(BUZZER_PIN, MELODIES[melodyIndex][note], 
           MELODY_DURATIONS[melodyIndex][note]);
      delay(MELODY_DURATIONS[melodyIndex][note] * 1.3);
    }
  }
  noTone(BUZZER_PIN);
}
