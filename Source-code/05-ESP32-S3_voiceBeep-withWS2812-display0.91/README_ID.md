# ESP32-S3 Hardware Test Suite - OLED, Audio, Mic & NeoPixel



[![Image](https://img.shields.io/badge/ESP32-S3%20Hardware%20Test-brightgreen)](https://github.com/yourusername/esp32-s3-hw-test)

![Image](https://img.shields.io/badge/Platform-Arduino-blue)

![Image](https://img.shields.io/badge/License-MIT-yellow.svg)



## 📖 Gambaran Umum

Proyek ini merupakan suite pengujian hardware komprehensif untuk **Goouuuu ESP32-S3 Board** yang terintegrasi dengan layar OLED, audio I2S (speaker dan mikrofon), serta LED WS2812B NeoPixel. Pengujian ini memeriksa semua periferal secara bersamaan, memberikan umpan balik visual di OLED, suara beep audio, reaktivitas mikrofon, dan animasi LED.

Fitur utama:

- Layar OLED (0.91" 128x32): Menampilkan status inisialisasi, mode pemantauan, dan level mikrofon sebagai VU meter.
- Speaker (via MAX98357A I2S Amp): Memainkan nada tes (beep) saat tombol ditekan.
- Mikrofon (INMP441 I2S): Deteksi level real-time dengan LED reaktif dan bar tampilan.
- LED NeoPixel (WS2812B): Beberapa mode (Off, Rainbow, Breathing, Reaktif Mikrofon) yang dapat dikontrol via tombol.
- Kontrol Tombol: Tekan pendek tombol BOOT untuk tes beep; tekan lama (1 detik) untuk ganti mode LED.

Dirancang untuk validasi hardware cepat selama prototipe atau pemecahan masalah.

## 🛠️ Hardware yang Dibutuhkan

| No      | Komponen                        | Deskripsi                                                    | Koneksi Utama                                                |
| ------- | ------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 1       | Goouuuu ESP32-S3 Board          | Papan mikrokontroler utama dengan chip ESP32-S3.             | USB untuk daya/programming.                                  |
| 2       | OLED Display 0.91" (128x32)     | Layar monokrom berbasis I2C SSD1306.                         | SDA: GPIO 41, SCL: GPIO 42, VCC: 3.3V, GND.                  |
| 3       | Goouuuu Shield                  | Papan terintegrasi dengan MAX98357A I2S Audio Amplifier dan INMP441 I2S Microphone. | Pasang pada ESP32-S3; Speaker: DOUT=7, BCLK=15, LRC=16; Mic: WS=4, SCK=5, SD=6. |
| 4       | Speaker                         | Speaker mini 4Ω 3W untuk output audio.                       | Hubungkan ke pin speaker Goouuuu Shield.                     |
| 5       | Kabel USB                       | Kabel USB-C atau Micro-USB untuk programming dan daya.       | Hubungkan ESP32-S3 ke PC.                                    |
| *Bonus* | WS2812B NeoPixel LED (opsional) | Satu LED RGB untuk efek visual.                              | Pin data: GPIO 48, VCC: 5V, GND.                             |

**Estimasi Biaya Total**: ~$20-30 USD (tidak termasuk papan ESP32-S3).

## 🔌 Diagram Koneksi

### OLED (I2C)

text

```
OLED    ESP32-S3
VCC  -> 3.3V
GND  -> GND
SDA  -> GPIO 41
SCL  -> GPIO 42
```

### Goouuuu Shield + Speaker & Mic

- Pasang shield langsung pada pin ESP32-S3 (jika kompatibel).
- Speaker: Hubungkan ke output shield (menggunakan pin I2S: 7, 15, 16).
- Mic: Terintegrasi di shield (menggunakan pin I2S: 4, 5, 6).

### NeoPixel LED

text

```
LED     ESP32-S3
VCC  -> 5V
GND  -> GND
DIN  -> GPIO 48
```

**Catatan**: Gunakan level shifter jika menghubungkan LED 5V ke GPIO 3.3V (opsional untuk satu LED).

## 📦 Instalasi

### 1. Pengaturan Arduino IDE

- Unduh dan instal [Arduino IDE](https://www.arduino.cc/en/software) (versi 2.x direkomendasikan).
- Instal dukungan papan ESP32:
  - Buka File > Preferences, tambahkan https://espressif.github.io/arduino-esp32/package_esp32_index.json ke Additional Boards Manager URLs.
  - Tools > Board > Boards Manager, cari "ESP32" dan instal esp32 by Espressif Systems (v2.0.14+).
- Pilih papan: Tools > Board > ESP32 Arduino > ESP32S3 Dev Module.
- Atur: Flash Size: 8MB, Partition Scheme: Default 4MB with spiffs, PSRAM: Enabled.

### 2. Library yang Diperlukan

Instal via **Tools > Manage Libraries**:

- **Adafruit GFX Library** oleh Adafruit (v1.11.9+)
- **Adafruit SSD1306** oleh Adafruit (v2.5.7+)
- **Adafruit NeoPixel** oleh Adafruit (v1.11.0+)

### 3. Unggah Kode

- Buka 05-ESP32-S3_voiceBeep-withWS2812-display0.91.ino di Arduino IDE.
- Hubungkan ESP32-S3 via USB.
- Pilih port yang benar: Tools > Port.
- Unggah: Sketch > Upload.
- Pantau output serial: Tools > Serial Monitor (115200 baud).

## 🚀 Penggunaan

1. **Nyalakan Daya**: Hubungkan USB; OLED menampilkan "ESP32-S3 HW TEST" dan kemajuan inisialisasi.
2. **Tes Otomatis**:
   - OLED: Menampilkan status (OK untuk setiap komponen).
   - Speaker: Memainkan beep startup dengan kilatan LED.
   - Mic: Mulai pemantauan (bicara/tepuk tangan untuk lihat level).
   - LED: Menjalankan urutan tes RGB.
3. **Kontrol Interaktif** (Tombol BOOT - GPIO 0):
   - Tekan Pendek (<1 detik): Memainkan tes beep (440Hz + 523Hz) dengan warna LED.
   - Tekan Lama (>1 detik): Ganti mode LED (0: Off → 1: Rainbow → 2: Breathing → 3: Reaktif Mikrofon).
4. **Mode Pemantauan**:
   - OLED: Menampilkan "MONITORING", mode LED, dan bar level mic (bawah).
   - Serial: VU meter real-time (bar █) + level (0-1000) + indikator emoji.
   - LED: Bereaksi terhadap input mic di Mode 3 (Hijau → Kuning → Merah berdasarkan volume).

**Mode LED**:

- 0: OFF - LED mati.
- 1: Rainbow - Siklus warna.
- 2: Breathing - Denyut biru.
- 3: Reaktif Mikrofon - Warna/intensitas berdasarkan level mic (gaya VU meter ⭐).

## 📊 Contoh Output Serial

text

```
╔════════════════════════════════════════════╗
║  SEMUA SISTEM SIAP!                       ║
║  - Bicara/tepuk untuk tes mikrofon        ║
║  - Tekan BOOT (pendek) untuk tes beep     ║
║  - Tahan BOOT (1s) untuk ganti mode LED   ║
╚════════════════════════════════════════════╝

[████████████████████                      ] Lvl:  245 🔉 | LED:3
[░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░] Lvl:   12 🔇 | LED:3
[██████████████████████████████████████████] Lvl:  856 📢 | LED:3
```

## 🔧 Pemecahan Masalah

| Masalah           | Penyebab Mungkin       | Solusi                                                       |
| ----------------- | ---------------------- | ------------------------------------------------------------ |
| OLED kosong       | Pin I2C/saluran salah  | Periksa koneksi (GPIO 41/42, 0x3C); Jalankan sketsa I2C scanner. |
| Tidak ada audio   | Konfig I2S tidak cocok | Verifikasi pin shield; Pastikan SAMPLE_RATE=16000 di kode.   |
| Mic diam          | Buffer overflow        | Flush buffer mic di init; Tes dengan suara lebih keras.      |
| LED tidak menyala | Masalah daya           | Gunakan suplai 5V; Periksa pin data (GPIO 48).               |
| Upload gagal      | Papan/port salah       | Pilih ESP32S3 Dev Module; Tahan BOOT saat upload jika diperlukan. |
| Serial acak       | Baud rate              | Atur ke 115200; Restart monitor setelah upload.              |

- **Tips Debug**: Komentari bagian (misalnya, initLED()) untuk isolasi masalah.
- **Daya**: Gunakan suplai stabil 5V/2A untuk beban penuh (speaker + LED).

## 📝 Kontribusi

Fork repo, buat branch, dan kirim PR. Masalah selamat datang!

## 📄 Lisensi

Proyek ini dilisensikan di bawah MIT License - lihat file LICENSE untuk detail.

## 🙏 Ucapan Terima Kasih

- Library Adafruit untuk GFX, SSD1306, dan NeoPixel.
- Espressif untuk dukungan ESP32-S3.
- Goouuuu untuk hardware ESP32 terjangkau.

------

copyrigth by hwthinker