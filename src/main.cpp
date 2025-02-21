#include <Arduino.h>
#include <CAN.h>
#include <PS4Controller.h>

void setup()
{
  Serial.begin(115200); // シリアル通信の開始（デバッグ用）
  while (!Serial)
    ;
  PS4.begin("08:B6:1F:ED:5E:34");
  Serial.println("Ready.");

  Serial.println("CAN Sender");

  CAN.setPins(26, 27); // TXピンとRXピンを指定
  if (!CAN.begin(500E3))
  { // 500 kbpsでCANバスを初期化
    Serial.println("Starting CAN failed!");
    while (1)
      ; // 失敗した場合は停止
  }
}

int nCount = 0;

void loop()
{
  // Serial.print("Sending packet: ");
  // Serial.println(nCount);

  int8_t l_x = PS4.LStickX();
  int8_t l_y = PS4.LStickY();
  int8_t r_x = PS4.RStickX();
  int8_t r_y = PS4.RStickY();
  CAN.beginPacket(0x12);
  CAN.write((uint8_t)l_x);
  CAN.write((uint8_t)l_y);
  CAN.write((uint8_t)r_x);
  CAN.write((uint8_t)r_y);
  CAN.endPacket();

  delay(10);
}
