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
    template <class F>
    static void thuLai(F buoc) {
        while (true) {
            try {
                buoc(); // buoc nhap du lieu va kra rang buoc
                return; // neu k co loi thi thoat vong lap
            }
            catch (const Baseexception& e) {
                std::cout << "[!] " << e.getMessage() << " -> Moi nhap lai.\n";
            }
        }
    }

    static std::string thuongHoa(std::string s) {
        for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }

    static void inBang(const std::vector<std::shared_ptr<Anten>>& ds) {
        Anten::displayHeader();
        for (const auto& a : ds) a->display();
    }

    static std::string kiemTraVaChuanHoaLoaiAnten(const std::string& loaiCot, const std::string& loaiAnten) {
        std::string lCot = thuongHoa(Inputhelper::catkhoangtrang(loaiCot));
        std::string lAnten = thuongHoa(Inputhelper::catkhoangtrang(loaiAnten));

        std::vector<std::string> dsHopLe;

        // phai co danh sach cac loai anten hop le cho tung loai cot, neu khong co thi nem ngoai le
        if (lCot == "cot tu dung") dsHopLe = { "Anten Ma Tran 5G", "Anten Chao Viba", "Anten Loa" };
        else if (lCot == "cot day co") dsHopLe = { "Anten Quat 4G", "Anten Yagi", "Anten GPS" };
        else if (lCot == "cot than ong") dsHopLe = { "Anten Quat 3G", "Anten Vi Mo" };
        else if (lCot == "cot tren mai") dsHopLe = { "Anten Op Tuong", "Anten Chao Mini" };
        else if (lCot == "cot nguy trang") dsHopLe = { "Anten Dang Huong", "Anten Ong Khoi", "Anten Roi" };
        else throw Ngoailerangbuoc("Loi: Loai cot cua Tram khong xac dinh tren he thong!");

        // quet xem chuoi nhap vao co khop voi bat ky Anten nao trong danh sach khong
        for (const auto& antenChuan : dsHopLe) {
            if (lAnten == thuongHoa(antenChuan)) {
                return antenChuan; // Tra ve chuoi da duoc format hoan hao (5G, GPS...)
            }
        }

        // ph nhap dung loai anten di voi loai cot tuong ung
        std::string loi = "Loi: Cot nay chi ho tro [";
        for (size_t i = 0; i < dsHopLe.size(); ++i) {
            loi += dsHopLe[i];
            if (i < dsHopLe.size() - 1) loi += ", ";
        }
        loi += "]!";

        throw Ngoailerangbuoc(loi);
    }

	static void inBangTuongThich(const std::string& loaiCot) { // khi them anten moi hoac cap nhat anten thi in ra bang cac loai anten tuong thich voi loai cot cua tram bts de nguoi dung tham khao
        std::string lCot = thuongHoa(Inputhelper::catkhoangtrang(loaiCot));
        std::cout << "\n================ CHU THICH LAP DAT ANTEN ================\n";
        std::cout << "Tram ban chon dang dung: [" << loaiCot << "]\n";
        std::cout << "Cac loai Anten tuong thich cho phep nhap:\n";
        if (lCot == "cot tu dung") std::cout << " -> Anten Ma Tran 5G, Anten Chao Viba, Anten Loa\n";
        else if (lCot == "cot day co") std::cout << " -> Anten Quat 4G, Anten Yagi, Anten GPS\n";
        else if (lCot == "cot than ong") std::cout << " -> Anten Quat 3G, Anten Vi Mo\n";
        else if (lCot == "cot tren mai") std::cout << " -> Anten Op Tuong, Anten Chao Mini\n";
        else if (lCot == "cot nguy trang") std::cout << " -> Anten Dang Huong, Anten Ong Khoi, Anten Roi\n";
        std::cout << "=========================================================\n";
    }

public:
    static void run(Repository<Anten>& repoAnten, Repository<TramBTS>& repoTramBTS) {
        while (true) {
            std::cout << "\n============= QUAN LY ANTEN & CAU HINH PHAT =============\n";
            std::cout << "1. Xem danh sach\n";
            std::cout << "2. Them anten moi\n";
            std::cout << "3. Tim kiem anten theo ID\n";
            std::cout << "4. Cap nhat thong so anten\n";
            std::cout << "5. Xoa anten\n";
            std::cout << "6. Danh gia nang luc anten\n";
            std::cout << "0. Quay lai menu chinh\n";
            std::cout << "=========================================================\n";

            int chon = Inputhelper::Docint(">> Nhap lua chon (0-6): ");
            if (chon == 0) break;

            try {
				// them anten moi
                if (chon == 2) {
                    auto anten = std::make_shared<Anten>();

                    thuLai([&]() {
                        std::string maInput = Inputhelper::Docstring("Nhap Ma Anten (4 so, vi du: 0001) [enter de huy]: ", true);
                        if (maInput.empty()) throw 0;

                        anten->setMa(maInput);
                        if (repoAnten.findById(anten->getId()))
                            throw Ngoailerangbuoc("Loi: Ma anten [" + anten->getId() + "] da ton tai!");
                        });

                    thuLai([&]() {
                        std::string rawTram = Inputhelper::Docstring("Nhap Ma Tram BTS lap dat (4 so, vi du: 1001) [enter de huy]: ", true);
                        if (rawTram.empty()) throw 0;

                        anten->setMaTram(rawTram);
                        if (!repoTramBTS.findById(anten->getMaTram()))
                            throw Ngoailerangbuoc("Loi: Ma Tram [" + anten->getMaTram() + "] khong ton tai tren he thong");
                        });

                    // Lay loai cot cua Tram vua nhap
                    auto tram = repoTramBTS.findById(anten->getMaTram());
                    std::string loaiCot = tram->getLoaiCot();

					inBangTuongThich(loaiCot); // se in ra cac loai anten tuong thich voi loai cot cua tram vua nhap de nguoi dung tham khao

                    thuLai([&]() {
                        std::string loai = Inputhelper::Docstring("Nhap Loai anten [enter de huy]: ", true);
                        if (loai.empty()) throw 0;

                        // Kiem tra rang buoc cung (Hard Constraint)
                        anten->setLoaiAnten(kiemTraVaChuanHoaLoaiAnten(loaiCot, loai));
                        });

                    thuLai([&]() {
                        std::string cao = Inputhelper::Docstring("Nhap Do cao anten [5.0 - 150.0 m] [enter de huy]: ", true);
                        if (cao.empty()) throw 0;
                        anten->setChieuCao(Anten::phanTichSoThuc(cao));
                        });

                    thuLai([&]() {
                        std::string goc = Inputhelper::Docstring("Nhap Goc Cua Anten [0 - 359 do] [enter de huy]: ", true);
                        if (goc.empty()) throw 0;
                        anten->setGocAzimuth(Anten::phanTichSoThuc(goc)); // ep kieu ve so thuc
                        });

                    repoAnten.add(anten);
                    std::cout << "[+] Them anten moi thanh cong (Ma: " << anten->getId() << ")\n";
                }
                // xem
                else if (chon == 1) {
                    const auto& ds = repoAnten.getAll();
                    if (ds.empty()) std::cout << "Danh sach anten dang trong.\n";
                    else inBang(ds);
                }
               
				// tim kiem anten theo ma
                else if (chon == 3) {
                    std::string tuKhoa = Inputhelper::Docstring("Nhap Ma Anten can tim (4 so, VD: 0001) [enter de huy]: ", true);
                    if (tuKhoa.empty()) throw 0;

                    // dung ham taoMaAnten de bat loi neu nhap chu va tu dong them tien to AT
                    std::string ma = Anten::taoMaAnten(tuKhoa);
                    std::vector<std::shared_ptr<Anten>> ketQua;

                    for (const auto& a : repoAnten.getAll()) {
                        if (a->getId() == ma) {
                            ketQua.push_back(a);
							break; // co ma la duy nhat nen tim thay la thoat vong lap luon
                        }
                    }
                    if (ketQua.empty()) std::cout << "[-] Khong tim thay anten phu hop.\n";
                    else inBang(ketQua);
                }
				// cap nhat anten
                else if (chon == 4) {
                    std::string inputSua = Inputhelper::Docstring("Nhap Ma Anten can sua (4 so, VD: 0001) [enter de huy]: ", true);
                    if (inputSua.empty()) throw 0;

                    std::string id = Anten::taoMaAnten(inputSua);
                    auto at = repoAnten.findById(id);
                    if (!at) throw Ngoailekotimthay("Loi: Khong tim thay anten [" + id + "]");
                    inBang({ at });

                    std::cout << " + Nhap gia tri moi (enter de giu nguyen):\n";
                    Anten banSao = *at;

                    thuLai([&]() {
                        std::string s = Inputhelper::Docstring("Ma Tram moi (4 so, hien tai " + banSao.getMaTram() + ") [enter de giu nguyen]: ", true);
                        if (!s.empty()) {
                            std::string maTramChuan = Anten::taoMaTram(s);
                            if (!repoTramBTS.findById(maTramChuan))
                                throw Ngoailerangbuoc("Loi: Ma Tram khong ton tai");
                            banSao.setMaTram(maTramChuan);
                        }
                        });

                    // Lay loai cot cua Tram
                    auto tramMoi = repoTramBTS.findById(banSao.getMaTram());
                    std::string loaiCot = tramMoi->getLoaiCot();

					inBangTuongThich(loaiCot); // in ra cac loai anten tuong thich voi loai cot cua tram moi

                    thuLai([&]() {
                        std::string s = Inputhelper::Docstring("Loai moi (" + banSao.getLoaiAnten() + ") [enter de giu nguyen]: ", true);
                        if (!s.empty()) {
                            banSao.setLoaiAnten(kiemTraVaChuanHoaLoaiAnten(loaiCot, s));
                        }
                        else {
                            // ktra  rang buoc cheo neu doi Tram thi Anten cux co the ko con hop le
                            banSao.setLoaiAnten(kiemTraVaChuanHoaLoaiAnten(loaiCot, banSao.getLoaiAnten()));
                        }
                        });

                    thuLai([&]() {
                        std::string s = Inputhelper::Docstring("Do cao moi (" + banSao.chieuCaoToString() + " m) [enter de giu nguyen]: ", true);
                        if (!s.empty()) banSao.setChieuCao(Anten::phanTichSoThuc(s));
                        });

                    thuLai([&]() {
                        // dung ham gocToString() de xoa bot so 0 thua khi in ra goi y
                        std::string s = Inputhelper::Docstring("Azimuth moi (" + banSao.gocToString() + " do) [enter de giu nguyen]: ", true);
                        if (!s.empty()) banSao.setGocAzimuth(Anten::phanTichSoThuc(s)); // ep kieu ve so thuc
                        });

                    *at = banSao;
                    repoAnten.update();
                    std::cout << " Cap nhat thanh cong\n";
                }
				// xoa anten
                else if (chon == 5) {
                    std::string inputXoa = Inputhelper::Docstring("Nhap Ma Anten can xoa (4 so, vi du: 0001) [enter de huy]: ", true);
                    if (inputXoa.empty()) throw 0;

                    std::string id = Anten::taoMaAnten(inputXoa);
                    auto at = repoAnten.findById(id);
                    if (!at) throw Ngoailekotimthay("Loi: Khong tim thay anten [" + id + "]!");
                    inBang({ at });

                    std::string xn = Inputhelper::Docstring("Ban co chac chan muon xoa (y/n)? ");
                    if (xn == "y" || xn == "Y") {
                        repoAnten.remove(id);
                        repoAnten.update();
                        std::cout << "Da xoa anten thanh cong!\n";
                    }
                    else {
                        std::cout << "Da huy thao tac xoa.\n";
                    }
                }
				// danh gia nang luc anten
                else if (chon == 6) {
                    std::cout << "\n-- Danh Gia Cu Ly Tam Nhin Thang (LOS) --\n";

                    std::string inputDG = Inputhelper::Docstring("Nhap Ma Anten can danh gia (4 so, vi du: 0001) [enter de huy]: ", true);
                    if (inputDG.empty()) throw 0;

                    std::string id = Anten::taoMaAnten(inputDG);
                    auto at = repoAnten.findById(id);

                    if (!at) throw Ngoailekotimthay("Loi: Khong tim thay anten [" + id + "]");

                    inBang({ at });

                    double r0 = at->tinhCuLyTamNhinThangToiThietBiDiDong();

                    std::cout << "\nTHONG SO KY THUAT:\n";
                    std::cout << " + Do cao cua anten phat (ht): " << at->getChieuCao() << " m\n";
                    std::cout << " + Do cao cua thiet bi thu (hr - Dien thoai): 1.5 m\n"; // chieu cao khi cam dien thoai
                    std::cout << " + Ap dung cong thuc: r0 = 3.57*(sqrt(ht) + sqrt(hr))\n";
                    std::cout << " -> Cu ly tam nhin thang toi da (r0): " << std::fixed << std::setprecision(2) << r0 << " km\n";
                }
                else {
                    std::cout << "Lua chon khong hop le!\n";
                }
            }

            catch (int) {
                std::cout << "\n Da huy thao tac \n";
            }
            catch (const Baseexception& e) { std::cout << "LOI NGHIEP VU: " << e.getMessage() << "\n"; }
            catch (const std::exception& e) { std::cout << "LOI HE THONG: " << e.what() << "\n"; }
        }
    }
};
