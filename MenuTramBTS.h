#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Repository.h"
#include "TramBTS.h"
#include "Inputhelper.h"
#include "Exceptions.h"

class MenuTramBTS {
private:
    // HÀM HỖ TRỢ BẮT LỖI: Nhập sai ở đâu lặp lại đúng chỗ đó
    template <class F>
    static void thuLai(F buoc) {
        while (true) {
            try {
                buoc();
                return; // Nếu không có lỗi thì thoát vòng lặp, đi tiếp
            }
            catch (const Baseexception& e) {
                std::cout << "[!] " << e.getMessage() << " -> Moi nhap lai.\n";
            }
            catch (const std::exception&) {
                std::cout << "[!] Gia tri khong hop le -> Moi nhap lai.\n";
            }
        }
    }

public:
    static void run(Repository<TramBTS>& repoTramBTS) {
        while (true) {
            std::cout << "\n--- QUAN LY TRAM BTS ---\n";
            std::cout << "1. Xem danh sach\n2. Them moi\n3. Cap nhat\n4. Xoa\n5. Tim kiem theo ID\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-5): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    repoTramBTS.loadFromFile();
                    const auto& ds = repoTramBTS.getAll();

                    if (ds.empty()) {
                        std::cout << "[-] Danh sach dang trong.\n";
                    }
                    else {
                        TramBTS::displayHeader();
                        for (const auto& tram : ds) {
                            tram->display();
                        }
                        std::cout << "Tong so ban ghi: " << ds.size() << "\n";
                    }
                }
                else if (chon == 2) {
                    std::cout << "\n-- Them Tram --\n";
                    auto tram = std::make_shared<TramBTS>();

                    thuLai([&]() {
                        std::string ma = Inputhelper::Docstring("Ma Tram (4 so): ");
                        tram->setId(Inputhelper::tuTaoMa(ma, "BTS"));
                        if (repoTramBTS.findById(tram->getId())) {
                            throw Ngoailerangbuoc("Loi: Ma Tram nay da ton tai!");
                        }
                        });

                    thuLai([&]() {
                        tram->setTenTram(Inputhelper::Docstring("Ten Tram: "));
                        });

                    thuLai([&]() {
                        tram->setLoaiCot(Inputhelper::Docstring("Loai Cot: "));
                        });

                    thuLai([&]() {
                        tram->setBanKinhPhuSong(Inputhelper::Docdouble("Ban Kinh phu song (km): "));
                        });

                    thuLai([&]() {
                        tram->setTrangThai(Inputhelper::Docstring("Trang thai (VD: Hoat dong, Bao tri, Loi): "));
                        });

                    repoTramBTS.add(tram);
                    std::cout << "[+] Them thanh cong!\n";
                }
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
                    std::string maInput = Inputhelper::Docstring("Ma Tram can sua (4 so): ");
                    auto tram = repoTramBTS.findById(Inputhelper::tuTaoMa(maInput, "BTS"));
                    if (!tram) throw Ngoailekotimthay("Khong tim thay Ma Tram nay!");

                    TramBTS banSao = *tram;

                    thuLai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten moi (" + banSao.getTenTram() + "): ", true);
                        if (!ten.empty()) banSao.setTenTram(ten);
                        });

                    thuLai([&]() {
                        std::string loai = Inputhelper::Docstring("Loai cot moi (" + banSao.getLoaiCot() + "): ", true);
                        if (!loai.empty()) banSao.setLoaiCot(loai);
                        });

                    thuLai([&]() {
                        double banKinhMoi = banSao.getBanKinhPhuSong();
                        if (Inputhelper::DocdoubleCapNhat("Ban kinh moi (" + std::to_string(banKinhMoi) + " km): ", banKinhMoi)) {
                            banSao.setBanKinhPhuSong(banKinhMoi);
                        }
                        });

                    thuLai([&]() {
                        std::string tt = Inputhelper::Docstring("Trang thai moi (" + banSao.getTrangThai() + "): ", true);
                        if (!tt.empty()) banSao.setTrangThai(tt);
                        });

                    *tram = banSao;
                    repoTramBTS.update();
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                else if (chon == 4) {
                    std::string maInput = Inputhelper::Docstring("\nMa Tram can xoa (4 so): ");
                    std::string ma = Inputhelper::tuTaoMa(maInput, "BTS");

                    if (!repoTramBTS.findById(ma)) throw Ngoailekotimthay("Khong tim thay Ma Tram nay!");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoTramBTS.remove(ma);
                        std::cout << "[+] Da xoa!\n";
                    }
                    else {
                        std::cout << "[*] Da huy thao tac xoa.\n";
                    }
                }
                else if (chon == 5) {
                    std::cout << "\n-- Tim Kiem --\n";
                    std::string maInput = Inputhelper::Docstring("Nhap Ma Tram can tim (4 so): ");
                    std::string ma = Inputhelper::tuTaoMa(maInput, "BTS");
                    auto tram = repoTramBTS.findById(ma);

                    if (!tram) {
                        std::cout << "[-] Khong tim thay tram co ma: " << ma << "\n";
                    }
                    else {
                        std::cout << "\n";
                        TramBTS::displayHeader();
                        tram->display();
                        std::cout << std::string(87, '-') << "\n";
                    }
                }
                else {
                    std::cout << "[-] Lua chon khong hop le!\n";
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
            catch (const std::exception& e) { std::cout << "[-] Loi He Thong: " << e.what() << "\n"; }
        }
    }
};