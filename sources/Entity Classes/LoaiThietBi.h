#pragma once
// Nguoi thuc hien: Nguyen Anh Tu - B24DCVT368
#pragma once
#include <iostream>
#include <iomanip>  // Thu vien iomanip de dung lenh setw() (can chinh le cot khi in bang)
#include <vector>
#include <string>
#include "Entity.h" // Bat buoc include de ke thua (Inheritance)
#include "InputHelper.h"

// Lop LoaiThietBi ke thua tu lop co so Entity
class LoaiThietBi : public Entity {
private:
    // Tinh dong goi (Encapsulation): Khai bao private de bao ve du lieu
    std::string tenLoai;
    std::string hangSanXuat;
    std::string dacTinhKyThuat;
    double congSuatTieuThu;

public:
    // 1. Constructor mac dinh (Ham khoi tao ko tham so) - Dung khi tao mang trong Repository
    LoaiThietBi() : congSuatTieuThu(0.0) {}

    // 2. Constructor co tham so - Dung khi them moi tu giao dien Menu
    LoaiThietBi(std::string ma, std::string ten, std::string hang, std::string dacTinh, double congSuat) {
        // Tu dong gan tien to LT cho ma nhap vao (VD: 0001 -> LT0001)
        setId(Inputhelper::TuTaoMa(ma, "LT"));
        // Goi cac ham Setter de kiem tra tinh hop le cua du lieu truoc khi luu
        setTenLoai(ten);
        setHangSanXuat(hang);
        setDacTinhKyThuat(dacTinh);
        setCongSuatTieuThu(congSuat);
    }

    // --- CÁC HÀM GETTER & SETTER ---
    std::string getTenLoai() const { return tenLoai; }
    void setTenLoai(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten); // Xoa khoang trang thua 2 dau
        if (clean.empty()) throw Loixacthuc("Loi: Ten loai thiet bi khong duoc de trong!"); // Nem ngoai le neu bo trong
        tenLoai = clean;
    }

    std::string getHangSanXuat() const { return hangSanXuat; }
    void setHangSanXuat(const std::string& hang) {
        std::string clean = Inputhelper::catkhoangtrang(hang);
        if (clean.empty()) throw Loixacthuc("Loi: Hang san xuat khong duoc de trong!");
        hangSanXuat = Inputhelper::VietHoaChuDau(clean); // Tu dong viet hoa chu cai dau (VD: ericsson -> Ericsson)
    }

    std::string getDacTinhKyThuat() const { return dacTinhKyThuat; }
    void setDacTinhKyThuat(const std::string& dacTinh) {
        std::string clean = Inputhelper::catkhoangtrang(dacTinh);
        if (clean.empty()) throw Loixacthuc("Loi: Dac tinh ky thuat khong duoc de trong!");
        dacTinhKyThuat = clean;
    }

    double getCongSuatTieuThu() const { return congSuatTieuThu; }
    void setCongSuatTieuThu(double congSuat) {
        if (congSuat < 0) throw Loixacthuc("Loi: Cong suat tieu thu phai >= 0W!"); // Chan so am
        congSuatTieuThu = congSuat;
    }

    // --- ĐA HÌNH (POLYMORPHISM): Ghi đè (Override) các hàm ảo từ lớp Entity ---

    // 1. Ham tofilerecord: Nen doi tuong thanh 1 chuoi de ghi xuong file .txt
    std::string tofilerecord() const override {
        // Ghep cac bien lai, cach nhau boi dau '|'
        return id + "|" + tenLoai + "|" + hangSanXuat + "|" +
            dacTinhKyThuat + "|" + std::to_string(congSuatTieuThu); // phai dung to_string de ep kieu double sang chuoi
    }

    // 2. Ham fromfilerecord: Dich tu 1 chuoi trong file .txt nap vao doi tuong
    void fromfilerecord(const std::string& record) override {
        // Goi ham chia trong InputHelper de bam chuoi thanh cac phan tu dua vao ky tu '|'
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() >= 5) { // Kiem tra du 5 cot thi moi doc
            setId(data[0]); // Cot 0 la ma ID
            tenLoai = data[1];
            hangSanXuat = data[2];
            dacTinhKyThuat = data[3];
            congSuatTieuThu = std::stod(data[4]); // stod = String to Double (ep kieu chuoi ve so thuc)
        }
    }

    // 3. Ham display: In doi tuong ra man hinh Console (1 dong cua bang)
    void display() const override {
        std::cout << std::left // Can le trai
            << std::setw(8) << id << "| " // Cot ID rong 8 ky tu
            // Toan tu 3 ngoi: Neu ten > 22 ky tu thi cat xuong 19 ky tu roi noi them "..."
            << std::setw(25) << (tenLoai.length() > 22 ? tenLoai.substr(0, 19) + "..." : tenLoai) << "| "
            << std::setw(15) << hangSanXuat << "| "
            << std::setw(20) << dacTinhKyThuat << "| "
            << std::setw(10) << congSuatTieuThu << "\n";
    }

    // Ham tinh (static) de in tieu de cua cai bang (Khong can khoi tao doi tuong van goi duoc)
    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma LT" << "| "
            << std::setw(25) << "Ten Loai Thiet Bi" << "| "
            << std::setw(15) << "Hang SX" << "| "
            << std::setw(20) << "Dac Tinh KT" << "| "
            << std::setw(10) << "CongSuat(W)" << "\n";
        std::cout << std::string(85, '-') << "\n"; // In ra 85 dau gach ngang
    }
};
