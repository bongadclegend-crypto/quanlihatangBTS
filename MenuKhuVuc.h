// Tac gia: Nguyen Cong Vinh
// code moi sua ngay 19
#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cctype>
#include "Exceptions.h"
#include "Repository.h"
#include "Inputhelper.h" // Đã fix
#include "KhuVuc.h"
#include "TramBTS.h"

class MenuKhuVuc {
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

    static void inBang(const std::vector<std::shared_ptr<KhuVuc>>& ds) {
        KhuVuc::displayHeader();
        for (const auto& kv : ds) kv->display();
    }

    static bool trungTen(const Repository<KhuVuc>& repo, const std::string& ten,
        const std::string& boQuaId) {
        std::string t = thuongHoa(Inputhelper::catkhoangtrang(ten));
        for (const auto& kv : repo.getAll())
            if (kv->getId() != boQuaId && thuongHoa(kv->getTenKhuVuc()) == t) return true;
        return false;
    }

public:
    // Giữ nguyên tham số repoTram để Application.h không bị báo lỗi sai cấu trúc
    static void run(Repository<KhuVuc>& repoKV, Repository<TramBTS>& repoTram) {
        while (true) {
            std::cout << "\n================= QUAN LY KHU VUC / TINH =================\n";
            std::cout << "1. Them khu vuc moi\n";
            std::cout << "2. Xem danh sach khu vuc\n";
            std::cout << "3. Tim kiem khu vuc\n";
            std::cout << "4. Cap nhat thong tin khu vuc\n";
            std::cout << "5. Xoa khu vuc\n";
            std::cout << "0. Quay lai menu chinh\n";
            std::cout << "==========================================================\n";

            int choice = Inputhelper::Docint(">> Nhap lua chon: ");
            if (choice == 0) break;

            try {
                if (choice == 1) {
                    auto kv = std::make_shared<KhuVuc>();
                    thuLai([&]() {
                        kv->setMa(Inputhelper::Docstring("Nhap Ma KV (4 so, VD: 0001): "));
                        if (repoKV.findById(kv->getId()))
                            throw Ngoailerangbuoc("Loi: Ma khu vuc [" + kv->getId() + "] da ton tai!");
                        });
                    thuLai([&]() {
                        std::string ten = Inputhelper::Docstring("Nhap Ten khu vuc / tinh (chi nhap chu): ");
                        kv->setTenKhuVuc(ten);
                        if (trungTen(repoKV, ten, ""))
                            throw Ngoailerangbuoc("Loi: Ten khu vuc da ton tai!");
                        });
                    thuLai([&]() {
                        kv->setNguoiQuanLy(Inputhelper::Docstring("Nhap Nguoi quan ly (chi nhap chu, Enter de bo qua): ", true));
                        });
                    repoKV.add(kv);
                    std::cout << "[+] Them khu vuc moi thanh cong (Ma: " << kv->getId() << ")!\n";
                }
                else if (choice == 2) {
                    const auto& ds = repoKV.getAll();
                    if (ds.empty()) std::cout << "[-] Danh sach khu vuc dang trong.\n";
                    else inBang(ds);
                }
                else if (choice == 3) {
                    std::string tuKhoa = Inputhelper::Docstring("Nhap ma (VD: 0001 / KV0001) hoac ten tinh can tim: ");
                    std::string ma = KhuVuc::chuanHoaMa(tuKhoa);
                    std::string ten = thuongHoa(tuKhoa);
                    std::vector<std::shared_ptr<KhuVuc>> ketQua;

                    for (const auto& kv : repoKV.getAll()) {
                        if (kv->getId() == ma || kv->getId() == "KV" + ma ||
                            thuongHoa(kv->getTenKhuVuc()).find(ten) != std::string::npos) {
                            ketQua.push_back(kv);
                        }
                    }
                    if (ketQua.empty()) std::cout << "[-] Khong tim thay khu vuc phu hop.\n";
                    else inBang(ketQua);
                }
                else if (choice == 4) {
                    std::string id = KhuVuc::taoMaKV(Inputhelper::Docstring("Nhap Ma KV can sua (4 so, VD: 0001): "));
                    auto kv = repoKV.findById(id);
                    if (!kv) throw Ngoailekotimthay("Loi: Khong tim thay ma khu vuc [" + id + "]!");
                    inBang({ kv });

                    std::cout << "(*) Nhap gia tri moi (Enter de giu nguyen):\n";
                    KhuVuc banSao = *kv;

                    thuLai([&]() {
                        std::string ql = Inputhelper::Docstring("Quan ly moi (" + banSao.getNguoiQuanLy() + ") [Nhap '-' de xoa]: ", true);
                        if (!ql.empty()) {
                            if (ql == "-") banSao.setNguoiQuanLy("");
                            else banSao.setNguoiQuanLy(ql);
                        }
                        });

                    *kv = banSao;
                    repoKV.update();
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                else if (choice == 5) {                   // XOA - ĐÃ THÁO CHẶN LIÊN KẾT
                    std::string id = KhuVuc::taoMaKV(Inputhelper::Docstring("Nhap Ma KV can xoa (4 so, VD: 0001): "));
                    auto kv = repoKV.findById(id);
                    if (!kv) throw Ngoailekotimthay("Loi: Khong tim thay ma khu vuc [" + id + "]!");
                    inBang({ kv });

                    // Đã bỏ hàm kiểm tra khóa ngoại demTramThuoc ở đây, cho phép xóa tự do

                    std::string xn = Inputhelper::Docstring("Ban co chac chan muon xoa (y/n)? ");
                    if (xn == "y" || xn == "Y") {
                        repoKV.remove(id);
                        std::cout << "[+] Da xoa khu vuc thanh cong!\n";
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
