#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Repository.h"
#include "ThietBiLapDat.h"
#include "LoaiThietBi.h" // Phai include de lay ham findById kiem tra
#include "TramBTS.h"     // Phai include de lay ham findById kiem tra
#include "InputHelper.h"

class MenuThietBiLapDat {
public:
    // HÀM QUAN TRỌNG: Phai truyen vao 3 kho chua (Repository) cung luc de doi chieu Khóa Ngoại
    static void run(Repository<ThietBiLapDat>& repoTB, Repository<LoaiThietBi>& repoLoai, Repository<TramBTS>& repoTram) {
        while (true) {
            std::cout << "\n--- QUAN LY THIET BI LAP DAT TAI TRAM ---\n";
            std::cout << "1. Xem danh sach\n2. Lap dat thiet bi moi\n3. Cap nhat trang thai\n4. Go bo thiet bi (Xoa)\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-4): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    repoTB.loadfromfile();
                    ThietBiLapDat::displayHeader();
                    for (const auto& tb : repoTB.getAll()) tb->display();
                }
                else if (chon == 2) {
                    std::cout << "\n-- Lap Dat Thiet Bi --\n";
                    std::string maTB = Inputhelper::Docstring("Ma Thiet Bi (4 so, VD Seri: 0001): ");
                    std::string ten = Inputhelper::Docstring("Ten the hien (VD: RRU Sector 1): ");

                    // ==========================================
                    // KIỂM TRA KHÓA NGOẠI TẦNG 1: MÃ LOẠI THIẾT BỊ
                    // ==========================================
                    std::string maLoai = Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma Loai (4 so): "), "LT");
                    // Goi ham findById cua kho Loai Thiet Bi. Neu == nullptr la khong tim thay
                    if (!repoLoai.findById(maLoai)) {
                        // Nem ra ngoai le rang buoc (se bi khoi catch tom duoc va quay lai menu, khong cho them thiet bi)
                        throw Ngoailerangbuoc("Tu choi: Ma Loai Thiet Bi khong ton tai trong danh muc!");
                    }

                    // ==========================================
                    // KIỂM TRA KHÓA NGOẠI TẦNG 2: MÃ TRẠM BTS
                    // ==========================================
                    std::string maTram = Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma Tram BTS (4 so): "), "BTS");
                    if (!repoTram.findById(maTram)) {
                        throw Ngoailerangbuoc("Tu choi: Ma Tram BTS khong ton tai tren he thong!");
                    }

                    // Vuot qua duoc 2 chot chan tren thi moi hoi tiep 2 cot nay
                    std::string ngay = Inputhelper::Docstring("Ngay lap dat (dd/mm/yyyy): ");
                    std::string tt = Inputhelper::Docstring("Trang thai (Hoat dong/Bao tri/Loi): ");

                    // Ghi nhan du lieu vao he thong
                    repoTB.add(std::make_shared<ThietBiLapDat>(maTB, ten, maLoai, maTram, ngay, tt));
                    std::cout << "[+] Da ghi nhan thiet bi len he thong trạm!\n";
                }
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat Trang Thai --\n";
                    auto tb = repoTB.findById(Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma TB can cap nhat: "), "TB"));
                    if (!tb) throw Loixacthuc("Khong tim thay Thiet bi nay!");

                    std::string ten = Inputhelper::Docstring("Ten the hien moi: ", true); if (!ten.empty()) tb->setTenThietBi(ten);

                    // Khi Cập nhật (Sửa), cung van phai doi chieu Khóa Ngoại
                    std::string mLoai = Inputhelper::Docstring("Ma Loai moi (4 so): ", true);
                    if (!mLoai.empty()) {
                        mLoai = Inputhelper::TuTaoMa(mLoai, "LT");
                        // Check xem ma moi nay co trong danh muc ko
                        if (!repoLoai.findById(mLoai)) throw Ngoailerangbuoc("Ma Loai moi khong ton tai!");
                        tb->setMaLoai(mLoai); // An toan roi moi Set
                    }

                    std::string mTram = Inputhelper::Docstring("Ma Tram moi (4 so): ", true);
                    if (!mTram.empty()) {
                        mTram = Inputhelper::TuTaoMa(mTram, "BTS");
                        if (!repoTram.findById(mTram)) throw Ngoailerangbuoc("Ma Tram moi khong ton tai!");
                        tb->setMaTram(mTram);
                    }

                    std::string ngay = Inputhelper::Docstring("Ngay lap dat moi (dd/mm/yyyy): ", true); if (!ngay.empty()) tb->setNgayLapDat(ngay);
                    std::string tt = Inputhelper::Docstring("Trang thai moi: ", true); if (!tt.empty()) tb->setTrangThai(tt);

                    repoTB.update(); // Ghi xuong file txt
                    std::cout << "[+] Cap nhat hoan tat!\n";
                }
                else if (chon == 4) {
                    std::string ma = Inputhelper::TuTaoMa(Inputhelper::Docstring("\nMa Thiet Bi can go bo: "), "TB");
                    if (!repoTB.findById(ma)) throw Loixacthuc("Khong tim thay Thiet bi nay!");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan go bo thiet bi khoi he thong? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoTB.remove(ma);
                        std::cout << "[+] Da go bo!\n";
                    }
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi: " << e.getMessage() << "\n"; }
            catch (...) { std::cout << "[-] Loi: Dinh dang khong hop le!\n"; }
        }
    }
};
