// #include "encoderRead.h"

// #define encoderA 16
// #define encoderB 17

// Encoder encoder(encoderA, encoderB, 4);

// void getfunc()
// {
//   encoder.readEncoder();
// }

// void setup() 
// {
//   Serial.begin(115200);
//   encoder.init();
//   encoder.setup(getfunc);
// }

// void loop() 
// {
//   int pulseCount = encoder.getPulse();
//   Serial.println(pulseCount);
//   delay(100); // Add a delay to control the printing rate (adjust as needed)
// }


#include <Arduino.h>
#include "encoderRead.h"

// Khai báo chân cho encoder và chế độ
const uint8_t encA_pin = 2;
const uint8_t encB_pin = 3;
const uint8_t mode = 2;

// Tạo đối tượng encoder
Encoder myEncoder(encA_pin, encB_pin, mode);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Đọc giá trị từ encoder
  // myEncoder.readEncoder();
  
  // Lấy giá trị số xung từ encoder
  int pulseCount = myEncoder.getPulse();

  // Hiển thị giá trị lên Serial Monitor
  Serial.print("Pulse Count: ");
  Serial.println(pulseCount);

  // Đợi một thời gian ngắn trước khi đọc lại encoder
  delay(100);
}
