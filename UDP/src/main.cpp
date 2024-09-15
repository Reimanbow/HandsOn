#include <Arduino.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi接続, UDP通信に必要な情報
const char* ssid = "00ADD5F9D913-2G";
const char* pass = "f4kc2v4z2nr5tf";
const char* server_ip = "192.168.3.4";
const int server_port = 50007;
const int client_port = 50008;

WiFiUDP wifiudp;

M5Canvas canvas(&M5.Display);

// UDPパケットを送信する
void sendUDP(String label) {
  wifiudp.beginPacket(server_ip, server_port);
  wifiudp.print(label);
  wifiudp.endPacket();
  delay(1000);
}

// 送信した結果をディスプレイに表示する
void printLabel(String label) {
  canvas.startWrite();
  canvas.clearDisplay();
  canvas.setCursor(0, 100);
  canvas.printf("you send %s", label);
  canvas.pushSprite(0, 0);
  canvas.endWrite();
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  canvas.createSprite(M5.Display.width(), M5.Display.height());
  canvas.setTextFont(&fonts::FreeMono24pt7b);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  wifiudp.begin(client_port);
}

void loop() {
  M5.update();

  // ボタンによって送信する文字を変更する
  if (M5.BtnA.wasPressed()) {
    printLabel("A");
    sendUDP("A");
  } else if (M5.BtnB.wasPressed()) {
    printLabel("B");
    sendUDP("B");    
  } else if (M5.BtnC.wasPressed()) {
    printLabel("C");
    sendUDP("C");
  } else {
    printLabel(" ");
  }
}
