#ifndef DieuKhienStep_h
#define DieuKhienStep_h
#include <stdint.h>

class DieuKhienDongCoBuoc 
{
  public:
    // Khởi tạo động cơ có truyền thông số
    DieuKhienDongCoBuoc(int chanDir, int chanPul, int chanEna, float viBuoc);
    void begin();
    // Tính toán xung Output
    float xungTheoVongvaDo(uint32_t vongQuay, uint16_t gocQuay); // Vòng và độ
    float xungTheoMilimet(float buocVitme);
    // Chế độ chạy
    //void runPosition(float, float, bool); // Chạy vị trí với vận tốc đặt
    void chayViTri(float xungDauRa, float tocDoDat, bool chieuQuay);
    void chayVantoc(float tocDoDat, bool chieuQuay); // Chạy liên tục với vận tốc đặt
    void dungDongCo(); // Dừng động cơ
    // Kiểm tra chạy
    bool kiemTraChay();
  private:
    // Chân kết nối
    int _chanDir;
    int _chanPul;
    int _chanEna;
    // Chiều quay
    bool _chieuQuay;
    // Động cơ
    float _gocBuocDco;
    float _viBuoc;
    // Cơ khí
    float _buocVitme;
    // Tính toán
    float _xungTrenVong; 
    float _thoiGianDelay;
    // Tốc độ đặt
    float _tocDoDat;
    // Đầu ra
    long _xungDauRa;
    bool _kiemTraDongCo = false;
    // Chạy động cơ
    void chayDongCo (float);
};
#endif

