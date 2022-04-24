// qt-nasa-systray.cpp : Defines the entry point for the application.
//

#include <QApplication>
#include <QMessageBox>

#include "window.h"
#include "IApiHandler.h"

std::string apiKey = "70O0cmhoP9IZshLlLVoGlN0kKq0qNy6cjvZeIvLx";
int main(int argc, char* argv[])
{
    Q_INIT_RESOURCE(systray);
    auto instance = durr::IApiHandler::getInstance(apiKey);
    QApplication app(argc, argv);
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
            QObject::tr("Error, Qt object not found "
                "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);
    Window window(instance);
    window.show();
    return app.exec();
}