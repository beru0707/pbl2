#include "DriverDialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QRegularExpression>

DriverDialog::DriverDialog(QWidget* parent, const TaiXe* existing)
    : QDialog(parent) {
    setWindowTitle(existing ? "Sửa thông tin tài xế" : "Thêm tài xế mới");
    setMinimumWidth(380);

    auto* root = new QVBoxLayout(this);
    auto* form = new QFormLayout();
    form->setSpacing(10);

    txtMaTX = new QLineEdit(this);
    if (existing) { txtMaTX->setText(QString::fromStdString(existing->getMaTX())); }
    txtMaTX->setReadOnly(true);
    form->addRow("Mã tài xế:", txtMaTX);

    txtTen = new QLineEdit(this);
    txtHoDem = new QLineEdit(this);
    if (existing) {
        txtTen->setText(QString::fromStdString(existing->getTen()));
        txtHoDem->setText(QString::fromStdString(existing->getHoDem()));
    }
    form->addRow("Tên:", txtTen);
    form->addRow("Họ & đệm:", txtHoDem);

    dateNgaySinh = new QDateEdit(this);
    dateNgaySinh->setDisplayFormat("dd/MM/yyyy");
    dateNgaySinh->setCalendarPopup(true);
    if (existing) {
        auto d = existing->getNgaySinh();
        dateNgaySinh->setDate(QDate(d.year, d.month, d.day));
    }
    else {
        dateNgaySinh->setDate(QDate(2000, 1, 1));
    }
    form->addRow("Ngày sinh:", dateNgaySinh);

    txtDiaChi = new QLineEdit(this);
    txtSdt = new QLineEdit(this);
    cbBangLai = new QComboBox(this);
    cbBangLai->addItems({ "B2", "C", "D", "E" });
    cbBangLai->setToolTip("Chỉ các hạng B2 trở lên mới được hành nghề lái xe Taxi");
    txtGPLX = new QLineEdit(this);
    if (existing) {
        txtDiaChi->setText(QString::fromStdString(existing->getDiaChi()));
        txtSdt->setText(QString::fromStdString(existing->getSoDienThoai()));
        int idx = cbBangLai->findText(QString::fromStdString(existing->getBangLai()));
        cbBangLai->setCurrentIndex(idx >= 0 ? idx : 0);
        txtGPLX->setText(QString::fromStdString(existing->getSoGPLX()));
    }
    else {
        cbBangLai->setCurrentText("B2");
    }
    form->addRow("Địa chỉ:", txtDiaChi);
    form->addRow("Số điện thoại:", txtSdt);
    form->addRow("Bằng lái:", cbBangLai);
    form->addRow("Số GPLX:", txtGPLX);

    cbTrangThai = new QComboBox(this);
    cbTrangThai->addItems({ "Đang hoạt động", "Nghỉ", "Ngừng hoạt động" });
    if (existing) cbTrangThai->setCurrentIndex((int)existing->getTrangThai());
    form->addRow("Trạng thái:", cbTrangThai);

    root->addLayout(form);

    auto* btnRow = new QHBoxLayout();
    auto* btnCancel = new QPushButton("Hủy", this);
    btnCancel->setObjectName("btnFlat");
    auto* btnSave = new QPushButton("Lưu", this);
    btnSave->setObjectName("btnPrimary");
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, this, [this]() {
        if (txtTen->text().trimmed().isEmpty() || txtHoDem->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập đầy đủ Họ & đệm và Tên.");
            return;
        }
        if (txtDiaChi->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập địa chỉ.");
            return;
        }

        static const QRegularExpression sdtRegex("^0[0-9]{9}$");
        QString sdt = txtSdt->text().trimmed();
        sdt.remove(' ');
        if (!sdtRegex.match(sdt).hasMatch()) {
            QMessageBox::warning(this, "Lỗi",
                "Số điện thoại không hợp lệ (phả bắt đầu bằng 0, gồm đúng 10 chữ số).");
            return;
        }

        if (cbBangLai->currentText().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập loại bằng lái.");
            return;
        }
        if (txtGPLX->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập số GPLX.");
            return;
        }

        QDate ns = dateNgaySinh->date();
        if (ns > QDate::currentDate()) {
            QMessageBox::warning(this, "Lỗi", "Ngày sinh không thể ở tương lai.");
            return;
        }
        int tuoi = ns.daysTo(QDate::currentDate()) / 365;
        if (tuoi < 18 || tuoi > 70) {
            QMessageBox::warning(this, "Lỗi", "Tài xế phải trong độ tuổi từ 18 đến 70.");
            return;
        }

        accept();
        });
    btnRow->addStretch();
    btnRow->addWidget(btnCancel);
    btnRow->addWidget(btnSave);
    root->addLayout(btnRow);
}

TaiXe DriverDialog::ketQua() const {
    Date d{ dateNgaySinh->date().day(), dateNgaySinh->date().month(), dateNgaySinh->date().year() };
    return TaiXe(
        txtMaTX->text().toStdString(),
        txtTen->text().toStdString(),
        txtHoDem->text().toStdString(),
        d,
        txtDiaChi->text().toStdString(),
        txtSdt->text().toStdString(),
        cbBangLai->currentText().toStdString(),
        txtGPLX->text().toStdString(),
        (TrangThai)cbTrangThai->currentIndex()
    );
}
