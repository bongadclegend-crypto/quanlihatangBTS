//Hoang Tuan Thanh
#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <stdexcept>
#include "Entity.h"
#include "Inputhelper.h"
#include "Date.h"

class HopDongThue : public Entity {
private:
    std::string maTram;       //khoa ngoai tham chieu sang tram Bts
    std::string tenChuNha;
    std::string soDienThoai;
    long long giaThueThang;   // gia thue hang thang
    Date ngayBatDau;          //  dd/mm/yyyy
    int thoiHanThang;         // so thoi han thang thue
public:
    // Constructor mac dinh (danh cho Repository khi doc file)
    HopDongThue() : giaThueThang(0), thoiHanThang(0) {}

    // Constructor day du du lieu
    HopDongThue(std::string maHD, std::string maBTS, std::string ten, std::string sdt, long long gia, std::string ngay, int thoiHan) {
        setId(Inputhelper::TuTaoMa(maHD, "HD"));
        setMaTram(maBTS);
        setTenChuNha(ten);
        setSoDienThoai(sdt);
        setGiaThueThang(gia);
        setNgayBatDau(ngay);
        setThoiHanThang(thoiHan);
    }

    // getter setter
    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& maBTS) {
        maTram = Inputhelper::TuTaoMa(maBTS, "BTS");
    }

    std::string getTenChuNha() const { return tenChuNha; }
    void setTenChuNha(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten);

        if (clean.empty()) throw Loixacthuc("Loi: Ten chu nha khong duoc de trong");
        if (clean.find('|') != std::string::npos) throw Loixacthuc("Loi: Ten chu nha khong duoc chua ky tu | ");
        if (!Inputhelper::chiChuaChuCaiVaKhoangTrang(clean)) {
            throw Loixacthuc("Loi: Ten chu nha chi duoc phep chua chu cai va khoang trang");
        }

        tenChuNha = Inputhelper::VietHoaChuDau(clean);
    }

    std::string getSoDienThoai() const { return soDienThoai; }
    void setSoDienThoai(const std::string& sdt) {
        std::string clean = Inputhelper::catkhoangtrang(sdt);
        Inputhelper::KiemTraSdt(clean);
        soDienThoai = clean;
    }

    long long getGiaThueThang() const { return giaThueThang; }

    // HAM GOC: Nhan so (dung khi doc file)
    void setGiaThueThang(long long gia) {
        if (gia <= 0) throw Loixacthuc("Loi: Gia thue moi thang phai lon hon 0 VND");
        giaThueThang = gia;
    }

    // HAM NAP CHONG: Nhan chuoi (dung khi nguoi dung nhap tu Menu)
    void setGiaThueThang(const std::string& giaStr) {
        std::size_t pos = 0;
        long long gia;
        try {
            gia = std::stoll(giaStr, &pos);
        }
        catch (const std::exception&) {
            throw Loixacthuc("Loi: Gia thue phai la so hop le");
        }
        if (pos != giaStr.length()) {
            throw Loixacthuc("Loi: Gia thue chi duoc phep chua cac chu so (khong co chu cai)");
        }
        setGiaThueThang(gia); // Goi lai ham goc de kiem tra <= 0
    }

    std::string getNgayBatDau() const {
        return ngayBatDau.toString();
    }
    void setNgayBatDau(const std::string& ngay) {
        std::string clean = Inputhelper::catkhoangtrang(ngay);
        if (clean.empty()) throw Loixacthuc("Loi: Ngay bat dau khong duoc de trong");
        ngayBatDau.docchuoi(clean);
    }

    int getThoiHanThang() const { return thoiHanThang; }

    // HAM GOC: Nhan so (dung khi doc file)
    void setThoiHanThang(int thoiHan) {
        if (thoiHan <= 0) throw Loixacthuc("Loi: Thoi han thue phai lon hon 0 thang");
        thoiHanThang = thoiHan;
    }

    // HAM NAP CHONG: Nhan chuoi (dung khi nguoi dung nhap tu Menu)
    void setThoiHanThang(const std::string& thangStr) {
        std::size_t pos = 0;
        int thang;
        try {
            thang = std::stoi(thangStr, &pos);
        }
        catch (const std::exception&) {
            throw Loixacthuc("Loi: Thoi han phai la so hop le");
        }
        if (pos != thangStr.length()) {
            throw Loixacthuc("Loi: Thoi han chi duoc phep chua cac chu so (khong co chu cai)");
        }
        setThoiHanThang(thang); // Goi lai ham goc de kiem tra <= 0
    }

    // doc ghi file txt
    std::string tofilerecord() const override {
        return id + "|" + maTram + "|" + tenChuNha + "|" +
            soDienThoai + "|" + std::to_string(giaThueThang) + "|" +
            ngayBatDau.toString() + "|" + std::to_string(thoiHanThang);
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() != 7) {
            throw Loixacthuc("Loi: Ban ghi Hop dong phai co dung 7 truong");
        }

        long long gia;
        int thoiHan;
        std::size_t vitricuoiGia = 0;
        std::size_t vitricuoiThang = 0;

        try {
            gia = std::stoll(data[4], &vitricuoiGia);
            thoiHan = std::stoi(data[6], &vitricuoiThang);
        }
        catch (const std::invalid_argument&) {
            throw Loixacthuc("Loi: Gia thue hoac thoi han trong file khong phai la so");
        }
        catch (const std::out_of_range&) {
            throw Loixacthuc("Loi: Gia thue hoac thoi han trong file vuot qua gioi han");
        }

        if (vitricuoiGia != data[4].size()) {
            throw Loixacthuc("Loi: Gia thue trong file chua ky tu khong hop le (chu cai hoac ky tu dac biet)!");
        }
        if (vitricuoiThang != data[6].size()) {
            throw Loixacthuc("Loi: Thoi han thue trong file chua ky tu khong hop le!");
        }

        HopDongThue hdTam(data[0], data[1], data[2], data[3], gia, data[5], thoiHan);
        *this = hdTam;
    }

    // Override de trinh bien dich khong bao loi Entity
    void display() const override {
        display("N/A");
    }

    // Hien thi tren console kem Ma Khu Vuc
    void display(const std::string& maKV) const {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(10) << maTram << "| "
            << std::setw(10) << maKV << "| "
            << std::setw(25) << (tenChuNha.length() > 22 ? tenChuNha.substr(0, 19) + "..." : tenChuNha) << "| "
            << std::setw(13) << soDienThoai << "| "
            << std::setw(12) << giaThueThang << "| "
            << std::setw(12) << ngayBatDau.toString() << "| "
            << std::setw(6) << thoiHanThang << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma HD" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(10) << "Ma KV" << "| "
            << std::setw(25) << "Ten Chu Nha" << "| "
            << std::setw(13) << "SDT" << "| "
            << std::setw(12) << "Gia(VND)" << "| "
            << std::setw(12) << "Ngay Thue" << "| "
            << std::setw(6) << "Thang" << "\n";
        std::cout << std::string(105, '-') << "\n";
    }
};
