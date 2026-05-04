#pragma once
#include <string>
#include <vector>
#include "Transaction.h"
#include "Constants.h"

using namespace std;

class Account {
protected:
    string accountNumber;
    string accountHolder;
    string cnic;
    string pin;

    double balance;
    bool isLocked;
    int loginAttempts;
    string lastError;

    // Transaction History (Array of pointers for polymorphism)
    Transaction* transactionHistory[MAX_HISTORY];
    int transactionCount;

public:
    Account();
    Account(std::string accNum, std::string nm, double bal, std::string p, std::string cnicNo = "");
    virtual ~Account(); // Virtual destructor for memory safety

    // Common Logic
    bool validatePIN(std::string p);
    bool matchesPIN(std::string p) const;
    void credit(double amount);
    void credit(double amount, TransactionType type, std::string desc);
    void addTransaction(TransactionType type, double amt, std::string desc);
    void restoreTransaction(const Transaction& transaction);
    std::vector<Transaction> getLastTransactions(int count) const;
    void printMiniStatement();

    // Getters
    double getBalance() const { return balance; }
    std::string getAccountNumber() const { return accountNumber; }
    std::string getAccountHolder() const { return accountHolder; }
    std::string getCnic() const { return cnic; }
    void setBalance(double b) { balance = b; } // For testing phase
    bool isAccountLocked() const;
    int getLoginAttempts() const { return loginAttempts; }
    std::string getLastError() const { return lastError; }
    void setSecurityState(bool locked, int attempts);
    void changePIN(std::string newPIN);
    std::string toStorageRecord() const;
    virtual double getDailyWithdrawn() const { return 0.0; }


    // Pure Virtual Functions (Must be implemented by children)
    virtual bool debit(double amount) = 0;
    virtual bool debit(double amount, TransactionType type, const std::string& desc) = 0;
    virtual std::string getAccountType() const = 0;
};
