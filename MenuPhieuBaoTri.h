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

class MenuPhieuBaoTri {
public:
	static void run(Repository<PhieuBaoTri>& repoPBT) {
		while (true) {
			std::cout << "\n======================================\n";
			std::cout << "       QUAN LY PHIEU BAO TRI           \n";
			std::cout << "======================================\n";
			std::cout << "1. Xem danh sach phieu moi\n";
			std::cout << "2. Cap nhat phieu bao tri\n";
			std::cout << "3. Them phieu bao tri\n";
			std::cout << "4. Xoa phieu bao tri\n";
			std::cout << "0. Quay lai menu chinh\n";
			int chon = Inputhelper::Docint(">> Nhap lua chon: ");
			if (chon == 0) break;

			try {
				if (chon == 1) {
					std::cout << "\n";
					repoPBT.loadfromfile();
					PhieuBaoTri::displayHeader();
					for (const auto& pbt : repoPBT.getAll()) pbt->display();
				}
				else if (chon == 2) {
					std::cout << "\n-- Them Phieu Bao Tri --\n";
					std::string maPBT = Inputhelper::Docstring("Ma Phieu Bao Tri (4 so): ");
					std::string maBTS = Inputhelper::Docstring("Ma Tram BTS: ");
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
					auto pbt = repoPBT.findById(Inputhelper::TuTaoMa(rawId, "PBT"));
					if (!pbt) throw Loixacthuc("Khong tim thay Ma PBT nay!");

					std::string maPBT = Inputhelper::Docstring("Ma Phieu Bao Tri moi: ", true); if (!maPBT.empty()) pbt->setId(Inputhelper::TuTaoMa(maPBT, "PBT"));
					std::string maBTS = Inputhelper::Docstring("Ma Tram BTS moi: ", true); if (!maBTS.empty()) pbt->setMaTram(maBTS);
					std::string ngay = Inputhelper::Docstring("Ngay Bao Tri moi: ", true); if (!ngay.empty()) pbt->setNgayBaoTri(ngay);
					std::string nd = Inputhelper::Docstring("Noi Dung moi: ", true); if (!nd.empty()) pbt->setNoiDung(nd);
					std::string tt = Inputhelper::Docstring("Trang Thai moi: ", true); if (!tt.empty()) pbt->setTrangThai(tt);
					double cp = pbt->getChiPhi(); // Lấy chi phí cũ ra trước
					if (Inputhelper::DocdoubleCapNhat("Chi Phi moi: ", cp)) {
						pbt->setChiPhi(cp); // Nếu người dùng nhập số mới (không bấm Enter) thì cập nhật
					}

					repoPBT.update();
					std::cout << "[+] Cap nhat thanh cong!\n";
				}
				else if (chon == 4) {
					std::string rawId = Inputhelper::Docstring("\nMa PBT can xoa (4 so): ");
					std::string id = Inputhelper::TuTaoMa(rawId, "PBT");
					if (!repoPBT.findById(id)) throw Loixacthuc("Khong tim thay Ma PBT nay!");

					std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
					if (xacNhan == "y" || xacNhan == "Y") {
						repoPBT.remove(id);
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