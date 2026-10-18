#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "Entity.h"
#include "InputHelper.h"
#include "Date.h"

class SuCoTram : public Entity {
private:
    std::string maTram;
    Date ngayXayRa;
    std::string moTaLoi;
    std::string mucDo;
    std::string maKTV;
    std::string trangThai;

public:
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

    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& maBTS) {
        maTram = Inputhelper::tuTaoMa(maBTS, "BTS");
    }

    std::string getNgayXayRa() const { return ngayXayRa.toString(); }
    void setNgayXayRa(const std::string& ngay) {
        if (ngay == "") throw Loixacthuc("Loi: Ngay khong duoc de trong!");
        ngayXayRa.docchuoi(ngay);
    }

    std::string getMoTaLoi() const { return moTaLoi; }
    void setMoTaLoi(const std::string& moTa) {
        if (moTa == "") throw Loixacthuc("Loi: Mo ta khong duoc de trong!");
        moTaLoi = moTa;
    }

    std::string getMucDo() const { return mucDo; }
    void setMucDo(const std::string& mucDoLoi) {
        if (mucDoLoi != "1" && mucDoLoi != "2" && mucDoLoi != "3") {
            throw Loixacthuc("Loi: Muc do chi duoc nhap 1, 2 hoac 3!");
        }
        mucDo = mucDoLoi;
    }

    std::string getMaKTV() const { return maKTV; }
    void setMaKTV(const std::string& ktv) {
        maKTV = Inputhelper::tuTaoMa(ktv, "KTV");
    }

    std::string getTrangThai() const { return trangThai; }
    void setTrangThai(const std::string& tt) {
        if (tt != "da sua" && tt != "chua sua" && tt != "Da sua" && tt != "Chua sua") {
            throw Loixacthuc("Loi: Trang thai chi duoc nhap 'da sua' hoac 'chua sua'!");
        }
        trangThai = tt;
    }

    std::string toFilerecord() const override {
        return id + "|" + maTram + "|" + ngayXayRa.toString() + "|" +
            moTaLoi + "|" + mucDo + "|" + maKTV + "|" + trangThai;
    }

    void fromFilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 7) {
            setId(data[0]);
            maTram = data[1];
            ngayXayRa.docchuoi(data[2]);
            moTaLoi = data[3];
            mucDo = data[4];
            maKTV = data[5];
            trangThai = data[6];
        }
    }

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
            << std::setw(10) << maTram << "| "
            << std::setw(15) << ngayXayRa.toString() << "| "
            << std::setw(20) << moTaLoi << "| "
            << std::setw(10) << mucDo << "| "
            << std::setw(10) << maKTV << "| "
            << std::setw(15) << trangThai << "\n";
    }
};