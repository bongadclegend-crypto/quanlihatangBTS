//Hoang Tuan Thanh
#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "Entity.h"
#include "Inputhelper.h"
#include "Date.h"
#include "Exceptions.h"

class ThietBiLapDat : public Entity {
private:
    std::string tenThietBi;
    std::string maLoai;
    std::string maTram;
    Date ngayLapDat;
    std::string trangThai;
public:
    ThietBiLapDat() {}

    // DA XOA: viTriLapDat khoi Constructor
    ThietBiLapDat(std::string maTB, std::string ten, std::string mLoai, std::string mTram, std::string ngay, std::string tt) {
        setId(Inputhelper::TuTaoMa(maTB, "TB"));
        setTenThietBi(ten);
        setMaLoai(mLoai);
        setMaTram(mTram);
        setNgayLapDat(ngay);
        setTrangThai(tt);
    }

    std::string getTenThietBi() const { return tenThietBi; }
    void setTenThietBi(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: Ten thiet bi khong duoc de trong!");
        tenThietBi = Inputhelper::VietHoaChuDau(clean);
    }

    std::string getMaLoai() const { return maLoai; }
    void setMaLoai(const std::string& mLoai) {
        maLoai = Inputhelper::TuTaoMa(mLoai, "LT");
    }

    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& tram) {
        maTram = Inputhelper::TuTaoMa(tram, "BTS");
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
        std::string chuanHoa = Inputhelper::VietHoaChuDau(clean);

        // UPDATE: Chi cho phep 2 trang thai
        if (chuanHoa != "Loi" && chuanHoa != "Hoat Dong") {
            throw Loixacthuc("Loi: Trang thai chi duoc phep nhap 'Loi' hoac 'Hoat Dong'");
        }
        trangThai = chuanHoa;
    }

    // Giam xuong con 6 thuoc tinh
    std::string tofilerecord() const override {
        return id + "|" + tenThietBi + "|" + maLoai + "|" + maTram + "|" + ngayLapDat.toString() + "|" + trangThai;
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() != 6) {
            throw Loixacthuc("Loi: Ban ghi Thiet Bi Lap Dat phai co dung 6 truong");
        }

        ThietBiLapDat tbTam(data[0], data[1], data[2], data[3], data[4], data[5]);
        *this = tbTam;
    }

    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(25) << (tenThietBi.length() > 22 ? tenThietBi.substr(0, 19) + "..." : tenThietBi) << "| "
            << std::setw(12) << maLoai << "| "
            << std::setw(12) << maTram << "| "
            << std::setw(15) << ngayLapDat.toString() << "| "
            << std::setw(15) << trangThai << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma TB" << "| "
            << std::setw(25) << "Ten Thiet Bi" << "| "
            << std::setw(12) << "Ma Loai" << "| "
            << std::setw(12) << "Ma Tram" << "| "
            << std::setw(15) << "Ngay Lap" << "| "
            << std::setw(15) << "Trang Thai" << "\n";
        std::cout << std::string(95, '-') << "\n";
    }
};
