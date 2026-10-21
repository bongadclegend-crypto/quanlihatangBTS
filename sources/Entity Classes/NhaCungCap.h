// Thanh vien thuc hien: Vo Manh Thang
// Sua ngay 21/10
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "Entity.h" 
#include "InputHelper.h"
#include "Exceptions.h"

using namespace std;

// Lop NhaCungCap ke thua tu Entity de dung chung thuoc tinh 'id' va cac ham ao
class NhaCungCap : public Entity {
private:
    string tenNCC;
    string soDienThoai;
    string diaChi;

public:
    NhaCungCap() {}

    NhaCungCap(string maNCC, string ten, string sdt, string dc) {
        setId(Inputhelper::tuTaoMa(maNCC, "NCC"));
        setTenNCC(ten);
        setSoDienThoai(sdt);
        setDiaChi(dc);
    }

     // --- GETTER & SETTER ---

    /* Cac phuong thuc Setter duoc tich hop san logic kiem tra tinh toan ven du lieu.
       Neu du lieu dau vao khong hop le, he thong se nem ra ngoai le Loixacthuc. */

    std::string getMaNCC() const { return id; }
    void setMaNCC(const std::string& maNCC) {
        id = Inputhelper::tuTaoMa(maNCC, "NCC");
    }

    string getTenNCC() const { return tenNCC; }
    void setTenNCC(const string& ten) {
        string clean = Inputhelper::catkhoangtrang(ten);
        if (clean.empty()) throw Loixacthuc("Loi: Ten nha cung cap khong duoc de trong!");
        tenNCC = clean;
    }

    string getSoDienThoai() const { return soDienThoai; }
    void setSoDienThoai(const string& sdt) {
        string clean = Inputhelper::catkhoangtrang(sdt);
        if (clean.length() < 10) throw Loixacthuc("Loi: So dien thoai phai tu 10 so tro len!");
        soDienThoai = clean;
    }

    string getDiaChi() const { return diaChi; }
    void setDiaChi(const string& dc) {
        string clean = Inputhelper::catkhoangtrang(dc);
        if (clean.empty()) throw Loixacthuc("Loi: Dia chi khong duoc de trong!");
        diaChi = clean;
    }

    // --- GHI DE (OVERRIDE) --- 

    /* Ghi de phuong thuc tu lop cha Entity de tuong tac voi File */

    // Tra ve chuoi dinh dang chuan de ghi dong moi vao file text
    string toFilerecord() const override {
        return id + "|" + tenNCC + "|" + soDienThoai + "|" + diaChi;
    }

    // Phan tich chuoi doc tu file va anh xa vao cac thuoc tinh cua doi tuong
    void fromFilerecord(const string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 4) {
            setId(data[0]);
            tenNCC = data[1];
            soDienThoai = data[2];
            diaChi = data[3];
        }
    }

    // --- HIEN THI TREN CONSOLE --- 
   
    // Dinh dang cot hien thi tren Console
    void display() const override {
        cout << left << setw(10) << id
            << "| " << setw(22) << tenNCC
            << "| " << setw(15) << soDienThoai
            << "| " << setw(25) << diaChi << endl;
    }

    static void displayHeader() {
        cout << left
            << setw(10) << "Ma NCC" << "| "
            << setw(22) << "Ten Nha Cung Cap" << "| "
            << setw(15) << "SDT" << "| "
            << setw(25) << "Dia Chi" << endl;
        cout << string(80, '-') << "\n";
    }
};
