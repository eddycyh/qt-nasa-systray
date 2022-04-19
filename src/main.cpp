// qt-nasa-systray.cpp : Defines the entry point for the application.
//

#include <QApplication>
#include <QMessageBox>

#include "window.h"
#include "IApiHandler.h"

int main(int argc, char* argv[])
{
    std::string date, title;
    auto apiInstance = durr::IApiHandler::getInstance();
    apiInstance->getAPODBasicInfo(date, title);

    Q_INIT_RESOURCE(systray);

    QApplication app(argc, argv);
    
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
            QObject::tr("Error, Qt object not found "
                "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    Window window;
    window.show();
    return app.exec();
}