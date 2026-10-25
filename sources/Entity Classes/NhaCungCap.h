//Hoang Tuan Thanh
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "Entity.h" 
#include "Inputhelper.h"
#include "Exceptions.h"

using namespace std;

class NhaCungCap : public Entity {
private:
    string tenNCC;
    string soDienThoai;
    string diaChi;
public:
    NhaCungCap() {}

    NhaCungCap(string maNCC, string ten, string sdt, string dc) {
        setId(Inputhelper::TuTaoMa(maNCC, "NCC"));
        setTenNCC(ten);
        setSoDienThoai(sdt);
        setDiaChi(dc);
    }

    std::string getMaNCC() const { return id; }

    void setMaNCC(const std::string& maNCC) {
        id = Inputhelper::TuTaoMa(maNCC, "NCC");
    }

    string getTenNCC() const { return tenNCC; }

    void setTenNCC(const string& ten) {
        string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: Ten nha cung cap khong duoc de trong!");
        tenNCC = Inputhelper::VietHoaChuDau(clean);
    }

    string getSoDienThoai() const { return soDienThoai; }

    void setSoDienThoai(const string& sdt) {
        Inputhelper::KiemTraSdt(sdt);
        soDienThoai = sdt;
    }

    string getDiaChi() const { return diaChi; }

    void setDiaChi(const string& dc) {
        string clean = Inputhelper::catkhoangtrang(dc);
        if (clean.empty()) throw Loixacthuc("Loi: Dia chi khong duoc de trong!");
        diaChi = clean;
    }

    string tofilerecord() const override {
        return id + "|" + tenNCC + "|" + soDienThoai + "|" + diaChi;
    }

    void fromfilerecord(const string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');

        if (data.size() != 4) {
            throw Loixacthuc("Loi: Ban ghi Nha Cung Cap phai co dung 4 truong");
        }

        NhaCungCap nccTam;
        nccTam.setId(data[0]);
        nccTam.setTenNCC(data[1]);
        nccTam.setSoDienThoai(data[2]);
        nccTam.setDiaChi(data[3]);

        *this = nccTam;
    }

    void display() const override {
        cout << left << setw(10) << id
            << "| " << setw(30) << (tenNCC.length() > 28 ? tenNCC.substr(0, 25) + "..." : tenNCC)
            << "| " << setw(15) << soDienThoai
            << "| " << setw(35) << (diaChi.length() > 33 ? diaChi.substr(0, 30) + "..." : diaChi) << endl;
    }

    static void displayHeader() {
        cout << left
            << setw(10) << "Ma NCC" << "| "
            << setw(30) << "Ten Nha Cung Cap" << "| "
            << setw(15) << "SDT" << "| "
            << setw(35) << "Dia Chi" << endl;
        cout << string(95, '-') << "\n";
    }
};
