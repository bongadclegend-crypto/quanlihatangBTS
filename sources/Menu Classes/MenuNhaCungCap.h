#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include "Repository.h"
#include "NhaCungCap.h"
#include "PhieuBaoTri.h"
#include "InputHelper.h"
#include "Exceptions.h"

class MenuNhaCungCap {
private:
	template <typename A>
	static void nhapAnToan(A thaoTac) {
		bool thanhCong = false;
		do {
			try { thaoTac(); thanhCong = true; }
			catch (const Baseexception& err) { std::cout << "   -> [Canh bao]: " << err.getMessage() << " (Vui long nhap lai)\n"; }
			catch (const std::exception&) { std::cout << "   -> [Loi kieu du lieu]: Dinh dang chua chinh xac. (Vui long nhap lai)\n"; }
		} while (!thanhCong);
	}

public:
	static void run(Repository<NhaCungCap>& repoNCC, Repository<PhieuBaoTri>& repoPBT) {
		while (true) {
			std::cout << "\n======================================\n";
			std::cout << "       QUAN LY NHA CUNG CAP           \n";
			std::cout << "======================================\n";
			std::cout << "1. Xem danh sach nha cung cap\n2. Them nha cung cap moi\n3. Cap nhat nha cung cap\n4. Xoa nha cung cap\n0. Quay lai menu chinh\n";
			int chon = Inputhelper::Docint(">> Nhap lua chon: ");
			if (chon == 0) break;

			try {
				if (chon == 1) {
					std::cout << "\n";
					if (repoNCC.getAll().empty()) {
						std::cout << "[-] Danh sach nha cung cap dang trong!\n";
					}
					else {
						NhaCungCap::displayHeader();
						for (const auto& ncc : repoNCC.getAll()) ncc->display();
					}
				}
				else if (chon == 2) {
					std::cout << "\n-- Them Nha Cung Cap --\n";
					auto ncc = std::make_shared<NhaCungCap>();

					nhapAnToan([&]() {
						std::string maNCC = Inputhelper::Docstring("Ma NCC (4 so): ");
						ncc->setMaNCC(maNCC);
						if (repoNCC.findById(ncc->getMaNCC())) throw Loixacthuc("Ma NCC nay da ton tai trong he thong!");
						});

					nhapAnToan([&]() { ncc->setTenNCC(Inputhelper::Docstring("Ten nha cung cap: ")); });
					nhapAnToan([&]() { ncc->setSoDienThoai(Inputhelper::Docstring("So dien thoai: ")); });
					nhapAnToan([&]() { ncc->setDiaChi(Inputhelper::Docstring("Dia chi: ")); });

					repoNCC.add(ncc);
					std::cout << "[+] Them thanh cong!\n";
				}
				else if (chon == 3) {
					std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
					std::string rawId = Inputhelper::Docstring("Ma NCC can sua: ");
					auto ncc = repoNCC.findById(Inputhelper::TuTaoMa(rawId, "NCC"));
					if (!ncc) throw Loixacthuc("Khong tim thay Ma NCC nay!");

					NhaCungCap banSao = *ncc;

					nhapAnToan([&]() {
						std::string s = Inputhelper::Docstring("Ten nha cung cap moi (" + banSao.getTenNCC() + "): ", true);
						if (!s.empty()) banSao.setTenNCC(s);
						});
					nhapAnToan([&]() {
						std::string s = Inputhelper::Docstring("So dien thoai moi (" + banSao.getSoDienThoai() + "): ", true);
						if (!s.empty()) banSao.setSoDienThoai(s);
						});
					nhapAnToan([&]() {
						std::string s = Inputhelper::Docstring("Dia chi moi (" + banSao.getDiaChi() + "): ", true);
						if (!s.empty()) banSao.setDiaChi(s);
						});

					*ncc = banSao;
					repoNCC.update();
					std::cout << "[+] Cap nhat thanh cong!\n";
				}
				else if (chon == 4) {
					std::string rawId = Inputhelper::Docstring("\nMa NCC can xoa (4 so): ");
					std::string id = Inputhelper::TuTaoMa(rawId, "NCC");

					if (!repoNCC.findById(id)) throw Loixacthuc("Khong tim thay Ma NCC nay!");

					for (const auto& pbt : repoPBT.getAll()) {
						if (pbt->getMaNCC() == id) throw Ngoailerangbuoc("Tu choi xoa! NCC nay dang duoc gan voi Phieu Bao Tri: " + pbt->getId());
					}

					std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
					if (xacNhan == "y" || xacNhan == "Y") {
						repoNCC.remove(id);
						std::cout << "[+] Da xoa!\n";
					}
				}
			}
			catch (const Baseexception& e) { std::cout << "[-] Loi: " << e.getMessage() << "\n"; }
			catch (const std::exception&) { std::cout << "[-] Loi he thong!\n"; }
		}
	}
};
