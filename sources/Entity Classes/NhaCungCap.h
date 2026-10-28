//Vo Manh Thang
#pragma once 
#include <iostream> 
#include <string>  
#include <memory>   
#include "Repository.h" 
#include "NhaCungCap.h"
#include "PhieuBaoTri.h"
#include "LoaiThietBi.h" 
#include "Inputhelper.h" 
#include "Exceptions.h"  

class MenuNhaCungCap {
private:
	// Thuat toan bat loi nhap lieu:
		// Chuong trinh khong bi dung dot ngot khi nhap sai
		// Su dung vong lap while(true) boc lay khoi try-catch. Bieu thuc Lambda (buoc) duoc truyen vao se 
		// thuc thi lenh nhap. Neu co bat ky loi nao (sai kieu du lieu, de trong), ngoai le se duoc nem ra, 
		// catch tai day hien thi thong bao va ep vong lap chay lai, buoc nguoi dung nhap lai cho den khi dung.
    template <class F>
    static void thulai(F buoc) {
        while (true) {
            try { buoc(); return; }
            catch (const Baseexception& e) { std::cout << "   -> [Canh bao]: " << e.getMessage() << " (Moi nhap lai) " << std::endl; }
            catch (const std::exception&) { std::cout << "   -> [Loi]: Gia tri khong hop le (Moi nhap lai)" << std::endl; }
        }
    }
public:
    static void run(Repository<NhaCungCap>& repoNCC, Repository<PhieuBaoTri>& repoPBT, Repository<LoaiThietBi>& repoLoai) {
        while (true) {
            std::cout << std::endl << "============================================" << std::endl;
            std::cout << "           QUAN LY NHA CUNG CAP           " << std::endl;
            std::cout << "============================================" << std::endl;
            std::cout << "1. Xem danh sach nha cung cap" << std::endl;
            std::cout << "2. Them nha cung cap moi" << std::endl;
            std::cout << "3. Cap nhat thong tin nha cung cap" << std::endl;
            std::cout << "4. Xoa nha cung cap" << std::endl;
            std::cout << "0. Quay lai menu chinh" << std::endl;
            std::cout << "--------------------------------------------" << std::endl;   

            int chon = Inputhelper::Docint(">> Chon (0-4): ");
            if (chon == 0) break;

            try {
				// [Chuc nang R - READ]: Xem danh sach Nha cung cap
                if (chon == 1) {
                    std::cout << std::endl;
                    const auto& ds = repoNCC.getAll();
                    if (ds.empty()) std::cout << "Danh sach Nha cung cap dang trong" << std::endl;
                    else {
                        NhaCungCap::displayHeader();
                        for (const auto& ncc : ds) ncc->display();
                    }
                }
                // [Chuc nang C - CREATE]: Them Nha cung cap moi
                else if (chon == 2) {
                    std::cout << std::endl << "-- Them Nha cung cap --" << std::endl;
                    auto ncc = std::make_shared<NhaCungCap>();

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Ma nha cung cap (4 so) [Enter de huy]: ", true);
                        if (ma.empty()) throw 0;    // Dung co che throw int de thoat ra menu nhanh

                        ncc->setId(Inputhelper::TuTaoMa(ma, "NCC"));
						// Tra cuu tren file xem ma NCC da ton tai chua (Khong duoc phep co 2 nha cung cap cung chung 1 ID)
                        if (repoNCC.findById(ncc->getId())) throw Ngoailerangbuoc("Loi: Ma NCC nay da ton tai");
                        });
                    // Cac thuoc tinh con lai da duoc set dieu kien rang buoc (validator) ben trong Entity
                    thulai([&]() { ncc->setTenNCC(Inputhelper::Docstring("Ten Nha Cung Cap: ")); });
                    thulai([&]() { ncc->setDiaChi(Inputhelper::Docstring("Dia chi: ")); });
                    thulai([&]() { ncc->setSoDienThoai(Inputhelper::Docstring("So dien thoai: ")); });

					repoNCC.add(ncc);   // Ghi ban ghi xuong file .txt
                    std::cout << "Da ghi nhan Nha cung cap moi len he thong" << std::endl;
                }
                // [Chuc nang U - UPDATE]: Cap nhat thong tin Nha cung cap
                else if (chon == 3) {
                    if (repoNCC.getAll().empty()) {
                        std::cout << "Chua co nha cung cap nao de cap nhat" << std::endl;
                        continue;
                    }

                    std::cout << std::endl << "-- Cap nhat Nha cung cap --" << std::endl;
                    std::shared_ptr<NhaCungCap> ncc = nullptr;
                    std::string oldId = "";

                    while (true) {
                        try {
                            std::string rawId = Inputhelper::Docstring("Ma nha cung cap can cap nhat (4 so) hoac nhap '0' de huy: ");
                            if (rawId == "0") throw 0;

                            oldId = Inputhelper::TuTaoMa(rawId, "NCC");
                            ncc = repoNCC.findById(oldId);
                            if (!ncc) throw Ngoailekotimthay("Khong tim thay Nha cung cap nay tren he thong");
                            break;
                        }
                        catch (int) { break; }
                        catch (const Baseexception& e) { std::cout << "   -> " << e.getMessage() << std::endl; }
                    }

                    if (!ncc) continue;

                    std::string newId = oldId;
                    thulai([&]() {
                        std::string rawNewId = Inputhelper::Docstring("Ma Nha cung cap moi (Enter de giu nguyen '" + oldId + "'): ", true);
                        if (!rawNewId.empty()) {
                            std::string candidateId = Inputhelper::TuTaoMa(rawNewId, "NCC");
                            if (candidateId != oldId && repoNCC.findById(candidateId)) {
                                throw Ngoailerangbuoc("Loi: Ma nha cung cap moi nay da ton tai tren he thong");
                            }
                            newId = candidateId;
                        }
                        });

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten Nha cung cap moi (Enter de bo qua): ", true);
                        if (!ten.empty()) ncc->setTenNCC(ten);
                        });
                    thulai([&]() {
                        std::string dc = Inputhelper::Docstring("Dia chi moi (Enter de bo qua): ", true);
                        if (!dc.empty()) ncc->setDiaChi(dc);
                        });
                    thulai([&]() {
                        std::string sdt = Inputhelper::Docstring("So dien thoai moi (Enter de bo qua): ", true);
                        if (!sdt.empty()) ncc->setSoDienThoai(sdt);
                        });
					// Neu nguoi dung thay doi ma NCC, can xoa ban ghi cu va them ban ghi moi voi ma moi
                    if (newId != oldId) {
                        repoNCC.remove(oldId);
                        ncc->setId(newId);
                        repoNCC.add(ncc);
                    }

                    repoNCC.update();
					// Neu co bat ky Loai thiet bi nao dang tham chieu den nha cung cap nay, can dong bo lai ma va ten NCC sang danh muc Loai thiet bi
                    bool coDongBo = false;
                    for (auto& lt : repoLoai.getAll()) {
                        if (lt->getMaNCC() == oldId || lt->getMaNCC() == newId) {
                            lt->setMaNCC(newId);
                            lt->setTenNCC(ncc->getTenNCC());
                            coDongBo = true;
                        }
                    }
                    if (coDongBo) {
						repoLoai.update();  // Ghi lai danh muc Loai thiet bi sau khi dong bo ma va ten NCC
                        std::cout << "Da tu dong cap nhat ma va ten Nha cung cap sang danh muc Loai thiet bi" << std::endl;
                    }

                    std::cout << "Cap nhat Nha cung cap hoan tat" << std::endl;
                }
				// [Chuc nang D - DELETE]: Xoa Nha cung cap
                else if (chon == 4) {
                    if (repoNCC.getAll().empty()) {
                        std::cout << "Chua co Nha cung cap nao de xoa" << std::endl;
                        continue;
                    }

                    std::shared_ptr<NhaCungCap> ncc = nullptr;
                    std::string maChuan;

                    while (true) {
                        try {
                            std::string rawId = Inputhelper::Docstring("\nMa Nha cung cap can go bo (4 so) hoac nhap '0' de huy: ");
                            if (rawId == "0") throw 0;

                            maChuan = Inputhelper::TuTaoMa(rawId, "NCC");
                            ncc = repoNCC.findById(maChuan);
                            if (!ncc) throw Ngoailekotimthay("Khong tim thay Nha cung cap nay tren he thong");
                            break;
                        }
                        catch (int) { break; }
                        catch (const Baseexception& e) { std::cout << "   -> " << e.getMessage() << std::endl; }
                    }

                    if (!ncc) continue;
					// Kiem tra rang buoc: Neu co bat ky Loai thiet bi nao dang tham chieu den nha cung cap nay, khong duoc phep xoa
                    int loiThamChieu = 0;
                    std::string baoLoi = "TU CHOI XOA: Nha cung cap dang phan phoi cho cac Loai thiet bi sau:\n";
                    for (const auto& lt : repoLoai.getAll()) {
                        if (lt->getMaNCC() == maChuan) {
                            loiThamChieu++;
                            baoLoi += "- " + lt->getId() + "\n";
                        }
                    }
					// Kich hoat Exception chan thao tac Xoa
                    if (loiThamChieu > 0) {
                        baoLoi += "Vui long go bo hoac doi Nha cung cap cho cac thiet bi tren truoc de tranh mat ket noi du lieu";
                        throw Ngoailerangbuoc(baoLoi);
                    }

                    if (Inputhelper::Docstring("Chac chan xoa? (co/khong): ") == "co") {
                        repoNCC.remove(maChuan);    //Xoa tren memory roi tu dong update file
                        std::cout << "Da xoa thanh cong" << std::endl;
                    }
                    else {
                        std::cout << "Da huy thao tac xoa" << std::endl;
                    }
                }
            }
            catch (int) {
                std::cout << std::endl << "DA HUY THAO TAC VA QUAY LAI MENU CHINH" << std::endl;
            }
            // Bat cac loi nghiep vu nam ngoai khoi thulai()
            catch (const Baseexception& e) { std::cout << "[-] Loi nghiep vu: " << e.getMessage() << std::endl; }
        }
    }
};
