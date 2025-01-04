# Çoklu Zamanlayıcı Alarm Sistemi

Arduino tabanlı, 2.8" TFT dokunmatik ekran kullanan çoklu zamanlayıcı alarm sistemi.

## Özellikler

- 4 bağımsız zamanlayıcı
- Her zamanlayıcı için farklı melodi seçeneği
- Dokunmatik kontrol arayüzü
- Görsel ve sesli alarm bildirimleri
- Kolay kullanımlı arayüz tasarımı

## Donanım Gereksinimleri

- Arduino Mega 2560
- 2.8" TFT Dokunmatik LCD Ekran (ILI9341)
- Buzzer

## Pin Bağlantıları

### TFT Ekran
- LCD_CS   - Pin 10
- LCD_CD   - Pin 9
- LCD_WR   - Pin 8
- LCD_RD   - Pin 7
- LCD_RESET- Pin 6

### Dokunmatik Panel
- YP - A2
- XM - A3
- YM - 8
- XP - 9

### Buzzer
- BUZZER_PIN - 8

## Kullanım

1. Her köşede bir zamanlayıcı bulunur (3dk, 7.5dk, 10dk, 20dk)
2. Zamanlayıcıyı başlatmak için ilgili butona dokunun
3. Her zamanlayıcının altında "M1-M10" arası melodi seçim butonu bulunur
4. Aktif zamanlayıcı kırmızı renkte gösterilir
5. Alarm çaldığında:
   - İlgili buton yanıp söner
   - Seçili melodi çalar
   - Butona dokunulduğunda 2 saniye mavi renkte kalır ve sonra normale döner

## Melodiler

1. Do-Re-Mi
2. Klasik Alarm
3. Kuş Sesi
4. Mario
5. Beethoven
6. Trumpet
7. Dijital
8. Soft
9. Yüksek
10. Artan

## Dosya Yapısı

- `alarmunov2.ino`: Ana program dosyası
- `config.h`: Yapılandırma sabitleri ve melodi tanımlamaları
- `Timer.h`: Zamanlayıcı sınıfı tanımlamaları
- `Timer.cpp`: Zamanlayıcı sınıfı implementasyonu

## Kurulum

1. Arduino IDE'yi yükleyin
2. Gerekli kütüphaneleri ekleyin:
   - MCUFRIEND_kbv
   - TouchScreen
3. Donanım bağlantılarını yapın
4. Kodu Arduino'ya yükleyin

## Kütüphaneler

- MCUFRIEND_kbv
- TouchScreen
- stdint.h
- Arduino.h

## Lisans

MIT License 