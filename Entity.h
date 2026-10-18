#pragma once
#include<string>
#include"Exceptions.h"
#include"Inputhelper.h"
//lop co so truu tuong cac doi tuong quan ly
class Entity {
protected:
	//ma dinh danh chung cua doi tuong
	std::string id;
public:
	//ham huy ao
	virtual ~Entity() = default;//ham huy ao dfault de tu sinh ra huy vi du nhu Entity *p=new nhanvien() delete p;
	//lay ma dinh danh
	std::string getId()const {//tra ve ma dinh danh doi tuong ma ko thay doi no
		return id;
	}
	//vgan ma dinh danh sa khi kiem tra
	void setId(const std::string& newId) {//gan du lieu cho phep sua du lieu nhung phai kiem duyet
		std::string cleanId = Inputhelper::catkhoangtrang(newId);

		if (cleanId.empty()) {
			throw Loixacthuc("Loi: Ma dinh danh ko dc de trong!");
		}
		if (cleanId[0] == '-') {
			throw Loixacthuc("Loi: Ma dinh danh ko the la so am!");
		}
		id = cleanId;
	}
	// chuyen doi tuong thanh chuoid e luu vao file
	virtual std::string toFilerecord() const = 0;//chi tao chuoi bieu dien de luu vao ban ghi
	//doc chuoi banghi va nap du lieu vao doi tuong
	virtual void fromFilerecord(const std::string& record) = 0;//chuyen doi doi tuong thanh chuoi, nhan chuoi tu file,nv chinh la cap nhat du lieu nen tra ve kieu void
	//hien thi doi tuong
	virtual void display() const = 0;//quy dinh moi lop con phai cung cap thong tin cua chinh no


};

