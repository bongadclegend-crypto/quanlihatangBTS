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
    std::string maTram;
    std::string maNCC;
    Date ngayLapDat;
    std::string trangThai;

public:
    ThietBiLapDat() {}

    // Đã xóa tham số loai và hàm setMaLoai
    ThietBiLapDat(std::string ma, std::string ten, std::string tram, std::string ncc, std::string ngay, std::string tt) {
        setId(Inputhelper::TuTaoMa(ma, "TB"));
        setTenThietBi(ten);
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

    // Xóa maLoai khỏi chuỗi lưu file
    std::string tofilerecord() const override {
        return id + "|" + tenThietBi + "|" + maTram + "|" + maNCC + "|" + ngayLapDat.toString() + "|" + trangThai;
    }

    // Cập nhật lại logic đọc file, giảm size xuống 6 và lùi các index lại
    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 6) {
            setId(data[0]);
            tenThietBi = data[1];
            maTram = data[2];
            maNCC = data[3];
            ngayLapDat.docchuoi(data[4]);
            trangThai = data[5];
        }
    }

    // Xóa cột maLoai khỏi bảng hiển thị
    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(22) << (tenThietBi.length() > 19 ? tenThietBi.substr(0, 16) + "..." : tenThietBi) << "| "
            << std::setw(10) << maTram << "| "
            << std::setw(10) << maNCC << "| "
            << std::setw(12) << ngayLapDat.toString() << "| "
            << std::setw(15) << trangThai << "\n";
    }

    // Cập nhật lại Header, thu ngắn đường kẻ ngang
    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma TB" << "| "
            << std::setw(22) << "Ten Thiet Bi" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(10) << "Ma NCC" << "| "
            << std::setw(12) << "Ngay Lap" << "| "
            << std::setw(15) << "Trang Thai" << "\n";
        std::cout << std::string(84, '-') << "\n";
    }
};
