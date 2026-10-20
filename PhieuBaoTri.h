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

    PhieuBaoTri(std::string maPBT, std::string maBTS, Date ngay, std::string nd, std::string tt, double cp) {
        setId(maPBT);
        setMaTram(maBTS);
        setNgayBaoTri(ngay);
        setNoiDung(nd);
        setTrangThai(tt);
        setChiPhi(cp);
    }

    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& maTram) {
        this->maTram = Inputhelper::tuTaoMa(maTram, "BTS");
    }

    Date getNgayBaoTri() const { return ngayBaoTri; }
    void setNgayBaoTri(const Date& ngay) { ngayBaoTri = ngay; }

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
        if (cp < 0) throw Loixacthuc("Loi: Chi phi bao tri khong duoc la so am!");
        chiPhi = cp;
    }

    std::string toFilerecord() const override {
        return id + "|" + maTram + "|" + ngayBaoTri.toString() + "|" + noiDung + "|" + trangThai + "|" + std::to_string(chiPhi);
    }

    void fromFilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 7) {
            setId(data[0]);
            maTram = data[1];
            ngayBaoTri.docchuoi(data[2]);
            noiDung = data[3];
            trangThai = data[4];
            chiPhi = std::stod(data[5]);
        }
    }

    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(10) << maTram << "| "
            << std::setw(12) << ngayBaoTri.toString() << "| "
            << std::setw(22) << noiDung << "| "
            << std::setw(15) << trangThai << "| "
            << std::setw(12) << std::fixed << std::setprecision(0) << chiPhi << " VND" << std::endl;
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma PBT" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(12) << "Ngay Bao Tri" << "| "
            << std::setw(22) << "Noi Dung" << "| "
            << std::setw(15) << "Trang Thai" << "| "
            << std::setw(12) << "Chi Phi" << "\n";
        std::cout << std::string(100, '-') << "\n";
    }
};