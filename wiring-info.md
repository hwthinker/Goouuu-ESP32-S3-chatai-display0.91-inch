# Wiring Info — ESP32-S3 Xiaozhi ChatAI (OLED 0.91")

Dokumen ini merangkum **seluruh pin GPIO** yang dipakai pada folder `Source-code/` (modul 00 s/d 07) dan **GPIO yang masih bebas** untuk pengembangan lanjutan.

Target board: **YD-ESP32-S3** dengan modul **ESP32-S3-WROOM-1 / N16R8** (16 MB QIO Flash + 8 MB OPI PSRAM).

---

## 1. Ringkasan GPIO yang Dipakai

| GPIO | Fungsi                | Komponen / Modul         | Arah    | Catatan                                                  |
|:----:|-----------------------|--------------------------|:-------:|----------------------------------------------------------|
|  0   | BOOT button           | Tombol BOOT on-board     | INPUT   | `INPUT_PULLUP`, juga strapping pin                       |
|  4   | I2S MIC WS            | INMP441 (WS / LRCK)      | OUT     | I2S_NUM_1 RX                                             |
|  5   | I2S MIC SCK           | INMP441 (BCLK)           | OUT     | I2S_NUM_1 RX                                             |
|  6   | I2S MIC SD            | INMP441 (DOUT)           | IN      | I2S_NUM_1 RX                                             |
|  7   | I2S SPK DOUT (DIN)    | MAX98357A (DIN)          | OUT     | I2S_NUM_0 TX                                             |
| 15   | I2S SPK BCLK          | MAX98357A (BCLK)         | OUT     | I2S_NUM_0 TX                                             |
| 16   | I2S SPK LRC (WS)      | MAX98357A (LRC)          | OUT     | I2S_NUM_0 TX                                             |
| 41   | I2C OLED SDA          | SSD1306 0.91" 128×32     | I/O     | `Wire(0)`, alamat `0x3C`                                 |
| 42   | I2C OLED SCL          | SSD1306 0.91" 128×32     | OUT     | `Wire(0)`                                                |
| 48   | WS2812B DIN           | NeoPixel on-board (1 px) | OUT     | FastLED / Adafruit_NeoPixel                              |

Pin tambahan yang **secara implisit dipakai** oleh board (bukan dari sketch, tapi tidak boleh dipakai ulang):

| GPIO     | Fungsi                          | Catatan                                                 |
|:--------:|---------------------------------|---------------------------------------------------------|
| 19, 20   | USB D− / D+                     | USB-OTG / USB-JTAG/Serial native                        |
| 43, 44   | U0TXD / U0RXD                   | Serial Monitor `Serial.begin(115200)` di semua sketch   |
| 26–32    | SPI Flash                       | Reserved internal modul                                 |
| 33–37    | OPI PSRAM (octal)               | Reserved karena modul 07 mengaktifkan **OPI PSRAM**     |

---

## 2. Penggunaan per Modul Source-code

| Modul                                                  | GPIO yang dipakai                          |
|--------------------------------------------------------|--------------------------------------------|
| `00-esp32S3-WS2812BTest`                               | 48                                         |
| `01-esp32S3-SpeakerTest`                               | 7, 15, 16                                  |
| `02-esp32-S3-display-test-display0.91`                 | 41, 42                                     |
| `03-esp32S3-MicTest`                                   | 4, 5, 6                                    |
| `04-ESP32-S3-VoiceBeep-display0.91`                    | 0, 4, 5, 6, 7, 15, 16, 41, 42              |
| `05-ESP32-S3_voiceBeep-withWS2812-display0.91`         | 0, 4, 5, 6, 7, 15, 16, 41, 42, 48          |
| `06-ESP32-S3-VoiceMonitoring-display0.91`              | 0, 4, 5, 6, 7, 15, 16, 41, 42, 48          |
| `07-ESP32S3-VoiceRecorder-withPSRAM-display0.91`       | 0, 4, 5, 6, 7, 15, 16, 41, 42, 48          |

Catatan: header pada `07-...ino` menulis komentar `SDA: GPIO 8 / SCL: GPIO 9`, tetapi **kode aktual** menggunakan `OLED_SDA 41` dan `OLED_SCL 42` (komentar di header sudah usang).

---

## 3. Skema Koneksi (Modul Lengkap 05 / 06 / 07)

```
ESP32-S3                          MAX98357A (Speaker amp)
─────────                          ──────────────────────
GPIO  7  ──────────────────────►  DIN
GPIO 15  ──────────────────────►  BCLK
GPIO 16  ──────────────────────►  LRC
5V       ──────────────────────►  VIN
GND      ──────────────────────►  GND
(SD pin biarkan floating = stereo mix, atau ke GND = mute)

ESP32-S3                          INMP441 (I2S Mic)
─────────                          ─────────────────
GPIO  4  ──────────────────────►  WS
GPIO  5  ──────────────────────►  SCK
GPIO  6  ◄──────────────────────  SD
3V3      ──────────────────────►  VDD
GND      ──────────────────────►  GND, L/R (kiri = LEFT channel)

ESP32-S3                          SSD1306 OLED 0.91" 128x32
─────────                          ──────────────────────────
GPIO 41  ◄────────────────────►  SDA
GPIO 42  ──────────────────────►  SCL
3V3      ──────────────────────►  VCC
GND      ──────────────────────►  GND
                                  (I2C address 0x3C)

ESP32-S3                          WS2812B (on-board NeoPixel)
─────────                          ───────────────────────────
GPIO 48  ──────────────────────►  DIN
```

---

## 4. GPIO yang Masih Bebas

ESP32-S3 menyediakan GPIO `0–21` dan `26–48` (GPIO `22–25` tidak ada di chip ini). Setelah dipotong oleh pin yang sudah terpakai dan oleh resource internal modul, pin yang **aman dipakai** adalah:

### 4.1 Bebas penuh (rekomendasi pertama)

| GPIO | Fitur tambahan                        | Catatan pemakaian                                                                          |
|:----:|---------------------------------------|--------------------------------------------------------------------------------------------|
|  1   | ADC1_CH0, RTC                         | Bebas                                                                                      |
|  2   | ADC1_CH1, RTC                         | Bebas                                                                                      |
|  8   | ADC1_CH7, RTC                         | Bebas (sering dipakai jadi I2C alternatif)                                                 |
|  9   | ADC1_CH8, RTC, FSPIHD                 | Bebas                                                                                      |
| 10   | ADC1_CH9, RTC, FSPICS0                | Bebas                                                                                      |
| 11   | ADC2_CH0, RTC, FSPID (MOSI)           | Bebas                                                                                      |
| 12   | ADC2_CH1, RTC, FSPICLK, TOUCH12       | **Dipakai expansion board** — jangan dipakai ulang sebagai output saat expansion terpasang. |
| 13   | ADC2_CH2, RTC, FSPIQ (MISO)           | Bebas                                                                                      |
| 14   | ADC2_CH3, RTC, FSPIWP                 | Bebas                                                                                      |
| 17   | ADC2_CH6, RTC                         | Bebas                                                                                      |
| 18   | ADC2_CH7, RTC                         | Bebas                                                                                      |
| 21   | —                                     | Bebas                                                                                      |
| 38   | —                                     | Bebas                                                                                      |
| 39   | MTCK (JTAG)                           | Bebas jika JTAG eksternal tidak dipakai                                                    |
| 40   | MTDO (JTAG)                           | Bebas jika JTAG eksternal tidak dipakai                                                    |
| 47   | —                                     | Bebas                                                                                      |

### 4.2 Bebas tapi **strapping pin** (hati-hati saat boot)

| GPIO | Catatan                                                                            |
|:----:|------------------------------------------------------------------------------------|
|  3   | Strapping (JTAG source select). Boleh dipakai, hindari pull-down kuat saat boot.   |
| 45   | Strapping (VDD_SPI voltage). Default pull-down. Hindari sebagai output level HIGH saat reset. |
| 46   | Strapping (boot mode). Default pull-down. Hindari pull-up kuat saat boot.          |

### 4.3 Bebas tapi mengorbankan fitur

| GPIO | Akibat jika dipakai                                                            |
|:----:|--------------------------------------------------------------------------------|
| 19   | Kehilangan USB-JTAG / USB-Serial native (D−). Upload via UART tetap bisa.      |
| 20   | Kehilangan USB-JTAG / USB-Serial native (D+).                                  |
| 43   | Kehilangan `Serial` (U0TXD). Tidak bisa lihat log di Serial Monitor.           |
| 44   | Kehilangan `Serial` (U0RXD).                                                   |

### 4.4 **Tidak boleh dipakai** (selalu di-reserve)

`26, 27, 28, 29, 30, 31, 32` — SPI Flash internal.
`33, 34, 35, 36, 37` — OPI PSRAM (modul `07` mengaktifkan OPI PSRAM via `Tools > PSRAM > OPI PSRAM`). Pin ini hanya kembali bebas kalau PSRAM dimatikan **dan** modul flash bukan tipe Octal.

`22, 23, 24, 25` — tidak ada secara fisik di ESP32-S3.

GPIO `12` — dipakai oleh expansion board (tidak ada di sketch demo `Source-code/` tapi terhubung fisik). Jangan dipakai ulang sebagai output saat expansion terpasang.

---

## 5. Ringkasan Cepat

- **Total GPIO terpakai aktif (sketch demo):** 10 pin → `0, 4, 5, 6, 7, 15, 16, 41, 42, 48`.
- **Total GPIO terpakai sistem (USB + UART):** +4 pin → `19, 20, 43, 44`.
- **GPIO terpakai expansion board:** `12`.
- **GPIO bebas & aman dipakai:** `1, 2, 8, 9, 10, 11, 13, 14, 17, 18, 21, 38, 39, 40, 47`.
- **GPIO bebas dengan catatan strapping:** `3, 45, 46`.
- **Reserved internal modul N16R8:** `26–32` (QIO Flash) + `33–37` (OPI PSRAM).

Untuk penambahan komponen baru (mis. tombol, sensor I2C kedua, encoder, SD card SPI), prioritaskan dulu dari **Bagian 4.1**.
