#include <Arduino.h>
#include <M5Unified.h>
#include <Wire.h>
#include <M5UnitENV.h>

M5Canvas canvas(&M5.Display);

SHT3X sht30;
QMP6988 qmp6988;

float temperature = 0.0F;
float humidity = 0.0F;
float pressure = 0.0F;

// ディスプレイをまっさらにして(0, 0)にカーソルを移動させる
void clearDisplay() {
  canvas.clearDisplay();
  canvas.setCursor(0, 0);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  canvas.createSprite(M5.Display.width(), M5.Display.height());
  canvas.setFont(&fonts::lgfxJapanMincho_24);

  // 各種センサの初期化
  Wire.begin();
  while (!sht30.begin(&Wire, 0x44, M5.Ex_I2C.getSDA(), M5.Ex_I2C.getSCL())) {
    clearDisplay();
    canvas.println("not connect sht30");
    canvas.pushSprite(0, 0);
    delay(500);
  }

  while (!qmp6988.begin(&Wire, 0x70, M5.Ex_I2C.getSDA(), M5.Ex_I2C.getSCL())) {
    clearDisplay();
    canvas.println("not connect qmp6988");
    canvas.pushSprite(0, 0);
    delay(500);
  }
}

void loop() {
  unsigned int start_ms = millis();

  if (sht30.update()) {
    temperature = sht30.cTemp;
    humidity = sht30.humidity;
  }

  if (qmp6988.update()) {
    pressure = qmp6988.pressure;
  }

  clearDisplay();
  canvas.printf("Temperature: %f\n", temperature);
  canvas.printf("Humidity: %f\n", humidity);
  canvas.printf("Pressure: %f\n", pressure);
  canvas.pushSprite(0, 0);

  while (millis() - start_ms <= 100);  
}
