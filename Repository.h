#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <memory>
#include "Exceptions.h"
#include "InputHelper.h"
//su dung template de lop nay co the dung chung cho moi doi tuong T
template<class T>
class Repository {
private:
	std::vector<std::shared_ptr<T>> dataList;// mang dong chua danh sach cac doi tuong
	std::string filePath;//duong dan den fiel txt
public:
	//ham khoi tao nhan vao duong dan den file txt
	Repository(const std::string& filePath) : filePath(filePath) {}
	//1 ham lay toan bo danh sach
	const std::vector<std::shared_ptr<T>>& getAll() const {
		return dataList;
	}
	//2 ham timkiem doi tuong theo ma Id
	std::shared_ptr<T> findById(const std::string& id) const {
		for (const auto& item : dataList) {
			if (item->getId() == id) {
				return item;
			}
		}
		return nullptr;//ko tim thay thi tra ve null
	}
	//3 ham them 1 doi tuong
	void add(std::shared_ptr<T> item) {
		// kiemtra xem ma id bi trung chua
		if (findById(item->getId()) != nullptr){
			throw Ngoailerangbuoc("Loi: Ma dinh danh da ton tai");
	}
		dataList.push_back(item);//them vao mang
		saveToFile();//luu vao file txt
}
	//4 ham cap nhat(sua)
	void update() {
		//vi doi tuong duoc sua truc tiep qua cho con tro ta ch can luu de mang
		saveToFile();
	}
	//5. ham xoa doi tuong theo ma id
	void remove(const std::string& id) {
		// dung thuat toan tim kiem c++ de xoa doi tuong theo ma id
		auto it = std::find_if(dataList.begin(), dataList.end(), [&id](const std::shared_ptr<T>& item) {return item->getId() == id;});
		if (it != dataList.end()) {
			dataList.erase(it);//xoa khoi mang
			saveToFile();
		}
		else {
			throw Ngoailerangbuoc("Loi: Khong tim thay doi tuong de xoa");
		}
	}
	//6 ham doc du lieu tu file txt len ram
	void loadFromFile() {
		std::ifstream file(filePath);
		if (!file.is_open()) return;//neu file chua ton tai thi bo qua
		std::string line;
		while (std::getline(file, line)) {
			line = Inputhelper::catkhoangtrang(line);
			if (line.empty() || line[0] == '#') continue;//bo qua don trong hoac chu thich
			try {
				//tao 1 doi tuyong con rong
				std::shared_ptr<T> item = std::make_shared<T>();
				//goi ham ao doi tuong cat chuoi va nap du lieu
				item->fromFilerecord(line);
				dataList.push_back(item);
			}
			catch (...) {
				//neu 1 dong bi loi dinh dang thi bo qua,doc tiep dong sau
			}
		}
		file.close();
	}
	//7 ham luu du lieu tu ram xuong file txt
	void saveToFile() {
		//mo che do trunc(xoa phan trang file cu de ghi lai file moi)
		std::ofstream file(filePath, std::ios::trunc);
		if (!file.is_open()) {
			throw std::runtime_error("Loi: khong the mo file de ghi: " + filePath);
		}
		file<<"#Danh sach doi tuong\n";
		for (const auto& item : dataList) {
			//goi ham ao de doi tuong tu nen thanh chuoi roi ghi lai thanh file
			file << item->toFilerecord() << "\n";
		}
		file.close();
	}

};