// Minimal HX711 reader: prints raw values in a plotter-friendly format.
// Wiring (ESP32 Upesy Wroom):
//   HX711 DT (aka DOUT) -> GPIO12
//   HX711 SCK (aka PD_SCK) -> GPIO14

#include <Arduino.h>
#include <HX711.h>

// Pin definitions
constexpr uint8_t HX711_DT_PIN = 26;  // DOUT
constexpr uint8_t HX711_SCK_PIN = 14; // SCK

HX711 scale;

void setup() {
  Serial.begin(115200);
  // Give USB serial some time to connect (esp. when using auto-reset)
  delay(300);

  // Initialize HX711 with our pins
  scale.begin(HX711_DT_PIN, HX711_SCK_PIN);

  // Optional: set gain/channel (default is 128 on channel A)
  // scale.set_gain(128);

  // No banner text to keep plotter input clean
}

void loop() {
  // Read raw value; waitForReady(true) ensures blocking until data ready
  if (scale.wait_ready_timeout(500)) {
    long raw = scale.read(); // raw 24-bit signed value
    // Serial Plotter format: lines start with '>' and contain name:value pairs
    // Example: >raw:12345\r\n
    Serial.print('>');
    Serial.print("raw:");
    Serial.println(raw); // println adds \r\n
  } else {
    // If not ready, skip emitting a data line to avoid non-numeric output
  }

  delay(100); // ~10 Hz (depends on HX711 rate and wiring)
}