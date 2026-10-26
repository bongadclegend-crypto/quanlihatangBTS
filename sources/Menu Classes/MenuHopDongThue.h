//Hoang Tuan Thanh
#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Repository.h"
#include "HopDongThue.h"
#include "TramBTS.h"
#include "Inputhelper.h"
#include "Exceptions.h"

class MenuHopDongThue {
private:
    //ho tro bat loi
    template <class F>
    static void thulai(F buoc) {
        while (true) {//von lap vo han
            try {
                buoc();
                return; // neu ko co loi thoat vong lap va tiep tuc
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
    //template khu vuc
    template <class KhuVuc>
    static void run(Repository<HopDongThue>& repoHopDong, Repository<TramBTS>& repoTramBTS, Repository<KhuVuc>& repoKV) {

        auto timMaKV = [&](const std::string& maBTS) {//tu dong in ra id ma khu vuc
            for (const auto& kv : repoKV.getAll()) {
                if (kv->getMaTram() == maBTS) return kv->getId();
            }
            return std::string("Chua co");
            };

        auto daChoThue = [&](const std::string& maBTS, const std::string& maHDBoQua = "") {//kiem tra xem chi co 1 tram  va 1 hop dong thoa man
            for (const auto& hd : repoHopDong.getAll()) {
                if (hd->getId() != maHDBoQua && hd->getMaTram() == maBTS) return true;
            }
            return false;
            };

        while (true) {//giao dien
            std::cout << "\n--- QUAN LY HOP DONG THUE ---\n";
            std::cout << "1. Xem danh sach\n2. Them moi\n3. Cap nhat\n4. Xoa\n5. Tim kiem theo ID\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-5): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {//lay danh sach hop dong
                    std::cout << "\n";
                    repoHopDong.loadfromfile();
                    const auto& ds = repoHopDong.getAll();

                    if (ds.empty()) {
                        std::cout << "[-] danh sach Hop dong dang trong"<<std::endl;
                    }
                    else {
                        HopDongThue::displayHeader();
                        for (const auto& hd : ds) {
                            hd->display(timMaKV(hd->getMaTram()));
                        }
                        std::cout << "tong so ban ghi: " << ds.size() << std::endl;
                    }
                }
                else if (chon == 2) {//them moi
                    std::cout << "\n-- Them Hop Dong --\n";
                    auto hd = std::make_shared<HopDongThue>();

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("ma HD (4 so) [Enter de huy]: ", true);
                        if (ma.empty()) throw 0;
                        hd->setId(Inputhelper::TuTaoMa(ma, "HD"));
                        if (repoHopDong.findById(hd->getId())) {
                            throw Ngoailerangbuoc("loi: ma hop dong nay da ton tai");
                        }
                        });

                    // kiem tra khoa ngoai ma tram
                    thulai([&]() {
                        std::string maBTS = Inputhelper::Docstring("ma tram BTS (4 so) [Enter de huy]: ", true);
                        if (maBTS.empty()) throw 0;

                        std::string maChuan = Inputhelper::TuTaoMa(maBTS, "BTS");
                        if (!repoTramBTS.findById(maChuan)) {
                            throw Ngoailerangbuoc("loi: ma Tram nay khong ton tai trong he thong! Vui long nhap dung Ma Tram");
                        }
                        if (daChoThue(maChuan)) {
                            throw Ngoailerangbuoc("tu choi: tram BTS nay da duoc dang ky hop dong thue khac roi!");
                        }
                        hd->setMaTram(maBTS);
                        std::cout << "   -> [He thong]: tram nay dang thuoc Khu Vuc: " << timMaKV(maChuan) << std::endl;
                        });

                    thulai([&]() {
                        std::string t = Inputhelper::Docstring("Ten chu nha [Enter de huy]: ", true);
                        if (t.empty()) throw 0; hd->setTenChuNha(t);
                        });

                    thulai([&]() {
                        std::string s = Inputhelper::Docstring("So dien thoai [Enter de huy]: ", true);
                        if (s.empty()) throw 0; hd->setSoDienThoai(s);
                        });

                    thulai([&]() {
                        std::string giaStr = Inputhelper::Docstring("Gia thue (VND) [Enter de huy]: ", true);
                        if (giaStr.empty()) throw 0;
                        hd->setGiaThueThang(giaStr);
                        });

                    thulai([&]() {
                        std::string d = Inputhelper::Docstring("Ngay bat dau (dd/mm/yyyy) [Enter de huy]: ", true);
                        if (d.empty()) throw 0; hd->setNgayBatDau(d);
                        });

                    thulai([&]() {
                        std::string th = Inputhelper::Docstring("Thoi han (thang) [Enter de huy]: ", true);
                        if (th.empty()) throw 0;
                        hd->setThoiHanThang(th);
                        });

                    repoHopDong.add(hd);
                    std::cout << "[+] Them thanh cong"<<std::endl;
                }
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
                    std::string rawId = Inputhelper::Docstring("Ma HD can sua (4 so) [Enter de huy]: ", true);
                    if (rawId.empty()) throw 0;

                    auto hd = repoHopDong.findById(Inputhelper::TuTaoMa(rawId, "HD"));
                    if (!hd) throw Ngoailekotimthay("Khong tim thay Ma HD nay");

                    // tao 1 ban sao du lieu
                    HopDongThue banSao = *hd;

                    thulai([&]() {
                        std::string maBTS = Inputhelper::Docstring("Ma Tram moi (" + banSao.getMaTram() + "): ", true);
                        if (!maBTS.empty()) {
                            std::string maChuan = Inputhelper::TuTaoMa(maBTS, "BTS");
                            if (!repoTramBTS.findById(maChuan)) {
                                throw Ngoailerangbuoc("Loi: Ma Tram nay khong ton tai trong he thong!");
                            }
                            if (daChoThue(maChuan, banSao.getId())) {
                                throw Ngoailerangbuoc("Tu choi: Tram BTS nay da co hop dong thue khac giu cho!");
                            }
                            banSao.setMaTram(maBTS);
                            std::cout << "   -> [He thong]: Tram moi nay thuoc Khu Vuc: " << timMaKV(maChuan) << "\n";
                        }
                        });

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten chu nha moi (" + banSao.getTenChuNha() + "): ", true);
                        if (!ten.empty()) banSao.setTenChuNha(ten);
                        });

                    thulai([&]() {
                        std::string sdt = Inputhelper::Docstring("SDT moi (" + banSao.getSoDienThoai() + "): ", true);
                        if (!sdt.empty()) banSao.setSoDienThoai(sdt);
                        });

                    thulai([&]() {
                        std::string giaStr = Inputhelper::Docstring("Gia moi (" + std::to_string(banSao.getGiaThueThang()) + " VND): ", true);
                        if (!giaStr.empty()) {
                            banSao.setGiaThueThang(giaStr);
                        }
                        });

                    thulai([&]() {
                        std::string ngay = Inputhelper::Docstring("Ngay bat dau moi (" + banSao.getNgayBatDau() + "): ", true);
                        if (!ngay.empty()) banSao.setNgayBatDau(ngay);
                        });

                    thulai([&]() {
                        std::string thangStr = Inputhelper::Docstring("Thoi han moi (" + std::to_string(banSao.getThoiHanThang()) + " thang): ", true);
                        if (!thangStr.empty()) {
                            banSao.setThoiHanThang(thangStr);
                        }
                        });

                    *hd = banSao; // ghi de ban nhap vao ban chinh
                    repoHopDong.update();
                    std::cout << "[+] Cap nhat thanh cong"<<std::endl;
                }
                else if (chon == 4) {
                    std::string rawId = Inputhelper::Docstring("\nMa HD can xoa (4 so) [Enter de huy]: ", true);
                    if (rawId.empty()) throw 0;

                    std::string id = Inputhelper::TuTaoMa(rawId, "HD");

                    if (!repoHopDong.findById(id)) throw Ngoailekotimthay("Khong tim thay Ma HD nay");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoHopDong.remove(id);
                        std::cout << "[+] Da xoa"<<std::endl;
                    }
                    else {
                        std::cout << "[-] Da huy thao tac xoa"<<std::endl;
                    }
                }
                else if (chon == 5) {
                    std::cout << "\n-- Tim Kiem --\n";
                    std::string rawId = Inputhelper::Docstring("Nhap Ma HD can tim (4 so) [Enter de huy]: ", true);
                    if (rawId.empty()) throw 0;

                    std::string id = Inputhelper::TuTaoMa(rawId, "HD");
                    auto hd = repoHopDong.findById(id);

                    if (!hd) {
                        std::cout << "[-] Khong tim thay Hop dong co ma: " << id << "\n";
                    }
                    else {
                        std::cout << "\n";
                        HopDongThue::displayHeader();
                        hd->display(timMaKV(hd->getMaTram()));
                        std::cout << std::string(105, '-') << std::endl;
                    }
                }
                else {
                    std::cout << "[-] Lua chon khong hop le"<<std::endl;
                }
            }
            catch (int) {
                std::cout << "\n[-] da huy thao tac xoa"<<std::endl;
            }
            catch (const Baseexception& e) { std::cout << "[-] loi nghiep vu: " << e.getMessage() << std::endl; }
            catch (const std::exception& e) { std::cout << "[-] loi he thong: " << e.what() << std::endl; }
        }
    }
};
