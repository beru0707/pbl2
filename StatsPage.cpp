#include "StatsPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QFrame>
#include <QLocale>

StatsPage::StatsPage(HeThongTaxi* ht, QWidget* parent) : QWidget(parent), heThong(ht) {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 16, 20, 16);

    auto* lblTitle = new QLabel("THỐNG KÊ / BÁO CÁO — Bảng doanh thu theo taxi", this);
    lblTitle->setStyleSheet("font-size:17px; font-weight:bold; color:#2C3E50;");
    root->addWidget(lblTitle);
    root->addSpacing(8);

    auto* row = new QHBoxLayout();
    row->addWidget(new QLabel("Chọn taxi:", this));
    cbTaxi = new QComboBox(this);
    cbTaxi->setFixedWidth(220);
    connect(cbTaxi, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StatsPage::onChonTaxi);
    row->addWidget(cbTaxi);
    row->addStretch();
    root->addLayout(row);
    root->addSpacing(10);

    table = new QTableWidget(this);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({ "Mã chuyến", "Điểm đón", "Điểm đến", "Khoảng cách", "Cước phí", "Trạng thái" });
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    root->addWidget(table, 1);

    auto* totalCard = new QFrame(this);
    totalCard->setObjectName("card");
    auto* totalLay = new QHBoxLayout(totalCard);
    totalLay->addWidget(new QLabel("<b>TỔNG CỘNG DOANH THU</b>", totalCard));
    totalLay->addStretch();
    lblTong = new QLabel("0 đ", totalCard);
    lblTong->setStyleSheet("font-size:15px; font-weight:bold; color:#4285C7;");
    totalLay->addWidget(lblTong);
    root->addWidget(totalCard);

    refresh();
}

void StatsPage::refresh() {
    // Ngắt tín hiệu ComboBox để tránh gọi sự kiện lặp vô ích khi clear
    cbTaxi->blockSignals(true);
    cbTaxi->clear();

    const auto& dsTaxi = heThong->taxi().getDanhSach();
    for (const auto& t : dsTaxi) {
        cbTaxi->addItem(QString::fromStdString(t.getBienSo()));
    }
    cbTaxi->blockSignals(false);

    if (cbTaxi->count() > 0) {
        cbTaxi->setCurrentIndex(0);
        onChonTaxi(0);
    }
    else {
        onChonTaxi(-1);
    }
}

void StatsPage::onChonTaxi(int index) {
    if (index < 0 || index >= cbTaxi->count()) {
        table->setRowCount(0);
        lblTong->setText("0 đ");
        return;
    }

    QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);
    std::string bienSo = cbTaxi->itemText(index).toStdString();

    // ds có kiểu vector<const ChuyenXe*> 
    auto ds = heThong->chuyenXe().theoTaxi(bienSo);

    table->setRowCount((int)ds.size());
    double tong = 0;

    for (int i = 0; i < (int)ds.size(); ++i) {
        const auto* cx = ds[i]; // cx là con trỏ const ChuyenXe*
        if (!cx) continue;

        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(cx->getMaChuyen())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(cx->getDiemDon())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(cx->getDiemDen())));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(cx->getKhoangCach(), 'f', 1) + " km"));

        // Định dạng số tiền
        QString cuocPhiStr = locale.toString((qlonglong)cx->getCuocPhi()) + " đ";
        table->setItem(i, 4, new QTableWidgetItem(cuocPhiStr));

        // Gọi hàm hiển thị trạng thái
        auto* itTrangThai = new QTableWidgetItem(QString::fromStdString(trangThaiChuyenToStr(cx->getTrangThai())));
        if (cx->getTrangThai() == DA_HUY) {
            itTrangThai->setForeground(QColor("#B8503F"));
        }
        else if (cx->getTrangThai() == HOAN_THANH) {
            itTrangThai->setForeground(QColor("#438F78"));
        }
        table->setItem(i, 5, itTrangThai);

        if (cx->getTrangThai() != DA_HUY) {
            tong += cx->getCuocPhi();
        }
    }

    lblTong->setText(locale.toString((qlonglong)tong) + " đ");
}