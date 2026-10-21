#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
#include "Exceptions.h"
#include "Entity.h"
#include "Inputhelper.h" 

class KhuVuc : public Entity { // ke thua lop entity 
private: // cac thuoc tinh trong lop khu vuc
    std::string tenKhuVuc;
    std::string nguoiQuanLy;
    std::string maTram;

    // doan nay dung stactic de tiet kiem bo nho 
 
    static void kiemTraChiChuaChu(const std::string& s, const std::string& tenTruong) {
        for (char c : s) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (std::isdigit(uc)) {
                throw Loixacthuc("Loi: " + tenTruong + " chi duoc nhap chu, khong duoc chua chu so!"); // neu co chu so thi bao loi
            }
            if (!std::isalpha(uc) && !std::isspace(uc)) {
                throw Loixacthuc("Loi: " + tenTruong + " chi duoc chua chu cai va khoang trang!");
            }
        }
    }
    // cai nay de check xem neu nhap so thi se bao loi va chi duoc nhap chu 

public: // cac ham khoi tao 
    KhuVuc() = default; 

    //khoi tao co tham so de khoi tao doi tuong khi doc file
    KhuVuc(const std::string& ma, const std::string& ten, const std::string& ql, const std::string& qlTram) {
        
        setMa(ma);
        setTenKhuVuc(ten);
        setNguoiQuanLy(ql);
        setMaTram(qlTram);
    }

    static std::string chuanHoaMa(const std::string& s) {
        std::string r = Inputhelper::catkhoangtrang(s);
        for (char& c : r) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return r;
    }

    // ham nay de tao ma khu vuc va kiem tra xem co dung dinh dang hay khong
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
    } // chi can nhap 4 chu so thi se tu dong them tien to KV vao truoc va kiem tra xem co dung dinh dang hay khong


    static std::string taoMaTram(const std::string& maNhap) { // tuong tu nhu ham tao ma khu vuc nhung tien to la BTS
        std::string clean = chuanHoaMa(maNhap);
        std::string ma = Inputhelper::TuTaoMa(clean, "BTS");

        if (ma.size() != 7 || ma.compare(0, 3, "BTS") != 0) {
            throw Loixacthuc("Loi: Ma Tram BTS phai gom dung 4 chu so (VD: nhap 1001)!");
        }
        for (size_t i = 3; i < 7; ++i) {
            if (!std::isdigit(static_cast<unsigned char>(ma[i]))) {
                throw Loixacthuc("Loi: Ma Tram BTS phai gom dung 4 chu so (VD: nhap 1001)!");
            }
        }
        return ma;
    }

    void setMa(const std::string& maNhap) {
        setId(taoMaKV(maNhap));
    }
    // de tao ma khu vuc va kiem tra xem co dung dinh dang hay khong

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
            // neu chua co nguoi quan ly thi se thong bao la chua phan cong
            nguoiQuanLy = "Chua phan cong";
        }
        else {
            kiemTraChiChuaChu(clean, "Nguoi quan ly");
            nguoiQuanLy = Inputhelper::VietHoaChuaDau(clean);
        }
    }

    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& mt) {
        if (mt.empty() || mt == "Chua xac dinh") {
            maTram = "Chua xac dinh";
        }
        else {
            maTram = taoMaTram(mt);
        }
    }
    std::string tofilerecord() const override {
        return id + "|" + tenKhuVuc + "|" + nguoiQuanLy + "|" + maTram;
    }

    void fromfilerecord(const std::string& record) override {
        
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        // kiem tra du 4 truong (id, tenKhuVuc, nguoiQuanLy, maTram)
        if (data.size() != 4) throw Loixacthuc("Loi: Dong du lieu Khu vuc sai dinh dang (Can 4 truong)!");
        KhuVuc parsed(data[0], data[1], data[2], data[3]);
        *this = parsed;
    } // ham nay de doc va ghi file txt
        void display() const override {
        // GIẢI THÍCH: Toán tử 3 ngôi (dieu_kien ? dung : sai). Nếu tên dài quá 22 ký tự thì cắt lấy 19 ký tự đầu rồi nối "...". Nếu không thì in bình thường.
        std::cout << std::left
            << std::setw(10) << id << "| "
            << std::setw(24) << (tenKhuVuc.length() > 22 ? tenKhuVuc.substr(0, 19) + "..." : tenKhuVuc) << "| "
            << std::setw(20) << (nguoiQuanLy.length() > 18 ? nguoiQuanLy.substr(0, 15) + "..." : nguoiQuanLy) << "| "
            << std::setw(12) << maTram << "\n";
    }
    static void displayHeader() {
        std::cout << std::left
            << std::setw(10) << "Ma KV" << "| "
            << std::setw(24) << "Ten Khu Vuc / Tinh" << "| "
            << std::setw(20) << "Nguoi Quan Ly" << "| "
            << std::setw(12) << "Ma Tram" << "\n";
        std::cout << std::string(72, '-') << "\n";
    } // nay de hien thi tieu de cua bang 
};
