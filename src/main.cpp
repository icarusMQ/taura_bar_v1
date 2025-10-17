// Minimal HX711 reader (2 scales): prints raw values in a plotter-friendly format.
// Wiring (ESP32 Upesy Wroom):
//   Scale 1: HX711 DOUT -> GPIO26, SCK -> GPIO14
//   Scale 2: HX711 DOUT -> GPIO12, SCK -> GPIO14 (SCK shared)

#include <Arduino.h>
#include <HX711.h>

// Pin definitions
constexpr uint8_t HX711_1_DT_PIN = 26;  // DOUT (scale 1)
constexpr uint8_t HX711_1_SCK_PIN = 14; // SCK  (shared)
constexpr uint8_t HX711_2_DT_PIN = 12;  // DOUT (scale 2)
constexpr uint8_t HX711_2_SCK_PIN = 14; // SCK  (shared)

HX711 scale1;
HX711 scale2;

void setup() {
  Serial.begin(115200);
  // Give USB serial some time to connect (esp. when using auto-reset)
  delay(300);

  // Initialize both HX711 modules
  scale1.begin(HX711_1_DT_PIN, HX711_1_SCK_PIN);
  scale2.begin(HX711_2_DT_PIN, HX711_2_SCK_PIN);

  // Optional: set gain/channel (default is 128 on channel A)
  // scale1.set_gain(128);
  // scale2.set_gain(128);

  // No banner text to keep plotter input clean
}

void loop() {
  // Attempt to read both scales; print one line with both values.
  static long v1 = 0;
  static long v2 = 0;

  bool r1 = scale1.wait_ready_timeout(100);
  if (r1) {
    v1 = scale1.read();
  }

  bool r2 = scale2.wait_ready_timeout(100);
  if (r2) {
    v2 = scale2.read();
  }

  // Serial Plotter format: >name:value[,name:value]*\r\n
  Serial.print('>');
  Serial.print("raw1:");
  Serial.print(v1);
  Serial.print(",raw2:");
  Serial.println(v2); // println adds \r\n
  delay(100); // ~10 Hz (depends on HX711 rate and wiring)
}