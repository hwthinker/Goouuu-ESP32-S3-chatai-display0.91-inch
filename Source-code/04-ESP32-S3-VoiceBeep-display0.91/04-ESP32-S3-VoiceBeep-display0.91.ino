/*
 * ESP32-S3 Complete Hardware Test
 * Modified for OLED I2C 0.91" 128x32
 * Tests: OLED Display + MAX98357A Speaker + INMP441 Microphone
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
#include <driver/i2s.h>
#include <math.h>

// ========== OLED SETTINGS ==========
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C
#define OLED_SDA 41
#define OLED_SCL 42

// ========== SPEAKER PINS (MAX98357A) ==========
#define SPEAKER_DOUT  7     // DIN
#define SPEAKER_BCLK  15    // BCLK
#define SPEAKER_LRC   16    // LRC

// ========== MICROPHONE PINS (INMP441) ==========
#define MIC_WS        4     // Word Select
#define MIC_SCK       5     // Serial Clock
#define MIC_SD        6     // Serial Data

// ========== I2S CONFIGURATION ==========
#define SAMPLE_RATE   16000
#define I2S_SPEAKER   I2S_NUM_0
#define I2S_MIC       I2S_NUM_1

// ========== OBJECTS ==========
TwoWire myWire = TwoWire(0);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &myWire, OLED_RESET);

// ========== VARIABLES ==========
int32_t mic_buffer[512];
int16_t speaker_buffer[512];
bool speaker_playing = false;
float tone_freq = 440.0f;
double phase = 0.0;

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
  display.print("v1.0");
  display.display();
}

void updateOLEDStatus(const char* device, const char* status) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(device);
  display.setCursor(0, 12);
  display.print(status);
  display.display();
}

void displayMonitoring() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("MONITORING");
  display.setCursor(0, 12);
  display.print("Speak/BOOT test");
  display.display();
}

void drawMicLevel(int level) {
  // Clear bottom area
  display.fillRect(0, 24, 128, 8, SSD1306_BLACK);
  
  // Draw mic level bar at bottom
  int barWidth = map(level, 0, 1000, 0, 128);
  barWidth = constrain(barWidth, 0, 128);
  
  // Draw bar
  display.drawRect(0, 24, 128, 8, SSD1306_WHITE);
  if (barWidth > 0) {
    display.fillRect(1, 25, barWidth - 2, 6, SSD1306_WHITE);
  }
  
  display.display();
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
  const int16_t AMP = 30000;
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
      speaker_buffer[i * 2] = sample;     // Left
      speaker_buffer[i * 2 + 1] = sample; // Right
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
  
  // Flush initial garbage
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
  Serial.println("║   ESP32-S3 Complete Hardware Test         ║");
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
  updateOLEDStatus("Speaker:", "OK");
  delay(500);
  
  // Test speaker with beep
  Serial.println("Testing speaker (beep)...");
  updateOLEDStatus("Spk Test:", "Beeping...");
  playTone(440, 300);  // A4 note for 300ms
  delay(200);
  playTone(523, 300);  // C5 note for 300ms
  updateOLEDStatus("Spk Test:", "Done!");
  delay(500);
  
  // Initialize Microphone
  Serial.print("Initializing Microphone... ");
  initMicrophone();
  Serial.println("OK!");
  updateOLEDStatus("Mic:", "OK");
  delay(500);
  
  // Show monitoring screen
  displayMonitoring();
  
  Serial.println("\n╔════════════════════════════════════════════╗");
  Serial.println("║  ALL SYSTEMS READY!                       ║");
  Serial.println("║  - Speak/clap to test microphone          ║");
  Serial.println("║  - Press BOOT button for beep test        ║");
  Serial.println("╚════════════════════════════════════════════╝\n");
  
  // Configure BOOT button
  pinMode(0, INPUT_PULLUP);
}

// ========== LOOP ==========
void loop() {
  static uint32_t last_update = 0;
  static uint32_t last_beep = 0;
  static bool boot_pressed = false;
  
  // Check BOOT button (GPIO 0)
  if (digitalRead(0) == LOW && !boot_pressed) {
    boot_pressed = true;
    
    if (millis() - last_beep > 1000) {  // Debounce
      Serial.println("🔊 Playing test beep...");
      
      // Show on OLED
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(20, 12);
      display.print("Beeping...");
      display.display();
      
      playTone(440, 200);
      delay(100);
      playTone(523, 200);
      
      // Back to monitoring
      displayMonitoring();
      
      last_beep = millis();
    }
  } else if (digitalRead(0) == HIGH) {
    boot_pressed = false;
  }
  
  // Update mic level every 100ms
  if (millis() - last_update > 100) {
    int mic_level = readMicLevel();
    
    // Update OLED bar
    drawMicLevel(mic_level);
    
    // Print to serial
    int bar_length = map(mic_level, 0, 1000, 0, 40);
    bar_length = constrain(bar_length, 0, 40);
    
    Serial.print("[");
    for (int i = 0; i < 40; i++) {
      Serial.print(i < bar_length ? "█" : " ");
    }
    Serial.print("] ");
    Serial.printf("Level: %4d ", mic_level);
    
    if (mic_level < 100) {
      Serial.println("🔇 Silent");
    } else if (mic_level < 300) {
      Serial.println("🔉 Quiet");
    } else if (mic_level < 600) {
      Serial.println("🔊 Normal");
    } else {
      Serial.println("📢 LOUD!");
    }
    
    last_update = millis();
  }
  
  delay(10);
}
