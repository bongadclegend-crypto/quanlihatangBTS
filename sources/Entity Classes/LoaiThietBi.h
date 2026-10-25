//Hoang Tuan Thanh
#pragma once 
#include <iostream> 
#include <string>  
#include <memory>   
#include "Repository.h" 
#include "ThietBiLapDat.h"
#include "LoaiThietBi.h" 
#include "TramBTS.h"     
#include "Inputhelper.h" 
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
    template <class PhieuBaoTri>
    static void run(Repository<ThietBiLapDat>& repoTB, Repository<LoaiThietBi>& repoLoai, Repository<TramBTS>& repoTram, Repository<PhieuBaoTri>& repoPBT) {
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
                        std::cout << "[-] Danh muc Loai Thiet Bi dang trong. Vui long them truoc!\n";
                        continue;
                    }

                    std::cout << "\n-- Lap Dat Thiet Bi --\n";
                    auto tb = std::make_shared<ThietBiLapDat>();

                    thulai([&]() {
                        std::string rawMaLoai = Inputhelper::Docstring("Nhap Ma Loai Thiet Bi (4 so) [Enter de huy]: ", true);
                        if (rawMaLoai.empty()) throw 0;

                        std::string maLoai = Inputhelper::TuTaoMa(rawMaLoai, "LT");
                        auto loai = repoLoai.findById(maLoai);
                        if (!loai) throw Ngoailerangbuoc("Tu choi: Ma Loai nay chua ton tai!");

                        std::cout << "   => [Hop le] Ban dang them cho loai: " << loai->getTenLoai() << "\n";
                        tb->setMaLoai(maLoai);
                        });

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten the hien [Enter de huy]: ", true);
                        if (ten.empty()) throw 0;
                        tb->setTenThietBi(ten);
                        });

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Nhap Ma Thiet Bi (4 so) [Enter de huy]: ", true);
                        if (ma.empty()) throw 0;

                        tb->setId(Inputhelper::TuTaoMa(ma, "TB"));
                        if (repoTB.findById(tb->getId())) throw Ngoailerangbuoc("Loi: Ma Thiet Bi nay da ton tai!");
                        });

                    thulai([&]() {
                        std::string rawMaTram = Inputhelper::Docstring("Ma Tram BTS (4 so) [Enter de huy]: ", true);
                        if (rawMaTram.empty()) throw 0;

                        std::string maTram = Inputhelper::TuTaoMa(rawMaTram, "BTS");
                        if (!repoTram.findById(maTram)) throw Ngoailerangbuoc("Tu choi: Ma Tram BTS khong ton tai!");
                        tb->setMaTram(maTram);
                        });

                    // DA XOA VI TRI LAP DAT O DAY
                    thulai([&]() {
                        std::string ngay = Inputhelper::Docstring("Ngay lap dat (dd/mm/yyyy) [Enter de huy]: ", true);
                        if (ngay.empty()) throw 0;
                        tb->setNgayLapDat(ngay);
                        });

                    thulai([&]() {
                        std::string tt = Inputhelper::Docstring("Trang thai (Loi / Hoat Dong) [Enter de huy]: ", true);
                        if (tt.empty()) throw 0;
                        tb->setTrangThai(tt);
                        });

                    repoTB.add(tb);
                    std::cout << "[+] Da ghi nhan thiet bi len he thong!\n";
                }
                else if (chon == 3) {
                    if (repoTB.getAll().empty()) {
                        std::cout << "[-] Chua co thiet bi nao de cap nhat!\n";
                        continue;
                    }

                    std::cout << "\n-- Cap Nhat (Nhan Enter de giu nguyen) --\n";
                    std::string rawId = Inputhelper::Docstring("Ma TB can cap nhat (4 so) [Enter de huy]: ", true);
                    if (rawId.empty()) throw 0;

                    std::shared_ptr<ThietBiLapDat> tb = repoTB.findById(Inputhelper::TuTaoMa(rawId, "TB"));
                    if (!tb) throw Ngoailekotimthay("Khong tim thay Thiet bi nay tren he thong!");

                    ThietBiLapDat banSao = *tb;

                    thulai([&]() {
                        std::string mLoai = Inputhelper::Docstring("Ma Loai moi (" + banSao.getMaLoai() + "): ", true);
                        if (!mLoai.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mLoai, "LT");
                            auto loai = repoLoai.findById(maChuan);
                            if (!loai) throw Ngoailerangbuoc("Ma Loai moi khong ton tai!");

                            std::cout << "   => [Hop le] Ten loai thiet bi la: " << loai->getTenLoai() << "\n";
                            banSao.setMaLoai(maChuan);
                        }
                        });

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten the hien moi (" + banSao.getTenThietBi() + "): ", true);
                        if (!ten.empty()) banSao.setTenThietBi(ten);
                        });

                    thulai([&]() {
                        std::string mTram = Inputhelper::Docstring("Ma Tram moi (" + banSao.getMaTram() + "): ", true);
                        if (!mTram.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mTram, "BTS");
                            if (!repoTram.findById(maChuan)) throw Ngoailerangbuoc("Ma Tram moi khong ton tai!");
                            banSao.setMaTram(maChuan);
                        }
                        });

                    // DA XOA VI TRI LAP DAT O DAY
                    thulai([&]() { std::string ngay = Inputhelper::Docstring("Ngay lap dat moi (" + banSao.getNgayLapDat() + "): ", true); if (!ngay.empty()) banSao.setNgayLapDat(ngay); });
                    thulai([&]() { std::string tt = Inputhelper::Docstring("Trang thai moi (" + banSao.getTrangThai() + "): ", true); if (!tt.empty()) banSao.setTrangThai(tt); });

                    *tb = banSao;
                    repoTB.update();
                    std::cout << "[+] Cap nhat hoan tat!\n";
                }
                else if (chon == 4) {
                    if (repoTB.getAll().empty()) {
                        std::cout << "[-] Chua co thiet bi nao de xoa!\n";
                        continue;
                    }

                    std::string rawId = Inputhelper::Docstring("\nMa Thiet Bi can go bo (4 so) [Enter de huy]: ", true);
                    if (rawId.empty()) throw 0;

                    std::string maChuan = Inputhelper::TuTaoMa(rawId, "TB");
                    std::shared_ptr<ThietBiLapDat> tb = repoTB.findById(maChuan);
                    if (!tb) throw Ngoailekotimthay("Khong tim thay Thiet bi nay tren he thong!");

                    // Thuat toan quet cheo chan xoa neu Thiet bi dang co Phieu Bao Tri
                    int loiThamChieu = 0;
                    for (const auto& pbt : repoPBT.getAll()) {
                        if (pbt->getMaTB() == maChuan) {
                            loiThamChieu++;
                            break;
                        }
                    }
                    if (loiThamChieu > 0) {
                        throw Ngoailerangbuoc("TU CHOI XOA: Thiet bi nay dang co lich su bao tri. Vui long xoa Phieu Bao Tri do truoc!");
                    }

                    if (Inputhelper::Docstring("Chac chan go bo? (y/n): ") == "y") {
                        repoTB.remove(maChuan);
                        std::cout << "[+] Da go bo!\n";
                    }
                    else {
                        std::cout << "[*] Da huy thao tac xoa.\n";
                    }
                }
            }
            catch (int) {
                std::cout << "\n[*] DA HUY THAO TAC VA QUAY LAI MENU CHINH!\n";
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
        }
    }
};
