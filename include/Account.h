#pragma once
#include <string>
#include "Transaction.h"
#include "Constants.h"

using namespace std;

class Account {
protected:
    string accountNumber;
    string accountHolder;
    string pin;

    double balance;
    bool isLocked;
    int loginAttempts;

    // Transaction History (Array of pointers for polymorphism)
    Transaction* transactionHistory[MAX_HISTORY];
    int transactionCount;

public:
    Account();
    virtual ~Account(); // Virtual destructor for memory safety

    // Common Logic
    bool validatePIN(std::string p);
    void credit(double amount);
    void addTransaction(TransactionType type, double amt, std::string desc);
    void printMiniStatement();

    // Getters
    double getBalance() const { return balance; }
    std::string getAccountNumber() const { return accountNumber; }
    void setBalance(double b) { balance = b; } // For testing phase
    bool isAccountLocked() const;
    void changePIN(std::string newPIN);

    void setAccountNumber(string num) { accountNumber = num; }
    void setPin(string p) { pin = p; }

    // Pure Virtual Functions (Must be implemented by children)
    virtual bool debit(double amount) = 0;
    virtual std::string getAccountType() = 0;
};