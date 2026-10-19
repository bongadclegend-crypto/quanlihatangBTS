#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "Entity.h"
#include "InputHelper.h"

class KyThuatVien : public Entity {
private:
    std::string tenktv;
    std::string chuyenmon;
    std::string sodienthoai;

public:
    KyThuatVien() {}
    KyThuatVien(std::string ma, std::string ten, std::string cm, std::string sdt) {
        setId(Inputhelper::tuTaoMa(ma, "KTV"));
        setTenKTV(ten);
        setChuyenMon(cm);
        setSoDienThoai(sdt);
    }

    std::string getTenKTV() const { return tenktv; }
    void setTenKTV(const std::string& ten) {
        if (ten == "") throw Loixacthuc("Loi: Ten khong duoc de trong!");
        if (!Inputhelper::coChuaChuCai(ten)) throw Loixacthuc("Loi: Ten phai co chu cai!");
        tenktv = Inputhelper::vietHoaChuDau(ten);
    }

    std::string getChuyenMon() const { return chuyenmon; }
    void setChuyenMon(const std::string& cm) {
        if (cm == "") throw Loixacthuc("Loi: Chuyen mon khong duoc de trong!");
        chuyenmon = Inputhelper::vietHoaChuDau(cm);
    }

    std::string getSoDienThoai() const { return sodienthoai; }
    void setSoDienThoai(const std::string& sdt) {
        if (sdt == "") throw Loixacthuc("Loi: So dien thoai khong duoc de trong!");
        Inputhelper::kiemTraSdt(sdt);
        sodienthoai = sdt;
    }

    std::string toFilerecord() const override {
        return id + "|" + tenktv + "|" + chuyenmon + "|" + sodienthoai;
    }

    void fromFilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 4) {
            setId(data[0]);
            tenktv = data[1];
            chuyenmon = data[2];
            sodienthoai = data[3];
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
            << std::setw(25) << tenktv << "| "
            << std::setw(25) << chuyenmon << "| "
            << std::setw(15) << sodienthoai << "\n";
    }
};
