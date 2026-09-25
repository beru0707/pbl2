#pragma once
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include "ChuyenXe.h"

using namespace std;

class QuanLyChuyenXe {
private:
	vector<ChuyenXe> dsChuyenXe;
	string duongDanFile;

	bool capNhatTrangThai(const string& maChuyen, TrangThaiChuyen trangThaiMoi) {
		for (auto& cx : dsChuyenXe) {
			if (cx.getMaChuyen() == maChuyen) {
				cx.setTrangThai(trangThaiMoi);
				ghiFile();
				return true;
			}
		}
		return false;
	}

public:
	explicit QuanLyChuyenXe(string file = "data/chuyenxe.txt")
		: duongDanFile(file)
	{ }

	const vector<ChuyenXe>& getDanhSach() const {
		return dsChuyenXe;
	}

	void themChuyen(const ChuyenXe& cx) { 
		dsChuyenXe.push_back(cx); 
		ghiFile(); 
	}

	bool hoanThanhChuyen(const std::string& maChuyen) {
		return capNhatTrangThai(maChuyen, HOAN_THANH);
	}

	bool huyChuyen(const std::string& maChuyen) {
		return capNhatTrangThai(maChuyen, DA_HUY);
	}

	string newMaCX() const {
		int maxId = 0;
		for (const auto& cx : dsChuyenXe) {
			const string& ma = cx.getMaChuyen();

			if (ma.size() > 2 && ma.rfind("CX", 0) == 0) 
				try {
					int id = stoi(ma.substr(2));
					maxId = max(maxId, id);
				}
				catch (...) {
					continue;
				}
		}

		ostringstream ans;
		ans << "CX" << setfill('0') << setw(4) << (maxId + 1);
		return ans.str();
	}

	vector<const ChuyenXe*> theoTaxi(const string& bienSo) const {
		vector<const ChuyenXe*> ketQua;
		for (const auto& cx : dsChuyenXe)
			if (cx.getBienSoTaxi() == bienSo)
				ketQua.push_back(&cx);
		return ketQua;
	}

	const ChuyenXe* timChuyenDangDi(const string& bienSo) const {
		for (const auto& cx : dsChuyenXe)
			if (cx.getBienSoTaxi() == bienSo && cx.getTrangThai() == DANG_DI)
				return &cx;
		return nullptr;
	}

	ChuyenXe* timChuyenDangDi(const string& bienSo) {
		return const_cast<ChuyenXe*>(static_cast<const QuanLyChuyenXe*>(this)->timChuyenDangDi(bienSo));
	}

	bool taxiDangChay(const string& bienSo) const {
		return timChuyenDangDi(bienSo) != nullptr;
	}

	bool taiXeDangChay(const string& maTX) const {
		for (const auto& cx : dsChuyenXe)
			if (cx.getMaTaiXe() == maTX && cx.getTrangThai() == DANG_DI)
				return true;
		return false;
	}

	double tongDoanhThu() const {
		double tong = 0;
		for (const auto& cx : dsChuyenXe)
			if (cx.getTrangThai() != DA_HUY)
				tong += cx.getCuocPhi();
		return tong;
	}

	void docFile() {
		dsChuyenXe.clear();
		ifstream fin(duongDanFile);
		if (!fin.is_open()) return;

		string line;
		while (getline(fin, line)) {
			if (line.empty()) continue;
			auto p = split(line, '|');
			if (p.size() < 11) continue;

			const string& maChuyen	   = p[0];
			const string& tenKhachHang = p[1];
			const string& sdtKhachHang = p[2];
			const string& bienSoTaxi   = p[3];
			const string& thoiDiem	   = p[4];
			const string& diemDon	   = p[5];
			const string& diemDen	   = p[6];
			double khoangCach		   = stod(p[7]);
			double cuocPhi			   = stod(p[8]);
			int trangThai			   = stoi(p[9]);
			const string& maTX		   = p[10];

			ChuyenXe cx(maChuyen, tenKhachHang, sdtKhachHang, bienSoTaxi,
				thoiDiem, diemDon, diemDen, khoangCach, cuocPhi, maTX);

			cx.setTrangThai(static_cast<TrangThaiChuyen>(trangThai));
			dsChuyenXe.push_back(cx);
		}
	}

	void ghiFile() const {
		ofstream fout(duongDanFile, ios::trunc);
		for (const auto& cx : dsChuyenXe)
			fout << cx.getMaChuyen() << "|" << cx.getTenKhachHang() << "|" << cx.getSdtKhachHang()
			<< "|" << cx.getBienSoTaxi() << "|" << cx.getThoiDiem() << "|" << cx.getDiemDon()
			<< "|" << cx.getDiemDen() << "|" << cx.getKhoangCach() << "|" << cx.getCuocPhi()
			<< "|" << int(cx.getTrangThai()) << "|" << cx.getMaTaiXe() << "\n";
	}
};