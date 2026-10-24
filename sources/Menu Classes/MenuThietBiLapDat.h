#pragma once 
#include <iostream> 
#include <string>  
#include <memory>   
#include "Repository.h" 
#include "ThietBiLapDat.h"
#include "TramBTS.h"     
#include "NhaCungCap.h" 
#include "InputHelper.h" 
#include "Exceptions.h"  

class MenuThietBiLapDat {
private:
    template <class F>
    static void thulai(F buoc) {
        while (true) {
            try { buoc(); return; }
            catch (const Baseexception& e) { std::cout << "   -> [Canh bao]: " << e.getMessage() << " (Moi nhap lai)\n"; }
            catch (const std::exception&) { std::cout << "   -> [Loi]: Gia tri khong hop le (Moi nhap lai)\n"; }
        }
    }

public:
    // LUU Y: Da xoa tham so Repository<LoaiThietBi>& repoLoai khoi ham run()
    static void run(Repository<ThietBiLapDat>& repoTB, Repository<TramBTS>& repoTram, Repository<NhaCungCap>& repoNCC) {
        while (true) {
            std::cout << "\n--- QUAN LY THIET BI LAP DAT TAI TRAM ---\n";
            std::cout << "1. Xem danh sach\n2. Lap dat thiet bi moi\n3. Cap nhat thong tin\n4. Go bo thiet bi\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-4): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    const auto& ds = repoTB.getAll();
                    if (ds.empty()) {
                        std::cout << "[-] Danh sach thiet bi lap dat dang trong.\n";
                    }
                    else {
                        ThietBiLapDat::displayHeader();
                        for (const auto& tb : ds) tb->display();
                        std::cout << "Tong so ban ghi: " << ds.size() << "\n";
                    }
                }
                else if (chon == 2) {
                    std::cout << "\n-- Lap Dat Thiet Bi --\n";
                    auto tb = std::make_shared<ThietBiLapDat>();

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Ma Thiet Bi (4 so): ");
                        tb->setId(Inputhelper::TuTaoMa(ma, "TB"));
                        if (repoTB.findById(tb->getId())) throw Ngoailerangbuoc("Loi: Ma Thiet Bi nay da ton tai!");
                        });
                    thulai([&]() { tb->setTenThietBi(Inputhelper::Docstring("Ten the hien: ")); });

                    // Da xoa khoi nhap maLoai

                    thulai([&]() {
                        std::string maTram = Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma Tram BTS (4 so): "), "BTS");
                        if (!repoTram.findById(maTram)) throw Ngoailerangbuoc("Tu choi: Ma Tram BTS khong ton tai!");
                        tb->setMaTram(maTram);
                        });
                    thulai([&]() {
                        std::string maNCC = Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma Nha Cung Cap (4 so): "), "NCC");
                        if (!repoNCC.findById(maNCC)) throw Ngoailerangbuoc("Tu choi: Ma NCC khong ton tai tren he thong!");
                        tb->setMaNCC(maNCC);
                        });
                    thulai([&]() { tb->setNgayLapDat(Inputhelper::Docstring("Ngay lap dat (dd/mm/yyyy): ")); });
                    thulai([&]() { tb->setTrangThai(Inputhelper::Docstring("Trang thai (VD: Hoat dong/Bao tri/Loi): ")); });

                    repoTB.add(tb);
                    std::cout << "[+] Da ghi nhan thiet bi len he thong!\n";
                }
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
                    std::string rawId = Inputhelper::Docstring("Ma TB can cap nhat (4 so): ");
                    auto tb = repoTB.findById(Inputhelper::TuTaoMa(rawId, "TB"));
                    if (!tb) throw Ngoailekotimthay("Khong tim thay Thiet bi nay!");

                    // Ghi de truc tiep, khong tao banSao, khong in trang thai cu
                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten the hien moi: ", true);
                        if (!ten.empty()) tb->setTenThietBi(ten);
                        });

                    // Da xoa khoi cap nhat maLoai

                    thulai([&]() {
                        std::string mTram = Inputhelper::Docstring("Ma Tram moi: ", true);
                        if (!mTram.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mTram, "BTS");
                            if (!repoTram.findById(maChuan)) throw Ngoailerangbuoc("Ma Tram moi khong ton tai!");
                            tb->setMaTram(mTram);
                        }
                        });
                    thulai([&]() {
                        std::string mNCC = Inputhelper::Docstring("Ma NCC moi: ", true);
                        if (!mNCC.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mNCC, "NCC");
                            if (!repoNCC.findById(maChuan)) throw Ngoailerangbuoc("Ma NCC moi khong ton tai!");
                            tb->setMaNCC(mNCC);
                        }
                        });
                    thulai([&]() {
                        std::string ngay = Inputhelper::Docstring("Ngay lap dat moi: ", true);
                        if (!ngay.empty()) tb->setNgayLapDat(ngay);
                        });
                    thulai([&]() {
                        std::string tt = Inputhelper::Docstring("Trang thai moi: ", true);
                        if (!tt.empty()) tb->setTrangThai(tt);
                        });

                    repoTB.update(); // Luu truc tiep su thay doi cua tb xuong file
                    std::cout << "[+] Cap nhat hoan tat!\n";
                }
                else if (chon == 4) {
                    std::string rawId = Inputhelper::Docstring("\nMa Thiet Bi can go bo (4 so): ");
                    std::string ma = Inputhelper::TuTaoMa(rawId, "TB");
                    if (!repoTB.findById(ma)) throw Ngoailekotimthay("Khong tim thay Thiet bi nay!");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan go bo thiet bi khoi he thong? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoTB.remove(ma);
                        std::cout << "[+] Da go bo!\n";
                    }
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
            catch (const std::exception& e) { std::cout << "[-] Loi He Thong: " << e.what() << "\n"; }
        }
    }
};
