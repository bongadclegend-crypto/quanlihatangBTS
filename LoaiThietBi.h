// Nguoi thuc hien: Nguyen Anh Tu - B24DCVT368
// Lenh #pragma once giup trinh bien dich chi nap file nay dung 1 lan, tranh loi trung lap (redefinition)
#pragma once

// --- KHU VUC KHAI BAO THU VIEN CHUAN CUA C++ ---
#include <iostream>  // Thu vien ho tro cac lenh nhap xuat du lieu co ban (cin, cout)
#include <iomanip>   // Thu vien ho tro dinh dang luong xuat hien thi (setw, left) de ve bang
#include <vector>    // Thu vien ho tro cau truc du lieu mang dong (vector)
#include <string>    // Thu vien ho tro cac thao tac voi chuoi ky tu (string)

// --- KHU VUC KHAI BAO THU VIEN TU DINH NGHIA ---
#include "Entity.h"      // Nhap file chua lop co so Entity de thuc hien ke thua
#include "InputHelper.h" // Nhap file chua cac ham tien ich chuan hoa chuoi va bat loi

// Khai bao su dung khong gian ten std de bo cac tien to std:: (VD: viet cout thay vi std::cout)
using namespace std;

// Khai bao lop LoaiThietBi ke thua (public) tu lop tru truong Entity
class LoaiThietBi : public Entity {
private:
    // TÍNH ĐÓNG GÓI (ENCAPSULATION) 
    // Khai bao cac thuoc tinh la private de ngan chan viec truy cap va sua doi tuy tien tu ben ngoai
    string tenLoai;         // Luu ten loai thiet bi (VD: Anten, RRU, BBU)
    string hangSanXuat;     // Luu ten hang san xuat (VD: Ericsson, Nokia)
    string dacTinhKyThuat;  // Luu thong so, dac tinh ky thuat cua thiet bi
    double congSuatTieuThu; // Luu muc tieu hao nang luong cua thiet bi (Don vi: Watt)

public:
    //  KHU VUC CONSTRUCTOR (HAM KHOI TAO)
    // Ham khoi tao mac dinh (Khong doi so), tu dong gan cong suat ban dau bang 0.0
    LoaiThietBi() : congSuatTieuThu(0.0) {}

    // Ham khoi tao co tham so dung de nap nhanh du lieu vao doi tuong khi doc file hoac them moi
    LoaiThietBi(string ma, string ten, string hang, string dacTinh, double congSuat) {
        // Goi ham tuTaoMa tu Inputhelper de tu dong ghep them tien to "LT" vao truoc ma nhap vao
        setId(Inputhelper::tuTaoMa(ma, "LT"));

        // Dua du lieu truyen vao di qua cac ham Setter de kiem tra tinh hop le truoc khi luu
        setTenLoai(ten);
        setHangSanXuat(hang);
        setDacTinhKyThuat(dacTinh);
        setCongSuatTieuThu(congSuat);
    }

    // --- KHU VUC GETTER VÀ SETTER (MÀNG LỌC DỮ LIỆU) ---

    // Ham Getter tra ve ten loai thiet bi. Tu khoa const bao dam ham nay khong lam thay doi du lieu
    string getTenLoai() const { return tenLoai; }

    // Ham Setter cap nhat ten loai. Truyen tham chieu hang (const string&) de tiet kiem bo nho RAM
    void setTenLoai(const string& ten) {
        // Goi ham catkhoangtrang de xoa bo cac dau cach thua o dau va cuoi chuoi
        string clean = Inputhelper::catkhoangtrang(ten);

        // Kiem tra rong: Neu chuoi rong thi chu dong nem (throw) ra ngoai le Loixacthuc
        if (clean.empty()) throw Loixacthuc("Loi: Ten loai thiet bi khong duoc de trong!");

        // Kiem tra logic: Ten thiet bi bat buoc phai co chua chu cai, khong duoc phep chi toan cac con so
        if (!Inputhelper::coChuaChuCai(clean)) throw Loixacthuc("Loi: Ten loai phai chua it nhat 1 chu cai (khong duoc toan so)!");

        // Neu qua duoc moi bai kiem tra tren thi moi chinh thuc gan vao thuoc tinh thuc te
        tenLoai = clean;
    }

    // Ham Getter lay hang san xuat
    string getHangSanXuat() const { return hangSanXuat; }

    // Ham Setter cap nhat hang san xuat
    void setHangSanXuat(const string& hang) {
        // Cat khoang trang thua
        string clean = Inputhelper::catkhoangtrang(hang);
        // Bat loi de trong
        if (clean.empty()) throw Loixacthuc("Loi: Hang san xuat khong duoc de trong!");
        // Bat loi ten hang chi toan so
        if (!Inputhelper::coChuaChuCai(clean)) throw Loixacthuc("Loi: Hang san xuat phai chua it nhat 1 chu cai!");

        // Chuan hoa du lieu: Tu dong viet hoa chu cai dau tien cua ten hang (VD: ericsson -> Ericsson)
        hangSanXuat = Inputhelper::vietHoaChuDau(clean);
    }

    // Ham Getter lay dac tinh ky thuat
    string getDacTinhKyThuat() const { return dacTinhKyThuat; }

    // Ham Setter cap nhat dac tinh ky thuat
    void setDacTinhKyThuat(const string& dacTinh) {
        // Cat khoang trang thua
        string clean = Inputhelper::catkhoangtrang(dacTinh);
        // Bat loi chuoi rong
        if (clean.empty()) throw Loixacthuc("Loi: Dac tinh ky thuat khong duoc de trong!");
        // Bat loi chuoi chi chua so
        if (!Inputhelper::coChuaChuCai(clean)) throw Loixacthuc("Loi: Dac tinh ky thuat phai chua it nhat 1 chu cai!");
        // Gan gia tri hop le
        dacTinhKyThuat = clean;
    }

    // Ham Getter lay cong suat tieu thu
    double getCongSuatTieuThu() const { return congSuatTieuThu; }

    // Ham Setter cap nhat cong suat tieu thu
    void setCongSuatTieuThu(double congSuat) {
        // Kiem tra logic nghiep vu: Cong suat tieu hao dien nang khong duoc phep mang gia tri am
        if (congSuat < 0) throw Loixacthuc("Loi: Cong suat tieu thu phai >= 0W!");
        // Gan gia tri hop le
        congSuatTieuThu = congSuat;
    }

    
    // Ghi de (override) ham toFilerecord cua lop cha Entity de dong goi object thanh chuoi van ban
    string toFilerecord() const override {
        // Noi cac thuoc tinh thanh 1 chuoi, ngan cach nhau bang ky tu '|'. Su dung to_string de ep kieu double sang string.
        return id + "|" + tenLoai + "|" + hangSanXuat + "|" +
            dacTinhKyThuat + "|" + to_string(congSuatTieuThu);
    }

   
    // Ghi de ham fromFilerecord de boc tach chuoi doc tu file txt va khoi tao lai thanh doi tuong tren RAM
    void fromFilerecord(const string& record) override {
        // Dung ham chia() de cat chuoi van ban dua vao ky tu '|' va dua vao mang dong (vector)
        vector<string> data = Inputhelper::chia(record, '|');

        // Chi thuc hien viec gan du lieu khi mang chua du 5 phan tu (Dam bao cau truc file khong bi hong)
        if (data.size() >= 5) {
            setId(data[0]);              // Phan tu so 0 la ID
            tenLoai = data[1];           // Phan tu so 1 la Ten Loai
            hangSanXuat = data[2];       // Phan tu so 2 la Hang san xuat
            dacTinhKyThuat = data[3];    // Phan tu so 3 la Dac tinh KT
            congSuatTieuThu = stod(data[4]); // Phan tu so 4 (Chuoi) phai dung stod de ep kieu nguoc lai thanh double
        }
    }

    
    // Ghi de ham display cua lop cha de in thong tin tung thiet bi ra man hinh console dang bang
    void display() const override {
        // Dung thu vien iomanip: left de can le trai, setw(x) de co dinh do rong cua cot
        cout << left
            << setw(8) << id << "| "
            // Toan tu 3 ngoi: Neu ten loai qua dai (>22), tu dong cat ngan den vi tri 19 va them "..." de khong lam vo bang
            << setw(25) << (tenLoai.length() > 22 ? tenLoai.substr(0, 19) + "..." : tenLoai) << "| "
            << setw(15) << hangSanXuat << "| "
            << setw(20) << dacTinhKyThuat << "| "
            << setw(10) << congSuatTieuThu << "\n";
    }

    // Ham displayHeader hien thi tieu de cua cac cot. Day la ham static nen co the goi truc tiep tu ten lop.
    static void displayHeader() {
        // Can le trai va thiet lap chieu rong cac cot giong y het ham display() o tren de thang hang
        cout << left
            << setw(8) << "Ma LT" << "| "
            << setw(25) << "Ten Loai Thiet Bi" << "| "
            << setw(15) << "Hang SX" << "| "
            << setw(20) << "Dac Tinh KT" << "| "
            << setw(10) << "CongSuat(W)" << "\n";
        // In ra 85 ky tu gach ngang '-' de lam duong vien phan cach tieu de va du lieu
        cout << string(85, '-') << "\n";
    }
};
