#pragma once
#include<iostream>
#include<iomanip>
#include<vector>
#include"Entity.h"
#include"InputHelper.h"

class LoaiThietBi : public Entity {
private:
	std::string tenLoai;
	std::string donViTinh;
	int soLuong; // BỔ SUNG: Số lượng
	std::string ghiChu;
public:
	LoaiThietBi() = default;

	LoaiThietBi(std::string ma, std::string ten, std::string dvt, int sl, std::string gc) {
		setId(ma);
		setTenLoai(ten);
		setDonViTinh(dvt);
		setSoLuong(sl);
		setGhiChu(gc);
	}

	std::string getTenLoai() const { return tenLoai; }
	void setTenLoai(const std::string& v) {
		if (!Inputhelper::coChuaChuCai(v)) throw Loixacthuc("Loi: Ten phai chua chu cai!");
		tenLoai = Inputhelper::vietHoaChuDau(v);
	}

	std::string getDonViTinh() const { return donViTinh; }
	void setDonViTinh(const std::string& v) {
		if (!Inputhelper::coChuaChuCai(v)) throw Loixacthuc("Loi: Don vi tinh khong hop le!");
		donViTinh = v;
	}

	int getSoLuong() const { return soLuong; }
	void setSoLuong(int v) {
		if (v < 0) throw Loixacthuc("Loi: So luong khong duoc am!");
		soLuong = v;
	}

	std::string getGhiChu() const { return ghiChu; }
	void setGhiChu(const std::string& v) { ghiChu = v; }

	// GHI ĐÈ FILE: Ép kiểu Số Nguyên thành Chuỗi (to_string)
	std::string toFilerecord() const override {
		return id + "|" + tenLoai + "|" + donViTinh + "|" + std::to_string(soLuong) + "|" + ghiChu;
	}

	void fromFilerecord(const std::string& record) override {
		std::vector<std::string> data = Inputhelper::chia(record, '|');
		if (data.size() >= 5) { // Đã tăng lên 5 phần tử
			setId(data[0]);
			tenLoai = data[1];
			donViTinh = data[2];
			soLuong = std::stoi(data[3]); // Ép kiểu Chuỗi về Số Nguyên
			ghiChu = data[4];
		}
	}

	void display() const override {
		std::cout << std::left
			<< std::setw(10) << id << "| "
			<< std::setw(25) << tenLoai << "| "
			<< std::setw(15) << donViTinh << "| "
			<< std::setw(10) << soLuong << "| "
			<< std::setw(20) << ghiChu << std::endl;
	}

	static void displayHeader() {
		std::cout << std::left
			<< std::setw(10) << "Ma Loai TB" << "| "
			<< std::setw(25) << "Ten Loai Thiet Bi" << "| "
			<< std::setw(15) << "Don Vi Tinh" << "| "
			<< std::setw(10) << "So Luong" << "| "
			<< std::setw(20) << "Ghi Chu" << "\n";
		std::cout << std::string(88, '-') << "\n";
	}
};