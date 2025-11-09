# 📺 ESP32-S3 OLED Display 0.91" Test - Panduan Lengkap untuk Pemula

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

Program ini adalah **tes display OLED 0.91"** (128x32 pixels) untuk ESP32-S3 yang akan:
- ✅ Mengecek apakah display OLED terpasang dengan benar
- ✅ Menampilkan teks "Hello" di baris pertama
- ✅ Menampilkan teks "ESP32-S3" di baris kedua
- ✅ Menggunakan I2C protokol untuk komunikasi

**Fitur Utama:**
- 📺 Display OLED 0.91 inch (128x32)
- 🔌 I2C communication (2 kabel data saja!)
- 📝 Text size 2 (optimal untuk layar kecil)
- 🎨 Monochrome (hitam-putih)
- ⚡ Low power consumption

---

## 🛠️ Hardware yang Dibutuhkan

1. Goouuu ESP32-S3 Board
1. OLED Display 0.91" (128x32)
1. Goouuu Shield ESP32-S3
1. Kabel USB

---

## 🔌 Koneksi Hardware

### Diagram Koneksi OLED 0.91" ke ESP32-S3

```
OLED 0.91"    →    ESP32-S3 Pin
─────────────────────────────────
VCC / VDD     →    3.3V  (atau 5V jika display support)
GND           →    GND
SCL / SCK     →    GPIO 42
SDA           →    GPIO 41
```

### ⚠️ **PENTING - I2C Address!**
- Default address program: **0x3C**
- Beberapa OLED pakai address **0x3D**
- Jika display tidak muncul, coba ganti address

### 🔍 Cara Cek I2C Address:
Upload sketch **I2C Scanner** untuk cek address:

```cpp
#include <Wire.h>

TwoWire myWire = TwoWire(0);

void setup() {
  Serial.begin(115200);
  myWire.begin(41, 42);  // SDA, SCL
  
  Serial.println("I2C Scanner");
  for(byte address = 1; address < 127; address++) {
    myWire.beginTransmission(address);
    if (myWire.endTransmission() == 0) {
      Serial.print("Found I2C device at 0x");
      Serial.println(address, HEX);
    }
  }
}

void loop() {}
```

---

## 💻 Instalasi Software

### Langkah 1: Install Arduino IDE
1. Download **Arduino IDE 2.x** dari: https://www.arduino.cc/en/software
2. Install sesuai sistem operasi (Windows/Mac/Linux)

### Langkah 2: Install Board ESP32-S3
1. Buka Arduino IDE
2. Klik **File** → **Preferences**
3. Di bagian **Additional Boards Manager URLs**, tambahkan:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Klik **OK**
5. Buka **Tools** → **Board** → **Boards Manager**
6. Cari "**esp32**" oleh Espressif Systems
7. Klik **Install** (versi 2.0.0 atau lebih baru)

### Langkah 3: Install Library yang Dibutuhkan

Program ini memerlukan 2 library:

#### **A. Adafruit GFX Library**
1. Buka **Tools** → **Manage Libraries** (atau Ctrl+Shift+I)
2. Cari "**Adafruit GFX**"
3. Pilih **Adafruit GFX Library by Adafruit**
4. Klik **Install**

#### **B. Adafruit SSD1306 Library**
1. Masih di Library Manager
2. Cari "**Adafruit SSD1306**"
3. Pilih **Adafruit SSD1306 by Adafruit**
4. Klik **Install**
5. Jika diminta install dependencies, klik **Install All**

✅ **Library yang dibutuhkan:**
- **Adafruit GFX Library** (versi 1.11.0 atau lebih baru)
- **Adafruit SSD1306** (versi 2.5.0 atau lebih baru)
- **Wire** (built-in, tidak perlu install)

---

## ⬆️ Cara Upload Program

### Langkah 1: Buka File Program
1. Buka file **`02-esp32-S3-display-test-display0_91.ino`** di Arduino IDE
2. Atau copy-paste kode ke sketch baru

### Langkah 2: Pilih Board
1. Klik **Tools** → **Board** → **ESP32 Arduino**
2. Pilih **ESP32S3 Dev Module**

### Langkah 3: Konfigurasi Board
Set konfigurasi seperti ini:
```
Tools →
  ├─ USB CDC On Boot:      "Enabled"
  ├─ CPU Frequency:        "240MHz (WiFi)"
  ├─ Flash Mode:           "QIO 80MHz"
  ├─ Flash Size:           "16MB (128Mb)" (sesuaikan board Anda)
  ├─ Partition Scheme:     "Default 4MB with spiffs"
  ├─ PSRAM:                "Disabled" (atau "OPI PSRAM" jika ada)
  └─ Upload Speed:         "921600"
```

### Langkah 4: Pilih Port
1. Hubungkan ESP32-S3 ke komputer via USB
2. **Tools** → **Port** → Pilih port yang muncul
   - Windows: `COM3`, `COM4`, dll
   - Mac/Linux: `/dev/ttyUSB0`, `/dev/cu.usbserial-xxx`

### Langkah 5: Upload!
1. Klik tombol **Upload** (ikon panah →)
2. Tunggu hingga proses selesai
3. Jika ada error "Failed to connect":
   - Tekan dan tahan tombol **BOOT** di ESP32-S3
   - Klik **Upload** lagi
   - Lepas tombol BOOT setelah "Connecting..."

---

## 🎮 Cara Menggunakan

### Yang Akan Terjadi Setelah Upload:

Setelah upload berhasil, display OLED akan langsung menampilkan:

```
┌──────────────────────┐
│ Hello                │  ← Baris 1 (Text Size 2)
│ ESP32-S3             │  ← Baris 2 (Text Size 2)
└──────────────────────┘
```

**Ciri-ciri display berfungsi dengan baik:**
- ✅ Teks terlihat jelas dan terang
- ✅ Tidak ada pixel yang mati
- ✅ Tidak ada noise/gangguan
- ✅ Background hitam sempurna

### 📝 Catatan:
- Program ini menampilkan teks **statis** (tidak berubah)
- `loop()` kosong, jadi tidak ada animasi
- Display akan tetap menyala selama ESP32 mendapat power

---

## 🔧 Troubleshooting

### ❌ Problem: Display tidak menyala sama sekali

**Kemungkinan Penyebab & Solusi:**

1. **Koneksi salah**
   - ✅ Cek koneksi VCC ke 3.3V (atau 5V)
   - ✅ Cek koneksi GND ke GND
   - ✅ Cek koneksi SCL ke GPIO 42
   - ✅ Cek koneksi SDA ke GPIO 41
   - ✅ Pastikan tidak ada kabel yang longgar

2. **I2C Address salah**
   - Display Anda mungkin pakai address **0x3D**
   - Ganti di kode baris 10:
   ```cpp
   #define OLED_ADDRESS 0x3C  // ← Coba ganti ke 0x3D
   ```
   - Upload ulang

3. **Display rusak**
   - Cek dengan multimeter, VCC harus 3.3V atau 5V
   - Coba display lain jika ada

4. **Library tidak terinstall**
   - Pastikan library Adafruit GFX dan SSD1306 sudah terinstall
   - Cek di **Tools** → **Manage Libraries**

---

### ❌ Problem: Teks tidak jelas / kabur

**Solusi:**
1. **Adjust brightness** (tambahkan di setup):
   ```cpp
   display.dim(false);  // Full brightness
   // atau:
   display.dim(true);   // Dim mode
   ```

2. **Cek contrast** (beberapa OLED punya trimpot contrast di belakang):
   - Putar trimpot dengan obeng kecil

---

### ❌ Problem: Display menampilkan karakter aneh

**Solusi:**
1. **Reset display** dengan menambahkan delay:
   ```cpp
   void setup() {
     myWire.begin(OLED_SDA, OLED_SCL);
     delay(100);  // ← Tambah delay sebelum init
     
     if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
       while(1);
     }
     // ...
   ```

2. **Clear buffer sebelum display**:
   ```cpp
   display.clearDisplay();  // Clear semua pixel
   display.display();       // Update display kosong dulu
   delay(100);
   // Baru tampilkan text
   ```

---

### ❌ Problem: Beberapa pixel mati/putih random

**Kemungkinan Penyebab:**
1. **Noise pada I2C line**
   - Gunakan kabel jumper pendek (<15cm)
   - Atau solder langsung

2. **Power supply tidak stabil**
   - Tambahkan capacitor 10µF di VCC-GND OLED
   - Gunakan power supply yang bagus

3. **Pull-up resistor kurang**
   - Beberapa OLED butuh pull-up 4.7kΩ di SDA & SCL
   - Atau aktifkan internal pull-up:
   ```cpp
   pinMode(OLED_SDA, INPUT_PULLUP);
   pinMode(OLED_SCL, INPUT_PULLUP);
   myWire.begin(OLED_SDA, OLED_SCL);
   ```

---

### ❌ Problem: Upload gagal "Failed to connect"

**Solusi:**
1. Tekan dan tahan tombol **BOOT** di ESP32-S3
2. Klik **Upload** di Arduino IDE
3. Lepas BOOT setelah muncul "Connecting..."
4. Tunggu proses selesai

---

### ❌ Problem: Display hanya menyala setengah

**Solusi:**
Kemungkinan salah pilih driver di kode. Coba ganti inisialisasi:

```cpp
// Jika pakai SSD1306 128x32:
display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);

// Atau coba:
display.begin(SSD1306_EXTERNALVCC, OLED_ADDRESS);
```

---

## 🎨 Modifikasi Program

### 1. Ubah Teks yang Ditampilkan

```cpp
// Di setup(), baris 32-36:
display.setCursor(0, 0);
display.print("Hello");       // ← Ganti text ini

display.setCursor(0, 16);
display.print("ESP32-S3");    // ← Ganti text ini
```

**Contoh:**
```cpp
display.setCursor(0, 0);
display.print("PENS");

display.setCursor(0, 16);
display.print("Surabaya");
```

---

### 2. Ubah Ukuran Font

```cpp
// Baris 27 - Ubah text size
display.setTextSize(2);  // ← Ubah angka ini

// Pilihan:
display.setTextSize(1);  // Kecil (8x8 pixels per karakter)
display.setTextSize(2);  // Sedang (16x16) ✅ Default
display.setTextSize(3);  // Besar (24x24) - Hanya muat 5 karakter
```

**Catatan:** Display 128x32 terbatas!
- Text size 1: Muat ~21 karakter per baris, 4 baris
- Text size 2: Muat ~10 karakter per baris, 2 baris
- Text size 3: Muat ~5 karakter per baris, 1 baris

---

### 3. Ubah Posisi Teks

```cpp
// setCursor(x, y) - x horizontal, y vertical
display.setCursor(0, 0);    // Kiri atas
display.setCursor(64, 0);   // Tengah atas
display.setCursor(0, 16);   // Kiri tengah
display.setCursor(50, 20);  // Custom position
```

**Koordinat:**
- **X**: 0 (kiri) - 127 (kanan)
- **Y**: 0 (atas) - 31 (bawah)

---

### 4. Tambah Baris Ketiga (Text Size 1)

Karena display tinggi 32 pixel, bisa tampilkan 4 baris text size 1:

```cpp
void setup() {
  myWire.begin(OLED_SDA, OLED_SCL);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while(1);
  }

  display.clearDisplay();
  display.setTextSize(1);           // ← Ganti ke size 1
  display.setTextColor(WHITE);
  display.setTextWrap(false);

  // Baris 1
  display.setCursor(0, 0);
  display.print("ESP32-S3");
  
  // Baris 2
  display.setCursor(0, 10);
  display.print("OLED 0.91\"");
  
  // Baris 3
  display.setCursor(0, 20);
  display.print("128x32 Pixels");
  
  display.display();
}
```

---

### 5. Tambah Animasi Text (Blink)

Ganti `loop()` dengan:

```cpp
void loop() {
  // Clear display
  display.clearDisplay();
  display.display();
  delay(500);  // OFF 500ms
  
  // Show text
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Hello");
  display.setCursor(0, 16);
  display.print("ESP32-S3");
  display.display();
  delay(500);  // ON 500ms
}
```

---

### 6. Tambah Scrolling Text

```cpp
void setup() {
  myWire.begin(OLED_SDA, OLED_SCL);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while(1);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Hello");
  display.setCursor(0, 16);
  display.print("World!");
  display.display();
  
  delay(2000);
  
  // Start scrolling
  display.startscrollright(0x00, 0x0F);  // Scroll right
  // Atau:
  // display.startscrollleft(0x00, 0x0F);  // Scroll left
}

void loop() {
  // Biarkan kosong, scrolling otomatis
}
```

---

### 7. Tampilkan Animasi Sederhana (Bouncing Ball)

```cpp
void setup() {
  myWire.begin(OLED_SDA, OLED_SCL);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while(1);
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  static int x = 0;
  static int dx = 2;
  
  display.clearDisplay();
  
  // Draw ball (circle)
  display.fillCircle(x, 16, 5, WHITE);
  
  // Update position
  x += dx;
  if (x >= 128 || x <= 0) {
    dx = -dx;  // Bounce
  }
  
  display.display();
  delay(20);
}
```

---

### 8. Tampilkan Grafik/Bentuk

```cpp
void setup() {
  myWire.begin(OLED_SDA, OLED_SCL);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while(1);
  }

  display.clearDisplay();
  
  // Rectangle
  display.drawRect(10, 5, 40, 22, WHITE);
  
  // Filled Rectangle
  display.fillRect(60, 5, 40, 22, WHITE);
  
  // Circle
  display.drawCircle(30, 16, 8, WHITE);
  
  // Line
  display.drawLine(0, 0, 127, 31, WHITE);
  
  // Triangle
  display.drawTriangle(100, 5, 90, 27, 110, 27, WHITE);
  
  display.display();
}

void loop() {}
```

---

## 📊 Spesifikasi OLED 0.91" (SSD1306)

### Karakteristik Display:
| Parameter | Nilai | Keterangan |
|-----------|-------|------------|
| **Resolution** | 128x32 | Pixels |
| **Size** | 0.91 inch | Diagonal |
| **Driver IC** | SSD1306 | OLED controller |
| **Interface** | I2C | 2-wire (SDA, SCL) |
| **I2C Address** | 0x3C atau 0x3D | Tergantung module |
| **Voltage** | 3.3V - 5V | Wide voltage range |
| **Current** | ~20mA | Active display |
| **Colors** | Monochrome | White or Blue |
| **Viewing Angle** | >160° | Very wide |
| **Contrast Ratio** | >2000:1 | Excellent |

### Pin Configuration:
| Pin | Function | Notes |
|-----|----------|-------|
| VCC | Power | 3.3V or 5V |
| GND | Ground | 0V |
| SCL | I2C Clock | GPIO 42 (default) |
| SDA | I2C Data | GPIO 41 (default) |

### I2C Speed:
- **Standard Mode**: 100 kHz
- **Fast Mode**: 400 kHz (default)
- **Fast Mode Plus**: 1 MHz (beberapa module support)

---

## 🎓 Belajar Lebih Lanjut

### Memahami Coordinate System:

OLED 128x32 menggunakan sistem koordinat:
```
  0,0 ──────────────────── 127,0
   │                          │
   │      Display Area        │
   │                          │
  0,31 ─────────────────── 127,31
```

- **Origin (0,0)**: Kiri atas
- **X-axis**: 0-127 (horizontal, left to right)
- **Y-axis**: 0-31 (vertical, top to bottom)

### Text Size vs Display Capacity:

| Text Size | Char Width | Char Height | Max Chars/Line | Max Lines |
|-----------|------------|-------------|----------------|-----------|
| 1 | 6px | 8px | ~21 chars | 4 lines |
| 2 | 12px | 16px | ~10 chars | 2 lines |
| 3 | 18px | 24px | ~7 chars | 1 line |

### Drawing Functions:

```cpp
// Text
display.print("text");
display.println("text with newline");

// Shapes
display.drawPixel(x, y, color);
display.drawLine(x0, y0, x1, y1, color);
display.drawRect(x, y, width, height, color);
display.fillRect(x, y, width, height, color);
display.drawCircle(x, y, radius, color);
display.fillCircle(x, y, radius, color);
display.drawTriangle(x0, y0, x1, y1, x2, y2, color);

// Colors: WHITE (1) or BLACK (0)
```

---

## 🚀 Project Ideas

Setelah berhasil, coba project ini:

1. **Real-Time Clock Display**
   - Tampilkan jam & tanggal
   - Pakai RTC module DS3231

2. **Sensor Monitor**
   - Temperature & Humidity (DHT22)
   - Air Quality (MQ-135)
   - Distance (HC-SR04)

3. **Status Indicator**
   - WiFi signal strength
   - Battery level
   - System status

4. **Simple Game**
   - Pong game
   - Snake game
   - Flappy bird

5. **Music Visualizer**
   - FFT spectrum analyzer
   - VU meter display

6. **Menu System**
   - Interactive menu dengan button
   - Setting configuration

---

## 📚 Referensi & Resources

### Datasheet & Documentation:
- **SSD1306 Datasheet**: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
- **Adafruit GFX Guide**: https://learn.adafruit.com/adafruit-gfx-graphics-library
- **Adafruit SSD1306 Guide**: https://learn.adafruit.com/monochrome-oled-breakouts

### Tutorial & Examples:
- **Arduino Examples**: File → Examples → Adafruit SSD1306
- **GFX Examples**: File → Examples → Adafruit GFX Library

---

## 📝 Changelog

**v1.0** (Current)
- ✅ Basic text display (2 lines)
- ✅ Text size 2 (optimal for 0.91")
- ✅ I2C custom pins (GPIO 41, 42)
- ✅ Static display (no animation)
- ✅ Clear & simple code for beginners

---

## ⚖️ License

Program ini gratis untuk digunakan untuk keperluan edukasi dan development.

---

**Selamat mencoba! Semoga display-nya nyala! 📺✨**

*Jika program berjalan dengan baik, Anda siap untuk membuat project display yang lebih kompleks seperti sensor monitor, status indicator, atau mini games!*

copyright by hwthinker
