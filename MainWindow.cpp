#include "MainWindow.h"
#include "DriverPage.h"
#include "TaxiPage.h"
#include "BookingPage.h"
#include "StatsPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>

MainWindow::MainWindow(HeThongTaxi* ht, std::shared_ptr<NguoiDung> nd, QWidget* parent)
    : QWidget(parent), heThong(ht), currentUser(nd) {

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // title bar
    auto* titleBar = new QFrame(this);
    titleBar->setObjectName("titleBar");
    titleBar->setFixedHeight(46);
    auto* titleLay = new QHBoxLayout(titleBar);
    titleLay->setContentsMargins(0, 0, 0, 0);
    QString hello = QString("HỆ THỐNG QUẢN LÝ TAXI TIÊN SA — Xin chào, %1 (%2)")
        .arg(QString::fromStdString(nd->getTenDangNhap()),
            QString::fromStdString(nd->tenVaiTro()));
    auto* titleLbl = new QLabel(hello, titleBar);
    titleLbl->setObjectName("titleLabel");
    titleLay->addWidget(titleLbl);
    root->addWidget(titleBar);

    auto* body = new QHBoxLayout();
    body->setContentsMargins(0, 0, 0, 0);
    body->setSpacing(0);

    // sidebar
    auto* sidebar = new QFrame(this);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(220);
    auto* sideLay = new QVBoxLayout(sidebar);
    sideLay->setContentsMargins(0, 14, 0, 14);
    sideLay->setSpacing(4);

    stack = new QStackedWidget(this);

    dashboardOverview = buildOverviewPage();
    stack->addWidget(dashboardOverview); // index 0

    auto addMenuBtn = [&](const QString& text, int stackIndex) {
        auto* btn = new QPushButton(text, sidebar);
        btn->setObjectName("menuBtn");
        btn->setCheckable(true);
        btn->setMinimumHeight(38);
        connect(btn, &QPushButton::clicked, this, [this, stackIndex, btn]() { chuyenTrang(stackIndex, btn); });
        sideLay->addWidget(btn);
        menuButtons.push_back(btn);
        return btn;
        };

    QPushButton* btnOverview = addMenuBtn("Tổng quan", 0);

    for (ChucNang cn : nd->danhSachChucNang()) {
        QWidget* page = nullptr;
        QString nhan;
        switch (cn) {
        case CN_QUAN_LY_TAI_XE:
            driverPage = new DriverPage(heThong, this);
            page = driverPage;
            nhan = "Quản lý tài xế";
            break;
        case CN_QUAN_LY_TAXI:
            taxiPage = new TaxiPage(heThong, this);
            page = taxiPage;
            nhan = "Quản lý taxi";
            break;
        case CN_DAT_CHUYEN_XE:
            bookingPage = new BookingPage(heThong, this);
            page = bookingPage;
            nhan = "Đặt chuyến xe";
            break;
        case CN_THONG_KE:
            statsPage = new StatsPage(heThong, this);
            page = statsPage;
            nhan = "Thống kê / Báo cáo";
            break;
        }
        stack->addWidget(page);
        addMenuBtn(nhan, stack->count() - 1);
    }

    sideLay->addStretch();
    auto* btnLogout = new QPushButton("Đăng xuất", sidebar);
    btnLogout->setObjectName("menuBtn");
    btnLogout->setMinimumHeight(38);
    connect(btnLogout, &QPushButton::clicked, this, [this]() { emit dangXuat(); });
    sideLay->addWidget(btnLogout);

    body->addWidget(sidebar);
    body->addWidget(stack, 1);
    root->addLayout(body, 1);

    chuyenTrang(0, btnOverview);
}

void MainWindow::chuyenTrang(int index, QPushButton* btnActive) {
    for (auto* b : menuButtons) b->setChecked(b == btnActive);
    stack->setCurrentIndex(index);
    if (stack->widget(index) == dashboardOverview) refreshOverview();
    if (driverPage && stack->widget(index) == driverPage) driverPage->refresh();
    if (taxiPage && stack->widget(index) == taxiPage) taxiPage->refresh();
    if (bookingPage && stack->widget(index) == bookingPage) bookingPage->refresh();
    if (statsPage && stack->widget(index) == statsPage) statsPage->refresh();
}

QWidget* MainWindow::buildOverviewPage() {
    auto* page = new QWidget(this);
    auto* lay = new QVBoxLayout(page);
    lay->setContentsMargins(20, 16, 20, 16);

    auto* lblTitle = new QLabel("Tổng quan hệ thống", page);
    lblTitle->setStyleSheet("font-size:17px; font-weight:bold; color:#2C3E50;");
    lay->addWidget(lblTitle);
    lay->addSpacing(12);

    auto* grid = new QHBoxLayout();
    auto themThe = [&](QLabel*& outValueLabel, const QString& nhan) {
        auto* card = new QFrame(page);
        card->setObjectName("statCard");
        card->setFixedSize(150, 90);
        auto* cardLay = new QVBoxLayout(card);
        outValueLabel = new QLabel("0", card);
        outValueLabel->setAlignment(Qt::AlignCenter);
        outValueLabel->setStyleSheet("font-size:20px; font-weight:bold; color:#4285C7;");
        auto* lblNhan = new QLabel(nhan, card);
        lblNhan->setAlignment(Qt::AlignCenter);
        lblNhan->setStyleSheet("font-size:11px; color:#5a5a56;");
        cardLay->addWidget(outValueLabel);
        cardLay->addWidget(lblNhan);
        grid->addWidget(card);
        };

    themThe(lblSoTaiXe, "Tài xế");
    themThe(lblSoTaxi, "Taxi");
    themThe(lblSoChuyen, "Chuyến xe đã ghi nhận");
    themThe(lblDoanhThu, "Tổng doanh thu");

    grid->addStretch();
    lay->addLayout(grid);
    lay->addStretch();
    return page;
}

void MainWindow::refreshOverview() {
    heThong->taiXe().docFile();
    heThong->taxi().docFile();
    heThong->chuyenXe().docFile();
    int soTaiXe = (int)heThong->taiXe().getDanhSach().size();
    int soTaxi = (int)heThong->taxi().getDanhSach().size();
    int soChuyen = (int)heThong->chuyenXe().getDanhSach().size();
    double doanhThu = heThong->chuyenXe().tongDoanhThu();

    lblSoTaiXe->setText(QString::number(soTaiXe));
    lblSoTaxi->setText(QString::number(soTaxi));
    lblSoChuyen->setText(QString::number(soChuyen));
    lblDoanhThu->setText(QString::number(doanhThu / 1e6, 'f', 1) + "tr");
}
