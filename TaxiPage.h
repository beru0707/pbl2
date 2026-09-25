#pragma once
#include <QWidget>
#include "HeThongTaxi.h"

class QTableWidget;

class TaxiPage : public QWidget {
	Q_OBJECT

public:
	explicit TaxiPage(HeThongTaxi* ht, QWidget* parent = nullptr);
	void refresh();

private:
	void onThem();
	void onXoa();
	void onPhanCong();
	void onHoanThanh();
	void onHuyChuyen();

	HeThongTaxi* heThong;
	QTableWidget* table;
};