#pragma once
#include <string>
#include <vector>
#include "Common.h"

using namespace std;

class TaiKhoan {
public :
	string tenDangNhap;
	string matKhau;
	VaiTro vaiTro;
	TaiKhoan()
		:vaiTro(DIEU_HANH)
	{ }
	TaiKhoan(string tk, string mk, VaiTro vt)
		:tenDangNhap(tk), matKhau(mk), vaiTro(vt)
	{ }
};

class NguoiDung {
protected:
	string tenDangNhap;
	VaiTro vaiTro;

public:
	NguoiDung(string tk, VaiTro vt)
		: tenDangNhap(tk), vaiTro(vt)
	{ }
	virtual ~NguoiDung() {}

	//get
	const string& getTenDangNhap() const {
		return tenDangNhap;
	}
	VaiTro getVaiTro() const {
		return vaiTro;
	}

	virtual vector<ChucNang> danhSachChucNang() const = 0;
	virtual string tenVaiTro() const = 0;
};

class QuanLy : public NguoiDung {
public:
	QuanLy(string tk)
		: NguoiDung(tk, QUAN_LY) 
	{ }
	vector<ChucNang> danhSachChucNang() const override {
		return {
			CN_QUAN_LY_TAI_XE,
			CN_QUAN_LY_TAXI,
			CN_DAT_CHUYEN_XE,
			CN_THONG_KE
		};
	}
	string tenVaiTro() const override {
		return "Quản lý";
	}
};

class DieuHanh : public NguoiDung {
public:
	DieuHanh(string tk)
		: NguoiDung(tk, DIEU_HANH)
	{ }
	vector<ChucNang> danhSachChucNang() const override {
		return {
			CN_DAT_CHUYEN_XE,
		};
	}
	string tenVaiTro() const override {
		return "Điều hành";
	}
};