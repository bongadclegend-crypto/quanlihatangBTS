#pragma once 
#include <iostream> // Thu vien in an console
#include <string>   // Thu vien chuoi
#include <memory>   // Thu vien chua con tro thong minh std::shared_ptr, std::make_shared
#include "Repository.h" // Ket noi file nay voi Repository de quan ly file
#include "LoaiThietBi.h" // Ket noi de nhan dien doi tuong LoaiThietBi
#include "InputHelper.h" // Dung cac ham nhap xuat chuan hoa
#include "Exceptions.h"  // Dung de bat cac class Ngoai le

class MenuLoaiThietBi { // Lop menu chua cac logic hien thi giao dien
private:
    // Ham Template: Chap nhan 1 kieu F bat ky (o day la Lambda function) de tai su dung logic bat loi
    template <class F>
    static void thulai(F buoc) {
        while (true) { // Mo ra 1 vong lap vo han chi dung lai khi nhap dung
            try {
                buoc(); // Chay khoi lenh duoc truyen vao
                return; // Neu khong co loi nao văng ra thi return de thoat vong lap
            }
            catch (const Baseexception& e) { // Bat cac loi nghiep vu do lap trinh vien quang ra (throw)
                std::cout << "[!] " << e.getMessage() << " -> Moi nhap lai\n"; // In loi va vong lap se tu quay lai tu dau
            }
            catch (const std::exception&) { // Bat cac loi tu he thong C++ (VD nhu ngoai le cua loi ep kieu chu thanh so)
                std::cout << "[!] Gia tri khong hop le -> Moi nhap lai\n"; // In loi va bat nhap lai
            }
        }
    }

public:
    // Ham static de chay menu ma khong can khoi tao MenuLoaiThietBi. Truyen Repository vao bang tham chieu (&) de thao tac truc tiep
    static void run(Repository<LoaiThietBi>& repoLoai) {
        while (true) { // Vong lap menu chinh cua module
            std::cout << "\n--- QUAN LY DANH MUC LOAI THIET BI ---\n"; // In banner giao dien
            std::cout << "1. Xem danh sach\n2. Them loai moi\n3. Cap nhat\n4. Xoa\n0. Quay lai\n"; // In cac lua chon

            int chon = Inputhelper::Docint(">> Chon (0-4): "); // Doc 1 so nguyen, ham nay tu bat loi neu nhap chu
            if (chon == 0) break; // Neu nguoi dung chon 0 thi pha vo vong lap de tro ve menu lon

            try { // Bao boc toan bo cac chuc nang de bat loi chong sup do he thong
                if (chon == 1) { // Neu chon tinh nang Xem danh sach
                    std::cout << "\n";
                    repoLoai.loadfromfile(); // Truoc khi xem, dong bo file text tu HDD len RAM
                    const auto& ds = repoLoai.getAll(); // Lay ve mang vector chua danh sach doi tuong
                    if (ds.empty()) { // Neu mang hien tai khong co gi
                        std::cout << "[-] Danh sach dang trong.\n"; // Thong bao rong
                    }
                    else {
                        LoaiThietBi::displayHeader(); // In dong tieu de cua bang
                        for (const auto& lt : ds) lt->display(); // Su dung vong lap for-each duyet qua tung the hien de in no ra
                        std::cout << "Tong so ban ghi: " << ds.size() << "\n"; // In tong so the hien trong mang
                    }
                }
                else if (chon == 2) { // Neu chon tinh nang Them moi (Create)
                    std::cout << "\n-- Them Loai Thiet Bi --\n";
                    auto lt = std::make_shared<LoaiThietBi>(); // Khoi tao 1 con tro thong minh quan ly the hien rong nam tren heap RAM

                    thulai([&]() { // Truyen vao 1 Lambda function ( [&] nghia la cho phep bat tat ca cac bien ben ngoai theo tham chieu)
                        std::string ma = Inputhelper::Docstring("Ma Loai (4 so, VD: 0001): "); // Yeu cau nhap ma
                        lt->setId(Inputhelper::TuTaoMa(ma, "LT")); // Phat sinh tien to va luu vao object
                        if (repoLoai.findById(lt->getId())) { // Truoc khi luu, tim xem ma nay da ton tai trong Repository chua
                            throw Ngoailerangbuoc("Loi: Ma Loai nay da ton tai!"); // Neu ton tai thi nem loi Primary Key
                        }
                        });

                    // Dung thulai de ep nguoi dung nhap dung tung dong mot, sai o dau bat nhap lai ngay o do
                    thulai([&]() { lt->setTenLoai(Inputhelper::Docstring("Ten Loai : ")); });

                    thulai([&]() { lt->setHangSanXuat(Inputhelper::Docstring("Hang San Xuat : ")); });

                    thulai([&]() { lt->setDacTinhKyThuat(Inputhelper::Docstring("Dac tinh KT: ")); });

                    thulai([&]() { lt->setCongSuatTieuThu(Inputhelper::Docdouble("Cong suat tieu thu (W): ")); });

                    repoLoai.add(lt); // Khi nap du 5 truong hop le, nem the hien vao kho chua (va kho se tu dong luu xuong file txt)
                    std::cout << "[+] Da them loai thiet bi vao he thong!\n"; // Thong bao hoan thanh
                }
                else if (chon == 3) { // Neu chon tinh nang Sua (Update)
                    std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
                    std::string rawId = Inputhelper::Docstring("Ma Loai can sua (4 so): "); // Yeu cau nhap khoa chinh
                    auto lt = repoLoai.findById(Inputhelper::TuTaoMa(rawId, "LT")); // Truy van khoa chinh trong mang RAM
                    if (!lt) throw Ngoailekotimthay("Khong tim thay Ma Loai nay!"); // Neu con tro null nghia la ko tim thay -> văng loi

                    LoaiThietBi banSao = *lt; // Giai tham chieu (*) de lay du lieu tao ra 1 ban sao. De khi sua loi, du lieu goc tren RAM ko bi lam hong

                    thulai([&]() { // Bat dau sua Ten
                        std::string ten = Inputhelper::Docstring("Ten moi (" + banSao.getTenLoai() + "): ", true); // Tham so 'true' cho phep an Enter de bo qua (rong)
                        if (!ten.empty()) banSao.setTenLoai(ten); // Neu nguoi dung co nhap, thi goi setter xac thuc roi nap vao ban sao
                        });

                    thulai([&]() { // Bat dau sua Hang san xuat
                        std::string hang = Inputhelper::Docstring("Hang SX moi (" + banSao.getHangSanXuat() + "): ", true);
                        if (!hang.empty()) banSao.setHangSanXuat(hang); // Kiem tra va cap nhat ban sao
                        });

                    thulai([&]() { // Bat dau sua Dac tinh KT
                        std::string dacTinh = Inputhelper::Docstring("Dac tinh KT moi (" + banSao.getDacTinhKyThuat() + "): ", true);
                        if (!dacTinh.empty()) banSao.setDacTinhKyThuat(dacTinh); // Kiem tra va cap nhat ban sao
                        });

                    thulai([&]() { // Bat dau sua Cong suat
                        double csMoi = banSao.getCongSuatTieuThu(); // Bien nay de hung du lieu moi neu nguoi dung nhap vao
                        // DocdoubleCapNhat tra ve true neu co nhap so, tra ve false neu an Enter
                        if (Inputhelper::DocdoubleCapNhat("Cong suat moi (" + std::to_string(csMoi) + " W): ", csMoi)) {
                            banSao.setCongSuatTieuThu(csMoi); // Cap nhat ban sao neu co du lieu
                        }
                        });

                    *lt = banSao; // Chep đè du lieu tu ban sao (da xac thuc an toan) tro lai con tro goc nam trong Repository
                    repoLoai.update(); // Yeu cau Repository dump toan bo mang tu RAM xuong file txt de cap nhat
                    std::cout << "[+] Cap nhat thanh cong!\n"; // Thong bao hoan tat
                }
                else if (chon == 4) { // Neu chon tinh nang Xoa (Delete)
                    std::string rawId = Inputhelper::Docstring("\nMa Loai can xoa (4 so): "); // Nhap ma
                    std::string ma = Inputhelper::TuTaoMa(rawId, "LT"); // Chuan hoa them tien to
                    if (!repoLoai.findById(ma)) throw Ngoailekotimthay("Khong tim thay Ma Loai nay!"); // Xac thuc ton tai

                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): "); // Yeu cau Double-check truoc khi xoa
                    if (xacNhan == "y" || xacNhan == "Y") { // Chap nhan
                        repoLoai.remove(ma); // Goi phuong thuc remove tren Repository (No se tu quet, xoa the hien va cap nhat lai file)
                        std::cout << "[+] Da xoa loai thiet bi!\n"; // Thong bao thanh cong
                    }
                }
            }
            catch (const Baseexception& e) { std::cout << "[-] Loi Nghiep Vu: " << e.getMessage() << "\n"; } // Bat ngoai le Baseexception in ra bao loi
            catch (const std::exception& e) { std::cout << "[-] Loi He Thong: " << e.what() << "\n"; } // Bat loi logic toan he thong
        }
    }
};
