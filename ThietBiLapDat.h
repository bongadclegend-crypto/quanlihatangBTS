// Nguoi thuc hien: Nguyen Anh Tu - B24DCVT368
#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "Entity.h"
#include "InputHelper.h"
#include "Date.h"

// Khai bao su dung khong gian ten std de bo cac tien to std::
using namespace std;

class ThietBiLapDat : public Entity {
private:
    string tenThietBi;
    string maLoai;
    string maTram;
    Date ngayLapDat;
    string trangThai;

public:
    ThietBiLapDat() {}

    ThietBiLapDat(string ma, string ten, string loai, string tram, string ngay, string tt) {
        setId(Inputhelper::tuTaoMa(ma, "TB"));
        setTenThietBi(ten);
        setMaLoai(loai);
        setMaTram(tram);
        setNgayLapDat(ngay);
        setTrangThai(tt);
    }

    string getTenThietBi() const { return tenThietBi; }
    void setTenThietBi(const string& ten) {
        string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: Ten thiet bi khong duoc de trong!");
        if (!Inputhelper::coChuaChuCai(clean)) throw Loixacthuc("Loi: Ten thiet bi phai chua it nhat 1 chu cai!");
        tenThietBi = clean;
    }

    string getMaLoai() const { return maLoai; }
    void setMaLoai(const string& loai) { maLoai = Inputhelper::tuTaoMa(loai, "LT"); }

    string getMaTram() const { return maTram; }
    void setMaTram(const string& tram) { maTram = Inputhelper::tuTaoMa(tram, "BTS"); }

    string getNgayLapDat() const { return ngayLapDat.toString(); }
    void setNgayLapDat(const string& ngay) {
        string clean = Inputhelper::catkhoangtrang(ngay);
        if (clean.empty()) throw Loixacthuc("Loi: Ngay lap dat khong duoc de trong!");
        ngayLapDat.docchuoi(clean);
    }

    string getTrangThai() const { return trangThai; }
    void setTrangThai(const string& tt) {
        string clean = Inputhelper::catkhoangtrang(tt);
        if (clean.empty()) throw Loixacthuc("Loi: Trang thai khong duoc de trong!");
        if (!Inputhelper::coChuaChuCai(clean)) throw Loixacthuc("Loi: Trang thai phai chua it nhat 1 chu cai!");
        trangThai = clean;
    }

    string toFilerecord() const override {
        return id + "|" + tenThietBi + "|" + maLoai + "|" + maTram + "|" + ngayLapDat.toString() + "|" + trangThai;
    }

    void fromFilerecord(const string& record) override {
        vector<string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 6) {
            setId(data[0]);
            tenThietBi = data[1];
            maLoai = data[2];
            maTram = data[3];
            ngayLapDat.docchuoi(data[4]);
            trangThai = data[5];
        }
    }

    void display() const override {
        cout << left
            << setw(8) << id << "| "
            << setw(25) << (tenThietBi.length() > 22 ? tenThietBi.substr(0, 19) + "..." : tenThietBi) << "| "
            << setw(10) << maLoai << "| "
            << setw(10) << maTram << "| "
            << setw(12) << ngayLapDat.toString() << "| "
            << setw(15) << trangThai << "\n";
    }

    static void displayHeader() {
        cout << left
            << setw(8) << "Ma TB" << "| "
            << setw(25) << "Ten Thiet Bi" << "| "
            << setw(10) << "Ma Loai" << "| "
            << setw(10) << "Ma Tram" << "| "
            << setw(12) << "Ngay Lap" << "| "
            << setw(15) << "Trang Thai" << "\n";
        cout << string(86, '-') << "\n";
    }
};