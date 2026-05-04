#include "../include/ATM.h"
#include "../include/CurrentAccount.h"
#include "../include/SavingsAccount.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#ifndef ATM_DEFAULT_DATA_DIR
#define ATM_DEFAULT_DATA_DIR "data"
#endif

namespace {
    vector<string> splitLine(const string& line, char delimiter) {
        vector<string> parts;
        string item;
        stringstream input(line);
        while (getline(input, item, delimiter)) {
            parts.push_back(item);
        }
        return parts;
    }

    bool isDigitsOnly(const string& value) {
        return !value.empty() && all_of(value.begin(), value.end(), [](unsigned char ch) {
            return ch >= '0' && ch <= '9';
        });
    }

    string normalizeAccountType(const string& value) {
        string normalized = value;
        transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char ch) {
            return static_cast<char>(tolower(ch));
        });
        if (normalized.find("saving") != string::npos) {
            return "Savings";
        }
        return "Current";
    }
}


ATM::ATM(double initialCash, string dataDir) {
    cashAvailable = initialCash;
    accountCount = 0;
    currentAccount = nullptr;
    dataDirectory = dataDir.empty() ? ATM_DEFAULT_DATA_DIR : dataDir;
    for (int i = 0; i < MAX_ACCOUNTS; i++) accounts[i] = nullptr;
}

ATM::~ATM() {
    saveData();
    clearAccounts();
}

void ATM::clearAccounts() {
    for (int i = 0; i < accountCount; i++) {
        delete accounts[i];
        accounts[i] = nullptr;
    }
    accountCount = 0;
    currentAccount = nullptr;
}

void ATM::addAccount(Account* acc) {
    if (acc == nullptr) {
        return;
    }

    if (accountCount < MAX_ACCOUNTS) {
        accounts[accountCount++] = acc;
        return;
    }

    delete acc;
}

string ATM::accountsFilePath() const {
    return dataDirectory + "/accounts.txt";
}

string ATM::transactionsFilePath() const {
    return dataDirectory + "/transactions.txt";
}

void ATM::seedDefaultAccounts() {
    addAccount(new CurrentAccount());
    addAccount(new SavingsAccount());
    if (accounts[0] != nullptr) {
        accounts[0]->addTransaction(TransactionType::ACCOUNT_CREATED, 0.0, "Default account loaded");
    }
    if (accounts[1] != nullptr) {
        accounts[1]->addTransaction(TransactionType::ACCOUNT_CREATED, 0.0, "Default account loaded");
    }
}

string ATM::generateAccountNumber() const {
    int highest = STARTING_ACC_NUM - 1;
    for (int i = 0; i < accountCount; i++) {
        try {
            highest = max(highest, stoi(accounts[i]->getAccountNumber()));
        }
        catch (...) {
        }
    }

    return to_string(highest + 1);
}

Account* ATM::getCurrentAccount() {
    return currentAccount;
}

const Account* ATM::getCurrentAccount() const {
    return currentAccount;
}

int ATM::searchAcc(string accNum) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i]->getAccountNumber() == accNum) return i;
    }
    return -1;
}

Account* ATM::findAccountByLastFourDigits(string lastFourDigits) const {
    if (lastFourDigits.length() != 4 || !isDigitsOnly(lastFourDigits)) {
        return nullptr;
    }

    for (int i = 0; i < accountCount; i++) {
        string accountNumber = accounts[i]->getAccountNumber();
        if (accountNumber.length() >= 4 &&
            accountNumber.substr(accountNumber.length() - 4) == lastFourDigits) {
            return accounts[i];
        }
    }

    return nullptr;
}

bool ATM::insertCardByLastFourDigits(string lastFourDigits) {
    Account* account = findAccountByLastFourDigits(lastFourDigits);
    if (account == nullptr) {
        cout << "[ERROR] Account last four digits were not found.\n";
        return false;
    }

    currentAccount = account;
    return true;
}

void ATM::start() {
    int choice;
    bool systemRunning = true;

    while (systemRunning) {
        cout << "\n====================================\n";
        cout << "      WELCOME TO THE ATM SYSTEM     \n";
        cout << "====================================\n";
        cout << "1. Insert Card\n";
        cout << "0. Power Off System\n";
        cout << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(1000, '\n');
            cout << "[INVALID] Please enter 1 or 0.\n";
            continue;
        }

        if (choice == 0) {
            cout << "Shutting down system... Goodbye!\n";
            systemRunning = false;
        }
        else if (choice == 1) {
            int accAttempts = 0;
            bool sessionActive = false;

            // Account Number Identification 
            while (accAttempts < 3) {
                string accNum;
                cout << "\nEnter Account Number: ";
                cin >> accNum;

                if (insertCard(accNum)) {
                    // Check if account is already locked before proceeding
                    if (currentAccount->isAccountLocked()) {
                        cout << LOCKED_ACCOUNT_MESSAGE << "\n";
                        sessionActive = true;
                        break;
                    }

                    //  PIN Verification 
                    int pinAttempts = 0;
                    bool pinSuccess = false;

                    while (pinAttempts < 3) {
                        string pin;
                        cout << "Enter 4-Digit PIN: ";
                        cin >> pin;

                        // validatePIN handles the internal counter and locking logic
                        if (currentAccount->validatePIN(pin)) {
                            cout << "Login accepted.\n";
                            pinSuccess = true;
                            showMainMenu(); // Enter banking system
                            break;
                        }
                        else {
                            pinAttempts++;
                            if (currentAccount->isAccountLocked()) {
                                cout << "\n[SECURITY ALERT] Wrong PIN entered 3 times.\n";
                                cout << "[!] ACCOUNT OFFICIALLY LOCKED.\n";
                                break;
                            }
                            cout << "Incorrect PIN. Try again.\n";
                        }
                    }

                    sessionActive = true;
                    break;
                }
                else {
                    accAttempts++;
                    if (accAttempts < 3) cout << "[ERROR] Account not recognized.\n";
                }
            }

            // Security cleanup after 3 failed account number tries
            if (!sessionActive && accAttempts == 3) {
                cout << "\n[SECURITY] 3 Invalid Account attempts. CARD EJECTED.\n";
            }

            // Ensure card is "ejected" (currentAccount set to null) for next user
            ejectCard();
        }
    }
}

bool ATM::insertCard(string accNum) {
    int temp = searchAcc(accNum);
    if (temp != -1) {
        currentAccount = accounts[temp];
        return true;
    }
    cout << "[ERROR] Card unrecognized or invalid account.\n";
    return false;
}

bool ATM::enterPIN(string pin) {
    if (currentAccount != nullptr && currentAccount->validatePIN(pin)) {
        cout << "[SUCCESS] Access Granted.\n";
        return true;
    }
    cout << "[DENIED] Incorrect PIN.\n";
    return false;
}

bool ATM::authenticatePin(string pin, string& message) {
    if (currentAccount == nullptr) {
        message = "No account is selected.";
        return false;
    }

    if (currentAccount->isAccountLocked()) {
        message = LOCKED_ACCOUNT_MESSAGE;
        return false;
    }

    if (currentAccount->validatePIN(pin)) {
        message = "Access granted.";
        saveData();
        return true;
    }

    if (currentAccount->isAccountLocked()) {
        message = LOCKED_ACCOUNT_MESSAGE;
        saveData();
        return false;
    }

    int remaining = MAX_PIN_ATTEMPTS - currentAccount->getLoginAttempts();
    message = "Incorrect PIN. Attempts remaining: " + to_string(remaining);
    saveData();
    return false;
}

void ATM::withdraw(double amount) {
    
    if (amount > ATM_WITHDRAWAL_TRANSACTION_LIMIT) {
        cout << "DENIED: Maximum withdrawal per transaction is Rs. 20,000.\n";
        return;
    }

    if (amount > cashAvailable) {
        cout << "ERROR: ATM has insufficient cash reserve.\n";
        return;
    }

    // currentAccount->debit will handle the Daily Limit check (50,000)
    if (currentAccount->debit(amount)) {
        cashAvailable -= amount;
        cout << "[SUCCESS] Please collect your cash: Rs. " << amount << endl;
    }
}

bool ATM::withdrawAmount(double amount) {
    string message;
    return withdrawAmount(amount, message, false);
}

bool ATM::withdrawAmount(double amount, string& message, bool fastCashTransaction) {
    if (currentAccount == nullptr) {
        message = "Please select and authenticate an account first.";
        return false;
    }

    if (currentAccount->isAccountLocked()) {
        message = LOCKED_ACCOUNT_MESSAGE;
        return false;
    }

    if (!std::isfinite(amount)) {
        message = "Please enter a valid numeric amount.";
        return false;
    }

    if (amount <= 0) {
        message = "Withdrawal amount must be greater than zero.";
        return false;
    }

    if (fmod(amount, 500.0) != 0.0) {
        message = "Withdrawal amount must be a multiple of Rs. 500.";
        return false;
    }

    if (amount > ATM_WITHDRAWAL_TRANSACTION_LIMIT) {
        message = "Maximum withdrawal per transaction is Rs. 20,000.";
        return false;
    }

    if (amount > cashAvailable) {
        message = "ATM has insufficient cash reserve.";
        return false;
    }

    TransactionType type = fastCashTransaction ? TransactionType::FAST_CASH : TransactionType::WITHDRAWAL;
    string description = fastCashTransaction ? "Fast Cash Withdrawal" : "ATM Withdrawal";
    if (currentAccount->debit(amount, type, description)) {
        cashAvailable -= amount;
        saveData();
        message = "Please collect your cash.";
        return true;
    }

    message = currentAccount->getLastError().empty()
        ? "Withdrawal denied by account limits or available balance."
        : currentAccount->getLastError();
    return false;
}

void ATM::fastCash() {
   
    double options[] = { 500, 1000, 2000, 5000, 10000, 20000 };
    cout << "\n======= FAST CASH =======\n";
    for (int i = 0; i < 6; i++) {
        cout << i + 1 << ". Rs. " << options[i] << endl;
    }

    int choice;
    cout << "Choice: "; 
    cin >> choice;

    if (choice >= 1 && choice <= 6) {
        // Fast cash automatically passes the amount to the withdraw function
        withdraw(options[choice - 1]);
    }
    else {
        cout << "Selection out of range.\n";
    }
}

void ATM::showMainMenu() {
    int choice = -1;
    while (choice != 0) {
        cout << "\n---------- MAIN MENU ----------\n";
        cout << "1. Balance Inquiry\n";
        cout << "2. Cash Withdrawal\n";
        cout << "3. Cash Deposit\n";     
        cout << "4. Fast Cash\n";
        cout << "5. Change PIN\n";        
        cout << "6. Mini Statement\n";
        cout << "0. Logout / Exit\n";
        cout << "-------------------------------\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: checkBalance(); break;
        case 2: {
            double amt; cout << "Amount: "; cin >> amt;
            withdraw(amt); break;
        }
        case 3: deposit(); break;
        case 4: fastCash(); break;
        case 5: changePIN(); break;
        case 6: currentAccount->printMiniStatement(); break;
        case 0: ejectCard(); break;
        default: cout << "Invalid choice.\n";
        }
    }
}

void ATM::deposit() {
    double amount;
    cout << "\n======= CASH DEPOSIT =======\n";
    cout << "Enter amount to deposit: Rs. ";
    cin >> amount;

    if (currentAccount == nullptr) {
        cout << "Please authenticate before depositing cash.\n";
        return;
    }

    if (currentAccount->isAccountLocked()) {
        cout << LOCKED_ACCOUNT_MESSAGE << "\n";
        return;
    }

    if (!std::isfinite(amount) || amount <= 0) {
        cout << "Deposit amount must be greater than zero.\n";
        return;
    }

    currentAccount->credit(amount);
    cashAvailable += amount;
    cout << "Deposit complete.\n";
}

bool ATM::depositAmount(double amount) {
    string message;
    return depositAmount(amount, message);
}

bool ATM::depositAmount(double amount, string& message) {
    if (currentAccount == nullptr) {
        message = "Please select and authenticate an account first.";
        return false;
    }

    if (currentAccount->isAccountLocked()) {
        message = LOCKED_ACCOUNT_MESSAGE;
        return false;
    }

    if (!std::isfinite(amount)) {
        message = "Please enter a valid numeric amount.";
        return false;
    }

    if (amount <= 0) {
        message = "Deposit amount must be greater than zero.";
        return false;
    }

    currentAccount->credit(amount);
    cashAvailable += amount;
    saveData();
    message = "Deposit complete.";
    return true;
}

void ATM::checkBalance() {
    cout << "\n======= BALANCE INQUIRY =======\n";
    cout << "Account Holder: " << currentAccount->getAccountNumber() << endl;
    cout << "Current Balance: Rs. " << currentAccount->getBalance() << endl;
}

void ATM::changePIN() {
    string oldPin, newPin, confirmPin;
    cout << "\n======= PIN CHANGE =======\n";
    cout << "Enter Current PIN: ";
    cin >> oldPin;

    // Verify current PIN before allowing change
    if (currentAccount->validatePIN(oldPin)) {
        cout << "Enter New 4-Digit PIN: ";
        cin >> newPin;

        
        if (newPin.length() != 4) {
            cout << "PIN must be exactly 4 digits.\n";
            return;
        }

        cout << "Confirm New PIN: ";
        cin >> confirmPin;

        if (newPin == confirmPin) {
            currentAccount->changePIN(newPin);
            cout << "PIN updated. Please use your new PIN next time.\n";
        }
        else {
            cout << "PINs do not match. Change aborted.\n";
        }
    }
    else {
        cout << "Incorrect current PIN.\n";
    }
}

void ATM::miniStatement() {
    if (currentAccount != nullptr) {
        currentAccount->printMiniStatement();
    }
}

vector<Transaction> ATM::getCurrentTransactions(int count) const {
    if (currentAccount == nullptr || currentAccount->isAccountLocked()) {
        return {};
    }
    return currentAccount->getLastTransactions(count);
}

bool ATM::createAccount(string cnicNo, string newPIN, string accountType, string& generatedAccountNumber, string& message) {
    if (accountCount >= MAX_ACCOUNTS) {
        message = "Account limit reached.";
        return false;
    }

    if (cnicNo.length() != 13 || !isDigitsOnly(cnicNo)) {
        message = "CNIC must contain exactly 13 numeric digits.";
        return false;
    }

    if (newPIN.length() != PIN_LENGTH || !isDigitsOnly(newPIN)) {
        message = "PIN must be exactly 4 numeric digits.";
        return false;
    }

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i]->getCnic() == cnicNo) {
            message = "An account already exists for this CNIC.";
            return false;
        }
    }

    generatedAccountNumber = generateAccountNumber();
    string holder = "Customer " + generatedAccountNumber;
    Account* account = nullptr;
    string normalizedType = normalizeAccountType(accountType);

    if (normalizedType == "Savings") {
        account = new SavingsAccount(generatedAccountNumber, holder, 0.0, newPIN, cnicNo);
    }
    else {
        account = new CurrentAccount(generatedAccountNumber, holder, 0.0, newPIN, cnicNo);
    }

    account->addTransaction(TransactionType::ACCOUNT_CREATED, 0.0, "Account Created");
    addAccount(account);
    saveData();
    message = "Account created successfully.";
    return true;
}

bool ATM::loadData() {
    clearAccounts();
    filesystem::create_directories(dataDirectory);

    ifstream accountFile(accountsFilePath());
    if (!accountFile.is_open()) {
        seedDefaultAccounts();
        saveData();
        return false;
    }

    string line;
    while (getline(accountFile, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        vector<string> fields = splitLine(line, '|');
        if (fields.size() < 9) {
            continue;
        }

        try {
            string type = normalizeAccountType(fields[0]);
            string accountNumber = fields[1];
            string holder = fields[2];
            string cnicValue = fields[3];
            string pinValue = fields[4];
            double accountBalance = stod(fields[5]);
            bool locked = stoi(fields[6]) != 0;
            int attempts = stoi(fields[7]);
            double withdrawnToday = stod(fields[8]);

            Account* account = nullptr;
            if (type == "Savings") {
                account = new SavingsAccount(accountNumber, holder, accountBalance, pinValue, cnicValue, withdrawnToday);
            }
            else {
                account = new CurrentAccount(accountNumber, holder, accountBalance, pinValue, cnicValue, withdrawnToday);
            }

            account->setSecurityState(locked, attempts);
            addAccount(account);
        }
        catch (...) {
            continue;
        }
    }

    accountFile.close();

    if (accountCount == 0) {
        seedDefaultAccounts();
        saveData();
        return false;
    }

    ifstream transactionFile(transactionsFilePath());
    while (transactionFile.is_open() && getline(transactionFile, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        vector<string> fields = splitLine(line, '|');
        if (fields.size() < 6) {
            continue;
        }

        int accountIndex = searchAcc(fields[0]);
        if (accountIndex == -1) {
            continue;
        }

        try {
            Transaction transaction(
                Transaction::stringToType(fields[2]),
                stod(fields[3]),
                stod(fields[4]),
                fields[1],
                fields[5]);
            accounts[accountIndex]->restoreTransaction(transaction);
        }
        catch (...) {
            continue;
        }
    }

    return true;
}

bool ATM::saveData() const {
    filesystem::create_directories(dataDirectory);

    ofstream accountFile(accountsFilePath());
    if (!accountFile.is_open()) {
        return false;
    }

    for (int i = 0; i < accountCount; i++) {
        accountFile << accounts[i]->toStorageRecord() << '\n';
    }

    ofstream transactionFile(transactionsFilePath());
    if (!transactionFile.is_open()) {
        return false;
    }

    for (int i = 0; i < accountCount; i++) {
        vector<Transaction> transactions = accounts[i]->getLastTransactions(MAX_HISTORY);
        for (const Transaction& transaction : transactions) {
            transactionFile << accounts[i]->getAccountNumber() << '|'
                << transaction.getTimestamp() << '|'
                << Transaction::typeToString(transaction.getType()) << '|'
                << transaction.getAmount() << '|'
                << transaction.getBalanceAfter() << '|'
                << transaction.getDescription() << '\n';
        }
    }

    return true;
}

bool ATM::changeCurrentPIN(string currentPIN, string newPIN, string& message) {
    if (currentAccount == nullptr) {
        message = "Please authenticate first.";
        return false;
    }

    if (currentAccount->isAccountLocked()) {
        message = LOCKED_ACCOUNT_MESSAGE;
        return false;
    }

    if (!currentAccount->matchesPIN(currentPIN)) {
        message = "Current PIN is incorrect.";
        return false;
    }

    if (newPIN.length() != PIN_LENGTH || !isDigitsOnly(newPIN)) {
        message = "New PIN must be exactly 4 numeric digits.";
        return false;
    }

    currentAccount->changePIN(newPIN);
    saveData();
    message = "PIN updated successfully.";
    return true;
}

void ATM::ejectCard() {
    cout << "[INFO] Card ejected. Goodbye!\n";
    currentAccount = nullptr;
}
