#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Repository.h"
#include "SuCoTram.h"
#include "TramBTS.h"
#include "KyThuatVien.h"
#include "InputHelper.h"

class MenuSuCoTram {
public:
    static void run(Repository<SuCoTram>& repoSC, Repository<TramBTS>& repoTram, Repository<KyThuatVien>& repoKTV) {
        while (true) {
            std::cout << "\n--- MENU SU CO TRAM ---\n";
            std::cout << "1. Xem danh sach\n";
            std::cout << "2. Them moi\n";
            std::cout << "3. Sua thong tin\n";
            std::cout << "4. Xoa\n";
            std::cout << "0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon so: ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    SuCoTram::displayHeader();
                    for (const auto& sc : repoSC.getAll()) {
                        sc->display();
                    }
                }
                else if (chon == 2) {
                    std::cout << "\n-- Ghi Nhan Su Co --\n";
                    auto suCoMoi = std::make_shared<SuCoTram>();
                    bool huy = false;
                    while (true) {
                        try {
                            std::string maSC = Inputhelper::Docstring("Nhap Ma SC (chi can 4 so, Hoac Enter de Huy): ", true);
                            if (maSC == "") { huy = true; break; }

                            std::string id = Inputhelper::tuTaoMa(maSC, "SC");
                            if (repoSC.findById(id) != nullptr) throw Loixacthuc("Loi: Ma Su co nay da ton tai!");
                            suCoMoi->setId(id);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (huy) { std::cout << "[!] Da huy thao tac them moi.\n"; continue; }
                    while (true) {
                        try {
                            std::string maBTS = Inputhelper::Docstring("Nhap Ma Tram bi loi (4 so): ");
                            maBTS = Inputhelper::tuTaoMa(maBTS, "BTS");
                            if (repoTram.findById(maBTS) == nullptr) throw Loixacthuc("Loi: Tram nay khong co that!");
                            suCoMoi->setMaTram(maBTS);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    while (true) {
                        try {
                            std::string ngay = Inputhelper::Docstring("Ngay (dd/mm/yyyy): ");
                            suCoMoi->setNgayXayRa(ngay);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    while (true) {
                        try {
                            std::string moTa = Inputhelper::Docstring("Mo ta loi: ");
                            suCoMoi->setMoTaLoi(moTa);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    while (true) {
                        try {
                            std::string mucDo = Inputhelper::Docstring("Muc do (1 / 2 / 3): ");
                            suCoMoi->setMucDo(mucDo);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    while (true) {
                        try {
                            std::string maKTV = Inputhelper::Docstring("Nhap Ma KTV xu ly (4 so): ");
                            maKTV = Inputhelper::tuTaoMa(maKTV, "KTV");
                            if (repoKTV.findById(maKTV) == nullptr) throw Loixacthuc("Loi: KTV nay khong co that!");
                            suCoMoi->setMaKTV(maKTV);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    while (true) {
                        try {
                            std::string trangThai = Inputhelper::Docstring("Trang thai (da sua / chua sua): ");
                            suCoMoi->setTrangThai(trangThai);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    repoSC.add(suCoMoi);
                    std::cout << "[+] Da them xong!\n";
                }
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat Thong Tin --\n";
                    std::string maCanSua;

                    while (true) {
                        try {
                            maCanSua = Inputhelper::Docstring("Nhap Ma SC can sua (Hoac nhan Enter de Huy): ", true);
                            if (maCanSua == "") break;
                            maCanSua = Inputhelper::tuTaoMa(maCanSua, "SC");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (maCanSua == "") { std::cout << "[!] Da huy.\n"; continue; }

                    auto sc = repoSC.findById(maCanSua);
                    if (sc == nullptr) throw Loixacthuc("Khong tim thay SC nay!");

                    std::cout << "\n-- Nhap thong tin moi (Nhan Enter de bo qua truong khong sua) --\n";

                    while (true) {
                        try {
                            std::string maBTS = Inputhelper::Docstring("Ma Tram moi (4 so): ", true);
                            if (maBTS != "") {
                                maBTS = Inputhelper::tuTaoMa(maBTS, "BTS");
                                if (repoTram.findById(maBTS) == nullptr) throw Loixacthuc("Tram khong co that!");
                                sc->setMaTram(maBTS);
                            }
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    while (true) {
                        try {
                            std::string ngay = Inputhelper::Docstring("Ngay moi (dd/mm/yyyy): ", true);
                            if (ngay != "") sc->setNgayXayRa(ngay);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    while (true) {
                        try {
                            std::string moTa = Inputhelper::Docstring("Mo ta moi: ", true);
                            if (moTa != "") sc->setMoTaLoi(moTa);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    while (true) {
                        try {
                            std::string mucDo = Inputhelper::Docstring("Muc do moi (1 / 2 / 3): ", true);
                            if (mucDo != "") sc->setMucDo(mucDo);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    while (true) {
                        try {
                            std::string maKTV = Inputhelper::Docstring("KTV moi (4 so): ", true);
                            if (maKTV != "") {
                                maKTV = Inputhelper::tuTaoMa(maKTV, "KTV");
                                if (repoKTV.findById(maKTV) == nullptr) throw Loixacthuc("KTV khong co that!");
                                sc->setMaKTV(maKTV);
                            }
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    while (true) {
                        try {
                            std::string tt = Inputhelper::Docstring("Trang thai moi (da sua / chua sua): ", true);
                            if (tt != "") sc->setTrangThai(tt);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }

                    repoSC.update();
                    std::cout << "[+] Da sua xong!\n";
                }
                else if (chon == 4) {
                    std::string maCanXoa;
                    while (true) {
                        try {
                            maCanXoa = Inputhelper::Docstring("\nNhap Ma SC can xoa (Hoac Enter de Huy): ", true);
                            if (maCanXoa == "") break;
                            maCanXoa = Inputhelper::tuTaoMa(maCanXoa, "SC");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (maCanXoa == "") { std::cout << "[!] Da huy.\n"; continue; }

                    if (repoSC.findById(maCanXoa) == nullptr) throw Loixacthuc("Khong tim thay SC nay!");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoSC.remove(maCanXoa);
                        std::cout << "[+] Da xoa xong!\n";
                    }
                }
            }
            catch (const Baseexception& e) {
                std::cout << "[-] " << e.getMessage() << "\n";
            }
        }
    }
};
