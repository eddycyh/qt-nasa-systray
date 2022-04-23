#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <QSystemTrayIcon>
#include <QDialog>

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollArea>
#include <memory>
#include "IApiHandler.h"

class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;

class Window : public QDialog
{
    Q_OBJECT

public:
    Window(std::shared_ptr<durr::IApiHandler> api);

    void setVisible(bool visible);

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void setIcon(int index);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage();
    void messageClicked();

private:
    void createIconGroupBox();
    void createMessageGroupBox();
    void createActions();
    void createTrayIcon();

    QGroupBox* imageGroupBox;
    QGroupBox* iconGroupBox;
    QLabel* iconLabel;
    QComboBox* iconComboBox;
    QCheckBox* showIconCheckBox;
    QLabel* dateLbl;
    QLabel* dateVal;
    QLabel* titleLbl;
    QLabel* titleVal;
    QImage* apodImg;
    QLabel* apodLbl;
    QScrollArea* scrollArea;
    QLabel* explanationLbl;
    QLabel* explanationVal;
    QLabel* statusLbl;
    QLabel* statusVal;

    QGroupBox* messageGroupBox;
    QLabel* typeLabel;
    QLabel* durationLabel;
    QLabel* durationWarningLabel;
    QLabel* titleLabel;
    QLabel* bodyLabel;
    QComboBox* typeComboBox;
    QSpinBox* durationSpinBox;
    QLabel* titleEdit;
    QTextEdit* bodyEdit;
    QPushButton* showMessageButton;
    
    QLabel* countLbl;
    QLabel* countVal;
    QLabel* nameLbl;
    QLabel* nameVal;
    QLabel* estDtLbl;
    QLabel* estDtVal;
    QLabel* mDistanceLbl;
    QLabel* mDistanceVal;
    QLabel* warningLbl;

    QAction* minimizeAction;
    QAction* maximizeAction;
    QAction* restoreAction;
    QAction* quitAction;

    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;
    std::shared_ptr<durr::IApiHandler> api = nullptr;
};

#endif	// _WINDOW_H_