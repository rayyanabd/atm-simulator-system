#pragma once

#include <QMainWindow>

#include "include/ATM.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QStackedWidget;
class QWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    enum PageIndex {
        LoginPage = 0,
        MenuPage = 1
    };

    ATM atm;
    QStackedWidget* stackedWidget;
    QLineEdit* accountNumberEdit;
    QLineEdit* pinEdit;
    QLabel* welcomeLabel;
    QLabel* balanceLabel;

    QWidget* createLoginPage();
    QWidget* createMenuPage();
    QPushButton* createMenuButton(const QString& text);

    void handleLogin();
    void updateAccountSummary();
    void showBalance();
    void handleDeposit();
    void handleWithdraw();
    void showPlaceholder(const QString& title);
    void logout();
    QString moneyText(double amount) const;
};
