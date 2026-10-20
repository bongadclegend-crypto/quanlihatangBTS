#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include "Repository.h"
#include "NhaCungCap.h"
#include "TramBTS.h"
#include "InputHelper.h"

class MenuNhaCungCap {
public:
	static void run(Repository<NhaCungCap>& repoNCC) {
		while (true) {
			std::cout << "\n======================================\n";
			std::cout << "       QUAN LY NHA CUNG CAP           \n";
			std::cout << "======================================\n";
			std::cout << "1. Xem danh sach nha cung cap\n";
			std::cout << "2. Them nha cung cap moi\n";
			std::cout << "3. Cap nhat nha cung cap\n";
			std::cout << "4. Xoa nha cung cap\n";
			std::cout << "0. Quay lai menu chinh\n";
			int chon = Inputhelper::Docint(">> Nhap lua chon: ");
			if (chon == 0) break;

			try {
				if (chon == 1) {
					std::cout << "\n";
					repoNCC.loadfromfile();
					NhaCungCap::displayHeader();
					for (const auto& ncc : repoNCC.getAll()) ncc->display();
				}
				else if (chon == 2) {
					std::cout << "\n-- Them Nha Cung Cap --\n";
					std::string maNCC = Inputhelper::Docstring("Ma NCC (4 so): ");
					std::string ten = Inputhelper::Docstring("Ten nha cung cap: ");
					std::string diaChi = Inputhelper::Docstring("Dia chi: ");
					std::string sdt = Inputhelper::Docstring("So dien thoai: ");

					repoNCC.add(std::make_shared<NhaCungCap>(
						maNCC, ten, diaChi, sdt
					));
					std::cout << "[+] Them thanh cong!\n";
				}
				else if (chon == 3) {
					std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
					std::string rawId = Inputhelper::Docstring("Ma NCC can sua: ");
					auto ncc = repoNCC.findById(Inputhelper::TuTaoMa(rawId, "NCC"));
					if (!ncc) throw Loixacthuc("Khong tim thay Ma NCC nay!");

					std::string maNCC = Inputhelper::Docstring("Ma NCC moi: ", true); if (!maNCC.empty()) ncc->setMaNCC(maNCC);
					std::string ten = Inputhelper::Docstring("Ten nha cung cap moi: ", true); if (!ten.empty()) ncc->setTenNCC(ten);
					std::string diaChi = Inputhelper::Docstring("Dia chi moi: ", true); if (!diaChi.empty()) ncc->setDiaChi(diaChi);
					std::string sdt = Inputhelper::Docstring("So dien thoai moi: ", true); if (!sdt.empty()) ncc->setSoDienThoai(sdt);

					repoNCC.update();
					std::cout << "[+] Cap nhat thanh cong!\n";
				}
				else if (chon == 4) {
					std::string rawId = Inputhelper::Docstring("\nMa NCC can xoa (4 so): ");
					std::string id = Inputhelper::TuTaoMa(rawId, "NCC");
					if (!repoNCC.findById(id)) throw Loixacthuc("Khong tim thay Ma NCC nay!");

					std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
					if (xacNhan == "y" || xacNhan == "Y") {
						repoNCC.remove(id);
						std::cout << "[+] Da xoa!\n";
					}
				}
			}
			catch (const Baseexception& e) { std::cout << "[-] Loi: " << e.getMessage() << "\n"; }
			catch (const std::exception&) { std::cout << "[-] Loi: Vui long nhap dung dinh dang!\n"; }
			catch (...) { std::cout << "[-] Loi khong xac dinh!\n"; }
		}
	}
};
