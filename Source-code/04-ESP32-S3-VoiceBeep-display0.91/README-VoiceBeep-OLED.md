# 🎤📺 ESP32-S3 Complete Hardware Test (OLED 0.91") - Panduan Lengkap

## 📋 Daftar Isi
- [Tentang Program](#-tentang-program)
- [Hardware yang Dibutuhkan](#-hardware-yang-dibutuhkan)
- [Koneksi Hardware](#-koneksi-hardware)
- [Instalasi Software](#-instalasi-software)
- [Cara Upload Program](#-cara-upload-program)
- [Cara Menggunakan](#-cara-menggunakan)
- [Troubleshooting](#-troubleshooting)
- [Modifikasi Program](#-modifikasi-program)

---

## 🎯 Tentang Program

Program ini adalah **complete hardware test** untuk ESP32-S3 yang menggabungkan:
- ✅ **OLED Display 0.91"** (128x32) - Status display
- ✅ **MAX98357A Speaker** - Audio output test
- ✅ **INMP441 Microphone** - Audio input monitoring
- ✅ **Real-time VU Meter** - Mic level visualization

**Fitur Utama:**
- 📺 OLED display untuk status & monitoring
- 🔊 Speaker test dengan beep (440Hz & 523Hz)
- 🎤 Microphone monitoring real-time
- 📊 Visual bar graph di OLED
- 🔘 Button control (BOOT = beep test)
- 📈 Serial Monitor output dengan emoji

**Sistem Yang Ditest:**
1. OLED I2C Display (SSD1306)
2. I2S Audio Output (MAX98357A)
3. I2S Audio Input (INMP441)
4. GPIO Button (BOOT button)

---

## 🛠️ Hardware yang Dibutuhkan

1. Goouuuu ESP32-S3 Board
2. OLED Display 0.91" (128x32)
3. Goouuu Shield  ( MAX98357A I2S Audio Amplifier dan INMP441 I2S Microphone integrated on board)
4. Speaker
5. Kabel USB

---

## 🔌 Koneksi Hardware

### 🔌 Koneksi Lengkap

#### **A. OLED Display 0.91" → ESP32-S3**
```
OLED Pin     →    ESP32-S3 Pin
─────────────────────────────
VCC          →    3.3V
GND          →    GND
SCL          →    GPIO 42
SDA          →    GPIO 41
```

#### **B. MAX98357A Speaker → ESP32-S3**
```
MAX98357A    →    ESP32-S3 Pin
─────────────────────────────
VIN          →    5V
GND          →    GND
DIN          →    GPIO 7
BCLK         →    GPIO 15
LRC          →    GPIO 16
```

#### **C. INMP441 Microphone → ESP32-S3**
```
INMP441      →    ESP32-S3 Pin
─────────────────────────────
VDD          →    3.3V
GND          →    GND
WS           →    GPIO 4
SCK          →    GPIO 5
SD           →    GPIO 6
L/R          →    GND  ⚠️ PENTING!
```

#### **D. Speaker → MAX98357A**
```
MAX98357A    →    Speaker
─────────────────────────
+            →    Speaker + (Merah)
-            →    Speaker - (Hitam)
```



## 💻 Instalasi Software

### Langkah 1: Install Arduino IDE
1. Download **Arduino IDE 2.x** dari: https://www.arduino.cc/en/software
2. Install sesuai sistem operasi

### Langkah 2: Install Board ESP32-S3
1. Buka Arduino IDE
2. **File** → **Preferences**
3. **Additional Boards Manager URLs**:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. **Tools** → **Board** → **Boards Manager**
5. Cari "**esp32**" → **Install**

### Langkah 3: Install Library yang Dibutuhkan

#### **A. Adafruit GFX Library**
1. **Tools** → **Manage Libraries**
2. Cari "**Adafruit GFX**"
3. **Install**

#### **B. Adafruit SSD1306 Library**
1. Masih di Library Manager
2. Cari "**Adafruit SSD1306**"
3. **Install** (+ dependencies jika diminta)

✅ **Library yang dibutuhkan:**
- **Adafruit GFX Library**
- **Adafruit SSD1306**
- **Wire** (built-in)
- **driver/i2s.h** (built-in ESP32)

---

## ⬆️ Cara Upload Program

### Langkah 1: Buka File Program
1. Buka **`04-ESP32-S3-VoiceBeep-display0_91.ino`**
2. Atau copy-paste kode

### Langkah 2: Pilih Board
1. **Tools** → **Board** → **ESP32 Arduino**
2. Pilih **ESP32S3 Dev Module**

### Langkah 3: Konfigurasi Board
```
Tools →
  ├─ USB CDC On Boot:      "Enabled"
  ├─ CPU Frequency:        "240MHz"
  ├─ Flash Mode:           "QIO 80MHz"
  ├─ Flash Size:           "16MB"
  ├─ Partition Scheme:     "Default 4MB"
  └─ Upload Speed:         "921600"
```

### Langkah 4: Pilih Port
1. Hubungkan ESP32-S3 via USB
2. **Tools** → **Port** → Pilih port

### Langkah 5: Upload!
1. Klik **Upload** (→)
2. Jika error: Tekan **BOOT** saat upload
3. Tunggu selesai

---

## 🎮 Cara Menggunakan

### Fase 1: Initialization (Boot Sequence)

Setelah upload, program akan menjalankan test sequence:

#### **Serial Monitor Output:**
```
╔════════════════════════════════════════════╗
║   ESP32-S3 Complete Hardware Test         ║
║   OLED I2C 0.91" 128x32 Version          ║
╚════════════════════════════════════════════╝

Initializing OLED... OK!
Initializing Speaker... OK!
Testing speaker (beep)...
Initializing Microphone... OK!

╔════════════════════════════════════════════╗
║  ALL SYSTEMS READY!                       ║
║  - Speak/clap to test microphone          ║
║  - Press BOOT button for beep test        ║
╚════════════════════════════════════════════╝
```

#### **OLED Display Sequence:**
```
1. "ESP32-S3 HW TEST v1.0"  (1 detik)
2. "Speaker: OK"             (0.5 detik)
3. "Spk Test: Beeping..."    (saat test beep)
4. "Spk Test: Done!"         (0.5 detik)
5. "Mic: OK"                 (0.5 detik)
6. "MONITORING"              (permanent)
   "Speak/BOOT test"
   [====Bar====]  ← Mic level bar
```

### Fase 2: Monitoring Mode

#### **OLED Display:**
```
┌──────────────────────┐
│ MONITORING           │
│ Speak/BOOT test      │
│ [████████░░░░░░░░]   │ ← Real-time mic level
└──────────────────────┘
```

#### **Serial Monitor (Real-time):**
```
[█████████░░░░░░░] Level:  245 🔇 Silent
[████████████████] Level:  680 🔊 Normal
[████████████████████] Level: 1120 📢 LOUD!
```

### Fase 3: Button Control

**Tekan BOOT button:**
- OLED: "Beeping..."
- Speaker: Beep 440Hz + 523Hz
- OLED: Kembali ke monitoring

---

## 🔧 Troubleshooting

### ❌ Problem: OLED tidak menyala

**Solusi:**
1. ✅ Cek koneksi I2C (GPIO 41 & 42)
2. ✅ Coba ganti I2C address:
   ```cpp
   #define OLED_ADDRESS 0x3C  // ← Coba 0x3D
   ```
3. ✅ Upload I2C Scanner untuk cek address
4. ✅ Pastikan library terinstall

---

### ❌ Problem: Speaker tidak berbunyi

**Solusi:**
1. ✅ Cek koneksi MAX98357A (GPIO 7, 15, 16)
2. ✅ Cek speaker terhubung ke +/-
3. ✅ Cek power 5V ke MAX98357A
4. ✅ Pastikan tidak pakai headphone

---

### ❌ Problem: Microphone tidak respon

**Solusi:**
1. ✅ **PENTING:** Pin L/R INMP441 HARUS ke GND!
2. ✅ Cek koneksi GPIO 4, 5, 6
3. ✅ Cek power 3.3V ke INMP441
4. ✅ Tunggu 5-10 detik setelah boot (warmup)

---

### ❌ Problem: Mic level bar tidak bergerak

**Solusi:**
1. ✅ Bicara/tepuk lebih keras
2. ✅ Cek L/R pin terhubung ke GND
3. ✅ Lihat Serial Monitor - apakah level berubah?
4. ✅ Tunggu setelah speaker beep (flush buffer)

---

### ❌ Problem: Display menampilkan karakter aneh

**Solusi:**
1. ✅ Reset ESP32-S3 (tekan RST)
2. ✅ Tambahkan delay di initOLED():
   ```cpp
   myWire.begin(OLED_SDA, OLED_SCL);
   delay(100);  // ← Tambah ini
   ```
3. ✅ Cek power supply stabil

---

### ❌ Problem: Serial Monitor tidak muncul

**Solusi:**
1. ✅ Baud rate = **115200**
2. ✅ Pilih port yang benar
3. ✅ Tekan RST di ESP32-S3
4. ✅ Cabut-pasang USB

---

## 🎨 Modifikasi Program

### 1. Ubah Teks Display

#### **Ubah Title:**
```cpp
// Baris 72-75 (di displayTitle)
display.print("ESP32-S3 HW TEST");  // ← Ganti ini
display.setCursor(0, 12);
display.print("v1.0");              // ← Ganti ini
```

#### **Ubah Monitoring Text:**
```cpp
// Baris 93-95 (di displayMonitoring)
display.print("MONITORING");       // ← Ganti baris 1
display.setCursor(0, 12);
display.print("Speak/BOOT test");  // ← Ganti baris 2
```

---

### 2. Ubah Frekuensi Beep

```cpp
// Baris 251-253 (di setup)
playTone(440, 300);  // ← Ubah frekuensi & durasi
delay(200);
playTone(523, 300);  // ← Ubah frekuensi & durasi

// Contoh:
playTone(262, 500);  // C4, 500ms
playTone(330, 500);  // E4, 500ms
playTone(392, 500);  // G4, 500ms
```

**Referensi Nada:**
- C4: 261.63 Hz
- E4: 329.63 Hz
- G4: 392.00 Hz
- A4: 440.00 Hz
- C5: 523.25 Hz

---

### 3. Ubah Volume Speaker

```cpp
// Baris 146 (di playTone)
const int16_t AMP = 30000;  // ← Ubah ini

// Pilihan volume:
const int16_t AMP = 16000;  // 49% - Normal
const int16_t AMP = 24000;  // 73% - Keras
const int16_t AMP = 30000;  // 92% - Sangat Keras (default)
```

---

### 4. Ubah Mic Level Bar Sensitivity

```cpp
// Baris 104 (di drawMicLevel)
int barWidth = map(level, 0, 1000, 0, 128);
//                         ^^^^  ← Ubah threshold

// Lebih sensitif (bar lebih cepat penuh):
int barWidth = map(level, 0, 500, 0, 128);

// Kurang sensitif (butuh suara lebih keras):
int barWidth = map(level, 0, 1500, 0, 128);
```

---

### 5. Ubah Update Rate Mic Level

```cpp
// Baris 311 (di loop)
if (millis() - last_update > 100) {  // ← Ubah interval
//                            ^^^

// Lebih cepat (bar lebih smooth):
if (millis() - last_update > 50) {

// Lebih lambat (hemat CPU):
if (millis() - last_update > 200) {
```

---

### 6. Tambah Display Info Tambahan

```cpp
// Di displayMonitoring(), setelah baris 95:
display.setCursor(0, 0);
display.print("MONITORING");

display.setCursor(0, 10);
display.print("Mic Level:"); // ← Tambah label

display.setCursor(70, 10);
display.printf("%d", mic_level); // ← Tampilkan angka

// Lalu tambahkan parameter mic_level ke fungsi:
void displayMonitoring(int level) {
  // ...
}
```

---

### 7. Ubah Sample Rate Audio

```cpp
// Baris 38 - Ubah sample rate
#define SAMPLE_RATE   16000  // ← Ubah ini

// Pilihan:
#define SAMPLE_RATE   8000   // 8kHz - voice quality
#define SAMPLE_RATE   16000  // 16kHz - wide band (default)
#define SAMPLE_RATE   22050  // 22kHz - FM radio quality
#define SAMPLE_RATE   44100  // 44.1kHz - CD quality
```

**Catatan:** Sample rate tinggi = kualitas bagus tapi CPU load tinggi.

---

### 8. Tambah Beep Pattern (Melody)

```cpp
// Ganti fungsi test speaker di setup():
void playMelody() {
  int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
  int duration[] = {250, 250, 250, 250, 250, 250, 250, 500};
  
  for(int i = 0; i < 8; i++) {
    playTone(melody[i], duration[i]);
    delay(50);
  }
}

// Panggil di setup():
playMelody();  // Mainkan C major scale
```

---

## 📊 Spesifikasi Hardware

### OLED Display (SSD1306)
| Parameter | Nilai |
|-----------|-------|
| Resolution | 128x32 pixels |
| Size | 0.91 inch |
| Interface | I2C (400kHz) |
| Address | 0x3C atau 0x3D |
| Voltage | 3.3V - 5V |
| Current | ~20mA |

### MAX98357A Amplifier
| Parameter | Nilai |
|-----------|-------|
| Output Power | 3.2W @ 4Ω |
| Input Voltage | 2.5V - 5.5V |
| Interface | I2S Digital |
| Sample Rate | 8kHz - 96kHz |
| Efficiency | 92% |

### INMP441 Microphone
| Parameter | Nilai |
|-----------|-------|
| Interface | I2S Digital |
| Sample Rate | 16kHz - 48kHz |
| Bit Depth | 24-bit |
| SNR | 61 dB |
| Sensitivity | -26 dBFS |

---

## 🎓 Belajar Lebih Lanjut

### I2C Protocol (OLED)
- 2-wire communication (SDA & SCL)
- Address-based device identification
- Pull-up resistors required (biasanya built-in)
- Max speed: 400kHz (Fast Mode)

### I2S Protocol (Audio)
- 3-wire digital audio (BCLK, LRCLK, DATA)
- High quality audio transmission
- No DAC/ADC needed
- Low jitter, low noise

### VU Meter Concept
- **VU** = Volume Unit
- Menampilkan level audio secara visual
- Formula: `Average = Sum(|samples|) / N`
- Bar width mapped dari level 0-1000

---

## 🚀 Project Ideas

Setelah berhasil, coba project ini:

1. **Voice Activated Light**
   - LED menyala saat deteksi suara
   - Threshold adjustable

2. **Audio Spectrum Analyzer**
   - FFT processing
   - Display frequency bands di OLED

3. **Voice Recorder**
   - Record ke SD card
   - Playback dari SD

4. **Doorbell System**
   - Button → play melody
   - Display notification

5. **Sound Level Meter**
   - Measure dB level
   - Log ke SD card

6. **Voice Command**
   - Speech recognition
   - Control relay/LED

---

## 📚 Referensi & Resources

### Datasheet:
- **SSD1306**: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
- **MAX98357A**: https://datasheets.maximintegrated.com/en/ds/MAX98357A.pdf
- **INMP441**: https://invensense.tdk.com/products/digital/inmp441/

### Library Documentation:
- **Adafruit GFX**: https://learn.adafruit.com/adafruit-gfx-graphics-library
- **Adafruit SSD1306**: https://learn.adafruit.com/monochrome-oled-breakouts
- **ESP32 I2S**: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/i2s.html

---

## 📝 Changelog

**v1.0** (Current)
- ✅ OLED Display 0.91" (128x32)
- ✅ MAX98357A Speaker Test
- ✅ INMP441 Microphone Monitoring
- ✅ Real-time VU Meter bar
- ✅ BOOT button beep test
- ✅ Serial Monitor output
- ✅ Complete hardware integration

---

## ⚖️ License

Program ini gratis untuk digunakan untuk keperluan edukasi dan development.

---

**Selamat mencoba! Semoga semua hardware-nya berfungsi! 🎤📺🔊**

*Complete hardware test ini memastikan semua komponen audio dan display bekerja dengan baik sebelum digunakan untuk project yang lebih kompleks!*

copyright by hwthinker
