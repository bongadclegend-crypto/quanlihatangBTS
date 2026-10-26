// Nguyen Cong Vinh 

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
#include "Exceptions.h"
#include "Entity.h"
#include "Inputhelper.h" 

class KhuVuc :
    public Entity {
private:
    std::string tenKhuVuc;
    std::string nguoiQuanLy;
    std::string maTram;

    static void kiemTraChiChuaChu(const std::string& s, const std::string& tenTruong) { // ham kiem tra chuoi chi chua chu cai va khoang trang 
        for (char c : s) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (std::isdigit(uc)) { //neu chua so thi nem ngoai le
                throw Loixacthuc("Loi: " + tenTruong + " chi duoc nhap chu, khong duoc chua chu so");
            }
            if (!std::isalpha(uc) && !std::isspace(uc)) { // neu chua ky tu dac biet thi nem ngoai le
                throw Loixacthuc("Loi: " + tenTruong + " chi duoc chua chu cai va khoang trang");
            }
        }
    }

public:
    KhuVuc() = default; // nghia la ham khoi tao mac dinh cua doi tuong khu vuc ko lam gi ca

    KhuVuc(const std::string& ma, const std::string& ten, const std::string& ql, const std::string& qlTram) { //nay la ham khoi tao doi tuong khu vuc, nhan vao ma, ten, nguoi quan ly va ma tram
        setMa(ma);
        setTenKhuVuc(ten);
        setNguoiQuanLy(ql);
        setMaTram(qlTram);
    }

    static std::string chuanHoaMa(const std::string& s) { // ham chuan hoa ma khu vuc, loai bo khoang trang va viet hoa chu cai
        std::string r = Inputhelper::catkhoangtrang(s);
        for (char& c : r) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return r;
    }

    static std::string taoMaKV(const std::string& maNhap) { // ham tao ma khu vuc tu ma nhap vao, neu ma nhap ko hop le thi nem ngoai le
        std::string clean = chuanHoaMa(maNhap);
        std::string ma = Inputhelper::TuTaoMa(clean, "KV");

        if (ma.size() != 6 || ma.compare(0, 2, "KV") != 0) { // neu nhap ki tu khong phai la so thi nem ngoai le
            throw Loixacthuc("Loi: Ma KV phai gom dung 4 chu so (vi du: nhap 0001)");
        }
        for (size_t i = 2; i < 6; ++i) {
            if (!std::isdigit(static_cast<unsigned char>(ma[i]))) {
                throw Loixacthuc("Loi: Ma KV phai gom dung 4 chu so (vi du: nhap 0001)");
            }
        }
        return ma;
    }

    static std::string taoMaTram(const std::string& maNhap) { // ham tao ma tram tu ma nhap vao, neu ma nhap ko hop le thi nem ngoai le
        std::string clean = chuanHoaMa(maNhap);
        std::string ma = Inputhelper::TuTaoMa(clean, "BTS"); // chi can nhap 4 so ma tram se tu dong them BTS vao dau ma tram

        if (ma.size() != 7 || ma.compare(0, 3, "BTS") != 0) {
            throw Loixacthuc("Loi: ma tram BTS phai gom dung 4 chu so (vi du: nhap 1001)");
        }
        for (size_t i = 3; i < 7; ++i) {
            if (!std::isdigit(static_cast<unsigned char>(ma[i]))) { // neu co ky tu khong phai la so thi nem ngoai le
                throw Loixacthuc("Loi: ma tram BTS phai gom dung 4 chu so (vi du: nhap 1001)");
            }
        }
        return ma;
    }

    void setMa(const std::string& maNhap) { // ham set ma khu vuc, neu ma nhap ko hop le thi nem ngoai le
        setId(taoMaKV(maNhap));
    }

    std::string getTenKhuVuc() const { return tenKhuVuc; } // ham lay ten khu vuc

    void setTenKhuVuc(const std::string& ten) { // 
        std::string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: ten khu vuc khong duoc de trong");
        kiemTraChiChuaChu(clean, "ten khu vuc");

        std::string tenChuanHoa = Inputhelper::VietHoaChuDau(clean); // neu viet chu dau la chu thuong thi chuyen sang viet hoa chu cai dau tien cua moi tu

        tenKhuVuc = tenChuanHoa; // Gan truc tiep vao thuoc tinh, bo het cac lenh kiem tra mang 12 quan
    }

    std::string getNguoiQuanLy() const { return nguoiQuanLy; } // ham lay nguoi quan ly
    void setNguoiQuanLy(const std::string& ql) {
        std::string clean = Inputhelper::catkhoangtrang(ql);
        if (clean.empty()) {
			nguoiQuanLy = "Chua phan cong"; // neu nguoi quan ly chua duoc nhap thi gan nguoi quan ly la "Chua phan cong"
        }
        else {
            kiemTraChiChuaChu(clean, "Nguoi quan ly");  // phan nay su dung ham kiem tra chi chua chu cai va khoang trang o Inputhelper de kiem tra nguoi quan ly co nhap dung hay khong
            nguoiQuanLy = Inputhelper::VietHoaChuDau(clean);
        }
    }

    std::string getMaTram() const { return maTram; } // ham lay ma tram
    void setMaTram(const std::string& mt) {
        if (mt.empty() || mt == "Chua xac dinh") { // neu ma tram chua duoc nhap thi gan ma tram la "Chua xac dinh"
            maTram = "Chua xac dinh";
        }
        else {
            maTram = taoMaTram(mt);
        }
    }

    std::string tofilerecord() const override { // ham chuyen doi doi tuong thanh chuoi de luu vao file
        return id + "|" + tenKhuVuc + "|" + nguoiQuanLy + "|" + maTram;  // tra ve chuoi bieu dien de luu vao ban ghi
    }

    void fromfilerecord(const std::string& record) override { // ham doc chuoi banghi va nap du lieu vao doi tuong
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() != 4) throw Loixacthuc("Loi: Dong du lieu Khu vuc sai dinh dang (Can 4 truong)"); // neu so truong khong bang 4 thi nem ngoai le 
        KhuVuc parsed(data[0], data[1], data[2], data[3]);
        *this = parsed;
    }

    void display() const override { // ham hien thi doi tuong
        std::cout << std::left
            << std::setw(10) << id << "| "
            << std::setw(24) << (tenKhuVuc.length() > 22 ? tenKhuVuc.substr(0, 19) + "..." : tenKhuVuc) << "| "
            << std::setw(20) << (nguoiQuanLy.length() > 18 ? nguoiQuanLy.substr(0, 15) + "..." : nguoiQuanLy) << "| "
            << std::setw(12) << maTram << "\n";
    }

    static void displayHeader() { // hien thi tieu de bang
        std::cout << std::left
            << std::setw(10) << "Ma KV" << "| "
            << std::setw(24) << "Ten Khu Vuc" << "| "
            << std::setw(20) << "Nguoi Quan Ly" << "| "
            << std::setw(12) << "Ma Tram" << "\n";
        std::cout << std::string(72, '-') << "\n";
    }
};
