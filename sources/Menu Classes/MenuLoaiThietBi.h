#pragma once
#include <iostream>
#include <string>
#include <memory> // Thu vien de dung con tro thong minh (shared_ptr)
#include "Repository.h"
#include "LoaiThietBi.h"
#include "InputHelper.h"
// KHONG include Application.h o day de tranh loi vong lap 39 loi

class MenuLoaiThietBi {
public:
    // Ham run duoc khai bao static, nhan vao kho chua Repository dang tham chieu (&)
    static void run(Repository<LoaiThietBi>& repoLoai) {
        while (true) {
            std::cout << "\n--- QUAN LY DANH MUC LOAI THIET BI ---\n";
            std::cout << "1. Xem danh sach\n2. Them loai moi\n3. Cap nhat\n4. Xoa\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-4): "); // Chan viec nhap chu 
            if (chon == 0) break; // Thoat vong lap de tro ve menu chinh

            try {
                // CHUC NANG 1: XEM (READ)
                if (chon == 1) {
                    std::cout << "\n";
                    repoLoai.loadfromfile(); // Luon nap moi file truoc khi hien thi
                    LoaiThietBi::displayHeader(); // In tieu de cot
                    // Duyet vong lap for each: Lay tung con tro thong minh tu mang ra
                    for (const auto& lt : repoLoai.getAll())
                        lt->display(); // Dung dau -> vi day la con tro (shared_ptr)
                }
                // CHUC NANG 2: THEM (CREATE)
                else if (chon == 2) {
                    std::cout << "\n-- Them Loai Thiet Bi --\n";
                    // Tao vung nho moi tren RAM bang make_shared va truyen luon 5 tham so vao Constructor
                    repoLoai.add(std::make_shared<LoaiThietBi>(
                        Inputhelper::Docstring("Ma Loai (4 so, VD: 0001): "),
                        Inputhelper::Docstring("Ten Loai (VD: Khoi vo tuyen RRU): "),
                        Inputhelper::Docstring("Hang San Xuat (VD: Ericsson): "),
                        Inputhelper::Docstring("Dac tinh KT (VD: 4G LTE B3): "),
                        std::stod(Inputhelper::Docstring("Cong suat tieu thu (W): ")) // ep kieu ve so
                    ));
                    std::cout << "[+] Da them loai thiet bi vao he thong!\n";
                }
                // CHUC NANG 3: SUA (UPDATE)
                else if (chon == 3) {
                    std::cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
                    // Tim kiem xem ma co trong mang khong
                    auto lt = repoLoai.findById(Inputhelper::TuTaoMa(Inputhelper::Docstring("Ma Loai can sua: "), "LT"));
                    if (!lt) throw Loixacthuc("Khong tim thay Ma Loai nay!"); // Khong thay thi nem loi văng ra ngay

                    // Cho nhap tung truong. Tham so 'true' nghia la cho phep nguoi dung an Enter de bo trong
                    std::string ten = Inputhelper::Docstring("Ten moi: ", true);
                    if (!ten.empty()) lt->setTenLoai(ten); // Neu nguoi dung co nhap chu (khong empty) thi moi goi ham set

                    std::string hang = Inputhelper::Docstring("Hang SX moi: ", true);
                    if (!hang.empty()) lt->setHangSanXuat(hang);

                    std::string dacTinh = Inputhelper::Docstring("Dac tinh KT moi: ", true);
                    if (!dacTinh.empty()) lt->setDacTinhKyThuat(dacTinh);

                    std::string csStr = Inputhelper::Docstring("Cong suat (W) moi: ", true);
                    if (!csStr.empty()) lt->setCongSuatTieuThu(std::stod(csStr)); // ep kieu truoc khi set

                    // Do dung con tro thong minh nen du lieu trong mang da tu doi, ta chi can goi saveToFile
                    repoLoai.update();
                    std::cout << "[+] Cap nhat thanh cong!\n";
                }
                // CHUC NANG 4: XOA (DELETE)
                else if (chon == 4) {
                    std::string ma = Inputhelper::TuTaoMa(Inputhelper::Docstring("\nMa Loai can xoa: "), "LT");
                    if (!repoLoai.findById(ma)) throw Loixacthuc("Khong tim thay Ma Loai nay!");

                    std::string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoLoai.remove(ma); // Goi ham xoa trong Repository (trong do co dung find_if de xoa)
                        std::cout << "[+] Da xoa loai thiet bi!\n";
                    }
                }
            }
            catch (const Baseexception& e) {
                // Neu cac ham Set hoac Validator nem (throw) Ngoai le, no se chay vao day de in chuoi loi ra
                std::cout << "[-] Loi: " << e.getMessage() << "\n";
            }
            catch (...) {
                // Bat moi loi khac (VD: loi ep kieu so stod)
                std::cout << "[-] Loi: Vui long nhap dung dinh dang so!\n";
            }
        }
    }
};
