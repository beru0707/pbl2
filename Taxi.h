#pragma once
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Taxi {
private:
	string bienSo;
	int sucChua;
	vector<string> dsMaTaiXe;
	bool dangSanSang;

public:
	Taxi()
		:sucChua(4), dangSanSang(true)
	{ }
	Taxi(const string& bs, int sc)
		:bienSo(bs), sucChua(sc), dangSanSang(true)
	{ }
	
	//get
	const string& getBienSo() const {
		return bienSo;
	}
	int getSucChua() const {
		return sucChua;
	}
	const vector<string>& getDsMaTaiXe() const {
		return dsMaTaiXe;
	}
	bool ktSanSang() const {
		return dangSanSang;
	}
	string dsMaTaiXeStr() const{
		string s;
		for (size_t i = 0; i < dsMaTaiXe.size(); i++) {
			s += dsMaTaiXe[i];
			if (i + 1 < dsMaTaiXe.size()) s += ",";
		}
		return s;
	}

	//set
	void setSanSang(bool ss) {
		dangSanSang = ss;
	}

	void themTaiXe(const string& maTX) {
		if (find(dsMaTaiXe.begin(), dsMaTaiXe.end(), maTX) == dsMaTaiXe.end())
			dsMaTaiXe.push_back(maTX);
	}
	void xoaTaiXe(const string& maTX) {
		dsMaTaiXe.erase(remove(dsMaTaiXe.begin(), dsMaTaiXe.end(), maTX), dsMaTaiXe.end());
	}
};	