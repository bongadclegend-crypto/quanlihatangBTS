
#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include "Exceptions.h"
#include "Repository.h"
#include "Inputhelper.h" 
#include "Anten.h"
#include "TramBTS.h"

class MenuAnten {
private:
    // Ham chay thu lai block code (try-catch), pass Lambda vao
    template <class F>
    static void thuLai(F buoc) {
        while (true) {
            try { buoc(); return; } // nhap chuan thi thoat
            catch (const Baseexception& e) {
                // in loi va bat nhap lai 
                std::cout << "[!] " << e.getMessage() << " -> Moi nhap lai.\n";
            }
        }
    }

    // Ep chuoi ve chu thuong de so sanh (tim kiem ko phan biet hoa thuong)
    static std::string thuongHoa(std::string s) {
        for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }

    // In ra dang bang kem header
    static void inBang(const std::vector<std::shared_ptr<Anten>>& ds) {
        Anten::displayHeader();
        for (const auto& a : ds) a->display();
    }

    // Kiem tra Ma Tram sau khi da ep chuan (Vi du: nhap "1001" -> ep thanh "BTS1001")
    static void kiemTraTramTonTai(const Repository<TramBTS>& repoTram, const std::string& maTramChuan) {
        // Neu ko tim thay ma nay ben danh sach Tram thi chui lien
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
            std::cout << "6. Danh gia nang luc phu song \n"; // kien thuc mon Truyen song & Anten
            std::cout << "0. Quay lai menu chinh\n";
            std::cout << "=========================================================\n";

            int chon = Inputhelper::Docint(">> Nhap lua chon (0-6): ");
            if (chon == 0) break;

            try {
                // 1. Them moi
                if (chon == 1) {
                    auto anten = std::make_shared<Anten>();
                    thuLai([&]() {
                        anten->setMa(Inputhelper::Docstring("Nhap Ma Anten (4 so, VD: 0001): "));
                        // Check xem ma nay da co ai tao chua
                        if (repoAnten.findById(anten->getId()))
                            throw Ngoailerangbuoc("Loi: Ma anten [" + anten->getId() + "] da ton tai!");
                        });

                    thuLai([&]() {
                        std::string rawTram = Inputhelper::Docstring("Nhap Ma Tram BTS lap dat (4 so, VD: 1001): ");
                        anten->setMaTram(rawTram);
                        // Check khoa ngoai ngay luc tao
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
                // 2. In danh sach
                else if (chon == 2) {
                    const auto& ds = repoAnten.getAll();
                    if (ds.empty()) std::cout << "[-] Danh sach anten dang trong.\n";
                    else inBang(ds);
                }
                // 3. Tim kiem chung
                else if (chon == 3) {
                    std::string tuKhoa = Inputhelper::Docstring("Nhap ma anten (VD: 0001), ma tram (VD: 1001) hoac loai anten: ");
                    std::string ma = Anten::chuanHoaMa(tuKhoa);
                    std::string loai = thuongHoa(tuKhoa);
                    std::vector<std::shared_ptr<Anten>> ketQua;

                    for (const auto& a : repoAnten.getAll()) {
                        // So sanh ca ma AT, ma BTS hoac ten loai anten
                        if (a->getId() == ma || a->getId() == "AT" + ma ||
                            a->getMaTram() == ma || a->getMaTram() == "BTS" + ma ||
                            thuongHoa(a->getLoaiAnten()).find(loai) != std::string::npos) {
                            ketQua.push_back(a);
                        }
                    }
                    if (ketQua.empty()) std::cout << "[-] Khong tim thay anten phu hop.\n";
                    else inBang(ketQua);
                }
                // 4. Update thong tin
                else if (chon == 4) {
                    std::string id = Anten::taoMaAnten(Inputhelper::Docstring("Nhap Ma Anten can sua (4 so, VD: 0001): "));
                    auto at = repoAnten.findById(id);
                    if (!at) throw Ngoailekotimthay("Loi: Khong tim thay anten [" + id + "]!");
                    inBang({ at });

                    std::cout << "(*) Nhap gia tri moi (Enter de giu nguyen):\n";
                    Anten banSao = *at; // Dung ban sao de cap nhat nhap ko bi hu class goc

                    thuLai([&]() {
                        std::string s = Inputhelper::Docstring("Ma Tram moi (4 so, hien tai " + banSao.getMaTram() + "): ", true);
                        if (!s.empty()) {
                            std::string maTramChuan = Anten::taoMaTram(s);
                            // sua cung phai check khoa ngoai lai
                            kiemTraTramTonTai(repoTramBTS, maTramChuan);
                            banSao.setMaTram(maTramChuan);
                        }
                        });

                    thuLai([&]() {
                        std::string s = Inputhelper::Docstring("Loai moi (" + banSao.getLoaiAnten() + "): ", true);
                        if (!s.empty()) banSao.setLoaiAnten(s);
                        });

                    thuLai([&]() {
                        std::string s = Inputhelper::Docstring("Do cao moi (" + banSao.chieuCaoToString() + " m): ", true);
                        if (!s.empty()) {
                            double cao = Anten::phanTichSoThuc(s);
                            banSao.setChieuCao(cao);
                        }
                        });

                    thuLai([&]() {
                        std::string s = Inputhelper::Docstring("Azimuth moi (" + std::to_string(banSao.getGocAzimuth()) + " do): ", true);
                        if (!s.empty()) {
                            int goc = Anten::phanTichSoNguyen(s);
                            banSao.setGocAzimuth(goc);
                        }
                        });

                    // Cap nhat va luu xuong file
                    *at = banSao;
                    repoAnten.update();
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                // 5. Xoa
                else if (chon == 5) {
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
                // 6. Danh gia nang luc ( cong thuc mon Truyen Song & Anten)
                else if (chon == 6) {
                    std::cout << "\n-- Danh Gia Cu Ly Tam Nhin Thang (LOS) --\n";
                    std::string id = Anten::taoMaAnten(Inputhelper::Docstring("Nhap Ma Anten can danh gia (4 so, VD: 0001): "));
                    auto at = repoAnten.findById(id);

                    if (!at) throw Ngoailekotimthay("Loi: Khong tim thay anten [" + id + "]!");

                    inBang({ at });
                    double r0 = at->tinhCuLyTamNhinThangToiThietBiDiDong();

                    std::cout << "\n[*] THONG SO KY THUAT:\n";
                    std::cout << " - Do Cao Anten Phat (ht): " << at->getChieuCao() << " m\n";
                    std::cout << " - Do Cao Thiet Bi Thu (hr - Dien thoai): 0.179 m\n";
                    std::cout << "[+] CU LY TAM NHIN THANG TOI DA (r0): " << std::fixed << std::setprecision(2) << r0 << " km\n";
                    std::cout << "(*) Ghi chu: Tinh toan theo mo hinh truyen song cuc ngan trong dieu kien li tuong, co xet den do cong cua be mat trai dat.\n";
                }
                else std::cout << "[-] Lua chon khong hop le!\n";
            }
            
            catch (const Baseexception& e) { std::cout << "[!] LOI NGHIEP VU: " << e.getMessage() << "\n"; }
            catch (const std::exception& e) { std::cout << "[!] LOI HE THONG: " << e.what() << "\n"; }
        }
    }
};
