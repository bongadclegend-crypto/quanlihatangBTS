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
    // Tinh Truu tuong: Luu ma khoa ngoai (chuoi) de sau nay anh xa, ko can phai luu nguyen doi tuong TramBTS
    std::string maLoai;       // Khóa ngoại tham chiếu bảng LoaiThietBi (LTxxxx)
    std::string maTram;       // Khóa ngoại tham chiếu bảng TramBTS (BTSxxxx)
    Date ngayLapDat;          // Component (Thanh phan): Su dung lop Date de quan ly thoi gian
    std::string trangThai;    // VD: Hoat dong / Bao tri / Loi

public:
    ThietBiLapDat() {}

    ThietBiLapDat(std::string ma, std::string ten, std::string loai, std::string tram, std::string ngay, std::string tt) {
        setId(Inputhelper::TuTaoMa(ma, "TB"));
        setTenThietBi(ten);
        setMaLoai(loai);
        setMaTram(tram);
        setNgayLapDat(ngay);
        setTrangThai(tt);
    }

    // --- GETTER & SETTER ---
    std::string getTenThietBi() const { return tenThietBi; }
    void setTenThietBi(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: Ten thiet bi khong duoc de trong!");
        tenThietBi = clean;
    }

    std::string getMaLoai() const { return maLoai; }
    // Khong can check rong vi ham tuTaoMa se tu bat loi nay
    void setMaLoai(const std::string& loai) { maLoai = Inputhelper::TuTaoMa(loai, "LT"); }

    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& tram) { maTram = Inputhelper::TuTaoMa(tram, "BTS"); }

    std::string getNgayLapDat() const { return ngayLapDat.toString(); }
    void setNgayLapDat(const std::string& ngay) {
        std::string clean = Inputhelper::catkhoangtrang(ngay);
        if (clean.empty()) throw Loixacthuc("Loi: Ngay lap dat khong duoc de trong!");
        // Gọi hàm docchuoi() của lớp Date. Lớp Date sẽ tự động quăng lỗi nếu nhập 30/2
        ngayLapDat.docchuoi(clean);
    }

    std::string getTrangThai() const { return trangThai; }
    void setTrangThai(const std::string& tt) {
        std::string clean = Inputhelper::catkhoangtrang(tt);
        if (clean.empty()) throw Loixacthuc("Loi: Trang thai khong duoc de trong!");
        trangThai = clean;
    }

    // --- GHI ĐÈ FILE ---
    std::string tofilerecord() const override {
        // Goi ngayLapDat.toString() de lay chuoi 10/10/2026 nhet vao file
        return id + "|" + tenThietBi + "|" + maLoai + "|" + maTram + "|" + ngayLapDat.toString() + "|" + trangThai;
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 6) { // Bang nay co 6 cot nen check size >= 6
            setId(data[0]);
            tenThietBi = data[1];
            maLoai = data[2];
            maTram = data[3];
            ngayLapDat.docchuoi(data[4]); // Đẩy ngược chuỗi từ file txt vào class Date
            trangThai = data[5];
        }
    }

    // --- HIỂN THỊ CONSOLE ---
    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(25) << (tenThietBi.length() > 22 ? tenThietBi.substr(0, 19) + "..." : tenThietBi) << "| "
            << std::setw(10) << maLoai << "| "
            << std::setw(10) << maTram << "| "
            << std::setw(12) << ngayLapDat.toString() << "| "
            << std::setw(15) << trangThai << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma TB" << "| "
            << std::setw(25) << "Ten Thiet Bi" << "| "
            << std::setw(10) << "Ma Loai" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(12) << "Ngay Lap" << "| "
            << std::setw(15) << "Trang Thai" << "\n";
        std::cout << std::string(86, '-') << "\n";
    }
};
