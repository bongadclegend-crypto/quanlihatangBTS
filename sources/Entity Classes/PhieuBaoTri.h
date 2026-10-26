//Hoang Tuan Thanh
#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include "Entity.h"
#include "Inputhelper.h"
#include "Date.h"
#include "Exceptions.h"

class PhieuBaoTri : public Entity {
private:
    std::string maTB;
    std::string maNCC;
    Date ngayBaoTri;
    std::string noiDung;
    std::string trangThai;
    long long chiPhi; // UPDATE: Chuyen sang long long de xu ly VND

public:
    PhieuBaoTri() { chiPhi = 0; }

    // DA XOA: maTram khoi Constructor
    PhieuBaoTri(std::string maPBT, std::string mTB, std::string mNCC, std::string ngay, std::string nd, std::string tt, long long cp) {
        setId(Inputhelper::TuTaoMa(maPBT, "PBT"));
        setMaTB(mTB);
        setMaNCC(mNCC);
        setNgayBaoTri(ngay);
        setNoiDung(nd);
        setTrangThai(tt);
        setChiPhi(cp);
    }

    std::string getMaTB() const { return maTB; }
    void setMaTB(const std::string& mTB) { this->maTB = Inputhelper::TuTaoMa(mTB, "TB"); }

    std::string getMaNCC() const { return maNCC; }
    void setMaNCC(const std::string& maNCC) { this->maNCC = Inputhelper::TuTaoMa(maNCC, "NCC"); }

    std::string getNgayBaoTri() const { return ngayBaoTri.toString(); }
    void setNgayBaoTri(const std::string& ngay) {
        std::string clean = Inputhelper::catkhoangtrang(ngay);
        if (clean.empty()) throw Loixacthuc("Loi: Ngay bao tri khong duoc de trong!");
        ngayBaoTri.docchuoi(clean);
    }

    std::string getNoiDung() const { return noiDung; }
    void setNoiDung(const std::string& nd) {
        std::string clean = Inputhelper::catkhoangtrang(nd);
        if (clean.empty()) throw Loixacthuc("Loi: Noi dung bao tri khong duoc de trong!");
        noiDung = Inputhelper::VietHoaChuDau(clean);
    }

    std::string getTrangThai() const { return trangThai; }
    void setTrangThai(const std::string& tt) {
        std::string clean = Inputhelper::catkhoangtrang(tt);
        if (clean.empty()) throw Loixacthuc("Loi: Trang thai bao tri khong duoc de trong!");
        std::string chuanHoa = Inputhelper::VietHoaChuDau(clean);

        // UPDATE: Chi cho phep 2 trang thai
        if (chuanHoa != "Hoan Tat" && chuanHoa != "Khong Hoan Tat") {
            throw Loixacthuc("Loi: Trang thai chi duoc phep nhap 'Hoan Tat' hoac 'Khong Hoan Tat'");
        }
        trangThai = chuanHoa;
    }

    long long getChiPhi() const { return chiPhi; }

    void setChiPhi(long long cp) {
        if (cp < 0) throw Loixacthuc("Loi: Chi phi bao tri khong duoc la so am!");
        chiPhi = cp;
    }

    void setChiPhi(const std::string& cpStr) {
        std::size_t pos = 0;
        long long cp;
        try {
            cp = std::stoll(cpStr, &pos);
        }
        catch (const std::exception&) {
            throw Loixacthuc("Loi: Chi phi phai la so hop le");
        }
        if (pos != cpStr.length()) {
            throw Loixacthuc("Loi: Chi phi chi duoc phep chua cac chu so");
        }
        setChiPhi(cp);
    }

    // Xoa maTram, giam xuong con 7 thuoc tinh
    std::string tofilerecord() const override {
        return id + "|" + maTB + "|" + maNCC + "|" + ngayBaoTri.toString() + "|" + noiDung + "|" + trangThai + "|" + std::to_string(chiPhi);
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');
        if (data.size() != 7) {
            throw Loixacthuc("Loi: Ban ghi Phieu Bao Tri phai co dung 7 truong");
        }

        long long cp;
        std::size_t vitricuoi = 0;
        try {
            cp = std::stoll(data[6], &vitricuoi);
        }
        catch (const std::exception&) {
            throw Loixacthuc("Loi: Chi phi trong file khong phai la so");
        }
        if (vitricuoi != data[6].size()) {
            throw Loixacthuc("Loi: Chi phi trong file chua ky tu khong hop le");
        }

        PhieuBaoTri pbtTam(data[0], data[1], data[2], data[3], data[4], data[5], cp);
        *this = pbtTam;
    }

    void display() const override {
        std::cout << std::left
            << std::setw(8) << id << "| "
            << std::setw(12) << maTB << "| "
            << std::setw(12) << maNCC << "| "
            << std::setw(15) << ngayBaoTri.toString() << "| "
            << std::setw(25) << (noiDung.length() > 22 ? noiDung.substr(0, 19) + "..." : noiDung) << "| "
            << std::setw(16) << trangThai << "| "
            << std::setw(15) << chiPhi << "\n";
    }

    static void displayHeader() {
        std::cout << std::left
            << std::setw(8) << "Ma PBT" << "| "
            << std::setw(12) << "Ma Thiet Bi" << "| "
            << std::setw(12) << "Ma NCC" << "| "
            << std::setw(15) << "Ngay Bao Tri" << "| "
            << std::setw(25) << "Noi Dung" << "| "
            << std::setw(16) << "Trang Thai" << "| "
            << std::setw(15) << "Chi Phi (VND)" << "\n";
        std::cout << std::string(110, '-') << "\n";
    }
};
