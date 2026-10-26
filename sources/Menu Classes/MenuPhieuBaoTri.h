//Vo Manh Thang
#pragma once 
#include <iostream> 
#include <string>  
#include <memory>   
#include <iomanip>
#include "Repository.h" 
#include "PhieuBaoTri.h"
#include "TramBTS.h"     
#include "NhaCungCap.h"  
#include "ThietBiLapDat.h"
#include "LoaiThietBi.h"
#include "Inputhelper.h" 
#include "Exceptions.h"  

class MenuPhieuBaoTri {
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
            catch (const Baseexception& e) { std::cout << "   -> [Canh bao]: " << e.getMessage() << " (Moi nhap lai)" << std::endl; }
            catch (const std::exception&) { std::cout << "   -> [Loi]: Gia tri khong hop le (Moi nhap lai)" << std::endl; }
        }
    }
public:
    static void run(Repository<PhieuBaoTri>& repoPBT, Repository<TramBTS>& repoTram, Repository<NhaCungCap>& repoNCC, Repository<ThietBiLapDat>& repoTB, Repository<LoaiThietBi>& repoLoai) {
        while (true) {
            std::cout << std::endl << "============================================" << std::endl;
            std::cout << "           QUAN LY PHIEU BAO TRI           " << std::endl;
            std::cout << "============================================" << std::endl;
            std::cout << "1. Xem danh sach phieu bao tri" << std::endl;
            std::cout << "2. Lap phieu bao tri moi" << std::endl;
            std::cout << "3. Cap nhat phieu bao tri" << std::endl;
            std::cout << "4. Xoa phieu bao tri" << std::endl;
            std::cout << "0. Quay lai menu chinh" << std::endl;
            std::cout << "--------------------------------------------" << std::endl;

            int chon = Inputhelper::Docint(">> Nhap lua chon: ");
            if (chon == 0) break;

            try {
                // [Chuc nang R - READ]: Xem danh sach phieu bao tri
                if (chon == 1) {
                    std::cout << std::endl;
                    const auto& ds = repoPBT.getAll();
                    if (ds.empty()) {
                        std::cout << "Danh sach phieu bao tri dang trong" << std::endl;
                    }
                    else {
                        PhieuBaoTri::displayHeader();
                        for (const auto& pbt : ds) {
                            pbt->display();
                        }
                        std::cout << "Tong so ban ghi: " << ds.size() << std::endl;
                    }
                }
                // [Chuc nang C - CREATE]: Them phieu bao tri moi 
                else if (chon == 2) {
                    // Kiem tra logic nghiep vu: Neu kho thiet bi rong thi khong the lap phieu bao tri
                    if (repoTB.getAll().empty()) {
                        std::cout << "Danh sach Thiet bi lap dat dang trong. Vui long lap dat thiet bi truoc" << std::endl;
                        continue;
                    }

                    std::cout << std::endl << "-- Lap Phieu bao tri moi --" << std::endl;
                    auto pbt = std::make_shared<PhieuBaoTri>();

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Ma phieu bao tri (4 so) [Enter de huy]: ", true);
                        if (ma.empty()) throw 0;    // Dung co che throw int de thoat ra menu nhanh
                        
                        // Tra cuu tren file xem ma phieu bao tri da ton tai chua
                        pbt->setId(Inputhelper::TuTaoMa(ma, "PBT"));
                        if (repoPBT.findById(pbt->getId())) throw Ngoailerangbuoc("Loi: Ma phieu bao tri nay da ton tai");
                        });

                    std::shared_ptr<ThietBiLapDat> tbTarget = nullptr;
                    thulai([&]() {
                        std::string maTB = Inputhelper::Docstring("Ma thiet bi (4 so) [Enter de huy]: ", true);
                        if (maTB.empty()) throw 0;
						
                        // Tra cuu tren file xem ma thiet bi co ton tai khong
                        maTB = Inputhelper::TuTaoMa(maTB, "TB");
                        tbTarget = repoTB.findById(maTB);
                        if (!tbTarget) throw Ngoailerangbuoc("Tu choi: Ma thiet bi nay khong ton tai");
                        pbt->setMaTB(tbTarget->getId());
                        });
					
                    // Tu dong xac dinh ma nha cung cap tu ma thiet bi -> giam thieu rui ro thao tac
                    std::string tuDongMaNCC = "";
                    auto loaiTb = repoLoai.findById(tbTarget->getMaLoai());
                    if (loaiTb) {
                        tuDongMaNCC = loaiTb->getMaNCC();
                    }

                    if (!tuDongMaNCC.empty()) {
                        pbt->setMaNCC(tuDongMaNCC);
                        std::cout << "   => [Tu dong]: Da xac dinh Nha cung cap cho thiet bi nay la: " << tuDongMaNCC << std::endl;
                    }
                    else {
                        thulai([&]() {
                            std::string maNCC = Inputhelper::Docstring("Ma nha cung cap (4 so) [Enter de huy]: ", true);
                            if (maNCC.empty()) throw 0;

                            maNCC = Inputhelper::TuTaoMa(maNCC, "NCC");
                            if (!repoNCC.findById(maNCC)) throw Ngoailerangbuoc("Tu choi: Ma nha cung cap khong ton tai");
                            pbt->setMaNCC(maNCC);
                            });
                    }
					
                    // Nhap lieu cho cac truong con lai
                    thulai([&]() {
                        std::string ngay = Inputhelper::Docstring("Ngay bao tri (dd/mm/yyyy) [Enter de huy]: ", true);
                        if (ngay.empty()) throw 0;
                        pbt->setNgayBaoTri(ngay);
                        });

                    thulai([&]() {
                        std::string nd = Inputhelper::Docstring("Noi dung bao tri [Enter de huy]: ", true);
                        if (nd.empty()) throw 0;
                        pbt->setNoiDung(nd);
                        });

                    thulai([&]() {
                        std::string tt = Inputhelper::Docstring("Trang thai (Hoan Tat / Khong Hoan Tat) [Enter de huy]: ", true);
                        if (tt.empty()) throw 0;
                        pbt->setTrangThai(tt);
                        });

                    thulai([&]() {
                        std::string cp = Inputhelper::Docstring("Chi phi (VND) [Enter de huy]: ", true);
                        if (cp.empty()) throw 0;
                        pbt->setChiPhi(cp);
                        });
					// Luu phieu bao tri moi vao repository
                    repoPBT.add(pbt);
                    std::cout << "Them phieu bao tri thanh cong" << std::endl;
                }
				// [Chuc nang U - UPDATE]: Cap nhat phieu bao tri
                else if (chon == 3) {
                    if (repoPBT.getAll().empty()) {
                        std::cout << "Chua co phieu bao tri nao de cap nhat" << std::endl;
                        continue;
                    }

                    std::cout << std::endl << "-- Cap nhat phieu bao tri (Nhan Enter de giu nguyen) --" << std::endl;
                    std::string rawId = Inputhelper::Docstring("Ma phieu bao tri can sua (4 so) [Enter de huy]: ", true);
                    if (rawId.empty()) throw 0;

                    std::shared_ptr<PhieuBaoTri> pbt = repoPBT.findById(Inputhelper::TuTaoMa(rawId, "PBT"));
                    if (!pbt) throw Ngoailekotimthay("Khong tim thay phieu bao tri nay");
					// Tao ban sao de thao tac, neu co loi thi khong lam thay doi tren doi tuong goc
                    PhieuBaoTri banSao = *pbt;

                    thulai([&]() {
                        std::string mTB = Inputhelper::Docstring("Ma thiet bi moi (" + banSao.getMaTB() + "): ", true);
                        if (!mTB.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mTB, "TB");
                            auto tbTarget = repoTB.findById(maChuan);
                            if (!tbTarget) throw Ngoailerangbuoc("Ma thiet bi khong ton tai");
                            banSao.setMaTB(maChuan);

                            auto loaiTb = repoLoai.findById(tbTarget->getMaLoai());
                            if (loaiTb && !loaiTb->getMaNCC().empty()) {
                                banSao.setMaNCC(loaiTb->getMaNCC());
                                std::cout << "   => [Tu dong]: Da cap nhat lai Ma nha cung cap la: " << loaiTb->getMaNCC() << std::endl;
                            }
                        }
                        });

                    thulai([&]() { std::string ngay = Inputhelper::Docstring("Ngay bao tri moi (" + banSao.getNgayBaoTri() + "): ", true); if (!ngay.empty()) banSao.setNgayBaoTri(ngay); });
                    thulai([&]() { std::string nd = Inputhelper::Docstring("Noi dung moi (" + banSao.getNoiDung() + "): ", true); if (!nd.empty()) banSao.setNoiDung(nd); });
                    thulai([&]() { std::string tt = Inputhelper::Docstring("Trang thai moi (" + banSao.getTrangThai() + "): ", true); if (!tt.empty()) banSao.setTrangThai(tt); });

                    thulai([&]() {
                        std::string cpStr = Inputhelper::Docstring("Chi phi moi (" + std::to_string(banSao.getChiPhi()) + " VND): ", true);
                        if (!cpStr.empty()) banSao.setChiPhi(cpStr);
                        });

                    *pbt = banSao;
                    repoPBT.update();
                    std::cout << "Cap nhat phieu bao tri thanh cong" << std::endl;
                }
				// [Chuc nang D - DELETE]: Xoa phieu bao tri
                else if (chon == 4) {
                    if (repoPBT.getAll().empty()) {
                        std::cout << "Chua co phieu bao tri nao de xoa" << std::endl;
                        continue;
                    }

                    std::string rawId = Inputhelper::Docstring("\nMa phieu bao tri can xoa (4 so) [Enter de huy]: ", true);
                    if (rawId.empty()) throw 0;

                    std::string maChuan = Inputhelper::TuTaoMa(rawId, "PBT");
                    std::shared_ptr<PhieuBaoTri> pbt = repoPBT.findById(maChuan);
                    if (!pbt) throw Ngoailekotimthay("Khong tim thay phieu bao tri nay");
					// Xac nhan truoc khi xoa tranh xoa nham du lieu quan trong
                    if (Inputhelper::Docstring("Chac chan xoa? (co/khong): ") == "co") {
                        repoPBT.remove(maChuan);
                        std::cout << "Da xoa thanh cong phieu bao tri" << std::endl;
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
            catch (const Baseexception& e) { std::cout << "Loi nghiep vu: " << e.getMessage() << std::endl; }
        }
    }
};
