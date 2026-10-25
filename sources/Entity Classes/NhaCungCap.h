#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "Entity.h"
#include "InputHelper.h"

class NhaCungCap : public Entity {
private:
    std::string tenNCC;
    std::string diaChi;
    std::string soDienThoai;

public:
    NhaCungCap() {}

    std::string getTenNCC() const { return tenNCC; }
    void setTenNCC(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: Ten NCC khong duoc de trong!");
        tenNCC = clean;
    }

    std::string getDiaChi() const { return diaChi; }
    void setDiaChi(const std::string& dc) {
        std::string clean = Inputhelper::catkhoangtrang(dc);
        if (clean.empty()) throw Loixacthuc("Loi: Dia chi khong duoc de trong!");
        diaChi = clean;
    }

    std::string getSoDienThoai() const { return soDienThoai; }
    void setSoDienThoai(const std::string& sdt) {
        Inputhelper::KiemTraSdt(sdt);
        soDienThoai = sdt;
    }

    std::string tofilerecord() const override {
        return id + "|" + tenNCC + "|" + diaChi + "|" + soDienThoai;
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 4) {
            setId(data[0]);
            tenNCC = data[1];
            diaChi = data[2];
            soDienThoai = data[3];
        }
    }

    void display() const override {
        std::cout << std::left
            << std::setw(10) << id << "| "
            << std::setw(25) << (tenNCC.length() > 22 ? tenNCC.substr(0, 19) + "..." : tenNCC) << "| "
            << std::setw(30) << (diaChi.length() > 27 ? diaChi.substr(0, 24) + "..." : diaChi) << "| "
            << std::setw(15) << soDienThoai << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(10) << "Ma NCC" << "| "
            << std::setw(25) << "Ten Nha Cung Cap" << "| "
            << std::setw(30) << "Dia Chi" << "| "
            << std::setw(15) << "So Dien Thoai" << "\n";
        std::cout << std::string(85, '-') << "\n";
    }
};
