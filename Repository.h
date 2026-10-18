#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include "Exceptions.h"
#include "Inputhelper.h"

//lop quan li danh sach va luu tru du lieu
template<class T> //cu phap khai bao 1 lopnkhuon mau voi T la 1 tham so kieu du lieu
class Repository {
private:
    std::vector<std::shared_ptr<T>> dataList; //shared_ptr la con tro trong o nho memory tu quan ly doi tuong bang co che dem so chu so huu
    //khi ko con chu so huu thui tu huy
    std::string filePath; //luu duong dan file du llieu

public:
    //khoi tao dung dan file
    explicit Repository(const std::string& duongDan) : filePath(duongDan) {}

    //lay toan bo danh sach
    const std::vector<std::shared_ptr<T>>& getAll() const { //danh sachs cac con tro thong minh tra ve kieu tham chieu ko sao chep toan bo vecto,ko cho pehp sua cau truc vecto
        return dataList;
    }

    //tiem doi tuong theo ma
    std::shared_ptr<T> findById(const std::string& ma) const { //kieu tra ve 1 con tro quan ly doi tuong kieu T
        std::string idCanTim = Inputhelper::catkhoangtrang(ma);

        for (const auto& item : dataList) { //duyet tung phan tu trong datalist
            if (item && item->getId() == idCanTim) { //conitem phai khac nullptr va xem cai id = id can timk
                return item; // va tra ve neu ma trung khop
            }
        }
        return nullptr;
    }

    //them doi tuong moi
    void add(const std::shared_ptr<T>& item) {
        if (!item) { //neu cotro giong null ptr thi phai ko cho phep khaioi bao
            throw Loixacthuc("Loi: Doi tuong them vao khong hop le!");
        }

        if (item->getId().empty()) { //id rong bao loi
            throw Loixacthuc("Loi: Ma dinh danh khong duoc rong!");
        }

        if (findById(item->getId()) != nullptr) { //neu da co id thi bao loi
            throw Ngoailerangbuoc("Loi: Ma dinh danh da ton tai!");
        }

        dataList.push_back(item);

        try {
            saveToFile();
        }
        catch (...) {
            dataList.pop_back();
            throw;
        }
    }

    void update() { //luu lai doi tuong khi cap nhat doi tuong
        saveToFile();
    }

    //xoa doi tuong theo ma
    void remove(const std::string& ma) {
        std::string idCanXoa = Inputhelper::catkhoangtrang(ma);

        auto it = std::find_if(dataList.begin(), dataList.end(),//duyet cac phan tu va tra ve vi tri dau tien thoan man dieu kien
            [&idCanXoa](const std::shared_ptr<T>& item) {//danh sach bien
                return item && item->getId() == idCanXoa;
            });

        if (it == dataList.end()) {//neu ko tim thay doi tuong phu hop 
            throw Ngoailekotimthay("Loi: Khong tim thay doi tuong de xoa!");
        }

        size_t viTri = static_cast<size_t>(it - dataList.begin());//xuat ra vitri= kc tu dau vec to den vi tri dang tim
        std::shared_ptr<T> itemCu = *it;//lay phan tu ma iterator chi toi

        dataList.erase(it);//xoa phan tu

        try {
            saveToFile();//gia su xoa khoi ram nhung  luu vao file that bai,neu ko khoi phuc ram se thieu trong ram nhung van co the chua
        }
        catch (...) {
            dataList.insert(dataList.begin() + viTri, itemCu);//them itemCu cao vitri chi dinh
            throw;// nem lai ngopai le de goi noi biet xoa chung
        }
    }

    //doc du lieu tu file vao danh sach
    void loadFromFile() {
        std::ifstream file(filePath);//doc du lieu tu file

        if (!file.is_open()) {
            std::ofstream taoFile(filePath, std::ios::app);//app noi tiep vao cuoi file ta su dung ofstream voi che do app de tao 1 file ms khi chua ton tai

            if (!taoFile.is_open()) {
                throw std::runtime_error("Loi: Khong the mo hoac tao file: " + filePath);//tao file that bai bao loi
            }

            taoFile.close();//dong luong file khi su dung

            if (!taoFile) {// neu co loi dc phan anh o trang thai luong ta phat sinh ngoai le
                throw std::runtime_error("Loi: Khong the tao file: " + filePath);
            }

            dataList.clear();//vi dang gia dinh co du lieu trong file nen danh sach duco lam rong
            return;
        }

        std::vector<std::shared_ptr<T>> dsMoi;
        std::string line;//luu noi dung dong hien tai
        int soDong = 0;//dem so dong da toa

        while (std::getline(file, line)) {
            ++soDong;
            line = Inputhelper::catkhoangtrang(line);

            if (line.empty() || line[0] == '#') {//bo qua rong va chu thich
                continue;
            }

            try {
                auto item = std::make_shared<T>();//tao 1 doi tuong kieu T va tar ve shared_ptr<T> quan luy doi tuong do
                item->fromFilerecord(line);//chuyen ban ghi thanh doi tuong

                if (item->getId().empty()) {//kiem tra ma rong
                    throw Loixacthuc("Ma dinh danh rong!");
                }

                bool trungMa = false;

                for (const auto& daDoc : dsMoi) {//duyet cac mang doi tuong da nap thanh cong
                    if (daDoc->getId() == item->getId()) {// so sanh doi uong cu va moi
                        trungMa = true;
                        break;//co roi thi ngat luon vi tim duoc ma trung
                    }
                }

                if (trungMa) {//phat sinh ngoai le ko dua ban ghi vao danh sach
                    throw Ngoailerangbuoc("Ma dinh danh bi trung!");
                }

                dsMoi.push_back(item);//dua doi tuong hop le vao danh sach
            }
            catch (const Baseexception& e) {//bat cac ngoai le tu dinh nghia ke thua baseexception
                std::cerr << "Canh bao dong " << soDong//luong xuat loi chuan
                    << ": " << e.getMessage() << '\n';//thong bao loi
            }
            catch (const std::exception& e) {//bat ngoai le ke thua std::excewption
                std::cerr << "Canh bao dong " << soDong
                    << ": " << e.what() << '\n';//thong baoloi
            }
        }

        if (file.bad()) {//kiem tra loi nghiem tronng cua luong
            throw std::runtime_error("Loi: Doc file that bai: " + filePath);//phat hien duoc sinh ra ngoai le loi
        }

        dataList.swap(dsMoi);//hoan doi 2 vecto danh sach cu va danh sach ms
    }

    // Ghi danh sach xuong file
    void saveToFile() {
        std::string fileTam = filePath + ".tmp";//ghi vao file tram
        std::ofstream file(fileTam, std::ios::trunc);//che do xoa noi dung cu cua file duoc mo thanh cong de ghi lai tu dau

        if (!file.is_open()) {
            throw std::runtime_error("Loi: Khong the mo file tam de ghi: " + fileTam);
        }//neu ko mo duoc file thi phat sinh ngoai le va ko tiep tuc qua trinh

        try {
            file << "#Danh sach doi tuong\n";//day la chu thich loadFromfile nhan dien va bo qua

            for (const auto& item : dataList) {//moi lan duyet tung con tro thong minh trong danh sach
                if (!item) {
                    throw Loixacthuc("Loi: Danh sach co doi tuong rong!");
                }

                file << item->toFilerecord() << '\n';//goi toFilerecord cua doi tuong de ghi vao file

                if (!file) {//neu file loi phat sinh ngoai le
                    throw std::runtime_error("Loi: Ghi file that bai: " + fileTam);
                }
            }

            file.close();//dong luong de thuc hien thao tac ghi

            if (!file) {
                throw std::runtime_error("Loi: Khong the ghi xong file: " + fileTam);
            }
        }
        catch (...) {//neu co loi ta ko muon du lai 
            file.close();
            std::remove(fileTam.c_str());//dc su dung de yec cau xoa file tram c_str()tra ve con tro den ky tu ket thuc ='\0'
            throw;
        }

        // Thu doi ten file tam thanh file chinh
        if (std::rename(fileTam.c_str(), filePath.c_str()) == 0) {//doi ten hoac chueyn 1 file
            return;
        }

        // Du phong cho he thong khong cho ghi de file bang rename
        std::string fileSaoLuu = filePath + ".bak";//mo rong file luu dunlai giu lieu cu trong luc thay the bang du lieu moi
        std::ifstream kiemTraBak(fileSaoLuu);//kiemtra ton taiko

        if (kiemTraBak.good()) {//good tra ve luon true
            std::remove(fileTam.c_str());//neu tra ve thanh cong xoa file tam
            throw std::runtime_error("Loi: Da ton tai file sao luu: " + fileSaoLuu);
        }

        kiemTraBak.close();

        // Chuyen file cu sang file sao luu
        if (std::rename(filePath.c_str(), fileSaoLuu.c_str()) != 0) {// doi ten hoac chuyen 1 file
            std::remove(fileTam.c_str());//xoa duong dan file cu di

            throw std::runtime_error("Loi: Khong the thay the file: " + filePath);
        }

        // Chuyen file tam thanh file chinh
        if (std::rename(fileTam.c_str(), filePath.c_str()) != 0) {
            bool khoiPhuc = (std::rename(fileSaoLuu.c_str(), filePath.c_str()) == 0);
            std::remove(fileTam.c_str());

            if (!khoiPhuc) {
                throw std::runtime_error("Loi: Khong the phuc hoi file, du lieu cu trong: " + fileSaoLuu);
            }

            throw std::runtime_error("Loi: Khong the ghi de file: " + filePath);
        }

        std::remove(fileSaoLuu.c_str());
    }
};
