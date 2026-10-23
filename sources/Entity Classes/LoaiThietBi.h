#pragma once 
#include <iostream>    
#include <iomanip>     
#include <vector>       
#include <string>     
#include "Entity.h"     
#include "InputHelper.h" 


class LoaiThietBi : public Entity {
private: 
    std::string tenLoai;          
    std::string hangSanXuat;      
    std::string dacTinhKyThuat;  
    double congSuatTieuThu;       

public:

    LoaiThietBi() : congSuatTieuThu(0.0) {} 

    
    LoaiThietBi(std::string ma, std::string ten, std::string hang, std::string dacTinh, double congSuat) {
        setId(Inputhelper::TuTaoMa(ma, "LT")); 
        setTenLoai(ten);                      
        setHangSanXuat(hang);                  
        setDacTinhKyThuat(dacTinh);           
        setCongSuatTieuThu(congSuat);         
    }

    
    std::string getTenLoai() const { return tenLoai; }

    
    void setTenLoai(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten); 
        if (clean.empty()) throw Loixacthuc("Loi: Ten loai thiet bi khong duoc de trong!"); 
        if (!Inputhelper::CoChuaChuCai(clean)) throw Loixacthuc("Loi: Ten loai phai chua it nhat 1 chu cai!");
        
        tenLoai = clean; 
    }

    
    std::string getHangSanXuat() const { return hangSanXuat; }

    // Ham setter kiem tra va cap nhat hang san xuat
    void setHangSanXuat(const std::string& hang) {
        std::string clean = Inputhelper::catkhoangtrang(hang); 
        if (clean.empty()) throw Loixacthuc("Loi: Hang san xuat khong duoc de trong!"); 
        if (!Inputhelper::chiChuaChuCaiVaKhoangTrang(clean)) { 
            throw Loixacthuc("Loi: Hang san xuat chi duoc chua chu cai va khoang trang!"); 
        }
        hangSanXuat = Inputhelper::VietHoaChuDau(clean); 
    }

    
    std::string getDacTinhKyThuat() const { return dacTinhKyThuat; }

    
    void setDacTinhKyThuat(const std::string& dacTinh) {
        std::string clean = Inputhelper::catkhoangtrang(dacTinh); 
        if (clean.empty()) throw Loixacthuc("Loi: Dac tinh ky thuat khong duoc de trong!");
        if (!Inputhelper::CoChuaChuCai(clean)) { 
            throw Loixacthuc("Loi: Dac tinh ky thuat phai chua it nhat 1 chu cai!");
        }
       
        dacTinhKyThuat = clean; 
    }

   
    double getCongSuatTieuThu() const { return congSuatTieuThu; }

    // Ham setter kiem tra va cap nhat cong suat
    void setCongSuatTieuThu(double congSuat) {
        if (congSuat < 0) throw Loixacthuc("Loi: Cong suat tieu thu phai >= 0W!");
        congSuatTieuThu = congSuat; 
    }

    
    std::string tofilerecord() const override {
        
        return id + "|" + tenLoai + "|" + hangSanXuat + "|" + dacTinhKyThuat + "|" + std::to_string(congSuatTieuThu);
    }

    
    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|'); 
        if (data.size() >= 5) { 
            setId(data[0]);                     
            tenLoai = data[1];                  
            hangSanXuat = data[2];              
            dacTinhKyThuat = data[3];           
            congSuatTieuThu = std::stod(data[4]);
        }
    }

    
    void display() const override {
        std::cout << std::left 
            << std::setw(8) << id << "| " 
            // Toan tu 3 ngoi: Kiem tra neu ten dai qua 22 ky tu thi cat lay 19 ky tu roi noi them "..."
            << std::setw(25) << (tenLoai.length() > 22 ? tenLoai.substr(0, 19) + "..." : tenLoai) << "| "
            << std::setw(15) << hangSanXuat << "| "       
            << std::setw(20) << dacTinhKyThuat << "| "    
            << std::setw(10) << congSuatTieuThu << "\n";  
    }

    
    static void displayHeader() {
        std::cout << std::left 
            << std::setw(8) << "Ma LT" << "| " 
            << std::setw(25) << "Ten Loai Thiet Bi" << "| " 
            << std::setw(15) << "Hang SX" << "| " 
            << std::setw(20) << "Dac Tinh KT" << "| " 
            << std::setw(10) << "CongSuat(W)" << "\n";
        std::cout << std::string(85, '-') << "\n";
    }
};
