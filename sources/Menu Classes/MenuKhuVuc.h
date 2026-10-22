

#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cctype>
#include "Exceptions.h"
#include "Repository.h"
#include "Inputhelper.h" 
#include "KhuVuc.h"
#include "TramBTS.h"

class MenuKhuVuc {
private:
    // Ham nay de nguoi dung nhap lai neu nhap sai logic
    // F o day la mot Lambda function truyen vao
    template <class F>
    static void thuLai(F buoc) {
        while (true) {
            try { buoc(); return; } // chay ok thi thoat vong lap
            catch (const Baseexception& e) {
                // neu co loi xac thuc tu tang Entity nem len thi in ra va bat nhap lai
                std::cout << "[!] " << e.getMessage() << " -> Moi nhap lai.\n";
            }
        }
    }

    // Ham phu tro de duaw chuoi ve chu thuong dung cho viec so sanh ko phan biet hoa thuong
    static std::string thuongHoa(std::string s) {
        for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }

    // in danh sach Khu vuc thanh dang bang co cot
    static void inBang(const std::vector<std::shared_ptr<KhuVuc>>& ds) {
        KhuVuc::displayHeader();
        for (const auto& kv : ds) kv->display();
    }

    // Kiem tra xem ten khu vuc nay co bi trung voi 1 ten nao do trong file chua
    static bool trungTen(const Repository<KhuVuc>& repo, const std::string& ten,
        const std::string& boQuaId) {
        std::string t = thuongHoa(Inputhelper::catkhoangtrang(ten));
        for (const auto& kv : repo.getAll())
            if (kv->getId() != boQuaId && thuongHoa(kv->getTenKhuVuc()) == t) return true;
        return false;
    }

public:
    // Ham chay chinh cua menu
    static void run(Repository<KhuVuc>& repoKV, Repository<TramBTS>& repoTram) {
        while (true) {
            std::cout << "\n================= QUAN LY KHU VUC / TINH =================\n";
            std::cout << "1. Xem danh sach\n";
            std::cout << "2. Them khu vuc moi \n";
            std::cout << "3. Tim kiem \n";
            std::cout << "4. Cap nhat thong tin\n";
            std::cout << "5. Xoa khu vuc\n";
            std::cout << "0. Quay lai menu chinh\n";
            std::cout << "==========================================================\n";

            int chon = Inputhelper::Docint(">> Nhap lua chon: ");
            if (chon == 0) break;

            try {
                // Them moi Khu Vuc
                if (chon == 2) {
                    auto kv = std::make_shared<KhuVuc>();

                    thuLai([&]() {
                        kv->setMa(Inputhelper::Docstring("Nhap Ma KV (4 so, VD: 0001): "));
                        // Check khoa chinh, neu ma da co trong mang thi bao loi
                        if (repoKV.findById(kv->getId()))
                            throw Ngoailerangbuoc("Loi: Ma khu vuc [" + kv->getId() + "] da ton tai!");
                        });

                    thuLai([&]() {
                        std::string ten = Inputhelper::Docstring("Nhap Ten khu vuc / tinh (chi nhap chu): ");
                        kv->setTenKhuVuc(ten);
                        // Check xem ten tinh nay da co nguoi tao chua
                        if (trungTen(repoKV, ten, ""))
                            throw Ngoailerangbuoc("Loi: Ten khu vuc da ton tai!");
                        });

                    thuLai([&]() {
                        kv->setNguoiQuanLy(Inputhelper::Docstring("Nhap Nguoi quan ly (chi nhap chu, Enter de bo qua): ", true));
                        });


                    thuLai([&]() {
                        std::string rawTram = Inputhelper::Docstring("Nhap Ma Tram BTS dieu phoi (4 so, VD: 1001, Enter de bo qua): ", true);
                        if (!rawTram.empty()) {
                            std::string maTramChuan = KhuVuc::taoMaTram(rawTram);
                            // Check khoa ngoai, truy van ben danh sach Tram xem ma nay co that ko
                            if (!repoTram.findById(maTramChuan)) {
                                throw Ngoailerangbuoc("Loi: Ma Tram [" + maTramChuan + "] khong ton tai tren he thong!");
                            }
                            kv->setMaTram(maTramChuan);
                        }
                        else {
                            kv->setMaTram("Chua xac dinh");
                        }
                        });

                    // Them vao repo va day xuong file txt
                    repoKV.add(kv);
                    std::cout << "[+] Them khu vuc moi thanh cong (Ma: " << kv->getId() << ")!\n";
                }
                // Xem toan bo danh sach
                else if (chon == 1) {
                    const auto& ds = repoKV.getAll();
                    if (ds.empty()) std::cout << "[-] Danh sach khu vuc dang trong.\n";
                    else inBang(ds);
                }
                // Tim kiem (ho tro tim theo ca id va chuoi ten)
                else if (chon == 3) {
                    std::string tuKhoa = Inputhelper::Docstring("Nhap ma (VD: 0001 / KV0001) hoac ten tinh can tim: ");
                    std::string ma = KhuVuc::chuanHoaMa(tuKhoa);
                    std::string ten = thuongHoa(tuKhoa);
                    std::vector<std::shared_ptr<KhuVuc>> ketQua;

                    for (const auto& kv : repoKV.getAll()) {
                        // Dung find de tim gan dung cho cai ten
                        if (kv->getId() == ma || kv->getId() == "KV" + ma ||
                            thuongHoa(kv->getTenKhuVuc()).find(ten) != std::string::npos) {
                            ketQua.push_back(kv);
                        }
                    }
                    if (ketQua.empty()) std::cout << "[-] Khong tim thay khu vuc phu hop.\n";
                    else inBang(ketQua);
                }
                // Cap nhat thong tin 
                else if (chon == 4) {
                    std::string id = KhuVuc::taoMaKV(Inputhelper::Docstring("Nhap Ma KV can sua (4 so, VD: 0001): "));
                    auto kv = repoKV.findById(id);
                    if (!kv) throw Ngoailekotimthay("Loi: Khong tim thay ma khu vuc [" + id + "]!");
                    inBang({ kv });

                    std::cout << "(*) Nhap gia tri moi (Enter de giu nguyen):\n";

                    // Clone ra mot ban sao de test nhap du lieu, neu nhap that bai thi class goc ko bi hong
                    KhuVuc banSao = *kv;

                    thuLai([&]() {
                        std::string ql = Inputhelper::Docstring("Quan ly moi (" + banSao.getNguoiQuanLy() + ") [Nhap '-' de xoa]: ", true);
                        if (!ql.empty()) {
                            if (ql == "-") banSao.setNguoiQuanLy("");
                            else banSao.setNguoiQuanLy(ql);
                        }
                        });


                    thuLai([&]() {
                        std::string rawTram = Inputhelper::Docstring("Ma Tram moi (" + banSao.getMaTram() + ") [Nhap '-' de xoa]: ", true);
                        if (!rawTram.empty()) {
                            if (rawTram == "-") {
                                banSao.setMaTram("Chua xac dinh");
                            }
                            else {
                                std::string maTramChuan = KhuVuc::taoMaTram(rawTram);
                                // Check lai khoa ngoai truoc khi cap nhat
                                if (!repoTram.findById(maTramChuan)) {
                                    throw Ngoailerangbuoc("Loi: Ma Tram [" + maTramChuan + "] khong ton tai tren he thong!");
                                }
                                banSao.setMaTram(maTramChuan);
                            }
                        }
                        });

                    // Gan lai ban sao vao con tro goc
                    *kv = banSao;
                    repoKV.update(); // dong bo xuong txt
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                // Xoa 
                else if (chon == 5) {
                    std::string id = KhuVuc::taoMaKV(Inputhelper::Docstring("Nhap Ma KV can xoa (4 so, VD: 0001): "));
                    auto kv = repoKV.findById(id);
                    if (!kv) throw Ngoailekotimthay("Loi: Khong tim thay ma khu vuc [" + id + "]!");
                    inBang({ kv });

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
            // Bat 2 loai loi, loi do logic nghiep vu hoac loi tu he thong
            catch (const Baseexception& e) { std::cout << "[!] LOI NGHIEP VU: " << e.getMessage() << "\n"; }
            catch (const std::exception& e) { std::cout << "[!] LOI HE THONG: " << e.what() << "\n"; }
        }
    }
};
