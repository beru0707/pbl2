#pragma once
#include "QuanLyTaiKhoan.h"
#include "QuanLyTaxi.h"
#include "QuanLyTaiXe.h"
#include "QuanLyChuyenXe.h"

class HeThongTaxi {
private:
	QuanLyTaiKhoan qlTaiKhoan;
	QuanLyTaxi qlTaxi;
	QuanLyTaiXe qlTaiXe;
	QuanLyChuyenXe qlChuyenXe;

public:
	HeThongTaxi() = default;

	void khoiDong() {
		qlTaiKhoan.docFile();
		qlTaxi.docFile();
		qlTaiXe.docFile();
		qlChuyenXe.docFile();
	}

	QuanLyTaiKhoan& taiKhoan() {
		return qlTaiKhoan;
	}
	QuanLyTaxi& taxi() {
		return qlTaxi;
	}
	QuanLyTaiXe& taiXe() {
		return qlTaiXe;
	}
	QuanLyChuyenXe& chuyenXe() {
		return qlChuyenXe;
	}
};