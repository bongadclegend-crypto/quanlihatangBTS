#pragma once
#include<string>
#include"Exceptions.h"
#include"InputHelper.h"
class Entity{
protected:
	std::string id;//ma dinh danh chung tren moi doi tuong
public:
	//ham huy ao
	virtual ~Entity() = default;
	std::string getId()const {
		return id;
	}
	//ham thiet lap ma co kiem tra
	void setId(const std::string& newId) {
		//cat khoang trang cua cai id ms
		std::string cleanId = Inputhelper::catkhoangtrang(newId);
		//neu trong thi nem ra loi xac thuc
		if (cleanId.empty()) {
			throw Loixacthuc("Loi:Ma dinh danh khong duoc de trong");
		}
		//chan so am
		if (cleanId[0] == '-') {
			throw Loixacthuc("Loi: Ma dinh danh khong duoc la so am!");
		}
		id = cleanId;
	}
	// ham ao thiet lap ma dinh danh
	//1 ham dong goi doi tuong thanh 1 dong chuoi
	virtual std::string toFilerecord() const = 0;
	//2 ham doc 1 chuoi tu file txt va cat chung ra de nap thanh 1 doi tuong
	virtual void fromFilerecord(const std::string& record) = 0;
	//3 ham in doi tuong ra man hinh console(dang bang)
	virtual void display() const = 0;
};