#pragma once 
#include <iostream> 
#include <string>   
#include <memory>   
#include "Repository.h" 
#include "LoaiThietBi.h" 
#include "NhaCungCap.h" 
#include "InputHelper.h" 
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
    static void run(Repository<LoaiThietBi>& repoLoai, Repository<NhaCungCap>& repoNCC) {
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
                        std::string ma = Inputhelper::Docstring("Ma Loai (4 so, VD: 0001): ");
                        lt->setId(Inputhelper::TuTaoMa(ma, "LT"));
                        if (repoLoai.findById(lt->getId())) throw Ngoailerangbuoc("Loi: Ma Loai nay da ton tai!");
                        });

                    thulai([&]() { lt->setTenLoai(Inputhelper::Docstring("Ten Loai : ")); });
                    thulai([&]() { lt->setHangSanXuat(Inputhelper::Docstring("Hang San Xuat : ")); });
                    thulai([&]() { lt->setDacTinhKyThuat(Inputhelper::Docstring("Dac tinh KT: ")); });
                    thulai([&]() { lt->setCongSuatTieuThu(Inputhelper::Docdouble("Cong suat tieu thu (W): ")); });

                    thulai([&]() {
                        std::string mNCC = Inputhelper::TuTaoMa(Inputhelper::Docstring("Nhap Ma NCC (4 so): "), "NCC");
                        std::shared_ptr<NhaCungCap> ncc = repoNCC.findById(mNCC);
                        if (!ncc) throw Ngoailerangbuoc("Loi: Ma NCC khong ton tai!");

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

                    std::cout << "\n-- Cap Nhat --\n";
                    std::shared_ptr<LoaiThietBi> lt = nullptr;

                    while (true) {
                        try {
                            std::string rawId = Inputhelper::Docstring("Ma Loai can sua (4 so) hoac '0' de huy: ");
                            if (rawId == "0") break;

                            lt = repoLoai.findById(Inputhelper::TuTaoMa(rawId, "LT"));
                            if (!lt) throw Ngoailekotimthay("Khong tim thay Ma Loai nay!");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "   -> " << e.getMessage() << "\n"; }
                    }

                    if (!lt) continue;

                    thulai([&]() { std::string ten = Inputhelper::Docstring("Ten moi: ", true); if (!ten.empty()) lt->setTenLoai(ten); });
                    thulai([&]() { std::string hang = Inputhelper::Docstring("Hang SX moi: ", true); if (!hang.empty()) lt->setHangSanXuat(hang); });
                    thulai([&]() { std::string dt = Inputhelper::Docstring("Dac tinh KT moi: ", true); if (!dt.empty()) lt->setDacTinhKyThuat(dt); });
                    thulai([&]() {
                        double csMoi = lt->getCongSuatTieuThu();
                        if (Inputhelper::DocdoubleCapNhat("Cong suat moi (W): ", csMoi)) lt->setCongSuatTieuThu(csMoi);
                        });

                    thulai([&]() {
                        std::string mNCC = Inputhelper::Docstring("Ma NCC moi (Enter de bo qua): ", true);
                        if (!mNCC.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mNCC, "NCC");
                            std::shared_ptr<NhaCungCap> ncc = repoNCC.findById(maChuan);
                            if (!ncc) throw Ngoailerangbuoc("Ma NCC khong ton tai!");

                            std::cout << "   => [Hop le] Da doi sang Nha Cung Cap: " << ncc->getTenNCC() << "\n";
                            lt->setMaNCC(maChuan);
                            lt->setTenNCC(ncc->getTenNCC());
                        }
                        });

                    repoLoai.update();
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                else if (chon == 4) {
                    if (repoLoai.getAll().empty()) {
                        std::cout << "[-] Chua co loai thiet bi nao de xoa!\n";
                        continue;
                    }

                    std::shared_ptr<LoaiThietBi> lt = nullptr;
                    std::string maChuan;

                    while (true) {
                        try {
                            std::string rawId = Inputhelper::Docstring("\nMa Loai can xoa (4 so) hoac '0' de huy: ");
                            if (rawId == "0") break;

                            maChuan = Inputhelper::TuTaoMa(rawId, "LT");
                            lt = repoLoai.findById(maChuan);
                            if (!lt) throw Ngoailekotimthay("Khong tim thay Ma Loai nay!");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "   -> " << e.getMessage() << "\n"; }
                    }

                    if (!lt) continue;

                    if (Inputhelper::Docstring("Chac chan xoa? (y/n): ") == "y") {
                        repoLoai.remove(maChuan);
                        std::cout << "[+] Da xoa!\n";
                    }
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
        }
    }
};
