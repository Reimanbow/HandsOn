#include <Arduino.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

M5Canvas canvas(&M5.Display);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// WiFi, MQTTに必要な情報
const char* ssid = "sensor-net";
const char* pass = "sensor-net0101";
const char* server_ip = "IPアドレス";
const int server_port = 1883;
const char* topic = "sample/imu";
const char* device_id = "M5Core2";

// IMUで取得するデータは構造体で定義する
struct IMUData {
  float accX;
  float accY;
  float accZ;
  float gyroX;
  float gyroY;
  float gyroZ;
} imuData;

// MQTTの再接続
void reconnect() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect(device_id)) {
    } else {
      M5.Display.print(",");
      delay(500);
    }
  }
}

// 描画
void printIMU(IMUData data) {
  canvas.startWrite();
  canvas.clearDisplay();
  canvas.setCursor(0, 0);
  canvas.printf("Accel\n X: %f\n Y: %f\n Z: %f\n", data.accX, data.accY, data.accZ);
  canvas.printf("Gyro\n X: %f\n Y: %f\n Z: %f\n", data.gyroX, data.gyroY, data.gyroZ);
  canvas.pushSprite(0, 0);
  canvas.endWrite();
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  canvas.createSprite(M5.Display.width(), M5.Display.height());
  canvas.setFont(&fonts::FreeMonoBold12pt7b);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    M5.Display.print(".");
    delay(500);
  }
  
  // MQTTブローカーの設定
  mqttClient.setServer(server_ip, server_port);
}

void loop() {
  unsigned long start_ms = millis();

  // MQTTの持続処理
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  M5.Imu.update();
  // センサデータの取得とMQTT送信
  if (M5.Imu.getAccel(&imuData.accX, &imuData.accY, &imuData.accZ)
      && M5.Imu.getGyro(&imuData.gyroX, &imuData.gyroY, &imuData.gyroZ)) {
    char json[200];
    StaticJsonDocument<200> doc;

    doc["accel"]["x"] = imuData.accX;
    doc["accel"]["y"] = imuData.accY;
    doc["accel"]["z"] = imuData.accZ;

    doc["gyro"]["x"] = imuData.gyroX;
    doc["gyro"]["y"] = imuData.gyroY;
    doc["gyro"]["z"] = imuData.gyroZ;

    serializeJson(doc, json);
    mqttClient.publish(topic, json);
    printIMU(imuData);
  }

  while (millis() - start_ms < 200);
}
