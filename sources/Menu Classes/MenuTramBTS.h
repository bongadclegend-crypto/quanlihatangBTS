//Hoang Tuan Thanh
#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Repository.h"
#include "TramBTS.h"
#include "Inputhelper.h"
#include "Exceptions.h"
#include "Anten.h"
#include "KhuVuc.h" 
#include "SuCoTram.h"
#include "ThietBiLapDat.h"
#include "HopDongThue.h"

class MenuTramBTS {
private:
    template <class F>
    static void thulai(F buoc) {
        while (true) {
            try {
                buoc();
                return;
            }
            catch (const Baseexception& e) {
                std::cout << "[!] " << e.getMessage() << " -> Moi nhap lai"<<std::endl;
            }
            catch (const std::exception&) {
                std::cout << "[!] Gia tri khong hop le -> Moi nhap lai"<<std::endl;
            }
        }
    }
public:
    template <class Anten, class KhuVuc, class SuCo, class ThietBiLapDat, class HopDongThue>
    static void run(Repository<TramBTS>& repoTramBTS,
        Repository<Anten>& repoAnten,
        Repository<KhuVuc>& repoKhuVuc,
        Repository<SuCo>& repoSuCo,
        Repository<ThietBiLapDat>& repoThietBiLapDat,
        Repository<HopDongThue>& repoHopDong) {

        while (true) {
            std::cout << "\n--- QUAN LY TRAM BTS ---\n";
            std::cout << "1. Xem danh sach\n2. Them moi\n3. Cap nhat\n4. Xoa\n5. Tim kiem theo ID\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-5): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    repoTramBTS.loadfromfile();
                    const auto& ds = repoTramBTS.getAll();

                    if (ds.empty()) {
                        std::cout << " Danh sach dang trong"<<std::endl;
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
                    std::cout << "\n-- them tram --\n";
                    auto tram = std::make_shared<TramBTS>();

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("ma tram (4 so): ");
                        tram->setId(Inputhelper::TuTaoMa(ma, "BTS"));
                        if (repoTramBTS.findById(tram->getId())) {
                            throw Ngoailerangbuoc("loi: ma tram nay da ton tai");
                        }
                        });

                    thulai([&]() {
                        tram->setTenTram(Inputhelper::Docstring("Ten Tram: "));
                        });

                    thulai([&]() {
                        std::cout << "Cac loai: Cot Tu Dung, Cot Day Co, Cot Nguy Trang, Cot Than Ong, Cot Tren Mai\n";
                        tram->setLoaiCot(Inputhelper::Docstring("Loai Cot: "));
                        });

                    thulai([&]() {
                        tram->setBanKinhPhuSong(Inputhelper::Docdouble("Ban Kinh phu song (km): "));
                        });

                    thulai([&]() {
                        std::cout << "Cac loai: Hoat Dong, Dung Hoat Dong\n";
                        tram->setTrangThai(Inputhelper::Docstring("Trang thai: "));
                        });

                    repoTramBTS.add(tram);
                    std::cout << "[+] Them thanh cong!\n";
                }
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat (van phai nhap id nhung cac truong khac ko nhap thi bo qua bang nut nhan enter) --\n";
                    std::string maInput = Inputhelper::Docstring("Ma Tram can sua (4 so): ");
                    auto tram = repoTramBTS.findById(Inputhelper::TuTaoMa(maInput, "BTS"));
                    if (!tram) throw Ngoailekotimthay("Khong tim thay Ma Tram nay!");

                    TramBTS banSao = *tram;

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten moi (" + banSao.getTenTram() + "): ", true);
                        if (!ten.empty()) banSao.setTenTram(ten);
                        });

                    // ra canh bao ve thay doi loai cot phai thay doi anten
                    std::cout << "\n thong bao quan trong:\n";
                    std::cout << "neu thay doi loai cot thi phai thay doi loai anten ,neu ko thay doi co loi gi thi tu chiu trach nhiem  "<<std::endl;
                    std::cout << "cap nhat lai cac anten dang treo neu chung khong con phu hop voi Cot moi!"<<std::endl;

                    thulai([&]() {
                        std::cout << "loai cot: Cot Tu Dung, Cot Day Co, Cot Nguy Trang, Cot Than Ong, Cot Tren Mai"<<std::endl;
                        std::string loai = Inputhelper::Docstring("loai cot moi (" + banSao.getLoaiCot() + "): ", true);
                        if (!loai.empty()) banSao.setLoaiCot(loai);
                        });

                    thulai([&]() {
                        double banKinhMoi = banSao.getBanKinhPhuSong();
                        if (Inputhelper::DocdoubleCapNhat("ban kinh moi (" + std::to_string(banKinhMoi) + " km): ", banKinhMoi)) {
                            banSao.setBanKinhPhuSong(banKinhMoi);
                        }
                        });

                    thulai([&]() {
                        std::cout << "Trang Thai: Hoat Dong, Dung Hoat Dong"<<std::endl;
                        std::string tt = Inputhelper::Docstring("Trang thai moi (" + banSao.getTrangThai() + "): ", true);
                        if (!tt.empty()) banSao.setTrangThai(tt);
                        });

                    *tram = banSao;
                    repoTramBTS.update();
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                else if (chon == 4) {
                    std::string maInput = Inputhelper::Docstring("\nMa Tram can xoa (4 so): ");
                    std::string ma = Inputhelper::TuTaoMa(maInput, "BTS");

                    if (!repoTramBTS.findById(ma)) throw Ngoailekotimthay("Khong tim thay Ma Tram nay");

                    int loiThamChieu = 0;
                    std::string baoLoi = "ko duoc xoa: Tram " + ma + " dang bi rang buoc boi:\n";

                    for (const auto& item : repoAnten.getAll()) {
                        if (item->getMaTram() == ma) { loiThamChieu++; baoLoi += "- Du lieu Anten\n"; break; }
                    }

                    
                    for (const auto& item : repoKhuVuc.getAll()) {
                        if (item->getMaTram() == ma) { loiThamChieu++; baoLoi += "- Du lieu Khu Vuc\n"; break; }
                    }

                    for (const auto& item : repoSuCo.getAll()) {
                        if (item->getMaTram() == ma) { loiThamChieu++; baoLoi += "- Du lieu Su Co Tram\n"; break; }
                    }
                    for (const auto& item : repoThietBiLapDat.getAll()) {
                        if (item->getMaTram() == ma) { loiThamChieu++; baoLoi += "- Du lieu Thiet Bi Lap Dat\n"; break; }
                    }
                    for (const auto& item : repoHopDong.getAll()) {
                        if (item->getMaTram() == ma) { loiThamChieu++; baoLoi += "- Hop Dong Thue Vi Tri\n"; break; }
                    }

                    if (loiThamChieu > 0) {
                        baoLoi += "vui long xoa cac du lieu phu thuoc tren truoc khi thao do tram";
                        throw Ngoailerangbuoc(baoLoi);
                    }

                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoTramBTS.remove(ma);
                        std::cout << "[+] Da xoa!\n";
                    }
                    else {
                        std::cout << "[-] Da huy thao tac xoa\n";
                    }
                }
                else if (chon == 5) {
                    std::cout << "\n-- tim kiem --\n";
                    std::string maInput = Inputhelper::Docstring("Nhap Ma Tram can tim (4 so): ");
                    std::string ma = Inputhelper::TuTaoMa(maInput, "BTS");
                    auto tram = repoTramBTS.findById(ma);

                    if (!tram) {
                        std::cout << "[-] Khong tim thay tram co ma: " << ma << "\n";
                    }
                    else {
                        std::cout << std::endl;
                        TramBTS::displayHeader();
                        tram->display();
                        std::cout << std::string(87, '-') << "\n";
                    }
                }
                else {
                    std::cout << "[-] Lua chon khong hop le\n";
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
            catch (const std::exception& e) { std::cout << "[-] Loi He Thong: " << e.what() << "\n"; }
        }
    }
};
