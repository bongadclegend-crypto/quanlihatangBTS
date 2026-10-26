// Nguyễn Anh Tú - B24DCVT368
#pragma once 
#include <iostream> 
#include <string>   
#include <memory>   
#include "Repository.h" 
#include "LoaiThietBi.h" 
#include "NhaCungCap.h" 
#include "ThietBiLapDat.h" 
#include "Inputhelper.h" 
#include "Exceptions.h"  

class MenuLoaiThietBi {
private:
    template <class F>
    static void thulai(F buoc) {
        while (true) {
            try { buoc(); return; }
            catch (const Baseexception& e) { std::cout << "[!] " << e.getMessage() << " -> moi nhap lai" << std::endl; }
            catch (const std::exception&) { std::cout << "[!] Gia tri khong hop le -> moi nhap lai" << std::endl; }
        }
    }
public:
    template <class ThietBiLapDat>
    static void run(Repository<LoaiThietBi>& repoLoai, Repository<NhaCungCap>& repoNCC, Repository<ThietBiLapDat>& repoTBLD) {
        while (true) {
            std::cout << std::endl << "--- Quan ly danh muc loai thiet bi ---" << std::endl;
            std::cout << "1. Xem danh sach" << std::endl;
            std::cout << "2. Them loai moi" << std::endl;
            std::cout << "3. Cap nhat" << std::endl;
            std::cout << "4. Xoa" << std::endl;
            std::cout << "0. Quay lai" << std::endl;

            int chon = Inputhelper::Docint(">> Chon (0-4): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << std::endl;
                    const auto& ds = repoLoai.getAll();
                    if (ds.empty()) {
                        std::cout << "[-] Danh sach dang trong." << std::endl;
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
                        std::cout << "Tong so ban ghi: " << ds.size() << std::endl;
                    }
                }
                else if (chon == 2) {
                    if (repoNCC.getAll().empty()) {
                        std::cout << "[-] Danh muc nha cung cap dang trong. Vui long them ncc truoc!" << std::endl;
                        continue;
                    }

                    std::cout << std::endl << "-- Them loai thiet bi --" << std::endl;
                    auto lt = std::make_shared<LoaiThietBi>();

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Ma loai (4 so, VD: 0001): ");
                        lt->setId(Inputhelper::TuTaoMa(ma, "LT"));
                        if (repoLoai.findById(lt->getId())) throw Ngoailerangbuoc("Loi: ma loai nay da ton tai!");
                        });

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten loai: ");
                        lt->setTenLoai(ten);
                        });

                    thulai([&]() {
                        std::string hang = Inputhelper::Docstring("Hang san xuat: ");
                        lt->setHangSanXuat(hang);
                        });

                    thulai([&]() {
                        std::string dacTinh = Inputhelper::Docstring("Dac tinh KT: ");
                        lt->setDacTinhKyThuat(dacTinh);
                        });

                    thulai([&]() {
                        std::string csStr = Inputhelper::Docstring("Cong suat tieu thu (W): ");
                        lt->setCongSuatTieuThu(csStr);
                        });

                    thulai([&]() {
                        std::string mNCC = Inputhelper::Docstring("Nhap ma ncc (4 so): ");
                        mNCC = Inputhelper::TuTaoMa(mNCC, "NCC");
                        std::shared_ptr<NhaCungCap> ncc = repoNCC.findById(mNCC);
                        if (!ncc) throw Ngoailerangbuoc("Loi: ma ncc khong ton tai tren he thong!");

                        std::cout << "   => [Hop le] Da chon nha cung cap: " << ncc->getTenNCC() << std::endl;
                        lt->setMaNCC(mNCC);
                        lt->setTenNCC(ncc->getTenNCC());
                        });

                    repoLoai.add(lt);
                    std::cout << "[+] Da them loai thiet bi!" << std::endl;
                }
                else if (chon == 3) {
                    if (repoLoai.getAll().empty()) {
                        std::cout << "[-] Chua co loai thiet bi nao de cap nhat!" << std::endl;
                        continue;
                    }

                    std::cout << std::endl << "-- Cap nhat (nhan enter de giu nguyen) --" << std::endl;
                    std::string rawId = Inputhelper::Docstring("Ma loai can sua (4 so): ");

                    std::shared_ptr<LoaiThietBi> lt = repoLoai.findById(Inputhelper::TuTaoMa(rawId, "LT"));
                    if (!lt) throw Ngoailekotimthay("Khong tim thay ma loai nay!");

                    LoaiThietBi banSao = *lt;

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten moi (" + banSao.getTenLoai() + "): ", true);
                        if (!ten.empty()) banSao.setTenLoai(ten);
                        });

                    thulai([&]() {
                        std::string hang = Inputhelper::Docstring("Hang sx moi (" + banSao.getHangSanXuat() + "): ", true);
                        if (!hang.empty()) banSao.setHangSanXuat(hang);
                        });

                    thulai([&]() {
                        std::string dt = Inputhelper::Docstring("Dac tinh kt moi (" + banSao.getDacTinhKyThuat() + "): ", true);
                        if (!dt.empty()) banSao.setDacTinhKyThuat(dt);
                        });

                    thulai([&]() {
                        std::string csStr = Inputhelper::Docstring("Cong suat moi (" + std::to_string(banSao.getCongSuatTieuThu()) + " W): ", true);
                        if (!csStr.empty()) banSao.setCongSuatTieuThu(csStr);
                        });

                    thulai([&]() {
                        std::string mNCC = Inputhelper::Docstring("Ma ncc moi (" + banSao.getMaNCC() + "): ", true);
                        if (!mNCC.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mNCC, "NCC");
                            std::shared_ptr<NhaCungCap> ncc = repoNCC.findById(maChuan);
                            if (!ncc) throw Ngoailerangbuoc("Loi: ma ncc khong ton tai!");

                            std::cout << "   => [Hop le] Da doi sang nha cung cap: " << ncc->getTenNCC() << std::endl;
                            banSao.setMaNCC(maChuan);
                            banSao.setTenNCC(ncc->getTenNCC());
                        }
                        });

                    *lt = banSao;
                    repoLoai.update();
                    std::cout << "[+] Cap nhat thanh cong!" << std::endl;
                }
                else if (chon == 4) {
                    if (repoLoai.getAll().empty()) {
                        std::cout << "[-] Chua co loai thiet bi nao de xoa!" << std::endl;
                        continue;
                    }

                    std::string rawId = Inputhelper::Docstring("\nMa loai can xoa (4 so): ");
                    std::string maChuan = Inputhelper::TuTaoMa(rawId, "LT");
                    std::shared_ptr<LoaiThietBi> lt = repoLoai.findById(maChuan);
                    if (!lt) throw Ngoailekotimthay("Khong tim thay ma loai nay!");

                    int loiThamChieu = 0;
                    std::string baoLoi = "Tu choi xoa: loai thiet bi nay dang thuoc ve cac thiet bi lap dat sau:\n";

                    for (const auto& tb : repoTBLD.getAll()) {
                        if (tb->getMaLoai() == maChuan) {
                            loiThamChieu++;
                            baoLoi += "- " + tb->getId() + "\n";
                        }
                    }

                    if (loiThamChieu > 0) {
                        baoLoi += "Vui long go bo hoac thay doi ma loai cua cac thiet bi tren truoc!";
                        throw Ngoailerangbuoc(baoLoi);
                    }

                    if (Inputhelper::Docstring("Chac chan xoa? (y/n): ") == "y") {
                        repoLoai.remove(maChuan);
                        std::cout << "[+] Da xoa thanh cong!" << std::endl;
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
