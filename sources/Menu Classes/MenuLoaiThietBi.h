#pragma once 
#include <iostream> 
#include <string>   
#include <memory>   
#include "Repository.h" 
#include "LoaiThietBi.h" 
#include "InputHelper.h" 
#include "Exceptions.h"  

class MenuLoaiThietBi {
private:
    // Ham Template thulai: Boc khoi lenh vao vong lap try-catch de chong crash
    template <class F>
    static void thulai(F buoc) {
        while (true) {
            try {
                buoc();
                return;
            }
            catch (const Baseexception& e) {
                std::cout << "[!] " << e.getMessage() << " -> Moi nhap lai\n";
            }
            catch (const std::exception&) {
                std::cout << "[!] Gia tri khong hop le -> Moi nhap lai\n";
            }
        }
    }

public:
    static void run(Repository<LoaiThietBi>& repoLoai) {
        while (true) {
            std::cout << "\n--- QUAN LY DANH MUC LOAI THIET BI ---\n";
            std::cout << "1. Xem danh sach\n2. Them loai moi\n3. Cap nhat\n4. Xoa\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-4): ");
            if (chon == 0) break;

            try {
                // CHUC NANG 1: XEM DANH SACH
                if (chon == 1) {
                    std::cout << "\n";
                    repoLoai.loadfromfile();
                    const auto& ds = repoLoai.getAll();
                    if (ds.empty()) {
                        std::cout << "[-] Danh sach dang trong.\n";
                    }
                    else {
                        LoaiThietBi::displayHeader();
                        for (const auto& lt : ds) lt->display();
                        std::cout << "Tong so ban ghi: " << ds.size() << "\n";
                    }
                }
                // CHUC NANG 2: THEM MOI
                else if (chon == 2) {
                    std::cout << "\n-- Them Loai Thiet Bi --\n";
                    auto lt = std::make_shared<LoaiThietBi>();

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Ma Loai (4 so, VD: 0001): ");
                        lt->setId(Inputhelper::TuTaoMa(ma, "LT"));
                        if (repoLoai.findById(lt->getId())) {
                            throw Ngoailerangbuoc("Loi: Ma Loai nay da ton tai!");
                        }
                        });

                    thulai([&]() { lt->setTenLoai(Inputhelper::Docstring("Ten Loai : ")); });
                    thulai([&]() { lt->setHangSanXuat(Inputhelper::Docstring("Hang San Xuat : ")); });
                    thulai([&]() { lt->setDacTinhKyThuat(Inputhelper::Docstring("Dac tinh KT: ")); });
                    thulai([&]() { lt->setCongSuatTieuThu(Inputhelper::Docdouble("Cong suat tieu thu (W): ")); });

                    repoLoai.add(lt);
                    std::cout << "[+] Da them loai thiet bi vao he thong!\n";
                }
                // CHUC NANG 3: CAP NHAT (Sua truc tiep, khong in du lieu cu)
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
                    std::string rawId = Inputhelper::Docstring("Ma Loai can sua (4 so): ");
                    auto lt = repoLoai.findById(Inputhelper::TuTaoMa(rawId, "LT"));
                    if (!lt) throw Ngoailekotimthay("Khong tim thay Ma Loai nay!");

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten moi: ", true);
                        if (!ten.empty()) lt->setTenLoai(ten);
                        });

                    thulai([&]() {
                        std::string hang = Inputhelper::Docstring("Hang SX moi: ", true);
                        if (!hang.empty()) lt->setHangSanXuat(hang);
                        });

                    thulai([&]() {
                        std::string dacTinh = Inputhelper::Docstring("Dac tinh KT moi: ", true);
                        if (!dacTinh.empty()) lt->setDacTinhKyThuat(dacTinh);
                        });

                    thulai([&]() {
                        double csMoi = lt->getCongSuatTieuThu();
                        if (Inputhelper::DocdoubleCapNhat("Cong suat moi (W): ", csMoi)) {
                            lt->setCongSuatTieuThu(csMoi);
                        }
                        });

                    repoLoai.update(); // Luu truc tiep xuong file vi lt tren RAM da duoc cap nhat
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                // CHUC NANG 4: XOA
                else if (chon == 4) {
                    std::string rawId = Inputhelper::Docstring("\nMa Loai can xoa (4 so): ");
                    std::string ma = Inputhelper::TuTaoMa(rawId, "LT");
                    if (!repoLoai.findById(ma)) throw Ngoailekotimthay("Khong tim thay Ma Loai nay!");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoLoai.remove(ma);
                        std::cout << "[+] Da xoa loai thiet bi!\n";
                    }
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
            catch (const std::exception& e) { std::cout << "[-] Loi He Thong: " << e.what() << "\n"; }
        }
    }
};
