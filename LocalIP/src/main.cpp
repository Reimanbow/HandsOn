#include <Arduino.h>
#include <M5Unified.h>
#include <WiFi.h>

const char* ssid = "sensor-net";
const char* pass = "sensor-net0101";

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  M5.Display.startWrite();
  M5.Display.printf("IP: %s", WiFi.localIP().toString());
  M5.Display.endWrite();
}

void loop() {
}
