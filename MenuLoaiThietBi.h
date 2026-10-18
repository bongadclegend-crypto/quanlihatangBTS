// Nguoi thuc hien: Nguyen Anh Tu - B24DCVT368
#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Repository.h"
#include "LoaiThietBi.h"
#include "InputHelper.h"

using namespace std;

class MenuLoaiThietBi {
public:
    static void run(Repository<LoaiThietBi>& repoLoai) {
        while (true) {
            cout << "\n--- QUAN LY DANH MUC LOAI THIET BI ---\n";
            cout << "1. Xem danh sach\n2. Them loai moi\n3. Cap nhat\n4. Xoa\n0. Quay lai\n";

            int chon = Inputhelper::Docint(">> Chon (0-4): ");
            if (chon == 0) break;

            try { 
                if (chon == 1) {
                    cout << "\n";
                    repoLoai.loadFromFile();
                    LoaiThietBi::displayHeader();
                    for (const auto& lt : repoLoai.getAll())
                        lt->display();
                }
                else if (chon == 2) {
                    cout << "\n-- Them Loai Thiet Bi --\n";
                    repoLoai.add(make_shared<LoaiThietBi>(
                        Inputhelper::Docstring("Ma Loai (4 chu so) : "),
                        Inputhelper::Docstring("Ten Loai : "),
                        Inputhelper::Docstring("Hang San Xuat: "),
                        Inputhelper::Docstring("Dac tinh KT: "),
                        stod(Inputhelper::Docstring("Cong suat tieu thu (W): "))
                    ));
                    cout << "[+] Da them loai thiet bi vao he thong!\n";
                }
                else if (chon == 3) {
                    cout << "\n-- Cap Nhat (Nhan Enter de bo qua truong khong sua) --\n";
                    auto lt = repoLoai.findById(Inputhelper::tuTaoMa(Inputhelper::Docstring("Ma Loai can sua: "), "LT"));
                    if (!lt) throw Loixacthuc("Khong tim thay Ma Loai nay!");

                    string ten = Inputhelper::Docstring("Ten moi: ", true);
                    if (!ten.empty()) lt->setTenLoai(ten);

                    string hang = Inputhelper::Docstring("Hang SX moi: ", true);
                    if (!hang.empty()) lt->setHangSanXuat(hang);

                    string dacTinh = Inputhelper::Docstring("Dac tinh KT moi: ", true);
                    if (!dacTinh.empty()) lt->setDacTinhKyThuat(dacTinh);

                    string csStr = Inputhelper::Docstring("Cong suat (W) moi: ", true);
                    if (!csStr.empty()) lt->setCongSuatTieuThu(stod(csStr));

                    repoLoai.update();
                    cout << "[+] Cap nhat thanh cong!\n";
                }
                else if (chon == 4) {
                    string ma = Inputhelper::tuTaoMa(Inputhelper::Docstring("\nMa Loai can xoa: "), "LT");
                    if (!repoLoai.findById(ma)) throw Loixacthuc("Khong tim thay Ma Loai nay!");

                    string xacNhan = Inputhelper::Docstring("Chac chan xoa? (y/n): ");
                    if (xacNhan == "y" || xacNhan == "Y") {
                        repoLoai.remove(ma);
                        cout << "[+] Da xoa loai thiet bi!\n";
                    }
                }
            }
            catch (const Baseexception& e) {
                cout << "[-] Loi: " << e.getMessage() << "\n";
            }
            catch (...) {
                cout << "[-] Loi: Vui long nhap dung dinh dang so!\n";
            }
        }
    }
};