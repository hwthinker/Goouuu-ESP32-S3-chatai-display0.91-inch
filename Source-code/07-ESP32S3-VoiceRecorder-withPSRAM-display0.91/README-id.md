# ESP32-S3 Voice Recorder dengan PSRAM dan OLED 0.91"

Proyek ini adalah perekam suara menggunakan ESP32-S3 yang memanfaatkan PSRAM untuk menyimpan rekaman audio selama 5 detik dengan kualitas 16kHz. Dilengkapi dengan display OLED 0.91" untuk menampilkan status dan progress.

## 🎯 Fitur Utama

- **Rekaman Audio 5 Detik**: Menggunakan I2S microphone dengan sample rate 16kHz
- **PSRAM Support**: Menggunakan PSRAM 8MB untuk buffer rekaman (960KB)
- **Display OLED 0.91"**: Menampilkan status, timer, dan progress bar
- **LED WS2812B Indikator**: 
  - 🔴 **Merah**: Sedang merekam
  - 🟢 **Hijau**: Sedang memutar
  - ⚫ **Mati**: Idle/siap
- **Auto Playback**: Memutar rekaman otomatis setelah selesai merekam
- **Button Delay**: Delay 500ms untuk menghindari suara klik tombol

## 📋 Spesifikasi Teknis

### Hardware
- **MCU**: ESP32-S3
- **Display**: OLED I2C 0.91" 128x32
- **Microphone**: I2S MEMS Microphone
- **Speaker**: I2S Speaker/Amplifier
- **LED**: WS2812B (NeoPixel)
- **Button**: BOOT Button (GPIO 0)

### Audio Settings
- **Sample Rate**: 16,000 Hz
- **Recording Time**: 5 detik
- **Buffer Size**: 512 samples
- **Total Samples**: 80,000 samples (5s × 16kHz)

### Pin Configuration

#### OLED Display (I2C)
- **SDA**: GPIO 41
- **SCL**: GPIO 42
- **Address**: 0x3C

#### Audio Pins
- **Speaker DOUT**: GPIO 7
- **Speaker BCLK**: GPIO 15  
- **Speaker LRC**: GPIO 16
- **Microphone WS**: GPIO 4
- **Microphone SCK**: GPIO 5
- **Microphone SD**: GPIO 6

#### LED
- **WS2812B**: GPIO 48

## 🛠️ Instalasi dan Setup

### Prerequisites
1. **Arduino IDE** dengan ESP32 board support
2. **Library yang diperlukan**:
   - `Adafruit_GFX`
   - `Adafruit_SSD1306`
   - `Adafruit_NeoPixel`

### Konfigurasi Arduino IDE

1. **Pilih Board**: 
   - Tools > Board > ESP32 Arduino > "ESP32S3 Dev Module"

2. **Enable PSRAM** (PENTING!):
   - Tools > PSRAM > "OPI PSRAM"

3. **Partition Scheme**:
   - Tools > Partition Scheme > "Huge APP (3MB No OTA/1MB SPIFFS)"

4. **Upload Speed**: 921600

### Wiring Diagram

```
ESP32-S3 Connections:

OLED Display (I2C):
  SDA  → GPIO 41
  SCL  → GPIO 42
  VCC  → 3.3V
  GND  → GND

I2S Microphone:
  WS   → GPIO 4
  SCK  → GPIO 5
  SD   → GPIO 6
  VCC  → 3.3V
  GND  → GND

I2S Speaker:
  DOUT → GPIO 7
  BCLK → GPIO 15
  LRC  → GPIO 16
  VCC  → 5V
  GND  → GND

WS2812B LED:
  DATA → GPIO 48
  VCC  → 3.3V
  GND  → GND

Button:
  BOOT → GPIO 0 (internal pull-up)
```

## 🚀 Cara Penggunaan

1. **Upload kode** ke ESP32-S3
2. **Buka Serial Monitor** (115200 baud) untuk melihat status
3. **Tekan BOOT button** untuk memulai rekaman
4. **Tunggu 500ms** (delay untuk skip button click)
5. **Rekaman dimulai** selama 5 detik
6. **Playback otomatis** setelah rekaman selesai

### Status Display

**Idle Screen**:
```
VOICE REC 5s
READY - Press BOOT
PSRAM Ready
```

**Waiting**:
```
GET READY...
0.5s
```

**Recording**:
```
RECORDING 5s
4s
```

**Playing**:
```
PLAYING 4.8s
75%
[==========      ]
```

## 🔧 Troubleshooting

### PSRAM Tidak Terdeteksi
```
❌ PSRAM not found!
   Please enable PSRAM in Arduino IDE:
   Tools > PSRAM > OPI PSRAM
```
**Solusi**: Pastikan PSRAM di-enable di Arduino IDE

### Buffer Allocation Failed
```
❌ Failed to allocate recording buffer!
```
**Solusi**: 
- Pastikan PSRAM aktif
- Coba restart Arduino IDE
- Gunakan partition scheme yang sesuai

### OLED Tidak Menyala
- Periksa koneksi SDA/SCL
- Pastikan address OLED 0x3C
- Cek kabel power 3.3V

## 📊 Memory Usage

- **PSRAM Required**: 960KB (80,000 samples × 2 bytes)
- **Heap Memory**: ~200KB untuk program
- **Total PSRAM Available**: 8MB

## 🎵 Audio Quality

- **Format**: 16-bit PCM
- **Sample Rate**: 16,000 Hz
- **Channels**: Mono (recording), Stereo (playback)
- **Dynamic Range**: ±16,000

## 📁 File Structure

```
07-ESP32S3-VoiceRecorder-withPSRAM-display0.91/
├── 07-ESP32S3-VoiceRecorder-withPSRAM-display0.91.ino
└── README.md
```

## 🔄 State Machine

Proyek menggunakan state machine dengan 4 state:
1. **IDLE**: Menunggu input tombol
2. **WAITING**: Delay 500ms sebelum rekaman
3. **RECORDING**: Sedang merekam audio
4. **PLAYING**: Memutar rekaman

## 📝 License

Proyek ini open source dan dapat digunakan untuk keperluan edukasi dan komersial.

## 🤝 Kontribusi

Silakan fork dan submit pull request untuk perbaikan atau fitur tambahan.

---

**Dibuat dengan ❤️ untuk komunitas ESP32 Indonesia**