#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Konfigurasi Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_SDA 41
#define OLED_SCL 42
#define OLED_ADDRESS 0x3C

// Inisialisasi I2C custom
TwoWire myWire = TwoWire(0);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &myWire, -1);

void setup() {
  // Inisialisasi I2C dengan pin khusus
  myWire.begin(OLED_SDA, OLED_SCL);
  
  // Inisialisasi OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while(1); // Stop jika OLED tidak terdeteksi
  }

  // Konfigurasi tampilan
  display.clearDisplay();
  display.setTextSize(2);             // Ukuran font optimal
  display.setTextColor(WHITE);
  display.setTextWrap(false);         // Nonaktifkan word wrap

  // Tampilkan teks 2 baris
  display.setCursor(0, 0);            // Baris 1 (posisi Y 0-15)
  display.print("Hello");
  
  display.setCursor(0, 16);           // Baris 2 (posisi Y 16-31)
  display.print("ESP32-S3");
  
  display.display();
}

void loop() {
  // Kosongkan loop untuk tampilan statis
}