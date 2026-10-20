// Tac gia: Nguyen Cong Vinh
// code moi ngay 20

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
#include "Exceptions.h"
#include "Entity.h"
#include "Inputhelper.h" 

class KhuVuc : public Entity {
private:
    std::string tenKhuVuc;
    std::string nguoiQuanLy;

    static void kiemTraChiChuaChu(const std::string& s, const std::string& tenTruong) {
        for (char c : s) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (std::isdigit(uc)) {
                throw Loixacthuc("Loi: " + tenTruong + " chi duoc nhap chu, khong duoc chua chu so!");
            }
            if (!std::isalpha(uc) && !std::isspace(uc)) {
                throw Loixacthuc("Loi: " + tenTruong + " chi duoc chua chu cai va khoang trang!");
            }
        }
    }
public:
    KhuVuc() = default;

    KhuVuc(const std::string& ma, const std::string& ten, const std::string& ql) {
        setMa(ma);
        setTenKhuVuc(ten);
        setNguoiQuanLy(ql);
    }

    static std::string chuanHoaMa(const std::string& s) {
        std::string r = Inputhelper::catkhoangtrang(s);
        for (char& c : r) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return r;
    }

    static std::string taoMaKV(const std::string& maNhap) {
        std::string clean = chuanHoaMa(maNhap);
        std::string ma = Inputhelper::TuTaoMa(clean, "KV");

        if (ma.size() != 6 || ma.compare(0, 2, "KV") != 0) {
            throw Loixacthuc("Loi: Ma KV phai gom dung 4 chu so (VD: nhap 0001)!");
        }
        for (size_t i = 2; i < 6; ++i) {
            if (!std::isdigit(static_cast<unsigned char>(ma[i]))) {
                throw Loixacthuc("Loi: Ma KV phai gom dung 4 chu so (VD: nhap 0001)!");
            }
        }
        return ma;
    }

    void setMa(const std::string& maNhap) {
        setId(taoMaKV(maNhap));
    }

    std::string getTenKhuVuc() const { return tenKhuVuc; }
    void setTenKhuVuc(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: Ten khu vuc khong duoc de trong!");
        kiemTraChiChuaChu(clean, "Ten khu vuc");
        
        tenKhuVuc = Inputhelper::VietHoaChuaDau(clean);
    }

    std::string getNguoiQuanLy() const { return nguoiQuanLy; }
    void setNguoiQuanLy(const std::string& ql) {
        std::string clean = Inputhelper::catkhoangtrang(ql);
        if (clean.empty()) {
            nguoiQuanLy = "Chua phan cong";
        }
        else {
            kiemTraChiChuaChu(clean, "Nguoi quan ly");
            
            nguoiQuanLy = Inputhelper::VietHoaChuaDau(clean);
        }
    }

    
    std::string tofilerecord() const override {
        return id + "|" + tenKhuVuc + "|" + nguoiQuanLy;
    }

    
    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() != 3) throw Loixacthuc("Loi: Dong du lieu Khu vuc sai dinh dang!");
        KhuVuc parsed(data[0], data[1], data[2]);
        *this = parsed;
    }

    void display() const override {
        std::cout << std::left
            << std::setw(10) << id << "| "
            << std::setw(24) << (tenKhuVuc.length() > 22 ? tenKhuVuc.substr(0, 19) + "..." : tenKhuVuc) << "| "
            << std::setw(25) << nguoiQuanLy << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(10) << "Ma KV" << "| "
            << std::setw(24) << "Ten Khu Vuc / Tinh" << "| "
            << std::setw(25) << "Nguoi Quan Ly" << "\n";
        std::cout << std::string(62, '-') << "\n";
    }
};
