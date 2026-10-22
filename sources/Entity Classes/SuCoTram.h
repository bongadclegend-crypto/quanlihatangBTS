// Kieu Binh Nhan B24DCVT285
#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "Entity.h"
#include "InputHelper.h"
#include "Date.h"
// Lop quan ly thong tin su co tram, ke thua tu lop Entity
class SuCoTram : public Entity {
private:
    std::string matram;
    Date ngayxayra;
    std::string motaloi;
    std::string mucdo;
    std::string maktv;
    std::string trangthai;

public:
	// Ham khoi tao mac dinh
    SuCoTram() {}
    SuCoTram(std::string maSC, std::string maBTS, std::string ngay, std::string moTa,
        std::string mucDoLoi, std::string ktv, std::string tt) {
        setId(Inputhelper::tuTaoMa(maSC, "SC"));
        setMaTram(maBTS);
        setNgayXayRa(ngay);
        setMoTaLoi(moTa);
        setMucDo(mucDoLoi);
        setMaKTV(ktv);
        setTrangThai(tt);
    }
	// Cac ham lay va gan du lieu
    std::string getMaTram() const { return matram; }
    void setMaTram(const std::string& maBTS) {
		matram = Inputhelper::tuTaoMa(maBTS, "BTS"); // Tu gan BTS cho ma tram
    }

    std::string getNgayXayRa() const { return ngayxayra.toString(); }
    void setNgayXayRa(const std::string& ngay) {
        if (ngay == "") throw Loixacthuc("Loi: Ngay khong duoc de trong!");
		ngayxayra.docchuoi(ngay); // Dung ham docchuoi de kiem tra va gan ngay thang nam
    }

    std::string getMoTaLoi() const { return motaloi; }
    void setMoTaLoi(const std::string& moTa) {
        if (moTa == "") throw Loixacthuc("Loi: Mo ta khong duoc de trong!");
        motaloi = moTa;
    }
	// Muc do chi duoc nhap 1, 2 hoac 3
    std::string getMucDo() const { return mucdo; }
    void setMucDo(const std::string& mucDoLoi) {
        if (mucDoLoi != "1" && mucDoLoi != "2" && mucDoLoi != "3") {
            throw Loixacthuc("Loi: Muc do chi duoc nhap 1, 2 hoac 3!");
        }
        mucdo = mucDoLoi;
    }

    std::string getMaKTV() const { return maktv; }
    void setMaKTV(const std::string& ktv) {
		maktv = Inputhelper::tuTaoMa(ktv, "KTV"); // Tu gan KTV cho ma ky thuat vien
    }
	// Trang thai chi duoc nhap "da sua" hoac "chua sua"
    std::string getTrangThai() const { return trangthai; }
    void setTrangThai(const std::string& tt) {
        if (tt != "da sua" && tt != "chua sua" && tt != "Da sua" && tt != "Chua sua") {
            throw Loixacthuc("Loi: Trang thai chi duoc nhap 'da sua' hoac 'chua sua'!");
        }
        trangthai = tt;
    }
	// Ham su ly file
    std::string toFilerecord() const override {
        return id + "|" + matram + "|" + ngayxayra.toString() + "|" +
            motaloi + "|" + mucdo + "|" + maktv + "|" + trangthai;
    }
	// Ham doc du lieu tu file va nap vao doi tuong
    void fromFilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 7) {
            setId(data[0]);
            matram = data[1];
            ngayxayra.docchuoi(data[2]);
            motaloi = data[3];
            mucdo = data[4];
            maktv = data[5];
            trangthai = data[6];
        }
    }
    // Ham hien thi thong tin su co tram
    static void displayHeader() {
        std::cout << std::left
            << std::setw(10) << "Ma SC" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(15) << "Ngay Loi" << "| "
            << std::setw(20) << "Mo Ta" << "| "
            << std::setw(10) << "Muc Do" << "| "
            << std::setw(10) << "Ma KTV" << "| "
            << std::setw(15) << "Trang Thai" << "\n";
        std::cout << std::string(96, '-') << "\n";
    }

    void display() const override {
        std::cout << std::left
            << std::setw(10) << id << "| "
            << std::setw(10) << matram << "| "
            << std::setw(15) << ngayxayra.toString() << "| "
            << std::setw(20) << motaloi << "| "
            << std::setw(10) << mucdo << "| "
            << std::setw(10) << maktv << "| "
            << std::setw(15) << trangthai << "\n";
    }
};
