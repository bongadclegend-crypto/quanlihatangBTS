#pragma once 
#include <iostream>   
#include <iomanip>    
#include <vector>     
#include <string>    
#include "Entity.h"   
#include "InputHelper.h" 
#include "Date.h"     

class ThietBiLapDat : public Entity {
private:
    std::string tenThietBi;
    std::string maLoai;
    std::string maTram;
    std::string viTriLapDat; // MOI THEM VAO, THAY THE NCC
    Date ngayLapDat;
    std::string trangThai;

public:
    ThietBiLapDat() {}

    std::string getTenThietBi() const { return tenThietBi; }
    void setTenThietBi(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: Ten thiet bi khong duoc de trong!");
        tenThietBi = clean;
    }

    std::string getMaLoai() const { return maLoai; }
    void setMaLoai(const std::string& mLoai) { maLoai = Inputhelper::TuTaoMa(mLoai, "LT"); }

    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& tram) { maTram = Inputhelper::TuTaoMa(tram, "BTS"); }

    std::string getViTriLapDat() const { return viTriLapDat; }
    void setViTriLapDat(const std::string& vt) {
        std::string clean = Inputhelper::catkhoangtrang(vt);
        if (clean.empty()) throw Loixacthuc("Loi: Vi tri lap dat khong duoc de trong!");
        viTriLapDat = clean;
    }

    std::string getNgayLapDat() const { return ngayLapDat.toString(); }
    void setNgayLapDat(const std::string& ngay) {
        std::string clean = Inputhelper::catkhoangtrang(ngay);
        if (clean.empty()) throw Loixacthuc("Loi: Ngay lap dat khong duoc de trong!");
        ngayLapDat.docchuoi(clean);
    }

    std::string getTrangThai() const { return trangThai; }
    void setTrangThai(const std::string& tt) {
        std::string clean = Inputhelper::catkhoangtrang(tt);
        if (clean.empty()) throw Loixacthuc("Loi: Trang thai khong duoc de trong!");
        trangThai = clean;
    }

    std::string tofilerecord() const override {
        return id + "|" + tenThietBi + "|" + maLoai + "|" + maTram + "|" + viTriLapDat + "|" + ngayLapDat.toString() + "|" + trangThai;
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 7) {
            setId(data[0]);
            tenThietBi = data[1];
            maLoai = data[2];
            maTram = data[3];
            viTriLapDat = data[4];
            ngayLapDat.docchuoi(data[5]);
            trangThai = data[6];
        }
    }

    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(20) << (tenThietBi.length() > 17 ? tenThietBi.substr(0, 14) + "..." : tenThietBi) << "| "
            << std::setw(10) << maLoai << "| "
            << std::setw(10) << maTram << "| "
            // Giam xuong 25 ky tu cho gon gang
            << std::setw(25) << (viTriLapDat.length() > 23 ? viTriLapDat.substr(0, 20) + "..." : viTriLapDat) << "| "
            << std::setw(12) << ngayLapDat.toString() << "| "
            << std::setw(15) << trangThai << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma TB" << "| "
            << std::setw(20) << "Ten Thiet Bi" << "| "
            << std::setw(10) << "Ma Loai" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            // Dong bo 25 ky tu
            << std::setw(25) << "Vi Tri Lap Dat" << "| "
            << std::setw(12) << "Ngay Lap" << "| "
            << std::setw(15) << "Trang Thai" << "\n";
        std::cout << std::string(110, '-') << "\n";
    }
};
