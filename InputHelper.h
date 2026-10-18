#pragma once
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
	//cat ca khoantrang o dau va cuoi
	static std::string catkhoangtrang(const std::string& str) {
		size_t first = str.find_first_not_of(' ');//cai vitri dau tien trong chuoi ko phai dau cach
		if (first == std::string::npos) return"";//neu chuoi toan dau cach hoac ko co j tra ve chuoi rong
		size_t last = str.find_last_not_of(' ');//tim ki tu cuoi cung ko phai dau cach
		return str.substr(first, last - first + 1);//ham substr(vitri ,soluong) lay 1 phan cua chuoi
	}
	//tach chuoi theo ky hieu phan cach
	static std::vector<std::string> chia(const std::string& str, char delimiter) {
		std::vector<std::string> tokens;//khai bao 1 cai vec to ki hieu rong
		std::string token;//1 cai token tam thoi de truyen ki hieu vao do
		std::stringstream ss(str);//hamf cho thao tac voi chuoi str nap chuoi str vaof ss
		while (std::getline(ss, token, delimiter)) {//doc du lieu tu luong ss gap dau| thi bo qua ko ghi vao token
			tokens.push_back(catkhoangtrang(token));//day du lieu vao vec to tokens 
		}
		if (!str.empty() && str.back() == delimiter) tokens.push_back("");//getline mac dinh bo qua dau phan cac o vitri cuoi cung kiem tra phai rong ko neu ko mafnguoi dung co tyinh de trong thi cho khoang trong do vao

		return tokens;//tra ve kq
	}
	//nhap chuoi
	static std::string Docstring(const std::string& prompt, bool chophepdetrong = false) {
		std::string input;
		while (true) {//vong lkap von tan chi khi nguoi dung nhap dulieu dung
			std::cout << prompt;//loi nhan huong dan
			if (!std::getline(std::cin, input)) {//doc tosan bo du lieu nguoii duyng viet ra getline kiemtra  xem luong nhap co gap loi dan den bi dong ko neu co nem ra 1 ngoai le
				throw std::runtime_error("khong the doc du lieu!");
			}
			input = catkhoangtrang(input);
			if (!chophepdetrong && input.empty()) {//neu ko cho phep de trong ma de trong thi bao loi
				std::cout << "Loi:ko duoc de trong! Vui long nhap lai!\n";
			}
			else return input;

		}
	}
	//nhap so nguyen
	static int Docint(const std::string& prompt) {
		while (true) {//vong lap vo han nhap dung so nguyen moi duoc thoat ra
			std::string input = Docstring(prompt, true);//goi ham docstring de lay du lieu tu ban phim cho phep de trong  de docint tu quan li va xu li chuoi rong = try=catch
			try {
				size_t pos = 0;
				int so = std::stoi(input, &pos);//chuyen input thanh sonnguyen va pos de luu vi tri dau tien ko phai so trong day
				if (pos == input.length()) return so;//neu vi tri do bang dung chieu dai chuoi thi se tra ve so
			}
			catch (const std::invalid_argument&) {//bat loi neu so ko phai so nguyen vi du ko chuyen doi duoc thanh so ngueyn
			}
			catch (const std::out_of_range&) {//bat loi neu so nhap vao vuot ngoai pham  vi int

			}
			std::cout << "Loi:Vui long nhap so nguyen hop le!\n";//neu o doan try ko return ddc thi cout ra cai nay
		}
	}
	//keimtra co it nhat 1 chu cai
	static bool coChuaChuCai(const std::string& str) {
		for (unsigned char c : str) {
			if (std::isalpha(c)) {
				return true;
			}
		}
		return false;
	}
	//viet hoa chu cai dau moi tu
	static std::string vietHoaChuDau(std::string str) {
		bool dauTu = true;//su dung bien co ,khi dauTu=0 chuong trinh se hieu kytu do la bat dau 1 tu
		for (char& c : str) {//duyet ki tung ki tu trong xau co tham chieu de thay doi truc tep
			unsigned char ch = static_cast<signed char>(c);//ep kieu sang usnsinged char  jup tranh loi
			if (std::isspace(ch)) {//kiem tra xem ky tu co phai khoang trang hay xuong dong k
				dauTu = true;//neu dung thi ki tu tiep theo la dautu

			}
			else {
				if (dauTu && std::isalpha(ch)) {
					c = static_cast<char>(std::toupper(ch));//neu day la dau tu va la 1 chu cai thi doi no thanh chu hoa
				}
				else {
					c = static_cast<char>(std::tolower(ch));//ko phai thi chuyen thanh chu thuong

				}
				dauTu = false;//dat co ve ve false
			}
		}
		return str;// trar ve str
	}
	//kiemtra sdt tu 10 den 12 so
	static void kiemTraSdt(const std::string& sdt) {
		if (sdt.length() < 10 || sdt.length() > 12) {
			throw Loixacthuc("Loi:So dien thoai phai co tu 10den 12 chu so!");
		}
		for (unsigned char c : sdt) {
			if (!std::isdigit(c)) throw Loixacthuc("Loi:So dien thoai chi duoc la chu so!");
		}
	}
	//ghep tien to vao ma
	static std::string tuTaoMa(const std::string& maNhap, const std::string& tienTo) {
		std::string clean = catkhoangtrang(maNhap);
		std::string phanSo = clean;
		if (clean.substr(0, tienTo.length()) == tienTo) {//kiemtra doan dau co trung vs ma ko
			phanSo = clean.substr(tienTo.length());//neu nguoi dung da nhap tien to thi cat trine to di giu lai so thoi
		}
		if (phanSo.length() != 4) {
			throw Loixacthuc("Loi: Ma phai co dunbg 4 chu so!");//chi duoc 4 chu so
		}
		for (unsigned char c : phanSo) {
			if (!std::isdigit(c)) {
				throw Loixacthuc("Loi:Ma chi duoc chua chu so!");//chi nhap so kitu khac se sai
			}
		}
		return tienTo + phanSo;
	}
	// Nhap so thuc thong thuong de nhap gia tien
	static double Docdouble(const std::string& prompt) {
		while (true) {
			std::string input = Docstring(prompt, true);

			try {
				size_t pos = 0;
				double so = std::stod(input, &pos);

				if (pos == input.length() && std::isfinite(so)) {
					return so;
				}
			}
			catch (const std::invalid_argument&) {
			}
			catch (const std::out_of_range&) {
			}

			std::cout << "Loi: Vui long nhap so thuc hop le!\n";
		}
	}
	// Nhap so thuc khi cap nhat
	// Enter de giu nguyen gia tri cu
	static bool DocdoubleCapNhat(const std::string& prompt,
		double& giaTri) {
		while (true) {
			std::string input = Docstring(prompt, true);

			if (input.empty()) {
				return false;
			}

			try {
				size_t pos = 0;
				double so = std::stod(input, &pos);

				if (pos == input.length() && std::isfinite(so)) {
					giaTri = so;
					return true;
				}
			}
			catch (const std::invalid_argument&) {
			}
			catch (const std::out_of_range&) {
			}

			std::cout << "Loi: Vui long nhap so thuc hop le!\n";
		}
	}
	// Kiem tra chi co chu cai ASCII va khoang trang
	static bool chiChuaChuCaiVaKhoangTrang(const std::string& str) {
		if (catkhoangtrang(str).empty()) {
			return false;
		}

		for (unsigned char c : str) {
			if (!std::isalpha(c) && !std::isspace(c)) {
				return false;
			}
		}

		return true;
	}
};
