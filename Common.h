#pragma once
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

struct Date {
	int day = 1;
	int month = 1;
	int year = 2000;
	string toString() const {
		auto pad = [](int n) {
			return (n < 10 ? "0" : "") + to_string(n);
		};
		return pad(day) + "/" +pad(month) + "/" + to_string(year);
	}
};

inline int tinhTuoi(const Date& ngaySinh) {
	time_t t = time(nullptr);
	tm now;
	localtime_s(&now, &t);

	int nam = now.tm_year + 1900;
	int thang = now.tm_mon + 1;
	int ngay = now.tm_mday;
	int tuoi = nam - ngaySinh.year;

	if (thang < ngaySinh.month || (thang == ngaySinh.month && ngay < ngaySinh.day)) tuoi--;
	return tuoi;
}

enum TrangThai {
	DANG_HOAT_DONG = 0,
	NGHI = 1,
	NGUNG_HOAT_DONG = 2
};

inline string trangThaiToStr(TrangThai t) {
	switch (t){
	case DANG_HOAT_DONG: return "Đang hoạt động";
	case NGHI: return "Nghỉ";
	default: return "Ngừng hoạt động";
	}
}

enum TrangThaiChuyen {
	DANG_DI = 0,
	HOAN_THANH = 1,
	DA_HUY = 2
};

inline string trangThaiChuyenToStr(TrangThaiChuyen t) {
	switch (t) {
	case DANG_DI: return "Đang đi";
	case HOAN_THANH: return "Hoàn thành";
	default: return "Đã hủy";
	}
}

inline bool hangBangLaiHopLeTaxi(const string& bangLai) {
	static const vector<string> hopLe = { "B2", "C", "D", "E" };
	return find(hopLe.begin(), hopLe.end(), bangLai) != hopLe.end();
}

inline bool hangBangLaiPhuHopSucChua(const string& bangLai, int sucChua) {
	if (bangLai == "B2") return sucChua <= 8;
	if (bangLai == "D") return sucChua <= 29;
	if (bangLai == "E" || bangLai == "C") return true;
	return false;
}

inline vector<string> split(const string& s, char delim) {
	vector<string> ans;
	stringstream ss(s);
	string temp;
	while (getline(ss, temp, delim))
		ans.push_back(temp);
	return ans;
}

inline string toLowerStr(string str) {
	transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
		return ::tolower(c);
		});
	return str;
}

enum VaiTro {
	QUAN_LY = 0,
	DIEU_HANH = 1
};

enum ChucNang {
	CN_QUAN_LY_TAI_XE,
	CN_QUAN_LY_TAXI,
	CN_DAT_CHUYEN_XE,
	CN_THONG_KE
};