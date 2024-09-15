#include <Arduino.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi接続, MQTTに必要な情報
const char* ssid = "sensor-net";
const char* pass = "sensor-net0101";
const char* server_ip = "ローカルIP";
const int server_port = 1883;
const char* topic = "/sample/button";
const char* device_id = "Core2";

WiFiClient client;
PubSubClient mqttClient(client);

M5Canvas canvas(&M5.Display);

// MQTTの再接続を試みる
void reconnect() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect(device_id)) {
    } else {
      delay(500);
    }
  }
}

// JSON形式に変換し, MQTTでメッセージを送信
void sendMQTT(String message) {
  char json[200];
  StaticJsonDocument<200> doc;
  doc["Button"] = message;
  serializeJson(doc, json);
  mqttClient.publish(topic, json);
  delay(500);
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

  // MQTTブローカーの設定
  mqttClient.setServer(server_ip, server_port);
}

void loop() {
  // MQTTの接続が切れた場合は再接続を試みる
  if (!mqttClient.connected()) {
    reconnect();
  }
  // 接続の維持を行う
  mqttClient.loop();
  M5.update();

  if (M5.BtnA.wasPressed()) {
    printLabel("A");
    sendMQTT("A");
  } else if (M5.BtnB.wasPressed()) {
    printLabel("B");
    sendMQTT("B");
  } else if (M5.BtnC.wasPressed()) {
    printLabel("C");
    sendMQTT("C");
  } else {
    printLabel(" ");
  }
}
