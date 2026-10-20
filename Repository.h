//Hoang Tuan Thanh
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include "Exceptions.h"

// Dung template de tai su dung code cho TramBTS, HopDongThue, KhuVuc...
template <class T>//day la cai khuon mau boi vi cac file co the dung chung boi vi cac usecase deu dung chung tim xoa danh sach va them moi neu phat hiennloi sai o ham doc chi can sua o repository la dc
class Repository {
private:
    std::vector<std::shared_ptr<T>> data; // Mang dong chua cac doi tuong va shared ptr de tu dong don rac  tranh lam ro gi ram lam mat du lieu thay bang viec dung mang dong new va delete dee xoa thu cong
    std::string filename;                 // Ten file txt de luu tru(duong dan file txt

public:
    // Constructor truyen vao ten file
    Repository(const std::string& file) {// tao 1 cho trong de nguoi dung nhap vo
        filename = file;
    }

    
    void loadFromFile() {//doc du lieu tu txt
        data.clear(); // xoa data cu tren RAM truoc khi doc moi
        std::ifstream file(filename);//mo file ra doc

        // Neu file chua ton tai thi thoi, khong bao loi
        if (!file.is_open()) {//neu file nay ko the mo dc thi quay lại
            return;
        }

        std::string line;
        while (std::getline(file, line)) {//lay tung dong tren file
            if (line.empty()) continue; // Bo qua dong trong

            auto item = std::make_shared<T>(); // Tao doi tuong moi,auto thay cho std::shared_ptr<T> item cho tung dong
            try {
                item->fromFilerecord(line);    // nap du lieu tu chuoi vao doi tuong
                data.push_back(item);          // day vao mang
            }
            catch (...) {//bat moi loai loi nhu ai nghich lam mat mat 1 cot 1o nao do
              
                continue;//bo qua cai dong bi loi do va nap tiep dong 4
            }
        }
        file.close();
    }

   
    void saveToFile() {
        // ios::trunc se xoa sach noi dung file cu di de ghi file moi
        std::ofstream file(filename, std::ios::trunc);//ifstream de doc thi ofstream la de ghi nho du lieu 

        if (!file.is_open()) {
            throw Baseexception("Loi: Khong the tao hoac mo file de ghi!");
        }

        for (const auto& item : data) {
            file << item->toFilerecord() << std::endl;//ghep cac doi tuong thanh chuoi phan cach = dau|
        }

        file.close();
    }

    //them moi
    void add(std::shared_ptr<T> item) {
        data.push_back(item);
        saveToFile(); // Them xong ghi luon xuong file cho an toan
    }

   //cap nhat
    void update() {
        // Vi cac Menu da sua truc tiep tren doi tuong, 
        // nen o day chi can goi ham ghi lai toan bo mang xuong file
        saveToFile();
    }

  //xoa theo id
    void remove(const std::string& id) {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if ((*it)->getId() == id) {//giai tham chieu de lay gia tri
                data.erase(it); // Xoa phan tu khoi mang
                saveToFile();   // Cap nhat lai file
                return;
            }
        }
        // Duyet het mang ma khong thay thi nem loi
        throw Ngoailekotimthay("Loi: Khong tim thay ID de xoa!");
    }

   //lay toan bo danh sach
    std::vector<std::shared_ptr<T>> getAll() const {
        return data;
    }

    //tim kiem theo id
    std::shared_ptr<T> findById(const std::string& id) const{
        for (const auto& item : data) {

            if (item->getId() == id) {
                return item;
            }
        }
        return nullptr;
    }
};
