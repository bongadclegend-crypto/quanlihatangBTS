#pragma once 
#include <iostream>     // Thu vien ho tro cac luong nhap xuat chuan (cin, cout)
#include <iomanip>      // Thu vien ho tro dinh dang luong (setw, left) de ve bang
#include <vector>       // Thu vien cung cap mang dong vector de luu danh sach cac chuoi sau khi cat
#include <string>       // Thu vien ho tro xu ly kieu du lieu chuoi (string)
#include "Entity.h"     // Include file lop cha Entity de thuc hien Tinh ke thua
#include "InputHelper.h" // Include file tien ich chuan hoa dau vao

// Lop LoaiThietBi dai dien cho the loai thiet bi, su dung Tinh ke thua (public Entity)
class LoaiThietBi : public Entity {
private: // Tinh dong goi (Encapsulation): An cac thuoc tinh nay de bao ve du lieu ben trong
    std::string tenLoai;          
    std::string hangSanXuat;      
    std::string dacTinhKyThuat;  
    double congSuatTieuThu;       

public: // Cac phuong thuc giao tiep (Interface) cho phep ben ngoai truy xuat va thao tac
    // Constructor mac dinh: Duoc goi tu dong khi tao mang hoac doc file ma chua co du lieu ngay
    LoaiThietBi() : congSuatTieuThu(0.0) {} // Khoi tao an toan cho thuoc tinh kieu so

    // Constructor co tham so: Dung de tao va nap ngay toan bo du lieu cho 1 the hien (object)
    LoaiThietBi(std::string ma, std::string ten, std::string hang, std::string dacTinh, double congSuat) {
        setId(Inputhelper::TuTaoMa(ma, "LT")); // Phat sinh ma tien to "LT" va luu vao thuoc tinh id cua lop cha Entity
        setTenLoai(ten);                       // Goi setter de xac thuc va gan ten loai
        setHangSanXuat(hang);                  // Goi setter de xac thuc va gan hang san xuat
        setDacTinhKyThuat(dacTinh);            // Goi setter de xac thuc va gan dac tinh KT
        setCongSuatTieuThu(congSuat);          // Goi setter de xac thuc va gan cong suat
    }

    // Ham getter tra ve tenLoai. Tu khoa 'const' dam bao ham khong lam thay doi trang thai cua doi tuong
    std::string getTenLoai() const { return tenLoai; }

    // Ham setter kiem tra va cap nhat tenLoai. Truyen bang tham chieu hang (const std::string&) de toi uu bo nho
    void setTenLoai(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten); // Xoa cac dau cach thua o 2 dau chuoi
        if (clean.empty()) throw Loixacthuc("Loi: Ten loai thiet bi khong duoc de trong!"); // Kiem tra rong va nem ngoai le
        if (!Inputhelper::CoChuaChuCai(clean)) throw Loixacthuc("Loi: Ten loai phai chua it nhat 1 chu cai!"); // Bat buoc phai co chu cai
        
        tenLoai = clean; // Chi gan vao bien khi da vuot qua moi bai kiem tra
    }

    // Ham getter tra ve hang san xuat
    std::string getHangSanXuat() const { return hangSanXuat; }

    // Ham setter kiem tra va cap nhat hang san xuat
    void setHangSanXuat(const std::string& hang) {
        std::string clean = Inputhelper::catkhoangtrang(hang); // Loai bo khoang trang thua
        if (clean.empty()) throw Loixacthuc("Loi: Hang san xuat khong duoc de trong!"); // Nem loi neu chuoi bi rong
        if (!Inputhelper::chiChuaChuCaiVaKhoangTrang(clean)) { // Chan viec nhap so hoac ky tu dac biet
            throw Loixacthuc("Loi: Hang san xuat chi duoc chua chu cai va khoang trang!"); // Nem ngoai le
        }
        hangSanXuat = Inputhelper::VietHoaChuDau(clean); // Viet hoa chu cai dau tien tung tu roi luu lai
    }

    // Ham getter tra ve dac tinh ky thuat
    std::string getDacTinhKyThuat() const { return dacTinhKyThuat; }

    // Ham setter kiem tra va cap nhat dac tinh ky thuat
    void setDacTinhKyThuat(const std::string& dacTinh) {
        std::string clean = Inputhelper::catkhoangtrang(dacTinh); // Don dep chuoi nhap vao
        if (clean.empty()) throw Loixacthuc("Loi: Dac tinh ky thuat khong duoc de trong!"); // Kiem tra rong
        if (!Inputhelper::CoChuaChuCai(clean)) { // Khong cho phep nhap toan so, phai co it nhat 1 chu cai
            throw Loixacthuc("Loi: Dac tinh ky thuat phai chua it nhat 1 chu cai!"); // Nem ngoai le
        }
       
        dacTinhKyThuat = clean; // Gan gia tri neu hop le
    }

    // Ham getter tra ve cong suat tieu thu (kieu double)
    double getCongSuatTieuThu() const { return congSuatTieuThu; }

    // Ham setter kiem tra va cap nhat cong suat
    void setCongSuatTieuThu(double congSuat) {
        if (congSuat < 0) throw Loixacthuc("Loi: Cong suat tieu thu phai >= 0W!"); // Nem loi neu gia tri am
        congSuatTieuThu = congSuat; // Gan gia tri vao thuoc tinh
    }

    // Tinh da hinh (Polymorphism): Ghi de (override) ham ao tofilerecord cua lop cha Entity
    std::string tofilerecord() const override {
        // Ghep cac thuoc tinh thanh 1 chuoi, cach nhau boi '|'. Ep kieu double thanh string bang to_string
        return id + "|" + tenLoai + "|" + hangSanXuat + "|" + dacTinhKyThuat + "|" + std::to_string(congSuatTieuThu);
    }

    // Tinh da hinh: Ghi de ham ao tu lop cha, lam nhiem vu giai ma chuoi doc tu file thanh doi tuong
    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|'); // Cat chuoi thanh cac phan tu dua vao dau '|'
        if (data.size() >= 5) { // Kiem tra xem co du 5 cot du lieu khong de tranh loi tran mang
            setId(data[0]);                     // Gan cot 0 cho ID
            tenLoai = data[1];                  // Gan cot 1 cho Ten loai
            hangSanXuat = data[2];              // Gan cot 2 cho Hang san xuat
            dacTinhKyThuat = data[3];           // Gan cot 3 cho Dac tinh KT
            congSuatTieuThu = std::stod(data[4]); // stod (String to Double): ep kieu chuoi ve so thuc de luu vao thuoc tinh
        }
    }

    // Tinh da hinh: Ghi de ham in thong tin the hien ra man hinh console dang cot bang
    void display() const override {
        std::cout << std::left // Chinh huong can le ve ben trai
            << std::setw(8) << id << "| " // In id voi do rong cot la 8
            // Toan tu 3 ngoi: Kiem tra neu ten dai qua 22 ky tu thi cat lay 19 ky tu roi noi them "..."
            << std::setw(25) << (tenLoai.length() > 22 ? tenLoai.substr(0, 19) + "..." : tenLoai) << "| "
            << std::setw(15) << hangSanXuat << "| "       // In hang san xuat voi do rong 15
            << std::setw(20) << dacTinhKyThuat << "| "    // In dac tinh voi do rong 20
            << std::setw(10) << congSuatTieuThu << "\n";  // In cong suat va xuong dong
    }

    // Ham tinh (static): Ham cua ca lop, khong can tao doi tuong van the goi de in dong tieu de cua bang
    static void displayHeader() {
        std::cout << std::left // Can le trai
            << std::setw(8) << "Ma LT" << "| " // Tieu de cot 1
            << std::setw(25) << "Ten Loai Thiet Bi" << "| " // Tieu de cot 2
            << std::setw(15) << "Hang SX" << "| " // Tieu de cot 3
            << std::setw(20) << "Dac Tinh KT" << "| " // Tieu de cot 4
            << std::setw(10) << "CongSuat(W)" << "\n"; // Tieu de cot 5
        std::cout << std::string(85, '-') << "\n"; // In ra 85 dau gach ngang de ngan cach tieu de va du lieu
    }
};
