#pragma once
#include <QWidget>
#include "HeThongTaxi.h"

class QTableWidget;
class QLineEdit;

class DriverPage : public QWidget {
	Q_OBJECT

public:
	explicit DriverPage(HeThongTaxi* ht, QWidget* parent = nullptr);
	void refresh();

private:
	void onThem();
	void onSua();
	void onXoa();
	void onTimKiem(const QString& key);

	HeThongTaxi* heThong;
	QTableWidget* table;
	QLineEdit* txtSearch;
	void fillTable(const std::vector<TaiXe>& dsTaixe);
};