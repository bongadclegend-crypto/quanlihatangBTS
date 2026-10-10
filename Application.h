#pragma once
#include <iostream>
#include "Repository.h"
#include "InputHelper.h"
#include "TramBTS.h"
#include "HopDongThue.h"
#include "MenuTramBTS.h"
#include "MenuHopDongThue.h"

class Application {
private:
    Repository<TramBTS> repoTramBTS{ "trambts.txt" };
    Repository<HopDongThue> repoHopDong{ "hopdongthue.txt" };

public:
    Application() {
        // Khởi động chỉ nạp dữ liệu của Trạm và Hợp đồng
        repoTramBTS.loadFromFile();
        repoHopDong.loadFromFile();
    }

    void run() {
        while (true) {
            std::cout << "\n============================================\n";
            std::cout << "   HE THONG QUAN LY HA TANG TRAM BTS\n";
            std::cout << "============================================\n";
            std::cout << "1. Quan ly Tram BTS\n";
            std::cout << "2. Quan ly Hop dong thue vi tri\n";
            std::cout << "0. Thoat chuong trinh\n";
            std::cout << "--------------------------------------------\n";

            int chon = Inputhelper::Docint(">> Vui long chon chuc nang: ");

            if (chon == 0) {
                std::cout << "\n[+] Da luu toan bo du lieu an toan. Tam biet!\n";
                break;
            }
            // Đi thẳng vào các Use Case
            else if (chon == 1) {
                MenuTramBTS::run(repoTramBTS);
            }
            else if (chon == 2) {
                MenuHopDongThue::run(repoHopDong, repoTramBTS);
            }
            else {
                std::cout << "[-] Lua chon khong hop le!\n";
            }
        }
    }
};
