#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <sstream>

using namespace std;

// ==========================================
// 1. CÁC LỚP NGOẠI LỆ (CUSTOM EXCEPTIONS)
// ==========================================
class ValidationException : public runtime_error {
public:
    ValidationException(const string& msg) : runtime_error(msg) {}
};

class NotFoundException : public runtime_error {
public:
    NotFoundException(const string& msg) : runtime_error(msg) {}
};

class ConstraintException : public runtime_error {
public:
    ConstraintException(const string& msg) : runtime_error(msg) {}
};

// ==========================================
// 2. LỚP TIỆN ÍCH (UTILITIES)
// ==========================================
class InputHelper {
public:
    static string trim(const string& str) {
        size_t first = str.find_first_not_of(' ');
        if (string::npos == first) return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }

    static vector<string> split(const string& str, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(str);
        while (getline(tokenStream, token, delimiter)) {
            tokens.push_back(trim(token));
        }
        return tokens;
    }

    static string readString(const string& prompt, bool allowEmpty = false) {
        string input;
        while (true) {
            cout << prompt;
            getline(cin, input);
            input = trim(input);
            if (!allowEmpty && input.empty()) {
                cout << "Loi: Khong duoc de trong. Vui long nhap lai!\n";
            }
            else {
                return input;
            }
        }
    }

    static int readInt(const string& prompt) {
        string input;
        while (true) {
            cout << prompt;
            getline(cin, input);
            try {
                return stoi(trim(input));
            }
            catch (...) {
                cout << "Loi: Vui long nhap mot so nguyen hop le!\n";
            }
        }
    }
};

// ==========================================
// 3. LỚP CƠ SỞ TRỪU TƯỢNG (ABSTRACT ENTITY)
// ==========================================
class Entity {
protected:
    string id;
public:
    virtual ~Entity() = default;

    string getId() const { return id; }
    void setId(const string& newId) {
        if (InputHelper::trim(newId).empty()) throw ValidationException("Ma dinh danh khong duoc rong!");
        id = InputHelper::trim(newId);
    }

    virtual string toFileRecord() const = 0;
    virtual void fromFileRecord(const string& record) = 0;
    virtual void display() const = 0;
};

// ==========================================
// 4. CÁC LỚP THỰC THỂ (10 ĐỐI TƯỢNG)
// ==========================================

// 4.1. KHU VỰC (Thành viên 1)
class KhuVuc : public Entity {
private:
    string tenKhuVuc, nguoiQuanLy;
public:
    KhuVuc() = default;
    KhuVuc(string id, string ten, string ql) { setId(id); tenKhuVuc = ten; nguoiQuanLy = ql; }
    string getTenKhuVuc() const { return tenKhuVuc; } void setTenKhuVuc(const string& v) { tenKhuVuc = v; }
    string getNguoiQuanLy() const { return nguoiQuanLy; } void setNguoiQuanLy(const string& v) { nguoiQuanLy = v; }

    string toFileRecord() const override { return id + "|" + tenKhuVuc + "|" + nguoiQuanLy; }
    void fromFileRecord(const string& record) override {
        auto data = InputHelper::split(record, '|');
        if (data.size() >= 3) { setId(data[0]); tenKhuVuc = data[1]; nguoiQuanLy = data[2]; }
    }
    void display() const override {
        cout << left << setw(10) << id << "| " << setw(20) << tenKhuVuc << "| " << setw(20) << nguoiQuanLy << endl;
    }
    static void displayHeader() { cout << left << setw(10) << "Ma KV" << "| " << setw(20) << "Ten Khu Vuc" << "| " << setw(20) << "Quan Ly" << "\n----------------------------------------------------\n"; }
};

// 4.2. TRẠM BTS (Thành viên 1)
class TramBTS : public Entity {
private:
    string tenTram, maKhuVuc, toaDo, trangThai;
public:
    TramBTS() = default;
    TramBTS(string id, string ten, string mKV, string td, string tt) { setId(id); tenTram = ten; maKhuVuc = mKV; toaDo = td; trangThai = tt; }
    string getTenTram() const { return tenTram; } void setTenTram(const string& v) { tenTram = v; }
    string getMaKhuVuc() const { return maKhuVuc; } void setMaKhuVuc(const string& v) { maKhuVuc = v; }
    string getToaDo() const { return toaDo; } void setToaDo(const string& v) { toaDo = v; }
    string getTrangThai() const { return trangThai; } void setTrangThai(const string& v) { trangThai = v; }

    string toFileRecord() const override { return id + "|" + tenTram + "|" + maKhuVuc + "|" + toaDo + "|" + trangThai; }
    void fromFileRecord(const string& record) override {
        auto data = InputHelper::split(record, '|');
        if (data.size() >= 5) { setId(data[0]); tenTram = data[1]; maKhuVuc = data[2]; toaDo = data[3]; trangThai = data[4]; }
    }
    void display() const override { cout << left << setw(10) << id << "| " << setw(20) << tenTram << "| " << setw(10) << maKhuVuc << "| " << setw(15) << toaDo << "| " << setw(15) << trangThai << endl; }
    static void displayHeader() { cout << left << setw(10) << "Ma Tram" << "| " << setw(20) << "Ten Tram" << "| " << setw(10) << "Ma KV" << "| " << setw(15) << "Toa Do" << "| " << setw(15) << "Trang Thai" << "\n------------------------------------------------------------------------\n"; }
};

// 4.3. NHÀ CUNG CẤP (Thành viên 2)
class NhaCungCap : public Entity {
private:
    string tenNCC, soDienThoai;
public:
    NhaCungCap() = default;
    NhaCungCap(string id, string ten, string sdt) { setId(id); tenNCC = ten; soDienThoai = sdt; }
    string getTenNCC() const { return tenNCC; } void setTenNCC(const string& v) { tenNCC = v; }
    string getSoDienThoai() const { return soDienThoai; } void setSoDienThoai(const string& v) { soDienThoai = v; }

    string toFileRecord() const override { return id + "|" + tenNCC + "|" + soDienThoai; }
    void fromFileRecord(const string& record) override {
        auto data = InputHelper::split(record, '|');
        if (data.size() >= 3) { setId(data[0]); tenNCC = data[1]; soDienThoai = data[2]; }
    }
    void display() const override { cout << left << setw(10) << id << "| " << setw(25) << tenNCC << "| " << setw(15) << soDienThoai << endl; }
    static void displayHeader() { cout << left << setw(10) << "Ma NCC" << "| " << setw(25) << "Ten Nha Cung Cap" << "| " << setw(15) << "So Dien Thoai" << "\n----------------------------------------------------\n"; }
};

// 4.4. LOẠI THIẾT BỊ (Thành viên 2)
class LoaiThietBi : public Entity {
private:
    string tenLoai, hangSanXuat;
public:
    LoaiThietBi() = default;
    LoaiThietBi(string id, string ten, string hang) { setId(id); tenLoai = ten; hangSanXuat = hang; }
    string getTenLoai() const { return tenLoai; } void setTenLoai(const string& v) { tenLoai = v; }
    string getHangSanXuat() const { return hangSanXuat; } void setHangSanXuat(const string& v) { hangSanXuat = v; }

    string toFileRecord() const override { return id + "|" + tenLoai + "|" + hangSanXuat; }
    void fromFileRecord(const string& record) override {
        auto data = InputHelper::split(record, '|');
        if (data.size() >= 3) { setId(data[0]); tenLoai = data[1]; hangSanXuat = data[2]; }
    }
    void display() const override { cout << left << setw(10) << id << "| " << setw(20) << tenLoai << "| " << setw(20) << hangSanXuat << endl; }
    static void displayHeader() { cout << left << setw(10) << "Ma Loai" << "| " << setw(20) << "Ten Loai" << "| " << setw(20) << "Hang San Xuat" << "\n----------------------------------------------------\n"; }
};

// 4.5. THIẾT BỊ LẮP ĐẶT (Thành viên 3)
class ThietBiLapDat : public Entity {
private:
    string tenTB, maTram, maLoai;
public:
    ThietBiLapDat() = default;
    ThietBiLapDat(string id, string ten, string mT, string mL) { setId(id); tenTB = ten; maTram = mT; maLoai = mL; }
    string getTenTB() const { return tenTB; } void setTenTB(const string& v) { tenTB = v; }
    string getMaTram() const { return maTram; } void setMaTram(const string& v) { maTram = v; }
    string getMaLoai() const { return maLoai; } void setMaLoai(const string& v) { maLoai = v; }

    string toFileRecord() const override { return id + "|" + tenTB + "|" + maTram + "|" + maLoai; }
    void fromFileRecord(const string& record) override {
        auto data = InputHelper::split(record, '|');
        if (data.size() >= 4) { setId(data[0]); tenTB = data[1]; maTram = data[2]; maLoai = data[3]; }
    }
    void display() const override { cout << left << setw(10) << id << "| " << setw(20) << tenTB << "| " << setw(10) << maTram << "| " << setw(10) << maLoai << endl; }
    static void displayHeader() { cout << left << setw(10) << "Ma TB" << "| " << setw(20) << "Ten Thiet Bi" << "| " << setw(10) << "Ma Tram" << "| " << setw(10) << "Ma Loai" << "\n----------------------------------------------------\n"; }
};

// 4.6. ANTEN & CẤU HÌNH PHÁT (Thành viên 3)
class Anten : public Entity {
private:
    string maTram, loaiAnten, chieuCao;
public:
    Anten() = default;
    Anten(string id, string mT, string lA, string cC) { setId(id); maTram = mT; loaiAnten = lA; chieuCao = cC; }
    string getMaTram() const { return maTram; } void setMaTram(const string& v) { maTram = v; }
    string getLoaiAnten() const { return loaiAnten; } void setLoaiAnten(const string& v) { loaiAnten = v; }
    string getChieuCao() const { return chieuCao; } void setChieuCao(const string& v) { chieuCao = v; }

    string toFileRecord() const override { return id + "|" + maTram + "|" + loaiAnten + "|" + chieuCao; }
    void fromFileRecord(const string& record) override {
        auto data = InputHelper::split(record, '|');
        if (data.size() >= 4) { setId(data[0]); maTram = data[1]; loaiAnten = data[2]; chieuCao = data[3]; }
    }
    void display() const override { cout << left << setw(10) << id << "| " << setw(10) << maTram << "| " << setw(15) << loaiAnten << "| " << setw(10) << chieuCao << endl; }
    static void displayHeader() { cout << left << setw(10) << "Ma Anten" << "| " << setw(10) << "Ma Tram" << "| " << setw(15) << "Loai Anten" << "| " << setw(10) << "Chieu Cao" << "\n----------------------------------------------------\n"; }
};

// 4.7. KỸ THUẬT VIÊN (Thành viên 4)
class KyThuatVien : public Entity {
private:
    string tenKTV, soDienThoai;
public:
    KyThuatVien() = default;
    KyThuatVien(string id, string ten, string sdt) { setId(id); tenKTV = ten; soDienThoai = sdt; }
    string getTenKTV() const { return tenKTV; } void setTenKTV(const string& v) { tenKTV = v; }
    string getSoDienThoai() const { return soDienThoai; } void setSoDienThoai(const string& v) { soDienThoai = v; }

    string toFileRecord() const override { return id + "|" + tenKTV + "|" + soDienThoai; }
    void fromFileRecord(const string& record) override {
        auto data = InputHelper::split(record, '|');
        if (data.size() >= 3) { setId(data[0]); tenKTV = data[1]; soDienThoai = data[2]; }
    }
    void display() const override { cout << left << setw(10) << id << "| " << setw(20) << tenKTV << "| " << setw(15) << soDienThoai << endl; }
    static void displayHeader() { cout << left << setw(10) << "Ma KTV" << "| " << setw(20) << "Ten Ky Thuat Vien" << "| " << setw(15) << "So Dien Thoai" << "\n----------------------------------------------------\n"; }
};

// 4.8. HỢP ĐỒNG THUÊ VỊ TRÍ (Thành viên 4)
class HopDong : public Entity {
private:
    string maTram, tenChuNha, giaThue;
public:
    HopDong() = default;
    HopDong(string id, string mT, string tCN, string gT) { setId(id); maTram = mT; tenChuNha = tCN; giaThue = gT; }
    string getMaTram() const { return maTram; } void setMaTram(const string& v) { maTram = v; }
    string getTenChuNha() const { return tenChuNha; } void setTenChuNha(const string& v) { tenChuNha = v; }
    string getGiaThue() const { return giaThue; } void setGiaThue(const string& v) { giaThue = v; }

    string toFileRecord() const override { return id + "|" + maTram + "|" + tenChuNha + "|" + giaThue; }
    void fromFileRecord(const string& record) override {
        auto data = InputHelper::split(record, '|');
        if (data.size() >= 4) { setId(data[0]); maTram = data[1]; tenChuNha = data[2]; giaThue = data[3]; }
    }
    void display() const override { cout << left << setw(10) << id << "| " << setw(10) << maTram << "| " << setw(20) << tenChuNha << "| " << setw(15) << giaThue << endl; }
    static void displayHeader() { cout << left << setw(10) << "Ma HD" << "| " << setw(10) << "Ma Tram" << "| " << setw(20) << "Ten Chu Nha" << "| " << setw(15) << "Gia Thue" << "\n----------------------------------------------------\n"; }
};

// 4.9. PHIẾU BẢO TRÍ ĐỊNH KỲ (Thành viên 5)
class PhieuBaoTri : public Entity {
private:
    string maTram, maKTV, ngayBaoTri;
public:
    PhieuBaoTri() = default;
    PhieuBaoTri(string id, string mT, string mK, string nBT) { setId(id); maTram = mT; maKTV = mK; ngayBaoTri = nBT; }
    string getMaTram() const { return maTram; } void setMaTram(const string& v) { maTram = v; }
    string getMaKTV() const { return maKTV; } void setMaKTV(const string& v) { maKTV = v; }
    string getNgayBaoTri() const { return ngayBaoTri; } void setNgayBaoTri(const string& v) { ngayBaoTri = v; }

    string toFileRecord() const override { return id + "|" + maTram + "|" + maKTV + "|" + ngayBaoTri; }
    void fromFileRecord(const string& record) override {
        auto data = InputHelper::split(record, '|');
        if (data.size() >= 4) { setId(data[0]); maTram = data[1]; maKTV = data[2]; ngayBaoTri = data[3]; }
    }
    void display() const override { cout << left << setw(10) << id << "| " << setw(10) << maTram << "| " << setw(10) << maKTV << "| " << setw(15) << ngayBaoTri << endl; }
    static void displayHeader() { cout << left << setw(10) << "Ma Phieu" << "| " << setw(10) << "Ma Tram" << "| " << setw(10) << "Ma KTV" << "| " << setw(15) << "Ngay Bao Tri" << "\n----------------------------------------------------\n"; }
};

// 4.10. NHẬT KÝ SỰ CỐ (Thành viên 5)
class NhatKySuCo : public Entity {
private:
    string maTram, thoiGian, moTa;
public:
    NhatKySuCo() = default;
    NhatKySuCo(string id, string mT, string tG, string mTt) { setId(id); maTram = mT; thoiGian = tG; moTa = mTt; }
    string getMaTram() const { return maTram; } void setMaTram(const string& v) { maTram = v; }
    string getThoiGian() const { return thoiGian; } void setThoiGian(const string& v) { thoiGian = v; }
    string getMoTa() const { return moTa; } void setMoTa(const string& v) { moTa = v; }

    string toFileRecord() const override { return id + "|" + maTram + "|" + thoiGian + "|" + moTa; }
    void fromFileRecord(const string& record) override {
        auto data = InputHelper::split(record, '|');
        if (data.size() >= 4) { setId(data[0]); maTram = data[1]; thoiGian = data[2]; moTa = data[3]; }
    }
    void display() const override { cout << left << setw(10) << id << "| " << setw(10) << maTram << "| " << setw(15) << thoiGian << "| " << setw(20) << moTa << endl; }
    static void displayHeader() { cout << left << setw(10) << "Ma NK" << "| " << setw(10) << "Ma Tram" << "| " << setw(15) << "Thoi Gian" << "| " << setw(20) << "Mo Ta" << "\n----------------------------------------------------\n"; }
};

// ==========================================
// 5. LỚP KHUÔN MẪU QUẢN LÝ (TEMPLATE REPOSITORY)
// ==========================================
template <class T>
class Repository {
private:
    vector<shared_ptr<T>> dataList;
    string filePath;
public:
    Repository(const string& path) : filePath(path) {}

    const vector<shared_ptr<T>>& getAll() const { return dataList; }

    shared_ptr<T> findById(const string& id) const {
        auto it = find_if(dataList.begin(), dataList.end(), [&id](const shared_ptr<T>& item) { return item->getId() == id; });
        return (it != dataList.end()) ? *it : nullptr;
    }

    void add(shared_ptr<T> item) {
        if (findById(item->getId()) != nullptr) throw ValidationException("Ma dinh danh da ton tai!");
        dataList.push_back(item);
        saveToFile();
    }

    void update() { saveToFile(); }

    void remove(const string& id) {
        auto it = find_if(dataList.begin(), dataList.end(), [&id](const shared_ptr<T>& item) { return item->getId() == id; });
        if (it != dataList.end()) {
            dataList.erase(it);
            saveToFile();
        }
        else throw NotFoundException("Khong tim thay ban ghi!");
    }

    void loadFromFile() {
        ifstream file(filePath);
        if (!file.is_open()) return;
        string line;
        while (getline(file, line)) {
            line = InputHelper::trim(line);
            if (line.empty() || line[0] == '#') continue;
            try {
                shared_ptr<T> item = make_shared<T>();
                item->fromFileRecord(line);
                dataList.push_back(item);
            }
            catch (...) {}
        }
        file.close();
    }

    void saveToFile() const {
        system("mkdir data 2> null");
        ofstream file(filePath, ios::trunc);
        if (!file.is_open()) throw runtime_error("Khong the mo file: " + filePath);
        file << "# Format data\n";
        for (const auto& item : dataList) file << item->toFileRecord() << "\n";
        file.close();
    }
};

// ==========================================
// 6. KHỞI TẠO REPOSITORY TOÀN CỤC CHO 10 USE CASE
// ==========================================
Repository<KhuVuc>* repoKhuVuc = nullptr;
Repository<TramBTS>* repoTramBTS = nullptr;
Repository<NhaCungCap>* repoNhaCungCap = nullptr;
Repository<LoaiThietBi>* repoLoaiThietBi = nullptr;
Repository<ThietBiLapDat>* repoThietBiLapDat = nullptr;
Repository<Anten>* repoAnten = nullptr;
Repository<KyThuatVien>* repoKTV = nullptr;
Repository<HopDong>* repoHopDong = nullptr;
Repository<PhieuBaoTri>* repoPhieuBaoTri = nullptr;
Repository<NhatKySuCo>* repoNhatKy = nullptr;

// ==========================================
// 7. HÀM MENU DÙNG CHUNG CHO CÁC USE CASE
// Thiết kế tinh gọn để không phải viết lặp lại mã C-R-U-D 10 lần
// ==========================================

// Hàm xem danh sách tổng quát
template <class T>
void viewAll(Repository<T>* repo) {
    const auto& list = repo->getAll();
    if (list.empty()) cout << "-> Danh sach trong.\n";
    else {
        T::displayHeader();
        for (const auto& item : list) item->display();
    }
}

// ----------------------------------------------------
// THÀNH VIÊN 1: Khu Vực & Trạm BTS (Mẫu hoàn chỉnh có khóa ngoại)
// ----------------------------------------------------
void menuKhuVuc() {
    while (true) {
        cout << "\n--- QUAN LY KHU VUC ---\n1. Them\n2. Xem\n3. Sua\n4. Xoa\n0. Quay lai\n";
        int choice = InputHelper::readInt("Chon: ");
        try {
            if (choice == 1) {
                string id = InputHelper::readString("Ma KV: ");
                string ten = InputHelper::readString("Ten KV: ");
                string ql = InputHelper::readString("Quan ly: ", true);
                repoKhuVuc->add(make_shared<KhuVuc>(id, ten, ql));
                cout << "-> Them thanh cong!\n";
            }
            else if (choice == 2) viewAll(repoKhuVuc);
            else if (choice == 3) {
                string id = InputHelper::readString("Ma KV can sua: ");
                auto obj = repoKhuVuc->findById(id);
                if (!obj) throw NotFoundException("Khong tim thay!");
                string ten = InputHelper::readString("Ten moi: ", true); if (!ten.empty()) obj->setTenKhuVuc(ten);
                string ql = InputHelper::readString("Quan ly moi: ", true); if (!ql.empty()) obj->setNguoiQuanLy(ql);
                repoKhuVuc->update(); cout << "-> Cap nhat thanh cong!\n";
            }
            else if (choice == 4) {
                string id = InputHelper::readString("Ma KV can xoa: ");
                for (auto& tram : repoTramBTS->getAll())
                    if (tram->getMaKhuVuc() == id) throw ConstraintException("Khong the xoa, dang chua Tram BTS!");
                repoKhuVuc->remove(id); cout << "-> Xoa thanh cong!\n";
            }
            else if (choice == 0) break;
        }
        catch (const exception& e) { cout << "Loi: " << e.what() << endl; }
    }
}

void menuTramBTS() {
    while (true) {
        cout << "\n--- QUAN LY TRAM BTS ---\n1. Them\n2. Xem\n3. Sua\n4. Xoa\n0. Quay lai\n";
        int choice = InputHelper::readInt("Chon: ");
        try {
            if (choice == 1) {
                string id = InputHelper::readString("Ma Tram: ");
                string ten = InputHelper::readString("Ten Tram: ");
                string mKV = InputHelper::readString("Ma KV: ");
                if (!repoKhuVuc->findById(mKV)) throw ConstraintException("Ma KV khong ton tai!");
                string td = InputHelper::readString("Toa do: ");
                string tt = InputHelper::readString("Trang thai: ");
                repoTramBTS->add(make_shared<TramBTS>(id, ten, mKV, td, tt));
                cout << "-> Them thanh cong!\n";
            }
            else if (choice == 2) viewAll(repoTramBTS);
            else if (choice == 3) {
                string id = InputHelper::readString("Ma Tram can sua: ");
                auto obj = repoTramBTS->findById(id);
                if (!obj) throw NotFoundException("Khong tim thay!");
                string ten = InputHelper::readString("Ten moi: ", true); if (!ten.empty()) obj->setTenTram(ten);
                string mKV = InputHelper::readString("Ma KV moi: ", true);
                if (!mKV.empty()) { if (!repoKhuVuc->findById(mKV)) throw ConstraintException("KV khong ton tai!"); obj->setMaKhuVuc(mKV); }
                string td = InputHelper::readString("Toa do moi: ", true); if (!td.empty()) obj->setToaDo(td);
                string tt = InputHelper::readString("Trang thai moi: ", true); if (!tt.empty()) obj->setTrangThai(tt);
                repoTramBTS->update(); cout << "-> Cap nhat thanh cong!\n";
            }
            else if (choice == 4) {
                repoTramBTS->remove(InputHelper::readString("Ma Tram can xoa: ")); cout << "-> Xoa thanh cong!\n";
            }
            else if (choice == 0) break;
        }
        catch (const exception& e) { cout << "Loi: " << e.what() << endl; }
    }
}

// ----------------------------------------------------
// THÀNH VIÊN 2 -> 5: Menu Rút Gọn (Chức năng tương tự)
// (Các bạn thành viên có thể copy logic kiểm tra khóa ngoại của TramBTS xuống đây)
// ----------------------------------------------------
void menuNhaCungCap() {
    while (true) {
        cout << "\n--- QUAN LY NHA CUNG CAP ---\n1. Them\n2. Xem\n3. Sua\n4. Xoa\n0. Quay lai\n";
        int choice = InputHelper::readInt("Chon: ");
        try {
            if (choice == 1) repoNhaCungCap->add(make_shared<NhaCungCap>(InputHelper::readString("Ma NCC: "), InputHelper::readString("Ten NCC: "), InputHelper::readString("SDT: ")));
            else if (choice == 2) viewAll(repoNhaCungCap);
            else if (choice == 3) {/* Viet tuong tu Menu Khu Vuc */ }
            else if (choice == 4) repoNhaCungCap->remove(InputHelper::readString("Ma xoa: "));
            else if (choice == 0) break;
        }
        catch (exception& e) { cout << "Loi: " << e.what() << endl; }
    }
}
void menuLoaiThietBi() {
    while (true) {
        cout << "\n--- QUAN LY LOAI THIET BI ---\n1. Them\n2. Xem\n3. Sua\n4. Xoa\n0. Quay lai\n";
        int choice = InputHelper::readInt("Chon: ");
        try {
            if (choice == 1) repoLoaiThietBi->add(make_shared<LoaiThietBi>(InputHelper::readString("Ma Loai: "), InputHelper::readString("Ten: "), InputHelper::readString("Hang SX: ")));
            else if (choice == 2) viewAll(repoLoaiThietBi);
            else if (choice == 3) {/* Viet tuong tu */ }
            else if (choice == 4) repoLoaiThietBi->remove(InputHelper::readString("Ma xoa: "));
            else if (choice == 0) break;
        }
        catch (exception& e) { cout << "Loi: " << e.what() << endl; }
    }
}
void menuThietBiLapDat() {
    // Luu y: Them rang buoc maTram phai ton tai o day
    while (true) {
        cout << "\n--- QUAN LY THIET BI LAP DAT ---\n1. Them\n2. Xem\n0. Quay lai\n";
        int choice = InputHelper::readInt("Chon: ");
        if (choice == 1) {
            try { repoThietBiLapDat->add(make_shared<ThietBiLapDat>(InputHelper::readString("Ma TB: "), InputHelper::readString("Ten TB: "), InputHelper::readString("Ma Tram: "), InputHelper::readString("Ma Loai: "))); }
            catch (exception& e) { cout << "Loi: " << e.what() << endl; }
        }
        else if (choice == 2) viewAll(repoThietBiLapDat);
        else if (choice == 0) break;
    }
}
void menuKyThuatVien() {
    while (true) {
        cout << "\n--- QUAN LY KY THUAT VIEN ---\n1. Them\n2. Xem\n0. Quay lai\n";
        int choice = InputHelper::readInt("Chon: ");
        if (choice == 1) {
            try { repoKTV->add(make_shared<KyThuatVien>(InputHelper::readString("Ma KTV: "), InputHelper::readString("Ten KTV: "), InputHelper::readString("SDT: "))); }
            catch (exception& e) { cout << "Loi: " << e.what() << endl; }
        }
        else if (choice == 2) viewAll(repoKTV); else if (choice == 0) break;
    }
}
void menuHopDong() {
    while (true) {
        cout << "\n--- QUAN LY HOP DONG ---\n1. Them\n2. Xem\n0. Quay lai\n";
        int choice = InputHelper::readInt("Chon: ");
        if (choice == 1) {
            try { repoHopDong->add(make_shared<HopDong>(InputHelper::readString("Ma HD: "), InputHelper::readString("Ma Tram: "), InputHelper::readString("Chu nha: "), InputHelper::readString("Gia thue: "))); }
            catch (exception& e) { cout << "Loi: " << e.what() << endl; }
        }
        else if (choice == 2) viewAll(repoHopDong); else if (choice == 0) break;
    }
}


// ==========================================
// 8. HÀM CHÍNH & MENU TỔNG
// ==========================================
int main() {
    // 1. Khoi tao Database Files
    Repository<KhuVuc> dbKhuVuc("data/khuvuc.txt"); repoKhuVuc = &dbKhuVuc;
    Repository<TramBTS> dbTramBTS("data/trambts.txt"); repoTramBTS = &dbTramBTS;
    Repository<NhaCungCap> dbNCC("data/nhacungcap.txt"); repoNhaCungCap = &dbNCC;
    Repository<LoaiThietBi> dbLoai("data/loaithietbi.txt"); repoLoaiThietBi = &dbLoai;
    Repository<ThietBiLapDat> dbTBLD("data/thietbilapdat.txt"); repoThietBiLapDat = &dbTBLD;
    Repository<Anten> dbAnten("data/anten.txt"); repoAnten = &dbAnten;
    Repository<KyThuatVien> dbKTV("data/kythuatvien.txt"); repoKTV = &dbKTV;
    Repository<HopDong> dbHD("data/hopdong.txt"); repoHopDong = &dbHD;
    Repository<PhieuBaoTri> dbPBT("data/phieubaotri.txt"); repoPhieuBaoTri = &dbPBT;
    Repository<NhatKySuCo> dbNK("data/nhatky.txt"); repoNhatKy = &dbNK;

    // 2. Nap du lieu tu 10 file len RAM
    cout << "Dang nap du lieu he thong...\n";
    repoKhuVuc->loadFromFile(); repoTramBTS->loadFromFile(); repoNhaCungCap->loadFromFile();
    repoLoaiThietBi->loadFromFile(); repoThietBiLapDat->loadFromFile(); repoAnten->loadFromFile();
    repoKTV->loadFromFile(); repoHopDong->loadFromFile(); repoPhieuBaoTri->loadFromFile(); repoNhatKy->loadFromFile();

    // 3. MENU TỔNG
    while (true) {
        cout << "\n============= HE THONG QUAN LY HA TANG BTS =============" << endl;
        cout << "[Nhom 1] 1. Khu Vuc          | 2. Tram BTS" << endl;
        cout << "[Nhom 2] 3. Nha Cung Cap     | 4. Loai Thiet Bi" << endl;
        cout << "[Nhom 3] 5. Thiet Bi Lap Dat | 6. Anten (Dang nang cap)" << endl;
        cout << "[Nhom 4] 7. Ky Thuat Vien    | 8. Hop Dong Thue" << endl;
        cout << "[Nhom 5] 9. Phieu Bao Tri    | 10. Nhat Ky Su Co" << endl;
        cout << "         0. Thoat va Luu Du Lieu" << endl;
        cout << "========================================================" << endl;

        int choice = InputHelper::readInt("Nhap lua chon: ");

        switch (choice) {
        case 1: menuKhuVuc(); break;
        case 2: menuTramBTS(); break;
        case 3: menuNhaCungCap(); break;
        case 4: menuLoaiThietBi(); break;
        case 5: menuThietBiLapDat(); break;
        case 7: menuKyThuatVien(); break;
        case 8: menuHopDong(); break;
        case 6: case 9: case 10:
            cout << "=> Tinh nang nay dang duoc hoan thien them. Vui long chon tinh nang khac!\n"; break;
        case 0:
            cout << "Du lieu da duoc luu an toan. Tam biet!\n";
            return 0;
        default:
            cout << "Lua chon sai!\n";
        }
    }
    return 0;
}