#pragma once

#include <QWidget>
#include "HeThongTaxi.h"

class QComboBox;
class QTableWidget;
class QLabel;

class StatsPage : public QWidget {
	Q_OBJECT

public:
	explicit StatsPage(HeThongTaxi* ht, QWidget* parent = nullptr);
	void refresh();

private:
	void onChonTaxi(int index);

	HeThongTaxi* heThong;
	QComboBox* cbTaxi;
	QTableWidget* table;
	QLabel* lblTong;
};