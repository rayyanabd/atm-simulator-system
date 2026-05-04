#pragma once

#include <QMainWindow>
#include <QStyle>
#include <vector>

#include "include/ATM.h"

class QFrame;
class QComboBox;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QStackedWidget;
class QTableWidget;
class QWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    struct PinToggle {
        QLineEdit* field;
        QPushButton* button;
    };

    enum PageIndex {
        WelcomePage = 0,
        AccountNumberPage,
        PinPage,
        MainMenuPage,
        FastCashPage,
        BalancePage,
        MiniStatementPage,
        DepositPage,
        WithdrawalPage,
        PinChangePage,
        NewAccountPage,
        SuccessPage
    };

    ATM atm;
    QStackedWidget* stackedWidget;
    QLabel* screenTitleLabel;
    QLabel* footerHintLabel;
    QLabel* footerTimeLabel;
    std::vector<QLabel*> accountInfoLabels;
    std::vector<PinToggle> pinToggles;

    QLineEdit* accountDigitsEdit;
    QLineEdit* pinEdit;
    QLabel* pinAttemptLabel;
    QPushButton* pinSubmitButton;
    QLabel* menuAccountLabel;
    QLabel* menuBalanceLabel;
    QLabel* balanceTypeValue;
    QLabel* balanceAmountValue;
    QTableWidget* statementTable;
    QLabel* statementEmptyLabel;
    QLineEdit* depositAmountEdit;
    QLineEdit* withdrawalAmountEdit;
    QLineEdit* currentPinEdit;
    QLineEdit* newPinEdit;
    QLineEdit* confirmPinEdit;
    QLineEdit* cnicEdit;
    QLineEdit* newAccountPinEdit;
    QLineEdit* confirmAccountPinEdit;
    QComboBox* accountTypeCombo;
    QLabel* successTitleLabel;
    QLabel* successDetailsLabel;
    QPushButton* successReceiptButton;
    QPushButton* successReturnButton;
    QString lastReceiptTitle;
    QString lastReceiptText;
    bool successReturnsToMenu;

    QWidget* createWelcomePage();
    QWidget* createAccountNumberPage();
    QWidget* createPinPage();
    QWidget* createMainMenuPage();
    QWidget* createFastCashPage();
    QWidget* createBalancePage();
    QWidget* createMiniStatementPage();
    QWidget* createDepositPage();
    QWidget* createWithdrawalPage();
    QWidget* createPinChangePage();
    QWidget* createNewAccountPage();
    QWidget* createSuccessPage();

    QPushButton* createActionButton(const QString& title, const QString& subtitle = QString(), QStyle::StandardPixmap icon = QStyle::SP_ArrowForward);
    QPushButton* createPrimaryButton(const QString& text);
    QPushButton* createSecondaryButton(const QString& text);
    QPushButton* createDangerButton(const QString& text);
    QLabel* createHeading(const QString& text);
    QLabel* createSubheading(const QString& text);
    QLineEdit* createAmountEdit(const QString& placeholder);
    QLineEdit* createPinEdit(const QString& placeholder);
    QWidget* createPinInputWithToggle(QLineEdit* pinField);
    QFrame* createAccountInfoBar();
    QHBoxLayout* createNavigationRow(QPushButton* leftButton, QPushButton* rightButton, QPushButton* dangerButton = nullptr);
    void setupPinVisibilityToggle(QLineEdit* pinField, QPushButton* eyeButton);
    void resetPinVisibilityToggles();

    void setPage(PageIndex page);
    void goWelcome();
    void goAccountNumber();
    void goPin();
    void goMainMenu();
    void goFastCash();
    void goBalance();
    void goMiniStatement();
    void goDeposit();
    void goWithdrawal();
    void goPinChange();
    void goNewAccount();

    void submitAccountDigits();
    void submitPin();
    void appendPinDigit(const QString& digit);
    void submitDeposit();
    void submitWithdrawal();
    void submitFastCash(double amount);
    void submitPinChange();
    void submitNewAccount();
    void logout();
    bool confirmEndSession();
    void showSuccess(const QString& title, const QString& details, const QString& receiptTitle, const QString& receiptText, bool returnToMenu);

    void refreshMenuSummary();
    void refreshAccountInfoBars();
    void refreshBalance();
    void refreshMiniStatement();
    void clearSensitiveFields();
    void maybeShowReceipt(const QString& transactionType, double amount);
    void showReceiptDialog(const QString& title, const QString& text);

    QString moneyText(double amount, int decimals = 2) const;
    QString currentAccountLastFour() const;
    QString digitsOnly(const QString& value) const;
    QString formattedTimestamp(const QString& timestamp) const;
    QString receiptText(const QString& transactionType, double amount) const;
    QString statementText() const;
};
