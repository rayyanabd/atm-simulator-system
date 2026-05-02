#include "MainWindow.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "include/CurrentAccount.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      atm(100000.0),
      stackedWidget(new QStackedWidget(this)),
      accountNumberEdit(nullptr),
      pinEdit(nullptr),
      welcomeLabel(nullptr),
      balanceLabel(nullptr) {
    atm.addAccount(new CurrentAccount());

    setWindowTitle("ATM Simulator");
    resize(520, 420);

    stackedWidget->addWidget(createLoginPage());
    stackedWidget->addWidget(createMenuPage());
    setCentralWidget(stackedWidget);

    setStyleSheet(
        "QMainWindow { background: #f5f7fb; }"
        "QLabel#TitleLabel { font-size: 24px; font-weight: 700; color: #172033; }"
        "QLabel#SummaryLabel { font-size: 15px; color: #263246; }"
        "QLineEdit { padding: 8px; font-size: 14px; }"
        "QPushButton { padding: 10px; font-size: 14px; }"
        "QPushButton#PrimaryButton { background: #176b87; color: white; border-radius: 5px; }"
        "QPushButton#MenuButton { background: white; border: 1px solid #c8d2df; border-radius: 5px; }");
}

QWidget* MainWindow::createLoginPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(56, 48, 56, 48);
    layout->setSpacing(18);

    QLabel* titleLabel = new QLabel("ATM Simulator", page);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    accountNumberEdit = new QLineEdit(page);
    accountNumberEdit->setPlaceholderText("Account number");

    pinEdit = new QLineEdit(page);
    pinEdit->setPlaceholderText("PIN");
    pinEdit->setEchoMode(QLineEdit::Password);
    pinEdit->setMaxLength(4);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSpacing(12);
    formLayout->addRow("Account Number", accountNumberEdit);
    formLayout->addRow("PIN", pinEdit);

    QPushButton* loginButton = new QPushButton("Login", page);
    loginButton->setObjectName("PrimaryButton");
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);
    connect(pinEdit, &QLineEdit::returnPressed, this, &MainWindow::handleLogin);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addLayout(formLayout);
    layout->addWidget(loginButton);
    layout->addStretch();

    return page;
}

QWidget* MainWindow::createMenuPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(40, 32, 40, 32);
    layout->setSpacing(16);

    QLabel* titleLabel = new QLabel("Main Menu", page);
    titleLabel->setObjectName("TitleLabel");

    welcomeLabel = new QLabel(page);
    welcomeLabel->setObjectName("SummaryLabel");

    balanceLabel = new QLabel(page);
    balanceLabel->setObjectName("SummaryLabel");

    QGridLayout* buttonLayout = new QGridLayout();
    buttonLayout->setSpacing(12);

    QPushButton* balanceButton = createMenuButton("Check Balance");
    QPushButton* depositButton = createMenuButton("Deposit");
    QPushButton* withdrawButton = createMenuButton("Withdraw");
    QPushButton* transferButton = createMenuButton("Transfer");
    QPushButton* statementButton = createMenuButton("Mini Statement");
    QPushButton* logoutButton = createMenuButton("Logout");

    connect(balanceButton, &QPushButton::clicked, this, &MainWindow::showBalance);
    connect(depositButton, &QPushButton::clicked, this, &MainWindow::handleDeposit);
    connect(withdrawButton, &QPushButton::clicked, this, &MainWindow::handleWithdraw);
    connect(transferButton, &QPushButton::clicked, this, [this]() {
        showPlaceholder("Transfer");
    });
    connect(statementButton, &QPushButton::clicked, this, [this]() {
        showPlaceholder("Mini Statement");
    });
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::logout);

    buttonLayout->addWidget(balanceButton, 0, 0);
    buttonLayout->addWidget(depositButton, 0, 1);
    buttonLayout->addWidget(withdrawButton, 1, 0);
    buttonLayout->addWidget(transferButton, 1, 1);
    buttonLayout->addWidget(statementButton, 2, 0);
    buttonLayout->addWidget(logoutButton, 2, 1);

    layout->addWidget(titleLabel);
    layout->addWidget(welcomeLabel);
    layout->addWidget(balanceLabel);
    layout->addSpacing(8);
    layout->addLayout(buttonLayout);
    layout->addStretch();

    return page;
}

QPushButton* MainWindow::createMenuButton(const QString& text) {
    QPushButton* button = new QPushButton(text, this);
    button->setObjectName("MenuButton");
    button->setMinimumHeight(48);
    return button;
}

void MainWindow::handleLogin() {
    const QString accountNumber = accountNumberEdit->text().trimmed();
    const QString pin = pinEdit->text().trimmed();

    if (accountNumber.isEmpty() || pin.isEmpty()) {
        QMessageBox::warning(this, "Login", "Please enter both account number and PIN.");
        return;
    }

    if (!atm.insertCard(accountNumber.toStdString())) {
        QMessageBox::warning(this, "Login", "Account number was not found.");
        return;
    }

    Account* account = atm.getCurrentAccount();
    if (account != nullptr && account->isAccountLocked()) {
        QMessageBox::critical(this, "Login", "This account is locked.");
        atm.ejectCard();
        return;
    }

    if (!atm.enterPIN(pin.toStdString())) {
        QMessageBox::warning(this, "Login", "Incorrect PIN.");
        pinEdit->clear();
        pinEdit->setFocus();
        return;
    }

    updateAccountSummary();
    stackedWidget->setCurrentIndex(MenuPage);
}

void MainWindow::updateAccountSummary() {
    Account* account = atm.getCurrentAccount();

    if (account == nullptr) {
        welcomeLabel->setText("No active account");
        balanceLabel->clear();
        return;
    }

    welcomeLabel->setText(
        QString("Account %1 - %2")
            .arg(QString::fromStdString(account->getAccountNumber()))
            .arg(QString::fromStdString(account->getAccountType())));
    balanceLabel->setText(QString("Balance: %1").arg(moneyText(account->getBalance())));
}

void MainWindow::showBalance() {
    Account* account = atm.getCurrentAccount();
    if (account == nullptr) {
        QMessageBox::warning(this, "Balance", "Please login first.");
        return;
    }

    QMessageBox::information(this, "Balance", QString("Current balance: %1").arg(moneyText(account->getBalance())));
    updateAccountSummary();
}

void MainWindow::handleDeposit() {
    bool ok = false;
    const double amount = QInputDialog::getDouble(
        this, "Deposit", "Amount (Rs.)", 1000.0, 1.0, 50000.0, 2, &ok);

    if (!ok) {
        return;
    }

    if (atm.depositAmount(amount)) {
        updateAccountSummary();
        QMessageBox::information(this, "Deposit", "Deposit complete.");
    }
    else {
        QMessageBox::warning(this, "Deposit", "Deposit could not be completed.");
    }
}

void MainWindow::handleWithdraw() {
    bool ok = false;
    const double amount = QInputDialog::getDouble(
        this, "Withdraw", "Amount (Rs.)", 1000.0, 1.0, 20000.0, 2, &ok);

    if (!ok) {
        return;
    }

    if (atm.withdrawAmount(amount)) {
        updateAccountSummary();
        QMessageBox::information(this, "Withdraw", QString("Please collect: %1").arg(moneyText(amount)));
    }
    else {
        QMessageBox::warning(this, "Withdraw", "Withdrawal could not be completed.");
    }
}

void MainWindow::showPlaceholder(const QString& title) {
    QMessageBox::information(
        this,
        title,
        title + " is currently a placeholder because this backend function is not exposed for the GUI yet.");
}

void MainWindow::logout() {
    atm.ejectCard();
    accountNumberEdit->clear();
    pinEdit->clear();
    stackedWidget->setCurrentIndex(LoginPage);
    accountNumberEdit->setFocus();
}

QString MainWindow::moneyText(double amount) const {
    return QString("Rs. %1").arg(amount, 0, 'f', 2);
}
