#include <Arduino.h>
#include <M5Unified.h>

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setColor(WHITE);
}

void loop() {
  M5.update();

  if (M5.Touch.isEnabled()) {
    auto t = M5.Touch.getDetail();
    M5.Display.startWrite();
    M5.Display.fillCircle(t.base_x, t.base_y, 5);
    M5.Display.endWrite();
  }
}
