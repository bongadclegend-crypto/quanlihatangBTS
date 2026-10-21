
#pragma once 
#include <iostream> 
#include <string>   
#include <memory>   
#include "Repository.h" 
#include "ThietBiLapDat.h"
#include "LoaiThietBi.h" 
#include "TramBTS.h"     
#include "InputHelper.h" 
#include "Exceptions.h"  

class MenuThietBiLapDat { // Lop quan ly giao dien chuc nang
private:
    // Ham Template: Cho phep boc mot doan lenh bat ky vao try-catch
    template <class F>
    static void thulai(F buoc) {
        while (true) {
            try {
                buoc(); // Chay doan logic Lambda chuyen vao
                return; // Dung thi cat vong lap va di tiep
            }
            catch (const Baseexception& e) { // Bat loi nghiep vu "throw Loixacthuc"
                std::cout << "[!] " << e.getMessage() << " -> MOI NHAP LAI\n";
            }
            catch (const std::exception&) { // Bat loi ngoai y muon cua C++
                std::cout << "[!] GIA TRI KHONG HOP LE -> MOI NHAP LAI\n";
            }
        }
    }

public:
    // Truyen vao ca 3 Repository de xu ly tinh toan ven cua Khoa Ngoai 
    static void run(Repository<ThietBiLapDat>& repoTB, Repository<LoaiThietBi>& repoLoai, Repository<TramBTS>& repoTram) {
        while (true) {
            std::cout << "\n--- QUAN LY THIET BI LAP DAT TAI TRAM ---\n";
            std::cout << "1. XEM DANH SACH\n2. LAP DAT THIET BI MOI\n3. CAP NHAT TRANG THAI\n4. GO BO THIET BI (XOA)\n0. QUAY LAI\n";

            int chon = Inputhelper::Docint(">> CHON (0-4): ");
            if (chon == 0) break;

            try {
                // CHUC NANG 1: XEM DANH SACH
                if (chon == 1) {
                    std::cout << "\n";
                    repoTB.loadfromfile(); // Nạp data tu HDD len RAM
                    const auto& ds = repoTB.getAll();

                    if (ds.empty()) {
                        std::cout << "[-] DANH SACH THIET BI LAP DAT DANG TRONG.\n";
                    }
                    else {
                        ThietBiLapDat::displayHeader();
                        for (const auto& tb : ds) tb->display(); // Vong lap range-based for
                        std::cout << "TONG SO BAN GHI: " << ds.size() << "\n";
                    }
                }
                // CHUC NANG 2: THEM MOI
                else if (chon == 2) {
                    std::cout << "\n-- LAP DAT THIET BI --\n";
                    auto tb = std::make_shared<ThietBiLapDat>(); // Su dung make_shared tao object tren RAM

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("MA THIET BI (4 SO, VD SERI: 0001): ");
                        tb->setId(Inputhelper::TuTaoMa(ma, "TB"));
                        if (repoTB.findById(tb->getId())) { // Kiem tra trung lap Primary Key
                            throw Ngoailerangbuoc("LOI: MA THIET BI NAY DA TON TAI!");
                        }
                        });

                    thulai([&]() { tb->setTenThietBi(Inputhelper::Docstring("TEN THE HIEN (VD: RRU SECTOR 1): ")); });

                    // === XU LY RANG BUOC TOAN VEN: KHOA NGOAI 1 (MA LOAI) ===
                    thulai([&]() {
                        std::string maLoai = Inputhelper::Docstring("MA LOAI (4 SO): ");
                        std::string maChuan = Inputhelper::TuTaoMa(maLoai, "LT"); // Chuan hoa thanh LTxxxx
                        // Query tren kho cua LoaiThietBi xem LTxxxx nay co ton tai khong
                        if (!repoLoai.findById(maChuan)) {
                            throw Ngoailerangbuoc("TU CHOI: MA LOAI THIET BI KHONG TON TAI TRONG DANH MUC!");
                        }
                        tb->setMaLoai(maLoai); // Neu co thi luu vao Thuoc tinh Object
                        });

                    // === XU LY RANG BUOC TOAN VEN: KHOA NGOAI 2 (MA TRAM BTS) ===
                    thulai([&]() {
                        std::string maTram = Inputhelper::Docstring("MA TRAM BTS (4 SO): ");
                        std::string maChuan = Inputhelper::TuTaoMa(maTram, "BTS");
                        // Query tren kho cua TramBTS (Tram phai co tren doi thi moi duoc lap dat)
                        if (!repoTram.findById(maChuan)) {
                            throw Ngoailerangbuoc("TU CHOI: MA TRAM BTS KHONG TON TAI TREN HE THONG!");
                        }
                        tb->setMaTram(maTram);
                        });

                    thulai([&]() { tb->setNgayLapDat(Inputhelper::Docstring("NGAY LAP DAT (DD/MM/YYYY): ")); });
                    thulai([&]() { tb->setTrangThai(Inputhelper::Docstring("TRANG THAI (VD: HOAT DONG/BAO TRI/LOI): ")); });

                    repoTB.add(tb); // Ghi xuong the nho file text
                    std::cout << "[+] DA GHI NHAN THIET BI LEN HE THONG TRAM!\n";
                }
                // CHUC NANG 3: CAP NHAT
                else if (chon == 3) {
                    std::cout << "\n-- CAP NHAT TRANG THAI (NHAN ENTER DE BO QUA) --\n";
                    std::string rawId = Inputhelper::Docstring("MA TB CAN CAP NHAT (4 SO): ");
                    auto tb = repoTB.findById(Inputhelper::TuTaoMa(rawId, "TB"));

                    if (!tb) throw Ngoailekotimthay("KHONG TIM THAY THIET BI NAY!");

                    ThietBiLapDat banSao = *tb; // Tao ban sao tam tren mang luoi de xu ly Update an toan

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("TEN THE HIEN MOI (" + banSao.getTenThietBi() + "): ", true);
                        if (!ten.empty()) banSao.setTenThietBi(ten);
                        });

                    thulai([&]() { // Sua Ma Loai
                        std::string mLoai = Inputhelper::Docstring("MA LOAI MOI (" + banSao.getMaLoai() + "): ", true);
                        if (!mLoai.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mLoai, "LT");
                            // Khi cap nhat, van phai check lai Foreign Key y nhu luc them moi
                            if (!repoLoai.findById(maChuan)) throw Ngoailerangbuoc("MA LOAI MOI KHONG TON TAI!");
                            banSao.setMaLoai(mLoai);
                        }
                        });

                    thulai([&]() { // Sua Ma Tram
                        std::string mTram = Inputhelper::Docstring("MA TRAM MOI (" + banSao.getMaTram() + "): ", true);
                        if (!mTram.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mTram, "BTS");
                            // Validate new Foreign Key of BTS
                            if (!repoTram.findById(maChuan)) throw Ngoailerangbuoc("MA TRAM MOI KHONG TON TAI!");
                            banSao.setMaTram(mTram);
                        }
                        });

                    thulai([&]() { // Sua Ngay
                        std::string ngay = Inputhelper::Docstring("NGAY LAP DAT MOI (" + banSao.getNgayLapDat() + "): ", true);
                        if (!ngay.empty()) banSao.setNgayLapDat(ngay);
                        });

                    thulai([&]() { // Sua Trang thai
                        std::string tt = Inputhelper::Docstring("TRANG THAI MOI (" + banSao.getTrangThai() + "): ", true);
                        if (!tt.empty()) banSao.setTrangThai(tt);
                        });

                    *tb = banSao; // Sync du lieu da qua validate tu Ban Sao nguoc ve tro lai Object Goc 
                    repoTB.update(); // Cap nhat lai file txt 
                    std::cout << "[+] CAP NHAT HOAN TAT!\n";
                }
                // CHUC NANG 4: XOA
                else if (chon == 4) {
                    std::string rawId = Inputhelper::Docstring("\nMA THIET BI CAN GO BO (4 SO): ");
                    std::string ma = Inputhelper::TuTaoMa(rawId, "TB");

                    if (!repoTB.findById(ma)) throw Ngoailekotimthay("KHONG TIM THAY THIET BI NAY!");

                    std::string xacNhan = Inputhelper::Docstring("CHAC CHAN GO BO THIET BI KHOI HE THONG? (Y/N): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoTB.remove(ma); // Pass to Repository function xoa khoi vector + file sync
                        std::cout << "[+] DA GO BO!\n";
                    }
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] LOI NGHIEP VU: " << e.getMessage() << "\n"; }
            catch (const std::exception& e) { std::cout << "[-] LOI HE THONG: " << e.what() << "\n"; }
        }
    }
};
