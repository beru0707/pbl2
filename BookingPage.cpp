#include "BookingPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QRegularExpression> 
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QMessageBox>
#include <QDateTime>

BookingPage::BookingPage(HeThongTaxi* ht, QWidget* parent)
    : QWidget(parent), heThong(ht) {

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 16, 20, 16);

    auto* lblTitle = new QLabel("ĐẶT CHUYẾN XE", this);
    lblTitle->setStyleSheet("font-size:17px; font-weight:bold; color:#2C3E50;");
    root->addWidget(lblTitle);
    root->addSpacing(10);

    auto* row = new QHBoxLayout();

    auto* leftCard = new QFrame(this);
    leftCard->setObjectName("card");
    auto* leftLay = new QVBoxLayout(leftCard);
    leftLay->addWidget(new QLabel("<b>Thông tin khách hàng</b>", leftCard));

    auto* form = new QFormLayout();
    txtTenKhach = new QLineEdit(leftCard);
    txtSdt = new QLineEdit(leftCard);
    txtDiemDon = new QLineEdit(leftCard);
    txtDiemDen = new QLineEdit(leftCard);

    spinKhoangCach = new QDoubleSpinBox(leftCard);
    spinKhoangCach->setRange(0.1, 200);
    spinKhoangCach->setValue(12.5);
    spinKhoangCach->setSuffix(" km");

    spinSoKhach = new QSpinBox(leftCard);
    spinSoKhach->setRange(1, 16);
    spinSoKhach->setValue(1);
    spinSoKhach->setSuffix(" khách");
    connect(spinSoKhach, QOverload<int>::of(&QSpinBox::valueChanged), this, &BookingPage::onTimTaxi);

    form->addRow("Họ tên khách hàng:", txtTenKhach);
    form->addRow("Số điện thoại:", txtSdt);
    form->addRow("Điểm đón:", txtDiemDon);
    form->addRow("Điểm đến:", txtDiemDen);
    form->addRow("Khoảng cách:", spinKhoangCach);
    form->addRow("Số khách:", spinSoKhach);
    leftLay->addLayout(form);

    auto* btnTim = new QPushButton("TÌM TAXI PHÙ HỢP", leftCard);
    btnTim->setObjectName("btnPrimary");
    btnTim->setMinimumHeight(38);
    connect(btnTim, &QPushButton::clicked, this, &BookingPage::onTimTaxi);
    leftLay->addSpacing(6);
    leftLay->addWidget(btnTim);
    leftLay->addStretch();

    auto* rightCard = new QFrame(this);
    rightCard->setObjectName("card");
    auto* rightLay = new QVBoxLayout(rightCard);
    rightLay->addWidget(new QLabel("<b>Danh sách taxi đề xuất</b>", rightCard));

    dsTaxiContainer = new QWidget(rightCard);
    dsTaxiLayout = new QVBoxLayout(dsTaxiContainer);
    dsTaxiLayout->setSpacing(8);
    rightLay->addWidget(dsTaxiContainer);
    rightLay->addStretch();

    rightLay->addWidget(new QLabel("Chọn tài xế thực hiện:", rightCard));
    cbTaiXe = new QComboBox(rightCard);
    rightLay->addWidget(cbTaiXe);

    btnXacNhan = new QPushButton("XÁC NHẬN ĐẶT XE", rightCard);
    btnXacNhan->setObjectName("btnSuccess");
    btnXacNhan->setMinimumHeight(38);
    btnXacNhan->setEnabled(false);
    connect(btnXacNhan, &QPushButton::clicked, this, &BookingPage::onXacNhan);
    rightLay->addWidget(btnXacNhan);

    row->addWidget(leftCard, 1);
    row->addWidget(rightCard, 1);
    root->addLayout(row);

    hienThiDanhSachTaxi();
}

void BookingPage::hienThiDanhSachTaxi() {
    QLayoutItem* item;
    while ((item = dsTaxiLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    bienSoDangChon.clear();
    btnXacNhan->setEnabled(false);
    cbTaiXe->clear();

    heThong->taxi().docFile();
    heThong->taiXe().docFile();

    auto ds = heThong->taxi().timTaxiPhuHop(spinSoKhach->value());
    if (ds.empty()) {
        dsTaxiLayout->addWidget(new QLabel(
            "Không có taxi nào sẵn sàng và đủ chỗ cho số khách đã nhập.", dsTaxiContainer));
        return;
    }
    bool firstSet = false;
    for (auto* t : ds) {
        QString bienSo = QString::fromStdString(t->getBienSo());
        auto* card = new QPushButton(dsTaxiContainer);
        card->setCheckable(true);
        card->setCursor(Qt::PointingHandCursor);
        card->setMinimumHeight(56);
        card->setProperty("bienSo", bienSo);
        card->setText(QString("Biển số: %1   |   %2 chỗ")
            .arg(bienSo).arg(t->getSucChua()));
        card->setStyleSheet(
            "QPushButton { text-align: left; padding: 10px 14px; border: 1px solid #96948C;"
            " border-radius: 6px; background: white; }"
            "QPushButton:checked { border: 2px solid #4285C7; background: #DFEBF7; font-weight: bold; }"
        );
        connect(card, &QPushButton::clicked, this, [this, bienSo]() { chonTaxi(bienSo); });
        dsTaxiLayout->addWidget(card);

        if (!firstSet) { chonTaxi(bienSo); firstSet = true; }
    }
}

void BookingPage::refresh() {
    hienThiDanhSachTaxi();
}

void BookingPage::chonTaxi(const QString& bienSo) {
    bienSoDangChon = bienSo;
    btnXacNhan->setEnabled(true);
    cbTaiXe->clear();
    for (int i = 0; i < dsTaxiLayout->count(); ++i) {
        auto* w = qobject_cast<QPushButton*>(dsTaxiLayout->itemAt(i)->widget());
        if (!w) continue;
        bool sel = (w->property("bienSo").toString() == bienSo);
        w->setChecked(sel);
    }

    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo.toStdString());
    if (taxi) {
        for (auto& maTX : taxi->getDsMaTaiXe()) {
            for (auto& tx : heThong->taiXe().getDanhSach()) {
                if (tx.getMaTX() == maTX && tx.getTrangThai() == DANG_HOAT_DONG && !heThong->chuyenXe().taiXeDangChay(maTX) && hangBangLaiPhuHopSucChua(tx.getBangLai(), taxi->getSucChua())) {
                    cbTaiXe->addItem(QString::fromStdString(tx.getHoTen()),
                        QString::fromStdString(maTX));
                }
            }
        }
    }
    if (cbTaiXe->count() == 0) {
        cbTaiXe->addItem("(Chưa có tài xế phù hợp)", "");
        btnXacNhan->setEnabled(false);
    }
    else {
        btnXacNhan->setEnabled(true);
    }
}

void BookingPage::onTimTaxi() {
    hienThiDanhSachTaxi();
}

void BookingPage::onXacNhan() {
    if (bienSoDangChon.isEmpty()) return;
    if (txtTenKhach->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập họ tên khách hàng.");
        return;
    }
    static const QRegularExpression sdtRegex("^0[0-9]{9}$");
    QString sdtKhach = txtSdt->text();
    sdtKhach.remove(' ');
    if (!sdtRegex.match(sdtKhach).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Số điện thoại khách hàng không hợp lệ.");
        return;
    }
    if (txtDiemDon->text().trimmed().isEmpty() || txtDiemDen->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập đầy đủ điểm đón và điểm đến.");
        return;
    }
    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSoDangChon.toStdString());
    if (!taxi) return;


    QString maTaiXeChon = cbTaiXe->currentData().toString();
    if (maTaiXeChon.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Taxi này chưa có tài xế phù hợp để thực hiện chuyến.");
        return;
    }

    if (heThong->chuyenXe().taiXeDangChay(maTaiXeChon.toStdString())) {
        QMessageBox::warning(this, "Lỗi",
            "Tài xế này hiện đang thực hiện một chuyến xe khác, vui lòng chọn tài xế khác.");
        chonTaxi(bienSoDangChon);
        return;
    }

    const TaiXe* taiXeChon = nullptr;
    for (auto& t : heThong->taiXe().getDanhSach()) {
        if (t.getMaTX() == maTaiXeChon.toStdString()) { taiXeChon = &t; break; }
    }
    if (!taiXeChon || !hangBangLaiPhuHopSucChua(taiXeChon->getBangLai(), taxi->getSucChua())) {
        QMessageBox::warning(this, "Lỗi",
            "Tài xế không đủ điều kiện hạng bằng lái cho xe này, vui lòng chọn tài xế khác.");
        chonTaxi(bienSoDangChon);
        return;
    }

    if (spinSoKhach->value() > taxi->getSucChua()) {
        QMessageBox::warning(this, "Lỗi",
            QString("Taxi %1 chỉ chở tối đa %2 khách, không đủ chở %3 khách đã nhập.")
            .arg(bienSoDangChon).arg(taxi->getSucChua()).arg(spinSoKhach->value()));
        hienThiDanhSachTaxi();
        return;
    }

    std::string maChuyen = heThong->chuyenXe().newMaCX();
    std::string thoiDiem = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm").toStdString();

    // Sửa tại đây: Truyền 0.0 cho giá cước (để tự tính) và maTaiXeChon vào tham số thứ 10
    ChuyenXe cx(maChuyen,
        txtTenKhach->text().toStdString(),
        txtSdt->text().toStdString(),
        bienSoDangChon.toStdString(),
        thoiDiem,
        txtDiemDon->text().toStdString(),
        txtDiemDen->text().toStdString(),
        spinKhoangCach->value(),
        0.0,
        maTaiXeChon.toStdString());

    heThong->chuyenXe().themChuyen(cx);
    taxi->setSanSang(false);
    heThong->taxi().ghiFile();

    QMessageBox box(this);
    box.setWindowTitle("Thành công");
    box.setIcon(QMessageBox::Information);
    box.setText(QString("ĐẶT CHUYẾN XE THÀNH CÔNG\n\nMã chuyến xe: %1\nTaxi: %2\nCước phí dự kiến: %3 d")
        .arg(QString::fromStdString(maChuyen), bienSoDangChon)
        .arg(QString::number(cx.getCuocPhi(), 'f', 0)));
    box.exec();

    hienThiDanhSachTaxi();
}
