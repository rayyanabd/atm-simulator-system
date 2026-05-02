#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include "Transaction.h"
#include "Constants.h"

using namespace std;

class Account
{
protected:
    string accountNumber;
    string accountHolder;
    string pin;

    double balance;
    double dailyWithdrawalTotal;

    bool isLocked;
    int loginAttempts;

    // Transaction history
    Transaction* transactionHistory[MAX_HISTORY];
    int transactionCount;

public:
    Account();
    virtual ~Account();

    // Setters
    void setAccountNumber(string n);
    void setPin(string p);
    void setBalance(double b);
    void setDailyWithdrawalTotal(double d);

    // Getters
    string getAccountNumber() const;
    string getPin() const;
    double getBalance() const;
    double getDailyWithdrawalTotal() const;
    bool isAccountLocked() const;

    // Common functions
    bool validatePIN(string p);
    void credit(double amount);
    void changePIN(string newPIN);

    void addTransaction(TransactionType type, double amt, string desc);
    void printMiniStatement();

    // Pure virtual functions
    virtual bool debit(double amount) = 0;
    virtual string getAccountType() = 0;
};

#endif