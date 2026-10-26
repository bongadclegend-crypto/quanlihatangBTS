//Hoang Tuan Thanh
#pragma once 
#include <iostream> 
#include <string>   
#include <memory>   
#include "Repository.h" 
#include "LoaiThietBi.h" 
#include "NhaCungCap.h" 
#include "ThietBiLapDat.h" // Bat buoc include de chan xoa
#include "Inputhelper.h" 
#include "Exceptions.h"  

class MenuLoaiThietBi {
private:
    template <class F>
    static void thulai(F buoc) {
        while (true) {
            try { buoc(); return; }
            catch (const Baseexception& e) { std::cout << "[!] " << e.getMessage() << " -> Moi nhap lai\n"; }
            catch (const std::exception&) { std::cout << "[!] Gia tri khong hop le -> Moi nhap lai\n"; }
        }
    }
public:
    // UPDATE: Them repoTBLD vao de chan xoa
    template <class ThietBiLapDat>
    static void run(Repository<LoaiThietBi>& repoLoai, Repository<NhaCungCap>& repoNCC, Repository<ThietBiLapDat>& repoTBLD) {
        while (true) {
            std::cout << "\n--- QUAN LY DANH MUC LOAI THIET BI ---\n";
            std::cout << "1. Xem danh sach\n2. Them loai moi\n3. Cap nhat\n4. Xoa\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-4): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    const auto& ds = repoLoai.getAll();
                    if (ds.empty()) {
                        std::cout << "[-] Danh sach dang trong.\n";
                    }
                    else {
                        bool coLuuMoi = false;
                        for (auto& lt : ds) {
                            std::shared_ptr<NhaCungCap> ncc = repoNCC.findById(lt->getMaNCC());
                            if (ncc && ncc->getTenNCC() != lt->getTenNCC()) {
                                lt->setTenNCC(ncc->getTenNCC());
                                coLuuMoi = true;
                            }
                        }
                        if (coLuuMoi) repoLoai.update();

                        LoaiThietBi::displayHeader();
                        for (const auto& lt : ds) lt->display();
                        std::cout << "Tong so ban ghi: " << ds.size() << "\n";
                    }
                }
                else if (chon == 2) {
                    if (repoNCC.getAll().empty()) {
                        std::cout << "[-] Danh muc Nha Cung Cap dang trong. Vui long them NCC truoc!\n";
                        continue;
                    }

                    std::cout << "\n-- Them Loai Thiet Bi --\n";
                    auto lt = std::make_shared<LoaiThietBi>();

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Ma Loai (4 so, VD: 0001) [Enter de huy]: ", true);
                        if (ma.empty()) throw 0;

                        lt->setId(Inputhelper::TuTaoMa(ma, "LT"));
                        if (repoLoai.findById(lt->getId())) throw Ngoailerangbuoc("Loi: Ma Loai nay da ton tai!");
                        });

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten Loai [Enter de huy]: ", true);
                        if (ten.empty()) throw 0;
                        lt->setTenLoai(ten);
                        });

                    thulai([&]() {
                        std::string hang = Inputhelper::Docstring("Hang San Xuat [Enter de huy]: ", true);
                        if (hang.empty()) throw 0;
                        lt->setHangSanXuat(hang);
                        });

                    thulai([&]() {
                        std::string dacTinh = Inputhelper::Docstring("Dac tinh KT [Enter de huy]: ", true);
                        if (dacTinh.empty()) throw 0;
                        lt->setDacTinhKyThuat(dacTinh);
                        });

                    thulai([&]() {
                        std::string csStr = Inputhelper::Docstring("Cong suat tieu thu (W) [Enter de huy]: ", true);
                        if (csStr.empty()) throw 0;
                        lt->setCongSuatTieuThu(csStr);
                        });

                    thulai([&]() {
                        std::string mNCC = Inputhelper::Docstring("Nhap Ma NCC (4 so) [Enter de huy]: ", true);
                        if (mNCC.empty()) throw 0;

                        mNCC = Inputhelper::TuTaoMa(mNCC, "NCC");
                        std::shared_ptr<NhaCungCap> ncc = repoNCC.findById(mNCC);
                        if (!ncc) throw Ngoailerangbuoc("Loi: Ma NCC khong ton tai tren he thong!");

                        std::cout << "   => [Hop le] Da chon Nha Cung Cap: " << ncc->getTenNCC() << "\n";
                        lt->setMaNCC(mNCC);
                        lt->setTenNCC(ncc->getTenNCC());
                        });

                    repoLoai.add(lt);
                    std::cout << "[+] Da them loai thiet bi!\n";
                }
                else if (chon == 3) {
                    if (repoLoai.getAll().empty()) {
                        std::cout << "[-] Chua co loai thiet bi nao de cap nhat!\n";
                        continue;
                    }

                    std::cout << "\n-- Cap Nhat (Nhan Enter de giu nguyen) --\n";
                    std::string rawId = Inputhelper::Docstring("Ma Loai can sua (4 so) [Enter de huy]: ", true);
                    if (rawId.empty()) throw 0;

                    std::shared_ptr<LoaiThietBi> lt = repoLoai.findById(Inputhelper::TuTaoMa(rawId, "LT"));
                    if (!lt) throw Ngoailekotimthay("Khong tim thay Ma Loai nay!");

                    // Su dung Deep Copy de giu an toan du lieu
                    LoaiThietBi banSao = *lt;

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten moi (" + banSao.getTenLoai() + "): ", true);
                        if (!ten.empty()) banSao.setTenLoai(ten);
                        });

                    thulai([&]() {
                        std::string hang = Inputhelper::Docstring("Hang SX moi (" + banSao.getHangSanXuat() + "): ", true);
                        if (!hang.empty()) banSao.setHangSanXuat(hang);
                        });

                    thulai([&]() {
                        std::string dt = Inputhelper::Docstring("Dac tinh KT moi (" + banSao.getDacTinhKyThuat() + "): ", true);
                        if (!dt.empty()) banSao.setDacTinhKyThuat(dt);
                        });

                    thulai([&]() {
                        std::string csStr = Inputhelper::Docstring("Cong suat moi (" + std::to_string(banSao.getCongSuatTieuThu()) + " W): ", true);
                        if (!csStr.empty()) banSao.setCongSuatTieuThu(csStr);
                        });

                    thulai([&]() {
                        std::string mNCC = Inputhelper::Docstring("Ma NCC moi (" + banSao.getMaNCC() + "): ", true);
                        if (!mNCC.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mNCC, "NCC");
                            std::shared_ptr<NhaCungCap> ncc = repoNCC.findById(maChuan);
                            if (!ncc) throw Ngoailerangbuoc("Loi: Ma NCC khong ton tai!");

                            std::cout << "   => [Hop le] Da doi sang Nha Cung Cap: " << ncc->getTenNCC() << "\n";
                            banSao.setMaNCC(maChuan);
                            banSao.setTenNCC(ncc->getTenNCC());
                        }
                        });

                    *lt = banSao; // Ghi de khi da hoan thanh
                    repoLoai.update();
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                else if (chon == 4) {
                    if (repoLoai.getAll().empty()) {
                        std::cout << "[-] Chua co loai thiet bi nao de xoa!\n";
                        continue;
                    }

                    std::string rawId = Inputhelper::Docstring("\nMa Loai can xoa (4 so) [Enter de huy]: ", true);
                    if (rawId.empty()) throw 0;

                    std::string maChuan = Inputhelper::TuTaoMa(rawId, "LT");
                    std::shared_ptr<LoaiThietBi> lt = repoLoai.findById(maChuan);
                    if (!lt) throw Ngoailekotimthay("Khong tim thay Ma Loai nay!");

                    // UPDATE: Thuat toan chan xoa cung chong Orphan Data
                    int loiThamChieu = 0;
                    std::string baoLoi = "TU CHOI XOA: Loai Thiet Bi nay dang thuoc ve cac Thiet Bi Lap Dat sau:\n";

                    for (const auto& tb : repoTBLD.getAll()) {
                        if (tb->getMaLoai() == maChuan) {
                            loiThamChieu++;
                            baoLoi += "- " + tb->getId() + "\n";
                        }
                    }

                    if (loiThamChieu > 0) {
                        baoLoi += "Vui long go bo hoac thay doi Ma Loai cua cac thiet bi tren truoc!";
                        throw Ngoailerangbuoc(baoLoi);
                    }

                    if (Inputhelper::Docstring("Chac chan xoa? (y/n): ") == "y") {
                        repoLoai.remove(maChuan);
                        std::cout << "[+] Da xoa thanh cong!\n";
                    }
                    else {
                        std::cout << "[*] Da huy thao tac xoa.\n";
                    }
                }
            }
            // Bat tin hieu thoat nhanh tu phim Enter
            catch (int) {
                std::cout << "\n[*] DA HUY THAO TAC VA QUAY LAI MENU CHINH!\n";
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
        }
    }
};
