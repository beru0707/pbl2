#pragma once
#include <string>
#include "Common.h"

using namespace std;

class TaiXe {
private:
	string maTX;
	string ten;
	string hoDem;
	Date ngaySinh;
	string diaChi;
	string soDienthoai;
	string bangLai;
	string soGPLX;
	TrangThai trangThai;

public:
	TaiXe()
		:trangThai(DANG_HOAT_DONG)
	{ }
	TaiXe(const string& ma, const string& ten, const string& hodem, Date ngaysinh,  const string& diachi, 
		  const string& sdt, const string& bang, const string& gplx, TrangThai t)
		: maTX(ma), ten(ten), hoDem(hodem), ngaySinh(ngaysinh), diaChi(diachi), 
		soDienthoai(sdt), bangLai(bang), soGPLX(gplx), trangThai(t)
	{ }

	//get
	const string& getMaTX() const {
		return maTX;
	}
	const string& getTen() const {
		return ten;
	}
	const string& getHoDem() const {
		return hoDem;
	}
	string getHoTen() const {
		return hoDem + " " + ten;
	}
	Date getNgaySinh() const {
		return ngaySinh;
	}
	const string& getDiaChi() const {
		return diaChi;
	}
	const string& getSoDienThoai() const {
		return soDienthoai;
	}
	const string& getBangLai() const {
		return bangLai;
	}
	const string& getSoGPLX() const {
		return soGPLX;
	}
	TrangThai getTrangThai() const {
		return trangThai;
	}

	//set
	void setTen(string t) {
		ten = t;
	}
	void setHoDem(string h) {
		hoDem = h;
	}
	void setNgaySinh(Date d) {
		ngaySinh = d;
	}
	void setDiaChi(string dc) {
		diaChi = dc;
	}
	void setSoDienthoai(string sdt) {
		soDienthoai = sdt;
	}
	void setBangLai(string bl) {
		bangLai = bl;
	}
	void setSoGPLX(string so) {
		soGPLX = so;
	}
};