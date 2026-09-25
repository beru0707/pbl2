#pragma once
#include <QWidget>
#include <memory>
#include "HeThongTaxi.h"
#include "TaiKhoan.h"

class QStackedWidget;
class QPushButton;
class QLabel;
class DriverPage;
class TaxiPage;
class BookingPage;
class StatsPage;

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(HeThongTaxi* ht, std::shared_ptr<NguoiDung> nguoiDung, QWidget* parent = nullptr);

signals:
    void dangXuat();

private:
    HeThongTaxi* heThong;
    std::shared_ptr<NguoiDung> currentUser;
    QStackedWidget* stack;
    QWidget* dashboardOverview;
    DriverPage* driverPage = nullptr;
    TaxiPage* taxiPage = nullptr;
    BookingPage* bookingPage = nullptr;
    StatsPage* statsPage = nullptr;
    std::vector<QPushButton*> menuButtons;

    QLabel* lblSoTaiXe = nullptr;
    QLabel* lblSoTaxi = nullptr;
    QLabel* lblSoChuyen = nullptr;
    QLabel* lblDoanhThu = nullptr;
    void refreshOverview();

    QWidget* buildOverviewPage();
    void chuyenTrang(int index, QPushButton* btnActive);
};