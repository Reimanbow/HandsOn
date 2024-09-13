#include <Arduino.h>
#include <M5Unified.h>

// 画面のちらつきを防止するためM5Canvasを使用する
M5Canvas canvas(&M5.Display);

// バイブレーション関数
void vibration(int power, int count) {
  for (int i = 0; i < count; i++) {
    M5.Power.Axp192.setLDO3(power);
    delay(500 / count);
    M5.Power.Axp192.setLDO3(0);
    delay(500 / count);
  }
}

// 画面の描画
void drawDisplay(String label) {
  canvas.startWrite();
  canvas.fillScreen(BLACK);
  canvas.setCursor(0, 100);
  canvas.print("You Pushing Button ");
  canvas.print(label);
  canvas.pushSprite(0, 0);
  canvas.endWrite();
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  canvas.createSprite(M5.Display.width(), M5.Display.height());
  canvas.setTextFont(&fonts::Font4);
}

void loop() {
  // ボタンの状態の更新
  M5.update();

  // どのボタンが押されたかによりバイブレーションの挙動を変える
  // どの条件下によってディスプレイの描画も変化させる
  if (M5.BtnA.isPressed()) {
    drawDisplay("A");
    vibration(2000, 4);
  } else if(M5.BtnB.isPressed()) {
    drawDisplay("B");
    vibration(2500, 2);
  } else if(M5.BtnC.isPressed()) {
    drawDisplay("C");
    vibration(3000, 1);
  } else {
    drawDisplay(" ");
  }
}
