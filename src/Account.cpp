#include "../include/Account.h"
#include <cmath>
#include <iostream>
#include <sstream>

using namespace std;

Account::Account() {
    balance = 0.0;
    isLocked = false;
    loginAttempts = 0;
    lastError.clear();
    transactionCount = 0;
    for (int i = 0; i < MAX_HISTORY; i++) transactionHistory[i] = nullptr;
}

Account::Account(string accNum, string nm, double bal, string p, string cnicNo) {
    accountNumber = accNum;
    accountHolder = nm;
    cnic = cnicNo;
    pin = p;
    balance = bal;
    isLocked = false;
    loginAttempts = 0;
    lastError.clear();
    transactionCount = 0;
    for (int i = 0; i < MAX_HISTORY; i++) transactionHistory[i] = nullptr;
}

Account::~Account() {
    for (int i = 0; i < transactionCount; i++) {
        delete transactionHistory[i];
    }
}

bool Account::isAccountLocked() const {
    return isLocked;
}

bool Account::validatePIN(string p) {
    if (pin == p) {
        loginAttempts = 0; // Reset counter on success
        lastError.clear();
        return true;
    }
    loginAttempts++;
    if (loginAttempts >= 3) {
        isLocked = true;
        lastError = LOCKED_ACCOUNT_MESSAGE;
    }
    else {
        lastError = "Incorrect PIN.";
    }
    return false;
}

bool Account::matchesPIN(string p) const {
    return pin == p;
}

void Account::credit(double amount) {
    credit(amount, TransactionType::DEPOSIT, "Cash Deposit");
}

void Account::credit(double amount, TransactionType type, string desc) {
    if (std::isfinite(amount) && amount > 0) {
        balance += amount;
        addTransaction(type, amount, desc);
        lastError.clear();
    }
    else {
        lastError = "Amount must be greater than zero.";
    }
}

void Account::addTransaction(TransactionType type, double amt, string desc) {
    if (transactionCount == MAX_HISTORY) {
        delete transactionHistory[0];
        for (int i = 1; i < MAX_HISTORY; i++) {
            transactionHistory[i - 1] = transactionHistory[i];
        }
        transactionHistory[MAX_HISTORY - 1] = nullptr;
        transactionCount--;
    }

    if (transactionCount < MAX_HISTORY) {
        transactionHistory[transactionCount++] = new Transaction(type, amt, balance, desc);
    }
}

void Account::restoreTransaction(const Transaction& transaction) {
    if (transactionCount == MAX_HISTORY) {
        delete transactionHistory[0];
        for (int i = 1; i < MAX_HISTORY; i++) {
            transactionHistory[i - 1] = transactionHistory[i];
        }
        transactionHistory[MAX_HISTORY - 1] = nullptr;
        transactionCount--;
    }

    transactionHistory[transactionCount++] = new Transaction(transaction);
}

vector<Transaction> Account::getLastTransactions(int count) const {
    vector<Transaction> transactions;
    if (count <= 0 || transactionCount == 0) {
        return transactions;
    }

    int start = transactionCount - count;
    if (start < 0) {
        start = 0;
    }

    for (int i = start; i < transactionCount; i++) {
        transactions.push_back(*transactionHistory[i]);
    }
    return transactions;
}

void Account::printMiniStatement() {
    cout << "\n--- Mini Statement for Account: " << accountNumber << " ---\n";
    if (transactionCount == 0) cout << "No recent transactions.\n";
    vector<Transaction> recentTransactions = getLastTransactions(MINI_STATEMENT_COUNT);
    for (const Transaction& transaction : recentTransactions) {
        transaction.printTransaction();
    }
    cout << "Current Balance: Rs. " << balance << "\n";
}

void Account::setSecurityState(bool locked, int attempts) {
    isLocked = locked;
    loginAttempts = attempts;
}

void Account::changePIN(string newPIN) {
    pin = newPIN;
    // Log PIN change 
    addTransaction(TransactionType::PIN_CHANGE, 0, "PIN Updated");
    lastError.clear();
}

string Account::toStorageRecord() const {
    ostringstream out;
    out << getAccountType() << '|'
        << accountNumber << '|'
        << accountHolder << '|'
        << cnic << '|'
        << pin << '|'
        << balance << '|'
        << (isLocked ? 1 : 0) << '|'
        << loginAttempts << '|'
        << getDailyWithdrawn();
    return out.str();
}
