#define encA 17 //vàng
#define encB 16//xanh lá

// khai báo biến đếm xung encoder
int pulseCount = 0;

// khai báo kênh A, B encoder
unsigned char encA_state;
unsigned char encB_state;

/********************ĐỌC ENCODER CHẾ ĐỘ X1*******************/
// void IRAM_ATTR readEncoderX1()
// {
//   encA_state = digitalRead(encA);
//   encB_state = digitalRead(encB);
//   switch ((encA_state << 1) | encB_state){
//     case 1:
//       pulseCount++;
//     break;
//     case 0:// Serial.println(pulseCount);
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
// void IRAM_ATTR readEncoderX4(){
//   encA_state = digitalRead(encA);
//   encB_state = digitalRead(encB);
//   // xác định kênh gây ra ngắt
//   int interruptPin = digitalPinToInterrupt(encA_state) == digitalPinToInterrupt(encA) ? encA : encB;
//   state = (state << 1) | encA_state;
//   state = (state << 1) | encB_state;
//   state = state&0x03; // bỏ hết các bit đầu chỉ giữ lại 2 bit cuối (0->3)
//   // xử lý ngắt cho kênh A
//   if (interruptPin == encA) {
//     switch(state){
//       case 2:
//         if (p_state == 0) pulseCount++;
//         else pulseCount--;
//       break;
//       case 1:
//         if (p_state == 3) pulseCount++;
//         else pulseCount--;
//       break;
//       case 0:
//         if (p_state == 2) pulseCount--;
//         else pulseCount++;
//       break;
//       case 3:
//         if (p_state == 1) pulseCount--;
//         else pulseCount++;
//       break;
//     }
//     p_state = state;
//   }

//   // xử lý ngắt cho kênh B
//   else if(interruptPin == encB){
//     switch(state){
//       case 3:
//         if (p_state == 2) pulseCount++;
//         else pulseCount--;
//       break;
//       case 0:
//         if (p_state == 1) pulseCount++;
//         else pulseCount--;
//       break;
//       case 1:
//         if (p_state == 0) pulseCount--;
//         else pulseCount++;
//       break;
//       case 2:
//         if (p_state == 3) pulseCount--;
//         else pulseCount++;
//       break;
//     }
//     p_state = state;
//   }
// }
/************************************************************/

void IRAM_ATTR readEncoderX4()
{
  encA_state = digitalRead(encA);
  encB_state = digitalRead(encB);
  
  int interruptPin = digitalPinToInterrupt(encA_state) == digitalPinToInterrupt(encA) ? encA : encB;
  state = (state << 2) | (encA_state << 1) | encB_state;
  state = state & 0x03; // chỉ giữ lại 2 bit cuối (0->3)
  
  static const int transitionTable[4][4] = {
    {0, 1, -1, 0},
    {-1, 0, 0, 1},
    {1, 0, 0, -1},
    {0, -1, 1, 0}
  };
  
  int nextState = state;
  
  if (interruptPin == encA) {
    nextState = (state << 2) | (encA_state << 1) | encB_state;
  } else if (interruptPin == encB) {
    nextState = (state << 2) | (encA_state << 1) | encB_state;
  }
  
  pulseCount += transitionTable[state][nextState];
  state = nextState;
}

void setup() {
  Serial.begin(115200);
  pinMode(encA, INPUT_PULLUP);
  pinMode(encB, INPUT_PULLUP);
  // x1
  // attachInterrupt(digitalPinToInterrupt(encA), readEncoderX1, FALLING);

  // x2
  // attachInterrupt(digitalPinToInterrupt(encA), readEncoderX2, CHANGE);
  
  // x4
  attachInterrupt(digitalPinToInterrupt(encA), readEncoderX4, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encB), readEncoderX4, CHANGE);
}

void loop() {
  Serial.println(pulseCount);
}
