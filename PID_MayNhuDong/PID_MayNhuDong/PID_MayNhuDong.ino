/*
_____BTS7960B - CW:
đỏ -> RPWM (33)
nâu -> LPWM (13)

_____encoder:
xanh lá -> 16
trắng -> 17
*/

#include "PID.h"

#define encA 16
#define encB 17
#define pinMotor 33

// một mảng để lưu trữ các con trỏ tới các bộ timer đã được tạo
TimerHandle_t timerPIDMotor = NULL;

// khai báo biến đếm xung encoder
int pulseCount = 0;
float pulseEnc = 1600.0;

// khai báo các thông số PID
const float Kp_motor = 10.0;
const float Ki_motor = 250.0;
const float Kd_motor = 0;
const float sampleTime_motor = 50.0;
const uint16_t freqMotor = 10000;

// khai báo các biến ngõ ra 
const float desiredSpeed = 5.0;
float speed = 0.0;
uint16_t outputValue = 0;

// khai báo các biến lọc trung bình
float speedAvr = 0.0;
float speedSum = 0.0;

// khai báo kênh A, B encoder
unsigned char encA_state;
unsigned char encB_state;

// khởi tạo PID
PID motorPID(Kp_motor, Ki_motor, Kd_motor, sampleTime_motor);

/********************ĐỌC ENCODER CHẾ ĐỘ X1*******************/
// void IRAM_ATTR readEncoderX1(){
//   encA_state = digitalRead(encA);
//   encB_state = digitalRead(encB);
//   switch ((encA_state << 1) | encB_state){
//     case 1:
//       pulseCount++;
//     break;
//     case 0:
//       pulseCount--;
//     break;
//   }
// }
/************************************************************/


/********************ĐỌC ENCODER CHẾ ĐỘ X2*******************/
// void IRAM_ATTR readEncoderX2(){
//   encA_state = digitalRead(encA);
//   encB_state = digitalRead(encB);
//   switch ((encA_state << 1) | encB_state){
//     case 1:
//     case 2:
//       pulseCount++;
//     break;
//     case 0:
//     case 3:
//       pulseCount--;
//     break;
//   }
// }
/************************************************************/


/********************ĐỌC ENCODER CHẾ ĐỘ X4*******************/
unsigned char state;
unsigned char p_state;
void IRAM_ATTR readEncoderX4(){
  encA_state = digitalRead(encA);
  encB_state = digitalRead(encB);
  // xác định kênh gây ra ngắt
  int interruptPin = digitalPinToInterrupt(encA_state) == digitalPinToInterrupt(encA) ? encA : encB;
  state = (state << 1) | encA_state;
  state = (state << 1) | encB_state;
  state = state&0x03; // bỏ hết các bit đầu chỉ giữ lại 2 bit cuối (0->3)
  // xử lý ngắt cho kênh A
  if (interruptPin == encA) {
    switch(state){
      case 2:
        if (p_state == 0) pulseCount++;
        else pulseCount--;
      break;
      case 1:
        if (p_state == 3) pulseCount++;
        else pulseCount--;
      break;
      case 0:
        if (p_state == 2) pulseCount--;
        else pulseCount++;
      break;
      case 3:
        if (p_state == 1) pulseCount--;
        else pulseCount++;
      break;
    }
    p_state = state;
  }

  // xử lý ngắt cho kênh B
  else if(interruptPin == encB){
    switch(state){
      case 3:
        if (p_state == 2) pulseCount++;
        else pulseCount--;
      break;
      case 0:
        if (p_state == 1) pulseCount++;
        else pulseCount--;
      break;
      case 1:
        if (p_state == 0) pulseCount--;
        else pulseCount++;
      break;
      case 2:
        if (p_state == 3) pulseCount--;
        else pulseCount++;
      break;
    }
    p_state = state;
  }
}
/************************************************************/

// hàm ngắt timer tính toán ngõ ra
void timerPIDMotorCallBack(TimerHandle_t xTimer){
  static int tempCount = 0;
  int data = 1;
  speed = ((pulseCount / pulseEnc) / (sampleTime_motor / 1000.0)) * 60.0;
  speedSum += speed;
  tempCount++;
  pulseCount = 0;

  if(tempCount == 50){
    speedAvr = speedSum/tempCount;
    speedSum = 0;
    tempCount = 0;
  }
  outputValue = (uint16_t)motorPID.getPIDcompute(desiredSpeed - speed);
  ledcWrite(0, outputValue);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(encA, INPUT_PULLUP);
  pinMode(encB, INPUT_PULLUP);

  pinMode(pinMotor, OUTPUT);
  ledcSetup(0, freqMotor, 12);
  ledcAttachPin(pinMotor, 0);

  // x1
  //attachInterrupt(digitalPinToInterrupt(encA), readEncoderX1, FALLING);

  // x2
  //attachInterrupt(digitalPinToInterrupt(encA), readEncoderX2, CHANGE);
  
  // x4
  attachInterrupt(digitalPinToInterrupt(encA), readEncoderX4, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encB), readEncoderX4, CHANGE);

  motorPID.setWindup(0.0, 4095.0);
  motorPID.setOutput(0.0, 4095.0);
  timerPIDMotor = xTimerCreate(
    "Auto-reload_timer",
    sampleTime_motor/portTICK_PERIOD_MS,  
    pdTRUE,
    (void *)1,
    timerPIDMotorCallBack
  );
  xTimerStart(timerPIDMotor, portMAX_DELAY);
}

void loop() {
  Serial.print("Đáp ứng ngõ ra: ");
  Serial.println(outputValue);

  Serial.print("Tốc độ: ");
  Serial.println(speedAvr);
  delay(500);

  // Serial.println(pulseCount);
}
