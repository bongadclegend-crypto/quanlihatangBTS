#pragma once 
#include <iostream>   
#include <iomanip>    
#include <vector>     
#include <string>    
#include "Entity.h"   
#include "InputHelper.h" 
#include "Date.h"     

class ThietBiLapDat : public Entity {
private:
    std::string tenThietBi;
    std::string maLoai;
    std::string maTram;
    std::string maNCC;
    Date ngayLapDat;
    std::string trangThai;

public:
    ThietBiLapDat() {}

    ThietBiLapDat(std::string ma, std::string ten, std::string loai, std::string tram, std::string ncc, std::string ngay, std::string tt) {
        setId(Inputhelper::TuTaoMa(ma, "TB"));
        setTenThietBi(ten);
        setMaLoai(loai);
        setMaTram(tram);
        setMaNCC(ncc);
        setNgayLapDat(ngay);
        setTrangThai(tt);
    }

    std::string getTenThietBi() const { return tenThietBi; }
    void setTenThietBi(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: Ten thiet bi khong duoc de trong!");
        if (!Inputhelper::CoChuaChuCai(clean)) throw Loixacthuc("Loi: Ten thiet bi phai chua it nhat 1 chu cai!");
        if (clean.find('|') != std::string::npos) throw Loixacthuc("Loi: Khong duoc chua ky tu |!");
        tenThietBi = clean;
    }

    std::string getMaLoai() const { return maLoai; }
    void setMaLoai(const std::string& loai) { maLoai = Inputhelper::TuTaoMa(loai, "LT"); }

    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& tram) { maTram = Inputhelper::TuTaoMa(tram, "BTS"); }

    std::string getMaNCC() const { return maNCC; }
    void setMaNCC(const std::string& ncc) { maNCC = Inputhelper::TuTaoMa(ncc, "NCC"); }

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
        if (!Inputhelper::chiChuaChuCaiVaKhoangTrang(clean)) throw Loixacthuc("Loi: Trang thai chi duoc chua chu cai va khoang trang!");
        trangThai = Inputhelper::VietHoaChuDau(clean);
    }

    std::string tofilerecord() const override {
        return id + "|" + tenThietBi + "|" + maLoai + "|" + maTram + "|" + maNCC + "|" + ngayLapDat.toString() + "|" + trangThai;
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 7) {
            setId(data[0]);
            tenThietBi = data[1];
            maLoai = data[2];
            maTram = data[3];
            maNCC = data[4];
            ngayLapDat.docchuoi(data[5]);
            trangThai = data[6];
        }
    }

    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(22) << (tenThietBi.length() > 19 ? tenThietBi.substr(0, 16) + "..." : tenThietBi) << "| "
            << std::setw(10) << maLoai << "| "
            << std::setw(10) << maTram << "| "
            << std::setw(10) << maNCC << "| "
            << std::setw(12) << ngayLapDat.toString() << "| "
            << std::setw(15) << trangThai << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma TB" << "| "
            << std::setw(22) << "Ten Thiet Bi" << "| "
            << std::setw(10) << "Ma Loai" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(10) << "Ma NCC" << "| "
            << std::setw(12) << "Ngay Lap" << "| "
            << std::setw(15) << "Trang Thai" << "\n";
        std::cout << std::string(96, '-') << "\n";
    }
};
