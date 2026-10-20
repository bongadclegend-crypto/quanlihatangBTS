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
                // 1. Xem
                if (chon == 1) {
                    std::cout << "\n";
                    SuCoTram::displayHeader();
                    for (const auto& sc : repoSC.getAll()) {
                        sc->display();
                    }
                }

                // 2. ThÃªm
                else if (chon == 2) {
                    std::cout << "\n-- Ghi Nhan Su Co --\n";
                    std::string maSC = Inputhelper::Docstring("Nhap Ma SC (4 so): ");

                    std::string maBTS = Inputhelper::Docstring("Nhap Ma Tram (4 so): ");
                    maBTS = Inputhelper::TuTaoMa(maBTS, "BTS");
                    if (repoTram.findById(maBTS) == nullptr) throw Loixacthuc("Loi: Tram nay khong co that!");

                    std::string ngay = Inputhelper::Docstring("Ngay (dd/mm/yyyy): ");
                    std::string moTa = Inputhelper::Docstring("Mo ta loi: ");


                    std::string mucDo = Inputhelper::Docstring("Muc do (1 / 2 / 3): ");

                    std::string maKTV = Inputhelper::Docstring("Nhap Ma KTV (4 so): ");
                    maKTV = Inputhelper::TuTaoMa(maKTV, "KTV");
                    if (repoKTV.findById(maKTV) == nullptr) throw Loixacthuc("Loi: KTV nay khong co that!");


                    std::string trangThai = Inputhelper::Docstring("Trang thai (da sua / chua sua): ");

                    auto suCoMoi = std::make_shared<SuCoTram>(maSC, maBTS, ngay, moTa, mucDo, maKTV, trangThai);
                    repoSC.add(suCoMoi);
                    std::cout << "[+] Da them xong!\n";
                }

                // 3. Sá»­a
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat Thong Tin --\n";
                    std::string maCanSua;
                    while (true) {
                        try {
                            maCanSua = Inputhelper::Docstring("Nhap Ma SC can sua (Hoac nhan Enter de Huy): ", true);
                            if (maCanSua == "") { std::cout << "[!] Da huy.\n"; break; }
                            maCanSua = Inputhelper::TuTaoMa(maCanSua, "SC");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (maCanSua == "") continue;

                    auto sc = repoSC.findById(maCanSua);
                    if (sc == nullptr) throw Loixacthuc("Khong tim thay SC nay!");

                    std::cout << "\n-- Nhap thong tin moi (Nhan Enter de bo qua) --\n";

                    std::string maBTS = Inputhelper::Docstring("Ma Tram moi (4 so): ", true);
                    if (maBTS != "") {
                        maBTS = Inputhelper::TuTaoMa(maBTS, "BTS");
                        if (repoTram.findById(maBTS) == nullptr) throw Loixacthuc("Tram khong co that!");
                        sc->setMaTram(maBTS);
                    }

                    std::string ngay = Inputhelper::Docstring("Ngay moi (dd/mm/yyyy): ", true);
                    if (ngay != "") sc->setNgayXayRa(ngay);

                    std::string moTa = Inputhelper::Docstring("Mo ta moi: ", true);
                    if (moTa != "") sc->setMoTaLoi(moTa);


                    std::string mucDo = Inputhelper::Docstring("Muc do moi (1 / 2 / 3): ", true);
                    if (mucDo != "") sc->setMucDo(mucDo);

                    std::string maKTV = Inputhelper::Docstring("KTV moi (4 so): ", true);
                    if (maKTV != "") {
                        maKTV = Inputhelper::TuTaoMa(maKTV, "KTV");
                        if (repoKTV.findById(maKTV) == nullptr) throw Loixacthuc("KTV khong co that!");
                        sc->setMaKTV(maKTV);
                    }
                    std::string tt = Inputhelper::Docstring("Trang thai moi (da sua / chua sua): ", true);
                    if (tt != "") sc->setTrangThai(tt);

                    repoSC.update();
                    std::cout << "[+] Da sua xong!\n";
                }

                // 4. XÃ³a
                else if (chon == 4) {
                    std::string ma = Inputhelper::Docstring("Nhap Ma SC can xoa: ");
                    ma = Inputhelper::TuTaoMa(ma, "SC");

                    if (repoSC.findById(ma) == nullptr) throw Loixacthuc("Khong tim thay SC nay!");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoSC.remove(ma);
                        std::cout << "[+] Da xoa xong!\n";
                    }
                }
            }
            catch (const Baseexception& e) {
                std::cout << "[-] Loi: " << e.getMessage() << "\n";
            }
        }
    }
};