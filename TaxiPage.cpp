#include "TaxiPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialog>
#include <QFormLayout>
#include <QMessageBox>
#include <QCheckBox>
#include <algorithm>

TaxiPage::TaxiPage(HeThongTaxi* ht, QWidget* parent) : QWidget(parent), heThong(ht) {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 16, 20, 16);

    auto* lblTitle = new QLabel("QUẢN LÝ TAXI", this);
    lblTitle->setStyleSheet("font-size:17px; font-weight:bold; color:#2C3E50;");
    root->addWidget(lblTitle);
    root->addSpacing(8);

    auto* toolbar = new QHBoxLayout();
    auto* btnThem = new QPushButton("+  Thêm taxi", this);
    btnThem->setObjectName("btnPrimary");
    auto* btnXoa = new QPushButton("Xóa taxi", this);
    btnXoa->setObjectName("btnDanger");
    auto* btnPhanCong = new QPushButton("Phân công tài xế", this);
    btnPhanCong->setObjectName("btnFlat");
    auto* btnHoanThanh = new QPushButton("Hoàn thành chuyến", this);
    btnHoanThanh->setObjectName("btnSuccess");
    auto* btnHuyChuyen = new QPushButton("Hủy chuyến", this);
    btnHuyChuyen->setObjectName("btnDanger");

    connect(btnHoanThanh, &QPushButton::clicked, this, &TaxiPage::onHoanThanh);
    connect(btnThem, &QPushButton::clicked, this, &TaxiPage::onThem);
    connect(btnXoa, &QPushButton::clicked, this, &TaxiPage::onXoa);
    connect(btnPhanCong, &QPushButton::clicked, this, &TaxiPage::onPhanCong);
    connect(btnHuyChuyen, &QPushButton::clicked, this, &TaxiPage::onHuyChuyen);

    toolbar->addWidget(btnThem);
    toolbar->addWidget(btnXoa);
    toolbar->addWidget(btnPhanCong);
    toolbar->addWidget(btnHoanThanh);
    toolbar->addWidget(btnHuyChuyen);
    toolbar->addStretch();
    root->addLayout(toolbar);
    root->addSpacing(10);

    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({ "Biển số", "Sức chứa", "Tài xế phân công", "Trạng thái" });
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    root->addWidget(table, 1);

    refresh();
}

void TaxiPage::refresh() {
    const auto& ds = heThong->taxi().getDanhSach();
    table->setRowCount((int)ds.size());
    for (int i = 0; i < (int)ds.size(); ++i) {
        const auto& t = ds[i];
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(t.getBienSo())));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(t.getSucChua()) + " chỗ"));

        QStringList tens;
        for (const auto& maTX : t.getDsMaTaiXe()) {
            for (const auto& tx : heThong->taiXe().getDanhSach()) {
                if (tx.getMaTX() == maTX) {
                    tens << QString::fromStdString(tx.getHoTen());
                    break;
                }
            }
        }
        table->setItem(i, 2, new QTableWidgetItem(tens.join(", ")));

        auto* itTrangThai = new QTableWidgetItem(t.ktSanSang() ? "Sẵn sàng" : "Đang phục vụ");
        if (t.ktSanSang()) itTrangThai->setForeground(QColor("#438F78"));
        table->setItem(i, 3, itTrangThai);
    }
}

void TaxiPage::onThem() {
    QDialog dlg(this);
    dlg.setWindowTitle("Thêm taxi mới");
    auto* form = new QFormLayout(&dlg);
    auto* txtBienSo = new QLineEdit(&dlg);
    auto* spinSucChua = new QSpinBox(&dlg);
    spinSucChua->setRange(2, 16);
    spinSucChua->setValue(4);
    form->addRow("Biển số xe:", txtBienSo);
    form->addRow("Sức chứa:", spinSucChua);

    auto* btnRow = new QHBoxLayout();
    auto* btnCancel = new QPushButton("Hủy", &dlg); btnCancel->setObjectName("btnFlat");
    auto* btnSave = new QPushButton("Lưu", &dlg); btnSave->setObjectName("btnPrimary");
    connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dlg, &QDialog::accept);
    btnRow->addStretch(); btnRow->addWidget(btnCancel); btnRow->addWidget(btnSave);
    form->addRow(btnRow);

    if (dlg.exec() == QDialog::Accepted) {
        std::string bienSo = txtBienSo->text().toStdString();
        if (bienSo.empty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập biển số xe!");
            return;
        }
        if (heThong->taxi().coBienSo(bienSo)) {
            QMessageBox::warning(this, "Lỗi", "Biển số này đã tồn tại!");
            return;
        }
        heThong->taxi().them(Taxi(bienSo, spinSucChua->value()));
        heThong->taxi().ghiFile(); // Ghi file lưu dữ liệu
        refresh();
    }
}

void TaxiPage::onXoa() {
    int row = table->currentRow();
    if (row < 0) { QMessageBox::information(this, "Thông báo", "Vui lòng chọn một taxi để xóa."); return; }
    std::string bienSo = table->item(row, 0)->text().toStdString();
    if (heThong->chuyenXe().taxiDangChay(bienSo)) {
        QMessageBox::warning(this, "Không thể xóa",
            QString("Taxi \"%1\" đang thực hiện một chuyến xe, không thể xóa lúc này.")
            .arg(QString::fromStdString(bienSo)));
        return;
    }
    auto ret = QMessageBox::question(this, "Xác nhận xóa",
        QString("Bạn có chắc muốn xóa taxi \"%1\"?").arg(QString::fromStdString(bienSo)));
    if (ret == QMessageBox::Yes) {
        heThong->taxi().xoa(bienSo);
        heThong->taxi().ghiFile();
        refresh();
    }
}

void TaxiPage::onPhanCong() {
    int row = table->currentRow();
    if (row < 0) { QMessageBox::information(this, "Thông báo", "Vui lòng chọn một taxi để phân công tài xế."); return; }
    std::string bienSo = table->item(row, 0)->text().toStdString();
    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo);
    if (!taxi) return;

    QDialog dlg(this);
    dlg.setWindowTitle("Phân công tài xế cho taxi " + QString::fromStdString(bienSo));
    dlg.setMinimumWidth(320);
    auto* lay = new QVBoxLayout(&dlg);
    lay->addWidget(new QLabel("Chọn tài xế được phân công lái xe này:", &dlg));

    std::vector<QCheckBox*> checks;
    for (auto& tx : heThong->taiXe().getDanhSach()) {
        bool phuHop = hangBangLaiPhuHopSucChua(tx.getBangLai(), taxi->getSucChua());

        QString nhan = QString::fromStdString(tx.getMaTX() + " - " + tx.getHoTen()
            + " (Bằng " + tx.getBangLai() + ")");
        if (!phuHop) nhan += "  [Không đủ điều kiện - xe quá số chỗ cho phép]";

        auto* cb = new QCheckBox(nhan, &dlg);
        cb->setProperty("maTX", QString::fromStdString(tx.getMaTX()));

        bool assigned = std::find(taxi->getDsMaTaiXe().begin(), taxi->getDsMaTaiXe().end(), tx.getMaTX()) != taxi->getDsMaTaiXe().end();
        cb->setChecked(assigned);

        if (!phuHop) {
            cb->setEnabled(false);
            cb->setChecked(false);
            cb->setStyleSheet("color: #B8503F;");
        }

        lay->addWidget(cb);
        checks.push_back(cb);
    }
    auto* btnRow = new QHBoxLayout();
    auto* btnCancel = new QPushButton("Hủy", &dlg); btnCancel->setObjectName("btnFlat");
    auto* btnSave = new QPushButton("Lưu", &dlg); btnSave->setObjectName("btnPrimary");
    connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dlg, &QDialog::accept);
    btnRow->addStretch(); btnRow->addWidget(btnCancel); btnRow->addWidget(btnSave);
    lay->addLayout(btnRow);

    if (dlg.exec() == QDialog::Accepted) {
        for (auto* cb : checks) {
            std::string maTX = cb->property("maTX").toString().toStdString();
            if (cb->isChecked()) taxi->themTaiXe(maTX);
            else taxi->xoaTaiXe(maTX);
        }
        heThong->taxi().ghiFile();
        refresh();
    }
}

void TaxiPage::onHoanThanh() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn một taxi.");
        return;
    }
    std::string bienSo = table->item(row, 0)->text().toStdString();
    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo);
    if (!taxi) return;

    if (taxi->ktSanSang()) {
        QMessageBox::information(this, "Thông báo",
            "Taxi này đang ở trạng thái sẵn sàng, không có chuyến nào cần hoàn thành.");
        return;
    }

    auto ret = QMessageBox::question(this, "Xác nhận",
        QString("Xác nhận taxi \"%1\" đã hoàn thành chuyến và sẵn sàng nhận khách mới?")
        .arg(QString::fromStdString(bienSo)));
    if (ret != QMessageBox::Yes) return;

    ChuyenXe* chuyen = heThong->chuyenXe().timChuyenDangDi(bienSo);
    if (chuyen) heThong->chuyenXe().hoanThanhChuyen(chuyen->getMaChuyen());

    taxi->setSanSang(true);
    heThong->taxi().ghiFile();
    refresh();
}

void TaxiPage::onHuyChuyen() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn một taxi.");
        return;
    }
    std::string bienSo = table->item(row, 0)->text().toStdString();
    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo);
    if (!taxi) return;

    if (taxi->ktSanSang()) {
        QMessageBox::information(this, "Thông báo",
            "Taxi này đang ở trạng thái sẵn sàng, không có chuyến nào để hủy.");
        return;
    }

    ChuyenXe* chuyen = heThong->chuyenXe().timChuyenDangDi(bienSo);
    QString maChuyen = chuyen ? QString::fromStdString(chuyen->getMaChuyen()) : "không xác định";

    auto ret = QMessageBox::warning(this, "Xác nhận hủy chuyến",
        QString("Xác nhận HỦY chuyến xe %1 của taxi \"%2\"?\nHành động này không thể hoàn tác.")
        .arg(maChuyen, QString::fromStdString(bienSo)),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret != QMessageBox::Yes) return;

    if (chuyen) heThong->chuyenXe().huyChuyen(chuyen->getMaChuyen());
    taxi->setSanSang(true);
    heThong->taxi().ghiFile();
    refresh();
}