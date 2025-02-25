#include <Arduino.h>
#include <CAN.h>
#include <PS4Controller.h>
using namespace std;

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
  int16_t l_x = PS4.LStickX();
  int16_t l_y = PS4.LStickY();
  int16_t r_x = PS4.RStickX();
  int16_t r_y = PS4.RStickY();
  if (abs(l_x) < 10)
    l_x = 0;
  if (abs(l_y) < 10)
    l_y = 0;
  if (abs(r_x) < 10)
    r_x = 0;
  if (abs(r_y) < 10)
    r_y = 0;
  CAN.beginPacket(0x12);
  CAN.write((uint8_t)l_x);
  CAN.write((uint8_t)l_y);
  CAN.write((uint8_t)r_x);
  CAN.write((uint8_t)r_y);
  CAN.endPacket();

  delay(10);
}
