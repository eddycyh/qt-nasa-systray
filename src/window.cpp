#include <QtGui>

#include "window.h"
#include <iostream>

Window::Window(std::shared_ptr<durr::IApiHandler> api) : api(api)
{
    createIconGroupBox();
    createMessageGroupBox();

    iconLabel->setMinimumWidth(durationLabel->sizeHint().width());

    createActions();
    createTrayIcon();

    connect(showMessageButton, SIGNAL(clicked()), this, SLOT(showMessage()));
    connect(showIconCheckBox, SIGNAL(toggled(bool)),
        trayIcon, SLOT(setVisible(bool)));
    connect(iconComboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(setIcon(int)));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(imageGroupBox);
    mainLayout->addWidget(iconGroupBox);
    mainLayout->addWidget(messageGroupBox);
    setLayout(mainLayout);

    iconComboBox->setCurrentIndex(1);
    trayIcon->show();

    setWindowTitle(tr("Systray"));
    resize(400, 300);
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

void Window::setIcon(int index)
{
    QIcon icon = iconComboBox->itemIcon(index);
    std::cout << "Setting item to index: " << index
        << " QIcon.name: " << icon.name().toStdString()
        << std::endl;
    //QIcon icon = QIcon(":/bad.svg");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    trayIcon->setToolTip(iconComboBox->itemText(index));
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1)
            % iconComboBox->count());
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
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(
        typeComboBox->itemData(typeComboBox->currentIndex()).toInt());
    trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), icon,
        durationSpinBox->value() * 1000);
}

void Window::messageClicked()
{
    QMessageBox::information(0, tr("Systray"),
        tr("Sorry, I already gave what help I could.\n"
            "Maybe you should try asking a human?"));
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
    /*scrollArea = new QScrollArea();
    scrollArea->setWidget(apodLbl);
    scrollArea->setMinimumSize(256, 256);
    scrollArea->setMaximumSize(512, 512);*/
    
    QVBoxLayout* imageLayout = new QVBoxLayout;
    imageLayout->addWidget(apodLbl);
    imageGroupBox->setLayout(imageLayout);

    iconGroupBox = new QGroupBox(tr("APOD Info"));

    iconLabel = new QLabel("Icon:");

    iconComboBox = new QComboBox;
    iconComboBox->addItem(QIcon(":/bad.svg"), tr("Bad"));
    iconComboBox->addItem(QIcon(":/heart.svg"), tr("Heart"));
    iconComboBox->addItem(QIcon(":/trash.svg"), tr("Trash"));
    iconComboBox->addItem(QIcon(":/Chrome.png"), tr("Chrome"));
    iconComboBox->addItem(QIcon(":/help.png"), tr("Help"));

    showIconCheckBox = new QCheckBox(tr("Show icon"));
    showIconCheckBox->setChecked(true);

    dateLbl = new QLabel(tr("Date:"));
    dateVal = new QLabel(tr(date.c_str()));

    titleLbl = new QLabel(tr("Title:"));
    titleVal = new QLabel(tr(title.c_str()));

    explanationLbl = new QLabel(tr("Explanation:"));
    explanationVal = new QLabel(tr(explanation.c_str()));
    explanationVal->setWordWrap(true);

    QGridLayout* iconLayout = new QGridLayout;
    /*iconLayout->addWidget(iconLabel);
    iconLayout->addWidget(iconComboBox);
    iconLayout->addStretch();
    iconLayout->addWidget(showIconCheckBox);*/
    
    iconLayout->addWidget(dateLbl, 0, 0);
    iconLayout->addWidget(dateVal, 0, 1);
    iconLayout->addWidget(titleLbl, 1, 0);
    iconLayout->addWidget(titleVal, 1, 1);
    iconLayout->addWidget(explanationLbl, 3, 0);
    iconLayout->addWidget(explanationVal, 3, 1);
    iconGroupBox->setLayout(iconLayout);
}

void Window::createMessageGroupBox()
{
    std::string date, title, name, estDt, missDistance, imgUrl, explanation;

    messageGroupBox = new QGroupBox(tr("NeoWs info"));

    typeLabel = new QLabel(tr("Type:"));

    typeComboBox = new QComboBox;
    typeComboBox->addItem(tr("None"), QSystemTrayIcon::NoIcon);
    typeComboBox->addItem(style()->standardIcon(
        QStyle::SP_MessageBoxInformation), tr("Information"),
        QSystemTrayIcon::Information);
    typeComboBox->addItem(style()->standardIcon(
        QStyle::SP_MessageBoxWarning), tr("Warning"),
        QSystemTrayIcon::Warning);
    typeComboBox->addItem(style()->standardIcon(
        QStyle::SP_MessageBoxCritical), tr("Critical"),
        QSystemTrayIcon::Critical);
    typeComboBox->setCurrentIndex(1);

    durationLabel = new QLabel(tr("Duration:"));

    durationSpinBox = new QSpinBox;
    durationSpinBox->setRange(5, 60);
    durationSpinBox->setSuffix(" s");
    durationSpinBox->setValue(15);

    durationWarningLabel = new QLabel(tr("(some systems might ignore this "
        "hint)"));
    durationWarningLabel->setIndent(10);

    titleLabel = new QLabel(tr("Title:"));

    titleEdit = new QLabel(tr("Cannot connect to network"));

    bodyLabel = new QLabel(tr("Body:"));

    bodyEdit = new QTextEdit;
    bodyEdit->setPlainText(tr("Don't believe me. Honestly, I don't have a "
        "clue.\nClick this balloon for details."));

    showMessageButton = new QPushButton(tr("Show Message"));
    showMessageButton->setDefault(true);
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
    /*messageLayout->addWidget(typeLabel, 0, 0);
    messageLayout->addWidget(typeComboBox, 0, 1, 1, 2);
    messageLayout->addWidget(durationLabel, 1, 0);
    messageLayout->addWidget(durationSpinBox, 1, 1);
    messageLayout->addWidget(durationWarningLabel, 1, 2, 1, 3);
    messageLayout->addWidget(titleLabel, 2, 0);
    messageLayout->addWidget(titleEdit, 2, 1, 1, 4);
    messageLayout->addWidget(bodyLabel, 3, 0);
    messageLayout->addWidget(bodyEdit, 3, 1, 2, 4);
    messageLayout->addWidget(showMessageButton, 5, 4);
    messageLayout->setColumnStretch(3, 1);
    messageLayout->setRowStretch(4, 1);*/
    messageGroupBox->setLayout(messageLayout);
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