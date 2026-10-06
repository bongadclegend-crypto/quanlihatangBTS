#pragma once
#include<string>
//xay dung lop co so base class cho moi ngoai le cho truong chinh
class Baseexception {
protected:
	std::string message;
public:
	//ham khoi tao  nhan vao chuoi thong bao 
	Baseexception(const std::string &msg) : message(msg){}
	//ham ao lay thong bao loi
	virtual std::string getMessage() const {
		return message;
	}
	//ham huy ao de giai phong bo nho khi co ngoai le xay ra
	virtual ~Baseexception() = default;
};
class Loixacthuc : public Baseexception {
public:
	Loixacthuc(const std::string& msg) : Baseexception(msg) {}
};
class Ngoailekotimthay : public Baseexception {
	public:
	Ngoailekotimthay(const std::string& msg) : Baseexception(msg) {}
};
class Ngoailerangbuoc : public Baseexception {
public:
	Ngoailerangbuoc(const std::string& msg) : Baseexception(msg) {}
};