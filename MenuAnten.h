// Tac gia: Nguyen Cong Vinh
// Menu console cho use case Quan ly Anten & Cau hinh phat
#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cctype>
#include "Exceptions.h"
#include "Repository.h"
#include "InputHelper.h"
#include "Anten.h"
#include "TramBTS.h"

class MenuAnten {
private:
    template <class F>
    static void thuLai(F buoc) {
        while (true) {
            try { buoc(); return; }
            catch (const Baseexception& e) {
                std::cout << "[!] " << e.getMessage() << " -> Moi nhap lai.\n";
            }
        }
    }

    static std::string thuongHoa(std::string s) {
        for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }

    static void inBang(const std::vector<std::shared_ptr<Anten>>& ds) {
        Anten::displayHeader();
        for (const auto& a : ds) a->display();
        // Xóa dòng Anten::displayFooter(); đi
    }

    // Kiem tra Ma Tram sau khi da ep chuan 4 so ("1001" -> "BTS1001")
    static void kiemTraTramTonTai(const Repository<TramBTS>& repoTram, const std::string& maTramChuan) {
        if (!repoTram.findById(maTramChuan))
            throw Ngoailerangbuoc("Loi: Ma Tram [" + maTramChuan + "] khong ton tai tren he thong!");
    }

public:
    static void run(Repository<Anten>& repoAnten, Repository<TramBTS>& repoTramBTS) {
        while (true) {
            std::cout << "\n============= QUAN LY ANTEN & CAU HINH PHAT =============\n";
            std::cout << "1. Them anten moi\n";
            std::cout << "2. Xem danh sach anten\n";
            std::cout << "3. Tim kiem anten\n";
            std::cout << "4. Cap nhat thong so anten\n";
            std::cout << "5. Xoa anten\n";
            std::cout << "0. Quay lai menu chinh\n";
            std::cout << "=========================================================\n";

            int choice = Inputhelper::Docint(">> Nhap lua chon: ");
            if (choice == 0) break;

            try {
                if (choice == 1) {                        // THEM MOI
                    auto anten = std::make_shared<Anten>();
                    thuLai([&]() {
                        anten->setMa(Inputhelper::Docstring("Nhap Ma Anten (4 so, VD: 0001): "));
                        if (repoAnten.findById(anten->getId()))
                            throw Ngoailerangbuoc("Loi: Ma anten [" + anten->getId() + "] da ton tai!");
                        });
                    thuLai([&]() {
                        std::string rawTram = Inputhelper::Docstring("Nhap Ma Tram BTS lap dat (4 so, VD: 1001): ");
                        anten->setMaTram(rawTram);
                        kiemTraTramTonTai(repoTramBTS, anten->getMaTram());
                        });
                    thuLai([&]() {
                        anten->setLoaiAnten(Inputhelper::Docstring("Nhap Loai anten (VD: Sector/Omni/Microwave): "));
                        });
                    thuLai([&]() {
                        anten->setChieuCao(Anten::phanTichSoThuc(
                            Inputhelper::Docstring("Nhap Do cao cot [5.0 - 150.0 m]: ")));
                        });
                    thuLai([&]() {
                        anten->setGocAzimuth(Anten::phanTichSoNguyen(
                            Inputhelper::Docstring("Nhap Goc Cua Anten [0 - 359 do]: ")));
                        });
                    repoAnten.add(anten);
                    std::cout << "[+] Them anten moi thanh cong (Ma: " << anten->getId() << ")!\n";
                }
                else if (choice == 2) {                   // XEM DANH SACH
                    const auto& ds = repoAnten.getAll();
                    if (ds.empty()) std::cout << "[-] Danh sach anten dang trong.\n";
                    else {
                        inBang(ds);
                        
                    }
                }
                else if (choice == 3) {                   // TIM KIEM
                    std::string tuKhoa = Inputhelper::Docstring("Nhap ma anten (VD: 0001), ma tram (VD: 1001) hoac loai anten: ");
                    std::string ma = Anten::chuanHoaMa(tuKhoa);
                    std::string loai = thuongHoa(tuKhoa);
                    std::vector<std::shared_ptr<Anten>> ketQua;
                    for (const auto& a : repoAnten.getAll()) {
                        if (a->getId() == ma || a->getId() == "AT" + ma ||
                            a->getMaTram() == ma || a->getMaTram() == "BTS" + ma ||
                            thuongHoa(a->getLoaiAnten()).find(loai) != std::string::npos) {
                            ketQua.push_back(a);
                        }
                    }
                    if (ketQua.empty()) std::cout << "[-] Khong tim thay anten phu hop.\n";
                    else inBang(ketQua);
                }
                else if (choice == 4) {                   // CAP NHAT
                    std::string id = Anten::taoMaAnten(Inputhelper::Docstring("Nhap Ma Anten can sua (4 so, VD: 0001): "));
                    auto at = repoAnten.findById(id);
                    if (!at) throw Ngoailekotimthay("Loi: Khong tim thay anten [" + id + "]!");
                    inBang({ at });

                    std::cout << "(*) Nhap gia tri moi (Enter de giu nguyen):\n";
                    Anten banSao = *at;

                    // 1. Cap nhat Ma Tram (Khoa ngoai) - Da sua loi State Leak
                    thuLai([&]() {
                        std::string s = Inputhelper::Docstring("Ma Tram moi (4 so, hien tai " + banSao.getMaTram() + "): ", true);
                        if (!s.empty()) {
                            std::string maTramChuan = Anten::taoMaTram(s);
                            kiemTraTramTonTai(repoTramBTS, maTramChuan); // Kiem tra truoc
                            banSao.setMaTram(maTramChuan);               // An toan roi moi gan
                        }
                        });

                    // 2. Cap nhat Loai Anten
                    thuLai([&]() {
                        std::string s = Inputhelper::Docstring("Loai moi (" + banSao.getLoaiAnten() + "): ", true);
                        if (!s.empty()) banSao.setLoaiAnten(s);
                        });

                    // 3. Cap nhat Do Cao
                    thuLai([&]() {
                        std::string s = Inputhelper::Docstring("Do cao moi (" + banSao.chieuCaoToString() + " m): ", true);
                        if (!s.empty()) {
                            // Ep kieu truoc, dung thi moi gan
                            double cao = Anten::phanTichSoThuc(s);
                            banSao.setChieuCao(cao);
                        }
                        });

                    // 4. Cap nhat Goc Azimuth
                    thuLai([&]() {
                        std::string s = Inputhelper::Docstring("Azimuth moi (" + std::to_string(banSao.getGocAzimuth()) + " do): ", true);
                        if (!s.empty()) {
                            // Ep kieu truoc, dung thi moi gan
                            int goc = Anten::phanTichSoNguyen(s);
                            banSao.setGocAzimuth(goc);
                        }
                        });

                    *at = banSao;
                    repoAnten.update();
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                else if (choice == 5) {                   // XOA
                    std::string id = Anten::taoMaAnten(Inputhelper::Docstring("Nhap Ma Anten can xoa (4 so, VD: 0001): "));
                    auto at = repoAnten.findById(id);
                    if (!at) throw Ngoailekotimthay("Loi: Khong tim thay anten [" + id + "]!");
                    inBang({ at });

                    std::string xn = Inputhelper::Docstring("Ban co chac chan muon xoa (y/n)? ");
                    if (xn == "y" || xn == "Y") {
                        repoAnten.remove(id);
                        std::cout << "[+] Da xoa anten thanh cong!\n";
                    }
                    else {
                        std::cout << "[*] Da huy thao tac xoa.\n";
                    }
                }
                else std::cout << "[-] Lua chon khong hop le!\n";
            }
            catch (const Baseexception& e) { std::cout << "[!] LOI NGHIEP VU: " << e.getMessage() << "\n"; }
            catch (const std::exception& e) { std::cout << "[!] LOI HE THONG: " << e.what() << "\n"; }
        }
    }
};