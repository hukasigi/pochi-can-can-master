#include <Arduino.h>
#include <CAN.h>
#include <PS4Controller.h>

const int8_t SLAVE_1 = 0x10; // スレーブ1のID
const int8_t SLAVE_2 = 0x11; // スレーブ2のID
const int8_t SLAVE_3 = 0x12; // スレーブ3のID

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  PS4.begin("08:B6:1F:ED:5E:34");

  CAN.setPins(26, 27);
  if (!CAN.begin(500E3))
  {
    Serial.println("Failed");
    while (1)
      ;
  }
  Serial.println("Success");
}

// デッドゾーン処理
int8_t DeadZone(int16_t value, int ZONE = 10)
{
  return (abs(value) < ZONE) ? 0 : value;
}

// 指定したスレーブにデータ送信
// commandは、どのような処理をさせるか
void sendCANData(uint8_t targetID, uint8_t command, int8_t send_date1, int8_t send_date2, int8_t send_date3, int8_t send_date4)
{
  CAN.beginPacket(0xFF); //すべてのスレーブにデータを送る
  CAN.write(targetID); // 送信先スレーブのID
  CAN.write(command);        // なにさせるか
  CAN.write(send_date1);
  CAN.write(send_date2);
  CAN.write(send_date3);
  CAN.write(send_date4);
  CAN.endPacket();
}

void loop()
{
  int8_t l_x = DeadZone(PS4.LStickX());
  int8_t l_y = DeadZone(PS4.LStickY());
  int8_t r_x = DeadZone(PS4.RStickX());
  int8_t r_y = DeadZone(PS4.RStickY());
  int8_t circle_button = PS4.Circle();
  int8_t triangle_button = PS4.Triangle();
  int8_t square_button = PS4.Square();
  int8_t cross_button = PS4.Cross();

  // スレーブ1 モーター制御
  sendCANData(SLAVE_1, 0x01, l_x, l_y, r_x, r_y);

  // スレーブ2 ボタン制御
  sendCANData(SLAVE_2, 0x02, circle_button, triangle_button, square_button, cross_button);

  // スレーブ3
  sendCANData(SLAVE_3, 0x01, l_x, l_y, r_x, r_y);

  delay(10);
}
