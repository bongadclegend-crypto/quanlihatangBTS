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
    std::string maktvxuly;
    std::string trangthai;

public:
    // Ham khoi tao mac dinh
    SuCoTram() {}
    SuCoTram(std::string maSC, std::string maBTS, std::string ngay, std::string moTa,
        std::string mucDoLoi, std::string ktv, std::string tt) {
        setId(Inputhelper::TuTaoMa(maSC, "SC"));
        setMaTram(maBTS);
        setNgayXayRa(ngay);
        setMoTaLoi(moTa);
        setMucDo(mucDoLoi);
        setMaKTVXuLy(ktv);
        setTrangThai(tt);
    }
    // Cac ham lay va gan du lieu
    std::string getMaTram() const { return matram; }
    void setMaTram(const std::string& maBTS) {
        matram = Inputhelper::TuTaoMa(maBTS, "BTS"); // Tu gan BTS cho ma tram
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

    std::string getMaKTVXuLy() const { return maktvxuly; }
    void setMaKTVXuLy(const std::string& ktv) {
        maktvxuly = Inputhelper::TuTaoMa(ktv, "KTV"); // Tu gan KTV cho ma ky thuat vien
    }
    // Trang thai chi duoc nhap "da sua" hoac "chua sua"
    std::string getTrangThai() const { return trangthai; }
    void setTrangThai(const std::string& tt) {
        std::string chuanhoa = Inputhelper::VietHoaChuDau(tt);
        if (chuanhoa != "Da sua" && chuanhoa != "Da Sua" &&
            chuanhoa != "Chua sua" && chuanhoa != "Chua Sua"){
            throw Loixacthuc("Loi: Trang thai chi duoc nhap 'Da sua' hoac 'Chua sua'!");
        }
        trangthai = chuanhoa;
    }
    // Ham su ly file
    std::string tofilerecord() const override {
        return id + "|" + matram + "|" + ngayxayra.toString() + "|" +
            motaloi + "|" + mucdo + "|" + maktvxuly + "|" + trangthai;
    }
    // Ham doc du lieu tu file va nap vao doi tuong
    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 7) {
            setId(data[0]);
            matram = Inputhelper::TuTaoMa(data[1], "BTS");
            ngayxayra.docchuoi(data[2]);
            motaloi = data[3];
            mucdo = data[4];
            maktvxuly = data[5];
            trangthai = data[6];
        }
    }
    // Ham hien thi thong tin su co tram
    static void displayHeader() {
        std::cout << std::left
            << std::setw(10) << "Ma SC" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(15) << "Ngay Loi" << "| "
            << std::setw(30) << "Mo Ta" << "| "
            << std::setw(10) << "Muc Do" << "| "
            << std::setw(12) << "Ma KTV Xu Ly" << "| "
            << std::setw(15) << "Trang Thai" << "\n";
        std::cout << std::string(109, '-') << "\n";
    }

    void display() const override {
        std::cout << std::left
            << std::setw(10) << id << "| "
            << std::setw(10) << matram << "| "
            << std::setw(15) << ngayxayra.toString() << "| "
            << std::setw(30) << motaloi << "| "
            << std::setw(10) << mucdo << "| "
            << std::setw(12) << maktvxuly << "| "
            << std::setw(15) << trangthai << "\n";
    }
};
