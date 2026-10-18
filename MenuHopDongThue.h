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
    // HÀM RUN TRUYỀN VÀO CẢ 2 KHO DỮ LIỆU ĐỂ KIỂM TRA KHÓA NGOẠI
    static void run(Repository<HopDongThue>& repoHopDong, Repository<TramBTS>& repoTramBTS) {
        while (true) {
            std::cout << "\n--- QUAN LY HOP DONG THUE ---\n";
            std::cout << "1. Xem danh sach\n2. Them moi\n3. Cap nhat\n4. Xoa\n5. Tim kiem theo ID\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-5): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    repoHopDong.loadFromFile();
                    const auto& ds = repoHopDong.getAll();

                    if (ds.empty()) {
                        std::cout << "[-] Danh sach Hop dong dang trong.\n";
                    }
                    else {
                        HopDongThue::displayHeader();
                        for (const auto& hd : ds) hd->display();
                        std::cout << "Tong so ban ghi: " << ds.size() << "\n";
                    }
                }
                else if (chon == 2) {
                    std::cout << "\n-- Them Hop Dong --\n";
                    auto hd = std::make_shared<HopDongThue>();

                    thuLai([&]() {
                        std::string ma = Inputhelper::Docstring("Ma HD (4 so): ");
                        hd->setId(Inputhelper::tuTaoMa(ma, "HD"));
                        if (repoHopDong.findById(hd->getId())) {
                            throw Ngoailerangbuoc("Loi: Ma Hop dong nay da ton tai!");
                        }
                        });

                    // KIỂM TRA KHÓA NGOẠI TỪ BẢNG TRAMBTS
                    thuLai([&]() {
                        std::string maBTS = Inputhelper::Docstring("Ma Tram BTS (4 so): ");
                        std::string maChuan = Inputhelper::tuTaoMa(maBTS, "BTS");
                        if (!repoTramBTS.findById(maChuan)) {
                            throw Ngoailerangbuoc("Loi: Ma Tram nay khong ton tai trong he thong! Vui long nhap dung Ma Tram.");
                        }
                        hd->setMaTram(maBTS);
                        });

                    thuLai([&]() { hd->setTenChuNha(Inputhelper::Docstring("Ten chu nha: ")); });

                    thuLai([&]() { hd->setSoDienThoai(Inputhelper::Docstring("So dien thoai: ")); });

                    thuLai([&]() {
                        std::string giaStr = Inputhelper::Docstring("Gia thue (VND): ");
                        hd->setGiaThueThang(std::stoll(giaStr));
                        });

                    thuLai([&]() { hd->setNgayBatDau(Inputhelper::Docstring("Ngay bat dau (dd/mm/yyyy): ")); });

                    thuLai([&]() { hd->setThoiHanThang(Inputhelper::Docint("Thoi han (thang): ")); });

                    repoHopDong.add(hd);
                    std::cout << "[+] Them thanh cong!\n";
                }
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
                    std::string rawId = Inputhelper::Docstring("Ma HD can sua (4 so): ");
                    auto hd = repoHopDong.findById(Inputhelper::tuTaoMa(rawId, "HD"));
                    if (!hd) throw Ngoailekotimthay("Khong tim thay Ma HD nay!");

                    // Tao ban sao de sua (An toan du lieu)
                    HopDongThue banSao = *hd;

                    thuLai([&]() {
                        std::string maBTS = Inputhelper::Docstring("Ma Tram moi (" + banSao.getMaTram() + "): ", true);
                        if (!maBTS.empty()) {
                            std::string maChuan = Inputhelper::tuTaoMa(maBTS, "BTS");
                            if (!repoTramBTS.findById(maChuan)) {
                                throw Ngoailerangbuoc("Loi: Ma Tram nay khong ton tai trong he thong!");
                            }
                            banSao.setMaTram(maBTS);
                        }
                        });

                    thuLai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten chu nha moi (" + banSao.getTenChuNha() + "): ", true);
                        if (!ten.empty()) banSao.setTenChuNha(ten);
                        });

                    thuLai([&]() {
                        std::string sdt = Inputhelper::Docstring("SDT moi (" + banSao.getSoDienThoai() + "): ", true);
                        if (!sdt.empty()) banSao.setSoDienThoai(sdt);
                        });

                    thuLai([&]() {
                        std::string giaStr = Inputhelper::Docstring("Gia moi (" + std::to_string(banSao.getGiaThueThang()) + " VND): ", true);
                        if (!giaStr.empty()) banSao.setGiaThueThang(std::stoll(giaStr));
                        });

                    thuLai([&]() {
                        std::string ngay = Inputhelper::Docstring("Ngay bat dau moi (" + banSao.getNgayBatDau() + "): ", true);
                        if (!ngay.empty()) banSao.setNgayBatDau(ngay);
                        });

                    thuLai([&]() {
                        std::string thangStr = Inputhelper::Docstring("Thoi han moi (" + std::to_string(banSao.getThoiHanThang()) + " thang): ", true);
                        if (!thangStr.empty()) banSao.setThoiHanThang(std::stoi(thangStr));
                        });

                    *hd = banSao; // Ghi de ban nhap vao ban chinh
                    repoHopDong.update();
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                else if (chon == 4) {
                    std::string rawId = Inputhelper::Docstring("\nMa HD can xoa (4 so): ");
                    std::string id = Inputhelper::tuTaoMa(rawId, "HD");

                    if (!repoHopDong.findById(id)) throw Ngoailekotimthay("Khong tim thay Ma HD nay!");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoHopDong.remove(id);
                        std::cout << "[+] Da xoa!\n";
                    }
                    else {
                        std::cout << "[*] Da huy thao tac xoa.\n";
                    }
                }
                else if (chon == 5) {
                    std::cout << "\n-- Tim Kiem --\n";
                    std::string rawId = Inputhelper::Docstring("Nhap Ma HD can tim (4 so): ");
                    std::string id = Inputhelper::tuTaoMa(rawId, "HD");
                    auto hd = repoHopDong.findById(id);

                    if (!hd) {
                        std::cout << "[-] Khong tim thay Hop dong co ma: " << id << "\n";
                    }
                    else {
                        std::cout << "\n";
                        HopDongThue::displayHeader();
                        hd->display();
                        std::cout << std::string(95, '-') << "\n";
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