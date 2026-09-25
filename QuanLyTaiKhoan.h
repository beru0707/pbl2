#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include "TaiKhoan.h"

using namespace std;

class QuanLyTaiKhoan {
private:
	vector<TaiKhoan> dsTaiKhoan;
	string duongDanFile;

public:
	explicit QuanLyTaiKhoan(string file = "data/taikhoan.txt")
		: duongDanFile(file)
	{ }

	unique_ptr<NguoiDung> ktDangNhap(const string& tk, const string& mk) const {
		for (const auto& acc : dsTaiKhoan)
			if (acc.tenDangNhap == tk && acc.matKhau == mk) {
				if (acc.vaiTro == QUAN_LY)
					return make_unique<QuanLy>(tk);
				return make_unique<DieuHanh>(tk);
			}
		return nullptr;
	}

	void docFile() {
		dsTaiKhoan.clear();
		ifstream fin(duongDanFile);
		if (!fin.is_open()) return;

		string line;
		while (getline(fin, line)) {
			if (line.empty()) return;

			auto p = split(line, '|');
			if (p.size() < 3) continue;

			try {
				const string& tenDangNhap = p[0];
				const string& matKhau = p[1];
				int vaiTro = stoi(p[2]);

				TaiKhoan t(tenDangNhap, matKhau, static_cast<VaiTro>(vaiTro));
				dsTaiKhoan.push_back(t);
			}
			catch (...) {
				continue;
			}
		}
		fin.close();
	}
};