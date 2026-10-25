
//Hoang Tuan Thanh
#pragma once
#include <iostream>
#include "Repository.h"
#include "InputHelper.h"
#include "TramBTS.h"
#include "HopDongThue.h"
#include "ThietBiLapDat.h"
#include"LoaiThietBi.h"
#include "MenuTramBTS.h"
#include "MenuHopDongThue.h"
#include "MenuThietBiLapDat.h"
#include "MenuLoaiThietBi.h"
#include "KhuVuc.h"
#include "Anten.h"
#include "MenuKhuVuc.h"
#include "MenuAnten.h"
#include "KyThuatVien.h"
#include "SuCoTram.h"
#include "MenuKyThuatVien.h"
#include "MenuSuCoTram.h"
#include"PhieuBaoTri.h"
#include"NhaCungCap.h"
#include"MenuPhieuBaoTri.h"
#include"MenuNhaCungCap.h"



class Application {
private:
    Repository<TramBTS> repoTramBTS{ "trambts.txt" };
    Repository<HopDongThue> repoHopDong{ "hopdongthue.txt" };
    Repository<ThietBiLapDat> repoThietBiLapDat{ "thietbilapdat.txt" };
    Repository<LoaiThietBi> repoLoaiThietBi{ "loaithietbi.txt" };
    Repository<KhuVuc>repoKhuVuc{ "KhuVuc.txt" };
    Repository<Anten>repoAnten{ "Anten.txt" };
    Repository<SuCoTram>repoSC{ "SuCoTram.txt" };
    Repository<KyThuatVien>repoKTV{ "KyThuatVien.txt" };
    Repository<NhaCungCap>repoNCC{ "NhaCungCap.txt" };
    Repository<PhieuBaoTri>repoPBT{ "PhieuBaoTri.txt" };

public:
    Application() {
        // Khởi động chỉ nạp dữ liệu của Trạm và Hợp đồng
        repoTramBTS.loadfromfile();
        repoHopDong.loadfromfile();
        repoThietBiLapDat.loadfromfile();
        repoLoaiThietBi.loadfromfile();
        repoKhuVuc.loadfromfile();
        repoAnten.loadfromfile();
        repoSC.loadfromfile();
        repoKTV.loadfromfile();
        repoNCC.loadfromfile();
        repoPBT.loadfromfile();

    }

    void run() {
        while (true) {
            std::cout << "\n============================================\n";
            std::cout << "   HE THONG QUAN LY HA TANG TRAM BTS\n";
            std::cout << "============================================\n";
            std::cout << "1. Quan ly Tram BTS\n";
            std::cout << "2. Quan ly Hop dong thue vi tri\n";
            std::cout << "3. Quan ly Loai thiet bi\n";
            std::cout << "4. Quan ly Thiet bi lap dat\n";
            std::cout << "5. Quan ly Khu Vuc / Tinh\n";
            std::cout << "6. Quan ly Anten va Cau hinh phat\n";
            std::cout << "7. Ky Thuat Vien\n";
            std::cout << "8. Su co Tram\n";
            std::cout << "9. Nha Cung Cap\n";
            std::cout << "10.Phieu Bao Tri\n";
            std::cout << "0. Thoat chuong trinh\n";
            std::cout << "--------------------------------------------\n";

            int chon = Inputhelper::Docint(">> Vui long chon chuc nang: ");

            if (chon == 0) {
                std::cout << "\n[+] Da luu toan bo du lieu an toan. Tam biet!\n";
                break;
            }
            // Đi thẳng vào các Use Case
            else if (chon == 1) {
                MenuTramBTS::run(repoTramBTS, repoAnten, repoKhuVuc, repoSC, repoThietBiLapDat, repoHopDong);
            }
            else if (chon == 2) {
                MenuHopDongThue::run(repoHopDong, repoTramBTS,repoKhuVuc);
            }
            else if (chon == 3) {
                MenuLoaiThietBi::run(repoLoaiThietBi,repoNCC);

            }
            else if (chon == 4) {
                MenuThietBiLapDat::run(repoThietBiLapDat, repoLoaiThietBi, repoTramBTS);
            }
            else if (chon == 5) {
                MenuKhuVuc::run(repoKhuVuc, repoTramBTS,repoHopDong);
            }
            else if (chon == 6) {
                MenuAnten::run(repoAnten, repoTramBTS);
            }
            else if (chon == 7) {
                MenuKyThuatVien::run(repoKTV);

            }
            else if (chon == 8) {
                MenuSuCoTram::run(repoSC, repoTramBTS, repoKTV);
            }
            else if (chon == 9) {
                MenuNhaCungCap::run(repoNCC, repoPBT,repoLoaiThietBi);
            }
            else if (chon == 10) {
                MenuPhieuBaoTri::run(repoPBT, repoTramBTS, repoNCC, repoThietBiLapDat,repoLoaiThietBi);
            }
            else {
                std::cout << "[-] Lua chon khong hop le!\n";
            }
        }
    }
};
