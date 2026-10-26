// Nguyễn Anh Tú - B24DCVT368
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
            catch (const Baseexception& e) { std::cout << "   -> [Canh bao]: " << e.getMessage() << " (moi nhap lai)" << std::endl; }
            catch (const std::exception&) { std::cout << "   -> [Loi]: Gia tri khong hop le (moi nhap lai)" << std::endl; }
        }
    }
public:
    template <class PhieuBaoTri>
    static void run(Repository<ThietBiLapDat>& repoTB, Repository<LoaiThietBi>& repoLoai, Repository<TramBTS>& repoTram, Repository<PhieuBaoTri>& repoPBT) {
        while (true) {
            std::cout << std::endl << "--- Quan ly thiet bi lap dat tai tram ---" << std::endl;
            std::cout << "1. Xem danh sach" << std::endl;
            std::cout << "2. Lap dat thiet bi moi" << std::endl;
            std::cout << "3. Cap nhat thong tin" << std::endl;
            std::cout << "4. Go bo thiet bi" << std::endl;
            std::cout << "0. Quay lai" << std::endl;

            int chon = Inputhelper::Docint(">> Chon (0-4): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << std::endl;
                    const auto& ds = repoTB.getAll();
                    if (ds.empty()) std::cout << "[-] Danh sach thiet bi lap dat dang trong." << std::endl;
                    else {
                        ThietBiLapDat::displayHeader();
                        for (const auto& tb : ds) tb->display();
                        std::cout << "Tong so ban ghi: " << ds.size() << std::endl;
                    }
                }
                else if (chon == 2) {
                    if (repoLoai.getAll().empty()) {
                        std::cout << "[-] Danh muc loai thiet bi dang trong. Vui long them truoc!" << std::endl;
                        continue;
                    }

                    std::cout << std::endl << "-- Lap dat thiet bi --" << std::endl;
                    auto tb = std::make_shared<ThietBiLapDat>();

                    thulai([&]() {
                        std::string rawMaLoai = Inputhelper::Docstring("Nhap ma loai thiet bi (4 so): ");
                        std::string maLoai = Inputhelper::TuTaoMa(rawMaLoai, "LT");
                        auto loai = repoLoai.findById(maLoai);
                        if (!loai) throw Ngoailerangbuoc("Tu choi: ma loai nay chua ton tai!");

                        std::cout << "   => [Hop le] Ban dang them cho loai: " << loai->getTenLoai() << std::endl;
                        tb->setMaLoai(maLoai);
                        });

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten the hien: ");
                        tb->setTenThietBi(ten);
                        });

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Nhap ma thiet bi (4 so): ");
                        tb->setId(Inputhelper::TuTaoMa(ma, "TB"));
                        if (repoTB.findById(tb->getId())) throw Ngoailerangbuoc("Loi: ma thiet bi nay da ton tai!");
                        });

                    thulai([&]() {
                        std::string rawMaTram = Inputhelper::Docstring("Ma tram BTS (4 so): ");
                        std::string maTram = Inputhelper::TuTaoMa(rawMaTram, "BTS");
                        if (!repoTram.findById(maTram)) throw Ngoailerangbuoc("Tu choi: ma tram BTS khong ton tai!");
                        tb->setMaTram(maTram);
                        });

                    thulai([&]() {
                        std::string ngay = Inputhelper::Docstring("Ngay lap dat (dd/mm/yyyy): ");
                        tb->setNgayLapDat(ngay);
                        });

                    thulai([&]() {
                        std::string tt = Inputhelper::Docstring("Trang thai (Loi / Hoat Dong): ");
                        tb->setTrangThai(tt);
                        });

                    repoTB.add(tb);
                    std::cout << "[+] Da ghi nhan thiet bi len he thong!" << std::endl;
                }
                else if (chon == 3) {
                    if (repoTB.getAll().empty()) {
                        std::cout << "[-] Chua co thiet bi nao de cap nhat!" << std::endl;
                        continue;
                    }

                    std::cout << std::endl << "-- Cap nhat (nhan enter de giu nguyen) --" << std::endl;
                    std::string rawId = Inputhelper::Docstring("Ma TB can cap nhat (4 so): ");

                    std::shared_ptr<ThietBiLapDat> tb = repoTB.findById(Inputhelper::TuTaoMa(rawId, "TB"));
                    if (!tb) throw Ngoailekotimthay("Khong tim thay thiet bi nay tren he thong!");

                    ThietBiLapDat banSao = *tb;

                    thulai([&]() {
                        std::string mLoai = Inputhelper::Docstring("Ma loai moi (" + banSao.getMaLoai() + "): ", true);
                        if (!mLoai.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mLoai, "LT");
                            auto loai = repoLoai.findById(maChuan);
                            if (!loai) throw Ngoailerangbuoc("Ma loai moi khong ton tai!");

                            std::cout << "   => [Hop le] Ten loai thiet bi la: " << loai->getTenLoai() << std::endl;
                            banSao.setMaLoai(maChuan);
                        }
                        });

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten the hien moi (" + banSao.getTenThietBi() + "): ", true);
                        if (!ten.empty()) banSao.setTenThietBi(ten);
                        });

                    thulai([&]() {
                        std::string mTram = Inputhelper::Docstring("Ma tram moi (" + banSao.getMaTram() + "): ", true);
                        if (!mTram.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mTram, "BTS");
                            if (!repoTram.findById(maChuan)) throw Ngoailerangbuoc("Ma tram moi khong ton tai!");
                            banSao.setMaTram(maChuan);
                        }
                        });

                    thulai([&]() { std::string ngay = Inputhelper::Docstring("Ngay lap dat moi (" + banSao.getNgayLapDat() + "): ", true); if (!ngay.empty()) banSao.setNgayLapDat(ngay); });
                    thulai([&]() { std::string tt = Inputhelper::Docstring("Trang thai moi (" + banSao.getTrangThai() + "): ", true); if (!tt.empty()) banSao.setTrangThai(tt); });

                    *tb = banSao;
                    repoTB.update();
                    std::cout << "[+] Cap nhat hoan tat!" << std::endl;
                }
                else if (chon == 4) {
                    if (repoTB.getAll().empty()) {
                        std::cout << "[-] Chua co thiet bi nao de xoa!" << std::endl;
                        continue;
                    }

                    std::string rawId = Inputhelper::Docstring("\nMa thiet bi can go bo (4 so): ");
                    std::string maChuan = Inputhelper::TuTaoMa(rawId, "TB");
                    std::shared_ptr<ThietBiLapDat> tb = repoTB.findById(maChuan);
                    if (!tb) throw Ngoailekotimthay("Khong tim thay thiet bi nay tren he thong!");

                    int loiThamChieu = 0;
                    for (const auto& pbt : repoPBT.getAll()) {
                        if (pbt->getMaTB() == maChuan) {
                            loiThamChieu++;
                            break;
                        }
                    }
                    if (loiThamChieu > 0) {
                        throw Ngoailerangbuoc("Tu choi xoa: thiet bi nay dang co lich su bao tri. Vui long xoa phieu bao tri do truoc!");
                    }

                    if (Inputhelper::Docstring("Chac chan go bo? (y/n): ") == "y") {
                        repoTB.remove(maChuan);
                        std::cout << "[+] Da go bo!" << std::endl;
                    }
                    else {
                        std::cout << "[*] Da huy thao tac xoa." << std::endl;
                    }
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi nghiep vu: " << e.getMessage() << std::endl; }
        }
    }
};
