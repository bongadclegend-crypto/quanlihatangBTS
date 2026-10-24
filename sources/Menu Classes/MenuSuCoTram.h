// Kieu Binh Nhan B24DCVT285
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
    // Chay menu quan ly su co tram, truyen vao cac repo de thao tac du lieu
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
                    const auto& ds = repoSC.getAll(); // Lay toan bo danh sach su co
                    // Kiem tra neu danh sach trong
                    if (ds.empty()) {
                        std::cout << "[-] Danh sach Su co dang trong!\n";
                    }
                    else {
                        // Neu co du lieu thi moi in tieu de va danh sach
                        SuCoTram::displayHeader();
                        for (const auto& sc : ds) {
                            sc->display();
                        }
                    }
                }
                else if (chon == 2) {
                    std::cout << "\n-- Ghi Nhan Su Co --\n";
                    auto sucomoi = std::make_shared<SuCoTram>();
                    bool huy = false;   // kiem tra xem nguoi dung co muon huy thao tac hay khong
                    // Nhap ma su co, kiem tra xem ma da ton tai hay chua
                    while (true) {
                        try {
                            std::string masc = Inputhelper::Docstring("Nhap Ma SC (chi can 4 so, Hoac Enter de Huy): ", true);
                            if (masc == "") { huy = true; break; }

                            std::string id = Inputhelper::TuTaoMa(masc, "SC");
                            if (repoSC.findById(id) != nullptr) throw Loixacthuc("Loi: Ma Su co nay da ton tai!");
                            sucomoi->setId(id);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (huy) { std::cout << "[!] Da huy.\n"; continue; } // neu nguoi dung nhap enter thi huy thao tac them moi  
                    // Nhap ma tram bi loi, kiem tra xem tram co ton tai hay khong
                    while (true) {
                        try {
                            std::string maBTS = Inputhelper::Docstring("Nhap Ma Tram bi loi (4 so, Enter de Huy): ", true);
                            if (maBTS == "") { huy = true; break; }
                            maBTS = Inputhelper::TuTaoMa(maBTS, "BTS");
                            if (repoTram.findById(maBTS) == nullptr) throw Loixacthuc("Loi: Tram nay khong co that!");
                            sucomoi->setMaTram(maBTS);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (huy) { std::cout << "[!] Da huy.\n"; continue; }
                    // Nhap ngay xay ra su co, kiem tra dinh dang ngay thang nam
                    while (true) {
                        try {
                            std::string ngay = Inputhelper::Docstring("Ngay (dd/mm/yyyy, Enter de Huy): ", true);
                            if (ngay == "") { huy = true; break; }
                            sucomoi->setNgayXayRa(ngay);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (huy) { std::cout << "[!] Da huy.\n"; continue; }
                    // Nhap mo ta loi, kiem tra xem co rong hay khong
                    while (true) {
                        try {
                            std::string moTa = Inputhelper::Docstring("Mo ta loi (Enter de Huy): ", true);
                            if (moTa == "") { huy = true; break; }
                            sucomoi->setMoTaLoi(moTa);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (huy) { std::cout << "[!] Da huy.\n"; continue; }
                    // Nhap muc do loi, chi duoc nhap 1, 2 hoac 3
                    while (true) {
                        try {
                            std::string mucDo = Inputhelper::Docstring("Muc do (1 / 2 / 3, Enter de Huy): ", true);
                            if (mucDo == "") { huy = true; break; }
                            sucomoi->setMucDo(mucDo);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (huy) { std::cout << "[!] Da huy.\n"; continue; }
                    // Nhap ma ky thuat vien xu ly, kiem tra xem KTV co ton tai hay khong
                    while (true) {
                        try {
                            std::string maKTV = Inputhelper::Docstring("Nhap Ma KTV xu ly (4 so, Enter de Huy): ", true);
                            if (maKTV == "") { huy = true; break; }
							// Kiem tra xem KTV co ton tai hay khong
                            maKTV = Inputhelper::TuTaoMa(maKTV, "KTV");
                            auto ktv = repoKTV.findById(maKTV);
                            if (ktv == nullptr) throw Loixacthuc("Loi: KTV nay khong co that!");
							// Kiem tra xem KTV co dung chuyen mon "Sua chua" hay khong
                            if (ktv->getDoiNguChuyenMon() != "Sua chua" && ktv->getDoiNguChuyenMon() != "Sua Chua") {
                                throw Loixacthuc("Loi: KTV nay khong dung doi ngu chuyen mon (Yeu cau doi ngu chuyen mon: Sua chua)!");
                            }
							sucomoi->setMaKTVXuLy(maKTV); // gan ma KTV xu ly cho su co
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (huy) { std::cout << "[!] Da huy thao tac them.\n"; continue; }
                    // Nhap trang thai su co, chi duoc nhap "da sua" hoac "chua sua"
                    while (true) {
                        try {
                            std::string trangThai = Inputhelper::Docstring("Trang thai (Da sua / Chua sua, Enter de Huy): ", true);
                            if (trangThai == "") { huy = true; break; }
                            sucomoi->setTrangThai(trangThai);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (huy) { std::cout << "[!] Da huy.\n"; continue; }

                    repoSC.add(sucomoi);    // them su co moi vao repoSC va luu vao file
                    std::cout << "[+] Da them xong!\n";
                }
                // Cap nhat thong tin su co tram
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat Thong Tin --\n";
                    std::string macansua;
                    // Nhap ma su co can sua, kiem tra xem su co co ton tai hay khong
                    while (true) {
                        try {
                            macansua = Inputhelper::Docstring("Nhap Ma SC can sua (Hoac nhan Enter de Huy): ", true);
                            if (macansua == "") break;
                            macansua = Inputhelper::TuTaoMa(macansua, "SC");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (macansua == "") { std::cout << "[!] Da huy.\n"; continue; }

                    auto sc = repoSC.findById(macansua); // tim su co theo ma
                    if (sc == nullptr) throw Loixacthuc("Khong tim thay SC nay!");

                    std::cout << "\n-- Nhap thong tin moi (Nhan Enter de bo qua truong khong sua) --\n";
                    // Nhap ma tram moi, kiem tra xem tram co ton tai hay khong
                    while (true) {
                        try {
                            std::string maBTS = Inputhelper::Docstring("Ma Tram moi (4 so): ", true);
                            if (maBTS != "") {
                                maBTS = Inputhelper::TuTaoMa(maBTS, "BTS");
                                if (repoTram.findById(maBTS) == nullptr) throw Loixacthuc("Tram khong co that!");
                                sc->setMaTram(maBTS);
                            }
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    // Nhap ngay xay ra moi, kiem tra dinh dang ngay thang nam
                    while (true) {
                        try {
                            std::string ngay = Inputhelper::Docstring("Ngay moi (dd/mm/yyyy): ", true);
                            if (ngay != "") sc->setNgayXayRa(ngay);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    // Nhap mo ta moi, kiem tra xem co rong hay khong
                    while (true) {
                        try {
                            std::string mota = Inputhelper::Docstring("Mo ta moi: ", true);
                            if (mota != "") sc->setMoTaLoi(mota);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    // Nhap muc do moi, chi duoc nhap 1, 2 hoac 3
                    while (true) {
                        try {
                            std::string mucdo = Inputhelper::Docstring("Muc do moi (1 / 2 / 3): ", true);
                            if (mucdo != "") sc->setMucDo(mucdo);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    // Nhap ma ky thuat vien moi, kiem tra xem KTV co ton tai hay khong
                    while (true) {
                        try {
                            std::string maKTV = Inputhelper::Docstring("KTV moi (4 so): ", true);
                            if (maKTV != "") {
                                maKTV = Inputhelper::TuTaoMa(maKTV, "KTV");
								// Tim KTV trong kho
                                auto ktv = repoKTV.findById(maKTV);
                                if (ktv == nullptr) throw Loixacthuc("Loi: KTV khong co that!");
                                // Kiem tra chuyen mon co phai la Sua chua khong
                                if (ktv->getDoiNguChuyenMon() != "Sua chua" && ktv->getDoiNguChuyenMon() != "Sua Chua") {
                                    throw Loixacthuc("Loi: KTV nay khong dung doi ngu chuyen mon (Yeu cau doi ngu chuyen mon: Sua chua)!");
                                }
                                sc->setMaKTVXuLy(maKTV);
                            }
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    // Nhap trang thai moi, chi duoc nhap "da sua" hoac "chua sua"
                    while (true) {
                        try {
                            std::string tt = Inputhelper::Docstring("Trang thai moi (Da sua / Chua sua): ", true);
                            if (tt != "") sc->setTrangThai(tt);
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    repoSC.update(); // luu lai doi tuong sau khi cap nhat vao file
                    std::cout << "[+] Da sua xong!\n";
                }
                // Xoa su co tram
                else if (chon == 4) {
                    std::string macanxoa;
                    // Nhap ma su co can xoa, kiem tra xem su co co ton tai hay khong
                    while (true) {
                        try {
                            macanxoa = Inputhelper::Docstring("\nNhap Ma SC can xoa (Hoac Enter de Huy): ", true);
                            if (macanxoa == "") break;
                            macanxoa = Inputhelper::TuTaoMa(macanxoa, "SC");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "[-] " << e.getMessage() << "\n"; }
                    }
                    if (macanxoa == "") { std::cout << "[!] Da huy.\n"; continue; }

                    if (repoSC.findById(macanxoa) == nullptr) throw Loixacthuc("Khong tim thay SC nay!");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoSC.remove(macanxoa); // xoa su co tram khoi repoSC va luu vao file
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
