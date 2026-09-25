#pragma once
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "TaiXe.h"

using namespace std;

class QuanLyTaiXe {
private:
	vector<TaiXe> dsTaiXe;
	string duongDanFile;

public:
	explicit QuanLyTaiXe(string file = "data/taixe.txt")
		: duongDanFile(file)
	{ }

	const vector<TaiXe>& getDanhSach() const {
		return dsTaiXe;
	}

	void them(const TaiXe& tx) {
		dsTaiXe.push_back(tx);
		ghiFile();
	}

	bool sua(const string& maTX, const TaiXe& newTX) {
		for (auto& t : dsTaiXe) 
			if (t.getMaTX() == maTX) {
				t = newTX;
				ghiFile();
				return true;	
			}
		return false;
	}

	bool xoa(const string& maTX) {
		auto it = remove_if(dsTaiXe.begin(), dsTaiXe.end(), [&](const TaiXe& t) {
			return t.getMaTX() == maTX;
			});
		if (it == dsTaiXe.end()) return false;
		dsTaiXe.erase(it, dsTaiXe.end());

		ghiFile();
		return true;
	}

	string newMaTX() const {
		int maxId = 0;
		for (const auto& t : dsTaiXe) {
			const string& ma = t.getMaTX();

			if (ma.size() > 2 && ma.rfind("TX", 0) == 0)
				try {
					int id = stoi(ma.substr(2));
					maxId = max(maxId, id);
				}
				catch (...) {
					continue;
				}

		}

		ostringstream ans;
		ans << "TX" << setfill('0') << setw(3) << (maxId + 1);
		return ans.str();
	}

	bool coMaTX(const string& maTX) const {
		for (const auto& t : dsTaiXe) 
			if (t.getMaTX() == maTX) return true;
		return false;
	}

	bool coSoGPLX(const string& gplx, const string& boQuaMa = "") const {
		for (const auto& t : dsTaiXe)
			if (t.getSoGPLX() == gplx && t.getMaTX() != boQuaMa) return true;
		return false;
	}

	vector <TaiXe> timKiem(const string& key) const {
		if (key.empty()) return dsTaiXe;

		vector<TaiXe> ketQua;
		string tk = toLowerStr(key);

		for (const auto& t : dsTaiXe) {
			string hoTen = toLowerStr(t.getHoTen());
			string gplx = toLowerStr(t.getSoGPLX());
			string bl = toLowerStr(t.getBangLai());
			string tuoi = to_string(tinhTuoi(t.getNgaySinh()));

			if (hoTen.find(tk) != std::string::npos || gplx.find(tk) != std::string::npos 
				|| bl.find(tk) != std::string::npos || tuoi.find(tk) != std::string::npos)
				ketQua.push_back(t);
		}
		return ketQua;
	}

	void docFile() {
		dsTaiXe.clear();
		ifstream fin(duongDanFile);
		if (!fin.is_open())	return;

		string line;
		while (getline(fin, line)) {
			if (line.empty()) continue;

			auto p = split(line, '|');
			if (p.size() < 9) continue;

			try {
				const string& maTX	   = p[0];
				const string& ten	   = p[1];
				const string& hoDem	   = p[2];
				const string& ngaySinh = p[3];
				const string& diaChi   = p[4];
				const string& sdt	   = p[5];
				const string& bangLai  = p[6];
				const string& gplx	   = p[7];
				int trangThai		   = stoi(p[8]);

				stringstream ss(ngaySinh);
				char delim;
				Date d{};
				ss >> d.day >> delim >> d.month >> delim >> d.year;

				TaiXe t(maTX, ten, hoDem, d, diaChi, sdt, bangLai, gplx, static_cast<TrangThai>(trangThai));
				dsTaiXe.push_back(t);
			}
			catch (...) {
				continue;
			}

		}
		fin.close();
	}

	void ghiFile() const {
		ofstream fout(duongDanFile, ios::trunc);
		for (const auto& t : dsTaiXe)
			fout << t.getMaTX() << "|" << t.getTen() << "|" << t.getHoDem() << "|"
			<< t.getNgaySinh().toString() << "|" << t.getDiaChi() << "|"
			<< t.getSoDienThoai() << "|" << t.getBangLai() << "|" << t.getSoGPLX()
			<< "|" << int(t.getTrangThai()) << "\n";
		fout.close();
	}
};