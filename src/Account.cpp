#include "Account.h"
#include <iostream>

using namespace std;

Account::Account()
{
    accountNumber = "";
    accountHolder = "";
    pin = "";
    balance = 0.0;
    dailyWithdrawalTotal = 0.0;

    isLocked = false;
    loginAttempts = 0;
    transactionCount = 0;

    for (int i = 0; i < MAX_HISTORY; i++)
    {
        transactionHistory[i] = nullptr;
    }
}

Account::~Account()
{
    for (int i = 0; i < transactionCount; i++)
    {
        delete transactionHistory[i];
    }
}

// Setters
void Account::setAccountNumber(string n)
{
    accountNumber = n;
}

void Account::setPin(string p)
{
    pin = p;
}

void Account::setBalance(double b)
{
    balance = b;
}

void Account::setDailyWithdrawalTotal(double d)
{
    dailyWithdrawalTotal = d;
}

// Getters
string Account::getAccountNumber() const
{
    return accountNumber;
}

string Account::getPin() const
{
    return pin;
}

double Account::getBalance() const
{
    return balance;
}

double Account::getDailyWithdrawalTotal() const
{
    return dailyWithdrawalTotal;
}

bool Account::isAccountLocked() const
{
    return isLocked;
}

// Common functions
bool Account::validatePIN(string p)
{
    if (pin == p)
    {
        loginAttempts = 0;
        return true;
    }

    loginAttempts++;

    if (loginAttempts >= 3)
    {
        isLocked = true;
    }

    return false;
}

void Account::credit(double amount)
{
    if (amount > 0)
    {
        balance += amount;
        addTransaction(TransactionType::DEPOSIT, amount, "Cash Deposit");
    }
}

void Account::addTransaction(TransactionType type, double amt, string desc)
{
    if (transactionCount < MAX_HISTORY)
    {
        transactionHistory[transactionCount++] =
            new Transaction(type, amt, balance, desc);
    }
}

void Account::printMiniStatement()
{
    cout << "\n--- Mini Statement for Account: " << accountNumber << " ---\n";

    if (transactionCount == 0)
    {
        cout << "No recent transactions.\n";
    }

    for (int i = 0; i < transactionCount; i++)
    {
        transactionHistory[i]->printTransaction();
    }

    cout << "Current Balance: Rs. " << balance << "\n";
}

void Account::changePIN(string newPIN)
{
    pin = newPIN;
    addTransaction(TransactionType::DEPOSIT, 0, "PIN Updated");
}