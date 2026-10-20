#pragma once
#include<iostream>
#include<iomanip>
#include<vector>
#include"Entity.h"
#include"InputHelper.h"

class NhaCungCap : public Entity {
private:
	std::string tenNhaCungCap;
	std::string soDienThoai;
	std::string diaChi;
public:
	NhaCungCap() = default;

	// SỬA: Bắt buộc gọi Setters để đi qua bộ lọc kiểm tra
	NhaCungCap(std::string ma, std::string ten, std::string sdt, std::string dc) {
		setId(ma);
		setTenNhaCungCap(ten);
		setSoDienThoai(sdt);
		setDiaChi(dc);
	}

	std::string getTenNhaCungCap() const { return tenNhaCungCap; }
	void setTenNhaCungCap(const std::string& v) {
		if (!Inputhelper::coChuaChuCai(v)) throw Loixacthuc("Loi: Ten phai chua it nhat 1 chu cai!");
		tenNhaCungCap = Inputhelper::vietHoaChuDau(v); // Tự động viết hoa
	}

	std::string getSoDienThoai() const { return soDienThoai; }
	void setSoDienThoai(const std::string& v) {
		Inputhelper::kiemTraSdt(v); // Ép kiểu sdt >= 10 số
		soDienThoai = v;
	}

	std::string getDiaChi() const { return diaChi; }
	void setDiaChi(const std::string& v) {
		if (!Inputhelper::coChuaChuCai(v)) throw Loixacthuc("Loi: Dia chi khong hop le!");
		diaChi = Inputhelper::vietHoaChuDau(v); // Tự động viết hoa (vd: hn -> Hn)
	}

	std::string toFilerecord() const override {
		return id + "|" + tenNhaCungCap + "|" + soDienThoai + "|" + diaChi;
	}

	void fromFilerecord(const std::string& record) override {
		std::vector<std::string> data = Inputhelper::chia(record, '|');
		if (data.size() >= 4) {
			setId(data[0]);
			tenNhaCungCap = data[1]; // Đọc từ file thì tin tưởng tuyệt đối, không lọc lại
			soDienThoai = data[2];
			diaChi = data[3];
		}
	}

	void display() const override {
		std::cout << std::left
			<< std::setw(10) << id << "| "
			<< std::setw(25) << tenNhaCungCap << "| "
			<< std::setw(15) << soDienThoai << "| "
			<< std::setw(25) << diaChi << std::endl;
	}

	static void displayHeader() {
		std::cout << std::left
			<< std::setw(10) << "Ma NCC" << "| "
			<< std::setw(25) << "Ten Nha Cung Cap" << "| "
			<< std::setw(15) << "So Dien Thoai" << "| "
			<< std::setw(25) << "Dia Chi" << "\n";
		std::cout << std::string(80, '-') << "\n";
	}
};