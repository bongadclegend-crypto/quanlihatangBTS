#pragma once
#include<string>
//lop co so de thong bao loi
class Baseexception {
protected:
	std::string message;
public:
	//khoi tao thong bao loi
	Baseexception(const std::string& msg) :message(msg) {}
	//tra ve noi dung thong bao
	virtual std::string getMessage() const {
		return message;
	}
	//ham huy ao giai phong bo nho khi co ngoai le xay ra
	virtual~Baseexception() = default;
};
class Loixacthuc :public Baseexception {
public:
	Loixacthuc(const std::string& msg) :Baseexception(msg) {}
};
class Ngoailekotimthay :public Baseexception {
public:
	Ngoailekotimthay(const std::string& msg) :Baseexception(msg) {}
};
class Ngoailerangbuoc :public Baseexception {
public:
	Ngoailerangbuoc(const std::string& msg) : Baseexception(msg) {}
};
