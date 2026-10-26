//Hoang Tuan Thanh
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
    template <class F>
    static void thulai(F buoc) {
        while (true) {
            try { buoc(); return; }
            catch (const Baseexception& e) { std::cout << "   -> [Canh bao]: " << e.getMessage() << " (Moi nhap lai)\n"; }
            catch (const std::exception&) { std::cout << "   -> [Loi]: Gia tri khong hop le (Moi nhap lai)\n"; }
        }
    }
public:
    static void run(Repository<PhieuBaoTri>& repoPBT, Repository<TramBTS>& repoTram, Repository<NhaCungCap>& repoNCC, Repository<ThietBiLapDat>& repoTB, Repository<LoaiThietBi>& repoLoai) {
        while (true) {
            std::cout << "\n============================================\n";
            std::cout << "           QUAN LY PHIEU BAO TRI           \n";
            std::cout << "============================================\n";
            std::cout << "1. Xem danh sach phieu bao tri\n";
            std::cout << "2. Lap phieu bao tri moi\n";
            std::cout << "3. Cap nhat phieu bao tri\n";
            std::cout << "4. Xoa phieu bao tri\n";
            std::cout << "0. Quay lai menu chinh\n";
            std::cout << "--------------------------------------------\n";

            int chon = Inputhelper::Docint(">> Nhap lua chon: ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    const auto& ds = repoPBT.getAll();
                    if (ds.empty()) {
                        std::cout << "[-] Danh sach phieu bao tri dang trong.\n";
                    }
                    else {
                        PhieuBaoTri::displayHeader();
                        for (const auto& pbt : ds) {
                            pbt->display();
                        }
                        std::cout << "Tong so ban ghi: " << ds.size() << "\n";
                    }
                }
                else if (chon == 2) {
                    if (repoTB.getAll().empty()) {
                        std::cout << "[-] Danh sach Thiet Bi Lap Dat dang trong. Vui long lap dat thiet bi truoc!\n";
                        continue;
                    }

                    std::cout << "\n-- Lap Phieu Bao Tri Moi --\n";
                    auto pbt = std::make_shared<PhieuBaoTri>();

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Ma Phieu Bao Tri (4 so) [Enter de huy]: ", true);
                        if (ma.empty()) throw 0;

                        pbt->setId(Inputhelper::TuTaoMa(ma, "PBT"));
                        if (repoPBT.findById(pbt->getId())) throw Ngoailerangbuoc("Loi: Ma PBT nay da ton tai!");
                        });

                    // DA XOA MA TRAM TAI DAY

                    std::shared_ptr<ThietBiLapDat> tbTarget = nullptr;
                    thulai([&]() {
                        std::string maTB = Inputhelper::Docstring("Ma Thiet Bi (4 so) [Enter de huy]: ", true);
                        if (maTB.empty()) throw 0;

                        maTB = Inputhelper::TuTaoMa(maTB, "TB");
                        tbTarget = repoTB.findById(maTB);
                        if (!tbTarget) throw Ngoailerangbuoc("Tu choi: Ma Thiet Bi nay khong ton tai!");
                        pbt->setMaTB(tbTarget->getId());
                        });

                    std::string tuDongMaNCC = "";
                    auto loaiTb = repoLoai.findById(tbTarget->getMaLoai());
                    if (loaiTb) {
                        tuDongMaNCC = loaiTb->getMaNCC();
                    }

                    if (!tuDongMaNCC.empty()) {
                        pbt->setMaNCC(tuDongMaNCC);
                        std::cout << "   => [Tu dong]: Da xac dinh Nha Cung Cap cho thiet bi nay la: " << tuDongMaNCC << "\n";
                    }
                    else {
                        thulai([&]() {
                            std::string maNCC = Inputhelper::Docstring("Ma NCC (4 so) [Enter de huy]: ", true);
                            if (maNCC.empty()) throw 0;

                            maNCC = Inputhelper::TuTaoMa(maNCC, "NCC");
                            if (!repoNCC.findById(maNCC)) throw Ngoailerangbuoc("Tu choi: Ma NCC khong ton tai!");
                            pbt->setMaNCC(maNCC);
                            });
                    }

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

                    repoPBT.add(pbt);
                    std::cout << "[+] Them phieu bao tri thanh cong!\n";
                }
                else if (chon == 3) {
                    if (repoPBT.getAll().empty()) {
                        std::cout << "[-] Chua co phieu bao tri nao de cap nhat!\n";
                        continue;
                    }

                    std::cout << "\n-- Cap Nhat Phieu Bao Tri (Nhan Enter de giu nguyen) --\n";
                    std::string rawId = Inputhelper::Docstring("Ma PBT can sua (4 so) [Enter de huy]: ", true);
                    if (rawId.empty()) throw 0;

                    std::shared_ptr<PhieuBaoTri> pbt = repoPBT.findById(Inputhelper::TuTaoMa(rawId, "PBT"));
                    if (!pbt) throw Ngoailekotimthay("Khong tim thay Phieu Bao Tri nay!");

                    PhieuBaoTri banSao = *pbt;

                    // DA XOA MA TRAM TAI DAY

                    thulai([&]() {
                        std::string mTB = Inputhelper::Docstring("Ma Thiet Bi moi (" + banSao.getMaTB() + "): ", true);
                        if (!mTB.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mTB, "TB");
                            auto tbTarget = repoTB.findById(maChuan);
                            if (!tbTarget) throw Ngoailerangbuoc("Ma Thiet Bi khong ton tai!");
                            banSao.setMaTB(maChuan);

                            auto loaiTb = repoLoai.findById(tbTarget->getMaLoai());
                            if (loaiTb && !loaiTb->getMaNCC().empty()) {
                                banSao.setMaNCC(loaiTb->getMaNCC());
                                std::cout << "   => [Tu dong]: Da cap nhat lai Ma NCC la: " << loaiTb->getMaNCC() << "\n";
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
                    std::cout << "[+] Cap nhat phieu bao tri thanh cong!\n";
                }
                else if (chon == 4) {
                    if (repoPBT.getAll().empty()) {
                        std::cout << "[-] Chua co phieu bao tri nao de xoa!\n";
                        continue;
                    }

                    std::string rawId = Inputhelper::Docstring("\nMa PBT can xoa (4 so) [Enter de huy]: ", true);
                    if (rawId.empty()) throw 0;

                    std::string maChuan = Inputhelper::TuTaoMa(rawId, "PBT");
                    std::shared_ptr<PhieuBaoTri> pbt = repoPBT.findById(maChuan);
                    if (!pbt) throw Ngoailekotimthay("Khong tim thay Phieu Bao Tri nay!");

                    if (Inputhelper::Docstring("Chac chan xoa? (y/n): ") == "y" || Inputhelper::Docstring("Chac chan xoa? (y/n): ") == "Y") {
                        repoPBT.remove(maChuan);
                        std::cout << "[+] Da xoa thanh cong phieu bao tri!\n";
                    }
                    else {
                        std::cout << "[*] Da huy thao tac xoa.\n";
                    }
                }
            }
            catch (int) {
                std::cout << "\n[*] DA HUY THAO TAC VA QUAY LAI MENU CHINH!\n";
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
        }
    }
};
