#include <Arduino.h>
#include <M5Unified.h>
#include <WiFi.h>

const char *ssid = "CPSLAB_WLX";
const char *pass = "6bepa8ideapbu";

M5Canvas canvas(&M5.Display);

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  canvas.createSprite(M5.Display.width(), M5.Display.height());

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  configTime(3600L*9, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");

  struct tm timeInfo;
  if (getLocalTime(&timeInfo)) {
    M5.Rtc.setDateTime(timeInfo);
  }

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop() {
  auto t = M5.Rtc.getTime();
  int x = M5.Display.width() / 2 + 100 * cos(radians(t.seconds * 6));
  int y = M5.Display.height() / 2 + 100 * sin(radians(t.seconds * 6));

  canvas.startWrite();
  canvas.clearDisplay();
  canvas.setCursor(0, 0);
  canvas.drawWedgeLine(M5.Display.width()/2, M5.Display.height()/2, x, y, 5, 1, WHITE);
  canvas.pushSprite(0, 0);
  canvas.endWrite();

  delay(1);
}
