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
#include "InputHelper.h" 
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
                        std::string ma = Inputhelper::Docstring("Ma Phieu Bao Tri (4 so): ");
                        pbt->setId(Inputhelper::TuTaoMa(ma, "PBT"));
                        if (repoPBT.findById(pbt->getId())) throw Ngoailerangbuoc("Loi: Ma PBT nay da ton tai!");
                        });

                    thulai([&]() {
                        std::string maTram = Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma Tram BTS (4 so): "), "BTS");
                        if (!repoTram.findById(maTram)) throw Ngoailerangbuoc("Tu choi: Ma Tram BTS khong ton tai!");
                        pbt->setMaTram(maTram);
                        });

                    // TỰ ĐỘNG ĐIỀN MÃ NCC KHI NHẬP MÃ THIẾT BỊ
                    std::shared_ptr<ThietBiLapDat> tbTarget = nullptr;
                    thulai([&]() {
                        std::string maTB = Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma Thiet Bi (4 so): "), "TB");
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
                            std::string maNCC = Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma NCC (4 so): "), "NCC");
                            if (!repoNCC.findById(maNCC)) throw Ngoailerangbuoc("Tu choi: Ma NCC khong ton tai!");
                            pbt->setMaNCC(maNCC);
                            });
                    }

                    thulai([&]() { pbt->setNgayBaoTri(Inputhelper::Docstring("Ngay bao tri (dd/mm/yyyy): ")); });
                    thulai([&]() { pbt->setNoiDung(Inputhelper::Docstring("Noi dung bao tri: ")); });
                    thulai([&]() { pbt->setTrangThai(Inputhelper::Docstring("Trang thai (VD: Hoan tat): ")); });
                    thulai([&]() { pbt->setChiPhi(Inputhelper::Docdouble("Chi phi ($): ")); });

                    repoPBT.add(pbt);
                    std::cout << "[+] Them phieu bao tri thanh cong!\n";
                }
                else if (chon == 3) {
                    if (repoPBT.getAll().empty()) {
                        std::cout << "[-] Chua co phieu bao tri nao de cap nhat!\n";
                        continue;
                    }

                    std::cout << "\n-- Cap Nhat Phieu Bao Tri --\n";
                    std::shared_ptr<PhieuBaoTri> pbt = nullptr;

                    while (true) {
                        try {
                            std::string rawId = Inputhelper::Docstring("Ma PBT can sua (4 so) hoac '0' de huy: ");
                            if (rawId == "0") break;

                            pbt = repoPBT.findById(Inputhelper::TuTaoMa(rawId, "PBT"));
                            if (!pbt) throw Ngoailekotimthay("Khong tim thay Phieu Bao Tri nay!");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "   -> " << e.getMessage() << "\n"; }
                    }

                    if (!pbt) continue;

                    thulai([&]() {
                        std::string mTram = Inputhelper::Docstring("Ma Tram moi (Enter de bo qua): ", true);
                        if (!mTram.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mTram, "BTS");
                            if (!repoTram.findById(maChuan)) throw Ngoailerangbuoc("Ma Tram khong ton tai!");
                            pbt->setMaTram(maChuan);
                        }
                        });

                    thulai([&]() {
                        std::string mTB = Inputhelper::Docstring("Ma Thiet Bi moi (Enter de bo qua): ", true);
                        if (!mTB.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(mTB, "TB");
                            auto tbTarget = repoTB.findById(maChuan);
                            if (!tbTarget) throw Ngoailerangbuoc("Ma Thiet Bi khong ton tai!");
                            pbt->setMaTB(maChuan);

                            auto loaiTb = repoLoai.findById(tbTarget->getMaLoai());
                            if (loaiTb && !loaiTb->getMaNCC().empty()) {
                                pbt->setMaNCC(loaiTb->getMaNCC());
                                std::cout << "   => [Tu dong]: Da cap nhat lai Ma NCC la: " << loaiTb->getMaNCC() << "\n";
                            }
                        }
                        });

                    thulai([&]() { std::string ngay = Inputhelper::Docstring("Ngay bao tri moi: ", true); if (!ngay.empty()) pbt->setNgayBaoTri(ngay); });
                    thulai([&]() { std::string nd = Inputhelper::Docstring("Noi dung moi: ", true); if (!nd.empty()) pbt->setNoiDung(nd); });
                    thulai([&]() { std::string tt = Inputhelper::Docstring("Trang thai moi: ", true); if (!tt.empty()) pbt->setTrangThai(tt); });
                    thulai([&]() {
                        double cpMoi = pbt->getChiPhi();
                        if (Inputhelper::DocdoubleCapNhat("Chi phi moi ($): ", cpMoi)) pbt->setChiPhi(cpMoi);
                        });

                    repoPBT.update();
                    std::cout << "[+] Cap nhat phieu bao tri thanh cong!\n";
                }
                else if (chon == 4) {
                    if (repoPBT.getAll().empty()) {
                        std::cout << "[-] Chua co phieu bao tri nao de xoa!\n";
                        continue;
                    }

                    std::shared_ptr<PhieuBaoTri> pbt = nullptr;
                    std::string maChuan;

                    while (true) {
                        try {
                            std::string rawId = Inputhelper::Docstring("\nMa PBT can xoa (4 so) hoac '0' de huy: ");
                            if (rawId == "0") break;

                            maChuan = Inputhelper::TuTaoMa(rawId, "PBT");
                            pbt = repoPBT.findById(maChuan);
                            if (!pbt) throw Ngoailekotimthay("Khong tim thay Phieu Bao Tri nay!");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "   -> " << e.getMessage() << "\n"; }
                    }

                    if (!pbt) continue;

                    if (Inputhelper::Docstring("Chac chan xoa? (y/n): ") == "y") {
                        repoPBT.remove(maChuan);
                        std::cout << "[+] Da xoa thanh cong phieu bao tri!\n";
                    }
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
        }
    }
};
