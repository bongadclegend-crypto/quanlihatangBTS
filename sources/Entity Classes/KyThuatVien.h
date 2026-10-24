// Kieu Binh Nhan B24DCVT285
#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "Entity.h"
#include "InputHelper.h"
// Lop quan ly thong tin ky thuat vien, ke thua tu lop Entity
class KyThuatVien : public Entity {
private:
    std::string tenktv;
    std::string doinguchuyenmon;
    std::string sodienthoai;
public:
    // Ham khoi tao mac dinh
    KyThuatVien() {}
    KyThuatVien(std::string ma, std::string ten, std::string cm, std::string sdt) {
        setId(Inputhelper::TuTaoMa(ma, "KTV"));
        setTenKTV(ten);
        setDoiNguChuyenMon(cm);
        setSoDienThoai(sdt);
    }
    // Cac ham lay va gan du lieu
    std::string getTenKTV() const { return tenktv; }
    void setTenKTV(const std::string& ten) {
        if (ten == "") throw Loixacthuc("Loi: Ten khong duoc de trong!");
        if (!Inputhelper::CoChuaChuCai(ten)) throw Loixacthuc("Loi: Ten phai co chu cai!");
        tenktv = Inputhelper::VietHoaChuDau(ten);
    }
    std::string getDoiNguChuyenMon() const { return doinguchuyenmon; }
    void setDoiNguChuyenMon(const std::string& cm) {
        if (cm == "") throw Loixacthuc("Loi: Doi ngu chuyen mon khong duoc de trong!");
        std::string chuanhoa = Inputhelper::VietHoaChuDau(cm);
        if (chuanhoa != "Sua chua" && chuanhoa != "Sua Chua" &&
            chuanhoa != "Lap dat" && chuanhoa != "Lap Dat" &&
            chuanhoa != "Van hanh" && chuanhoa != "Van Hanh" &&
            chuanhoa != "Phat trien" && chuanhoa != "Phat Trien") {
            throw Loixacthuc("Loi: Doi ngu chuyen mon chi duoc nhap: Sua chua, Lap dat, Van hanh, hoac Phat trien!");
        }
        doinguchuyenmon = chuanhoa;
    }
    std::string getSoDienThoai() const { return sodienthoai; }
    void setSoDienThoai(const std::string& sdt) {
        if (sdt == "") throw Loixacthuc("Loi: So dien thoai khong duoc de trong!");
        Inputhelper::KiemTraSdt(sdt);
        sodienthoai = sdt;
    }
    // Ham su ly file 
    std::string tofilerecord() const override {
        return id + "|" + tenktv + "|" + doinguchuyenmon + "|" + sodienthoai;
    }
    // Ham doc file
    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 4) {
            setId(data[0]);
            tenktv = data[1];
            doinguchuyenmon = data[2];
            sodienthoai = data[3];
        }
    }
    // Ham hien thi danh sach KTV
    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma KTV" << "| "
            << std::setw(25) << "Ten Ky Thuat Vien" << "| "
            << std::setw(20) << "Doi ngu chuyen mon" << "| "
            << std::setw(15) << "So Dien Thoai" << "\n";
        std::cout << std::string(72, '-') << "\n";
    }
    // Ham hien thi thong tin KTV
    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(25) << tenktv << "| "
            << std::setw(20) << doinguchuyenmon << "| "
            << std::setw(15) << sodienthoai << "\n";
    }
};
