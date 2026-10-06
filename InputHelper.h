#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
class Inputhelper {
	public:
		//ham cat khoang trang thua o hai au chuoi
		static std::string catkhoangtrang(const std::string& str) {
			size_t frist = str.find_first_not_of(' ');
			if (frist == std::string::npos) return "";
			size_t last = str.find_last_not_of(' ');
			return str.substr(frist, last - frist + 1);
		}
		//ham tach chuoi dua tren ky tu phan tach
		static std::vector<std::string> chia(const std::string& str, char delimiter) {
			std::vector<std::string> tokens;
			std::string token;
			
			std::stringstream tokenStream(str);
			while(std::getline(tokenStream,token,delimiter)){
				tokens.push_back(catkhoangtrang(token));
			}
			return tokens;
		}
		//ham yeu cau nguoi dung nhap 1 chuoi an toan tu ban phim
		static std::string Docstring(const std::string& prompt,bool chophepdetrong=false) {
			std::string input;
			while (true) {
				std::cout << prompt;
				std::getline(std::cin,input);
				input = catkhoangtrang(input);
				//neu ko cho phep rong ma ng dung de rong thi bat nhap lai
				if (!chophepdetrong && input.empty()) {
					std::cout << "Loi:ko de trong .Vuilong nhap lai!\n";
				}
				else return input;
				}
			}
		//ham yeu cau nhap 1 so nguyen an toan chong troi lenh
		static int Docint(const std::string& prompt) {
			std::string input;
			while (true) {
				std::cout << prompt;
				std::getline(std::cin, input);
				try {
					//thu chuyen doi chuoi vua nhap thanh so nguyen
					return std::stoi(catkhoangtrang(input));
				}
				catch (...) {
					//neu nguoi dung nhap chu(ko the ep sang kieu so) bao loi va vong lai
					std::cout << "Loi:Vui long nhap so nguyen hop le";
				}
			}
		}
		// 1. Kiểm tra chuỗi có chứa chữ cái không (Chống nhập tên toàn số "111")
		static bool coChuaChuCai(const std::string& str) {
			for (char c : str) {
				if (isalpha(c)) return true; // Có ít nhất 1 chữ cái (a-z, A-Z)
			}
			return false;
		}

		// 2. Viết hoa chữ cái đầu (VD: "ha noi" -> "Ha Noi", "vinh phuc" -> "Vinh Phuc")
		static std::string vietHoaChuDau(std::string str) {
			bool newWord = true;
			for (char& c : str) {
				if (isspace(c)) {
					newWord = true; // Gặp khoảng trắng thì từ tiếp theo là từ mới
				}
				else if (newWord && isalpha(c)) {
					c = toupper(c); // Viết hoa
					newWord = false;
				}
				else {
					c = tolower(c); // Các chữ còn lại viết thường
				}
			}
			return str;
		}

		// 3. Kiểm tra định dạng Số điện thoại (Chỉ chứa số, từ 11 đến 12 số)
		static void kiemTraSdt(const std::string& sdt) {
			// Kiểm tra độ dài: Nếu độ dài nhỏ hơn 11 HOẶC lớn hơn 12 thì báo lỗi
			if (sdt.length() < 11 || sdt.length() > 12) {
				throw Loixacthuc("Loi: So dien thoai phai co tu 11 den 12 chu so!");
			}

			// Kiểm tra từng ký tự xem có bị lẫn chữ cái hay ký tự đặc biệt không
			for (char c : sdt) {
				if (!isdigit(c)) {
					throw Loixacthuc("Loi: So dien thoai chi duoc chua con so!");
				}
			}
		}

};
