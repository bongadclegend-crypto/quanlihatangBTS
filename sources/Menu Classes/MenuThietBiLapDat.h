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
    static void run(Repository<ThietBiLapDat>& repoTB, Repository<LoaiThietBi>& repoLoai, Repository<TramBTS>& repoTram) {
        while (true) {
            std::cout << "\n--- QUAN LY THIET BI LAP DAT TAI TRAM ---\n";
            std::cout << "1. Xem danh sach\n2. Lap dat thiet bi moi\n3. Cap nhat thong tin\n4. Go bo thiet bi\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-4): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    const auto& ds = repoTB.getAll();
                    if (ds.empty()) std::cout << "[-] Danh sach thiet bi lap dat dang trong.\n";
                    else {
                        ThietBiLapDat::displayHeader();
                        for (const auto& tb : ds) tb->display();
                        std::cout << "Tong so ban ghi: " << ds.size() << "\n";
                    }
                }
                else if (chon == 2) {
                    if (repoLoai.getAll().empty()) {
                        std::cout << "[-] Danh muc Loai Thiet Bi dang trong. Vui long them Loai Thiet Bi truoc!\n";
                        continue;
                    }

                    std::cout << "\n-- Lap Dat Thiet Bi --\n";
                    auto tb = std::make_shared<ThietBiLapDat>();

                    thulai([&]() {
                        std::string maLoai = Inputhelper::TuTaoMa(Inputhelper::Docstring("Nhap Ma Loai Thiet Bi (4 so): "), "LT");
                        auto loai = repoLoai.findById(maLoai);
                        if (!loai) throw Ngoailerangbuoc("Tu choi: Ma Loai nay chua ton tai!");

                        std::cout << "   => [Hop le] Ban dang them cho loai: " << loai->getTenLoai() << "\n";
                        tb->setMaLoai(maLoai);
                        });

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten the hien: ");
                        tb->setTenThietBi(ten);
                        });

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Nhap Ma Thiet Bi (4 so): ");
                        tb->setId(Inputhelper::TuTaoMa(ma, "TB"));
                        if (repoTB.findById(tb->getId())) throw Ngoailerangbuoc("Loi: Ma Thiet Bi nay da ton tai!");
                        });

                    thulai([&]() {
                        std::string maTram = Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma Tram BTS (4 so): "), "BTS");
                        if (!repoTram.findById(maTram)) throw Ngoailerangbuoc("Tu choi: Ma Tram BTS khong ton tai!");
                        tb->setMaTram(maTram);
                        });

                    thulai([&]() { tb->setViTriLapDat(Inputhelper::Docstring("Vi tri lap dat (VD: Tu Rack 01): ")); });
                    thulai([&]() { tb->setNgayLapDat(Inputhelper::Docstring("Ngay lap dat (dd/mm/yyyy): ")); });
                    thulai([&]() { tb->setTrangThai(Inputhelper::Docstring("Trang thai (VD: Hoat dong): ")); });

                    repoTB.add(tb);
                    std::cout << "[+] Da ghi nhan thiet bi len he thong!\n";
                }
                else if (chon == 3) {
                    if (repoTB.getAll().empty()) {
                        std::cout << "[-] Chua co thiet bi nao de cap nhat!\n";
                        continue;
                    }

                    std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
                    std::shared_ptr<ThietBiLapDat> tb = nullptr;

                    while (true) {
                        try {
                            std::string rawId = Inputhelper::Docstring("Ma TB can cap nhat (4 so) hoac nhap '0' de huy: ");
                            if (rawId == "0") break;

                            tb = repoTB.findById(Inputhelper::TuTaoMa(rawId, "TB"));
                            if (!tb) throw Ngoailekotimthay("Khong tim thay Thiet bi nay tren he thong!");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "   -> " << e.getMessage() << "\n"; }
                    }

                    if (!tb) continue;

                    thulai([&]() {
                        std::string mLoai = Inputhelper::Docstring("Ma Loai moi: ", true);
                        if (!mLoai.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mLoai, "LT");
                            auto loai = repoLoai.findById(maChuan);
                            if (!loai) throw Ngoailerangbuoc("Ma Loai moi khong ton tai!");

                            std::cout << "   => [Hop le] Ten loai thiet bi la: " << loai->getTenLoai() << "\n";
                            tb->setMaLoai(maChuan);
                        }
                        });

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten the hien moi: ", true);
                        if (!ten.empty()) tb->setTenThietBi(ten);
                        });

                    thulai([&]() {
                        std::string mTram = Inputhelper::Docstring("Ma Tram moi: ", true);
                        if (!mTram.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mTram, "BTS");
                            if (!repoTram.findById(maChuan)) throw Ngoailerangbuoc("Ma Tram moi khong ton tai!");
                            tb->setMaTram(maChuan);
                        }
                        });

                    thulai([&]() { std::string vt = Inputhelper::Docstring("Vi tri lap dat moi: ", true); if (!vt.empty()) tb->setViTriLapDat(vt); });
                    thulai([&]() { std::string ngay = Inputhelper::Docstring("Ngay lap dat moi: ", true); if (!ngay.empty()) tb->setNgayLapDat(ngay); });
                    thulai([&]() { std::string tt = Inputhelper::Docstring("Trang thai moi: ", true); if (!tt.empty()) tb->setTrangThai(tt); });

                    repoTB.update();
                    std::cout << "[+] Cap nhat hoan tat!\n";
                }
                else if (chon == 4) {
                    if (repoTB.getAll().empty()) {
                        std::cout << "[-] Chua co thiet bi nao de xoa!\n";
                        continue;
                    }

                    std::shared_ptr<ThietBiLapDat> tb = nullptr;
                    std::string maChuan;

                    while (true) {
                        try {
                            std::string rawId = Inputhelper::Docstring("\nMa Thiet Bi can go bo (4 so) hoac nhap '0' de huy: ");
                            if (rawId == "0") break;

                            maChuan = Inputhelper::TuTaoMa(rawId, "TB");
                            tb = repoTB.findById(maChuan);
                            if (!tb) throw Ngoailekotimthay("Khong tim thay Thiet bi nay tren he thong!");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "   -> " << e.getMessage() << "\n"; }
                    }

                    if (!tb) continue;

                    if (Inputhelper::Docstring("Chac chan go bo? (y/n): ") == "y") {
                        repoTB.remove(maChuan);
                        std::cout << "[+] Da go bo!\n";
                    }
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
        }
    }
};
