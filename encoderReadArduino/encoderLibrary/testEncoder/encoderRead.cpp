#include "encoderRead.h"

Encoder::Encoder(int encA, int encB, uint8_t mode) {
  _encA = encA;
  _encB = encB;
  _mode = mode;
  _pulseCount = 0;
}

void Encoder::init() 
{
  pinMode(_encA, INPUT_PULLUP);
  pinMode(_encB, INPUT_PULLUP);

  encA_state = digitalRead(_encA);
  encB_state = digitalRead(_encB);
}

void Encoder::readEncoder()
{
  switch (_mode) {
    case 1:
      readEncoderX1();
      break;
    case 2:
      readEncoderX2();
      break;
    case 4:
      readEncoderX4();
      break;
    default:
      Serial.println("Sai chế độ!");
      break;
  }
}

void Encoder::setup()
{
  switch (_mode) {
    case 1:
      attachInterrupt(digitalPinToInterrupt(_encA), readEncoder, FALLING);
      break;
    case 2:
      attachInterrupt(digitalPinToInterrupt(_encA), readEncoder, CHANGE);
      break;
    case 4:
      attachInterrupt(digitalPinToInterrupt(_encA), readEncoder, CHANGE);
      attachInterrupt(digitalPinToInterrupt(_encB), readEncoder, CHANGE);
      break;
    default:
      Serial.println("Sai chế độ!");
      break;
  }
}

void Encoder::readEncoderX1()
{
  encA_state = digitalRead(_encA);
  encB_state = digitalRead(_encB);

  switch ((encA_state << 1) | encB_state){
    case 1:
      _pulseCount++;
    break;
    case 0:
      _pulseCount--;
    break;
  }
}

void Encoder::readEncoderX2()
{
  encA_state = digitalRead(_encA);
  encB_state = digitalRead(_encB);

  switch ((encA_state << 1) | encB_state){
    case 1:
    case 2:
      _pulseCount++;
    break;
    case 0:
    case 3:
      _pulseCount--;
    break;
  }
}

void Encoder::readEncoderX4()
{
  encA_state = digitalRead(_encA);
  encB_state = digitalRead(_encB);
  // xác định kênh gây ra ngắt
  int interruptPin = digitalPinToInterrupt(encA_state) == digitalPinToInterrupt(_encA) ? _encA : _encB;
  state = (state << 1) | encA_state;
  state = (state << 1) | encB_state;
  state = state&0x03; // bỏ hết các bit đầu chỉ giữ lại 2 bit cuối (0->3)
  // xử lý ngắt cho kênh A
  if (interruptPin == _encA) {
    switch(state){
      case 2:
        if (p_state == 0) _pulseCount++;
        else _pulseCount--;
      break;
      case 1:
        if (p_state == 3) _pulseCount++;
        else _pulseCount--;
      break;
      case 0:
        if (p_state == 2) _pulseCount--;
        else _pulseCount++;
      break;
      case 3:
        if (p_state == 1) _pulseCount--;
        else _pulseCount++;
      break;
    }
    // p_state = state;
  }

  // xử lý ngắt cho kênh B
  else if(interruptPin == _encB){
    switch(state){
      case 3:
        if (p_state == 2) _pulseCount++;
        else _pulseCount--;
      break;
      case 0:
        if (p_state == 1) _pulseCount++;
        else _pulseCount--;
      break;
      case 1:
        if (p_state == 0) _pulseCount--;
        else _pulseCount++;
      break;
      case 2:
        if (p_state == 3) _pulseCount--;
        else _pulseCount++;
      break;
    }
  }
  p_state = state;
}

int Encoder::getPulse()
{
  return _pulseCount;
}
