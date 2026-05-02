#include "../include/Account.h"
#include <iostream>
using namespace std;

Account::Account() {

     accountNumber = "UNKNOWN";
     accountHolder = "UNKNOWN";
     CNIC = "NULL";
     phone_number = "NULL";
     pin = "NULL";

    balance = 0.0;
    isLocked = false;
    loginAttempts = 0;
    transactionCount = 0;
    for (int i = 0; i < MAX_HISTORY; i++) transactionHistory[i] = nullptr;
}

Account::~Account() {
    for (int i = 0; i < transactionCount; i++) {
        delete transactionHistory[i];
    }
}

void Account::setdata(string accno, string name, string cnic, string phoneno, double b, string pin)
{
    accountHolder = name;
    CNIC = cnic;
    phone_number = phoneno;
    accountNumber = accno;
    balance = b;
    this->pin = pin;
}
bool Account::isAccountLocked() const {
    return isLocked;
}

bool Account::validatePIN(string p) {
    if (pin == p) {
        loginAttempts = 0; // Reset counter on success
        return true;
    }
    loginAttempts++;
    if (loginAttempts >= 3) {
        isLocked = true;
    }
    return false;
}

void Account::credit(double amount) {
    if (amount > 0) {
        balance += amount;
        addTransaction(TransactionType::DEPOSIT, amount, "Cash Deposit");
    }
}

void Account::addTransaction(TransactionType type, double amt, string desc) {
    if (transactionCount < MAX_HISTORY) {
        transactionHistory[transactionCount++] = new Transaction(type, amt, balance, desc);
    }
}

void Account::printMiniStatement() {
    cout << "\n--- Mini Statement for Account: " << accountNumber << " ---\n";
    if (transactionCount == 0) cout << "No recent transactions.\n";
    for (int i = 0; i < transactionCount; i++) {
        transactionHistory[i]->printTransaction();
    }
    cout << "Current Balance: Rs. " << balance << "\n";
}

void Account::changePIN(string newPIN) {
    pin = newPIN;
    // Log PIN change 
    addTransaction(TransactionType::DEPOSIT, 0, "PIN Updated");
}