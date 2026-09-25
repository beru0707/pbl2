#pragma once
#include <QWidget>
#include <memory>
#include "HeThongTaxi.h"
#include "TaiKhoan.h"

class QLineEdit;
class QLabel;

class LoginWidget : public QWidget {
    Q_OBJECT
public:
    explicit LoginWidget(HeThongTaxi* ht, QWidget* parent = nullptr);

signals:
    void dangNhapThanhCong(std::shared_ptr<NguoiDung> nguoiDung);

private slots:
    void onDangNhap();

private:
    HeThongTaxi* heThong;
    QLineEdit* txtTaiKhoan;
    QLineEdit* txtMatKhau;
    QLabel* lblLoi;
    QPixmap bgPixmap;
};