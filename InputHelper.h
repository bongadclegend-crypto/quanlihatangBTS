//Hoàng Tuấn Thành 
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include "Exceptions.h"

class Inputhelper {
public:
    // cat ky tu trang o dau va cuoi chuoi
    static std::string catkhoangtrang(const std::string& str) {
        std::size_t first = str.find_first_not_of(' ');

        if (first == std::string::npos) {
            return "";
        }

        std::size_t last = str.find_last_not_of(' ');

        return str.substr(first, last - first + 1);
    }

    // tach chuoi theo ky tu phan cach
    static std::vector<std::string> chia(
        const std::string& str, char delimiter) {

        std::vector<std::string> tokens;
        std::string token;
        std::stringstream ss(str);

        while (std::getline(ss, token, delimiter)) {//cha tren luong ss cu den cho delimiter thi dung lai nap vào cái bien token
            tokens.push_back(catkhoangtrang(token));
        }

        // Giu lai truong rong neu cuoi chuoi la dau phan cach
        if (!str.empty() && str.back() == delimiter) {//gia su co 3 thuoc tinh ten|khu vuc |nguoi quan li neu chi cap nhat 2 o thoi va ko cap nhat o con lai no se lam mat du lieu o do
            tokens.push_back("");//de boi them 1 khoang rong o cuoi luc nay no sew co 3 phan tu pt nguoi quan li duoc giu nguyuen ko can cap nhat
        }

        return tokens;
    }
 // nhap chuoi tu ban phim
    static std::string Docstring(const std::string& prompt,bool chophepdetrong = false) {//no gom co chuoi lenh va 1 cai co cho phep de trong mac dinh la false
std::string input;
while (true) {//vong lap chi co the thoat neu nhap dung quy tac
            std::cout << prompt;

            if (!std::getline(std::cin, input)) {//chi co the nhap neu thuc hien hanh dong khac thi nem ngoai le va thoat chuong trinh
                throw std::runtime_error("Khong the doc du lieu");//ngoai le doc ghi file bi hong
            }
input = catkhoangtrang(input);

            if (!chophepdetrong && input.empty()) {
                std::cout << "Loi: Khong duoc de trong! Vui long nhap lai"<<std::endl;
            }
            else {
                return input;
            }
        }
    }

    // nhap so nguyen, nhap sai thi nhap lai
    static int Docint(const std::string& prompt) {//const & giup chuong trinh ko phai copy loai chuoi trong ram
        while (true) {//vong lap vo han ket thuc dc o return
            std::string input = Docstring(prompt, true);//true thi ko bat loi voi ma de di qua va try_catch o ben duoi

            try {
                std::size_t pos = 0;//luu do dai chuoi
                int so = std::stoi(input, &pos);//chuyen doi so va chuyen do dai so vo pos

                if (pos == input.length()) {//neu pos= chieu dai chuoi ban dau
                    return so; //thi tra ve so
                }
            }
            catch (const std::invalid_argument& e) {//sai khai bao ngoai le
                // Chuoi khong chuyen duoc thanh so nguyen
            }
            catch (const std::out_of_range& e) {
                // So vuot pham vi cua int
            }

            std::cout << "Loi: Vui long nhap so nguyen hop le" << std::endl;;
        }
    }

    // kiem tra chuoi co it nhat mot chu cai
    static bool coChuaChuCai(const std::string& str) {
        for (unsigned char c : str) {
            if (std::isalpha(c)) {
                return true;
            }
        }

        return false;
    }

    // viet hoa chu cai dau moi tu
    static std:: string vietHoaChuDau(std::string  str) {
        bool dauTu = true;

        for (char& c : str) { // Da bo unsigned char
            if (isspace(c)) {
                dauTu = true;
            }
            else {
                if (dauTu && isalpha(c)) {
                    c = toupper(c);
                }
                else {
                    c = tolower(c);
                }
                dauTu = false;
            }
        }
        return str;
    }

    // Kiem tra so dien thoai co tu 10 den 12 chu so
    static void kiemTraSdt(const std::string& sdt) {
        if (sdt.length() < 10 || sdt.length() > 12) {
            throw Loixacthuc(
                "Loi: So dien thoai phai co tu 10 den 12 chu so"
            );
        }

        for (unsigned char c : sdt) {
            if (!std::isdigit(c)) {
                throw Loixacthuc(
                    "Loi: So dien thoai chi duoc chua chu so"
                );
            }
        }
    }

    // Ghep tien to voi phan ma gom 4 chu so
    static std::string tuTaoMa( const std::string& maNhap,const std::string& tienTo) {

        std::string clean = catkhoangtrang(maNhap);
        std::string phanSo = clean;

        if (clean.substr(0, tienTo.length()) == tienTo) {
            phanSo = clean.substr(tienTo.length());
        }

        if (phanSo.length() != 4) {
            throw Loixacthuc("Loi: Ma phai co dung 4 chu so");
        }

        for (unsigned char c : phanSo) {
            if (!std::isdigit(c)) {
                throw Loixacthuc(
                    "Loi: Phan so cua ma chi duoc chua chu so"
                );
            }
        }

        return tienTo + phanSo;
    }

    // Nhap so thuc(giong cai nhap so nguyen thoi)
    static double Docdouble(const std::string& prompt) {
        while (true) {
            std::string input = Docstring(prompt, true);

            try {
                std::size_t pos = 0;
                double so = std::stod(input, &pos);

                if (pos == input.length() && std::isfinite(so)) {
                    return so;
                }
            }
            catch (const std::invalid_argument&) {
                // Chuoi khong chuyen duoc thanh so thuc
            }
            catch (const std::out_of_range&) {
                // So vuot pham vi chuyen doi
            }

            std::cout << "Loi: Vui long nhap so thuc hop le.\n";
        }
    }

    //cai nay thi giong cai doc songuyen va dung de ghep voi tien
    static bool DocdoubleCapNhat(const std::string& prompt, double& giaTri) {

        while (true) {
            std::string input = Docstring(prompt, true);

            if (input.empty()) {
                return false;
            }

            try {
                std::size_t pos = 0;
                double so = std::stod(input, &pos);

                if (pos == input.length() && std::isfinite(so)) {//infinitive so nay co the la 1 so vo han hoac ko phai so
                    giaTri = so;
                    return true;
                }
            }
            catch (const std::invalid_argument&) {
                // Chuoi khong chuyen duoc thanh so thuc
            }
            catch (const std::out_of_range&) {
                // So vuot pham vi chuyen doi
            }

            std::cout << "Loi: Vui long nhap so thuc hop le.\n";
        }
    }

    // Kiem tra chuoi co chu cai va chi gom chu cai, ky tu trang
    static bool chiChuaChuCaiVaKhoangTrang(const std::string& str) {
        if (!coChuaChuCai(str)) {
            return false;
        }

        for (unsigned char c : str) {
            if (!std::isalpha(c) && !std::isspace(c)) {
                return false;
            }
        }

        return true;
    }
};//dung cac bien static cho biet ham nay thuoc ve lop inputhelper chu ko thuoc ve 1 doi tuong cu the nao nho vay co the goi truc tiep ma ko can dung lenh neww de =tao doi tuong
//try{
//doan code co the ra nhieu loinkac nhau
//}
//catch{bat rieng loi cu the}
//catch(...){ bat moi loai ngoai le chx dc khai bao o tren
