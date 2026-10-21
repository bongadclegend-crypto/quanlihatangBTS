// Thanh vien thuc hien: Vo Manh Thang
// Sua ngay 21/10
#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include "Entity.h"
#include "InputHelper.h"
#include "Date.h"

class PhieuBaoTri : public Entity {
private:
    std::string maTram;
    Date ngayBaoTri;
    std::string noiDung;
    std::string trangThai;
    double chiPhi;

public:
    PhieuBaoTri() { chiPhi = 0; }

    PhieuBaoTri(std::string maPBT, std::string maBTS, std::string ngay, std::string nd, std::string tt, double cp) {
        setId(Inputhelper::tuTaoMa(maPBT, "PBT"));
        setMaTram(maBTS);
        setNgayBaoTri(ngay);
        setNoiDung(nd);
        setTrangThai(tt);
        setChiPhi(cp);
    }

    // --- GETTER & SETTER ---

    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& maTram) {
        this->maTram = Inputhelper::tuTaoMa(maTram, "BTS");
    }

    std::string getNgayBaoTri() const { return ngayBaoTri.toString(); }
    void setNgayBaoTri(const std::string& ngay) {
        std::string clean = Inputhelper::catkhoangtrang(ngay);
        if (clean.empty()) throw Loixacthuc("Loi: Ngay bao tri khong duoc de trong!");

        // Chuyen viec xac thuc cau truc chuoi ngay thang cho object Date xu ly
        ngayBaoTri.docchuoi(clean);
    }

    std::string getNoiDung() const { return noiDung; }
    void setNoiDung(const std::string& nd) {
        std::string clean = Inputhelper::catkhoangtrang(nd);
        if (clean.empty()) throw Loixacthuc("Loi: Noi dung bao tri khong duoc de trong!");
        noiDung = clean;
    }

    std::string getTrangThai() const { return trangThai; }
    void setTrangThai(const std::string& tt) {
        std::string clean = Inputhelper::catkhoangtrang(tt);
        if (clean.empty()) throw Loixacthuc("Loi: Trang thai bao tri khong duoc de trong!");
        trangThai = clean;
    }

    double getChiPhi() const { return chiPhi; }
    void setChiPhi(double cp) {
        // Rang buoc gia tri so: chi phi bao tri khong duoc phep nho hon 0
        if (cp < 0) throw Loixacthuc("Loi: Chi phi bao tri khong duoc la so am!");
        chiPhi = cp;
    }

    // --- GHI DE (OVERRIDE) --- 

    /* Cac ham xu ly I/O dinh dang file */

    std::string toFilerecord() const override {
        return id + "|" + maTram + "|" + ngayBaoTri.toString() + "|" + noiDung + "|" + trangThai + "|" + std::to_string(chiPhi);
    }

    void fromFilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        // Phai dam bao doc du 6 cot du lieu de tranh loi tran bo nho
        if (data.size() >= 6) {
            setId(data[0]);
            maTram = data[1];
            ngayBaoTri.docchuoi(data[2]);
            noiDung = data[3];
            trangThai = data[4];
            chiPhi = std::stod(data[5]);
        }
    }

    // --- HIEN THI TREN CONSOLE ---

    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(10) << maTram << "| "
            << std::setw(14) << ngayBaoTri.toString() << "| "
            << std::setw(22) << noiDung << "| "
            << std::setw(15) << trangThai << "| "
            << std::setw(12) << std::fixed << std::setprecision(0) << chiPhi << " VND" << std::endl;
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma PBT" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(14) << "Ngay Bao Tri" << "| "
            << std::setw(22) << "Noi Dung" << "| "
            << std::setw(15) << "Trang Thai" << "| "
            << std::setw(12) << "Chi Phi" << "\n";
        std::cout << std::string(95, '-') << "\n";
    }
};
