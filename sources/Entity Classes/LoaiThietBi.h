#pragma once 
#include <iostream>    
#include <iomanip>     
#include <vector>       
#include <string>     
#include "Entity.h"     
#include "InputHelper.h" 

class LoaiThietBi : public Entity {
private:
    std::string tenLoai;
    std::string hangSanXuat;
    std::string dacTinhKyThuat;
    double congSuatTieuThu;
    std::string maNCC;
    std::string tenNCC;

public:
    LoaiThietBi() : congSuatTieuThu(0.0) {}

    std::string getTenLoai() const { return tenLoai; }
    void setTenLoai(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: Ten loai thiet bi khong duoc de trong!");
        tenLoai = clean;
    }

    std::string getHangSanXuat() const { return hangSanXuat; }
    void setHangSanXuat(const std::string& hang) {
        std::string clean = Inputhelper::catkhoangtrang(hang);
        if (clean.empty()) throw Loixacthuc("Loi: Hang san xuat khong duoc de trong!");
        hangSanXuat = Inputhelper::VietHoaChuDau(clean);
    }

    std::string getDacTinhKyThuat() const { return dacTinhKyThuat; }
    void setDacTinhKyThuat(const std::string& dacTinh) {
        std::string clean = Inputhelper::catkhoangtrang(dacTinh);
        if (clean.empty()) throw Loixacthuc("Loi: Dac tinh ky thuat khong duoc de trong!");
        dacTinhKyThuat = clean;
    }

    double getCongSuatTieuThu() const { return congSuatTieuThu; }
    void setCongSuatTieuThu(double congSuat) {
        if (congSuat < 0) throw Loixacthuc("Loi: Cong suat tieu thu phai >= 0W!");
        congSuatTieuThu = congSuat;
    }

    std::string getMaNCC() const { return maNCC; }
    void setMaNCC(const std::string& ncc) { maNCC = Inputhelper::TuTaoMa(ncc, "NCC"); }

    std::string getTenNCC() const { return tenNCC; }
    void setTenNCC(const std::string& ten) { tenNCC = ten; }

    std::string tofilerecord() const override {
        return id + "|" + tenLoai + "|" + hangSanXuat + "|" + dacTinhKyThuat + "|" + std::to_string(congSuatTieuThu) + "|" + maNCC + "|" + tenNCC;
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 7) {
            setId(data[0]);
            tenLoai = data[1];
            hangSanXuat = data[2];
            dacTinhKyThuat = data[3];
            congSuatTieuThu = std::stod(data[4]);
            maNCC = data[5];
            tenNCC = data[6];
        }
    }

    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(22) << (tenLoai.length() > 19 ? tenLoai.substr(0, 16) + "..." : tenLoai) << "| "
            << std::setw(15) << hangSanXuat << "| "
            << std::setw(18) << (dacTinhKyThuat.length() > 15 ? dacTinhKyThuat.substr(0, 12) + "..." : dacTinhKyThuat) << "| "
            << std::setw(10) << congSuatTieuThu << "| "
            << std::setw(10) << maNCC << "| "
            << std::setw(18) << (tenNCC.length() > 15 ? tenNCC.substr(0, 12) + "..." : tenNCC) << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma LT" << "| "
            << std::setw(22) << "Ten Loai Thiet Bi" << "| "
            << std::setw(15) << "Hang SX" << "| "
            << std::setw(18) << "Dac Tinh KT" << "| "
            << std::setw(10) << "CongSuat" << "| "
            << std::setw(10) << "Ma NCC" << "| "
            << std::setw(18) << "Ten NCC" << "\n";
        std::cout << std::string(115, '-') << "\n";
    }
};
