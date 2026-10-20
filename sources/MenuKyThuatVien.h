#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Repository.h"
#include "KyThuatVien.h"
#include "InputHelper.h"

class MenuKyThuatVien {
public:
    static void run(Repository<KyThuatVien>& repoKTV) {
        while (true) {
            std::cout << "\n--- QUAN LY KY THUAT VIEN ---\n";
            std::cout << "1. Xem danh sach KTV\n";
            std::cout << "2. Them KTV moi\n";
            std::cout << "3. Cap nhat thong tin\n";
            std::cout << "4. Xoa KTV\n";
            std::cout << "0. Quay lai Menu Chinh\n";

            int chon = Inputhelper::Docint(">> Ban chon so may (0-4): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    KyThuatVien::displayHeader();
                    for (const auto& ktv : repoKTV.getAll()) {
                        ktv->display();
                    }
                }

                else if (chon == 2) {
                    std::cout << "\n-- Them Ky Thuat Vien --\n";
                    std::string ma = Inputhelper::Docstring("Nhap Ma KTV (chi can 4 so): ");
                    std::string ten = Inputhelper::Docstring("Nhap Ten KTV: ");
                    std::string cm = Inputhelper::Docstring("Nhap Chuyen mon: ");
                    std::string sdt = Inputhelper::Docstring("Nhap So dien thoai: ");

                    auto ktvMoi = std::make_shared<KyThuatVien>(ma, ten, cm, sdt);

                    repoKTV.add(ktvMoi);
                    std::cout << "[+] Da them thanh cong!\n";
                }

                else if (chon == 3) {
                    std::string maCanSua = Inputhelper::Docstring("Nhap Ma KTV can sua (4 so): ");
                    maCanSua = Inputhelper::TuTaoMa(maCanSua, "KTV");
                    auto ktv = repoKTV.findById(maCanSua);

                    if (ktv == nullptr) {
                        throw Loixacthuc("Khong tim thay Ma KTV nay!");
                    }

                    std::string tenMoi = Inputhelper::Docstring("Ten moi: ", true);
                    if (tenMoi != "") {
                        ktv->setTenKTV(tenMoi);
                    }

                    std::string cmMoi = Inputhelper::Docstring("Chuyen mon moi: ", true);
                    if (cmMoi != "") {
                        ktv->setChuyenMon(cmMoi);
                    }

                    std::string sdtMoi = Inputhelper::Docstring("SDT moi: ", true);
                    if (sdtMoi != "") {
                        ktv->setSoDienThoai(sdtMoi);
                    }

                    repoKTV.update();
                    std::cout << "[+] Da cap nhat thanh cong!\n";
                }

                else if (chon == 4) {
                    std::string maCanXoa = Inputhelper::Docstring("\nNhap Ma KTV can xoa (4 so): ");
                    maCanXoa = Inputhelper::TuTaoMa(maCanXoa, "KTV");

                    if (repoKTV.findById(maCanXoa) == nullptr) {
                        throw Loixacthuc("Khong tim thay Ma KTV nay!");
                    }

                    std::string xacNhan = Inputhelper::Docstring("Ban co chac chan xoa KTV nay? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoKTV.remove(maCanXoa);
                        std::cout << "[+] Da xoa thanh cong!\n";
                    }
                }
            }

            catch (const Baseexception& e) {
                std::cout << "[-] Loi: " << e.getMessage() << "\n";
            }
        }
    }
};
