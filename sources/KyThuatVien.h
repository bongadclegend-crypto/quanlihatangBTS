#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "Entity.h"
#include "InputHelper.h"

class KyThuatVien : public Entity {
private:
    std::string tenKTV;
    std::string chuyenMon;
    std::string soDienThoai;

public:
    KyThuatVien() {}
    KyThuatVien(std::string ma, std::string ten, std::string cm, std::string sdt) {
        setId(Inputhelper::TuTaoMa(ma, "KTV"));
        setTenKTV(ten);
        setChuyenMon(cm);
        setSoDienThoai(sdt);
    }

    std::string getTenKTV() const { return tenKTV; }
    void setTenKTV(const std::string& ten) {
        if (ten == "") throw Loixacthuc("Loi: Ten khong duoc de trong!");
        if (!Inputhelper::CoChuaChuCai(ten)) throw Loixacthuc("Loi: Ten phai co chu cai!");
        tenKTV = Inputhelper::VietHoaChuDau(ten);
    }

    std::string getChuyenMon() const { return chuyenMon; }
    void setChuyenMon(const std::string& cm) {
        if (cm == "") throw Loixacthuc("Loi: Chuyen mon khong duoc de trong!");
        chuyenMon = Inputhelper::VietHoaChuDau(cm);
    }

    std::string getSoDienThoai() const { return soDienThoai; }
    void setSoDienThoai(const std::string& sdt) {
        if (sdt == "") throw Loixacthuc("Loi: So dien thoai khong duoc de trong!");
        Inputhelper::KiemTraSdt(sdt);
        soDienThoai = sdt;
    }

    std::string tofilerecord() const override {
        return id + "|" + tenKTV + "|" + chuyenMon + "|" + soDienThoai;
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 4) {
            setId(data[0]);
            tenKTV = data[1];
            chuyenMon = data[2];
            soDienThoai = data[3];
        }
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(10) << "Ma KTV" << "| "
            << std::setw(25) << "Ten Ky Thuat Vien" << "| "
            << std::setw(25) << "Chuyen Mon" << "| "
            << std::setw(15) << "So Dien Thoai" << "\n";
        std::cout << std::string(79, '-') << "\n";
    }

    void display() const override {
        std::cout << std::left
            << std::setw(10) << id << "| "
            << std::setw(25) << tenKTV << "| "
            << std::setw(25) << chuyenMon << "| "
            << std::setw(15) << soDienThoai << "\n";
    }
};
