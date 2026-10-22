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
#include "NhaCungCap.h" 
#include "ThietBiLapDat.h"
#include "Exceptions.h"

class MenuPhieuBaoTri {
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
	static void run(Repository<PhieuBaoTri>& repoPBT, Repository<TramBTS>& repoTram, Repository<NhaCungCap>& repoNCC, Repository<ThietBiLapDat>& repoTB) {
		while (true) {
			std::cout << "\n======================================\n";
			std::cout << "       QUAN LY PHIEU BAO TRI           \n";
			std::cout << "======================================\n";
			std::cout << "1. Xem danh sach phieu bao tri\n2. Lap phieu bao tri moi\n3. Cap nhat phieu bao tri\n4. Xoa phieu bao tri\n0. Quay lai menu chinh\n";
			int chon = Inputhelper::Docint(">> Nhap lua chon: ");
			if (chon == 0) break;

			try {
				if (chon == 1) {
					std::cout << "\n";
					if (repoPBT.getAll().empty()) {
						std::cout << "[-] Danh sach phieu bao tri dang trong.\n";
					}
					else {
						PhieuBaoTri::displayHeader();
						for (const auto& pbt : repoPBT.getAll()) pbt->display();
					}
				}
				else if (chon == 2) {
					std::cout << "\n-- Lap Phieu Bao Tri --\n";
					auto pbt = std::make_shared<PhieuBaoTri>();

					nhapAnToan([&]() {
						std::string maPBT = Inputhelper::Docstring("Ma Phieu Bao Tri (4 so): ");
						pbt->setId(Inputhelper::TuTaoMa(maPBT, "PBT"));
						if (repoPBT.findById(pbt->getId())) throw Loixacthuc("Ma Phieu nay da ton tai!");
						});
					nhapAnToan([&]() {
						std::string maBTS = Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma Tram BTS (4 so): "), "BTS");
						if (!repoTram.findById(maBTS)) throw Loixacthuc("Tu choi: Ma Tram BTS khong ton tai tren he thong!");
						pbt->setMaTram(maBTS);
						});
					nhapAnToan([&]() {
						std::string maTB = Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma Thiet Bi bao tri (4 so): "), "TB");
						auto tbObj = repoTB.findById(maTB);
						if (!tbObj) throw Loixacthuc("Tu choi: Ma Thiet Bi khong ton tai!");
						if (tbObj->getMaTram() != pbt->getMaTram()) {
							throw Ngoailerangbuoc("Loi logic: Thiet bi nay hien khong duoc lap dat tai tram " + pbt->getMaTram());
						}
						pbt->setMaTB(maTB);
						});
					nhapAnToan([&]() {
						std::string maNCC = Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma Nha Cung Cap thuc hien (4 so): "), "NCC");
						if (!repoNCC.findById(maNCC)) throw Loixacthuc("Tu choi: Ma NCC khong ton tai tren he thong!");
						pbt->setMaNCC(maNCC);
						});
					nhapAnToan([&]() { pbt->setNgayBaoTri(Inputhelper::Docstring("Ngay Bao Tri (dd/mm/yyyy): ")); });
					nhapAnToan([&]() { pbt->setNoiDung(Inputhelper::Docstring("Noi Dung: ")); });
					nhapAnToan([&]() { pbt->setTrangThai(Inputhelper::Docstring("Trang Thai: ")); });
					nhapAnToan([&]() { pbt->setChiPhi(Inputhelper::Docdouble("Chi Phi: ")); });

					repoPBT.add(pbt);
					std::cout << "[+] Them thanh cong!\n";
				}
				else if (chon == 3) {
					std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
					std::string rawId = Inputhelper::Docstring("Ma PBT can sua: ");
					auto pbt = repoPBT.findById(Inputhelper::TuTaoMa(rawId, "PBT"));
					if (!pbt) throw Loixacthuc("Khong tim thay Ma PBT nay!");

					std::string maBTS = Inputhelper::Docstring("Ma Tram BTS moi (4 so): ", true);
					if (!maBTS.empty()) {
						std::string idTram = Inputhelper::TuTaoMa(maBTS, "BTS");
						if (!repoTram.findById(idTram)) throw Loixacthuc("Tu choi: Ma Tram BTS moi khong ton tai!");
						pbt->setMaTram(idTram);
					}
					std::string maTB = Inputhelper::Docstring("Ma Thiet Bi moi (4 so): ", true);
					if (!maTB.empty()) {
						std::string idTB = Inputhelper::TuTaoMa(maTB, "TB");
						if (!repoTB.findById(idTB)) throw Loixacthuc("Tu choi: Ma TB moi khong ton tai!");
						pbt->setMaTB(idTB);
					}
					std::string maNCC = Inputhelper::Docstring("Ma Nha Cung Cap moi (4 so): ", true);
					if (!maNCC.empty()) {
						std::string idNCC = Inputhelper::TuTaoMa(maNCC, "NCC");
						if (!repoNCC.findById(idNCC)) throw Loixacthuc("Tu choi: Ma NCC moi khong ton tai!");
						pbt->setMaNCC(idNCC);
					}

					std::string ngay = Inputhelper::Docstring("Ngay Bao Tri moi: ", true); if (!ngay.empty()) pbt->setNgayBaoTri(ngay);
					std::string nd = Inputhelper::Docstring("Noi Dung moi: ", true); if (!nd.empty()) pbt->setNoiDung(nd);
					std::string tt = Inputhelper::Docstring("Trang Thai moi: ", true); if (!tt.empty()) pbt->setTrangThai(tt);

					std::string cpStr = Inputhelper::Docstring("Chi Phi moi: ", true);
					if (!cpStr.empty()) pbt->setChiPhi(std::stod(cpStr));

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
			catch (const std::exception&) { std::cout << "[-] Loi he thong, vui long kiem tra lai du lieu!\n"; }
		}
	}
};
