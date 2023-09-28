#ifndef Encoder_h_
#define Encoder_h_

#include <Arduino.h>

class Encoder {
  private:
    int _encA;
    int _encB;
    unsigned char encA_state;
    unsigned char encB_state;
    unsigned char p_state;
    unsigned char state;
    long _pulseCount;
    uint8_t _mode;

  public:
    Encoder(int encA, int encB, uint8_t mode);
    void init();
    void setup(); 
    void readEncoderX1();
    void readEncoderX2();
    void readEncoderX4();
    int getPulse();
};

#endif // Encoder_h_
