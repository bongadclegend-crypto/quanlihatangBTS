// Nguoi thuc hien: Nguyen Anh Tu - B24DCVT368
#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "Entity.h"
#include "InputHelper.h"

// Khai bao su dung khong gian ten std de bo cac tien to std::
using namespace std;

class LoaiThietBi : public Entity {
private:
    string tenLoai;
    string hangSanXuat;
    string dacTinhKyThuat;
    double congSuatTieuThu;

public:
    LoaiThietBi() : congSuatTieuThu(0.0) {}

    LoaiThietBi(string ma, string ten, string hang, string dacTinh, double congSuat) {
        setId(Inputhelper::tuTaoMa(ma, "LT"));
        setTenLoai(ten);
        setHangSanXuat(hang);
        setDacTinhKyThuat(dacTinh);
        setCongSuatTieuThu(congSuat);
    }

    string getTenLoai() const { return tenLoai; }
    void setTenLoai(const string& ten) {
        string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: Ten loai thiet bi khong duoc de trong!");
        if (!Inputhelper::coChuaChuCai(clean)) throw Loixacthuc("Loi: Ten loai phai chua it nhat 1 chu cai (khong duoc toan so)!");
        tenLoai = clean;
    }

    string getHangSanXuat() const { return hangSanXuat; }
    void setHangSanXuat(const string& hang) {
        string clean = Inputhelper::catkhoangtrang(hang);
        if (clean.empty()) throw Loixacthuc("Loi: Hang san xuat khong duoc de trong!");
        if (!Inputhelper::coChuaChuCai(clean)) throw Loixacthuc("Loi: Hang san xuat phai chua it nhat 1 chu cai!");
        hangSanXuat = Inputhelper::vietHoaChuDau(clean);
    }

    string getDacTinhKyThuat() const { return dacTinhKyThuat; }
    void setDacTinhKyThuat(const string& dacTinh) {
        string clean = Inputhelper::catkhoangtrang(dacTinh);
        if (clean.empty()) throw Loixacthuc("Loi: Dac tinh ky thuat khong duoc de trong!");
        if (!Inputhelper::coChuaChuCai(clean)) throw Loixacthuc("Loi: Dac tinh ky thuat phai chua it nhat 1 chu cai!");
        dacTinhKyThuat = clean;
    }

    double getCongSuatTieuThu() const { return congSuatTieuThu; }
    void setCongSuatTieuThu(double congSuat) {
        if (congSuat < 0) throw Loixacthuc("Loi: Cong suat tieu thu phai >= 0W!");
        congSuatTieuThu = congSuat;
    }

    string toFilerecord() const override {
        return id + "|" + tenLoai + "|" + hangSanXuat + "|" +
            dacTinhKyThuat + "|" + to_string(congSuatTieuThu);
    }

    void fromFilerecord(const string& record) override {
        vector<string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 5) {
            setId(data[0]);
            tenLoai = data[1];
            hangSanXuat = data[2];
            dacTinhKyThuat = data[3];
            congSuatTieuThu = stod(data[4]);
        }
    }

    void display() const override {
        cout << left
            << setw(8) << id << "| "
            << setw(25) << (tenLoai.length() > 22 ? tenLoai.substr(0, 19) + "..." : tenLoai) << "| "
            << setw(15) << hangSanXuat << "| "
            << setw(20) << dacTinhKyThuat << "| "
            << setw(10) << congSuatTieuThu << "\n";
    }

    static void displayHeader() {
        cout << left
            << setw(8) << "Ma LT" << "| "
            << setw(25) << "Ten Loai Thiet Bi" << "| "
            << setw(15) << "Hang SX" << "| "
            << setw(20) << "Dac Tinh KT" << "| "
            << setw(10) << "CongSuat(W)" << "\n";
        cout << string(85, '-') << "\n";
    }
};
