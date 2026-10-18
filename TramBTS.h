
#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include "Entity.h"
#include "Inputhelper.h"

class TramBTS : public Entity {
private:
    std::string tenTram;
    std::string loaiCot;       // Cột tự dựng / Dây neo / Ngụy trang
    double banKinhPhuSong;     // Đơn vị: km (phải > 0)
    std::string trangThai;     // VD: Hoat dong, Bao tri, Loi

    //ham kiem tra va chuan hoa ma tram BTS
    static std::string chuanHoaMaBTS(const std::string& ma) {
        std::string maChuan = Inputhelper::tuTaoMa(ma, "BTS");

        if (maChuan.size() != 7 || maChuan.substr(0, 3) != "BTS") {
            throw Loixacthuc("Loi: Ma tram phai co dang BTS + 4 chu so!");
        }

        for (std::size_t i = 3; i < maChuan.size(); i++) {
            if (maChuan[i] < '0' || maChuan[i] > '9') {
                throw Loixacthuc("Loi: Ma tram phai co 4 chu so sau BTS!");
            }
        }

        return maChuan;
    }

public:
    // constructor mac dinh giup doc file
    TramBTS() : banKinhPhuSong(0.0) {}

    // constructor ham khoi tao co so (Thay chieuCao và maKV bằng banKinh và trangThai)
    TramBTS(std::string ma, std::string ten, std::string loai,
            double banKinh, std::string tt) {

        setId(chuanHoaMaBTS(ma));
        setTenTram(ten);
        setLoaiCot(loai);
        setBanKinhPhuSong(banKinh);
        setTrangThai(tt);
    }

    // getter setter
    std::string getTenTram() const {
        return tenTram;
    }

    void setTenTram(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten);

        if (clean.empty()) {
            throw Loixacthuc("Loi: Ten Tram BTS khong duoc de trong!");
        }

        // BẮT LỖI: Không cho phép nhập toàn số (VD: "0000")
        if (!Inputhelper::coChuaChuCai(clean)) {
            throw Loixacthuc("Loi: Ten Tram phai chua it nhat 1 chu cai!");
        }

        if (clean.find('|') != std::string::npos) {
            throw Loixacthuc("Loi: Ten tram khong duoc chua ky tu | !");
        }

        // Tự động viết hoa chữ cái đầu
        tenTram = Inputhelper::vietHoaChuDau(clean);
    }

    std::string getLoaiCot() const {
        return loaiCot;
    }

    void setLoaiCot(const std::string& loai) {
        std::string clean = Inputhelper::catkhoangtrang(loai);

        if (clean.empty()) {
            throw Loixacthuc("Loi: Loai cot khong duoc de trong!");
        }

        if (!Inputhelper::chiChuaChuCaiVaKhoangTrang(clean)) {
            throw Loixacthuc("Loi: Loai cot chi duoc phep chua chu cai va khoang trang!");
        }

        // Tự động viết hoa chữ cái đầu
        loaiCot = Inputhelper::vietHoaChuDau(clean);
    }

    double getBanKinhPhuSong() const {
        return banKinhPhuSong;
    }

    void setBanKinhPhuSong(double banKinh) {
        if (!std::isfinite(banKinh) || banKinh <= 0.0) {
            throw Loixacthuc("Loi: Ban kinh phu song phai la so duong hop le!");
        }

        banKinhPhuSong = banKinh;
    }

    std::string getTrangThai() const {
        return trangThai;
    }

    void setTrangThai(const std::string& tt) {
        std::string clean = Inputhelper::catkhoangtrang(tt);

        if (clean.empty()) {
            throw Loixacthuc("Loi: Trang thai khong duoc de trong!");
        }

        if (!Inputhelper::coChuaChuCai(clean)) {
            throw Loixacthuc("Loi: Trang thai phai chua it nhat 1 chu cai!");
        }

        if (clean.find('|') != std::string::npos) {
            throw Loixacthuc("Loi: Trang thai khong duoc chua ky tu | !");
        }

        // Tự động viết hoa chữ cái đầu (VD: "hoat dong" -> "Hoat Dong")
        trangThai = Inputhelper::vietHoaChuDau(clean);
    }

    // cap nhat doc va ghi file
    std::string toFilerecord() const override {
        return id + "|" + tenTram + "|" + loaiCot + "|"
            + std::to_string(banKinhPhuSong) + "|" + trangThai;
    }

    void fromFilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');

        if (data.size() != 5) {
            throw Loixacthuc("Loi: Ban ghi tram BTS phai co dung 5 truong!");
        }

        double banKinh;
        std::size_t viTriCuoi = 0;

        try {
            banKinh = std::stod(data[3], &viTriCuoi);
        }
        catch (const std::invalid_argument&) {
            throw Loixacthuc("Loi: Ban kinh phu song phai la so!");
        }
        catch (const std::out_of_range&) {
            throw Loixacthuc("Loi: Ban kinh phu song vuot qua pham vi!");
        }

        if (viTriCuoi != data[3].size()) {
            throw Loixacthuc("Loi: Ban kinh phu song chua ky tu khong hop le!");
        }

        //tao doi tuong tam de kiem tra du lieu truoc
        TramBTS tramTam(data[0], data[1], data[2], banKinh, data[4]);

        //chi cap nhat doi tuong khi du lieu da hop le
        *this = tramTam;
    }

    // giao dien hien thi con sole
    void display() const override {
        std::cout << std::left
            << std::setw(10) << id << "| "
            << std::setw(25) << (tenTram.length() > 22 ? tenTram.substr(0, 19) + "..." : tenTram) << "| "
            << std::setw(15) << loaiCot << "| "
            << std::setw(14) << banKinhPhuSong << "| "
            << std::setw(15) << trangThai << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(25) << "Ten Tram" << "| "
            << std::setw(15) << "Loai Cot" << "| "
            << std::setw(14) << "Ban Kinh(km)" << "| "
            << std::setw(15) << "Trang Thai" << "\n";

        std::cout << std::string(87, '-') << "\n";
    }
};
