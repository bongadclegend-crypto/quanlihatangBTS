// Kieu Binh Nhan B24DCVT285
#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Repository.h"
#include "KyThuatVien.h"
#include "InputHelper.h"

class MenuKyThuatVien {
public:
    // Chay menu quan ly, truyen vao repoKTV de thao tac du lieu
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
                    // Duyet tung ktv trong repoKTV va in ra
                    for (const auto& ktv : repoKTV.getAll()) ktv->display();
                }
                else if (chon == 2) {
                    std::cout << "\n-- Them Ky Thuat Vien --\n";
					auto ktv = std::make_shared<KyThuatVien>(); // Tao doi tuong KTV moi
					// Nhap ma KTV, kiem tra xem ma da ton tai hay chua
                    while (true) {
                        try { 
                            std::string ma = Inputhelper::Docstring("Ma KTV (4 so, Enter de Huy): ", true);
                            if (ma == "") break;

                            std::string id = Inputhelper::tuTaoMa(ma, "KTV");
                            if (repoKTV.findById(id)) throw Loixacthuc("Loi: Ma nay da ton tai!");

                            ktv->setId(id); // Ma hop le luu vao KTV
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
					if (ktv->getId() == "") { std::cout << "[!] Da huy.\n"; continue; } // Huy thi quay lai menu
					// Nhap cac thong tin con lai
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

					repoKTV.add(ktv); // Them KTV vao repo va luu vao file
                    std::cout << "[+] Them thanh cong!\n";
                }
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat Thong Tin --\n";
                    std::string masua;
					// Nhap ma KTV can sua, kiem tra ton tai
                    while (true) {
                        try {
                            masua = Inputhelper::Docstring("Ma KTV can sua (Enter de Huy): ", true);
                            if (masua == "") break;

                            masua = Inputhelper::tuTaoMa(masua, "KTV");
                            if (!repoKTV.findById(masua)) throw Loixacthuc("Loi: Khong tim thay Ma KTV nay!");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (masua == "") { std::cout << "[!] Da huy.\n"; continue; }

					auto ktv = repoKTV.findById(masua); // Lay doi tuong KTV can sua
                    std::cout << "\n--- Nhan Enter de bo qua neu khong muon sua ---\n";
                    // Nhap thong tin moi, neu an enter thi bo qua
                    while (true) {
                        try {
                            std::string ten = Inputhelper::Docstring("Ten moi: ", true);
                            if (ten != "") ktv->setTenKTV(ten);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
					// Nhap chuyen mon moi, neu an enter thi bo qua
                    while (true) {
                        try {
                            std::string cm = Inputhelper::Docstring("Chuyen mon moi: ", true);
                            if (cm != "") ktv->setChuyenMon(cm);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
					// Nhap so dien thoai moi, neu an enter thi bo qua
                    while (true) {
                        try {
                            std::string sdt = Inputhelper::Docstring("SDT moi: ", true);
                            if (sdt != "") ktv->setSoDienThoai(sdt);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

					repoKTV.update(); // Luu lai thay doi vao file
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                else if (chon == 4) {
                    std::string maXoa;
					// Nhap ma KTV can xoa, kiem tra ton tai
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
					// Xac nhan truoc khi xoa
                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa KTV nay? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
						repoKTV.remove(maXoa); // Xoa KTV khoi repo va luu vao file
                        std::cout << "[+] Da xoa thanh cong!\n";
                    }
                }
            }
			// Bat loi tu cac ham trong repo va KTV, in ra thong bao loi
            catch (const Baseexception& e) {
                std::cout << "[-] Loi he thong: " << e.getMessage() << "\n";
            }
        }
    }
};
