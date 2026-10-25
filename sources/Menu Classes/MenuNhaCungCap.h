#pragma once 
#include <iostream> 
#include <string>  
#include <memory>   
#include "Repository.h" 
#include "NhaCungCap.h"
#include "PhieuBaoTri.h"
#include "LoaiThietBi.h" // Thêm để đồng bộ chéo
#include "InputHelper.h" 
#include "Exceptions.h"  

class MenuNhaCungCap {
private:
    template <class F>
    static void thulai(F buoc) {
        while (true) {
            try { buoc(); return; }
            catch (const Baseexception& e) { std::cout << "   -> [Canh bao]: " << e.getMessage() << " (Moi nhap lai)\n"; }
            catch (const std::exception&) { std::cout << "   -> [Loi]: Gia tri khong hop le (Moi nhap lai)\n"; }
        }
    }

public:
    // Nhận thêm repoLoai để thực hiện cập nhật lan truyền
    static void run(Repository<NhaCungCap>& repoNCC, Repository<PhieuBaoTri>& repoPBT, Repository<LoaiThietBi>& repoLoai) {
        while (true) {
            std::cout << "\n--- QUAN LY NHA CUNG CAP ---\n";
            std::cout << "1. Xem danh sach\n2. Them NCC moi\n3. Cap nhat thong tin\n4. Xoa NCC\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-4): ");
            if (chon == 0) break;

            try {
                if (chon == 1) {
                    std::cout << "\n";
                    const auto& ds = repoNCC.getAll();
                    if (ds.empty()) std::cout << "[-] Danh sach Nha Cung Cap dang trong.\n";
                    else {
                        NhaCungCap::displayHeader();
                        for (const auto& ncc : ds) ncc->display();
                    }
                }
                else if (chon == 2) {
                    std::cout << "\n-- Them Nha Cung Cap --\n";
                    auto ncc = std::make_shared<NhaCungCap>();

                    thulai([&]() {
                        std::string ma = Inputhelper::Docstring("Ma NCC (4 so): ");
                        ncc->setId(Inputhelper::TuTaoMa(ma, "NCC"));
                        if (repoNCC.findById(ncc->getId())) throw Ngoailerangbuoc("Loi: Ma NCC nay da ton tai!");
                        });

                    thulai([&]() { ncc->setTenNCC(Inputhelper::Docstring("Ten Nha Cung Cap: ")); });
                    thulai([&]() { ncc->setDiaChi(Inputhelper::Docstring("Dia chi: ")); });
                    thulai([&]() { ncc->setSoDienThoai(Inputhelper::Docstring("So dien thoai: ")); });

                    repoNCC.add(ncc);
                    std::cout << "[+] Da ghi nhan Nha Cung Cap len he thong!\n";
                }
                else if (chon == 3) {
                    if (repoNCC.getAll().empty()) {
                        std::cout << "[-] Chua co NCC nao de cap nhat!\n";
                        continue;
                    }

                    std::cout << "\n-- Cap Nhat Nha Cung Cap --\n";
                    std::shared_ptr<NhaCungCap> ncc = nullptr;
                    std::string oldId = "";

                    while (true) {
                        try {
                            std::string rawId = Inputhelper::Docstring("Ma NCC can cap nhat (4 so) hoac nhap '0' de huy: ");
                            if (rawId == "0") break;

                            oldId = Inputhelper::TuTaoMa(rawId, "NCC");
                            ncc = repoNCC.findById(oldId);
                            if (!ncc) throw Ngoailekotimthay("Khong tim thay Nha Cung Cap nay tren he thong!");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "   -> " << e.getMessage() << "\n"; }
                    }

                    if (!ncc) continue;

                    // 1. Cho phép sửa Mã NCC
                    std::string newId = oldId;
                    thulai([&]() {
                        std::string rawNewId = Inputhelper::Docstring("Ma NCC moi (Enter de giu nguyen '" + oldId + "'): ", true);
                        if (!rawNewId.empty()) {
                            std::string candidateId = Inputhelper::TuTaoMa(rawNewId, "NCC");
                            if (candidateId != oldId && repoNCC.findById(candidateId)) {
                                throw Ngoailerangbuoc("Loi: Ma NCC moi nay da ton tai tren he thong!");
                            }
                            newId = candidateId;
                        }
                        });

                    thulai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten NCC moi (Enter de bo qua): ", true);
                        if (!ten.empty()) ncc->setTenNCC(ten);
                        });
                    thulai([&]() {
                        std::string dc = Inputhelper::Docstring("Dia chi moi (Enter de bo qua): ", true);
                        if (!dc.empty()) ncc->setDiaChi(dc);
                        });
                    thulai([&]() {
                        std::string sdt = Inputhelper::Docstring("So dien thoai moi (Enter de bo qua): ", true);
                        if (!sdt.empty()) ncc->setSoDienThoai(sdt);
                        });

                    // Nếu mã thay đổi, thực hiện đổi ID cho đối tượng trong Repository
                    if (newId != oldId) {
                        repoNCC.remove(oldId); // Xóa bản ghi cũ
                        ncc->setId(newId);     // Gán ID mới
                        repoNCC.add(ncc);      // Thêm lại vào Repo
                    }

                    repoNCC.update(); // Lưu xuống file nhacungcap.txt

                    // 2. Tự động đồng bộ lan truyền sang Loại thiết bị
                    bool coDongBo = false;
                    for (auto& lt : repoLoai.getAll()) {
                        if (lt->getMaNCC() == oldId || lt->getMaNCC() == newId) {
                            lt->setMaNCC(newId);          // Cập nhật mã mới
                            lt->setTenNCC(ncc->getTenNCC()); // Cập nhật tên mới nhất
                            coDongBo = true;
                        }
                    }
                    if (coDongBo) {
                        repoLoai.update(); // Lưu đè xuống file loaithietbi.txt
                        std::cout << "[+] Da tu dong cap nhat ma va ten NCC sang danh muc Loai Thiet Bi!\n";
                    }

                    std::cout << "[+] Cap nhat Nha Cung Cap hoan tat!\n";
                }
                else if (chon == 4) {
                    if (repoNCC.getAll().empty()) {
                        std::cout << "[-] Chua co NCC nao de xoa!\n";
                        continue;
                    }

                    std::shared_ptr<NhaCungCap> ncc = nullptr;
                    std::string maChuan;

                    while (true) {
                        try {
                            std::string rawId = Inputhelper::Docstring("\nMa NCC can go bo (4 so) hoac nhap '0' de huy: ");
                            if (rawId == "0") break;

                            maChuan = Inputhelper::TuTaoMa(rawId, "NCC");
                            ncc = repoNCC.findById(maChuan);
                            if (!ncc) throw Ngoailekotimthay("Khong tim thay NCC nay tren he thong!");
                            break;
                        }
                        catch (const Baseexception& e) { std::cout << "   -> " << e.getMessage() << "\n"; }
                    }

                    if (!ncc) continue;

                    if (Inputhelper::Docstring("Chac chan xoa? (y/n): ") == "y") {
                        repoNCC.remove(maChuan);
                        std::cout << "[+] Da xoa thanh cong!\n";
                    }
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; }
        }
    }
};
