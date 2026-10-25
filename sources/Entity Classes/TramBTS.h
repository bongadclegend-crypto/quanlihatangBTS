//Hoang Tuan Thanh
#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <algorithm> // Thu vien ho tro kiem tra mang
#include "Entity.h"
#include "Inputhelper.h"

class TramBTS : public Entity {
private:
    std::string tenTram;
    std::string loaiCot;       // cot tu dung, day neo, nguy trang... (Chi dc 5 loai)
    double banKinhPhuSong;     // don vi km
    std::string trangThai;     // Hoat dong, Dung hoat dong

    //ham kiem tra va chuan hoa ma tram BTS
    static std::string chuanHoaMaBTS(const std::string& ma) {
        std::string machuan = Inputhelper::TuTaoMa(ma, "BTS");

        if (machuan.size() != 7 || machuan.substr(0, 3) != "BTS") {//ma chuan phai dai du 7 ki tu BTS va 4 chu so
            throw Loixacthuc("Loi: Ma tram phai co dang BTS + 4 chu so");
        }

        for (std::size_t i = 3; i < machuan.size(); i++) {//tuy vi tri so 3 neu co xuat hien ki tu khac ngoai so bao loi luon 
            if (machuan[i] < '0' || machuan[i] > '9') {
                throw Loixacthuc("Loi: Ma tram phai co 4 chu so sau BTS");
            }
        }

        return machuan;
    }

public:
    // constructor mac dinh giup doc file
    TramBTS() : banKinhPhuSong(0.0) {}

    // constructor ham khoi tao co so 
    TramBTS(std::string ma, std::string ten, std::string loai,
        double bankinh, std::string tt) {

        setId(chuanHoaMaBTS(ma));
        setTenTram(ten);
        setLoaiCot(loai);
        setBanKinhPhuSong(bankinh);
        setTrangThai(tt);
    }

    // getter setter lay du lieu va dat du lieu
    std::string getTenTram() const {
        return tenTram;
    }

    void setTenTram(const std::string& ten) {
        std::string clean = Inputhelper::catkhoangtrang(ten);

        if (clean.empty()) {//neu chi co dau cach bao sai luon
            throw Loixacthuc("Loi: Ten Tram BTS khong duoc de trong");
        }

        //ten tram phai co chu cai
        if (!Inputhelper::CoChuaChuCai(clean)) {
            throw Loixacthuc("Loi: Ten Tram phai chua it nhat 1 chu cai");
        }

        if (clean.find('|') != std::string::npos) {//neu tim thay cai dau| thi bao loi
            throw Loixacthuc("Loi: Ten tram khong duoc chua ky tu | ");
        }

        // tu dong viet hoa chu cai dau
        tenTram = Inputhelper::VietHoaChuDau(clean);
    }

    std::string getLoaiCot() const {
        return loaiCot;
    }

    void setLoaiCot(const std::string& loai) {
        std::string clean = Inputhelper::catkhoangtrang(loai);

        if (clean.empty()) {
            throw Loixacthuc("Loi: Loai cot khong duoc de trong");
        }

        if (!Inputhelper::chiChuaChuCaiVaKhoangTrang(clean)) {//chi co chu thoi
            throw Loixacthuc("Loi: Loai cot chi duoc phep chua chu cai va khoang trang");
        }

        // viet hoa de chuan hoa truoc khi so sanh
        std::string loaiDaChuanHoa = Inputhelper::VietHoaChuDau(clean);

        // rang buoc chi co 5 loai cot
        std::vector<std::string> danhSachCot = {
            "Cot Tu Dung", "Cot Day Co", "Cot Nguy Trang", "Cot Than Ong", "Cot Tren Mai"
        };

        // cot nhap vao co dung 5 loai ko
        bool hople = false;
        for (const std::string& cotChuam : danhSachCot) {
            if (loaiDaChuanHoa == cotChuam) {
                hople = true;
                break;
            }
        }

        if (!hople) {
            throw Loixacthuc("Loi: Loai cot chi duoc phep la 1 trong 5 loai: Cot Tu Dung, Cot Day Co, Cot Nguy Trang, Cot Than Ong, Cot Tren Mai");
        }

        loaiCot = loaiDaChuanHoa;
    }

    double getBanKinhPhuSong() const {
        return banKinhPhuSong;
    }

    void setBanKinhPhuSong(double bankinh) {
        if (!std::isfinite(bankinh) || bankinh <= 0.0) {//neu ban kinh phu song la vo cuc hay nho hon 0 bao loi
            throw Loixacthuc("Loi: Ban kinh phu song phai la so duong hop le");
        }

        banKinhPhuSong = bankinh;
    }

    std::string getTrangThai() const {
        return trangThai;
    }

    void setTrangThai(const std::string& tt) {
        std::string clean = Inputhelper::catkhoangtrang(tt);

        if (clean.empty()) {
            throw Loixacthuc("Loi: Trang thai khong duoc de trong");
        }

        if (clean.find('|') != std::string::npos) {
            throw Loixacthuc("Loi: Trang thai khong duoc chua ky tu | ");
        }

        //viet hoa chu cai dau de chuan hoa so sanh
        std::string ttDaChuanHoa = Inputhelper::VietHoaChuDau(clean);

        // rang buoc 2 tyrang thai
        if (ttDaChuanHoa != "Hoat Dong" && ttDaChuanHoa != "Dung Hoat Dong") {
            throw Loixacthuc("Loi: Trang thai chi duoc nhap 'Hoat Dong' hoac 'Dung Hoat Dong'");
        }

        trangThai = ttDaChuanHoa;
    }

    // cap nhat doc va ghi file
    std::string tofilerecord() const override {//chi doc override doi chieu va bat loi luc bien dich
        return id + "|" + tenTram + "|" + loaiCot + "|"
            + std::to_string(banKinhPhuSong) + "|" + trangThai;//chuyen ban kinh ve ki tu vi ko the thuc hien toan tu cong chuoi voi csai
    }

    void fromfilerecord(const std::string& record) override {
        std::vector<std::string> data = Inputhelper::chia(record, '|');//chia cac doan co chuadau |

        if (data.size() != 5) {//neu ko du 5 cai thi bao loi
            throw Loixacthuc("Loi: Ban ghi tram BTS phai co dung 5 truong");
        }

        double banKinh;
        std::size_t vitricuoi = 0;

        try {
            banKinh = std::stod(data[3], &vitricuoi);//chuyen doi du lieu tu o data thu3  va luu cai vi tri cuoi cung doc dc
        }
        catch (const std::invalid_argument&) {
            throw Loixacthuc("Loi: Ban kinh phu song phai la so");
        }
        catch (const std::out_of_range&) {
            throw Loixacthuc("Loi: Ban kinh phu song vuot qua pham vi");
        }

        if (vitricuoi != data[3].size()) {//day neu cac vi tri cuoi cung ko phai ss
            throw Loixacthuc("Loi: Ban kinh phu song chua ky tu khong hop le!");
        }

        //tao doi tuong tam de kiem tra du lieu truoc
        TramBTS tramTam(data[0], data[1], data[2], banKinh, data[4]);//tao ra 1 doi tuong ao de dua len ram neu trong qua trinh co loi no se vut bo ma ko lam ton hai j

        //chi cap nhat doi tuong khi du lieu da hop le
        *this = tramTam;//con tro this,dau * de giai tham chieu ch con tro this va gan no voi tram tam
    }

    // giao dien hien thi con sole
    void display() const override {//dien chu cho tung ham
        std::cout << std::left//cai nay lam ki tu dc viet ra bam sat vo ben trai
            << std::setw(10) << id << "| "//setw de lay do rong
            << std::setw(25) << (tenTram.length() > 22 ? tenTram.substr(0, 19) + "..." : tenTram) << "| "//toan tu 3 ngoi ? va : (dieu kien? lay cai nay neu dung:lay cai nay neu sai) dai qua 22 ki tu thi 20 ki tu dau du nguyen 3 ki tu sau thay = dau ...
            << std::setw(15) << loaiCot << "| "
            << std::setw(14) << banKinhPhuSong << "| "
            << std::setw(15) << trangThai << "\n";
    }

    static void displayHeader() {//in ra cai giao dien
        std::cout << std::left
            << std::setw(10) << "Ma Tram" << "| "
            << std::setw(25) << "Ten Tram" << "| "
            << std::setw(15) << "Loai Cot" << "| "
            << std::setw(14) << "Ban Kinh(km)" << "| "
            << std::setw(15) << "Trang Thai" << std::endl;

        std::cout << std::string(87, '-') << std::endl;
    }
};
