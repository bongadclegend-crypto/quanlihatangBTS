// Nguoi thuc hien: Nguyen Anh Tu - B24DCVT368
#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Repository.h"
#include "ThietBiLapDat.h"
#include "LoaiThietBi.h" 
#include "TramBTS.h"     
#include "InputHelper.h"

using namespace std;

class MenuThietBiLapDat {
public:
    static void run(Repository<ThietBiLapDat>& repoTB, Repository<LoaiThietBi>& repoLoai, Repository<TramBTS>& repoTram) {
        while (true) {
            cout << "\n--- QUAN LY THIET BI LAP DAT TAI TRAM ---\n";
            cout << "1. Xem danh sach\n2. Lap dat thiet bi moi\n3. Cap nhat trang thai\n4. Go bo thiet bi (Xoa)\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-4): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    cout << "\n";
                    repoTB.loadFromFile();
                    ThietBiLapDat::displayHeader();
                    for (const auto& tb : repoTB.getAll()) tb->display();
                }
                else if (chon == 2) {
                    cout << "\n-- Lap Dat Thiet Bi --\n";
                    string maTB = Inputhelper::Docstring("Ma Thiet Bi (4 so): ");
                    string ten = Inputhelper::Docstring("Ten the hien : ");

                    string maLoai = Inputhelper::tuTaoMa(Inputhelper::Docstring("Ma Loai (4 so): "), "LT");
                    if (!repoLoai.findById(maLoai)) {
                        throw Ngoailerangbuoc("Tu choi: Ma Loai Thiet Bi khong ton tai trong danh muc!");
                    }

                    string maTram = Inputhelper::tuTaoMa(Inputhelper::Docstring("Ma Tram BTS (4 so): "), "BTS");
                    if (!repoTram.findById(maTram)) {
                        throw Ngoailerangbuoc("Tu choi: Ma Tram BTS khong ton tai tren he thong!");
                    }

                    string ngay = Inputhelper::Docstring("Ngay lap dat (dd/mm/yyyy): ");
                    string tt = Inputhelper::Docstring("Trang thai (Hoat dong/Bao tri/Loi): ");

                    repoTB.add(make_shared<ThietBiLapDat>(maTB, ten, maLoai, maTram, ngay, tt));
                    cout << "[+] Da ghi nhan thiet bi len he thong trạm!\n";
                }
                else if (chon == 3) {
                    cout << "\n-- Cap Nhat Trang Thai --\n";
                    auto tb = repoTB.findById(Inputhelper::tuTaoMa(Inputhelper::Docstring("Ma TB can cap nhat: "), "TB"));
                    if (!tb) throw Loixacthuc("Khong tim thay Thiet bi nay!");

                    string ten = Inputhelper::Docstring("Ten the hien moi: ", true); if (!ten.empty()) tb->setTenThietBi(ten);

                    string mLoai = Inputhelper::Docstring("Ma Loai (4 so): ", true);
                    if (!mLoai.empty()) {
                        mLoai = Inputhelper::tuTaoMa(mLoai, "LT");
                        if (!repoLoai.findById(mLoai)) throw Ngoailerangbuoc("Ma Loai khong ton tai!");
                        tb->setMaLoai(mLoai);
                    }

                    string mTram = Inputhelper::Docstring("Ma Tram moi (4 so): ", true);
                    if (!mTram.empty()) {
                        mTram = Inputhelper::tuTaoMa(mTram, "BTS");
                        if (!repoTram.findById(mTram)) throw Ngoailerangbuoc("Ma Tram moi khong ton tai!");
                        tb->setMaTram(mTram);
                    }

                    string ngay = Inputhelper::Docstring("Ngay lap dat moi (dd/mm/yyyy): ", true); if (!ngay.empty()) tb->setNgayLapDat(ngay);
                    string tt = Inputhelper::Docstring("Trang thai moi: ", true); if (!tt.empty()) tb->setTrangThai(tt);

                    repoTB.update();
                    cout << "[+] Cap nhat hoan tat!\n";
                }
                else if (chon == 4) {
                    string ma = Inputhelper::tuTaoMa(Inputhelper::Docstring("\nMa Thiet Bi can go bo: "), "TB");
                    if (!repoTB.findById(ma)) throw Loixacthuc("Khong tim thay Thiet bi nay!");

                    string xacNhan = Inputhelper::Docstring("Chac chan go bo thiet bi khoi he thong? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoTB.remove(ma);
                        cout << "[+] Da go bo!\n";
                    }
                }
            }
            catch (const Baseexception& e) { cout << "[-] Loi: " << e.getMessage() << "\n"; }
            catch (...) { cout << "[-] Loi: Dinh dang khong hop le!\n"; }
        }
    }
};