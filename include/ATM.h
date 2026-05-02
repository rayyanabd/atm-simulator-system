#ifndef ATM_H
#define ATM_H

#include <iostream>
#include <string>
#include "Account.h"
#include "Constants.h"

using namespace std;

class ATM
{
private:
    Account* accounts[MAX_ACCOUNTS];
    int totalAccounts;
    int lastAccNumber;

    Account* currentAccount;
    double cashAvailable;

    int findAccount(string accNum);
    bool isValidPin(string p);
    void showMainMenu();

public:
    ATM(double initialCash = 0.0);
    ~ATM();

    // Account management
    void addAccount(Account* account);
    int getTotalAccounts() const { return totalAccounts; }

    // User authentication
    bool insertCard(string accNum);
    bool enterPIN(string pin);
    void ejectCard();

    // User operations
    void deposit();
    void withdraw(double amount);
    void checkBalance();
    void changePIN();
    void fastCash();
    void miniStatement();

    // Admin operations
    void loadWithdrawals();
    void saveWithdrawals();
    void addNewAccount();
    void adminPortal();

    // System start
    void start();
};

#endif