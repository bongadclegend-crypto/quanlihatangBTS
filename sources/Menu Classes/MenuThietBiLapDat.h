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
    // Ham Template: Cho phep "boc" mot doan lenh bat ky vao try-catch
    template <class F>
    static void thulai(F buoc) {
        while (true) { // Lap lai cho den khi thao tac dung
            try {
                buoc(); // Chay doan logic Lambda chuyen vao
                return; // Dung thi cat vong lap va di tiep
            }
            catch (const Baseexception& e) { // Bat loi nghiep vu
                std::cout << "[!] " << e.getMessage() << " -> Moi nhap lai\n";
            }
            catch (const std::exception&) { // Bat loi ngoai y muon cua C++
                std::cout << "[!] Gia tri khong hop le -> Moi nhap lai\n";
            }
        }
    }

public:
    // Truyen vao ca 3 Repository de xu ly tinh toan ven cua Khoa Ngoai (Relationship)
    static void run(Repository<ThietBiLapDat>& repoTB, Repository<LoaiThietBi>& repoLoai, Repository<TramBTS>& repoTram) {
        while (true) { // Vong lap ton tai cua giao dien Menu nay
            std::cout << "\n--- QUAN LY THIET BI LAP DAT TAI TRAM ---\n";
            std::cout << "1. Xem danh sach\n2. Lap dat thiet bi moi\n3. Cap nhat trang thai\n4. Go bo thiet bi (Xoa)\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-4): "); // Lay lua chon
            if (chon == 0) break; // Thoat neu chon 0

            try {
                // CHUC NANG 1: XEM DANH SACH
                if (chon == 1) {
                    std::cout << "\n";
                    repoTB.loadfromfile(); // Nạp data tu file len RAM
                    const auto& ds = repoTB.getAll();

                    if (ds.empty()) {
                        std::cout << "[-] Danh sach thiet bi lap dat dang trong.\n";
                    }
                    else {
                        ThietBiLapDat::displayHeader(); // In tieu de bang
                        for (const auto& tb : ds) tb->display(); // In tung the hien
                        std::cout << "Tong so ban ghi: " << ds.size() << "\n";
                    }
                }
                // CHUC NANG 2: THEM MOI
                else if (chon == 2) {
                    std::cout << "\n-- Lap Dat Thiet Bi --\n";
                    auto tb = std::make_shared<ThietBiLapDat>(); // Su dung make_shared tao object tren RAM

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Ma Thiet Bi (4 so): ");
                        tb->setId(Inputhelper::TuTaoMa(ma, "TB"));
                        if (repoTB.findById(tb->getId())) { // Kiem tra trung lap Primary Key
                            throw Ngoailerangbuoc("Loi: Ma Thiet Bi nay da ton tai!");
                        }
                        });

                    thulai([&]() { tb->setTenThietBi(Inputhelper::Docstring("Ten the hien : ")); });

                    // === XU LY RANG BUOC TOAN VEN: KHOA NGOAI 1 (MA LOAI) ===
                    thulai([&]() {
                        std::string maLoai = Inputhelper::Docstring("Ma Loai (4 so): ");
                        std::string maChuan = Inputhelper::TuTaoMa(maLoai, "LT");
                       
                        if (!repoLoai.findById(maChuan)) {
                            throw Ngoailerangbuoc("Tu choi: Ma Loai Thiet Bi khong ton tai trong danh muc!");
                        }
                        tb->setMaLoai(maLoai); // Luu vao object neu tim thay
                        });

                    // === XU LY RANG BUOC TOAN VEN: KHOA NGOAI 2 (MA TRAM BTS) ===
                    thulai([&]() {
                        std::string maTram = Inputhelper::Docstring("Ma Tram BTS (4 so): ");
                        std::string maChuan = Inputhelper::TuTaoMa(maTram, "BTS");
                        
                        if (!repoTram.findById(maChuan)) {
                            throw Ngoailerangbuoc("Tu choi: Ma Tram BTS khong ton tai tren he thong!");
                        }
                        tb->setMaTram(maTram);
                        });

                    thulai([&]() { tb->setNgayLapDat(Inputhelper::Docstring("Ngay lap dat (dd/mm/yyyy): ")); });
                    thulai([&]() { tb->setTrangThai(Inputhelper::Docstring("Trang thai (VD: Hoat dong/Bao tri/Loi): ")); });

                    repoTB.add(tb); // Ghi xuong file text
                    std::cout << "[+] Da ghi nhan thiet bi len he thong tram!\n";
                }
                // CHUC NANG 3: CAP NHAT
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat Trang Thai (Nhan Enter de bo qua) --\n";
                    std::string rawId = Inputhelper::Docstring("Ma TB can cap nhat (4 so): ");
                    auto tb = repoTB.findById(Inputhelper::TuTaoMa(rawId, "TB"));

                    if (!tb) throw Ngoailekotimthay("Khong tim thay Thiet bi nay!");

                    ThietBiLapDat banSao = *tb; // Tao ban sao de xu ly Update an toan

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten the hien moi (" + banSao.getTenThietBi() + "): ", true);
                        if (!ten.empty()) banSao.setTenThietBi(ten);
                        });

                    thulai([&]() { // Sua Ma Loai
                        std::string mLoai = Inputhelper::Docstring("Ma Loai moi (" + banSao.getMaLoai() + "): ", true);
                        if (!mLoai.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mLoai, "LT");
                            // Khi cap nhat, van phai check lai Foreign Key
                            if (!repoLoai.findById(maChuan)) throw Ngoailerangbuoc("Ma Loai moi khong ton tai!");
                            banSao.setMaLoai(mLoai);
                        }
                        });

                    thulai([&]() { // Sua Ma Tram
                        std::string mTram = Inputhelper::Docstring("Ma Tram moi (" + banSao.getMaTram() + "): ", true);
                        if (!mTram.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mTram, "BTS");
                            // Kiem tra Khoa Ngoai 2
                            if (!repoTram.findById(maChuan)) throw Ngoailerangbuoc("Ma Tram moi khong ton tai!");
                            banSao.setMaTram(mTram);
                        }
                        });

                    thulai([&]() { // Sua Ngay
                        std::string ngay = Inputhelper::Docstring("Ngay lap dat moi (" + banSao.getNgayLapDat() + "): ", true);
                        if (!ngay.empty()) banSao.setNgayLapDat(ngay);
                        });

                    thulai([&]() { // Sua Trang thai
                        std::string tt = Inputhelper::Docstring("Trang thai moi (" + banSao.getTrangThai() + "): ", true);
                        if (!tt.empty()) banSao.setTrangThai(tt);
                        });

                    *tb = banSao; // Sync du lieu vao Object Goc tren RAM
                    repoTB.update(); // Cap nhat file text
                    std::cout << "[+] Cap nhat hoan tat!\n";
                }
                // CHUC NANG 4: XOA
                else if (chon == 4) {
                    std::string rawId = Inputhelper::Docstring("\nMa Thiet Bi can go bo (4 so): ");
                    std::string ma = Inputhelper::TuTaoMa(rawId, "TB");

                    if (!repoTB.findById(ma)) throw Ngoailekotimthay("Khong tim thay Thiet bi nay!");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan go bo thiet bi khoi he thong? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoTB.remove(ma); // Xoa phan tu khoi vector va luu file
                        std::cout << "[+] Da go bo!\n";
                    }
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
            catch (const std::exception& e) { std::cout << "[-] Loi He Thong: " << e.what() << "\n"; }
        }
    }
};
