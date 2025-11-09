/*
 * ESP32-S3 Complete Hardware Test - NeoPixel Version
 * Modified for OLED I2C 0.91" 128x32
 * Tests: OLED + Speaker + Microphone + WS2812B (using NeoPixel lib)
 * 
 * OLED Connection (I2C):
 * - SDA: GPIO 41
 * - SCL: GPIO 42
 * - Address: 0x3C
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <driver/i2s.h>
#include <math.h>

// ========== OLED SETTINGS ==========
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C
#define OLED_SDA 41
#define OLED_SCL 42

// ========== SPEAKER PINS ==========
#define SPEAKER_DOUT  7
#define SPEAKER_BCLK  15
#define SPEAKER_LRC   16

// ========== MICROPHONE PINS ==========
#define MIC_WS        4
#define MIC_SCK       5
#define MIC_SD        6

// ========== WS2812B LED (NeoPixel) ==========
#define LED_PIN       48
#define NUM_LEDS      1

// ========== I2S CONFIGURATION ==========
#define SAMPLE_RATE   16000
#define I2S_SPEAKER   I2S_NUM_0
#define I2S_MIC       I2S_NUM_1

// ========== OBJECTS ==========
TwoWire myWire = TwoWire(0);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &myWire, OLED_RESET);
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ========== VARIABLES ==========
int32_t mic_buffer[512];
int16_t speaker_buffer[512];
bool speaker_playing = false;
float tone_freq = 440.0f;
double phase = 0.0;
int led_mode = 3;  // 0=off, 1=rainbow, 2=breathing, 3=mic reactive

// ========== OLED FUNCTIONS ==========
void initOLED() {
  // Inisialisasi I2C dengan pin khusus
  myWire.begin(OLED_SDA, OLED_SCL);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  display.display();
}

void displayTitle() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("ESP32-S3 HW TEST");
  display.setCursor(0, 12);
  display.print("Initializing...");
  display.display();
}

void displayMonitoring() {
  // Display header only once
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("MONITORING");
  display.display();
}

void drawMicLevel(int level) {
  // Clear bottom area for level display
  display.fillRect(0, 16, 128, 16, SSD1306_BLACK);
  
  // Show LED mode
  display.setTextSize(1);
  display.setCursor(0, 16);
  display.print("LED:");
  switch(led_mode) {
    case 0: display.print("OFF"); break;
    case 1: display.print("Rain"); break;
    case 2: display.print("Brth"); break;
    case 3: display.print("Mic"); break;
  }
  
  // Mic level bar
  int barWidth = map(level, 0, 1000, 0, 128);
  barWidth = constrain(barWidth, 0, 128);
  
  // Progress bar at bottom
  display.drawRect(0, 28, 128, 4, SSD1306_WHITE);
  if (barWidth > 0) {
    display.fillRect(0, 28, barWidth, 4, SSD1306_WHITE);
  }
  
  display.display();
}

// ========== LED FUNCTIONS (NeoPixel) ==========
void initLED() {
  Serial.print("Initializing WS2812B (NeoPixel) on GPIO 48... ");
  
  strip.begin();
  strip.setBrightness(80);  // 0-255
  strip.clear();
  strip.show();
  
  delay(200);
  
  Serial.println("OK!");
  Serial.println("Testing LED: R-G-B-W...");
  
  // Red
  strip.setPixelColor(0, strip.Color(255, 0, 0));
  strip.show();
  delay(300);
  
  // Green
  strip.setPixelColor(0, strip.Color(0, 255, 0));
  strip.show();
  delay(300);
  
  // Blue
  strip.setPixelColor(0, strip.Color(0, 0, 255));
  strip.show();
  delay(300);
  
  // White
  strip.setPixelColor(0, strip.Color(255, 255, 255));
  strip.show();
  delay(300);
  
  // Off
  strip.setPixelColor(0, strip.Color(0, 0, 0));
  strip.show();
  
  Serial.println("LED test complete!");
}

// Helper function: Convert HSV to RGB
void setPixelColorHSV(uint16_t hue, uint8_t sat, uint8_t val) {
  uint32_t color = strip.gamma32(strip.ColorHSV(hue * 65536 / 360, sat, val));
  strip.setPixelColor(0, color);
}

void updateLED(int mic_level) {
  static uint16_t hue = 0;
  static uint8_t brightness = 0;
  static bool breathing_up = true;
  
  switch(led_mode) {
    case 0:  // OFF
      strip.setPixelColor(0, strip.Color(0, 0, 0));
      break;
      
    case 1:  // Rainbow cycle
      hue += 2;
      if (hue >= 360) hue = 0;
      setPixelColorHSV(hue, 255, 255);
      break;
      
    case 2:  // Breathing effect (Blue)
      if (breathing_up) {
        brightness += 5;
        if (brightness >= 250) breathing_up = false;
      } else {
        brightness -= 5;
        if (brightness <= 5) breathing_up = true;
      }
      strip.setPixelColor(0, strip.Color(0, 0, brightness));
      break;
      
    case 3:  // Mic reactive
      {
        uint8_t led_brightness = map(mic_level, 0, 1000, 0, 255);
        led_brightness = constrain(led_brightness, 0, 255);
        
        // Color transition: Green (low) -> Yellow (med) -> Red (high)
        uint8_t red, green, blue;
        
        if (mic_level < 500) {
          // Green to Yellow
          red = map(mic_level, 0, 500, 0, 255);
          green = 255;
          blue = 0;
        } else {
          // Yellow to Red
          red = 255;
          green = map(mic_level, 500, 1000, 255, 0);
          blue = 0;
        }
        
        // Apply brightness
        red = (red * led_brightness) / 255;
        green = (green * led_brightness) / 255;
        blue = (blue * led_brightness) / 255;
        
        strip.setPixelColor(0, strip.Color(red, green, blue));
      }
      break;
  }
  
  strip.show();
}

// ========== SPEAKER FUNCTIONS ==========
void initSpeaker() {
  i2s_config_t speaker_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 256,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };
  
  i2s_pin_config_t speaker_pins = {
    .bck_io_num = SPEAKER_BCLK,
    .ws_io_num = SPEAKER_LRC,
    .data_out_num = SPEAKER_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  
  i2s_driver_install(I2S_SPEAKER, &speaker_config, 0, NULL);
  i2s_set_pin(I2S_SPEAKER, &speaker_pins);
  i2s_set_clk(I2S_SPEAKER, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);
}

void playTone(float freq, int duration_ms) {
  speaker_playing = true;
  const int16_t AMP = 27000;
  const double TWOPI = 6.283185307179586;
  double dphi = TWOPI * freq / SAMPLE_RATE;
  
  int total_samples = (SAMPLE_RATE * duration_ms) / 1000;
  int samples_written = 0;
  
  while (samples_written < total_samples) {
    int samples_this_round = min(256, total_samples - samples_written);
    
    for (int i = 0; i < samples_this_round; i++) {
      phase += dphi;
      if (phase >= TWOPI) phase -= TWOPI;
      
      int16_t sample = (int16_t)(AMP * sin(phase));
      speaker_buffer[i * 2] = sample;
      speaker_buffer[i * 2 + 1] = sample;
    }
    
    size_t written;
    i2s_write(I2S_SPEAKER, speaker_buffer, samples_this_round * 4, &written, portMAX_DELAY);
    samples_written += samples_this_round;
  }
  
  speaker_playing = false;
}

// ========== MICROPHONE FUNCTIONS ==========
void initMicrophone() {
  i2s_config_t mic_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };
  
  i2s_pin_config_t mic_pins = {
    .bck_io_num = MIC_SCK,
    .ws_io_num = MIC_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = MIC_SD
  };
  
  i2s_driver_install(I2S_MIC, &mic_config, 0, NULL);
  i2s_set_pin(I2S_MIC, &mic_pins);
  i2s_zero_dma_buffer(I2S_MIC);
  
  for (int i = 0; i < 10; i++) {
    size_t bytes_read;
    i2s_read(I2S_MIC, mic_buffer, sizeof(mic_buffer), &bytes_read, 100);
  }
}

int readMicLevel() {
  size_t bytes_read;
  i2s_read(I2S_MIC, mic_buffer, sizeof(mic_buffer), &bytes_read, 100);
  
  if (bytes_read == 0) return 0;
  
  int samples = bytes_read / sizeof(int32_t);
  int64_t sum = 0;
  
  for (int i = 0; i < samples; i++) {
    int16_t sample = (int16_t)(mic_buffer[i] >> 14);
    sum += abs(sample);
  }
  
  return sum / samples;
}

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════════════╗");
  Serial.println("║   ESP32-S3 Complete Hardware Test v2.1    ║");
  Serial.println("║   OLED I2C 0.91\" 128x32 Version          ║");
  Serial.println("╚════════════════════════════════════════════╝\n");
  
  // Initialize OLED
  Serial.print("Initializing OLED... ");
  initOLED();
  Serial.println("OK!");
  displayTitle();
  delay(1000);
  
  // Initialize Speaker
  Serial.print("Initializing Speaker... ");
  initSpeaker();
  Serial.println("OK!");
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Speaker: OK");
  display.display();
  delay(400);
  
  // Initialize Microphone
  Serial.print("Initializing Microphone... ");
  initMicrophone();
  Serial.println("OK!");
  
  display.setCursor(0, 12);
  display.print("Mic: OK");
  display.display();
  delay(400);
  
  // Initialize LED (NeoPixel)
  initLED();
  
  display.setCursor(0, 24);
  display.print("RGB LED: OK");
  display.display();
  delay(400);
  
  // Test speaker with beep + LED flash
  Serial.println("Testing speaker (beep)...");
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20, 12);
  display.print("Testing...");
  display.display();
  
  strip.setPixelColor(0, strip.Color(255, 255, 0));  // Yellow
  strip.show();
  playTone(440, 300);
  
  strip.setPixelColor(0, strip.Color(0, 255, 255));  // Cyan
  strip.show();
  delay(200);
  playTone(523, 300);
  
  strip.setPixelColor(0, strip.Color(0, 0, 0));  // Off
  strip.show();
  
  delay(400);
  
  // Show monitoring screen
  displayMonitoring();
  
  Serial.println("\n╔════════════════════════════════════════════╗");
  Serial.println("║  ALL SYSTEMS READY!                       ║");
  Serial.println("║  - Speak/clap to test microphone          ║");
  Serial.println("║  - Press BOOT (short) for beep test       ║");
  Serial.println("║  - Hold BOOT (1s) to change LED mode      ║");
  Serial.println("║                                            ║");
  Serial.println("║  LED Modes:                                ║");
  Serial.println("║    0 = OFF                                 ║");
  Serial.println("║    1 = Rainbow                             ║");
  Serial.println("║    2 = Breathing                           ║");
  Serial.println("║    3 = Mic Reactive (VU Meter) ⭐         ║");
  Serial.println("╚════════════════════════════════════════════╝\n");
  
  pinMode(0, INPUT_PULLUP);
}

// ========== LOOP ==========
void loop() {
  static uint32_t last_update = 0;
  static uint32_t last_beep = 0;
  static uint32_t boot_press_time = 0;
  static bool boot_pressed = false;
  static bool long_press_handled = false;
  
  // Check BOOT button
  if (digitalRead(0) == LOW) {
    if (!boot_pressed) {
      boot_pressed = true;
      boot_press_time = millis();
      long_press_handled = false;
    }
    
    // Long press (>1 second) = change LED mode
    if (!long_press_handled && (millis() - boot_press_time > 1000)) {
      led_mode = (led_mode + 1) % 4;
      
      Serial.printf("\n💡 LED Mode changed to: %d\n", led_mode);
      
      // Visual feedback
      strip.setPixelColor(0, strip.Color(255, 255, 255));
      strip.show();
      delay(100);
      strip.setPixelColor(0, strip.Color(0, 0, 0));
      strip.show();
      
      long_press_handled = true;
    }
    
  } else if (boot_pressed) {
    uint32_t press_duration = millis() - boot_press_time;
    
    // Short press (<1 second) = beep test
    if (press_duration < 1000 && !long_press_handled) {
      if (millis() - last_beep > 500) {
        Serial.println("\n🔊 Playing test beep...");
        
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(20, 12);
        display.print("Beeping...");
        display.display();
        
        strip.setPixelColor(0, strip.Color(255, 0, 255));  // Magenta
        strip.show();
        playTone(440, 200);
        
        delay(100);
        
        strip.setPixelColor(0, strip.Color(0, 255, 255));  // Cyan
        strip.show();
        playTone(523, 200);
        
        strip.setPixelColor(0, strip.Color(0, 0, 0));
        strip.show();
        
        displayMonitoring();
        last_beep = millis();
      }
    }
    
    boot_pressed = false;
  }
  
  // Update mic level and LED every 50ms
  if (millis() - last_update > 50) {
    int mic_level = readMicLevel();
    
    // Update OLED display
    drawMicLevel(mic_level);
    
    // Update LED based on mode
    updateLED(mic_level);
    
    // Print to serial every 200ms
    static uint32_t last_serial = 0;
    if (millis() - last_serial > 200) {
      int bar_length = map(mic_level, 0, 1000, 0, 40);
      bar_length = constrain(bar_length, 0, 40);
      
      Serial.print("[");
      for (int i = 0; i < 40; i++) {
        Serial.print(i < bar_length ? "█" : " ");
      }
      Serial.print("] ");
      Serial.printf("Lvl:%4d ", mic_level);
      
      if (mic_level < 100) {
        Serial.print("🔇");
      } else if (mic_level < 300) {
        Serial.print("🔉");
      } else if (mic_level < 600) {
        Serial.print("🔊");
      } else {
        Serial.print("📢");
      }
      
      Serial.printf(" | LED:%d", led_mode);
      Serial.println();
      
      last_serial = millis();
    }
    
    last_update = millis();
  }
  
  delay(10);
}
