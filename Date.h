#pragma once
#include<string>
#include<vector>
#include<stdexcept>
#include "Exceptions.h"
#include"InputHelper.h"
class Date {
private:
	int day;
	int month;
	int year;
	bool namnuan(int y) const {
		return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
	}
	//ham lay so ngay toi da cua 1 thang
	int ngaytoida(int m, int y) const {
		switch (m) {
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			return 31;
		case 4: case 6: case 9: case 11:
			return 30;
		case 2:
			return namnuan(y) ? 29 : 28;
		default:
			throw Ngoailerangbuoc("Loi: Thang khong hop le");
		}
	}
	//kiemtra ngay thang nam co hop le hay k
	bool kiemtraNgay(int d, int m, int y) const {
		if (y < 1900 || y>2100) return false;
		if (m < 1 || m>12) return false;
		if (d<1 || d>ngaytoida(m, y)) return false;
		return true;
	}
public:
	//khoi tao mac dinh (01/01/2000)
	Date() : day(1), month(10), year(2026) {}
	//khoi tao co tham so
	Date(int d, int m, int y) {
		if (!kiemtraNgay(d, m, y)) {
			throw Loixacthuc("Loi: Ngay thang nam ko hop le tren lich!");

		}
		day =d;
		month=m;
		year =y;

	}
	//1.ham doc chuoi tu file hoac ban phim nap vao doi tuong
	void docchuoi(const std:: string& dateStr) {
		//dung chia de cat du lieu tai dau '/'
		std::vector<std::string> parts = Inputhelper::chia(dateStr, '/');
		if (parts.size() != 3) {
			throw Loixacthuc("Loi: Dinh dang ngay phai la dd/mm/yyyy");
			
		}
		try {
			//ep kieu 3 manh chuoi thanh so nguyen
			int d = std::stoi(parts[0]);
			int m = std::stoi(parts[1]);
			int y = std::stoi(parts[2]);
			if (!kiemtraNgay(d, m, y)) {
				throw Loixacthuc("Loi: Ngay thang nam ko hop le tren lich!");

			}
			day = d;
			month = m;
			year = y;
		}
		catch (const std::exception&) {// bat loi neu nguoi dung nhap chu
			throw Loixacthuc("Loi: Ngay thang nam phai la con so !");
		}

	}
	//2 ham xuat doi tuong date ra chuoi de in ra man hin hoac file
	std::string toString()const {
		//neu thu ngay thang be on 10 thi tu dong chen them so 0 o phia trc
		std::string d = (day < 10 ? "0" : "") + std::to_string(day);
		std::string m = (month < 10 ? "0" : "") + std::to_string(month);
		return d+ "/" + m + "/" + std::to_string(year);
	}

};

