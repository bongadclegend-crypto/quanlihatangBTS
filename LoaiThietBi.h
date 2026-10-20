#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "Entity.h"
#include "InputHelper.h"
using namespace std;
//ke thua lop truu tuong public Entity
class LoaiThietBi:public Entity{
	private:
		string tenLoai;//Ten hien thi
		string hangSanXuat;//hang san xuat
		string dacTinhKyThuat;//dac tinh ky thuat
		double congSuatTieuThu;//cong suat
		
		
		public:
			//khoi tao constructor mac dinh cong suat la 0
			LoaiThietBi():congSuatTieuThu(0.0){}
			//constructor co tham so de khoi tao nhanh doi tuong hay khi doc file
			LoaiThietBi(string ma,string ten,string hang,string dactinh,double congsuat){
			//goi ham tu dong them ma khi nhap ma voi tien to LT o phia truoc
				setId(InputHelper::tuTaoMa(ma,"LT"));
				//goi cac setter de du lieu duoc di qua kiem tra loi
				setTenLoai(ten);
				setHangSanXuat(hang);
				setDacTinhKyThuat(dactinh);
				setCongSuatTieuThu(congsuat);
				
			}
			
			string getTenLoai ()const {return tenLoai;}
void setTenLoai(const string&ten){  
//cat bo khoang chong o dau va cuoi do nguoi dung vo tinh nhap phai   
	string clean=InputHelper::catkhoangtrang(ten);
//neu rong bao loi
	if(clean.empty()) throw Loixacthuc("ten khong duoc de trong");
	//bat loi logic 
	if(!InputHelper::coChuaChuCai(clean)) throw Loixacthuc("ten loai phai chua it nhat 1 chu cai ");
	tenLoai=clean;

}
//tuong tu voi cac getter va setter khac 
string getHangSanXuat ()const {return hangSanXuat;}
void setHangSanXuat(const string&hang){
	string clean=InputHelper::catkhoangtrang(hang);
	if(clean.empty()) throw Loixacthuc("hang khong duoc de trong");
	if(!InputHelper::coChuaChuCai(clean)) throw Loixacthuc("ten hang phai chua it nhat 1 chu cai ");
	hangSanXuat=clean;

}
string getDacTinhKyThuat()const{return dacTinhKyThuat;}
void setDacTinhKyThuat(const string&dactinh){
	string clean=InputHelper::catkhoangtrang(dactinh);
	if(clean.empty()) throw Loixacthuc("dac tinh khong duoc de trong");
	if(!InputHelper::coChuaChuCai(clean)) throw Loixacthuc("dac tinh ki thuat phai chua it nhat 1 chu cai ");
	dacTinhKyThuat=clean;

}
double getCongSuatTieuThu()const{return congSuatTieuThu;}
void setCongSuatTieuThu(double congsuat){
	if(congsuat<0) throw Loixacthuc("cong suat phai la so duong");
	congSuatTieuThu=congsuat;
	
	
	
	
}
//Luu du lieu vao file
string toFilerecord()const override{
	return id+"|"+tenLoai+"|"+hangSanXuat+"|"+dacTinhKyThuat+"|"+to_string(congSuatTieuThu);
}
//Load du lieu tu file vao va boc tach doi tuong tren RAM
void fromFilerecord(const string&record) override{
	vector<string> data=InputHelper::chia(record,'|');
	//file chia cat phai du 5 doi tuong
	if(data.size()>=5){
		setId(data[0]);
		tenLoai=data[1];
		hangSanXuat=data[2];
		dacTinhKyThuat=data[3];
		congSuatTieuThu=stod(data[4]);
	}
	
}
//ghi de ham in thong tin dang bang bieu cua Entity
void display() const override{
cout<<left<<setw(8)<<id<<"|"<<setw(25)<<(tenLoai.length()>22?tenLoai.substr(0,19)+"...":tenLoai)<<"|"
<<setw(15)<<hangSanXuat<<"|"<<setw(20)<<dacTinhKyThuat<<"|"<<setw(10)<<congSuatTieuThu<<endl;

}
//hien thi tieu de 
static void displayHeader(){
	cout<<left<<setw(8)<<"Ma Loai TB"<<"|"<<setw(25)<<"Ten loai Thiet Bi"<<"|"<<setw(15)<<"Hang san xuat"<<"|"<<setw(20)<<"Dac Tinh KT"
	<<"|"<<setw(10)<<"Cong suat "<<endl;
	//in ra vach ke ngang 85 ky tu -
	cout<<string(85,'-')<<endl;
}
};
