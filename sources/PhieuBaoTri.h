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
        setId(Inputhelper::TuTaoMa(maPBT, "PBT"));
        setMaTram(maBTS);
        setNgayBaoTri(ngay);
        setNoiDung(nd);
        setTrangThai(tt);
        setChiPhi(cp);
    }

    // --- CÃC HÃ€M GETTER & SETTER ---

    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& maTram) {
        this->maTram = Inputhelper::TuTaoMa(maTram, "BTS");
    }

    std::string getNgayBaoTri() const { return ngayBaoTri.toString(); }
    void setNgayBaoTri(const std::string& ngay) {
        std::string clean = Inputhelper::catkhoangtrang(ngay);
        if (clean.empty()) throw Loixacthuc("Loi: Ngay bao tri khong duoc de trong!");
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
        if (cp < 0) throw Loixacthuc("Loi: Chi phi bao tri khong duoc la so am!");
        chiPhi = cp;
    }

    // GHI DE (OVERRIDE)

    std::string tofilerecord() const override {
        return id + "|" + maTram + "|" + ngayBaoTri.toString() + "|" + noiDung + "|" + trangThai + "|" + std::to_string(chiPhi);
    }

    void fromfilerecord(const std::string& record) override {
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

    // HIEN THI TREN CONSOLE

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
