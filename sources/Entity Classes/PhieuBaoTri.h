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
    std::string maTB;  //khoa ngoai thietbilapdat
    std::string maNCC;
    Date ngayBaoTri;
    std::string noiDung;
    std::string trangThai;
    double chiPhi;

public:
    PhieuBaoTri() { chiPhi = 0; }

    
    PhieuBaoTri(std::string maPBT, std::string maBTS, std::string mTB, std::string mNCC, std::string ngay, std::string nd, std::string tt, double cp) {
        setId(Inputhelper::TuTaoMa(maPBT, "PBT"));
        setMaTram(maBTS);
        setMaTB(mTB);
        setMaNCC(mNCC);
        setNgayBaoTri(ngay);
        setNoiDung(nd);
        setTrangThai(tt);
        setChiPhi(cp);
    }

    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& maTram) { this->maTram = Inputhelper::TuTaoMa(maTram, "BTS"); }

  
    std::string getMaTB() const { return maTB; }
    void setMaTB(const std::string& mTB) { this->maTB = Inputhelper::TuTaoMa(mTB, "TB"); }

    std::string getMaNCC() const { return maNCC; }
    void setMaNCC(const std::string& maNCC) { this->maNCC = Inputhelper::TuTaoMa(maNCC, "NCC"); }

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

    
    std::string tofilerecord() const override {
        return id + "|" + maTram + "|" + maTB + "|" + maNCC + "|" + ngayBaoTri.toString() + "|" + noiDung + "|" + trangThai + "|" + std::to_string(chiPhi);
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 8) {
            setId(data[0]);
            maTram = data[1];
            maTB = data[2];
            maNCC = data[3];
            ngayBaoTri.docchuoi(data[4]);
            noiDung = data[5];
            trangThai = data[6];
            chiPhi = std::stod(data[7]);
        }
    }

    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(10) << maTram << "| "
            << std::setw(10) << maTB << "| "
            << std::setw(10) << maNCC << "| "
            << std::setw(14) << ngayBaoTri.toString() << "| "
            << std::setw(18) << (noiDung.length() > 15 ? noiDung.substr(0, 12) + "..." : noiDung) << "| "
            << std::setw(12) << trangThai << "| "
            << std::setw(10) << std::fixed << std::setprecision(0) << chiPhi << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma PBT" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(10) << "Ma TB" << "| "
            << std::setw(10) << "Ma NCC" << "| "
            << std::setw(14) << "Ngay Bao Tri" << "| "
            << std::setw(18) << "Noi Dung" << "| "
            << std::setw(12) << "Trang Thai" << "| "
            << std::setw(10) << "Chi Phi" << "\n";
        std::cout << std::string(105, '-') << "\n";
    }
};
