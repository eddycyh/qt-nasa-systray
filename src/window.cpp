#include <QtGui>

#include "window.h"
#include <iostream>

Window::Window(std::shared_ptr<durr::IApiHandler> api) : api(api)
{
    createIconGroupBox();
    createMessageGroupBox();
    createActions();
    createTrayIcon();
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(imageGroupBox);
    mainLayout->addWidget(apodGroupBox);
    mainLayout->addWidget(neowsGroupBox);
    setLayout(mainLayout);

    setIcon();
    trayIcon->show();
    showMessage();
    setWindowTitle(tr("Systray"));
    resize(400, 300);
    hide();
}

void Window::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}

void Window::closeEvent(QCloseEvent* event)
{
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Systray"),
            tr("The program will keep running in the "
                "system tray. To terminate the program, "
                "choose <b>Quit</b> in the context menu "
                "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void Window::setIcon()
{
    QIcon icon = QIcon(":/heart.svg");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
    trayIcon->setToolTip("Astronomy of the day");
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}

void Window::showMessage()
{
    std::string date, title, msg;
    int elementCount;
    bool res = false;
    QIcon icon = QIcon(":/heart.svg");
    if (api) {
        res = api->getAPODBasicInfo(date, title);
        res = api->listNeows(date, date, elementCount);
        msg = std::string("Date: ") + date + std::string("\nTitle: ") + title + std::string("\nWarning: ") + std::to_string(elementCount);
    }
    if(res)
        trayIcon->showMessage("Astronomy of the day", msg.c_str(), icon,
            3000);
}

void Window::messageClicked()
{

}

void Window::createIconGroupBox()
{
    std::string imagePath = "out.jpg";
    std::string date, title, imgUrl, explanation;
    bool isDataReady = false;
    if (api) {
        isDataReady = api->getAPODDetailInfo(date, title, imgUrl, explanation);
    }

    imageGroupBox = new QGroupBox(tr("Astronomy of the day"));
    api->downloadImage(imgUrl, imagePath);
    apodImg = new QImage(imagePath.c_str());
    apodLbl = new QLabel("");
    apodLbl->setPixmap(QPixmap::fromImage(*apodImg));
    apodLbl->adjustSize();
    
    QVBoxLayout* imageLayout = new QVBoxLayout;
    imageLayout->addWidget(apodLbl);
    imageGroupBox->setLayout(imageLayout);

    apodGroupBox = new QGroupBox(tr("APOD Info"));
    dateLbl = new QLabel(tr("Date:"));
    dateVal = new QLabel(tr(date.c_str()));

    titleLbl = new QLabel(tr("Title:"));
    titleVal = new QLabel(tr(title.c_str()));

    explanationLbl = new QLabel(tr("Explanation:"));
    explanationVal = new QLabel(tr(explanation.c_str()));
    explanationVal->setWordWrap(true);

    QGridLayout* iconLayout = new QGridLayout;
    
    iconLayout->addWidget(dateLbl, 0, 0);
    iconLayout->addWidget(dateVal, 0, 1);
    iconLayout->addWidget(titleLbl, 1, 0);
    iconLayout->addWidget(titleVal, 1, 1);
    iconLayout->addWidget(explanationLbl, 3, 0);
    iconLayout->addWidget(explanationVal, 3, 1);
    apodGroupBox->setLayout(iconLayout);
}

void Window::createMessageGroupBox()
{
    std::string date, title, name, estDt, missDistance, imgUrl, explanation;

    neowsGroupBox = new QGroupBox(tr("NeoWs info"));
    bool res = false;
    if (api) {
        res = api->getAPODBasicInfo(date, title);
        res = api->getNearestNeowsInfo(date, date, name, estDt, missDistance);
    }

    
    QGridLayout* messageLayout = new QGridLayout;
    if (res) {
        nameLbl = new QLabel(tr("Name:"));
        nameVal = new QLabel(tr(name.c_str()));
        estDtLbl = new QLabel(tr("Approach DateTime:"));
        estDtVal = new QLabel(tr(estDt.c_str()));
        mDistanceLbl = new QLabel(tr("Distance (lunar):"));
        mDistanceVal = new QLabel(tr(missDistance.c_str()));
        messageLayout->addWidget(nameLbl, 0, 0);
        messageLayout->addWidget(nameVal, 0, 1);
        messageLayout->addWidget(estDtLbl, 1, 0);
        messageLayout->addWidget(estDtVal, 1, 1);
        messageLayout->addWidget(mDistanceLbl, 2, 0);
        messageLayout->addWidget(mDistanceVal, 2, 1);
    }
    else {
        warningLbl = new QLabel(tr("No Object found!!!"));
        messageLayout->addWidget(warningLbl);
    }
    neowsGroupBox->setLayout(messageLayout);
}

void Window::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}