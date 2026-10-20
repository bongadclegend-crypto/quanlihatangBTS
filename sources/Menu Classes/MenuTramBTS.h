//Hoang Tuan Thanh
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
    // ham bat loi bat loi lai ngay khi nhap sai ngay lap tuc
    template <class F>//tempplatye nay la 1 buoc nhap du lieu 
    static void thulai(F buoc) {
        while (true) {//vong lap vo han
            try {
                buoc();
                return; // Nếu không có lỗi thì thoát vòng lặp, đi tiếp
            }
            catch (const Baseexception& e) {
                std::cout << "[!] " << e.getMessage() << " -> Moi nhap lai"<<std::endl;
            }
            catch (const std::exception&) {
                std::cout << "[!] Gia tri khong hop le -> Moi nhap lai" << std::endl;;
            }
        }
    }

public:
    static void run(Repository<TramBTS>& repoTramBTS) {//truyen tham chieu de tim kiem tren ram ,static de ko bat may tinh tao ra 1 doi tuong 
        while (true) {
            std::cout << "\n--- QUAN LY TRAM BTS ---\n";
            std::cout << "1. Xem danh sach\n2. Them moi\n3. Cap nhat\n4. Xoa\n5. Tim kiem theo ID\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-5): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    repoTramBTS.loadfromfile();
                    const auto& ds = repoTramBTS.getAll();//lay toan bo danh sach trong tram bts au to thay shared_ptr,getall lay toan bo danh sach

                    if (ds.empty()) {
                        std::cout << "[-] Danh sach dang trong" << std::endl;
                    }
                    else {
                        TramBTS::displayHeader();
                        for (const auto& tram : ds) {
                            tram->display();
                        }
                        std::cout << "Tong so ban ghi: " << ds.size() << std::endl;
                    }
                }
                else if (chon == 2) {
                    std::cout << "\n-- Them Tram --\n";
                    auto tram = std::make_shared<TramBTS>();//make_shareed ptr tao 1 ban nhap tren ma ko can xoa

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Ma Tram (4 so): ");
                        tram->setId(Inputhelper::TuTaoMa(ma, "BTS"));
                        if (repoTramBTS.findById(tram->getId())) {
                            throw Ngoailerangbuoc("Loi: Ma Tram nay da ton tai!");
                        }
                        });

                    thulai([&]() {
                        tram->setTenTram(Inputhelper::Docstring("Ten Tram: "));
                        });

                    thulai([&]() {
                        tram->setLoaiCot(Inputhelper::Docstring("Loai Cot: "));
                        });

                    thulai([&]() {
                        tram->setBanKinhPhuSong(Inputhelper::Docdouble("Ban Kinh phu song (km): "));
                        });

                    thulai([&]() {
                        tram->setTrangThai(Inputhelper::Docstring("Trang thai (VD: Hoat dong, Bao tri, Loi): "));
                        });

                    repoTramBTS.add(tram);
                    std::cout << "[+] Them thanh cong!\n";
                }
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
                    std::string maInput = Inputhelper::Docstring("Ma Tram can sua (4 so): ");
                    auto tram = repoTramBTS.findById(Inputhelper::TuTaoMa(maInput, "BTS"));
                    if (!tram) throw Ngoailekotimthay("Khong tim thay Ma Tram nay!");

                    TramBTS banSao = *tram;

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten moi (" + banSao.getTenTram() + "): ", true);
                        if (!ten.empty()) banSao.setTenTram(ten);
                        });

                    thulai([&]() {
                        std::string loai = Inputhelper::Docstring("Loai cot moi (" + banSao.getLoaiCot() + "): ", true);
                        if (!loai.empty()) banSao.setLoaiCot(loai);
                        });

                    thulai([&]() {
                        double banKinhMoi = banSao.getBanKinhPhuSong();
                        if (Inputhelper::DocdoubleCapNhat("Ban kinh moi (" + std::to_string(banKinhMoi) + " km): ", banKinhMoi)) {
                            banSao.setBanKinhPhuSong(banKinhMoi);
                        }
                        });

                    thulai([&]() {
                        std::string tt = Inputhelper::Docstring("Trang thai moi (" + banSao.getTrangThai() + "): ", true);
                        if (!tt.empty()) banSao.setTrangThai(tt);
                        });

                    *tram = banSao;
                    repoTramBTS.update();
                    std::cout << "[+] Cap nhat thanh cong!"<<std::endl;
                }
                else if (chon == 4) {
                    std::string maInput = Inputhelper::Docstring("\nMa Tram can xoa (4 so): ");
                    std::string ma = Inputhelper::TuTaoMa(maInput, "BTS");

                    if (!repoTramBTS.findById(ma)) throw Ngoailekotimthay("Khong tim thay Ma Tram nay!");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoTramBTS.remove(ma);
                        std::cout << "[+] Da xoa!"<<std::endl;
                    }
                    else {
                        std::cout << "[*] Da huy thao tac xoa"<<std::endl;
                    }
                }
                else if (chon == 5) {
                    std::cout << "\n-- Tim Kiem --\n";
                    std::string maInput = Inputhelper::Docstring("Nhap Ma Tram can tim (4 so): ");
                    std::string ma = Inputhelper::TuTaoMa(maInput, "BTS");
                    auto tram = repoTramBTS.findById(ma);

                    if (!tram) {
                        std::cout << "[-] Khong tim thay tram co ma: " << ma << std::endl;
                    }
                    else {
                        std::cout << "\n";
                        TramBTS::displayHeader();
                        tram->display();
                        std::cout << std::string(87, '-') << std::endl;
                    }
                }
                else {
                    std::cout << "[-] Lua chon khong hop le"<<std::endl;
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
            catch (const std::exception& e) { std::cout << "[-] Loi He Thong: " << e.what() << "\n"; }
        }
    }
};
