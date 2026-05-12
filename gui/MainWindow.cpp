#include "MainWindow.h"

#define PIN_LENGTH 4
#define MAX_PIN_ATTEMPTS 3
#define MINI_STATEMENT_COUNT 10

#include <QAbstractItemView>
#include <QColor>
#include <QComboBox>
#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QStyle>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>
#include <QVector>
#include <QStringList>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    atm(),
    stackedWidget(new QStackedWidget(this)),
    screenTitleLabel(new QLabel(this)),
    footerHintLabel(new QLabel(this)),
    footerTimeLabel(new QLabel(this)),
    accountDigitsEdit(nullptr),
    pinEdit(nullptr),
    pinAttemptLabel(nullptr),
    pinSubmitButton(nullptr),
    menuAccountLabel(nullptr),
    menuBalanceLabel(nullptr),
    balanceTypeValue(nullptr),
    balanceAmountValue(nullptr),
    statementTable(nullptr),
    statementEmptyLabel(nullptr),
    depositAmountEdit(nullptr),
    withdrawalAmountEdit(nullptr),
    currentPinEdit(nullptr),
    newPinEdit(nullptr),
    confirmPinEdit(nullptr),
    cnicEdit(nullptr),
    newAccountPinEdit(nullptr),
    confirmAccountPinEdit(nullptr),
    successTitleLabel(nullptr),
    successDetailsLabel(nullptr),
    successReceiptButton(nullptr),
    successReturnButton(nullptr),
    successReturnsToMenu(true),
    isAdminAttempt(false),
    adminPinAttempts(0),
    adminAccountsTable(nullptr),
    adminReturnButton(nullptr) {
    atm.cashAvailable = 100000.0;
    atm.readAccountdata();

    setWindowTitle("Fast NUCES Banking ATM Simulator");
    resize(1280, 760);
    setMinimumSize(1060, 680);

    QWidget* shell = new QWidget(this);
    shell->setObjectName("Shell");
    QVBoxLayout* shellLayout = new QVBoxLayout(shell);
    shellLayout->setContentsMargins(0, 0, 0, 0);
    shellLayout->setSpacing(0);

    QFrame* header = new QFrame(shell);
    header->setObjectName("Header");
    QHBoxLayout* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(34, 18, 34, 18);
    headerLayout->setSpacing(18);

    QLabel* logoMark = new QLabel("SE-B", header);
    logoMark->setObjectName("LogoMark");
    logoMark->setFixedSize(70, 54);
    logoMark->setAlignment(Qt::AlignCenter);

    QVBoxLayout* bankLayout = new QVBoxLayout();
    bankLayout->setSpacing(1);
    QLabel* bankName = new QLabel("Anda Shami Banking", header);
    bankName->setObjectName("BankName");
    QLabel* bankTagline = new QLabel("Apka Aitbaar Hamara Mayaar", header);
    bankTagline->setObjectName("MutedLabel");
    bankLayout->addWidget(bankName);
    bankLayout->addWidget(bankTagline);

    QLabel* secureLabel = new QLabel("Secure Session", header);
    secureLabel->setObjectName("StatusLabel");
    QLabel* onlineLabel = new QLabel("ATM Online", header);
    onlineLabel->setObjectName("StatusLabel");
    screenTitleLabel->setObjectName("ScreenTitle");
    screenTitleLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    headerLayout->addWidget(logoMark);
    headerLayout->addLayout(bankLayout);
    headerLayout->addStretch();
    headerLayout->addWidget(secureLabel);
    headerLayout->addWidget(onlineLabel);
    headerLayout->addWidget(screenTitleLabel, 1);

    stackedWidget->addWidget(createWelcomePage());
    stackedWidget->addWidget(createAccountNumberPage());
    stackedWidget->addWidget(createPinPage());
    stackedWidget->addWidget(createMainMenuPage());
    stackedWidget->addWidget(createFastCashPage());
    stackedWidget->addWidget(createBalancePage());
    stackedWidget->addWidget(createMiniStatementPage());
    stackedWidget->addWidget(createDepositPage());
    stackedWidget->addWidget(createWithdrawalPage());
    stackedWidget->addWidget(createPinChangePage());
    stackedWidget->addWidget(createNewAccountPage());
    stackedWidget->addWidget(createSuccessPage());
    stackedWidget->addWidget(createAdminDashboardPage());

    QFrame* footer = new QFrame(shell);
    footer->setObjectName("Footer");
    QHBoxLayout* footerLayout = new QHBoxLayout(footer);
    footerLayout->setContentsMargins(34, 14, 34, 14);
    footerHintLabel->setObjectName("MutedLabel");
    footerTimeLabel->setObjectName("MutedLabel");
    footerLayout->addWidget(footerHintLabel);
    footerLayout->addStretch();
    footerLayout->addWidget(footerTimeLabel);

    shellLayout->addWidget(header);
    shellLayout->addWidget(stackedWidget, 1);
    shellLayout->addWidget(footer);
    setCentralWidget(shell);

    setStyleSheet(
        "QWidget#Shell { background: #EEF2EE; color: #1D2327; }"
        "QMessageBox { background: #EEF2EE; }"
        "QFrame#Header, QFrame#Footer { background: #98b8b3; border-color: #D0D0C8; }"
        "QFrame#Header { border-bottom: 1px solid #D0D0C8; }"
        "QFrame#Footer { border-top: 1px solid #D0D0C8; }"
        "QLabel { color: #1D2327; }"
        "QLabel#LogoMark { background: #008B8B; color: #FFFFFF; border-radius: 12px; font-size: 21px; font-weight: 900; }"
        "QLabel#BankName { font-size: 27px; font-weight: 850; letter-spacing: 0px; }"
        "QLabel#ScreenTitle { font-size: 25px; font-weight: 800; }"
        "QLabel#MutedLabel { color: #5C6B73; font-size: 15px; }"
        "QLabel#StatusLabel { color: #43A047; font-size: 15px; padding: 0 12px; }"
        "QLabel#HeroHeading { font-size: 54px; font-weight: 900; letter-spacing: 0px; }"
        "QLabel#WelcomeHero { font-size: 64px; font-weight: 900; letter-spacing: 0px; }"
        "QLabel#PageHeading { font-size: 40px; font-weight: 850; letter-spacing: 0px; }"
        "QLabel#Subheading { color: #5C6B73; font-size: 21px; }"
        "QLabel#InlineError { color: #D32F2F; font-size: 17px; }"
        "QFrame#Panel { background: #FFFFFF; border: 1px solid #D0D0C8; border-radius: 14px; }"
        "QFrame#SoftPanel { background: #FFFFFF; border: 1px solid #98b8b3; border-radius: 12px; }"
        "QFrame#AccountInfoBar { background: #FFFFFF; border: 1px solid #D0D0C8; border-radius: 10px; }"
        "QLabel#AccountInfoText { color: #37474F; font-size: 16px; font-weight: 650; }"
        "QLineEdit, QComboBox { background: #FFFFFF; border: 1px solid #D0D0C8; border-radius: 9px; color: #1D2327; padding: 14px 16px; font-size: 24px; selection-background-color: #008B8B; }"
        "QLineEdit:focus, QComboBox:focus { border: 2px solid #008B8B; }"
        "QComboBox { font-size: 20px; min-height: 36px; }"
        "QPushButton { border-radius: 10px; color: #1D2327; font-size: 18px; font-weight: 750; padding: 13px 20px; min-height: 56px; }"
        "QPushButton#PrimaryButton { background: #008B8B; border: 1px solid #007373; color: #FFFFFF; }"
        "QPushButton#PrimaryButton:hover { background: #00A3A3; }"
        "QPushButton#PrimaryButton:pressed { background: #007373; }"
        "QPushButton#PrimaryButton:disabled { background: #98b8b3; border-color: #D0D0C8; color: #9E9E9E; }"
        "QPushButton#SecondaryButton { background: #FFFFFF; border: 1px solid #D0D0C8; }"
        "QPushButton#SecondaryButton:hover { border-color: #008B8B; }"
        "QPushButton#SecondaryButton:pressed { background: #98b8b3; }"
        "QPushButton#DangerButton { background: #FFEBEE; border: 1px solid #EF9A9A; color: #C62828; }"
        "QPushButton#DangerButton:hover { background: #FFCDD2; border-color: #E53935; color: #B71C1C; }"
        "QPushButton#DangerButton:pressed { background: #EF9A9A; }"
        "QPushButton#EyeButton { background: #FFFFFF; border: 1px solid #D0D0C8; border-radius: 9px; color: #1D2327; font-size: 17px; font-weight: 800; padding: 0; min-height: 54px; max-height: 54px; min-width: 58px; max-width: 58px; }"
        "QPushButton#EyeButton:hover { border-color: #008B8B; background: #EEF2EE; }"
        "QPushButton#EyeButton:pressed { background: #98b8b3; }"
        "QPushButton#ActionButton { background: #FFFFFF; border: 1px solid #D0D0C8; text-align: left; min-height: 74px; }"
        "QPushButton#ActionButton:hover { border-color: #008B8B; background: #EEF2EE; }"
        "QTableWidget { background: #FFFFFF; border: 1px solid #D0D0C8; border-radius: 10px; gridline-color: #98b8b3; color: #1D2327; font-size: 16px; alternate-background-color: #EEF2EE; }"
        "QHeaderView::section { background: #98b8b3; color: #1D2327; border: 0; padding: 12px; font-size: 16px; font-weight: 800; }"
        "QTableWidget::item { padding: 10px; border-bottom: 1px solid #98b8b3; }");

    setPage(WelcomePage);
}

QWidget* MainWindow::createWelcomePage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(70, 52, 70, 46);
    layout->setSpacing(28);

    QHBoxLayout* body = new QHBoxLayout();
    body->setSpacing(46);

    QVBoxLayout* copy = new QVBoxLayout();
    copy->setSpacing(18);
    QLabel* heading = new QLabel("PLEASE\nINSERT\nYOUR CARD", page);
    heading->setObjectName("WelcomeHero");
    heading->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QLabel* subheading = createSubheading("To begin your transaction");
    copy->addStretch();
    copy->addWidget(heading);
    copy->addWidget(subheading);
    copy->addStretch();

    QFrame* slotPanel = new QFrame(page);
    slotPanel->setObjectName("Panel");
    slotPanel->setMinimumSize(380, 280);
    QVBoxLayout* slotLayout = new QVBoxLayout(slotPanel);
    slotLayout->setContentsMargins(34, 34, 34, 34);
    QLabel* slot = new QLabel("CARD SLOT", slotPanel);
    slot->setObjectName("PageHeading");
    slot->setAlignment(Qt::AlignCenter);
    QLabel* card = new QLabel("Banking System\n**** **** **** 1234", slotPanel);
    card->setObjectName("Subheading");
    card->setAlignment(Qt::AlignCenter);
    slotLayout->addStretch();
    slotLayout->addWidget(slot);
    slotLayout->addWidget(card);
    slotLayout->addStretch();

    body->addLayout(copy, 3);
    body->addWidget(slotPanel, 2);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->setSpacing(26);
    QPushButton* newAccountButton = createSecondaryButton("Open New Account");
    QPushButton* continueButton = createPrimaryButton("Continue");
    connect(newAccountButton, &QPushButton::clicked, this, &MainWindow::goNewAccount);
    connect(continueButton, &QPushButton::clicked, this, &MainWindow::goAccountNumber);
    buttons->addWidget(newAccountButton);
    buttons->addWidget(continueButton);

    layout->addLayout(body, 1);
    layout->addLayout(buttons);
    return page;
}

QWidget* MainWindow::createAccountNumberPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(110, 64, 110, 58);
    layout->setSpacing(26);

    layout->addWidget(createHeading("FIND YOUR ACCOUNT"));
    layout->addWidget(createSubheading("Enter the last 4 digits of your account number."));

    QFrame* panel = new QFrame(page);
    panel->setObjectName("Panel");
    QVBoxLayout* panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(44, 40, 44, 40);
    panelLayout->setSpacing(18);
    QLabel* label = new QLabel("Account Last 4 Digits", panel);
    label->setObjectName("Subheading");
    accountDigitsEdit = new QLineEdit(panel);
    accountDigitsEdit->setMaxLength(4);
    accountDigitsEdit->setAlignment(Qt::AlignCenter);
    accountDigitsEdit->setPlaceholderText("0000");
    accountDigitsEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{0,4}"), accountDigitsEdit));
    connect(accountDigitsEdit, &QLineEdit::returnPressed, this, &MainWindow::submitAccountDigits);
    panelLayout->addWidget(label);
    panelLayout->addWidget(accountDigitsEdit);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->setSpacing(20);
    QPushButton* backButton = createSecondaryButton("Back");
    QPushButton* submitButton = createPrimaryButton("Find Account");
    connect(backButton, &QPushButton::clicked, this, &MainWindow::goWelcome);
    connect(submitButton, &QPushButton::clicked, this, &MainWindow::submitAccountDigits);
    buttons->addWidget(backButton);
    buttons->addWidget(submitButton);

    layout->addWidget(panel);
    layout->addStretch();
    layout->addLayout(buttons);
    return page;
}

QWidget* MainWindow::createPinPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* outer = new QVBoxLayout(page);
    outer->setContentsMargins(70, 46, 70, 38);
    outer->setSpacing(22);

    QHBoxLayout* body = new QHBoxLayout();
    body->setSpacing(38);

    QFrame* leftPanel = new QFrame(page);
    leftPanel->setObjectName("Panel");
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(44, 42, 44, 42);
    leftLayout->setSpacing(22);
    QLabel* prompt = createHeading("PLEASE ENTER YOUR\n4-DIGIT PIN");
    QLabel* sub = createSubheading("To authorize your access");
    pinEdit = createPinEdit("PIN");
    pinEdit->setAlignment(Qt::AlignCenter);
    connect(pinEdit, &QLineEdit::returnPressed, this, &MainWindow::submitPin);
    pinAttemptLabel = new QLabel(leftPanel);
    pinAttemptLabel->setObjectName("InlineError");
    pinSubmitButton = createPrimaryButton("Submit PIN");
    pinSubmitButton->setEnabled(false);
    connect(pinSubmitButton, &QPushButton::clicked, this, &MainWindow::submitPin);
    connect(pinEdit, &QLineEdit::textChanged, this, [this](const QString& value) {
        pinSubmitButton->setEnabled(value.length() == PIN_LENGTH);
        });
    leftLayout->addWidget(prompt);
    leftLayout->addWidget(sub);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(createPinInputWithToggle(pinEdit));
    leftLayout->addWidget(pinAttemptLabel);
    leftLayout->addStretch();
    leftLayout->addWidget(pinSubmitButton);

    QFrame* keypadPanel = new QFrame(page);
    keypadPanel->setObjectName("Panel");
    QGridLayout* keypad = new QGridLayout(keypadPanel);
    keypad->setContentsMargins(34, 34, 34, 34);
    keypad->setSpacing(14);

    for (int i = 1; i <= 9; i++) {
        QPushButton* button = createSecondaryButton(QString::number(i));
        button->setMinimumHeight(74);
        connect(button, &QPushButton::clicked, this, [this, i]() {
            appendPinDigit(QString::number(i));
            });
        keypad->addWidget(button, (i - 1) / 3, (i - 1) % 3);
    }

    QPushButton* backspaceButton = createSecondaryButton("Backspace");
    QPushButton* zeroButton = createSecondaryButton("0");
    QPushButton* clearButton = createSecondaryButton("CLEAR");
    backspaceButton->setMinimumHeight(74);
    zeroButton->setMinimumHeight(74);
    clearButton->setMinimumHeight(74);
    connect(backspaceButton, &QPushButton::clicked, this, [this]() {
        pinEdit->backspace();
        });
    connect(zeroButton, &QPushButton::clicked, this, [this]() {
        appendPinDigit("0");
        });
    connect(clearButton, &QPushButton::clicked, pinEdit, &QLineEdit::clear);
    keypad->addWidget(backspaceButton, 3, 0);
    keypad->addWidget(zeroButton, 3, 1);
    keypad->addWidget(clearButton, 3, 2);

    body->addWidget(leftPanel, 3);
    body->addWidget(keypadPanel, 2);

    QPushButton* backButton = createSecondaryButton("Back");
    QPushButton* ejectButton = createDangerButton("Eject Card / End Session");
    connect(backButton, &QPushButton::clicked, this, &MainWindow::goAccountNumber);
    connect(ejectButton, &QPushButton::clicked, this, &MainWindow::logout);

    outer->addLayout(body, 1);
    outer->addLayout(createNavigationRow(backButton, nullptr, ejectButton));
    return page;
}

QWidget* MainWindow::createMainMenuPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(64, 42, 64, 42);
    layout->setSpacing(22);

    layout->addWidget(createHeading("SELECT TRANSACTION"));

    QFrame* summaryPanel = new QFrame(page);
    summaryPanel->setObjectName("AccountInfoBar");
    QHBoxLayout* summaryLayout = new QHBoxLayout(summaryPanel);
    summaryLayout->setContentsMargins(28, 18, 28, 18);
    menuAccountLabel = new QLabel("Account", summaryPanel);
    menuAccountLabel->setObjectName("AccountInfoText");
    menuBalanceLabel = new QLabel("Balance", summaryPanel);
    menuBalanceLabel->setObjectName("AccountInfoText");
    summaryLayout->addWidget(menuAccountLabel, 2);
    summaryLayout->addWidget(menuBalanceLabel, 1);

    QGridLayout* grid = new QGridLayout();
    grid->setSpacing(20);
    QPushButton* fastCashButton = createActionButton("  Fast Cash", "   Get cash quickly", QStyle::SP_BrowserReload);
    QPushButton* balanceButton = createActionButton("  Balance Inquiry", "   View account balance", QStyle::SP_DriveHDIcon);
    QPushButton* pinChangeButton = createActionButton("  PIN Change", "   Change your ATM PIN", QStyle::SP_MessageBoxWarning);
    QPushButton* statementButton = createActionButton("  Mini Statement", "   View last transactions", QStyle::SP_FileDialogListView);
    QPushButton* withdrawButton = createActionButton("  Cash Withdrawal", "   Withdraw cash from ATM", QStyle::SP_ArrowDown);
    QPushButton* depositButton = createActionButton("  Cash Deposit", "   Deposit cash to account", QStyle::SP_ArrowUp);
    QPushButton* exitButton = createDangerButton("Exit\nEnd session / Logout");

    connect(fastCashButton, &QPushButton::clicked, this, &MainWindow::goFastCash);
    connect(balanceButton, &QPushButton::clicked, this, &MainWindow::goBalance);
    connect(pinChangeButton, &QPushButton::clicked, this, &MainWindow::goPinChange);
    connect(statementButton, &QPushButton::clicked, this, &MainWindow::goMiniStatement);
    connect(withdrawButton, &QPushButton::clicked, this, &MainWindow::goWithdrawal);
    connect(depositButton, &QPushButton::clicked, this, &MainWindow::goDeposit);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::logout);

    grid->addWidget(fastCashButton, 0, 0);
    grid->addWidget(withdrawButton, 0, 1);
    grid->addWidget(balanceButton, 1, 0);
    grid->addWidget(depositButton, 1, 1);
    grid->addWidget(pinChangeButton, 2, 0);
    grid->addWidget(exitButton, 2, 1);
    grid->addWidget(statementButton, 3, 0);

    layout->addWidget(summaryPanel);
    layout->addLayout(grid, 1);
    return page;
}

QWidget* MainWindow::createFastCashPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(72, 44, 72, 46);
    layout->setSpacing(22);
    layout->addWidget(createHeading("SELECT AMOUNT TO WITHDRAW"));
    layout->addWidget(createAccountInfoBar());

    QGridLayout* grid = new QGridLayout();
    grid->setSpacing(18);
    const QVector<double> amounts = { 500, 1000, 2000, 5000, 10000, 20000 };
    for (int i = 0; i < amounts.size(); i++) {
        QPushButton* button = createActionButton(moneyText(amounts[i], 0));
        connect(button, &QPushButton::clicked, this, [this, amount = amounts[i]]() {
            submitFastCash(amount);
            });
        grid->addWidget(button, i % 3, i / 3);
    }

    QPushButton* backButton = createSecondaryButton("Back to Main Menu");
    QPushButton* endButton = createDangerButton("End Session");
    connect(backButton, &QPushButton::clicked, this, &MainWindow::goMainMenu);
    connect(endButton, &QPushButton::clicked, this, &MainWindow::logout);

    layout->addLayout(grid, 1);
    layout->addLayout(createNavigationRow(backButton, nullptr, endButton));
    return page;
}

QWidget* MainWindow::createBalancePage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(92, 54, 92, 46);
    layout->setSpacing(24);
    layout->addWidget(createHeading("RESULTS"));
    layout->addWidget(createAccountInfoBar());
    layout->addWidget(createSubheading("Your account balance details are shown below."));

    QFrame* panel = new QFrame(page);
    panel->setObjectName("Panel");
    QGridLayout* grid = new QGridLayout(panel);
    grid->setContentsMargins(44, 36, 44, 36);
    grid->setHorizontalSpacing(28);
    grid->setVerticalSpacing(24);
    QLabel* typeLabel = createSubheading("Account Type");
    QLabel* amountLabel = createSubheading("Available Balance");
    balanceTypeValue = createHeading("");
    balanceAmountValue = createHeading("");
    balanceTypeValue->setStyleSheet("color: #4ea1ff;");
    balanceAmountValue->setStyleSheet("color: #45d18b;");
    grid->addWidget(typeLabel, 0, 0);
    grid->addWidget(balanceTypeValue, 0, 1);
    grid->addWidget(amountLabel, 1, 0);
    grid->addWidget(balanceAmountValue, 1, 1);

    QPushButton* printButton = createSecondaryButton("Print Receipt");
    QPushButton* returnButton = createPrimaryButton("Back to Main Menu");
    QPushButton* endButton = createDangerButton("End Session");
    connect(printButton, &QPushButton::clicked, this, [this]() {
        showReceiptDialog("Balance Receipt", receiptText("Balance Inquiry", 0.0));
        });
    connect(returnButton, &QPushButton::clicked, this, &MainWindow::goMainMenu);
    connect(endButton, &QPushButton::clicked, this, &MainWindow::logout);

    layout->addWidget(panel);
    layout->addStretch();
    layout->addLayout(createNavigationRow(printButton, returnButton, endButton));
    return page;
}

QWidget* MainWindow::createMiniStatementPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(56, 38, 56, 40);
    layout->setSpacing(18);
    layout->addWidget(createHeading("TRANSACTION HISTORY"));
    layout->addWidget(createAccountInfoBar());

    statementEmptyLabel = createSubheading("No transactions yet.");
    statementEmptyLabel->setAlignment(Qt::AlignCenter);
    statementTable = new QTableWidget(page);
    statementTable->setObjectName("StatementTable");
    statementTable->setColumnCount(4);
    statementTable->setHorizontalHeaderLabels({ "Date", "Transaction Type", "Amount", "Balance" });
    statementTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    statementTable->setAlternatingRowColors(true);
    statementTable->setColumnWidth(0, 260);
    statementTable->setColumnWidth(1, 300);
    statementTable->setColumnWidth(2, 220);
    statementTable->horizontalHeader()->setStretchLastSection(true);
    statementTable->verticalHeader()->setVisible(false);
    statementTable->verticalHeader()->setDefaultSectionSize(48);
    statementTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    statementTable->setSelectionMode(QAbstractItemView::NoSelection);

    QPushButton* printButton = createSecondaryButton("Print Statement");
    QPushButton* backButton = createPrimaryButton("Back to Main Menu");
    QPushButton* endButton = createDangerButton("End Session");
    connect(printButton, &QPushButton::clicked, this, [this]() {
        showReceiptDialog("Mini Statement", statementText());
        });
    connect(backButton, &QPushButton::clicked, this, &MainWindow::goMainMenu);
    connect(endButton, &QPushButton::clicked, this, &MainWindow::logout);

    layout->addWidget(statementTable, 1);
    layout->addWidget(statementEmptyLabel);
    layout->addLayout(createNavigationRow(printButton, backButton, endButton));
    return page;
}

QWidget* MainWindow::createDepositPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(92, 52, 92, 46);
    layout->setSpacing(24);
    layout->addWidget(createHeading("CASH DEPOSIT"));
    layout->addWidget(createAccountInfoBar());

    QFrame* panel = new QFrame(page);
    panel->setObjectName("Panel");
    QVBoxLayout* panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(48, 42, 48, 42);
    panelLayout->setSpacing(22);
    QLabel* instructions = createSubheading(
        "Insert a single stack of up to 50 currency notes into the slot\n"
        "Notes must be in usable condition");
    depositAmountEdit = createAmountEdit("Enter Deposit Amount");
    connect(depositAmountEdit, &QLineEdit::returnPressed, this, &MainWindow::submitDeposit);
    panelLayout->addWidget(instructions);
    panelLayout->addWidget(depositAmountEdit);

    QPushButton* menuButton = createSecondaryButton("Back to Main Menu");
    QPushButton* submitButton = createPrimaryButton("Confirm Deposit");
    QPushButton* endButton = createDangerButton("End Session");
    connect(menuButton, &QPushButton::clicked, this, &MainWindow::goMainMenu);
    connect(submitButton, &QPushButton::clicked, this, &MainWindow::submitDeposit);
    connect(endButton, &QPushButton::clicked, this, &MainWindow::logout);
    layout->addWidget(panel);
    layout->addStretch();
    layout->addLayout(createNavigationRow(menuButton, submitButton, endButton));
    return page;
}

QWidget* MainWindow::createWithdrawalPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(92, 52, 92, 46);
    layout->setSpacing(24);
    layout->addWidget(createHeading("PLEASE ENTER DESIRED AMOUNT"));
    layout->addWidget(createAccountInfoBar());

    QFrame* panel = new QFrame(page);
    panel->setObjectName("Panel");
    QVBoxLayout* panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(48, 42, 48, 42);
    panelLayout->setSpacing(22);
    withdrawalAmountEdit = createAmountEdit("Withdrawal Amount");
    connect(withdrawalAmountEdit, &QLineEdit::returnPressed, this, &MainWindow::submitWithdrawal);
    QLabel* note = createSubheading("Multiples of 500 only");
    note->setAlignment(Qt::AlignCenter);
    panelLayout->addWidget(withdrawalAmountEdit);
    panelLayout->addWidget(note);

    QPushButton* menuButton = createSecondaryButton("Back to Main Menu");
    QPushButton* withdrawButton = createPrimaryButton("Withdraw");
    QPushButton* endButton = createDangerButton("End Session");
    connect(menuButton, &QPushButton::clicked, this, &MainWindow::goMainMenu);
    connect(withdrawButton, &QPushButton::clicked, this, &MainWindow::submitWithdrawal);
    connect(endButton, &QPushButton::clicked, this, &MainWindow::logout);
    layout->addWidget(panel);
    layout->addStretch();
    layout->addLayout(createNavigationRow(menuButton, withdrawButton, endButton));
    return page;
}

QWidget* MainWindow::createPinChangePage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(92, 48, 92, 42);
    layout->setSpacing(24);
    layout->addWidget(createHeading("FOR YOUR SECURITY, CHOOSE A UNIQUE PIN"));
    layout->addWidget(createAccountInfoBar());

    QFrame* panel = new QFrame(page);
    panel->setObjectName("Panel");
    QFormLayout* form = new QFormLayout(panel);
    form->setContentsMargins(48, 38, 48, 38);
    form->setHorizontalSpacing(34);
    form->setVerticalSpacing(18);
    currentPinEdit = createPinEdit("Current PIN");
    newPinEdit = createPinEdit("New PIN");
    confirmPinEdit = createPinEdit("Confirm New PIN");
    form->addRow(createSubheading("Enter Current PIN"), createPinInputWithToggle(currentPinEdit));
    form->addRow(createSubheading("Enter New PIN"), createPinInputWithToggle(newPinEdit));
    form->addRow(createSubheading("Re-enter New PIN to Confirm"), createPinInputWithToggle(confirmPinEdit));

    QPushButton* cancelButton = createSecondaryButton("Cancel PIN Change");
    QPushButton* confirmButton = createPrimaryButton("Confirm New PIN");
    QPushButton* endButton = createDangerButton("End Session");
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::goMainMenu);
    connect(confirmButton, &QPushButton::clicked, this, &MainWindow::submitPinChange);
    connect(endButton, &QPushButton::clicked, this, &MainWindow::logout);

    layout->addWidget(panel);
    layout->addLayout(createNavigationRow(cancelButton, confirmButton, endButton));
    layout->addStretch();
    return page;
}

QWidget* MainWindow::createNewAccountPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(92, 42, 92, 42);
    layout->setSpacing(20);
    layout->addWidget(createHeading("CREATE NEW ACCOUNT"));

    QFrame* panel = new QFrame(page);
    panel->setObjectName("Panel");
    QFormLayout* form = new QFormLayout(panel);
    form->setContentsMargins(48, 38, 48, 38);
    form->setHorizontalSpacing(34);
    form->setVerticalSpacing(16);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->setFormAlignment(Qt::AlignHCenter);

    cnicEdit = new QLineEdit(panel);
    cnicEdit->setInputMask("00000-0000000-0;_");
    cnicEdit->setPlaceholderText("XXXXX-XXXXXXX-X");
    newAccountPinEdit = createPinEdit("Create New PIN");
    confirmAccountPinEdit = createPinEdit("Confirm PIN");

    const int fieldWidth = 420;
    cnicEdit->setFixedWidth(fieldWidth);
    newAccountPinEdit->setFixedWidth(fieldWidth);
    confirmAccountPinEdit->setFixedWidth(fieldWidth);

    QWidget* cnicWrapper = new QWidget(panel);
    QHBoxLayout* cnicLayout = new QHBoxLayout(cnicWrapper);
    cnicLayout->setContentsMargins(0, 0, 0, 0);
    cnicLayout->setSpacing(10);
    cnicLayout->addWidget(cnicEdit, 1);
    QWidget* dummySpacer = new QWidget(cnicWrapper);
    dummySpacer->setFixedWidth(58);
    cnicLayout->addWidget(dummySpacer);

    form->addRow(createSubheading("CNIC No."), cnicWrapper);
    form->addRow(createSubheading("Create New PIN"), createPinInputWithToggle(newAccountPinEdit));
    form->addRow(createSubheading("Re-enter PIN to Confirm"), createPinInputWithToggle(confirmAccountPinEdit));

    QPushButton* createButton = createPrimaryButton("Create Account");
    QPushButton* menuButton = createSecondaryButton("Back");
    connect(createButton, &QPushButton::clicked, this, &MainWindow::submitNewAccount);
    connect(menuButton, &QPushButton::clicked, this, &MainWindow::goWelcome);

    layout->addWidget(panel);
    layout->addLayout(createNavigationRow(menuButton, createButton));
    return page;
}

QWidget* MainWindow::createSuccessPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(96, 58, 96, 48);
    layout->setSpacing(24);

    QFrame* panel = new QFrame(page);
    panel->setObjectName("Panel");
    QVBoxLayout* panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(56, 48, 56, 48);
    panelLayout->setSpacing(22);

    successTitleLabel = createHeading("Transaction Successful");
    successTitleLabel->setAlignment(Qt::AlignCenter);
    successTitleLabel->setStyleSheet("color: #45d18b;");
    successDetailsLabel = createSubheading("");
    successDetailsLabel->setAlignment(Qt::AlignCenter);
    successDetailsLabel->setTextFormat(Qt::PlainText);
    panelLayout->addStretch();
    panelLayout->addWidget(successTitleLabel);
    panelLayout->addWidget(successDetailsLabel);
    panelLayout->addStretch();

    successReceiptButton = createSecondaryButton("Print Receipt");
    successReturnButton = createPrimaryButton("Back to Main Menu");
    QPushButton* endButton = createDangerButton("End Session");
    connect(successReceiptButton, &QPushButton::clicked, this, [this]() {
        showReceiptDialog(lastReceiptTitle, lastReceiptText);
        });
    connect(successReturnButton, &QPushButton::clicked, this, [this]() {
        if (successReturnsToMenu && atm.currentAccount != nullptr) {
            goMainMenu();
        }
        else {
            goWelcome();
        }
        });
    connect(endButton, &QPushButton::clicked, this, &MainWindow::logout);

    layout->addWidget(panel, 1);
    layout->addLayout(createNavigationRow(successReceiptButton, successReturnButton, endButton));
    return page;
}

QPushButton* MainWindow::createActionButton(const QString& title, const QString& subtitle, QStyle::StandardPixmap icon) {
    QPushButton* button = new QPushButton(this);
    button->setObjectName("ActionButton");
    button->setText(subtitle.isEmpty() ? title : title + "\n" + subtitle);
    button->setIcon(style()->standardIcon(icon));
    button->setIconSize(QSize(30, 30));
    button->setMinimumHeight(90);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return button;
}

QPushButton* MainWindow::createPrimaryButton(const QString& text) {
    QPushButton* button = new QPushButton(text, this);
    button->setObjectName("PrimaryButton");
    button->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    button->setIconSize(QSize(30, 30));
    button->setMinimumHeight(64);
    return button;
}

QPushButton* MainWindow::createSecondaryButton(const QString& text) {
    QPushButton* button = new QPushButton(text, this);
    button->setObjectName("SecondaryButton");
    button->setMinimumHeight(64);
    return button;
}

QPushButton* MainWindow::createDangerButton(const QString& text) {
    QPushButton* button = new QPushButton(text, this);
    button->setObjectName("DangerButton");
    button->setMinimumHeight(64);
    return button;
}

QLabel* MainWindow::createHeading(const QString& text) {
    QLabel* label = new QLabel(text, this);
    label->setObjectName("PageHeading");
    label->setWordWrap(true);
    return label;
}

QLabel* MainWindow::createSubheading(const QString& text) {
    QLabel* label = new QLabel(text, this);
    label->setObjectName("Subheading");
    label->setWordWrap(true);
    return label;
}

QLineEdit* MainWindow::createAmountEdit(const QString& placeholder) {
    QLineEdit* edit = new QLineEdit(this);
    edit->setPlaceholderText(placeholder + " in rupees");
    edit->setAlignment(Qt::AlignCenter);
    edit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{0,8}"), edit));
    return edit;
}

QLineEdit* MainWindow::createPinEdit(const QString& placeholder) {
    QLineEdit* edit = new QLineEdit(this);
    edit->setPlaceholderText(placeholder);
    edit->setEchoMode(QLineEdit::Password);
    edit->setMaxLength(PIN_LENGTH);
    edit->setAlignment(Qt::AlignCenter);
    edit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{0,4}"), edit));
    return edit;
}

QWidget* MainWindow::createPinInputWithToggle(QLineEdit* pinField) {
    QWidget* wrapper = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(wrapper);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    QPushButton* eyeButton = new QPushButton(wrapper);
    eyeButton->setObjectName("EyeButton");
    setupPinVisibilityToggle(pinField, eyeButton);

    layout->addWidget(pinField, 1);
    layout->addWidget(eyeButton);
    return wrapper;
}

void MainWindow::setupPinVisibilityToggle(QLineEdit* pinField, QPushButton* eyeButton) {
    pinField->setEchoMode(QLineEdit::Password);
    eyeButton->setText(QString::fromUtf8("👁"));
    eyeButton->setToolTip("Show PIN");
    eyeButton->setCheckable(true);
    pinToggles.push_back({ pinField, eyeButton });

    connect(eyeButton, &QPushButton::clicked, this, [pinField, eyeButton](bool checked) {
        pinField->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
        eyeButton->setText(checked ? QString::fromUtf8("✕") : QString::fromUtf8("👁"));
        eyeButton->setToolTip(checked ? "Hide PIN" : "Show PIN");
        });
}

void MainWindow::resetPinVisibilityToggles() {
    for (const PinToggle& toggle : pinToggles) {
        if (toggle.field != nullptr) {
            toggle.field->setEchoMode(QLineEdit::Password);
        }
        if (toggle.button != nullptr) {
            toggle.button->setChecked(false);
            toggle.button->setText(QString::fromUtf8("👁"));
            toggle.button->setToolTip("Show PIN");
        }
    }
}

QFrame* MainWindow::createAccountInfoBar() {
    QFrame* bar = new QFrame(this);
    bar->setObjectName("AccountInfoBar");
    QHBoxLayout* layout = new QHBoxLayout(bar);
    layout->setContentsMargins(22, 12, 22, 12);
    QLabel* label = new QLabel(bar);
    label->setObjectName("AccountInfoText");
    layout->addWidget(label);
    accountInfoLabels.push_back(label);
    return bar;
}

QHBoxLayout* MainWindow::createNavigationRow(QPushButton* leftButton, QPushButton* rightButton, QPushButton* dangerButton) {
    QHBoxLayout* row = new QHBoxLayout();
    row->setSpacing(18);
    if (leftButton != nullptr) {
        row->addWidget(leftButton);
    }
    row->addStretch();
    if (dangerButton != nullptr) {
        row->addWidget(dangerButton);
    }
    if (rightButton != nullptr) {
        row->addWidget(rightButton);
    }
    return row;
}

void MainWindow::setPage(PageIndex page) {
    resetPinVisibilityToggles();

    static const QStringList titles = {
        "WELCOME",
        "ACCOUNT NUMBER",
        "PIN REQUIRED",
        "MAIN MENU",
        "FAST CASH",
        "BALANCE INQUIRY",
        "MINI STATEMENT",
        "CASH DEPOSIT",
        "CASH WITHDRAWAL",
        "PIN CHANGE",
        "NEW ACCOUNT",
        "SUCCESS",
        "ADMIN DASHBOARD"
    };

    screenTitleLabel->setText(titles.value(page));
    stackedWidget->setCurrentIndex(page);
    footerTimeLabel->setText(QDateTime::currentDateTime().toString("dd MMM yyyy, hh:mm AP"));

    if (page == WelcomePage) {
        footerHintLabel->setText("Enter - Continue | I - Open New Account");
    }
    else if (page == PinPage) {
        footerHintLabel->setText("ESC - Eject Card");
    }
    else if (page == AccountNumberPage || page == NewAccountPage) {
        footerHintLabel->setText("ESC - Welcome");
    }
    else {
        footerHintLabel->setText("Use visible navigation buttons below. ESC also ends the session.");
    }
}

void MainWindow::goWelcome() {
    isAdminAttempt = false;
    adminPinAttempts = 0;
    atm.ejectCard();
    clearSensitiveFields();
    setPage(WelcomePage);
}

void MainWindow::goAccountNumber() {
    atm.ejectCard();
    clearSensitiveFields();
    setPage(AccountNumberPage);
    accountDigitsEdit->setFocus();
}

void MainWindow::goPin() {
    pinEdit->clear();
    if (pinSubmitButton != nullptr) {
        pinSubmitButton->setEnabled(false);
    }

    int attempts = 0;
    if (isAdminAttempt) {
        attempts = adminPinAttempts;
    }
    else {
        Account* account = atm.currentAccount;
        attempts = account == nullptr ? 0 : account->loginAttempts;
    }

    pinAttemptLabel->setText(QString("PIN attempts used: %1 of %2").arg(attempts).arg(MAX_PIN_ATTEMPTS));
    setPage(PinPage);
    pinEdit->setFocus();
}

void MainWindow::goMainMenu() {
    refreshMenuSummary();
    refreshAccountInfoBars();
    setPage(MainMenuPage);
}

void MainWindow::goFastCash() {
    refreshAccountInfoBars();
    setPage(FastCashPage);
}

void MainWindow::goBalance() {
    refreshAccountInfoBars();
    refreshBalance();
    setPage(BalancePage);
}

void MainWindow::goMiniStatement() {
    refreshAccountInfoBars();
    refreshMiniStatement();
    setPage(MiniStatementPage);
}

void MainWindow::goDeposit() {
    refreshAccountInfoBars();
    depositAmountEdit->clear();
    setPage(DepositPage);
    depositAmountEdit->setFocus();
}

void MainWindow::goWithdrawal() {
    refreshAccountInfoBars();
    withdrawalAmountEdit->clear();
    setPage(WithdrawalPage);
    withdrawalAmountEdit->setFocus();
}

void MainWindow::goPinChange() {
    refreshAccountInfoBars();
    currentPinEdit->clear();
    newPinEdit->clear();
    confirmPinEdit->clear();
    setPage(PinChangePage);
    currentPinEdit->setFocus();
}

void MainWindow::goNewAccount() {
    atm.ejectCard();
    clearSensitiveFields();
    setPage(NewAccountPage);
    cnicEdit->setFocus();
}

void MainWindow::submitAccountDigits() {
    const QString digits = accountDigitsEdit->text().trimmed();

    if (digits == QString::number(ADMIN_ACCNO)) {
        isAdminAttempt = true;
        goPin();
        return;
    }

    if (digits.length() != 4) {
        QMessageBox::warning(this, "Account Number", "Please enter exactly 4 numeric digits.");
        return;
    }

    bool found = false;
    for (int i = 0; i < atm.accountCount; i++) {
        std::string acc = atm.accounts[i]->getAccountNumber();
        if (acc.length() >= 4 && acc.substr(acc.length() - 4) == digits.toStdString()) {
            atm.currentAccount = atm.accounts[i];
            atm.cashAvailable = atm.currentAccount->getBalance();

            // Clear existing transactions to prevent duplicates on re-login
            for (int j = 0; j < atm.currentAccount->transactionCount; j++) {
                delete atm.currentAccount->transactionHistory[j];
                atm.currentAccount->transactionHistory[j] = nullptr;
            }
            atm.currentAccount->transactionCount = 0;

            atm.readTransactiondata();
            found = true;
            break;
        }
    }

    if (!found) {
        QMessageBox::warning(this, "Account Number", "Account not found. Please check the last 4 digits.");
        accountDigitsEdit->selectAll();
        accountDigitsEdit->setFocus();
        return;
    }

    Account* account = atm.currentAccount;
    if (account != nullptr && account->isAccountLocked()) {
        QMessageBox::critical(
            this,
            "Account Locked",
            "This account is locked. Please contact Admin.");
        atm.ejectCard();
        accountDigitsEdit->selectAll();
        accountDigitsEdit->setFocus();
        return;
    }

    goPin();
}

void MainWindow::submitPin() {
    const QString pin = pinEdit->text().trimmed();

    if (isAdminAttempt) {
        if (pin.toStdString() == ADMIN_PIN) {
            adminPinAttempts = 0;
            pinEdit->clear();
            goAdminDashboard();
            return;
        }
        else {
            adminPinAttempts++;
            if (adminPinAttempts >= MAX_PIN_ATTEMPTS) {
                QMessageBox::critical(this, "Admin Login", "Maximum PIN attempts reached.");
                goWelcome();
                return;
            }
            pinAttemptLabel->setText(QString("PIN attempts used: %1 of %2").arg(adminPinAttempts).arg(MAX_PIN_ATTEMPTS));
            QMessageBox::warning(this, "Admin Login", "Incorrect Admin PIN.");
            pinEdit->clear();
            return;
        }
    }

    if (pin.length() != PIN_LENGTH) {
        QMessageBox::warning(this, "PIN Required", "PIN must be exactly 4 digits.");
        return;
    }

    if (atm.currentAccount->validatePIN(pin.toStdString())) {
        pinEdit->clear();
        goMainMenu();
        return;
    }

    Account* account = atm.currentAccount;
    if (account != nullptr && account->isAccountLocked()) {
        QMessageBox::critical(this, "Card Ejected", "Account officially locked due to too many failed attempts.");
        atm.saveAccountsToFile(); // Save the lock status!
        goWelcome();
        return;
    }

    int attempts = account == nullptr ? 0 : account->loginAttempts;
    pinAttemptLabel->setText(QString("Incorrect PIN. You have used %1 of %2 attempts.")
        .arg(attempts)
        .arg(MAX_PIN_ATTEMPTS));
    pinEdit->clear();
    pinEdit->setFocus();
}

void MainWindow::appendPinDigit(const QString& digit) {
    if (pinEdit->text().length() < PIN_LENGTH) {
        pinEdit->insert(digit);
    }
}

void MainWindow::submitDeposit() {
    const QString rawAmount = depositAmountEdit->text().trimmed();
    if (rawAmount.isEmpty()) {
        QMessageBox::warning(this, "Deposit Amount", "Amount should not be empty.");
        depositAmountEdit->setFocus();
        return;
    }

    bool ok = false;
    const double amount = rawAmount.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Deposit Amount", "Please enter a valid numeric amount.");
        depositAmountEdit->setFocus();
        return;
    }

    if (amount <= 0.0) {
        QMessageBox::warning(this, "Deposit Amount", "Amount must be greater than 0.");
        depositAmountEdit->setFocus();
        return;
    }

    if (amount > 50000) {
        QMessageBox::warning(this, "Cash Deposit", "Error: Maximum deposit limit per transaction is Rs. 50,000.");
        return;
    }

    atm.currentAccount->credit(amount);
    atm.cashAvailable += amount;

    atm.saveAccountsToFile();

    const Account* account = atm.currentAccount;
    const QString details = QString("Amount Deposited: %1\nNew Balance: %2")
        .arg(moneyText(amount))
        .arg(account == nullptr ? moneyText(0) : moneyText(account->getBalance()));
    showSuccess("Deposit Successful", details, "Deposit Receipt", receiptText("Cash Deposit", amount), true);
}

void MainWindow::submitWithdrawal() {
    const QString rawAmount = withdrawalAmountEdit->text().trimmed();
    if (rawAmount.isEmpty()) {
        QMessageBox::warning(this, "Withdrawal Amount", "Amount should not be empty.");
        withdrawalAmountEdit->setFocus();
        return;
    }

    bool ok = false;
    const double amount = rawAmount.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Withdrawal Amount", "Please enter a valid numeric amount.");
        withdrawalAmountEdit->setFocus();
        return;
    }

    if (amount <= 0.0) {
        QMessageBox::warning(this, "Withdrawal Amount", "Amount must be greater than 0.");
        withdrawalAmountEdit->setFocus();
        return;
    }

    if (amount > 20000) {
        QMessageBox::warning(this, "Cash Withdrawal", "DENIED: Maximum withdrawal per transaction is Rs. 20,000.");
        return;
    }
    if (amount > atm.cashAvailable) {
        QMessageBox::warning(this, "Cash Withdrawal", "ERROR: ATM has insufficient cash reserve.");
        return;
    }

    if (!atm.currentAccount->debit(amount)) {
        QMessageBox::warning(this, "Cash Withdrawal", "Limit reached or insufficient funds.");
        return;
    }

    atm.cashAvailable -= amount;
    atm.saveAccountsToFile();

    const Account* account = atm.currentAccount;
    const QString details = QString("Amount Withdrawn: %1\nNew Balance: %2")
        .arg(moneyText(amount))
        .arg(account == nullptr ? moneyText(0) : moneyText(account->getBalance()));
    showSuccess("Withdrawal Successful", details, "Withdrawal Receipt", receiptText("Cash Withdrawal", amount), true);
}

void MainWindow::submitFastCash(double amount) {
    if (amount > 20000) {
        QMessageBox::warning(this, "Fast Cash", "DENIED: Maximum withdrawal per transaction is Rs. 20,000.");
        return;
    }
    if (amount > atm.cashAvailable) {
        QMessageBox::warning(this, "Fast Cash", "ERROR: ATM has insufficient cash reserve.");
        return;
    }

    if (!atm.currentAccount->debit(amount)) {
        QMessageBox::warning(this, "Fast Cash", "Limit reached or insufficient funds.");
        return;
    }

    atm.cashAvailable -= amount;
    atm.saveAccountsToFile();

    const Account* account = atm.currentAccount;
    const QString details = QString("Amount Withdrawn: %1\nNew Balance: %2")
        .arg(moneyText(amount))
        .arg(account == nullptr ? moneyText(0) : moneyText(account->getBalance()));
    showSuccess("Withdrawal Successful", details, "Fast Cash Receipt", receiptText("Fast Cash", amount), true);
}

void MainWindow::submitPinChange() {
    const QString currentPIN = currentPinEdit->text().trimmed();
    const QString newPIN = newPinEdit->text().trimmed();
    const QString confirmPIN = confirmPinEdit->text().trimmed();

    if (currentPIN.length() != PIN_LENGTH || newPIN.length() != PIN_LENGTH || confirmPIN.length() != PIN_LENGTH) {
        QMessageBox::warning(this, "PIN Change", "PIN must be exactly 4 digits.");
        return;
    }

    if (newPIN != confirmPIN) {
        QMessageBox::warning(this, "PIN Change", "PIN confirmation does not match.");
        return;
    }

    if (!atm.currentAccount->validatePIN(currentPIN.toStdString())) {
        QMessageBox::warning(this, "PIN Change", "Incorrect current PIN.");
        return;
    }

    atm.currentAccount->changePIN(newPIN.toStdString());
    atm.saveAccountsToFile();

    clearSensitiveFields();
    showSuccess(
        "PIN Changed Successfully",
        "Your ATM PIN has been updated.\nUse the new PIN the next time you sign in.",
        "PIN Change Receipt",
        receiptText("PIN Change", 0.0),
        true);
}

void MainWindow::submitNewAccount() {
    const QString cnic = digitsOnly(cnicEdit->text());
    const QString pin = newAccountPinEdit->text().trimmed();
    const QString confirmPIN = confirmAccountPinEdit->text().trimmed();

    if (cnic.length() != 13) {
        QMessageBox::warning(this, "New Account", "Invalid CNIC format. Use XXXXX-XXXXXXX-X.");
        cnicEdit->setFocus();
        return;
    }

    if (pin.length() != PIN_LENGTH || confirmPIN.length() != PIN_LENGTH) {
        QMessageBox::warning(this, "New Account", "PIN must be exactly 4 digits.");
        return;
    }

    if (pin != confirmPIN) {
        QMessageBox::warning(this, "New Account", "PIN confirmation does not match.");
        return;
    }

    std::string accountNumber = std::to_string(STARTING_ACC_NUM + atm.accountCount);

    Account* newAcc = new CurrentAccount();
    newAcc->setdata(accountNumber, "New User", cnic.toStdString(), "00000000000", 1000.0, pin.toStdString(), false);

    atm.addAccount(newAcc);
    atm.saveAccountsToFile();

    const QString generated = QString::fromStdString(accountNumber);
    const QString details = QString("Account Number: %1\nRemember Last 4 Digits: %2")
        .arg(generated)
        .arg(generated.right(4));
    const QString receipt = QString(
        "Fast NUCES Banking\n"
        "NEW ACCOUNT RECEIPT\n"
        "--------------------------------\n"
        "Date/Time       : %1\n"
        "Account Number  : %2\n"
        "Last 4 Digits   : %3\n"
        "--------------------------------\n"
        "Never share your PIN.")
        .arg(QDateTime::currentDateTime().toString("dd MMM yyyy, hh:mm:ss AP"))
        .arg(generated)
        .arg(generated.right(4));
    showSuccess("Account Created Successfully", details, "New Account Receipt", receipt, false);
}

void MainWindow::logout() {
    if (!confirmEndSession()) {
        return;
    }
    goWelcome();
}

bool MainWindow::confirmEndSession() {
    if (atm.currentAccount == nullptr) {
        return true;
    }

    QMessageBox confirmation(this);
    confirmation.setWindowTitle("End Session");
    confirmation.setText("Are you sure you want to end your session?");
    confirmation.setInformativeText("Your card will be ejected and this ATM session will close.");
    confirmation.setIcon(QMessageBox::Question);
    QPushButton* cancelButton = confirmation.addButton("Cancel", QMessageBox::RejectRole);
    QPushButton* endButton = confirmation.addButton("End Session", QMessageBox::AcceptRole);
    confirmation.setDefaultButton(cancelButton);
    confirmation.exec();
    return confirmation.clickedButton() == endButton;
}

void MainWindow::showSuccess(
    const QString& title,
    const QString& details,
    const QString& receiptTitle,
    const QString& receiptTextValue,
    bool returnToMenu) {
    lastReceiptTitle = receiptTitle;
    lastReceiptText = receiptTextValue;
    successReturnsToMenu = returnToMenu;
    successTitleLabel->setText(title);
    successDetailsLabel->setText(details);
    successReceiptButton->setVisible(!receiptTextValue.isEmpty());
    successReturnButton->setText(returnToMenu ? "Back to Main Menu" : "Back to Welcome");
    refreshAccountInfoBars();
    setPage(SuccessPage);
}

void MainWindow::refreshMenuSummary() {
    Account* account = atm.currentAccount;
    if (account == nullptr) {
        menuAccountLabel->setText("No active account");
        menuBalanceLabel->clear();
        return;
    }

    menuAccountLabel->setText(
        QString("Account: **** %1     Type: %2")
        .arg(currentAccountLastFour())
        .arg(QString::fromStdString(account->getAccountType())));
    menuBalanceLabel->setText("Available Balance: " + moneyText(account->getBalance()));
    refreshAccountInfoBars();
}

void MainWindow::refreshAccountInfoBars() {
    Account* account = atm.currentAccount;
    QString text = "No active account";
    if (account != nullptr) {
        text = QString("Account: **** %1     Type: %2     Available Balance: %3")
            .arg(currentAccountLastFour())
            .arg(QString::fromStdString(account->getAccountType()).replace(" Account", ""))
            .arg(moneyText(account->getBalance()));
    }

    for (QLabel* label : accountInfoLabels) {
        if (label != nullptr) {
            label->setText(text);
        }
    }
}

void MainWindow::refreshBalance() {
    Account* account = atm.currentAccount;
    if (account == nullptr) {
        balanceTypeValue->setText("N/A");
        balanceAmountValue->setText(moneyText(0));
        return;
    }

    balanceTypeValue->setText(QString::fromStdString(account->getAccountType()));
    balanceAmountValue->setText(moneyText(account->getBalance()));
}

void MainWindow::refreshMiniStatement() {
    statementTable->setRowCount(0);
    Account* account = atm.currentAccount;
    if (account == nullptr) return;

    std::vector<Transaction> transactions;
    for (int i = 0; i < account->transactionCount; i++) {
        if (account->transactionHistory[i] != nullptr) {
            transactions.push_back(*(account->transactionHistory[i]));
        }
    }

    statementEmptyLabel->setVisible(transactions.empty());
    statementTable->setVisible(!transactions.empty());

    for (int row = 0; row < static_cast<int>(transactions.size()); row++) {
        const Transaction& transaction = transactions[row];
        statementTable->insertRow(row);

        QString type = QString::fromStdString(transaction.typeToString()).replace('_', ' ');
        QString amount = moneyText(transaction.amount);
        QColor amountColor("#d9e8ff");
        if (transaction.type == TransactionType::DEPOSIT) {
            amount = "+ " + amount;
            amountColor = QColor("#45d18b");
        }
        else if (transaction.type == TransactionType::WITHDRAWAL ||
            transaction.type == TransactionType::FAST_CASH) {
            amount = "- " + amount;
            amountColor = QColor("#ff665c");
        }

        QTableWidgetItem* dateItem = new QTableWidgetItem(formattedTimestamp(QString::fromStdString(transaction.timestamp)));
        QTableWidgetItem* typeItem = new QTableWidgetItem(type);
        QTableWidgetItem* amountItem = new QTableWidgetItem(amount);
        QTableWidgetItem* balanceItem = new QTableWidgetItem(moneyText(transaction.balanceAfter));
        dateItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        typeItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        amountItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
        balanceItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
        amountItem->setForeground(amountColor);

        statementTable->setItem(row, 0, dateItem);
        statementTable->setItem(row, 1, typeItem);
        statementTable->setItem(row, 2, amountItem);
        statementTable->setItem(row, 3, balanceItem);
    }
}

void MainWindow::clearSensitiveFields() {
    resetPinVisibilityToggles();
    if (accountDigitsEdit) accountDigitsEdit->clear();
    if (pinEdit) pinEdit->clear();
    if (depositAmountEdit) depositAmountEdit->clear();
    if (withdrawalAmountEdit) withdrawalAmountEdit->clear();
    if (currentPinEdit) currentPinEdit->clear();
    if (newPinEdit) newPinEdit->clear();
    if (confirmPinEdit) confirmPinEdit->clear();
    if (cnicEdit) cnicEdit->clear();
    if (newAccountPinEdit) newAccountPinEdit->clear();
    if (confirmAccountPinEdit) confirmAccountPinEdit->clear();
}

void MainWindow::maybeShowReceipt(const QString& transactionType, double amount) {
    const QMessageBox::StandardButton choice = QMessageBox::question(
        this,
        "Receipt",
        "Print receipt for this transaction?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::Yes);

    if (choice == QMessageBox::Yes) {
        showReceiptDialog(transactionType + " Receipt", receiptText(transactionType, amount));
    }
}

void MainWindow::showReceiptDialog(const QString& title, const QString& text) {
    QDialog dialog(this);
    dialog.setWindowTitle(title);
    dialog.resize(520, 520);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QTextEdit* receiptView = new QTextEdit(&dialog);
    receiptView->setReadOnly(true);
    receiptView->setPlainText(text);
    receiptView->setStyleSheet("font-family: Menlo, Monaco, monospace; font-size: 14px;");

    QDialogButtonBox* buttons = new QDialogButtonBox(&dialog);
    QPushButton* saveButton = buttons->addButton("Save .txt", QDialogButtonBox::ActionRole);
    QPushButton* closeButton = buttons->addButton(QDialogButtonBox::Close);
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(saveButton, &QPushButton::clicked, this, [this, text]() {
        const QString path = QFileDialog::getSaveFileName(
            this,
            "Save Receipt",
            "atm-receipt.txt",
            "Text Files (*.txt)");
        if (path.isEmpty()) {
            return;
        }

        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Save Receipt", "Could not save the receipt file.");
            return;
        }

        QTextStream stream(&file);
        stream << text;
        });

    layout->addWidget(receiptView);
    layout->addWidget(buttons);
    dialog.exec();
}

QString MainWindow::moneyText(double amount, int decimals) const {
    QLocale locale(QLocale::English, QLocale::Pakistan);
    return "Rs. " + locale.toString(amount, 'f', decimals);
}

QString MainWindow::currentAccountLastFour() const {
    Account* account = atm.currentAccount;
    if (account == nullptr) {
        return "0000";
    }

    const QString accountNumber = QString::fromStdString(account->getAccountNumber());
    return accountNumber.right(4);
}

QString MainWindow::digitsOnly(const QString& value) const {
    QString digits;
    for (const QChar ch : value) {
        if (ch.isDigit()) {
            digits.append(ch);
        }
    }
    return digits;
}

QString MainWindow::formattedTimestamp(const QString& timestamp) const {
    QString normalized = timestamp.simplified();
    QDateTime parsed = QDateTime::fromString(normalized, "ddd MMM d HH:mm:ss yyyy");
    if (!parsed.isValid()) {
        parsed = QDateTime::fromString(normalized, "ddd MMM dd HH:mm:ss yyyy");
    }
    if (!parsed.isValid()) {
        return timestamp;
    }
    return parsed.toString("dd MMM yyyy, hh:mm AP");
}

QString MainWindow::receiptText(const QString& transactionType, double amount) const {
    Account* account = atm.currentAccount;
    QStringList lines;
    lines << "Fast NUCES Banking";
    lines << "Smart Campus Banking System";
    lines << "ATM TRANSACTION RECEIPT";
    lines << "--------------------------------";
    lines << "Date/Time       : " + QDateTime::currentDateTime().toString("dd MMM yyyy, hh:mm:ss AP");
    lines << "Account         : **** " + currentAccountLastFour();
    if (account != nullptr) {
        lines << "Account Type    : " + QString::fromStdString(account->getAccountType());
    }
    lines << "Transaction     : " + transactionType;
    if (amount > 0.0) {
        lines << "Amount          : " + moneyText(amount);
    }
    if (account != nullptr) {
        lines << "Available Bal.  : " + moneyText(account->getBalance());
    }
    lines << "--------------------------------";
    lines << "Thank you for banking with Fast NUCES Banking.";
    return lines.join('\n');
}

QString MainWindow::statementText() const {
    Account* account = atm.currentAccount;
    QStringList lines;
    lines << "Fast NUCES Banking";
    lines << "Smart Campus Banking System";
    lines << "MINI STATEMENT";
    lines << "--------------------------------";
    lines << "Date/Time       : " + QDateTime::currentDateTime().toString("dd MMM yyyy, hh:mm:ss AP");
    lines << "Account         : **** " + currentAccountLastFour();
    if (account != nullptr) {
        lines << "Account Type    : " + QString::fromStdString(account->getAccountType());
    }
    lines << "--------------------------------";

    std::vector<Transaction> transactions;
    if (account != nullptr) {
        for (int i = 0; i < account->transactionCount; i++) {
            if (account->transactionHistory[i] != nullptr) {
                transactions.push_back(*(account->transactionHistory[i]));
            }
        }
    }

    if (transactions.empty()) {
        lines << "No transactions yet.";
    }
    else {
        for (const Transaction& transaction : transactions) {
            lines << QString("%1 | %2 | %3 | Bal %4")
                .arg(formattedTimestamp(QString::fromStdString(transaction.timestamp)))
                .arg(QString::fromStdString(transaction.typeToString()))
                .arg(moneyText(transaction.amount))
                .arg(moneyText(transaction.balanceAfter));
        }
    }

    lines << "--------------------------------";
    lines << "Never share your PIN.";
    return lines.join('\n');
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    const int page = stackedWidget->currentIndex();
    if (page == WelcomePage && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)) {
        goAccountNumber();
        return;
    }

    if (page == WelcomePage && event->key() == Qt::Key_I) {
        goNewAccount();
        return;
    }

    if (event->key() == Qt::Key_Escape) {
        if (page == WelcomePage) return;

        if (page == AccountNumberPage || page == NewAccountPage || page == AdminDashboardPage) {
            goWelcome();
            return;
        }
        if (page == PinPage) {
            goAccountNumber();
            return;
        }
        if (page == MainMenuPage) {
            logout();
            return;
        }

        goMainMenu();
        return;
    }

    QMainWindow::keyPressEvent(event);
}

QWidget* MainWindow::createAdminDashboardPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(60, 40, 60, 40);
    layout->setSpacing(20);

    QLabel* heading = createHeading("ADMIN DASHBOARD");
    layout->addWidget(heading);

    adminAccountsTable = new QTableWidget(this);
    adminAccountsTable->setColumnCount(5);
    adminAccountsTable->setHorizontalHeaderLabels({ "Account No", "Type", "Balance", "Status", "Action" });
    adminAccountsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    adminAccountsTable->setSelectionMode(QAbstractItemView::NoSelection);
    adminAccountsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    adminAccountsTable->verticalHeader()->setVisible(false);
    layout->addWidget(adminAccountsTable, 1);

    adminReturnButton = createPrimaryButton("Logout");
    connect(adminReturnButton, &QPushButton::clicked, this, &MainWindow::goWelcome);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(adminReturnButton);
    layout->addLayout(btnLayout);

    return page;
}

void MainWindow::goAdminDashboard() {
    clearSensitiveFields();
    refreshAdminDashboard();
    setPage(AdminDashboardPage);
}

void MainWindow::refreshAdminDashboard() {
    adminAccountsTable->setRowCount(0);
    for (int i = 0; i < atm.accountCount; i++) {
        Account* acc = atm.accounts[i];
        if (!acc) continue;

        int row = adminAccountsTable->rowCount();
        adminAccountsTable->insertRow(row);
        adminAccountsTable->setRowHeight(row, 64);

        QTableWidgetItem* accNoItem = new QTableWidgetItem(QString::fromStdString(acc->getAccountNumber()));

        QString typeStr = QString::fromStdString(acc->getAccountType());
        typeStr.replace(" Account", "");
        QTableWidgetItem* typeItem = new QTableWidgetItem(typeStr);

        QTableWidgetItem* balItem = new QTableWidgetItem(moneyText(acc->getBalance()));
        QTableWidgetItem* statusItem = new QTableWidgetItem(acc->isAccountLocked() ? "LOCKED" : "ACTIVE");

        accNoItem->setTextAlignment(Qt::AlignCenter);
        typeItem->setTextAlignment(Qt::AlignCenter);
        balItem->setTextAlignment(Qt::AlignCenter);
        statusItem->setTextAlignment(Qt::AlignCenter);

        if (acc->isAccountLocked()) {
            statusItem->setForeground(QColor("#D32F2F"));
        }
        else {
            statusItem->setForeground(QColor("#43A047"));
        }

        adminAccountsTable->setItem(row, 0, accNoItem);
        adminAccountsTable->setItem(row, 1, typeItem);
        adminAccountsTable->setItem(row, 2, balItem);
        adminAccountsTable->setItem(row, 3, statusItem);

        QPushButton* toggleBtn = new QPushButton(acc->isAccountLocked() ? "Unlock" : "Lock");
        toggleBtn->setCursor(Qt::PointingHandCursor);
        toggleBtn->setStyleSheet(acc->isAccountLocked()
            ? "background-color: #008B8B; color: #FFFFFF; font-weight: bold; border-radius: 5px; padding: 0px 10px; min-height: 32px; max-height: 32px;"
            : "background-color: #FFCDD2; color: #B71C1C; font-weight: bold; border-radius: 5px; padding: 0px 10px; min-height: 32px; max-height: 32px;");

        connect(toggleBtn, &QPushButton::clicked, this, [this, acc]() {
            acc->setLock(!acc->isAccountLocked());
            atm.saveAccountsToFile();
            refreshAdminDashboard();
            });

        QWidget* btnWidget = new QWidget();
        QHBoxLayout* btnLayout = new QHBoxLayout(btnWidget);
        btnLayout->setContentsMargins(5, 5, 5, 5);
        btnLayout->addWidget(toggleBtn);
        adminAccountsTable->setCellWidget(row, 4, btnWidget);
    }
}
