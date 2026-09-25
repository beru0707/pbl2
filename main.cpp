#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QStackedWidget>
#include "Style.h"
#include "LoginWidget.h"
#include "MainWindow.h"
#include "HeThongTaxi.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    if (!QFileInfo::exists("data/taikhoan.txt")) {
        QDir dir(QCoreApplication::applicationDirPath());
        bool found = false;
        for (int i = 0; i < 5; ++i) {
            if (QFileInfo::exists(dir.filePath("data/taikhoan.txt"))) {
                QDir::setCurrent(dir.absolutePath());
                found = true;
                break;
            }
            if (!dir.cdUp()) break;
        }
        if (!found) {
            qWarning() << "KHONG TIM THAY thu muc 'data/' chua taikhoan.txt!"
                << "Thu muc lam viec hien tai:" << QDir::currentPath();
        }
    }
    app.setStyleSheet(appStyleSheet());

    HeThongTaxi heThong;
    heThong.khoiDong();

    auto* root = new QStackedWidget();
    root->setWindowTitle("Hệ thống quản lý Taxi Tiên Sa");
    root->resize(1000, 640);
    root->setMaximumSize(1280, 800);

    auto* login = new LoginWidget(&heThong);
    root->addWidget(login);

    QObject::connect(login, &LoginWidget::dangNhapThanhCong, root,
        [root, &heThong](std::shared_ptr<NguoiDung> nd) {
            auto* mw = new MainWindow(&heThong, nd);
            root->addWidget(mw);
            root->setCurrentWidget(mw);
            QObject::connect(mw, &MainWindow::dangXuat, root, [root]() {
                root->setCurrentIndex(0);
                QWidget* w = root->widget(1);
                if (w) { root->removeWidget(w); w->deleteLater(); }
                });
        });

    root->setCurrentWidget(login);
    root->show();

    return app.exec();
}
