#include "StepperMotorControl.h"

#define chanDir 2
#define chanPul 3
#define chanEna 8

float tocDoDat = 60.0; //rpm
uint32_t soVongQuay = 1;  //round
uint16_t gocQuay = 270;

uint8_t viBuoc = 32.0; //microstep /32

float xungChayDCo = 0;

DieuKhienDongCoBuoc dongCo(chanDir, chanPul, chanEna, viBuoc);

void setup() {
  Serial.begin(9600);
  dongCo.begin();
  xungChayDCo = dongCo.xungTheoVongvaDo(2, 0);
  Serial.print("xung chay dong co: ");
  Serial.println(xungChayDCo);
}
  
void loop() {
  // int temp =0;
  // temp = millis();
  
  // dongCo.chayViTri(xungChayDCo, tocDoDat, HIGH);
  dongCo.chayVantoc(tocDoDat, HIGH);

  // Serial.println(millis()-temp);
}
