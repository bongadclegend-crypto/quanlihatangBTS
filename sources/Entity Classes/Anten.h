
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
    std::string maTram;
    std::string loaiAnten;
    double chieuCao;
    int gocAzimuth;

    static void kiemTraKyTuCam(const std::string& s, const std::string& tenTruong) {
        if (s.find('|') != std::string::npos)
            throw Loixacthuc("Loi: " + tenTruong + " khong duoc chua ky tu '|'!");
    }
public:
    Anten() : chieuCao(0.0), gocAzimuth(0) {}

    Anten(const std::string& ma, const std::string& mt, const std::string& loai,
        double cao, int az) {
        setMa(ma);
        setMaTram(mt);
        setLoaiAnten(loai);
        setChieuCao(cao);
        setGocAzimuth(az);
    }

    static std::string chuanHoaMa(const std::string& s) {
        std::string r = Inputhelper::catkhoangtrang(s);
        for (char& c : r) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return r;
    }

    static std::string taoMaAnten(const std::string& maNhap) {
        std::string clean = chuanHoaMa(maNhap);

        std::string ma = Inputhelper::TuTaoMa(clean, "AT");

        if (ma.size() != 6 || ma.compare(0, 2, "AT") != 0) {
            throw Loixacthuc("Loi: Ma Anten phai gom dung 4 chu so (VD: nhap 0001)!");
        }
        for (size_t i = 2; i < 6; ++i) {
            if (!std::isdigit(static_cast<unsigned char>(ma[i]))) {
                throw Loixacthuc("Loi: Ma Anten phai gom dung 4 chu so (VD: nhap 0001)!");
            }
        }
        return ma;
    }

    static std::string taoMaTram(const std::string& maNhap) {
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
        setId(taoMaAnten(maNhap));
    }

    static double phanTichSoThuc(const std::string& s) {
        if (s.empty()) throw Loixacthuc("Loi: Gia tri phai la so thuc!");
        for (size_t i = 0; i < s.size(); ++i) {
            char c = s[i];
            bool hopLe = std::isdigit(static_cast<unsigned char>(c)) || c == '.' ||
                (i == 0 && (c == '-' || c == '+'));
            if (!hopLe) throw Loixacthuc("Loi: Gia tri phai la so thuc!");
        }
        size_t pos = 0;
        double v = 0.0;
        try { v = std::stod(s, &pos); }
        catch (...) { throw Loixacthuc("Loi: Gia tri phai la so thuc!"); }
        if (pos != s.size()) throw Loixacthuc("Loi: Gia tri co ky tu thua!");
        return v;
    }

    static int phanTichSoNguyen(const std::string& s) {
        size_t pos = 0;
        int v = 0;
        try { v = std::stoi(s, &pos); }
        catch (...) { throw Loixacthuc("Loi: Gia tri phai la so nguyen!"); }
        if (pos != s.size()) throw Loixacthuc("Loi: Gia tri co ky tu thua!");
        return v;
    }

    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& mt) {
        maTram = taoMaTram(mt);
    }

    std::string getLoaiAnten() const { return loaiAnten; }
    void setLoaiAnten(const std::string& loai) {
        std::string clean = Inputhelper::catkhoangtrang(loai);
        if (clean.empty()) throw Loixacthuc("Loi: Loai Anten khong duoc de trong!");
        kiemTraKyTuCam(clean, "Loai Anten");


        loaiAnten = Inputhelper::VietHoaChuDau(clean);
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

    int getGocAzimuth() const { return gocAzimuth; }
    void setGocAzimuth(int az) {
        if (az < 0 || az > 359)
            throw Loixacthuc("Loi: Goc Azimuth phai trong khoang [0, 359] do!");
        gocAzimuth = az;
    }
    double tinhCuLyTamNhinThangToiThietBiDiDong() const {
        // Chieu cao thiet bi thu (dien thoai di dong) co dinh la 0.179 m
        const double hr = 0.179;
        // Cong thuc: r0 = 3.57 * (sqrt(ht) + sqrt(hr))
        // Voi ht chinh la chieuCao cua Anten
        return 3.57 * (std::sqrt(chieuCao) + std::sqrt(hr));
    }
    std::string tofilerecord() const override {
        return id + "|" + maTram + "|" + loaiAnten + "|" +
            chieuCaoToString() + "|" + std::to_string(gocAzimuth);
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() != 5) throw Loixacthuc("Loi: Dong du lieu Anten sai dinh dang!");
        Anten parsed(data[0], data[1], data[2],
            phanTichSoThuc(data[3]), phanTichSoNguyen(data[4]));
        *this = parsed;
    }

    void display() const override {
        std::cout << std::left
            << std::setw(10) << id << "| "
            << std::setw(10) << maTram << "| "
            << std::setw(16) << loaiAnten << "| "
            << std::setw(10) << chieuCaoToString() << "| "
            << std::setw(8) << gocAzimuth << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(10) << "Ma Anten" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(16) << "Loai Anten" << "| "
            << std::setw(10) << "Cao(m)" << "| "
            << std::setw(8) << "Azimuth" << "\n";
        std::cout << std::string(59, '-') << "\n";
    }
};
