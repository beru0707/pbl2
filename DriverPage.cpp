#include "DriverPage.h"
#include "DriverDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QFrame>

DriverPage::DriverPage(HeThongTaxi* ht, QWidget* parent)
    : QWidget(parent), heThong(ht) {

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 16, 20, 16);

    auto* lblTitle = new QLabel("QUẢN LÝ TÀI XẾ", this);
    lblTitle->setStyleSheet("font-size:17px; font-weight:bold; color:#2C3E50;");
    root->addWidget(lblTitle);
    root->addSpacing(8);

    auto* toolbar = new QHBoxLayout();
    auto* btnThem = new QPushButton("+  Thêm", this);
    btnThem->setObjectName("btnPrimary");
    auto* btnSua = new QPushButton("Sửa", this);
    btnSua->setObjectName("btnFlat");
    auto* btnXoa = new QPushButton("Xóa", this);
    btnXoa->setObjectName("btnDanger");
    connect(btnThem, &QPushButton::clicked, this, &DriverPage::onThem);
    connect(btnSua, &QPushButton::clicked, this, &DriverPage::onSua);
    connect(btnXoa, &QPushButton::clicked, this, &DriverPage::onXoa);
    toolbar->addWidget(btnThem);
    toolbar->addWidget(btnSua);
    toolbar->addWidget(btnXoa);
    toolbar->addStretch();

    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("Tìm kiếm: tên / bằng lái / GPLX");
    txtSearch->setFixedWidth(280);
    connect(txtSearch, &QLineEdit::textChanged, this, &DriverPage::onTimKiem);
    toolbar->addWidget(txtSearch);
    root->addLayout(toolbar);
    root->addSpacing(10);

    table = new QTableWidget(this);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({ "Mã TX", "Họ và tên", "SĐT", "Bằng lái", "Số GPLX", "Trạng thái" });
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    root->addWidget(table, 1);

    refresh();
}

void DriverPage::fillTable(const std::vector<TaiXe>& ds) {
    table->setRowCount((int)ds.size());
    for (int i = 0; i < (int)ds.size(); ++i) {
        const auto& t = ds[i];
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(t.getMaTX())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(t.getHoTen())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(t.getSoDienThoai())));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(t.getBangLai())));
        table->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(t.getSoGPLX())));
        auto* itTrangThai = new QTableWidgetItem(QString::fromStdString(trangThaiToStr(t.getTrangThai())));
        if (t.getTrangThai() == DANG_HOAT_DONG) itTrangThai->setForeground(QColor("#438F78"));
        table->setItem(i, 5, itTrangThai);
    }
}

void DriverPage::refresh() {
    heThong->taiXe().docFile();
    fillTable(heThong->taiXe().getDanhSach());
}

void DriverPage::onTimKiem(const QString& key) {
    fillTable(heThong->taiXe().timKiem(key.toStdString()));
}

void DriverPage::onThem() {
    DriverDialog dlg(this);
    dlg.findChild<QLineEdit*>()->setText(QString::fromStdString(heThong->taiXe().newMaTX()));
    if (dlg.exec() == QDialog::Accepted) {
        TaiXe moi = dlg.ketQua();
        if (!hangBangLaiHopLeTaxi(moi.getBangLai())) {
            QMessageBox::warning(this, "Lỗi",
                "Hạng bằng lái không hợp lệ để hành nghề lái xe taxi!\n"
                "Chỉ chấp nhận các hạng: B2, C, D, E.");
            return;
        }
        if (heThong->taiXe().coMaTX(moi.getMaTX())) {
            QMessageBox::warning(this, "Lỗi", "Mã tài xế này đã tồn tại! Vui lòng thử lại.");
            return;
        }
        if (heThong->taiXe().coSoGPLX(moi.getSoGPLX())) {
            QMessageBox::warning(this, "Lỗi", "Số GPLX này đã tồn tại trong hệ thống!");
            return;
        }
        heThong->taiXe().them(moi);
        refresh();
    }
}

void DriverPage::onSua() {
    int row = table->currentRow();
    if (row < 0) { QMessageBox::information(this, "Thông báo", "Vui lòng chọn một tài xế để sửa."); return; }
    std::string maTX = table->item(row, 0)->text().toStdString();
    const TaiXe* found = nullptr;
    for (auto& t : heThong->taiXe().getDanhSach()) if (t.getMaTX() == maTX) { found = &t; break; }
    if (!found) return;

    DriverDialog dlg(this, found);
    if (dlg.exec() == QDialog::Accepted) {
        TaiXe moi = dlg.ketQua();
        if (!hangBangLaiHopLeTaxi(moi.getBangLai())) {
            QMessageBox::warning(this, "Lỗi",
                "Hạng bằng lái không hợp lệ để hành nghề lái xe taxi!\n"
                "Chỉ chấp nhận các hạng: B2, C, D, E.");
            return;
        }
        if (heThong->taiXe().coSoGPLX(moi.getSoGPLX(), maTX)) {
            QMessageBox::warning(this, "Lỗi", "Số GPLX này đã tồn tại ở một tài xế khác!");
            return;
        }
        heThong->taiXe().sua(maTX, moi);
        refresh();
    }
}

void DriverPage::onXoa() {
    int row = table->currentRow();
    if (row < 0) { QMessageBox::information(this, "Thông báo", "Vui lòng chọn một tài xế để xóa."); return; }
    std::string maTX = table->item(row, 0)->text().toStdString();
    std::string hoTen = table->item(row, 1)->text().toStdString();
    if (heThong->chuyenXe().taiXeDangChay(maTX)) {
        QMessageBox::warning(this, "Không thể xoá",
            QString("Tài xế \"%1\" đang thực hiện một chuyến xe, không thể xóa lúc này.")
            .arg(QString::fromStdString(hoTen)));
        return;
    }
    auto ret = QMessageBox::question(this, "Xác nhận xóa",
        QString("Bạn có chắc muốn xóa tài xế \"%1\" (%2)?").arg(QString::fromStdString(hoTen), QString::fromStdString(maTX)));
    if (ret == QMessageBox::Yes) {
        heThong->taiXe().xoa(maTX);
        heThong->taxi().goBoTaiXeKhoiTatCaTaxi(maTX);
        refresh();
    }
}
