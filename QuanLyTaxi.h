#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "Taxi.h"
#include "Common.h"

using namespace std;

class QuanLyTaxi {
private:
	vector<Taxi> dsTaxi;
	string duongDanFile;

public:
	explicit QuanLyTaxi(string file = "data/taxi.txt") 
		:duongDanFile(file)
	{ }

	const vector<Taxi>& getDanhSach() const {
		return dsTaxi;
	}

	void them(const Taxi& t) {
		dsTaxi.push_back(t);
		ghiFile();
	}

	bool xoa(const string& bienSo) {
		auto it = remove_if(dsTaxi.begin(), dsTaxi.end(), [&](const Taxi& t) {
			return t.getBienSo() == bienSo;
			});
		if (it == dsTaxi.end()) return false;
		dsTaxi.erase(it, dsTaxi.end());

		ghiFile();
		return true;
	}

	void goBoTaiXeKhoiTatCaTaxi(const string& maTX) {
		for (auto& t : dsTaxi)
			t.xoaTaiXe(maTX);
		ghiFile();
	}

	const Taxi* timTheoBienSo(const string& bienSo) const {
		auto it = find_if(dsTaxi.begin(), dsTaxi.end(), [&](const Taxi& t) {
			return t.getBienSo() == bienSo;
			});
		return (it != dsTaxi.end()) ? &(*it) : nullptr;
	}

	Taxi* timTheoBienSo(const string& bienSo) {
		return const_cast<Taxi*>(static_cast<const QuanLyTaxi*>(this)->timTheoBienSo(bienSo));
	}

	bool coBienSo(const string& bienSo) {
		return timTheoBienSo(bienSo) != nullptr;
	}

	vector<const Taxi*> timTaxiPhuHop(int soKhachToiThieu = 1) const {
		vector<const Taxi*> ketQua;
		for (const auto& t : dsTaxi)
			if (t.ktSanSang() && t.getSucChua() >= soKhachToiThieu)
				ketQua.push_back(&t);

		sort(ketQua.begin(), ketQua.end(), [](const Taxi* a, const Taxi* b) {
			return a->getBienSo() < b->getBienSo();
			});
		return ketQua;
	}

	void docFile() {
		dsTaxi.clear();
		ifstream fin(duongDanFile);
		if (!fin.is_open()) return;

		string line;
		while (getline(fin, line)) {
			if (line.empty()) continue;

			auto p = split(line, '|');
			if (p.size() < 4) continue;

			try {
				const string& bienSo	= p[0];
				const int sucChua		= stoi(p[1]);
				const string& dsMaTaiXe = p[2];
				const string& trangThai = p[3];
				Taxi t(bienSo, sucChua);

				if (!dsMaTaiXe.empty())
					for (const auto& ma : split(dsMaTaiXe, ','))
						if (!ma.empty())
							t.themTaiXe(ma);
				t.setSanSang(trangThai == "1");

				dsTaxi.push_back(t);
			}
			catch (...) { 
				continue;
			}
		}
		fin.close();
	}

	void ghiFile() const {
		ofstream fout(duongDanFile, ios::trunc);
		for (const auto& t : dsTaxi)
			fout << t.getBienSo() << "|" << t.getSucChua() << "|" << t.dsMaTaiXeStr() << "|" << (t.ktSanSang() ? 1 : 0) << "\n";
		fout.close();
	}
};