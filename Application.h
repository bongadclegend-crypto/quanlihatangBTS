#pragma once
#include <iostream>
#include <memory>
#include "Repository.h"
#include "NhaCungCap.h"
#include "LoaiThietBi.h"
#include "InputHelper.h"

class Application {
private:
    Repository<NhaCungCap> repoNCC;
    Repository<LoaiThietBi> repoLoaiTB;

public:
    Application() : repoNCC("nhacungcap.txt"), repoLoaiTB("loaithietbi.txt") {
        repoNCC.loadFromFile();
        repoLoaiTB.loadFromFile();
    }

    void run() {
        while (true) {
            std::cout << "\n============================================\n";
            std::cout << "   HE THONG QUAN LY HA TANG TRAM BTS\n";
            std::cout << "============================================\n";
            std::cout << "1. Quan ly Nha Cung Cap\n";
            std::cout << "2. Quan ly Loai Thiet Bi\n";
            std::cout << "0. Thoat chuong trinh\n";
            std::cout << "--------------------------------------------\n";

            int chon = Inputhelper::Docint("Vui long chon chuc nang (0-2): ");

            if (chon == 0) {
                std::cout << "\nDa luu toan bo du lieu. Tam biet!\n";
                break;
            }
            else if (chon == 1) {
                menuNhaCungCap();
            }
            else if (chon == 2) {
                menuLoaiThietBi();
            }
            else {
                std::cout << "Lua chon khong hop le. Vui long nhap lai!\n";
            }
        }
    }

private:
    // ==========================================================
    // MENU 1: QUẢN LÝ NHÀ CUNG CẤP (FULL CRUD)
    // ==========================================================
    void menuNhaCungCap() {
        while (true) {
            std::cout << "\n--- QUAN LY NHA CUNG CAP ---\n";
            std::cout << "1. Xem danh sach (Read)\n";
            std::cout << "2. Them moi (Create)\n";
            std::cout << "3. Cap nhat thong tin (Update)\n";
            std::cout << "4. Xoa (Delete)\n";
            std::cout << "0. Quay lai Menu Chinh\n";

            int chon = Inputhelper::Docint("Chon chuc nang (0-4): ");
            if (chon == 0) break;

            try {
                // [READ]
                if (chon == 1) {
                    std::cout << "\n";
                    NhaCungCap::displayHeader();
                    for (const auto& ncc : repoNCC.getAll()) {
                        ncc->display();
                    }
                }
                // [CREATE]
                else if (chon == 2) {
                    std::cout << "\n-- Them Nha Cung Cap --\n";
                    std::string ma = Inputhelper::Docstring("Nhap Ma NCC: ");
                    std::string ten = Inputhelper::Docstring("Nhap Ten NCC: ");
                    std::string sdt = Inputhelper::Docstring("Nhap So Dien Thoai: ");
                    std::string dc = Inputhelper::Docstring("Nhap Dia Chi: ");

                    auto ncc = std::make_shared<NhaCungCap>(ma, ten, sdt, dc);
                    repoNCC.add(ncc);
                    std::cout << "[+] Them thanh cong!\n";
                }
                // [UPDATE]
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat Nha Cung Cap --\n";
                    std::string ma = Inputhelper::Docstring("Nhap Ma NCC can sua: ");
                    auto ncc = repoNCC.findById(ma); // Tìm theo mã

                    if (ncc == nullptr) {
                        std::cout << "[-] Loi: Khong tim thay Ma NCC nay!\n";
                    }
                    else {
                        std::cout << "\nThong tin hien tai:\n";
                        NhaCungCap::displayHeader();
                        ncc->display();
                        std::cout << "(Meo: Nhan Enter de bo qua neu khong muon sua truong do)\n";

                        // Truyền true vào tham số thứ 2 của Docstring để cho phép nhập rỗng
                        std::string ten = Inputhelper::Docstring("Nhap Ten NCC moi: ", true);
                        if (!ten.empty()) ncc->setTenNhaCungCap(ten);

                        std::string sdt = Inputhelper::Docstring("Nhap So Dien Thoai moi: ", true);
                        if (!sdt.empty()) ncc->setSoDienThoai(sdt);

                        std::string dc = Inputhelper::Docstring("Nhap Dia Chi moi: ", true);
                        if (!dc.empty()) ncc->setDiaChi(dc);

                        repoNCC.update(); // Ghi đè file txt
                        std::cout << "[+] Cap nhat thanh cong!\n";
                    }
                }
                // [DELETE]
                else if (chon == 4) {
                    std::cout << "\n-- Xoa Nha Cung Cap --\n";
                    std::string ma = Inputhelper::Docstring("Nhap Ma NCC can xoa: ");
                    auto ncc = repoNCC.findById(ma);

                    if (ncc == nullptr) {
                        std::cout << "[-] Loi: Khong tim thay Ma NCC nay!\n";
                    }
                    else {
                        std::cout << "\nBan dang chuan bi xoa ban ghi sau:\n";
                        NhaCungCap::displayHeader();
                        ncc->display();

                        std::string xacNhan = Inputhelper::Docstring("Ban co chac chan muon xoa? (y/n): ");
                        if (xacNhan == "y" || xacNhan == "Y") {
                            repoNCC.remove(ma); // Xóa trên RAM và cập nhật File
                            std::cout << "[+] Xoa thanh cong!\n";
                        }
                        else {
                            std::cout << "[-] Da huy thao tac xoa.\n";
                        }
                    }
                }
            }
            catch (const Baseexception& e) {
                std::cout << "\n[Loi he thong]: " << e.getMessage() << "\n";
            }
        }
    }

    // ==========================================================
    // MENU 2: QUẢN LÝ LOẠI THIẾT BỊ (FULL CRUD)
    // ==========================================================
    void menuLoaiThietBi() {
        while (true) {
            std::cout << "\n--- QUAN LY LOAI THIET BI ---\n";
            std::cout << "1. Xem danh sach (Read)\n";
            std::cout << "2. Them moi (Create)\n";
            std::cout << "3. Cap nhat thong tin (Update)\n";
            std::cout << "4. Xoa (Delete)\n";
            std::cout << "0. Quay lai Menu Chinh\n";

            int chon = Inputhelper::Docint("Chon chuc nang (0-4): ");
            if (chon == 0) break;

            try {
                // [READ]
                if (chon == 1) {
                    std::cout << "\n";
                    LoaiThietBi::displayHeader();
                    for (const auto& ltb : repoLoaiTB.getAll()) {
                        ltb->display();
                    }
                }
                // [CREATE]
                else if (chon == 2) {
                    std::cout << "\n-- Them Loai Thiet Bi --\n";
                    std::string ma = Inputhelper::Docstring("Nhap Ma Loai TB: ");
                    std::string ten = Inputhelper::Docstring("Nhap Ten Loai: ");
                    std::string dvt = Inputhelper::Docstring("Nhap Don Vi Tinh: ");
                    int sl = Inputhelper::Docint("Nhap So Luong: "); // THÊM DÒNG NÀY
                    std::string gc = Inputhelper::Docstring("Nhap Ghi Chu (co the bo qua): ", true);

                    auto ltb = std::make_shared<LoaiThietBi>(ma, ten, dvt,sl, gc);
                    repoLoaiTB.add(ltb);
                    std::cout << "[+] Them thanh cong!\n";
                }
                // [UPDATE]
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat Loai Thiet Bi --\n";
                    std::string ma = Inputhelper::Docstring("Nhap Ma Loai TB can sua: ");
                    auto ltb = repoLoaiTB.findById(ma);

                    if (ltb == nullptr) {
                        std::cout << "[-] Loi: Khong tim thay Ma nay!\n";
                    }
                    else {
                        std::cout << "\nThong tin hien tai:\n";
                        LoaiThietBi::displayHeader();
                        ltb->display();
                        std::cout << "(Meo: Nhan Enter de bo qua neu khong muon sua truong do)\n";

                        std::string ten = Inputhelper::Docstring("Nhap Ten Loai moi: ", true);
                        if (!ten.empty()) ltb->setTenLoai(ten);

                        std::string dvt = Inputhelper::Docstring("Nhap Don Vi Tinh moi: ", true);
                        if (!dvt.empty()) ltb->setDonViTinh(dvt);
                        // Dùng chuỗi để cho phép họ Enter bỏ trống nếu không muốn sửa số lượng
                        std::string slStr = Inputhelper::Docstring("Nhap So Luong moi: ", true);
                        if (!slStr.empty()) ltb->setSoLuong(std::stoi(slStr));

                        std::string gc = Inputhelper::Docstring("Nhap Ghi Chu moi: ", true);
                        if (!gc.empty()) ltb->setGhiChu(gc);

                        repoLoaiTB.update();
                        std::cout << "[+] Cap nhat thanh cong!\n";
                    }
                }
                // [DELETE]
                else if (chon == 4) {
                    std::cout << "\n-- Xoa Loai Thiet Bi --\n";
                    std::string ma = Inputhelper::Docstring("Nhap Ma Loai TB can xoa: ");
                    auto ltb = repoLoaiTB.findById(ma);

                    if (ltb == nullptr) {
                        std::cout << "[-] Loi: Khong tim thay Ma nay!\n";
                    }
                    else {
                        std::cout << "\nBan dang chuan bi xoa ban ghi sau:\n";
                        LoaiThietBi::displayHeader();
                        ltb->display();

                        std::string xacNhan = Inputhelper::Docstring("Ban co chac chan muon xoa? (y/n): ");
                        if (xacNhan == "y" || xacNhan == "Y") {
                            repoLoaiTB.remove(ma);
                            std::cout << "[+] Xoa thanh cong!\n";
                        }
                        else {
                            std::cout << "[-] Da huy thao tac xoa.\n";
                        }
                    }
                }
            }
            catch (const Baseexception& e) {
                std::cout << "\n[Loi he thong]: " << e.getMessage() << "\n";
            }
        }
    }
};
