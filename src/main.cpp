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
  Serial.print("Sending packet: ");
  Serial.println(nCount);
  int l_x = map(PS4.LStickX(), -127, 127, -255, 255);
  int l_y = map(PS4.LStickY(), -127, 127, -255, 255);
  int r_x = map(PS4.RStickX(), -127, 127, -255, 255);
  int r_y = map(PS4.RStickY(), -127, 127, -255, 255);

  CAN.beginPacket(0x12); // ID 0x12のCANパケットを送信開始
  // char buf[256];
  //  sprintf(buf, "w:%d", nCount);           // 送信するデータ内容を文字列として作成
  //  CAN.write((uint8_t *)buf, strlen(buf)); // データを書き込む
  CAN.write(l_x);  // データを書き込む
  CAN.endPacket(); // パケットの送信を完了

  delay(1000); // 1秒ごとに送信
}
