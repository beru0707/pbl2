#pragma once
#include <string>
#include "Common.h"

using namespace std;

class ChuyenXe {
private:
	string maChuyen;
	string tenKhachHang;
	string sdtKhachHang;
	string bienSoTaxi;
	string thoiDiem;
	string diemDon;
	string diemDen;
	double khoangCach = 0;
	double cuocPhi = 0;
	TrangThaiChuyen trangThai = DANG_DI;
	string maTaiXe;

public:
	ChuyenXe() {}
	ChuyenXe(const string& ma, const string& ten, const string& sdt, const string& bienSo, const string& time, 
			 const string& start, const string& end, double kc, double giaCuoc = 0.0, const string& maTX = "")
		: maChuyen(ma), tenKhachHang(ten), sdtKhachHang(sdt), bienSoTaxi(bienSo), thoiDiem(time), diemDon(start), diemDen(end), khoangCach(kc), maTaiXe(maTX) {
		cuocPhi = (giaCuoc > 0.0) ? giaCuoc : tinhCuocPhi();
	}

	double tinhCuocPhi() const {
		const double GIA_XE = 11000.0;
		const double GIA_KM = 15000.0;
		return GIA_XE + GIA_KM * khoangCach;
	}

	//get
	const string& getMaChuyen() const {
		return maChuyen;
	}
	const string& getTenKhachHang() const {
		return tenKhachHang;
	}
	const string& getSdtKhachHang() const {
		return sdtKhachHang;
	}
	const string& getBienSoTaxi() const {
		return bienSoTaxi;
	}
	const string& getThoiDiem() const {
		return thoiDiem;
	}
	const string& getDiemDon() const {
		return diemDon;
	}
	const string& getDiemDen() const {
		return diemDen;
	}
	double getKhoangCach() const {
		return khoangCach;
	}
	double getCuocPhi() const {
		return cuocPhi;
	}
	TrangThaiChuyen getTrangThai() const {
		return trangThai;
	}
	const string& getMaTaiXe() const {
		return maTaiXe;
	}

	//set
	void setCuocPhi(double c) {
		cuocPhi = c;
	}
	void setTrangThai(TrangThaiChuyen t) {
		trangThai = t;
	}
	void setMaTaiXe(const string& ma) {
		maTaiXe = ma;
	}
};