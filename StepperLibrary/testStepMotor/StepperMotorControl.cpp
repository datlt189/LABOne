#ifndef _StepperMotorControl_H_
#define _StepperMotorControl_H_
// #import <Arduino.h>
#include "StepperMotorControl.h"


/*---------------- HÀM KHỞI TẠO ĐỘNG CƠ CÓ TRUYỀN THÔNG SỐ ---------------*/
DieuKhienDongCoBuoc::DieuKhienDongCoBuoc(int chanDir, int chanPul, int chanEna, float viBuoc) 
{
  _chanDir = chanDir;
  _chanPul = chanPul;
  _chanEna = chanEna;

  _gocBuocDco = 1.8;
  _viBuoc = viBuoc;
  _xungTrenVong = 360.0 / (_gocBuocDco / _viBuoc);
}
/*------------------------------------------------------------------------*/


/*------------------------- HÀM CẤU HÌNH ĐỘNG CƠ -------------------------*/
void DieuKhienDongCoBuoc::begin() 
{
  pinMode(_chanDir, OUTPUT);
  pinMode(_chanPul, OUTPUT);
  pinMode(_chanEna, OUTPUT);
  digitalWrite(_chanEna, LOW);
}
/*------------------------------------------------------------------------*/


/*----------------------- TÍNH TOÁN XUNG OUTPUT --------------------------*/
// Vòng và độ
float DieuKhienDongCoBuoc::xungTheoVongvaDo(uint32_t vongQuay, uint16_t gocQuay)
{
  return (vongQuay + (gocQuay / 360.0)) * _xungTrenVong;
}
// Milimet
float DieuKhienDongCoBuoc::xungTheoMilimet(float buocVitme)
{
  _buocVitme = buocVitme;
  return _xungTrenVong * _viBuoc / _buocVitme;
}
/*------------------------------------------------------------------------*/


/*------------------------- HÀM CHẠY ĐỘNG CƠ -----------------------------*/
void DieuKhienDongCoBuoc::chayDongCo(float thoiGianDelay)
{
  _kiemTraDongCo = true;
  digitalWrite(_chanPul, HIGH);
  delayMicroseconds(_thoiGianDelay);
  digitalWrite(_chanPul, LOW);
  delayMicroseconds(_thoiGianDelay);
}
/*------------------------------------------------------------------------*/


/*--------------------------- HÀM CHẠY VỊ TRÍ ----------------------------*/
void DieuKhienDongCoBuoc::chayViTri(float xungDauRa, float tocDoDat, bool chieuQuay) 
{
  _chieuQuay = chieuQuay;
  _tocDoDat = tocDoDat;
  _xungDauRa = xungDauRa;

  digitalWrite(_chanDir, _chieuQuay ? HIGH:LOW);

  _thoiGianDelay = 500000.0 / ((_tocDoDat / 60.0) * _xungTrenVong);

  for (long i = 0; i < _xungDauRa; i++) {
    chayDongCo(_thoiGianDelay);
  }
}
/*------------------------------------------------------------------------*/



/*--------------------------- HÀM CHẠY LIÊN TỤC --------------------------*/
void DieuKhienDongCoBuoc::chayVantoc(float tocDoDat, bool chieuQuay) 
{
  _chieuQuay = chieuQuay;
  _tocDoDat = tocDoDat;

  digitalWrite(_chanDir, chieuQuay ? HIGH : LOW);

  _thoiGianDelay = 500000.0 / ((tocDoDat / 60.0) * _xungTrenVong);

  chayDongCo(_thoiGianDelay);
}


/*--------------------------- HÀM DỪNG ĐỘNG CƠ ---------------------------*/
void DieuKhienDongCoBuoc::dungDongCo()
{
  digitalWrite(_chanPul, LOW);
  _kiemTraDongCo = false;
}
/*------------------------------------------------------------------------*/


/*--------------------------- HÀM CHECK ĐỘNG CƠ --------------------------*/
bool DieuKhienDongCoBuoc::kiemTraChay()
{
  return _kiemTraDongCo ? true : false;
}
/*------------------------------------------------------------------------*/


#endif //_StepperMotorControl_H_