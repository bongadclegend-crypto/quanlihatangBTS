#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <vector>
#include <cmath>
#include "Exceptions.h"
#include "Entity.h"
#include "Inputhelper.h"

class Anten : public Entity {
private:
    std::string maTram; // ma tram BTS ma anten thuoc ve
    std::string loaiAnten; // Loai anten 
    double chieuCao; // do cao treo anten
    double gocAzimuth; // goc phuong vi 

    static void kiemTraKyTuCam(const std::string& s, const std::string& tenTruong) {
        if (s.find('|') != std::string::npos)
            throw Loixacthuc("Loi: " + tenTruong + " khong duoc chua ky tu '|'");
    }
public:
    Anten() : chieuCao(0.0), gocAzimuth(0) {} // thuoc tinh mac dinh cua anten, chieu cao 0.0 va goc azimuth 0

    Anten(const std::string& ma, const std::string& mt, const std::string& loai, // ham khoi tao doi tuong anten, nhan vao ma, ma tram, loai anten, chieu cao va goc azimuth
        double cao, double az) {
        setMa(ma);
        setMaTram(mt);
        setLoaiAnten(loai);
        setChieuCao(cao);
        setGocAzimuth(az);
    }

    static std::string chuanHoaMa(const std::string& s) { // ham de chuan hoa ma antenloai bo khoang trang va viet hoa chu cai
        std::string r = Inputhelper::catkhoangtrang(s);
        for (char& c : r) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return r;
    }

    static std::string taoMaAnten(const std::string& maNhap) { // chi can nhap 4 so thi ham nay se tu dong them AT vao dau
        std::string clean = chuanHoaMa(maNhap);
        std::string ma = Inputhelper::TuTaoMa(clean, "AT");
        if (ma.size() != 6 || ma.compare(0, 2, "AT") != 0) { // neu nhap ki tu khong phai la so thi nem ngoai le
            throw Loixacthuc("Loi: Ma Anten phai gom dung 4 chu so (vi du: nhap 0001)");
        }
        for (size_t i = 2; i < 6; ++i) {
            if (!std::isdigit(static_cast<unsigned char>(ma[i]))) { // neu co ky tu khong phai la so thi nem ngoai le
                throw Loixacthuc("Loi: Ma Anten phai gom dung 4 chu so (vi du: nhap 0001)");
            }
        }
        return ma; // tra ve ma anten hop le
    }

    static std::string taoMaTram(const std::string& maNhap) { // chi can nhap 4 so ma tram se tu dong them BTS vao dau ma tram
        std::string clean = chuanHoaMa(maNhap);
        std::string ma = Inputhelper::TuTaoMa(clean, "BTS");
        if (ma.size() != 7 || ma.compare(0, 3, "BTS") != 0) {
            throw Loixacthuc("Loi: Ma tram BTS phai gom dung 4 chu so (vi du: nhap 1001)"); // kiem tra tuong tu nhu ma anten, neu khong hop le thi nem ngoai le
        }
        for (size_t i = 3; i < 7; ++i) {
            if (!std::isdigit(static_cast<unsigned char>(ma[i]))) {
                throw Loixacthuc("Loi: Ma tram BTS phai gom dung 4 chu so (vi du: nhap 1001)");
            }
        }
        return ma;
    }

    // ktra thong so cua anten co phai so thuc hay khong neu khong thi nem ngoai le
    // phan tich so thuc tu dong doi dau phay thanh dau cham 
    static double phanTichSoThuc(const std::string& input) {
        std::string s = Inputhelper::catkhoangtrang(input);
        for (char& c : s) {
			if (c == ',') c = '.'; // doi dau phay thanh dau cham
        }
        try {
            size_t pos;
            double val = std::stod(s, &pos);
            if (pos != s.length()) throw std::exception();
            return val;
        }
        catch (...) {
            throw Loixacthuc("Loi: Gia tri phai la so thuc (vi du: 45.5 hoac 45,5)");
        }
    }

    void setMa(const std::string& maNhap) { // chuan hoa ma 
        setId(taoMaAnten(maNhap));
    }

    static int phanTichSoNguyen(const std::string& s) {
        size_t pos = 0;
        int v = 0;
        try { v = std::stoi(s, &pos); }
        catch (...) { throw Loixacthuc("Loi: Gia tri phai la so "); }
        if (pos != s.size()) throw Loixacthuc("Loi: Gia tri co ky tu khong hop le ");
        return v;
    }
    
    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& mt) {
        maTram = taoMaTram(mt);
    }

    static std::vector<std::string> danhSachAntenHopLe(const std::string& loaiCot) {
        if (loaiCot == "Cot Tu Dung") {
            return { "Anten Ma Tran 5G", "Anten Chao Viba", "Anten Loa" };
        }
        else if (loaiCot == "Cot Day Co") {
            return { "Anten Quat 4G", "Anten Yagi", "Anten GPS" };
        }
        else if (loaiCot == "Cot Than Ong") {
            return { "Anten Quat 3G", "Anten Vi Mo" };
        }
        else if (loaiCot == "Cot Tren Mai") {
            return { "Anten Op Tuong", "Anten Chao Mini" };
        }
        else if (loaiCot == "Cot Nguy Trang") {
            return { "Anten Dang Huong", "Anten Ong Khoi", "Anten Roi" };
        }
        return {}; // Truong hop loi, tra ve mang rong
    }

    std::string getLoaiAnten() const { return loaiAnten; } // ham lay loai anten

    void setLoaiAnten(const std::string& loai, const std::string& loaiCot = "") { // ham set loai anten, neu loai cot khong rong thi kiem tra xem loai anten co hop le voi loai cot hay khong
        std::string clean = Inputhelper::catkhoangtrang(loai);
        if (clean.empty()) throw Loixacthuc("Loi: Loai Anten khong duoc de trong"); // phai dien vao loai ant
        kiemTraKyTuCam(clean, "Loai Anten");

        std::string loaiChuan = Inputhelper::VietHoaChuDau(clean);

		if (!loaiCot.empty()) { // neu loai cot khong rong thi kiem tra xem loai anten co hop le voi loai cot hay khong
            std::vector<std::string> hopLe = danhSachAntenHopLe(loaiCot);
            bool ok = false;
            for (const auto& a : hopLe) {
                if (loaiChuan == a) { ok = true; break; }
            }
            if (!ok) {
                throw Ngoailerangbuoc("Tu choi: [" + loaiCot + "] khong ho tro lap dat [" + loaiChuan + "]!");
            }
        }
        loaiAnten = loaiChuan;
    }

    double getChieuCao() const { return chieuCao; }
    void setChieuCao(double cao) {
        if (!(cao >= 5.0 && cao <= 150.0))
            throw Loixacthuc("Loi: Do cao cot anten phai trong khoang [5.0, 150.0] m!");
        chieuCao = cao;
    }

    std::string chieuCaoToString() const {
        std::ostringstream os;
        os << std::setprecision(12) << chieuCao;
        return os.str();
    }

    double getGocAzimuth() const { return gocAzimuth; }
    void setGocAzimuth(double az) {
        if (az < 0 || az > 359)
            throw Loixacthuc("Loi: Goc Azimuth phai trong khoang [0, 359] do");
        gocAzimuth = az;
    }

    std::string gocToString() const {
        std::string s = std::to_string(gocAzimuth);
        s.erase(s.find_last_not_of('0') + 1, std::string::npos);
        if (s.back() == '.') s.pop_back();
        return s;
    }

    double tinhCuLyTamNhinThangToiThietBiDiDong() const {
        const double hr = 1.5; // Do cao thiet bi thu 1.5m
        return 3.57 * (std::sqrt(chieuCao) + std::sqrt(hr));
    }

    std::string tofilerecord() const override {
        return id + "|" + maTram + "|" + loaiAnten + "|" +
            chieuCaoToString() + "|" + gocToString();
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() != 5) throw Loixacthuc("Loi: du lieu anten sai dinh dang");
        Anten parsed(data[0], data[1], data[2],
            phanTichSoThuc(data[3]), phanTichSoThuc(data[4]));
        *this = parsed;
    }

    void display() const override {
        std::cout << std::left
            << std::setw(10) << id << "| "
            << std::setw(10) << maTram << "| "
            << std::setw(22) << (loaiAnten.length() > 20 ? loaiAnten.substr(0, 17) + "..." : loaiAnten) << "| "
            << std::setw(10) << chieuCaoToString() << "| "
            << std::setw(8) << gocToString() << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(10) << "Ma Anten" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(22) << "Loai Anten" << "| "
            << std::setw(10) << "Do cao treo Anten" << "| "
            << std::setw(8)  << "Azimuth" << "\n";
        std::cout << std::string(68, '-') << "\n";
    }
};
