#include "Account.h"
#include <iostream>
using namespace std;

Account::Account() {

    accountNumber = "UNKNOWN";
    accountHolder = "UNKNOWN";
    CNIC = "NULL";
    phone_number = "NULL";
    pin = "NULL";
    creationDate = "";
    lastInterestDate = "";

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

void Account::setdata(string accno, string name, string cnic, string phoneno, double b, string pin, bool lock)
{
    accountHolder = name;
    CNIC = cnic;
    phone_number = phoneno;
    accountNumber = accno;
    balance = b;
    this->pin = pin;
    isLocked = lock;
}
void Account::setdata(string accno, string name, string cnic, string phoneno, double b, string pin, bool lock, string created, string lastInterest)
{
    setdata(accno, name, cnic, phoneno, b, pin, lock);
    creationDate = created;
    lastInterestDate = lastInterest;
}
string Account::getname()
{
    return accountHolder;
}
string Account::getCNIC()
{
    return CNIC;
}
string Account::getPIN()
{
    return pin;
}
string Account::getPhone()
{
    return phone_number;
}
string Account::getCreationDate()
{
    return creationDate;
}
string Account::getLastInterestDate()
{
    return lastInterestDate;
}
void Account::setCreationDate(string date)
{
    creationDate = date;
}
void Account::setLastInterestDate(string date)
{
    lastInterestDate = date;
}
void Account::setLock(bool status)
{
    isLocked = status;
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

    int insertIndex = transactionCount;
    if (transactionCount >= MAX_HISTORY) {
        delete transactionHistory[0];
        for (int i = 1; i < MAX_HISTORY; i++) {
            transactionHistory[i - 1] = transactionHistory[i];
        }
        insertIndex = MAX_HISTORY - 1;
        transactionHistory[insertIndex] = nullptr;
    }

    transactionHistory[insertIndex] =
        new Transaction(type, amt, balance, desc);
    transactionHistory[insertIndex]->saveToFile(accountNumber);
    if (transactionCount < MAX_HISTORY) {
        transactionCount++;
    }
}
void Account::addTransaction(TransactionType type, double amt, std::string desc, std::string time)
{
    int insertIndex = transactionCount;
    if (transactionCount >= MAX_HISTORY) {
        delete transactionHistory[0];
        for (int i = 1; i < MAX_HISTORY; i++) {
            transactionHistory[i - 1] = transactionHistory[i];
        }
        insertIndex = MAX_HISTORY - 1;
        transactionHistory[insertIndex] = nullptr;
    }

    transactionHistory[insertIndex] =
        new Transaction(type, amt, balance, desc, time);

    if (transactionCount < MAX_HISTORY) {
        transactionCount++;
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
}
