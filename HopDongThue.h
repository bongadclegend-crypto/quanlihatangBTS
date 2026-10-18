#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <stdexcept>
#include "Entity.h"
#include "Inputhelper.h"
#include "Date.h"

class HopDongThue : public Entity {
private:
    std::string maTram;       // KHÓA NGOẠI: Tham chiếu sang TramBTS
    std::string tenChuNha;
    std::string soDienThoai;
    long long giaThueThang;   // Đơn vị: VNĐ
    Date ngayBatDau;          // Định dạng: dd/mm/yyyy
    int thoiHanThang;         // Số tháng thuê (phải > 0)

public:
    // Constructor mac dinh (danh cho Repository khi doc file)
    HopDongThue() : giaThueThang(0), thoiHanThang(0) {}

    // Constructor day du du lieu
    HopDongThue(std::string maHD, std::string maBTS, std::string ten,
        std::string sdt, long long gia, std::string ngay, int thoiHan) {

        setId(Inputhelper::tuTaoMa(maHD, "HD"));
        setMaTram(maBTS);
        setTenChuNha(ten);
        setSoDienThoai(sdt);
        setGiaThueThang(gia);
        setNgayBatDau(ngay);
        setThoiHanThang(thoiHan);
    }

    // getter setter
    std::string getMaTram() const { return maTram; }
    void setMaTram(const std::string& maBTS) {
        maTram = Inputhelper::tuTaoMa(maBTS, "BTS");
    }

    std::string getTenChuNha() const { return tenChuNha; }
    void setTenChuNha(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten);

        if (clean.empty()) throw Loixacthuc("Loi: Ten chu nha khong duoc de trong!");
        if (clean.find('|') != std::string::npos) throw Loixacthuc("Loi: Ten chu nha khong duoc chua ky tu | !");
        if (!Inputhelper::chiChuaChuCaiVaKhoangTrang(clean)) {
            throw Loixacthuc("Loi: Ten chu nha chi duoc phep chua chu cai va khoang trang!");
        }

        // Tự động viết hoa chữ cái đầu
        tenChuNha = Inputhelper::vietHoaChuDau(clean);
    }

    std::string getSoDienThoai() const { return soDienThoai; }
    void setSoDienThoai(const std::string& sdt) {
        std::string clean = Inputhelper::catkhoangtrang(sdt);
        // Tái sử dụng hàm check SĐT cực mạnh từ InputHelper
        Inputhelper::kiemTraSdt(clean);
        soDienThoai = clean;
    }

    long long getGiaThueThang() const { return giaThueThang; }
    void setGiaThueThang(long long gia) {
        if (gia <= 0) throw Loixacthuc("Loi: Gia thue moi thang phai lon hon 0 VND!");
        giaThueThang = gia;
    }

    std::string getNgayBatDau() const {
        return ngayBatDau.toString();
    }
    void setNgayBatDau(const std::string& ngay) {
        std::string clean = Inputhelper::catkhoangtrang(ngay);
        if (clean.empty()) throw Loixacthuc("Loi: Ngay bat dau khong duoc de trong!");
        ngayBatDau.docchuoi(clean); // Date.h đã lo khâu bẫy lỗi ngày tháng
    }

    int getThoiHanThang() const { return thoiHanThang; }
    void setThoiHanThang(int thoiHan) {
        if (thoiHan <= 0) throw Loixacthuc("Loi: Thoi han thue phai lon hon 0 thang!");
        thoiHanThang = thoiHan;
    }

    // doc ghi file txt
    std::string toFilerecord() const override {
        return id + "|" + maTram + "|" + tenChuNha + "|" +
            soDienThoai + "|" + std::to_string(giaThueThang) + "|" +
            ngayBatDau.toString() + "|" + std::to_string(thoiHanThang);
    }

    void fromFilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() != 7) {
            throw Loixacthuc("Loi: Ban ghi Hop dong phai co dung 7 truong!");
        }

        try {
            long long gia = std::stoll(data[4]);
            int thoiHan = std::stoi(data[6]);

            // Kỹ thuật an toàn: Tạo đối tượng tạm, đúng hết 100% mới gán vào bộ nhớ
            HopDongThue hdTam(data[0], data[1], data[2], data[3], gia, data[5], thoiHan);
            *this = hdTam;
        }
        catch (const std::invalid_argument&) {
            throw Loixacthuc("Loi: Gia thue hoac thoi han trong file khong phai la so!");
        }
        catch (const std::out_of_range&) {
            throw Loixacthuc("Loi: Gia thue hoac thoi han trong file vuot qua gioi han!");
        }
    }

    // hien du lieu tren con sole
    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(10) << maTram << "| "
            << std::setw(22) << (tenChuNha.length() > 19 ? tenChuNha.substr(0, 16) + "..." : tenChuNha) << "| "
            << std::setw(13) << soDienThoai << "| "
            << std::setw(15) << giaThueThang << "| "
            << std::setw(12) << ngayBatDau.toString() << "| "
            << std::setw(6) << thoiHanThang << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma HD" << "| "
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(22) << "Ten Chu Nha" << "| "
            << std::setw(13) << "SDT" << "| "
            << std::setw(15) << "Gia(VND)/Th" << "| "
            << std::setw(12) << "Ngay Thue" << "| "
            << std::setw(6) << "Thang" << "\n";
        std::cout << std::string(95, '-') << "\n";
    }
};