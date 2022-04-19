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
    Window();

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

    QGroupBox* iconGroupBox;
    QLabel* iconLabel;
    QComboBox* iconComboBox;
    QCheckBox* showIconCheckBox;

    QGroupBox* messageGroupBox;
    QLabel* typeLabel;
    QLabel* durationLabel;
    QLabel* durationWarningLabel;
    QLabel* titleLabel;
    QLabel* bodyLabel;
    QComboBox* typeComboBox;
    QSpinBox* durationSpinBox;
    QLineEdit* titleEdit;
    QTextEdit* bodyEdit;
    QPushButton* showMessageButton;

    QAction* minimizeAction;
    QAction* maximizeAction;
    QAction* restoreAction;
    QAction* quitAction;

    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;
};

#endif	// _WINDOW_H_