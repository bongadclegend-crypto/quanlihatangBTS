// Nguyen Cong Vinh

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
#include "HopDongThue.h"

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
    } // ham chuyen chuoi thanh chu thuong de so sanh

    static void inBang(const std::vector<std::shared_ptr<KhuVuc>>& ds) {// ham in bang danh sach khu vuc
        KhuVuc::displayHeader();
        for (const auto& kv : ds) kv->display();
    }

    static bool trungTen(const Repository<KhuVuc>& repo, const std::string& ten, // ham kiem tra trung ten khu vuc, bo qua id de cap nhat
        const std::string& boQuaId) {
        std::string t = thuongHoa(Inputhelper::catkhoangtrang(ten));
        for (const auto& kv : repo.getAll())
            if (kv->getId() != boQuaId && thuongHoa(kv->getTenKhuVuc()) == t) return true;
        return false;
    }

    static bool kiemTraTramDaCoKhuVuc(const Repository<KhuVuc>& repoKV, const std::string& maTram, const std::string& boQuaIdKV = "") { // phan nay ktra xem ma tram da ton tai hay chua
        for (const auto& kv : repoKV.getAll()) {
            if (kv->getId() != boQuaIdKV && kv->getMaTram() == maTram) {
                return true;
            }
        }
        return false;
    }

public:
    static void run(Repository<KhuVuc>& repoKV, Repository<TramBTS>& repoTram, Repository<HopDongThue>& repoHopDong) { // ham chinh de quan ly khu vuc repoHopDong de kiem tra xem co hop dong thue nao dang quan ly khu vuc nay hay khong
        while (true) {
            std::cout << "\n================= QUAN LY KHU VUC =================\n";
            std::cout << "1. Xem danh sach\n";
            std::cout << "2. Them khu vuc moi \n";
            std::cout << "3. Tim kiem theo ID\n";
            std::cout << "4. Cap nhat thong tin\n";
            std::cout << "5. Xoa khu vuc\n";
            std::cout << "0. Quay lai menu chinh\n";
            std::cout << "===================================================\n";

            int chon = Inputhelper::Docint(">> Nhap lua chon: ");
            if (chon == 0) break;

            try {
                // them moi
                if (chon == 2) {
                    auto kv = std::make_shared<KhuVuc>(); // lay ra 1 doi tuong khu vuc moi de them vao danh sach

                    thuLai([&]() {
                        std::string maInput = Inputhelper::Docstring("Nhap Ma KV (4 so, VD: 0001) [enter de huy]: ", true);
                        if (maInput.empty()) throw 0;

                        kv->setMa(maInput);
                        if (repoKV.findById(kv->getId()))
                            throw Ngoailerangbuoc("Loi: Ma khu vuc [" + kv->getId() + "] da ton tai");
                        });

                    thuLai([&]() {
                        std::string ten = Inputhelper::Docstring("Nhap Ten Khu Vuc (vi du: Ha Noi , Ninh Binh ...) [enter de huy]: ", true);
                        if (ten.empty()) throw 0;

                        kv->setTenKhuVuc(ten); // neu nhap dung thi set ten khu vuc cho doi tuong kv
                        if (trungTen(repoKV, ten, ""))
                            throw Ngoailerangbuoc("Loi: Ten khu vuc nay da ton tai tren he thong!");
                        });

                    thuLai([&]() {
                        kv->setNguoiQuanLy(Inputhelper::Docstring("Nhap Nguoi quan ly (chi nhap chu, enter de bo qua): ", true));
                        });

                    thuLai([&]() {
                        std::string rawTram = Inputhelper::Docstring("Nhap Ma Tram BTS dieu phoi (4 so, enter de bo qua): ", true);
                        if (!rawTram.empty()) {
                            std::string maTramChuan = KhuVuc::taoMaTram(rawTram);
                            if (!repoTram.findById(maTramChuan)) {
                                throw Ngoailerangbuoc("Loi: Ma Tram [" + maTramChuan + "] khong ton tai tren he thong");
                            }
                            if (kiemTraTramDaCoKhuVuc(repoKV, maTramChuan)) {
                                throw Ngoailerangbuoc("Tu choi: Tram [" + maTramChuan + "] dang duoc quan ly boi mot Khu Vuc khac");
                            }
                            kv->setMaTram(maTramChuan);
							// if de ktra xem tram da ton tai hay ch va ktra xem tram da thuoc ql o khu vuc khac hay chua neu co thi bao loi va yeu cau nhap lai
                        }
                        else {
                            kv->setMaTram("Chua xac dinh");
                        }
                        });

                    repoKV.add(kv);
                    std::cout << "- Them khu vuc moi thanh cong (Ma: " << kv->getId() << ")!\n";
                    
                }
                // xem danh sach
                else if (chon == 1) {
                    const auto& ds = repoKV.getAll();
                    if (ds.empty()) std::cout << "- Danh sach khu vuc dang trong.\n";
                    else inBang(ds);
                }
                // tim kiem
                else if (chon == 3) {
                    std::string tuKhoa = Inputhelper::Docstring("Nhap ma KV can tim (4 so, vi du: 0001) [enter de huy]: ", true);
                    if (tuKhoa.empty()) throw 0;

                    // dung luon ham taoMaKV de chuan hoa dau vao thanh chuan "KVxxxx"
                    std::string ma = KhuVuc::taoMaKV(tuKhoa);
                    std::vector<std::shared_ptr<KhuVuc>> ketQua;

                    // Tim kiem trong danh sach
                    for (const auto& kv : repoKV.getAll()) {
                        if (kv->getId() == ma) {
                            ketQua.push_back(kv);
                            break; // vi ma la duy nhat nen tim thay la thoat vong lap luon
                        }
                    }
                    if (ketQua.empty()) std::cout << "-Khong tim thay khu vuc phu hop.\n";
                    else inBang(ketQua);
                }
                // cap nhat
                else if (chon == 4) {
                    std::string inputSua = Inputhelper::Docstring("Nhap Ma KV can sua (4 so, VD: 0001) [enter de huy]: ", true);
                    if (inputSua.empty()) throw 0;

                    std::string id = KhuVuc::taoMaKV(inputSua);
                    auto kv = repoKV.findById(id);
                    if (!kv) throw Ngoailekotimthay("Loi: Khong tim thay ma khu vuc [" + id + "]!");
                    inBang({ kv });

                    std::cout << "- Nhap gia tri moi (Enter de giu nguyen):\n";
                    KhuVuc banSao = *kv;

                    thuLai([&]() {
                        std::string ten = Inputhelper::Docstring("Ten Khu Vuc moi (" + banSao.getTenKhuVuc() + ") [enter de giu nguyen]: ", true);
                        if (!ten.empty()) {
                            banSao.setTenKhuVuc(ten);
                            // Check xem ten moi co trung voi ten cua cac khu vuc khac hay khong
                            if (trungTen(repoKV, ten, banSao.getId()))
                                throw Ngoailerangbuoc("Loi: Ten khu vuc nay da ton tai tren he thong"); // neu trung thi bao loi va yeu cau nhap lai
                        }
                        });

                    thuLai([&]() { // ham thuLai de nhap lai neu co loi xay ra
                        std::string ql = Inputhelper::Docstring("Quan ly moi (" + banSao.getNguoiQuanLy() + ") [nhap '-' de xoa, enter de giu nguyen]: ", true);
                        if (!ql.empty()) { // if nay de kiem tra xem nguoi quan ly co nhap gi hay khong neu khong thi giu nguyen
                            if (ql == "-") banSao.setNguoiQuanLy("");
                            else banSao.setNguoiQuanLy(ql);
                        }
                        });

                    thuLai([&]() { 
                        // ham nay kiem tra xem tram moi co ton tai trong he thong hay khon neu khong thi bao loi va yeu cau nhap lai
                        std::string rawTram = Inputhelper::Docstring("Ma Tram moi (" + banSao.getMaTram() + ") [Nhap '-' de xoa, enter de giu nguyen]: ", true);
                        if (!rawTram.empty()) {
                            // neu nhap ma tram chua them o tram bts thi bao la chua xac dinh vi phai them o lop tram bts truoc khi them vao khu vuc
                            if (rawTram == "-") {
                                banSao.setMaTram("Chua xac dinh");
                            }
                            else {
                                std::string maTramChuan = KhuVuc::taoMaTram(rawTram);
                                if (!repoTram.findById(maTramChuan)) { 
                                    // doi chieu xem ma tram co ton tai trong he thong hay khong neu khong thi bao loi va yeu cau nhap lai
                                    throw Ngoailerangbuoc("Loi: Ma Tram [" + maTramChuan + "] khong ton tai tren he thong");
                                }
                                if (kiemTraTramDaCoKhuVuc(repoKV, maTramChuan, banSao.getId())) {
                                    // doi chieu xem tram da tom tai o khu vuc khac hay chua neu co thi bao loi va yeu cau nhap lai
                                    throw Ngoailerangbuoc("Tu choi: Tram [" + maTramChuan + "] da ton tai o Khu Vuc khac");
                                }
                                banSao.setMaTram(maTramChuan);
                                // nay la neu ma tram hop le va chua ton tai o khu vuc khac thi moi cap nhat vao khu vuc hien tai
                            }
                        }
                        });

                    *kv = banSao; 
                    // cap nhat doi tuong khu vuc hien tai voi ban sao moi
                    repoKV.update();
                    std::cout << "- Cap nhat thanh cong!\n";
                }
                // xao 
                else if (chon == 5) {
                    std::string inputXoa = Inputhelper::Docstring("Nhap Ma KV can xoa (4 so, VD: 0001) [enter de huy]: ", true);
                    if (inputXoa.empty()) throw 0;

                    std::string id = KhuVuc::taoMaKV(inputXoa);
                    auto kv = repoKV.findById(id); // nay tim xem ma khu vuc da co hay chua neu co thi moi xoa dc
                    if (!kv) throw Ngoailekotimthay("Loi: Khong tim thay ma khu vuc [" + id + "]!");
                    inBang({ kv });

                    int loiThamChieu = 0; // bien dem xem co hop dong thue nao tham chieu toi khu vuc nay khong
                    std::string maTramCuaKV = kv->getMaTram();

                    // Neu khu vuc co Tram, thi moi di quet xem Tram do co dang bi thue khong
                    if (maTramCuaKV != "Chua xac dinh") {
                        for (const auto& hd : repoHopDong.getAll()) {
                            if (hd->getMaTram() == maTramCuaKV) {
                                loiThamChieu++;
                                break;
                            }
                        }
                    }

                    if (loiThamChieu > 0) {
                        throw Ngoailerangbuoc("TU CHOI XOA: Khu vuc nay dang quan ly tram co Hop Dong Thue ,vui long xoa Hop Dong Thue do truoc");
                    }
                    // neu muon xoa thi phai xoa hop dong tham chieu toi khu vuc do truoc
                    std::string xn = Inputhelper::Docstring("Ban co chac chan muon xoa (y/n)? ");
                    if (xn == "y" || xn == "Y") {
                        repoKV.remove(id);
                        repoKV.update();
                        std::cout << "Da xoa khu vuc thanh cong\n";
                    }
                    else {
                        std::cout << "-Da huy thao tac xoa\n";
                    }
                }
                else std::cout << "- Lua chon khong hop le\n";
            }
            catch (int) {
                std::cout << "\n DA HUY THAO TAC VA QUAY LAI MENU CHINH\n";
            }
            catch (const Baseexception& e) { std::cout << "LOI NGHIEP VU: " << e.getMessage() << "\n"; }
            catch (const std::exception& e) { std::cout << "LOI HE THONG: " << e.what() << "\n"; }
        }
    }
};
