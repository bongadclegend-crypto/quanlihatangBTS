#pragma once // Lenh tien xu ly, dam bao file chi bien dich 1 lan
#include <iostream>   // Thu vien I/O
#include <iomanip>    // Thu vien dinh dang cot (setw)
#include <vector>     // Thu vien xu ly mang dong
#include <string>     // Thu vien kieu du lieu string
#include "Entity.h"   // Tinh ke thua: lop co so Entity
#include "InputHelper.h" // Tien ich input validation
#include "Date.h"     // Tien ich xu ly ngay thang

// Lop ThietBiLapDat dai dien cho Thiet bi dang lap o mot Tram, ke thua Entity (Relationship is-a)
class ThietBiLapDat : public Entity {
private: // Pham vi bao ve (Encapsulation)
    std::string tenThietBi;   
    std::string maLoai;       
    std::string maTram;       
    Date ngayLapDat;          
    std::string trangThai;    

public:
    // Ham khoi tao mac dinh: tao object rong de do du lieu tu file txt len truoc
    ThietBiLapDat() {}

    // Ham khoi tao 6 tham so de dung cho luong tao moi object (Create) tu Menu
    ThietBiLapDat(std::string ma, std::string ten, std::string loai, std::string tram, std::string ngay, std::string tt) {
        setId(Inputhelper::TuTaoMa(ma, "TB")); // Sinh ma "TB"
        setTenThietBi(ten);                  
        setMaLoai(loai);                       
        setMaTram(tram);                       
        setNgayLapDat(ngay);                 
        setTrangThai(tt);                      
    }

    // Ham getter lay Ten thiet bi
    std::string getTenThietBi() const { return tenThietBi; }

    // Ham setter xac thuc va cap nhat Ten thiet bi
    void setTenThietBi(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten); // Don dep chuoi
        if (clean.empty()) throw Loixacthuc("Loi: Ten thiet bi khong duoc de trong!"); // Chan chuoi rong
        if (!Inputhelper::CoChuaChuCai(clean)) throw Loixacthuc("Loi: Ten thiet bi phai chua it nhat 1 chu cai!"); // Chan chuoi toan so
     
        tenThietBi = clean; // Hoan tat gan bien
    }

    // Ham getter lay Ma Loai
    std::string getMaLoai() const { return maLoai; }
    // Ham setter cap nhat Ma Loai (tuTaoMa da bao gom kiem tra do dai va toan so ben trong roi)
    void setMaLoai(const std::string& loai) { maLoai = Inputhelper::TuTaoMa(loai, "LT"); }

    // Ham getter lay Ma Tram
    std::string getMaTram() const { return maTram; }
    // Ham setter cap nhat Ma Tram, tu dong ghep tien to BTS
    void setMaTram(const std::string& tram) { maTram = Inputhelper::TuTaoMa(tram, "BTS"); }

    // Ham getter lay ngay lap dat. Su dung ham toString() cua class Date de dich object Date thanh chuoi dd/mm/yyyy
    std::string getNgayLapDat() const { return ngayLapDat.toString(); }

    // Ham setter nhan chuoi ky tu, nem vao object Date de no tu validate ngay hop le tren lich
    void setNgayLapDat(const std::string& ngay) {
        std::string clean = Inputhelper::catkhoangtrang(ngay); // Don dep
        if (clean.empty()) throw Loixacthuc("Loi: Ngay lap dat khong duoc de trong!"); // Kiem tra rong
        ngayLapDat.docchuoi(clean); // class Date se tu phan tich '/' va quang loi neu nhap ngay sai (VD: 30/2)
    }

    // Ham getter lay Trang thai
    std::string getTrangThai() const { return trangThai; }

    // Ham setter kiem tra va cap nhat Trang thai
    void setTrangThai(const std::string& tt) {
        std::string clean = Inputhelper::catkhoangtrang(tt); // Don dep
        if (clean.empty()) throw Loixacthuc("Loi: Trang thai khong duoc de trong!"); // Chan rong
        if (!Inputhelper::chiChuaChuCaiVaKhoangTrang(clean)) { // Trang thai chi duoc ghi chu (VD: hoat dong, bao tri) -> Cam nhap so
            throw Loixacthuc("Loi: Trang thai chi duoc chua chu cai va khoang trang!"); // Nem ngoai le
        }
        trangThai = Inputhelper::VietHoaChuDau(clean); // Viet hoa chu dau de chuan hoa database
    }

    // Ghi de (override) ham ao tofilerecord cua Entity
    std::string tofilerecord() const override {
        // Ghep 6 truong lai thanh 1 chuoi kieu CSV (dung ky tu |), gọi luon ngayLapDat.toString() de dich Date ve chuoi
        return id + "|" + tenThietBi + "|" + maLoai + "|" + maTram + "|" + ngayLapDat.toString() + "|" + trangThai;
    }

    // Ghi de ham fromfilerecord de anh xa data tu txt vao RAM
    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|'); // Tach mang
        if (data.size() >= 6) { // Phai du 6 cot theo thiet ke
            setId(data[0]);               // Cot 0: Id
            tenThietBi = data[1];         // Cot 1: Ten Thiet Bi
            maLoai = data[2];             // Cot 2: Ma Khoa Ngoai Loai
            maTram = data[3];             // Cot 3: Ma Khoa Ngoai Tram
            ngayLapDat.docchuoi(data[4]); // Cot 4: Nhet nguoc chuoi thoi gian vao class Date de no parse thanh int d,m,y
            trangThai = data[5];          // Cot 5: Trang thai
        }
    }

    // Ghi de ham hien thi thong tin len man hinh
    void display() const override {
        std::cout << std::left // Canh le van ban sang trai
            << std::setw(8) << id << "| " // In id
            << std::setw(25) << (tenThietBi.length() > 22 ? tenThietBi.substr(0, 19) + "..." : tenThietBi) << "| " // Xu ly chuoi dai
            << std::setw(10) << maLoai << "| " // In ma loai
            << std::setw(10) << maTram << "| " // In ma tram
            << std::setw(12) << ngayLapDat.toString() << "| " // Dich Date thanh chuoi de hien thi
            << std::setw(15) << trangThai << "\n"; // In trang thai va xuong dong
    }

    // Ham static in tieu de cho the hien dang bang tren console
    static void displayHeader() {
        std::cout << std::left // Can le trai
            << std::setw(8) << "Ma TB" << "| " // Tieu de cot 1
            << std::setw(25) << "Ten Thiet Bi" << "| " // Tieu de cot 2
            << std::setw(10) << "Ma Loai" << "| " // Tieu de cot 3
            << std::setw(10) << "Ma Tram" << "| " // Tieu de cot 4
            << std::setw(12) << "Ngay Lap" << "| " // Tieu de cot 5
            << std::setw(15) << "Trang Thai" << "\n"; // Tieu de cot 6
        std::cout << std::string(86, '-') << "\n"; // In duong line phan cach
    }
};
