#pragma once
#include <QWidget>
#include "HeThongTaxi.h"

class QLineEdit;
class QDoubleSpinBox;
class QSpinBox;
class QComboBox;
class QVBoxLayout;
class QPushButton;

class BookingPage : public QWidget {
        Q_OBJECT
public:
    explicit BookingPage(HeThongTaxi* ht, QWidget* parent = nullptr);
    void refresh();

private slots:
    void onTimTaxi();
    void onXacNhan();

private:
    HeThongTaxi* heThong;
    QLineEdit* txtTenKhach;
    QLineEdit* txtSdt;
    QLineEdit* txtDiemDon;
    QLineEdit* txtDiemDen;
    QDoubleSpinBox* spinKhoangCach;
    QSpinBox* spinSoKhach;
    QComboBox* cbTaiXe;
    QVBoxLayout* dsTaxiLayout;
    QWidget* dsTaxiContainer;
    QPushButton* btnXacNhan;
    QString bienSoDangChon;

    void hienThiDanhSachTaxi();
    void chonTaxi(const QString& bienSo);
};