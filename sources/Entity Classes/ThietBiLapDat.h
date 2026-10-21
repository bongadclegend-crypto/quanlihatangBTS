#pragma once 
#include <iostream>   
#include <iomanip>    
#include <vector>     
#include <string>    
#include "Entity.h"   
#include "InputHelper.h" 
#include "Date.h"     

// Lop ThietBiLapDat dai dien cho Thiet bi dang lap o mot Tram, ke thua Entity 
class ThietBiLapDat : public Entity {
private: // Tinh dong goi (Encapsulation): An cac thuoc tinh
    std::string tenThietBi;  
    std::string maLoai;       
    std::string maTram;      
    Date ngayLapDat;          
    std::string trangThai;   

public:
    // Ham khoi tao mac dinh: tao object rong de do du lieu tu file txt len
    ThietBiLapDat() {}

    // Ham khoi tao 6 tham so de dung cho luong tao moi object (Create)
    ThietBiLapDat(std::string ma, std::string ten, std::string loai, std::string tram, std::string ngay, std::string tt) {
        setId(Inputhelper::TuTaoMa(ma, "TB")); 
        setTenThietBi(ten);                  
        setMaLoai(loai);                      
        setNgayLapDat(ngay);                  
        setTrangThai(tt);                    
    }

    // --- GETTER & SETTER ---
    std::string getTenThietBi() const { return tenThietBi; } // Ham lay ten thiet bi
    void setTenThietBi(const std::string& ten) { // Ham cap nhat ten thiet bi
        std::string clean = Inputhelper::catkhoangtrang(ten); // Don dep chuoi
        if (clean.empty()) throw Loixacthuc("Loi: Ten thiet bi khong duoc de trong!"); // Kiem tra rong
        if (!Inputhelper::CoChuaChuCai(clean)) throw Loixacthuc("Loi: Ten thiet bi phai chua it nhat 1 chu cai!"); // Bat buoc co chu cai
        if (clean.find('|') != std::string::npos) throw Loixacthuc("Loi: Khong duoc chua ky tu |!"); // Chan pha hoai file
        tenThietBi = clean; // Hoan tat gan
    }

    std::string getMaLoai() const { return maLoai; }
    void setMaLoai(const std::string& loai) { maLoai = Inputhelper::TuTaoMa(loai, "LT"); } // Chuan hoa ma loai

    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& tram) { maTram = Inputhelper::TuTaoMa(tram, "BTS"); } // Chuan hoa ma tram

    std::string getNgayLapDat() const { return ngayLapDat.toString(); } // Dich object Date thanh chuoi
    void setNgayLapDat(const std::string& ngay) {
        std::string clean = Inputhelper::catkhoangtrang(ngay);
        if (clean.empty()) throw Loixacthuc("Loi: Ngay lap dat khong duoc de trong!");
        ngayLapDat.docchuoi(clean); // class Date tu parse '/' va kiem tra ngay tren lich
    }

    std::string getTrangThai() const { return trangThai; }
    void setTrangThai(const std::string& tt) {
        std::string clean = Inputhelper::catkhoangtrang(tt);
        if (clean.empty()) throw Loixacthuc("Loi: Trang thai khong duoc de trong!");
        if (!Inputhelper::chiChuaChuCaiVaKhoangTrang(clean)) { // Trang thai chi duoc la chu cai
            throw Loixacthuc("Loi: Trang thai chi duoc chua chu cai va khoang trang!");
        }
        trangThai = Inputhelper::VietHoaChuDau(clean); // Viet hoa chu cai dau tung tu
    }

    // --- GHI ĐÈ HAM CUA LOP CHA (OVERRIDE) ---

    // Ghep thanh 1 dong de ghi vao file txt
    std::string tofilerecord() const override {
        return id + "|" + tenThietBi + "|" + maLoai + "|" + maTram + "|" + ngayLapDat.toString() + "|" + trangThai;
    }

    // Tach 1 dong tu file txt bo vao cac thuoc tinh cua doi tuong tren RAM
    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|'); // Tach mang dua vao dau |
        if (data.size() >= 6) { // Phai co du 6 truong de tranh tran mang
            setId(data[0]);               // Id
            tenThietBi = data[1];         // Ten
            maLoai = data[2];             // Ma loai
            maTram = data[3];             // Ma tram
            ngayLapDat.docchuoi(data[4]); // Nhet chuoi vao class Date
            trangThai = data[5];          // Trang thai
        }
    }

    // Ghi de ham hien thi thong tin len man hinh console
    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(25) << (tenThietBi.length() > 22 ? tenThietBi.substr(0, 19) + "..." : tenThietBi) << "| " // Thu gon neu chuoi qua dai
            << std::setw(10) << maLoai << "| "
            << std::setw(10) << maTram << "| "
            << std::setw(14) << ngayLapDat.toString() << "| " // Keo chuoi ngay lap dat
            << std::setw(15) << trangThai << "\n";
    }

    // Ham static in tieu de cho the hien dang bang tren console
    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma TB" << "| "
            << std::setw(25) << "Ten Thiet Bi" << "| "
            << std::setw(10) << "Ma Loai" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(14) << "Ngay Lap" << "| "
            << std::setw(15) << "Trang Thai" << "\n";
        std::cout << std::string(90, '-') << "\n";
    }
};
