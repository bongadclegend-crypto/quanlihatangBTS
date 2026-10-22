// Thanh vien thuc hien: Vo Manh Thang
// Sua ngay 22/10
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <iomanip>
#include "PhieuBaoTri.h"
#include "Repository.h"
#include "InputHelper.h"
#include "TramBTS.h"
#include "Exceptions.h"

class MenuPhieuBaoTri {
public:
	static void run(Repository<PhieuBaoTri>& repoPBT, Repository<TramBTS>& repoTram) {
		while (true) {
			std::cout << "\n======================================\n";
			std::cout << "       QUAN LY PHIEU BAO TRI           \n";
			std::cout << "======================================\n";
			std::cout << "1. Xem danh sach phieu moi\n";
			std::cout << "2. Them phieu bao tri\n";
			std::cout << "3. Cap nhat phieu bao tri\n";
			std::cout << "4. Xoa phieu bao tri\n";
			std::cout << "0. Quay lai menu chinh\n";
			int chon = Inputhelper::Docint(">> Nhap lua chon: ");
			if (chon == 0) break;

			try {
				if (chon == 1) {
					std::cout << "\n";
					repoPBT.loadFromFile();
					PhieuBaoTri::displayHeader();
					for (const auto& pbt : repoPBT.getAll()) pbt->display();
				}
				else if (chon == 2) {
					std::cout << "\n-- Them Phieu Bao Tri --\n";
					std::string maPBT = Inputhelper::Docstring("Ma Phieu Bao Tri (4 so): ");

					//Ma Tram bat buoc phai nam trong danh muc he thong
					std::string maBTS = Inputhelper::tuTaoMa(Inputhelper::Docstring("Ma Tram BTS (4 so): "), "BTS");

					// Neu ham tra ve nullptr, lap tuc gian doan tien trinh them moi
					if (!repoTram.findById(maBTS)) {
						throw Loixacthuc("Tu choi: Ma Tram BTS khong ton tai tren he thong!");
					}

					// Qua buoc kiem tra tram moi cho nhap tiep cac thong tin khac
					std::string ngay = Inputhelper::Docstring("Ngay Bao Tri (dd/mm/yyyy): ");
					std::string nd = Inputhelper::Docstring("Noi Dung: ");
					std::string tt = Inputhelper::Docstring("Trang Thai: ");
					double cp = Inputhelper::Docdouble("Chi Phi: ");

					repoPBT.add(std::make_shared<PhieuBaoTri>(
						maPBT, maBTS, ngay, nd, tt, cp
					));
					std::cout << "[+] Them thanh cong!\n";
				}
				else if (chon == 3) {
					std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
					std::string rawId = Inputhelper::Docstring("Ma PBT can sua: ");
					auto pbt = repoPBT.findById(Inputhelper::tuTaoMa(rawId, "PBT"));
					if (!pbt) throw Loixacthuc("Khong tim thay Ma PBT nay!");

					std::string maPBT = Inputhelper::Docstring("Ma Phieu Bao Tri moi: ", true);
					if (!maPBT.empty()) pbt->setId(Inputhelper::tuTaoMa(maPBT, "PBT"));

					std::string maBTS = Inputhelper::Docstring("Ma Tram BTS moi (4 so): ", true);
					if (!maBTS.empty()) {
						std::string idTram = Inputhelper::tuTaoMa(maBTS, "BTS");

						// Rang buoc tinh nhat quan: Ma tram thay the cung phai hop le
						if (!repoTram.findById(idTram)) throw Loixacthuc("Tu choi: Ma Tram BTS moi khong ton tai!");
						pbt->setMaTram(idTram);
					}

					std::string ngay = Inputhelper::Docstring("Ngay Bao Tri moi: ", true); if (!ngay.empty()) pbt->setNgayBaoTri(ngay);
					std::string nd = Inputhelper::Docstring("Noi Dung moi: ", true); if (!nd.empty()) pbt->setNoiDung(nd);
					std::string tt = Inputhelper::Docstring("Trang Thai moi: ", true); if (!tt.empty()) pbt->setTrangThai(tt);
					double cp = Inputhelper::Docdouble("Chi Phi moi: ", true); if (cp != 0) pbt->setChiPhi(cp);

					repoPBT.update();
					std::cout << "[+] Cap nhat thanh cong!\n";
				}
				else if (chon == 4) {
					std::string rawId = Inputhelper::Docstring("\nMa PBT can xoa (4 so): ");
					std::string id = Inputhelper::tuTaoMa(rawId, "PBT");
					if (!repoPBT.findById(id)) throw Loixacthuc("Khong tim thay Ma PBT nay!");

					std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
					if (xacNhan == "y" || xacNhan == "Y") {
						repoPBT.remove(id);
						std::cout << "[+] Da xoa!\n";
					}
				}
				else {
					// Bat cac truong hop nguoi dung nhap so 5, 6, 7...
					std::cout << "[-] Lua chon khong hop le! Vui long chon tu 0 den 4.\n";
				}
			}
			catch (const Baseexception& e) { std::cout << "[-] Loi: " << e.getMessage() << "\n"; }
			catch (const std::exception&) { std::cout << "[-] Loi: Vui long nhap dung dinh dang!\n"; }
			catch (...) { std::cout << "[-] Loi khong xac dinh!\n"; }
		}
	}
};
