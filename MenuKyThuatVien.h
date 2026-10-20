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
            std::cout << "1. Xem danh sach\n2. Them moi\n3. Cap nhat\n4. Xoa\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon so (0-4): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    KyThuatVien::displayHeader();
                    for (const auto& ktv : repoKTV.getAll()) ktv->display();
                }
                else if (chon == 2) {
                    std::cout << "\n-- Them Ky Thuat Vien --\n";
                    auto ktv = std::make_shared<KyThuatVien>();

                    while (true) {
                        try {
                            std::string ma = Inputhelper::Docstring("Ma KTV (4 so, Enter de Huy): ", true);
                            if (ma == "") break;

                            std::string id = Inputhelper::tuTaoMa(ma, "KTV");
                            if (repoKTV.findById(id)) throw Loixacthuc("Loi: Ma nay da ton tai!");

                            ktv->setId(id);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (ktv->getId() == "") { std::cout << "[!] Da huy.\n"; continue; }

                    while (true) {
                        try { ktv->setTenKTV(Inputhelper::Docstring("Ten KTV: ")); break; }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    while (true) {
                        try { ktv->setChuyenMon(Inputhelper::Docstring("Chuyen mon: ")); break; }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    while (true) {
                        try { ktv->setSoDienThoai(Inputhelper::Docstring("So dien thoai: ")); break; }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    repoKTV.add(ktv);
                    std::cout << "[+] Them thanh cong!\n";
                }
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat Thong Tin --\n";
                    std::string maSua;

                    while (true) {
                        try {
                            maSua = Inputhelper::Docstring("Ma KTV can sua (Enter de Huy): ", true);
                            if (maSua == "") break;

                            maSua = Inputhelper::tuTaoMa(maSua, "KTV");
                            if (!repoKTV.findById(maSua)) throw Loixacthuc("Loi: Khong tim thay Ma KTV nay!");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (maSua == "") { std::cout << "[!] Da huy.\n"; continue; }

                    auto ktv = repoKTV.findById(maSua);
                    std::cout << "\n--- Nhan Enter de bo qua neu khong muon sua ---\n";

                    while (true) {
                        try {
                            std::string ten = Inputhelper::Docstring("Ten moi: ", true);
                            if (ten != "") ktv->setTenKTV(ten);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    while (true) {
                        try {
                            std::string cm = Inputhelper::Docstring("Chuyen mon moi: ", true);
                            if (cm != "") ktv->setChuyenMon(cm);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    while (true) {
                        try {
                            std::string sdt = Inputhelper::Docstring("SDT moi: ", true);
                            if (sdt != "") ktv->setSoDienThoai(sdt);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    repoKTV.update();
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                else if (chon == 4) {
                    std::string maXoa;
                    while (true) {
                        try {
                            maXoa = Inputhelper::Docstring("\nMa KTV can xoa (Enter de Huy): ", true);
                            if (maXoa == "") break;
                            maXoa = Inputhelper::tuTaoMa(maXoa, "KTV");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (maXoa == "") { std::cout << "[!] Da huy.\n"; continue; }

                    if (!repoKTV.findById(maXoa)) throw Loixacthuc("Loi: Khong tim thay Ma KTV nay!");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa KTV nay? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoKTV.remove(maXoa);
                        std::cout << "[+] Da xoa thanh cong!\n";
                    }
                }
            }
            catch (const Baseexception& e) {
                std::cout << "[-] Loi he thong: " << e.getMessage() << "\n";
            }
        }
    }
};
