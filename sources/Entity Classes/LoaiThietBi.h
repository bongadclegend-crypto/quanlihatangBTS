//Hoang Tuan Thanh
#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "Entity.h"
#include "Inputhelper.h"
#include "Exceptions.h" // Bat buoc phai co de quuong ra Loixacthuc

class LoaiThietBi : public Entity {
private:
    std::string tenLoai;
    std::string hangSanXuat;
    std::string dacTinhKyThuat;
    double congSuatTieuThu;
    std::string maNCC;
    std::string tenNCC;
public:
    LoaiThietBi() : congSuatTieuThu(0.0) {}

    // Constructor day du
    LoaiThietBi(std::string maLT, std::string ten, std::string hang, std::string dacTinh, double congSuat, std::string mancc, std::string tenncc) {
        setId(Inputhelper::TuTaoMa(maLT, "LT"));
        setTenLoai(ten);
        setHangSanXuat(hang);
        setDacTinhKyThuat(dacTinh);
        setCongSuatTieuThu(congSuat);
        setMaNCC(mancc);
        setTenNCC(tenncc);
    }

    std::string getTenLoai() const { return tenLoai; }
    void setTenLoai(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: Ten loai thiet bi khong duoc de trong!");
        tenLoai = Inputhelper::VietHoaChuDau(clean);
    }

    std::string getHangSanXuat() const { return hangSanXuat; }
    void setHangSanXuat(const std::string& hang) {
        std::string clean = Inputhelper::catkhoangtrang(hang);
        if (clean.empty()) throw Loixacthuc("Loi: Hang san xuat khong duoc de trong!");
        hangSanXuat = Inputhelper::VietHoaChuDau(clean);
    }

    std::string getDacTinhKyThuat() const { return dacTinhKyThuat; }
    void setDacTinhKyThuat(const std::string& dacTinh) {
        std::string clean = Inputhelper::catkhoangtrang(dacTinh);
        if (clean.empty()) throw Loixacthuc("Loi: Dac tinh ky thuat khong duoc de trong!");
        dacTinhKyThuat = clean;
    }

    double getCongSuatTieuThu() const { return congSuatTieuThu; }

    // HAM GOC: Nhan so (dung khi doc file)
    void setCongSuatTieuThu(double congSuat) {
        if (congSuat < 0) throw Loixacthuc("Loi: Cong suat tieu thu phai >= 0W!");
        congSuatTieuThu = congSuat;
    }

    // HAM NAP CHONG: Nhan chuoi thong minh (dung khi Menu truyen vao)
    void setCongSuatTieuThu(const std::string& congSuatStr) {
        std::size_t pos = 0;
        double congSuat;
        try {
            congSuat = std::stod(congSuatStr, &pos);
        }
        catch (const std::exception&) {
            throw Loixacthuc("Loi: Cong suat phai la so hop le");
        }
        if (pos != congSuatStr.length()) {
            throw Loixacthuc("Loi: Cong suat chi duoc phep chua so (va dau cham thap phan)");
        }
        setCongSuatTieuThu(congSuat);
    }

    std::string getMaNCC() const { return maNCC; }
    void setMaNCC(const std::string& ncc) {
        maNCC = Inputhelper::TuTaoMa(ncc, "NCC");
    }

    std::string getTenNCC() const { return tenNCC; }
    void setTenNCC(const std::string& ten) {
        tenNCC = Inputhelper::catkhoangtrang(ten);
    }

    std::string tofilerecord() const override {
        return id + "|" + tenLoai + "|" + hangSanXuat + "|" + dacTinhKyThuat + "|" +
            std::to_string(congSuatTieuThu) + "|" + maNCC + "|" + tenNCC;
    }

    // UPDATE: Ky thuat Deep Copy va bat loi ep kieu an toan tuyet doi
    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() != 7) {
            throw Loixacthuc("Loi: Ban ghi Loai Thiet Bi phai co dung 7 truong");
        }

        double congSuat;
        std::size_t vitricuoi = 0;
        try {
            congSuat = std::stod(data[4], &vitricuoi);
        }
        catch (const std::exception&) {
            throw Loixacthuc("Loi: Cong suat trong file khong phai la so");
        }

        if (vitricuoi != data[4].size()) {
            throw Loixacthuc("Loi: Cong suat trong file chua ky tu khong hop le");
        }

        // Kiem duyet bang doi tuong tam
        LoaiThietBi ltbTam(data[0], data[1], data[2], data[3], congSuat, data[5], data[6]);
        *this = ltbTam;
    }

    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(22) << (tenLoai.length() > 19 ? tenLoai.substr(0, 16) + "..." : tenLoai) << "| "
            << std::setw(15) << (hangSanXuat.length() > 12 ? hangSanXuat.substr(0, 9) + "..." : hangSanXuat) << "| "
            << std::setw(18) << (dacTinhKyThuat.length() > 15 ? dacTinhKyThuat.substr(0, 12) + "..." : dacTinhKyThuat) << "| "
            << std::setw(10) << congSuatTieuThu << "| "
            << std::setw(10) << maNCC << "| "
            << std::setw(18) << (tenNCC.length() > 15 ? tenNCC.substr(0, 12) + "..." : tenNCC) << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma LT" << "| "
            << std::setw(22) << "Ten Loai Thiet Bi" << "| "
            << std::setw(15) << "Hang SX" << "| "
            << std::setw(18) << "Dac Tinh KT" << "| "
            << std::setw(10) << "CongSuat" << "| "
            << std::setw(10) << "Ma NCC" << "| "
            << std::setw(18) << "Ten NCC" << "\n";
        std::cout << std::string(110, '-') << "\n";
    }
};
